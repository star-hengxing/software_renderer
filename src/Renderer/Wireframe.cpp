#include "Wireframe.hpp"

#include <algorithm>

#include <Hinae/Transform.hpp>

#include <Clip/Clip.hpp>
#include <util/util.hpp>

void Wireframe::set_line_color(const Color& color)
{
    line_color = color;
}

void Wireframe::set_vertex_index(const std::vector<Vertex_index>& v)
{
    vertex_index.insert(vertex_index.end(), v.begin(), v.end());
}

void Wireframe::bresenham(isize x1, isize y1, isize x2, isize y2, const Color& color)
{
    isize x, y;

    if(x1 != x2 && y1 != y2)
    {
        isize epsilon, inc;
        isize dx, dy, ddy, ddx;
        dx = std::abs(x2 - x1);
        dy = std::abs(y2 - y1);

        ddy = 2 * dy;
        ddx = 2 * dx;

        if(dx >= dy)
        {
            epsilon = ddy - dx;
            if(x2 < x1)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            inc = (y2 >= y1 ? 1 : -1);
            y = y1;
            for(x = x1; x <= x2; x++)
            {
                set_pixel(x, y, color);
                if(epsilon < 0)
                {
                    epsilon += ddy;
                }
                else
                {
                    y += inc;
                    epsilon = epsilon + ddy - ddx;
                }
            }
        }
        else
        {
            epsilon = ddx - dy;
            if(y2 < y1)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            inc = (x2 >= x1 ? 1 : -1);
            x = x1;
            for(y = y1; y <= y2; y++)
            {
                set_pixel(x, y, color);
                if(epsilon < 0)
                {
                    epsilon += ddx;
                }
                else
                {
                    x += inc;
                    epsilon = epsilon + ddx - ddy;
                }
            }
        }
    }
    else if(x1 == x2)
    {
        if(y1 > y2) std::swap(y1, y2);
        for(y = y1; y <= y2; y++) set_pixel(x1, y, color);
    }
    else if(y1 == y2)
    {
        if(x1 > x2) std::swap(x1, x2);
        for(x = x1; x <= x2; x++) set_pixel(x, y1, color);
    }
    else
    {
        set_pixel(x1, y1, color);
    }
}

void Wireframe::draw_line(const Point2i& begin, const Point2i& end)
{
    /*
    * 1.Ax + By + C = 0
    * 2.y = kx + b
    *
    * k = y2 - y1 / x2 - x1 = dy / dx
    *
    * k to 2.
    * y = dy / dx * x + b
    *
    * transform to 1.
    * (dy)x - (dx)y +(dx)b
    *
    * so
    * A = dy B = -dx C = (dx)b
    */
    bresenham(begin.x, begin.y, end.x, end.y, line_color);
}

void Wireframe::draw()
{
    const Matrix4f mvp = projection * view * model;
    
    map(vertex, clip_vertics, [&](const Point3f& p) -> Point4f { return mvp * Point4f{p}; });

    for(const auto& indexes : vertex_index)
    {
        const std::array<Point4f, 3> clip
        {
            clip_vertics[indexes.a],
            clip_vertics[indexes.b],
            clip_vertics[indexes.c],
        };

        if(!clip_spacae_culling(clip[0], clip[1], clip[2], camera.z_near)) continue;

        std::array<Point2i, 3> screen;
        map(clip, screen, [&](const Point4f& p) -> Point2i
        {
            return
            {
                static_cast<isize>(((1.0f + p.x / p.w) * width  / 2) + 0.5f),
                static_cast<isize>(((1.0f - p.y / p.w) * height / 2) + 0.5f)
            };
        });

        draw_line(screen[0], screen[1]);
        draw_line(screen[1], screen[2]);
        draw_line(screen[2], screen[0]);

        // std::array<Point2f, 3> screenf;
        // map(clip, screenf, [&](const Point4f& p) -> Point2f
        // {
        //     return
        //     {
        //         (1.0f + p.x / p.w) * (width  / 2),
        //         (1.0f - p.y / p.w) * (height / 2)
        //     };
        // });
        
        // for(usize i : range(screenf.size()))
        // {
        //     const Point2f& current = screenf[i];
        //     const Point2f& next = screenf[(i + screenf.size() - 1) % screenf.size()];

        //     Cohen_Sutherland clipping(width - 1, height - 1);
        //     const u8 code1 = clipping.which_area(current);
        //     const u8 code2 = clipping.which_area(next);

        //     Point2i p1, p2;
        //     if(!(code1 | code2))
        //     {
        //        p1 = static_cast<Point2i>(current);
        //        p2 = static_cast<Point2i>(next);
        //     }
        //     // else if(code1 & code2)
        //     // {
        //     //     continue;
        //     // }
        //     else
        //     {
        //         p1 = static_cast<Point2i>(code1 ? clipping.find_intersection(code1, current, next) : current);
        //         p2 = static_cast<Point2i>(code2 ? clipping.find_intersection(code2, current, next) : next);
        //     }
        //     draw_line(p1, p2);
        // }
    }
}
