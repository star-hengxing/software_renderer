#pragma once

#include <Hinae/Matrix4.hpp>
#include <Hinae/Vector3.hpp>
#include <Hinae/Point3.hpp>

using namespace Hinae;

struct Camera
{
    f32 fov, aspect;
    f32 z_near, z_far;
    Point3f pos, at;
    Vector3f up;

    Camera() = default;

    Camera& set_fov(f32 fov);

    Camera& set_aspect(f32 aspect);

    Camera& set_frustum(f32 z_near, f32 z_far);

    Camera& set_pos(const Point3f& pos);

    Camera& set_at(const Point3f& at);

    Camera& set_up(const Vector3f& up);

    Matrix4f look_at() const;

    Matrix4f perspective() const;
};
