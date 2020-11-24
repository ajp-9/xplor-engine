#ifdef GAME
#include <glad/glad.h>
#include <SDL/SDL.h>
#include <iostream>

#include "Game.hpp"

int main(int argc, char* argv[])
{
#ifdef XPLOR_DEBUG
	std::cout << "DEBUG MODE" << std::endl;
#endif

	Game game;

	game.ready();

	while (game.running)
	{
		game.update();
	}

	game.~Game();

	return 0;
}
#endif