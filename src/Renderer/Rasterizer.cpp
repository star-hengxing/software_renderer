#include "Rasterizer.hpp"

#include <algorithm>

#include <Hinae/Transform.hpp>

#include <util/util.hpp>
#include <Clip/Clip.hpp>

Rasterizer::Rasterizer(usize width, usize height, Camera& camera) :
    Renderer(width, height, camera)
    , depth_buffer{width, height}
{
    z_scale     = (camera.z_far - camera.z_near) / 2;
    z_translate = (camera.z_far + camera.z_near) / 2;
    clean_depth_buffer();
}

Rasterizer& Rasterizer::set_face(const std::vector<std::array<Face, 3>>& v)
{
    faces.insert(faces.end(), v.begin(), v.end());
    return *this;
}

Rasterizer& Rasterizer::set_vertex_color(const std::vector<Vector3f>& v)
{
    vertex_color.insert(vertex_color.end(), v.begin(), v.end());
    return *this;
}

Rasterizer& Rasterizer::set_vertex_normal(const std::vector<Vector3f>& v)
{
    vertex_normal.insert(vertex_normal.end(), v.begin(), v.end());
    return *this;
}

Rasterizer& Rasterizer::set_uv(const std::vector<Point2f>& v)
{
    texture_uv.insert(texture_uv.end(), v.begin(), v.end());
    return *this;
}

Rasterizer& Rasterizer::set_shader(const shader_fn& fs)
{
    shader.shader = fs;
    return *this;
}

Rasterizer& Rasterizer::set_texture(Image&& texture)
{
    payload.texture = std::move(texture);
    return *this;
}

void Rasterizer::clean_depth_buffer()
{
    depth_buffer.fill(-INFINITY_<f32>);
}

static std::tuple<usize, usize, usize, usize>
bounding_box(const Trianglef& t)
{
    return
    {
        static_cast<usize>(std::floor(min(t.A.x, t.B.x, t.C.x))),
        static_cast<usize>(std::ceil (max(t.A.x, t.B.x, t.C.x))),
        static_cast<usize>(std::floor(min(t.A.y, t.B.y, t.C.y))),
        static_cast<usize>(std::ceil (max(t.A.y, t.B.y, t.C.y)))
    };
}

void Rasterizer::draw()
{
    const Matrix4f mv     = view * model;
    const Matrix4f mvp    = projection * mv;
    const Matrix4f inv_mv = mv.adjugate().transpose();

    map(vertex, clip_vertics, [&](const Point3f& p) -> Point4f { return mvp * Point4f{p}; });

    for(const auto& face : faces)
    {
        std::array<Point4f, 3> clip
        {
            clip_vertics[face[0].point],
            clip_vertics[face[1].point],
            clip_vertics[face[2].point],
        };
        
        if(!clip_spacae_culling(clip[0], clip[1], clip[2], camera.z_near))
            continue;
        if(back_face_culling(cast<Vector3>(clip[0]), cast<Vector3>(clip[1]), cast<Vector3>(clip[2])))
            continue;

        for(auto& p : clip)
        {
            const f32 inv_w = reciprocal(p.w);
            p.x = (1.0f + p.x * inv_w) * width  / 2;
            p.y = (1.0f - p.y * inv_w) * height / 2;
            p.z = p.z * inv_w * z_scale + z_translate;
        }

        Triangle_payload tp;
        for(const auto&& [i, value] : Enumerate(clip))
        {
            tp[i].p      = value;
            tp[i].n      = inv_mv * vertex_normal[face[i].normal].normalized();
            tp[i].view_p = cast<Point3>(mv * Point4f{vertex[face[i].point]});
            if(payload.texture)
            {
                tp[i].uv = texture_uv[face[i].uv];
            }
        }

        draw_triangle(tp);
    }
}

void Rasterizer::draw_triangle(const Triangle_payload& tp)
{
    const Triangle t{cast<Point3>(tp.A.p), cast<Point3>(tp.B.p), cast<Point3>(tp.C.p)};
    const auto [min_x, max_x, min_y, max_y] = bounding_box(t);

    for(usize y : range(min_y, max_y))
    {
        for(usize x : range(min_x, max_x))
        {
            // const Point2f pixel = Point2f(x, y) + Vector2f{0.5};
            const Point2f pixel{static_cast<f32>(x), static_cast<f32>(y)};

            Barycentric_Coordinates bary = t.barycentric2D(pixel);
            if(!bary.is_inside()) continue;
            
            const f32 Z = bary.correct(tp.A.p.w, tp.B.p.w, tp.C.p.w);
            // const f32 Z = 1.0f;
            const f32 pixel_depth = bary.interpolated(t.A.z, t.B.z, t.C.z, Z);
            
            if(pixel_depth <= depth_buffer[x][y]) continue;
            depth_buffer[x][y] = pixel_depth;

            // payload.color = bary.interpolated(tp.A.c, tp.B.c, tp.C.c, Z);
            payload.normal = bary.interpolated(tp.A.n, tp.B.n, tp.C.n, Z);
            payload.view_point = bary.interpolated(tp.A.view_p, tp.B.view_p, tp.C.view_p, Z);
            if(payload.texture)
            {
                payload.uv = bary.interpolated(tp.A.uv, tp.B.uv, tp.C.uv, Z);
            }

            shader(payload);
            set_pixel(x, y, payload.pixel_color);
        }
    }
}

