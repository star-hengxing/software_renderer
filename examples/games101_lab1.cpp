#include <Renderer/Wireframe.hpp>
#include <window/Warp_SDL2.hpp>

using namespace Hinae;

constexpr usize width = 700, height = 700;

int main()
{
    auto camera = Camera{}
        .set_pos(Point3f{0, 0, 5})
        .set_at(Point3f{0, 0, 0})
        .set_up(Vector3f{0, 1, 0})
        .set_aspect(width / height)
        .set_fov(45)
        .set_frustum(-0.1, -50);

    Wireframe renderer{width, height, camera};

    std::vector<Point3f> vertex
    {
        {2, 0, -2}, {0, 2, -2}, {-2, 0, -2}
    };

    renderer.set_vertex(vertex);
    renderer.set_vertex_index({{0, 1, 2}});
    renderer.draw();
    
    Warp_SDL2 window;
    window.init(width, height, "games101_lab1")
        .write(renderer.raw_frame())
        .run();
}