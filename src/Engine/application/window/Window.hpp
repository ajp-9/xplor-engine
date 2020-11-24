#pragma once

#include <SDL/SDL.h>
#include <glm/vec2.hpp>

#include "../GUI/GUI_Manager.hpp"

namespace xplor
{
	struct Window
	{
		SDL_Window* window = nullptr;

		SDL_GLContext context = SDL_GLContext();

		const char* title = "Xplor Engine v0.0.0";
		int width = 800, height = 800;
		glm::ivec2 position = glm::ivec2(1100, 100);
		bool isMouseCatched = false;

		Window() {}

		void Init();

		void update();

		void catchMouse(bool catched);

		void resizeWindow(int width, int height);

		~Window();

	private:

		void setUpOpenGL();

	};
}