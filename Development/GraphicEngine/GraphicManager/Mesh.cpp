#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vboData, const std::vector<GLuint>& eboData, const std::vector<Texture>& texture, GLuint instance, const std::vector<glm::mat4x4>& instanceMatrix, GLenum DrawType, GLenum target) :
	m_vertices{ vboData },
	m_indices{ eboData },
	m_instance{ instance },
	m_texture{texture},
	m_EBO(eboData),
	m_VBO(vboData, DrawType, target),
	m_instanceVBO(instanceMatrix, DrawType, target)
{

	m_VAO.Bind();

	m_VAO.LinkAttribute(m_VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_VAO.LinkAttribute(m_VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	m_VAO.LinkAttribute(m_VBO, 2, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
	m_VAO.LinkAttribute(m_VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
	if (instance != 1)
	{
		m_instanceVBO.Bind();
		// Can't link to a mat4 so you need to link four vec4s
		m_VAO.LinkAttribute(m_instanceVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
		m_VAO.LinkAttribute(m_instanceVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
		m_VAO.LinkAttribute(m_instanceVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		m_VAO.LinkAttribute(m_instanceVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		
		// Makes it so the transform is only switched when drawing the next instance
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
	}
	// Unbind all to prevent accidentally modifying them
	m_VAO.Unbind();
	m_VBO.Unbind();
	m_instanceVBO.Unbind();
	m_EBO.Unbind();
}



Mesh::~Mesh()
{
	m_VAO.Delete();
	m_VBO.Delete();
	m_EBO.Delete();
	m_instanceVBO.Delete();
}

void Mesh::Draw(Shader& shader, camera::Camera& camera,
	const glm::mat4& model,
	const glm::vec3& translation,
	const glm::quat& rotation,
	const glm::vec3& scale,
	GLenum mode)
{
	shader.Activate();
	shader.setUniform("camVP", camera.GetViewProjectionMatrix());
	m_VAO.Bind();

	if (m_instance == 1) {

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, translation);
		rot = glm::mat4_cast(rotation);
		sca = glm::scale(sca, scale);

		shader.setUniform("translation", trans);
		shader.setUniform("rotation", rot);
		shader.setUniform("scale", sca);
		shader.setUniform("model", model);

		glDrawElements(mode, m_indices.size(), GL_UNSIGNED_INT, 0);
		
	}
	else {
		glDrawElementsInstanced(mode, m_indices.size(), GL_UNSIGNED_INT, 0, m_instance);
	}

	m_VAO.Unbind();
}