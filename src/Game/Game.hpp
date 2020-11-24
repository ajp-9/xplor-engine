#pragma once

#include <SDL/SDL.h>

#include "../Engine/xplor.hpp"
#include "entities/Entity.hpp"
#include "entities/player/Player.hpp"
#include "entities/ModelManager.hpp"

class Game
{
	xplor::Core core;
	xplor::LightingShader shader;
	xplor::ShaderManager shader_manager;
	SDL_Event e;

	Player player = Player(glm::vec3(0));
	ModelManager model_manager;
	xplor::World world;

	std::vector<Entity> entities;
	glm::vec3 position = glm::vec3(0);
	xplor::Texture grass;

	xplor::light::PointLight pointLight = xplor::light::PointLight(0, glm::vec3(1.0f), glm::vec3(1.0f));

public:

	bool running = true;

	Game();
	
	void ready();
	void update();
	
	void tryKeyPress();
	void mouseMove();
	void mouseClick();

	void tryEvents();

	~Game();
};