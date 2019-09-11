#include "display.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

uint32 Display::numInstances = 0;

Display::Display(const char* title, uint32 width, uint32 height,
			bool visible)
		: window(nullptr)
		, width(width)
		, height(height) {
	if (numInstances == 0) {
		glfwInit();
	}
	
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, visible);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewInit();

	++numInstances;
}

bool Display::isCloseRequested() const {
	return glfwWindowShouldClose(window);
}

void Display::pollEvents() {
	glfwPollEvents();
}

void Display::render() {
	glfwSwapBuffers(window);
}

Display::~Display() {
	--numInstances;

	glfwDestroyWindow(window);

	if (numInstances == 0) {
		glfwTerminate();
	}
}
