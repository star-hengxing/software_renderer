#pragma once

#include <vector>

#include <Hinae/Triangle.hpp>
#include <Hinae/Matrix4.hpp>
#include <Hinae/Point4.hpp>

#include <Camera/Camera.hpp>
#include <util/Buffer2D.hpp>
#include <Image/Color.hpp>

using namespace Hinae;

struct Vertex_index
{
    usize a, b, c;
    
    usize operator [] (usize i) const
    { 
        assert(i <= 2);
        return (&a)[i];
    }
};

class Renderer
{
protected:
    Buffer2D<Color> frame_buffer;
    usize width, height;
    Camera& camera;
    Matrix4f model;
    Matrix4f view;
    Matrix4f projection;

    std::vector<Point3f> vertex;
    std::vector<Point4f> clip_vertics;

public:
    Renderer() = delete;

    Renderer(usize width, usize height, Camera& camera);

    void set_model(const Matrix4f& m);

    void set_view(const Matrix4f& m);

    void set_projection(const Matrix4f& m);

    void set_pixel(usize x, usize y, const Color& color);

    void set_vertex(const std::vector<Point3f>& v);

    void clean_frame_buffer();

    void screenshot(std::string_view filename = "output") const;
    
    const u8* raw_frame() const;

    Color operator () (usize x, usize y) const { return frame_buffer[x][y]; }
};
