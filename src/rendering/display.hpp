#pragma once

#include "core/common.hpp"

class GLFWwindow;

class Display {
	public:
		Display(const char* title, uint32 width, uint32 height,
				bool visible = true);

		bool isCloseRequested() const;
		void pollEvents();
		void render();

		inline uint32 getWidth() const { return width; }
		inline uint32 getHeight() const { return height; }

		inline GLFWwindow* getHandle() { return window; }

		~Display();
	private:
		NULL_COPY_AND_ASSIGN(Display);

		GLFWwindow* window;
		uint32 width;
		uint32 height;

		static uint32 numInstances;
};
