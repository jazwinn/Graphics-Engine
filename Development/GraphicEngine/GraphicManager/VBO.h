#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color = glm::vec4{1.f,1.f,1.f,1.f};
	glm::vec2 texUV;
};

class VBO
{
public:

	template <typename T>
	VBO(const std::vector<T>& vertices, GLenum drawMode = GL_DYNAMIC_DRAW, GLenum target = GL_ARRAY_BUFFER) :
		m_Target(target),
		m_DrawMode(drawMode),
		m_Size(vertices.size() * sizeof(T))
	{
		glGenBuffers(1, &m_ID);//genereate vertex buffer object to store data
		glBindBuffer(target, m_ID); //bind VBO

		//assign vertices data into the VBO
		glBufferData(target, m_Size, vertices.data(), drawMode);

	}

	void UpdateData(const void* data, GLsizeiptr size, GLintptr offset);

	void Bind();
	void Unbind();
	void Delete();

private:
	GLuint m_ID;
	GLenum m_Target;
	GLenum m_DrawMode;
	GLuint m_Size;
};
