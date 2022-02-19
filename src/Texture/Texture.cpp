#include "Texture.hpp"

#include <util/util.hpp>

Colorf get_color(const Image& texture, const Point2f& uv, State state)
{
    // f32 u = clamp(ZERO<f32>, uv.x, ONE<f32>);
    // f32 v = clamp(ZERO<f32>, uv.y, ONE<f32>);
    f32 u = min(std::abs(uv.x), 1.0f);
    f32 v = min(std::abs(uv.y), 1.0f);

    const auto [width, height] = texture.get_width_height();
    u *= static_cast<f32>(width - 1);
    v  = static_cast<f32>(height - 1) * (1.0f - v); // flip y axis

    if(state == State::Standard)
    {
        return rgb_to_float(texture[static_cast<usize>(u)][static_cast<usize>(v)]);
    }
    else if(state == State::Bilinear)
    {
        const auto u_min = static_cast<usize>(std::floor(u));
        const auto u_max = static_cast<usize>(std::ceil(u));
        const auto v_min = static_cast<usize>(std::floor(v));
        const auto v_max = static_cast<usize>(std::ceil(v));

        const Colorf p1 = rgb_to_float(texture[u_min][v_max]);
        const Colorf p2 = rgb_to_float(texture[u_max][v_max]);
        const Colorf p3 = rgb_to_float(texture[u_min][v_min]);
        const Colorf p4 = rgb_to_float(texture[u_max][v_min]);

        const f32 s = u - u_min;
        const f32 t = v - v_min;

        const Colorf u0 = lerp(p1, p2, s);
        const Colorf u1 = lerp(p3, p4, s);

        return lerp(u0, u1, t);
    }
    return {};
}
