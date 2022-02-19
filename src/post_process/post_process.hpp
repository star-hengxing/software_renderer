#pragma once

#include <util/Buffer2D.hpp>
#include <Image/Color.hpp>

NAMESPACE_BEGIN(PostProcess)

Buffer2D<Color> SSAA(usize size, const Buffer2D<Color>& frame);

NAMESPACE_END(PostProcess)
