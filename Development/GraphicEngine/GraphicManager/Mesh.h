#pragma once

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"



struct Instance {
	
	glm::mat4 modelMatrix;
	GLfloat visable = 0.f;
};


class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vboData, const std::vector<GLuint>& eboData, const std::vector<Texture>& texture, GLuint instanced = 1, 
		 const std::vector<glm::mat4x4>& instanceMatrix = {},  GLenum DrawType = GL_DYNAMIC_DRAW, GLenum target = GL_ARRAY_BUFFER );
	~Mesh();
	
	VBO& GetInstanceVBO() {
		return m_instanceVBO;
	}

	VBO& GetVBO() {
		return m_VBO;
	}

	VAO& GetVAO() {
		return m_VAO;
	}
	void SetInstanceCount(GLuint count) {
		m_instance = count;
	}
	
	
	void Draw(Shader& shader, camera::Camera& camera,
		const glm::mat4& model = glm::mat4(1.0f), //so either mode, or the whole TRS
		const glm::vec3& translation = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), GLenum mode = GL_TRIANGLES);


private:
	VAO m_VAO;
	VBO m_VBO;
	VBO m_instanceVBO;
	EBO m_EBO;

	GLuint m_instance;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_texture;
};

