#pragma once
#include "Mesh.h"

class Light {
public:
	Light(Shader& lightShader, const std::vector<Vertex>& vboData = std::vector<Vertex>(), const std::vector<GLuint>& eboDat = std::vector<GLuint>(), const std::vector<Texture>& texture = std::vector<Texture>());
	void Draw(Shader& shader, camera::Camera& camera);






	glm::vec4 GetColor() { return m_color; }
	glm::vec3 GetPosition() { return m_position; }
	void SetColor(glm::vec4 clr) { m_color = clr; }
	void SetPosition(glm::vec3 pos) { m_position = pos; }

private:
	Shader& m_lightShader;
	Mesh m_mesh;
	glm::vec4 m_color = glm::vec4{1.f,1.f,1.f,1.f};
	glm::vec3 m_position;
	glm::mat4 m_model;
};