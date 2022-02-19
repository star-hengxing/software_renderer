#include "Camera.hpp"

#include <Hinae/Transform.hpp>

Camera& Camera::set_fov(f32 fov)
{
    this->fov = fov;
    return *this;
}

Camera& Camera::set_aspect(f32 aspect)
{
    this->aspect = aspect;
    return *this;
}

Camera& Camera::set_frustum(f32 z_near, f32 z_far)
{
    this->z_near = z_near;
    this->z_far  = z_far;
    return *this;
}

Camera& Camera::set_pos(const Point3f& pos)
{
    this->pos = pos;
    return *this;
}

Camera& Camera::set_at(const Point3f& at)
{
    this->at = at;
    return *this;
}

Camera& Camera::set_up(const Vector3f& up)
{
    this->up = up;
    return *this;
}

Matrix4f Camera::look_at() const
{
    return Transform<f32>::look_at(pos, at, up);
}

Matrix4f Camera::perspective() const
{
    return Transform<f32>::perspective(fov, aspect, z_near, z_far);
}
