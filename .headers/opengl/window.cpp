#include <glad/glad.h>
#include <opengl/window.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

namespace GL_TEST{

	Window::Window(const unsigned int width, const unsigned int height, const std::string title, GLFWmonitor* monitor) 
		: height{ height }, width{ width }, title{ title } , monitor(monitor)
	{
		try {
			initWindow();
		}catch(const std::runtime_error& e){
			std::cerr << "\n[WINDOW] init failed! Why : " << e.what() << '\n';
			failed = true;
		}
	}

	void Window::initWindow(){
		if (!glfwInit()) {
			throw std::runtime_error("\n[WINDOW]GLFW couldn't init!\n");
			glfwTerminate();
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		Window::window = glfwCreateWindow(Window::width, Window::height, Window::title.c_str(),Window::monitor , NULL);
		if (window == NULL) {
			throw std::runtime_error("\n[WINDOW] Creation Failed!\n");
			glfwTerminate();
		}

		glfwMakeContextCurrent(Window::window);

	}

	Window::~Window() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}