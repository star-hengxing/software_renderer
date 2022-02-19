#include <Rasterizer/Renderer.hpp>
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

    Renderer rasterizer{width, height, camera};
    std::vector<Point3f> vertex
    {
        {2, 0, -2}, {0, 2, -2}, {-2, 0, -2},
        {3.5, -1, -5}, {2.5, 1.5, -5}, {-1, 0.5, -5}
    };

    rasterizer.set_vertex(vertex);
    // rasterizer.set_vertex_index({{0, 1, 2}, {3, 4, 5}});

    constexpr Colorf yellow{1, 1, 0};
    constexpr Colorf cyan{0, 1, 1};
    constexpr Colorf magenta{1, 0, 1};
    rasterizer.set_vertex_color({
        cyan, yellow, magenta,
        Colorf{1, 0, 0}, Colorf{0, 1, 0}, Colorf{0, 0, 1}
    }).set_shader(point_color_shader);

    rasterizer.draw();
    
    Warp_SDL2 window;
    window.init(width, height, "games101_lab2")
        .write(rasterizer.raw_frame())
        .run();
}