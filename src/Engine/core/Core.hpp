#pragma once

#include "../application/window/Window.hpp"
#include "../renderer/Renderer.hpp"

namespace xplor
{
	struct Core // the interface between the engine and the game
	{
		//Game game;
		Window window;
		Renderer renderer;

	public:

		Core() {}
		void initApplicatiion();

		~Core();
	};
}