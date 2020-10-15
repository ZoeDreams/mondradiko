#pragma once

#include <map>

#include "api_headers.hpp"

class Filesystem;
class Renderer;

class Scene
{
public:
    Scene(Filesystem*, Renderer*);
    ~Scene();

    void update(double);

    bool loadModel(const char*);
private:
    Filesystem* fs;
    Renderer* renderer;

    Assimp::Importer importer;
};
