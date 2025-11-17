#include "GraphicPipeline.h"
#include <stb_image.h>

namespace graphic { 
	


	GraphicPipeline::GraphicPipeline(window::Window& _window):
		m_genericShader("../GraphicEngine/GraphicManager/Shaders/genericVertexShader.vert", "../GraphicEngine/GraphicManager/Shaders/genericFragmentShader.frag"),
		m_InstancedShader("../GraphicEngine/GraphicManager/Shaders/instancedVertexShader.vert", "../GraphicEngine/GraphicManager/Shaders/instancedFragmentShader.frag"),
		m_lightShader("../GraphicEngine/GraphicManager/Shaders/lightVertexShader.vert", "../GraphicEngine/GraphicManager/Shaders/lightFragmentShader.frag"),
		m_window{ _window },
		m_camera(m_window.GetWindowWidth(), m_window.GetWindowHeigth()),
		m_Shapes{m_camera, m_genericShader, m_InstancedShader }
	{
		m_window.OnResize.Add([&](auto width, auto height) {
			int fbWidth, fbHeight;
			glfwGetFramebufferSize(m_window.GetWindow(), &fbWidth, &fbHeight);
			glViewport(0, 0, fbWidth, fbHeight);
			glScissor(0,0,fbWidth, fbHeight);

			m_camera.Resize(width, height);
		});


		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);

		if (config.backFaceCulling) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
		}
		else {
			glDisable(GL_CULL_FACE);
		}

	}

	GraphicPipeline::~GraphicPipeline()
	{
		m_genericShader.Delete();
	}

	void GraphicPipeline::Update(float dt)
	{
		int display_w = 0;
		int display_h = 0;
		glfwGetFramebufferSize(m_window.GetWindow(), &display_w, &display_h);

		double cursor_x = 0.0;
		double cursor_y = 0.0;
		glfwGetCursorPos(m_window.GetWindow(), &cursor_x, &cursor_y);

		if (glfwGetMouseButton(m_window.GetWindow(), GLFW_MOUSE_BUTTON_2) != 0) {
			glm::vec3 side = glm::normalize(glm::cross(m_camera.direction, { 0, 1, 0 }));
			glm::vec3 up = glm::normalize(glm::cross(m_camera.direction, side));
			glm::vec3 forward = glm::normalize(m_camera.direction);


			glm::vec3 movement(0.0f);

			float speed = 80.0f;
			if (glfwGetKey(m_window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) movement += forward;
			if (glfwGetKey(m_window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) movement -= forward;
			if (glfwGetKey(m_window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) movement += side;
			if (glfwGetKey(m_window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) movement -= side;
			if (glfwGetKey(m_window.GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) movement += up;
			if (glfwGetKey(m_window.GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) movement -= up;

			if (glm::length(movement) > 0.0f) {
				m_camera.SetPosition(m_camera.GetPosition() + glm::normalize(movement) * speed * dt);
			}
				

			glm::vec2 cursor_delta = { (float)cursor_x - m_camera.prevMouseCursor.x, (float)cursor_y - m_camera.prevMouseCursor.y };
			float     rotation_speed = 0.01f;
			m_camera.direction = glm::vec3(glm::vec4(m_camera.direction, 0) * glm::rotate(glm::mat4(1.0f), cursor_delta.y * rotation_speed, side));
			m_camera.direction = glm::vec3(glm::vec4(m_camera.direction, 0) * glm::rotate(glm::mat4(1.0f), cursor_delta.x * rotation_speed, glm::vec3(0, 1, 0)));

		}

		m_camera.UpdateProjection();
		m_camera.UpdateView();
		m_camera.UpdateViewProjection();
		m_camera.prevMouseCursor = glm::vec2{cursor_x, cursor_y};
	}



	void GraphicPipeline::Draw() {

		m_Shapes.Draw();

	}

	void GraphicPipeline::StartFrame()
	{
		glClearDepth(1.0);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GraphicPipeline::EndFrame()
	{

	}

}