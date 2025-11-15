#include "DrawShapes.h"

namespace shapes {

	void CreateSphere(float radius, int sectorCount, int stackCount,
		std::vector<GLfloat>& vertices,
		std::vector<GLuint>& indices);



	Shapes::Shapes(camera::Camera& camera, Shader& shader, Shader& instancedShader) :
		m_camera(camera),
		m_Shader(shader),
		m_InstancedShader(instancedShader)
	{

		std::vector<GLfloat> verticesAABB =
		{
			-0.5, -0.5, -0.5 ,
			 -0.5, -0.5, 0.5 ,
			 -0.5, 0.5, -0.5 ,
			 -0.5, 0.5, 0.5 ,
			 0.5, -0.5, -0.5 ,
			 0.5, -0.5, 0.5 ,
			 0.5, 0.5, -0.5 ,
			 0.5, 0.5, 0.5 ,
		};

		std::vector<GLuint> indicesAABB = {
			// Bottom face edges
			0, 1,
			1, 5,
			5, 4,
			4, 0,

			// Top face edges
			2, 3,
			3, 7,
			7, 6,
			6, 2,

			// Vertical edges
			0, 2,
			1, 3,
			5, 7,
			4, 6
		};
		m_RectangleMesh = std::make_unique<Mesh>(verticesAABB, indicesAABB);
		m_shapeInstance[AABB] = std::make_unique<InstancedMesh>(verticesAABB, indicesAABB, m_shapeData[AABB].transformVector.size(), m_shapeData[AABB].transformVector, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);


		std::vector<GLfloat> sphereVertices;
		std::vector<GLuint> sphereIndices;
		CreateSphere(1.0f, 36, 18, sphereVertices, sphereIndices);
		m_shapeInstance[SPHERE] = std::make_unique<InstancedMesh>(sphereVertices, sphereIndices, m_shapeData[SPHERE].transformVector.size(), m_shapeData[SPHERE].transformVector, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

		std::vector<GLfloat> planeVertices = {
		-0.5f, -0.5f, 0.0f,  // bottom-left
		 0.5f, -0.5f, 0.0f,  // bottom-right
		 0.5f,  0.5f, 0.0f,  // top-right
		-0.5f,  0.5f, 0.0f   // top-left
			};
		std::vector<GLuint> planeIndices = {
		0, 1, 2,  // first triangle
		2, 3, 0   // second triangle
			};
		m_shapeInstance[PLANE] = std::make_unique<InstancedMesh>(planeVertices, planeIndices, m_shapeData[PLANE].transformVector.size(), m_shapeData[PLANE].transformVector, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

		std::vector<GLfloat> lineVertices = {
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f
		};

		std::vector<GLuint> lineIndices = {
			0, 1
		};

		m_LineMesh = std::make_unique<Mesh>(lineVertices, lineIndices, 1, std::vector<glm::mat4x4>(), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

	}

	void Shapes::Draw_Rectangle(const glm::mat4& vp, const glm::vec3 center, const glm::vec3 scale, const glm::vec4& color, const DrawType drawtype, const glm::vec4& wireFrameColor)
	{
		m_Shader.Activate();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), scale);

		glm::mat4 m2w = vp * model;

		m_Shader.setUniform("uniform_m2w", m2w);
		m_Shader.setUniform("uniform_color", color);



		switch (drawtype)
		{
		case FILLED:
			m_RectangleMesh->Draw(GL_TRIANGLE_FAN);
			break;
		case WIREFRAME:

			m_RectangleMesh->Draw(GL_LINES);
			break;
		case FILLEDWIREFRAME:
			m_RectangleMesh->Draw(GL_TRIANGLE_FAN);

			m_Shader.setUniform("uniform_color", wireFrameColor);
			m_RectangleMesh->Draw(GL_LINES);
			break;
		default:
			break;
		}

	}

	void Shapes::Draw_Line(const glm::mat4& vp, const glm::vec3 start, const glm::vec3 end, const glm::vec4& color)
	{
		m_Shader.Activate();
		//auto const& model = glm::translate(glm::mat4(1.f), start) * glm::scale(glm::mat4(1.f), end - start);

		//glm::mat4 m2w = vp * model;

		m_Shader.setUniform("uniform_m2w", vp);
		m_Shader.setUniform("uniform_color", color);

		std::vector<GLfloat> lineVertices = {
			start.x, start.y, start.z,
			end.x, end.y, end.z
		};
		m_LineMesh->GetVAO().Bind();
		m_LineMesh->GetVBO().UpdateData(lineVertices.data(), lineVertices.size() * sizeof(GLfloat), 0);

		m_LineMesh->Draw(GL_LINES);

	}

	void Shapes::DrawCube(const glm::vec3& center, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, center);
		model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
		model = glm::scale(model, scale);

		m_shapeData[AABB].transformVector.push_back(model);
		m_shapeData[AABB].color.push_back(color);
	}

