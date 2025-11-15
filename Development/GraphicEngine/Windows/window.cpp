#include "Window.h"
#include <iostream>

namespace window {
	void sizeCallback(GLFWwindow* pWindow, int width, int height)
	{
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(pWindow, &fbWidth, &fbHeight);

		
		
		auto* window = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
		if (window) {
			window->OnResize.Invoke(width, height);
		}
	}

	void Window::InitializeCallBacks() {
		glfwSetWindowSizeCallback(m_window, sizeCallback);
	}



	Window::Window(int width, int height) {
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		// Create window
		m_window = glfwCreateWindow(width, height, "3D project", NULL, NULL);

		// Check if create window is successfull, else stop program
		if (m_window == NULL) {
			std::cout << "Fail to create window" << std::endl;
			glfwTerminate();
			return;
		}

		// Set window to be be current context
		glfwMakeContextCurrent(m_window);

		// Initialize glew
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD!" << std::endl;
			return;
		}

		// Area for open gl to render in (usually the entire window)
		glViewport(0, 0, 1600, 800);

		// Set window color to black
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap buffers
		glfwSwapBuffers(m_window);

		//Callbacks
		glfwSetWindowUserPointer(m_window, this);
		InitializeCallBacks();
	}

	Window::~Window() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::Startframe()
	{
		glfwPollEvents();

	}

	void Window::Endframe()
	{
		glfwSwapBuffers(m_window);
	}

	int Window::GetWindowWidth() {
		int width{}, height{};
		glfwGetWindowSize(m_window, &width, &height);
		return width;
	}

	int Window::GetWindowHeigth() {
		int width{}, height{};
		glfwGetWindowSize(m_window, &width, &height);
		return height;
	}
}