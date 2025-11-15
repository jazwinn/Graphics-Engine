#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Events/Delegate.h"

namespace window {
	class Window {
	public:
		Window(int width, int height);
		Window() = delete;
		~Window();

		void Startframe();
		//ensure is the bottom most
		void Endframe();


		int GetWindowWidth();
		int GetWindowHeigth();



		GLFWwindow* GetWindow() { return m_window; };

		Delegate<unsigned, unsigned> OnResize;

	private:
		void InitializeCallBacks();
		GLFWwindow* m_window;
	};
}