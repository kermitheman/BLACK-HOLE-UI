#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace GL_TEST {
	class Window {
	private:
		const std::string title;
		GLFWmonitor* monitor;
		const unsigned int width;
		const unsigned int height;
	
		void initWindow();

	public:
		GLFWwindow* window;
		Window(const unsigned int width, const unsigned int height, const std::string title, GLFWmonitor* monitor);
		~Window();
		bool failed = false;
	};
}