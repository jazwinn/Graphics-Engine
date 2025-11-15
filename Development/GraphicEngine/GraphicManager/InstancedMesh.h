#pragma once

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"




class InstancedMesh
{
public:
	InstancedMesh(std::vector<GLfloat>& vboData, std::vector<GLuint>& eboData, GLuint instanced = 1,
		 const std::vector<glm::mat4x4>& instanceMatrix = {}, GLenum target = GL_ARRAY_BUFFER, GLenum DrawType = GL_STATIC_DRAW);
	~InstancedMesh();
	
	VBO& GetInstancedVBO() {
		return m_instanceVBO;
	}
	VBO& GetColorVBO() {
		return m_ColorVBO;
	}

	VAO& GetVAO() {
		return m_VAO;
	}
	void SetInstanceCount(GLuint count) {
		m_instance = count;
	}
	
	void Draw(GLenum mode);


private:
	VAO m_VAO;
	VBO m_VBO;
	VBO m_instanceVBO;
	VBO m_ColorVBO;
	EBO m_EBO;

	GLuint m_indexCount;

	GLuint m_instance;

	std::vector<GLfloat> m_vertices;
	std::vector<GLuint> m_indices;
};

