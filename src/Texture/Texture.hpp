#pragma once

#include <Image/Image.hpp>

#include <Hinae/Vector3.hpp>
#include <Hinae/Point2.hpp>

using namespace Hinae;

using Colorf = Vector3f;

enum class State
{
    Standard,
    Bilinear
};

Colorf get_color(const Image& texture, const Point2f& uv, State state = State::Standard);
