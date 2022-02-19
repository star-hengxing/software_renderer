#pragma once

#include <Hinae/Point4.hpp>
#include <Hinae/Point2.hpp>
#include <Hinae/Vector3.hpp>

using namespace Hinae;

bool clip_spacae_culling(const Point4f& a, const Point4f& b, const Point4f& c, f32 z_near);

bool back_face_culling(const Vector3f& a, const Vector3f& b, const Vector3f& c);

struct Cohen_Sutherland
{
private:
    static constexpr u8 INSIDE = 0; // 0000
    static constexpr u8 LEFT   = 1; // 0001
    static constexpr u8 RIGHT  = 2; // 0010
    static constexpr u8 BOTTOM = 4; // 0100
    static constexpr u8 TOP    = 8; // 1000
    
    using Code = u8;

    const usize width, height;

public:
    Cohen_Sutherland(usize width, usize height) : width(width), height(height) {}

    template <arithmetic T>
    Code which_area(const Point2<T>& p) const
    {
        u8 code = INSIDE;

        if(p.x < 0)
            code |= LEFT;
        else if(p.x > width)
            code |= RIGHT;

        if(p.y < 0)
            code |= TOP;
        else if(p.y > height)
            code |= BOTTOM;
        
        return code; 
    }
    
    template <arithmetic T>
    Point2<T> find_intersection(u8 code, const Point2<T>& p1, const Point2<T>& p2) const
    {
        T x, y;
        const auto& [x1, y1] = p1;
        const auto& [x2, y2] = p2;

        if(code & TOP)
        {
            x = x1 - y1 * (x2 - x1) / (y2 - y1);
            y = 0;
        }
        else if(code & BOTTOM)
        {
            x = x1 + (x2 - x1) * (height - y1) / (y2 - y1);
            y = height;
        }
        else if(code & LEFT)
        {
            y = y1 - x1 * (y2 - y1) / (x2 -x1);
            x = 0;
        }
        else if(code & RIGHT)
        {
            y = y1 + (y2 - y1) * (width - x1) / (x2 - x1);
            x = width;
        }

        return
        {
            clamp(ZERO<T>, x, static_cast<T>(width)),
            clamp(ZERO<T>, y, static_cast<T>(height))
        };
    }
};
