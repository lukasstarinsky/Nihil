#pragma once

#include <filesystem>
#include "Mesh.hpp"

class Model
{
public:
    Model(const std::filesystem::path& filePath);

private:
    std::vector<Mesh> Meshes;
};