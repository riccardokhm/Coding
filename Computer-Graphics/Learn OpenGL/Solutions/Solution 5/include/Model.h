#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <assimp/scene.h>


class Model
{
public:
	Model(const char* path) { loadModel(path); };
	void Draw(Shader &shader);

private:
	vector<Mesh> meshes;
	vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

};

