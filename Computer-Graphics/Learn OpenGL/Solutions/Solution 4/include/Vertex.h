#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"

using namespace std;
using namespace glm;

struct Vertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};
