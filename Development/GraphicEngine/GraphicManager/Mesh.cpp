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
    // Collect texture IDs by type
    std::vector<GLint> diffuseTextures;
    std::vector<GLint> specularTextures;

    int count = 0;
    for (auto& tex : m_texture)
    {
        if (tex.type == DIFFUSE)
            diffuseTextures.push_back(tex.ID);  // actual OpenGL texture ID
        else if (tex.type == SPECULAR)
            specularTextures.push_back(tex.ID);
        count++;
        if (count == 2) break;
    }

    // Bind textures to texture units
    glUseProgram(shader.m_ID);

    // Diffuse textures
    for (int i = 0; i < diffuseTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, diffuseTextures[i]);
    }

    // Specular textures (after diffuse units)
    int specOffset = diffuseTextures.size();
    for (int i = 0; i < specularTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + specOffset + i);
        glBindTexture(GL_TEXTURE_2D, specularTextures[i]);
    }

    // Set uniform arrays in shader (texture units)
    if (!diffuseTextures.empty())
    {
        GLint loc = glGetUniformLocation(shader.m_ID, "diffuse");
        std::vector<GLint> units(diffuseTextures.size());
        for (int i = 0; i < units.size(); i++) units[i] = i;
        shader.setUniform("diffuse", units.data(), units.size());

        glUniform1iv(loc, units.size(), units.data());
    }

    if (!specularTextures.empty())
    {
        GLint loc = glGetUniformLocation(shader.m_ID, "specular");
        std::vector<GLint> units(specularTextures.size());
        for (int i = 0; i < units.size(); i++) units[i] = specOffset + i;
        glUniform1iv(loc, units.size(), units.data());
    }

    // Set additional uniforms
    shader.Activate();
    shader.setUniform("diffuseCount", static_cast<int>(diffuseTextures.size()));
    shader.setUniform("specularCount", static_cast<int>(specularTextures.size()));
    shader.setUniform("useTexture", !m_texture.empty());
    shader.setUniform("camVP", camera.GetViewProjectionMatrix());
    shader.setUniform("camPos", camera.GetPosition());

    // Compute model transforms
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 rot = glm::mat4_cast(rotation);
    glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

    shader.setUniform("translation", trans);
    shader.setUniform("rotation", rot);
    shader.setUniform("scale", sca);
    shader.setUniform("model", model);

    // Draw
    m_VAO.Bind();
    if (m_instance == 1)
        glDrawElements(mode, m_indices.size(), GL_UNSIGNED_INT, 0);
    else
        glDrawElementsInstanced(mode, m_indices.size(), GL_UNSIGNED_INT, 0, m_instance);
    m_VAO.Unbind();

    // Reset active texture (optional, good practice)
    glActiveTexture(GL_TEXTURE0);
}
