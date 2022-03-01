#include "post_process.hpp"

#include <Hinae/Vector3.hpp>

using namespace Hinae;

#include <util/util.hpp>

Buffer2D<Color> SSAA(usize size, const Buffer2D<Color>& frame)
{
    const auto [width, height] = frame.get_width_height();
    Buffer2D<Color> buffer{width, height};
    for(usize y : range(height))
    {
        for(usize x : range(width))
        {
            Vector3f color{0};
            for(usize j : range(size))
            {
                for(usize i : range(size))
                {
                    color += rgb_to_float<f32>(frame[x * size + i][y * size + j]);
                }
            }
            buffer[x][y] = float_to_rgb(color / static_cast<f32>(size * size));
        }
    }
    return buffer;
}
