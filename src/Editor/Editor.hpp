#pragma once

#include <SDL/SDL.h>

#include "../Engine/xplor.hpp"

#include "entities/player/Player.hpp"
#include "gui/EditorGUI_Manager.hpp"
#include "world/World.hpp"

class Editor
{
	xplor::Core core;
	xplor::LightingShader shader;
	xplor::ShaderManager shader_manager;
	SDL_Event e;

	Player player = Player(glm::vec3(0));
	ModelManager model_manager;
	World world;

	// Lots of shared pointers; cuz editor needs to have both entities and the container while also renderering it
	//std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>> entities = std::make_shared<std::vector<std::shared_ptr<xplor::Entity>>>();
	glm::vec3 position = glm::vec3(0);
	xplor::Texture grass;

	EditorGUI_Manager gui_manager;

	xplor::light::PointLight pointLight = xplor::light::PointLight(0, glm::vec3(1.0f), glm::vec3(1.0f));

public:

	bool running = true;

	Editor();
	
	void ready();
	void update();
	
	void tryKeyPress();
	void mouseMove();
	void mouseClick();

	void tryEvents();

	~Editor();
};