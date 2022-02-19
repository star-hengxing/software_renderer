#include "Mesh.hpp"

#include <string_view>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::optional<Mesh> Mesh::read(std::string_view obj_path, std::string_view texture_path)
{
    Mesh mesh;
    if(!mesh.load_obj(obj_path) || mesh.empty())
    {
        std::cout << "Cannot open " << obj_path << '\n';
        return {};
    }

    if(!texture_path.empty())
    {
        if(!mesh.load_texture(texture_path))
        {
            std::cout << "Cannot open " << texture_path << '\n';
            return {};
        }
    }
    return std::move(mesh);
}

bool Mesh::load_obj(std::string_view filename)
{
    if(!std::filesystem::exists(filename))
        return false;

    parse(filename);
    return true;
}

bool Mesh::load_texture(std::string_view filename)
{
    texture = Image::read(filename);
    return texture.has_value();
}

bool Mesh::empty() const
{
    return vertices.empty() || faces.empty();
}

void Mesh::info() const
{
    std::cout
        << "vertex: " << vertices.size() << '\n'
        << "face: "   << faces.size() << '\n'
        << std::boolalpha
        << "normal: " << !normals.empty() << '\n'
        << "uv: "     << !uvs.empty() << '\n';
}

void Mesh::parse(std::string_view filename)
{
    std::ifstream in(filename.data(), std::ifstream::in);

    std::string line;
    while(std::getline(in, line))
    {
        if(line[0] == 'v')
        {
            if(line[1] == ' ')
            {
                std::istringstream iss(line.substr(2));
                Point3f p;
                iss >> p.x >> p.y >> p.z;
                vertices.push_back(p);
            }
            else if(line[1] == 'n' && line[2] == ' ')
            {
                std::istringstream iss(line.substr(2));
                Vector3f n;
                iss >> n.x >> n.y >> n.z;
                normals.push_back(n);
            }
            else if(line[1] == 't' && line[2] == ' ')
            {
                std::istringstream iss(line.substr(2));
                Point2f uv;
                iss >> uv.x >> uv.y;
                uvs.push_back(uv);
            }
        }
        else if(line[0] == 'f' && line[1] == ' ')
        {
            i8 trash;
            std::array<Face, 3> f;
            std::istringstream iss(line.substr(2));

            for(auto& face : f)
            {
                iss >> face.point >> trash;
                face.point--;
                if(isspace(trash)) continue;

                iss >> face.uv >> trash;
                face.uv--;
                if(isspace(trash)) continue;
                
                iss >> face.normal;
                face.normal--;
            }
            faces.push_back(f);
        }
    }
    in.close();
}
