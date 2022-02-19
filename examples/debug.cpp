#include <Hinae/Transform.hpp>

using namespace Hinae;

#include <Renderer/Rasterizer.hpp>
#include <window/Warp_SDL2.hpp>
#include <util/util.hpp>
#include <Mesh/Mesh.hpp>

constexpr usize width = 700, height = 700;

auto camera = Camera{}
    .set_pos(Point3f{0, 0, 3})
    .set_at(Point3f{0, 0, 0})
    .set_up(Vector3f{0, 1, 0})
    .set_aspect(width / height)
    .set_fov(45)
    .set_frustum(-0.1f, -100.0f);

Rasterizer renderer{width, height, camera};

std::optional<Mesh> mesh = Mesh::read(
    "models/spot/spot_triangulated_good.obj",
    // "models/spot/spot_texture.png");
    "models/spot/hmap.jpg");

int main(int argc, char const *argv[])
{
    if(!mesh) return -1;
    mesh->info();

    renderer.set_vertex(mesh->vertices);
    renderer.set_model(Transform<f32>::rotate<Axis::Y>(140));

    renderer.set_face(mesh->faces)
        .set_vertex_normal(mesh->normals)
        .set_texture(std::move(mesh->texture.value()))
        .set_uv(mesh->uvs)
        .set_shader(bump_shader);

    Timer timer;
    renderer.draw();
    timer.elapsed();

    Warp_SDL2 window;
    window.init(width, height, "star");
    window.write(renderer.raw_frame())
        .run();
}
