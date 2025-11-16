#pragma once
#include "Mesh.h"
#include <assimp/scene.h>

class Model {
public:
	Model(const char* file);

	void Draw(Shader& shader, camera::Camera& cam);

private:
	const char* m_file;
	
	

	std::vector<Mesh> m_meshes;
	std::vector<glm::vec3> m_translationsMeshes;
	std::vector<glm::quat> m_rotationsMeshes;
	std::vector<glm::vec3> m_scalesMeshes;
	std::vector<glm::mat4> m_matricesMeshes;

	void ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform = glm::mat4(1.0f));
	glm::mat4 aiMat4ToGlm(const aiMatrix4x4& from);
	std::vector<Vertex> LoadVertex(const aiMesh* mesh);
	std::vector<GLuint> LoadIndices(const aiMesh* mesh);
	std::vector<Texture> LoadTexture(const aiMaterial* material);

};
