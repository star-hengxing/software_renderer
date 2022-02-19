#include "Renderer.hpp"

#include <Image/Image.hpp>

#include <Hinae/Transform.hpp>

using namespace Hinae;

Renderer::Renderer(usize width, usize height, Camera& camera)
    : frame_buffer{width, height}
    , width(width)
    , height(height)
    , camera(camera)
{
    clean_frame_buffer();
    
    set_model(Matrix4f::identity());
    set_view(camera.look_at());
    set_projection(camera.perspective());
}

void Renderer::set_model(const Matrix4f& m) { model = m; }

void Renderer::set_view(const Matrix4f& m) { view = m; }

void Renderer::set_projection(const Matrix4f& m) { projection = m; }

void Renderer::set_pixel(usize x, usize y, const Color& color)
{
    if(x < width && y < height)
    {
        frame_buffer[x][y] = color;
    }
    // frame_buffer[x][y] = color;
}

void Renderer::set_vertex(const std::vector<Point3f>& v)
{
    vertex.insert(vertex.end(), v.begin(), v.end());
    clip_vertics.resize(vertex.size());
}

void Renderer::clean_frame_buffer() { frame_buffer.fill(Color::black()); }

const u8* Renderer::raw_frame() const { return &(frame_buffer.raw()->r); }

void Renderer::screenshot(std::string_view filename) const
{
    Image::write(filename, Image::Type::JPG, raw_frame(), width, height);
}
