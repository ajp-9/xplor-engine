#ifdef EDITOR
#include <glad/glad.h>
#include <SDL/SDL.h>
#include <iostream>

#include <json.hpp>

#include <fstream>

#include "Editor.hpp"

#include "../Engine/xplor.hpp"

int main(int argc, char* argv[])
{
#ifdef XPLOR_DEBUG
	std::cout << "DEBUG MODE" << std::endl;
#endif

	Editor editor;

	editor.ready();

	while (editor.running)
	{
		editor.update();
	}

	editor.~Editor();

	return 0;
}
#endif