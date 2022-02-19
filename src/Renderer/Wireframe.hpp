#pragma once

#include "Renderer.hpp"

class Wireframe : public Renderer
{
private:
    Color line_color = Color::white();
    std::vector<Vertex_index> vertex_index;

    void bresenham(isize x1, isize y1, isize x2, isize y2, const Color& color);

public:
    using Renderer::Renderer;

    void set_line_color(const Color& color);

    void set_vertex_index(const std::vector<Vertex_index>& v);

    void draw_line(const Point2i& begin, const Point2i& end);

    void draw();
};
