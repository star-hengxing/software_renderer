#pragma once

#include <array>

#include <Hinae/Vector3.hpp>
#include <Hinae/Point4.hpp>

#include <Shader/Shader.hpp>
#include <Mesh/Mesh.hpp>

#include "Renderer.hpp"

struct Triangle_payload
{
    struct point
    {
        Point4f  p;      // position
        Vector3f n;      // normal
        Vector3f c;      // color
        Point2f  uv;     // texture uv position
        Point3f  view_p; // view space position
    };
    
    point A, B, C;
    
    point& operator [] (usize i)
    { 
        assert(i <= 2);
        return (&A)[i];
    }
};

class Rasterizer : public Renderer
{
private:
    Buffer2D<f32> depth_buffer;
    f32 z_scale, z_translate;

    std::vector<Vector3f> vertex_color;
    std::vector<Vector3f> vertex_normal;
    std::vector<Point2f> texture_uv;
    std::vector<std::array<Face, 3>> faces;
    
    Payload payload;
    Shader shader;

public:
    Rasterizer() = default;

    Rasterizer(usize width, usize height, Camera& camera);
    
    Rasterizer& set_face(const std::vector<std::array<Face, 3>>& v);

    Rasterizer& set_vertex_color(const std::vector<Vector3f>& v);

    Rasterizer& set_vertex_normal(const std::vector<Vector3f>& v);

    Rasterizer& set_uv(const std::vector<Point2f>& v);

    Rasterizer& set_texture(Image&& texture);

    Rasterizer& set_shader(const shader_fn& fs);

    void clean_depth_buffer();

    void draw();

    void draw_triangle(const Triangle_payload& tp);
};
