#include <Hinae/Transform.hpp>

using namespace Hinae;

#include <Renderer/Rasterizer.hpp>
#include <window/Warp_SDL2.hpp>
#include <util/util.hpp>
#include <Mesh/Mesh.hpp>

constexpr usize width = 700, height = 700;

int main(int argc, char const *argv[])
{
    Point3f pos{0, -40, 40};
    auto camera = Camera{}
        .set_pos(pos)
        .set_at(Point3f{0, 0, 0})
        .set_up(Vector3f{0, 1, 0})
        .set_aspect(width / height)
        .set_fov(45)
        .set_frustum(-0.1f, -100.0f);

    Rasterizer renderer{width, height, camera};

    std::optional<Mesh> mesh = Mesh::read("models/bunny/bunny.obj");

    if(!mesh) return -1;

    mesh->info();

    renderer.set_vertex(mesh->vertices);

    renderer.set_face(mesh->faces)
        .set_vertex_normal(mesh->normals)
        .set_shader(blinn_phong_shader);

    Warp_SDL2 window;
    window.init(width, height, "bunny");
    
    i32 key = 0;
    f32 degree = 30;
    f32 scale = 1;

    bool run = true;
    while(run)
    {
        key = window.delay_key(0);
        switch(key)
        {
            case 'j': degree -= 1.0f; break;
            case 'k': degree += 1.0f; break;

            case 'w': pos.z  -= 0.1f; break;
            case 's': pos.z  += 0.1f; break;
            
            case 'z': scale  += 0.01f; break;
            case 'x': scale  -= 0.01f; break;

            case 27:
            case 'q':
                run = false;
                break;
            default:
                break;
        }
        camera.set_pos(pos);
        renderer.set_view(camera.look_at());
        renderer.set_model(Transform<f32>::scale(scale) * Transform<f32>::rotate<Axis::Y>(degree));
        renderer.draw();
        window.write(renderer.raw_frame());
        renderer.clean_frame_buffer();
        renderer.clean_depth_buffer();
        FPS();
    }
}