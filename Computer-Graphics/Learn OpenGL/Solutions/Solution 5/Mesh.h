#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"

using namespace std;
using namespace glm;

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader& shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();

};

