#pragma once

#include "../../../Engine/xplor.hpp"
#include "PlayerController.hpp"

struct Player : public PlayerController
{
	//xplor::light::SpotLight flashlight = xplor::light::SpotLight();

	Player(glm::vec3 position);
};