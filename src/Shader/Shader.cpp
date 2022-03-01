#include "Shader.hpp"

#include <array>

#include <Hinae/coordinate_system.hpp>
#include <Hinae/Transform.hpp>

#include <Texture/Texture.hpp>
#include <Image/Color.hpp>
#include <util/util.hpp>

struct Light
{
    Point3f position;
    Vector3f intensity;
};

const std::array<Light, 2> lights
{
    Light{{20, 20, 20}, {500, 500, 500}},
    Light{{-20, 20, 0}, {500, 500, 500}}
};

constexpr Vector3f ka = 0.005f;
constexpr Vector3f ks = 0.7937f;
constexpr Vector3f ambient_light_intensity = 10;
constexpr f32 p = 150;

static Vector3f blinn_phong(const Point3f& point, const Vector3f& normal, const Vector3f& kd)
{
    Vector3f result_color = ka * ambient_light_intensity;
    const Vector3f view_dir = (-point).normalized();
    
    for(const Light& light : lights)
    {
        const f32 r2 = (light.position - point).norm2();
        const Vector3f light_dir = (light.position - point).normalized();
        const Vector3f intensity = light.intensity / r2;

        const Vector3f diffuse = kd * intensity * max(ZERO<f32>, dot(normal, light_dir));
        result_color += diffuse;

        const Vector3f half_vector = (view_dir + light_dir).normalized();
        const Vector3f specular = ks * intensity * std::pow(max(ZERO<f32>, dot(normal, half_vector)), p);
        result_color += specular;
    }
    return result_color;
}

constexpr f32 kh = 0.2f, kn = 0.1f;

static std::tuple<Vector3f, f32> bump(Payload& payload)
{
    const Vector3f n = payload.normal;
    // const auto [x, y, z] = n;
    // const Vector3f t = Vector3f{x * y, 0, z * y} / std::sqrt(x * x + z * z);
    // const Vector3f b = cross(n, t);
    const auto [t, b] = local_coordinate_system(n);

    const auto [u, v] = payload.uv;
    const auto [w, h] = payload.texture->get_width_height();

    const f32 huv = get_color(payload.texture.value(), payload.uv).norm();

    const f32 dU = kh * kn * (get_color(payload.texture.value(), {u + 1.0f / w, v}).norm() - huv);
    const f32 dV = kh * kn * (get_color(payload.texture.value(), {u, v + 1.0f / h}).norm() - huv);

    // const Matrix4f tbn
    // {
    //     t.x, b.x, n.x, 0,
    //     t.y, b.y, n.y, 0,
    //     t.z, b.z, n.z, 0,
    //     0, 0, 0, 1
    // };
    // const auto normal = tbn * Vector3f{-dU, -dV, 1.0f};
    const Vector3 normal
    {
        t.x * -dU + b.x * -dV + n.x,
        t.y * -dU + b.y * -dV + n.y,
        t.z * -dU + b.z * -dV + n.z
    };
    return {normal, huv};
}

void point_color_shader(Payload& payload)
{
    payload.pixel_color = payload.color;
}

void normal_shader(Payload& payload)
{
    // transforms from [-1,1] to [0,1]  
    const Vector3f color = (payload.normal.normalized() + ONE<f32>) / 2;
    payload.pixel_color = float_to_rgb(color);
}

void blinn_phong_shader(Payload& payload)
{
    const Vector3f kd = payload.texture
    ? get_color(payload.texture.value(), payload.uv)
    : Vector3f{1.0f};

    const Vector3f result_color = blinn_phong(
          payload.view_point
        , payload.normal.normalized()
        , kd);
    payload.pixel_color = float_to_rgb(result_color);
}

void bump_shader(Payload& payload)
{
    const auto [normal, huv] = bump(payload);
    payload.pixel_color = float_to_rgb(normal);
}

void displacement_shader(Payload& payload)
{
    const Vector3f kd = rgb_to_float<f32>(Color{148, 121, 92});
    const auto [normal, huv] = bump(payload);
    const Point3f pos = payload.view_point + (kn * normal * huv);
    payload.pixel_color = float_to_rgb(blinn_phong(pos, normal, kd));
}
