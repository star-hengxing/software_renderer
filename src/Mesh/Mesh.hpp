#pragma once

#include <vector>
#include <array>

#include <Hinae/Vector3.hpp>
#include <Hinae/Point3.hpp>
#include <Hinae/Point2.hpp>

using namespace Hinae;

#include <Image/Image.hpp>

struct Face
{
    usize point, uv, normal;

    usize& operator [] (usize i) { return (&point)[i]; }
};

struct Mesh
{
    std::vector<Point3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<Point2f> uvs;
    std::vector<std::array<Face, 3>> faces;

    std::optional<Image> texture;

private:
    void parse(std::string_view filename);

public:
    static std::optional<Mesh> read(std::string_view obj_path, std::string_view texture_path = "");

    bool load_obj(std::string_view filename);

    bool load_texture(std::string_view filename);

    bool empty() const;

    void info() const;
};
