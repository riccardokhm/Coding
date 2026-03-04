#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"
#include "Vertex.h"

using namespace std;
using namespace glm;

struct Texture
{
	unsigned int id;
	string type;
	string path;
};
