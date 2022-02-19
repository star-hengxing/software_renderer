#pragma once

#include <optional>

#include <Hinae/Vector3.hpp>
#include <Hinae/Point3.hpp>
#include <Hinae/Point2.hpp>

#include <Image/Image.hpp>

using namespace Hinae;

struct Payload
{
    Point3f view_point;
    Vector3f normal;
    Color color;
    Point2f uv;

    std::optional<Image> texture;

    Color pixel_color;
};

using shader_fn = void (*)(Payload& payload);

struct Shader
{
    shader_fn shader = nullptr;

    void operator () (Payload& payload)
    {
        shader(payload);
    }
};

void point_color_shader(Payload& payload);

void normal_shader(Payload& payload);

void blinn_phong_shader(Payload& payload);

void bump_shader(Payload& payload);

void displacement_shader(Payload& payload);