	void Shapes::DrawCube(const shapes::Aabb& aabb, const glm::vec4& color)
	{

		glm::mat4 model(1.0f);

		glm::vec3 center = aabb.get_center();
		glm::vec3 extents = aabb.get_extents();

		model = glm::translate(model, center);

		// Scale by full size (2 * extents)
		model = glm::scale(model, extents * 2.0f);

		m_shapeData[AABB].transformVector.push_back(model);
		m_shapeData[AABB].color.push_back(color);
	}

	void Shapes::DrawSphere(const shapes::Sphere& sphere, const glm::vec4& color)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, sphere.center);
		model = glm::scale(model, glm::vec3(sphere.radius));

		m_shapeData[SPHERE].transformVector.push_back(model);
		m_shapeData[SPHERE].color.push_back(color);
	}

	void Shapes::DrawPlane(const shapes::Plane& plane, float width, float height, const glm::vec4& color)
	{
		glm::vec3 pointOnPlane = -plane.normal * plane.d;
		glm::vec3 defaultNormal = glm::vec3(0, 0, 1);
		glm::vec3 rotationAxis = glm::cross(defaultNormal, plane.normal);
		float cosAngle = glm::dot(defaultNormal, plane.normal);
		float angle = acosf(glm::clamp(cosAngle, -1.0f, 1.0f));
		glm::mat4 rot(1.0f);
		if (glm::length(rotationAxis) > 0.0001f)
			rot = glm::rotate(glm::mat4(1.0f), angle, glm::normalize(rotationAxis));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), pointOnPlane);
		glm::mat4 model = translate * rot * scale;


		m_shapeData[PLANE].transformVector.push_back(model);
		m_shapeData[PLANE].color.push_back(color);
	}





	void Shapes::Draw() {

		for (const auto& [type, instance] : m_shapeInstance) {
			auto& shapeData = m_shapeData[type];
			if (shapeData.transformVector.size() > 0) {
				instance->SetInstanceCount(shapeData.transformVector.size());
				instance->GetInstancedVBO().UpdateData(shapeData.transformVector.data(), shapeData.transformVector.size() * sizeof(glm::mat4), 0);

				//set color
				instance->SetInstanceCount(shapeData.color.size());
				instance->GetColorVBO().UpdateData(shapeData.color.data(), shapeData.color.size() * sizeof(glm::vec4), 0);

				{
					m_InstancedShader.Activate();
					m_InstancedShader.setUniform("uniform_m2w", m_camera.GetViewProjectionMatrix());

					switch (FILLEDWIREFRAME)
					{
					case FILLED:
						instance->Draw(GL_TRIANGLE_FAN);
						break;
					case WIREFRAME:

						instance->Draw(GL_LINES);
						break;
					case FILLEDWIREFRAME:
						instance->Draw(GL_TRIANGLE_FAN);

						m_Shader.setUniform("uniform_color", glm::vec4{1.f,1.f,1.f,1.f});
						instance->Draw(GL_LINES);
						break;
					default:
						break;
					}
				}


				shapeData.transformVector.clear();
			}
		}

	}



	void CreateSphere(float radius, int sectorCount, int stackCount,
		std::vector<GLfloat>& vertices,
		std::vector<GLuint>& indices)
	{
		const float PI = 3.14159265359f;

		for (int i = 0; i <= stackCount; ++i) {
			float stackAngle = PI / 2 - i * (PI / stackCount); // from pi/2 to -pi/2
			float xy = radius * cosf(stackAngle);
			float z = radius * sinf(stackAngle);

			for (int j = 0; j <= sectorCount; ++j) {
				float sectorAngle = j * (2 * PI / sectorCount);

				float x = xy * cosf(sectorAngle);
				float y = xy * sinf(sectorAngle);

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
		}

		// Generate indices (two triangles per quad)
		for (int i = 0; i < stackCount; ++i) {
			int k1 = i * (sectorCount + 1);
			int k2 = k1 + sectorCount + 1;

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				if (i != (stackCount - 1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}

}

