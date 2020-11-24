#pragma once

#include "Terrain/Terrain.hpp"

namespace xplor
{
	struct World
	{
		std::shared_ptr<terrain::Terrain> terrain = std::make_shared<terrain::Terrain>();

		std::vector<light::PointLight> pointLights;

		light::DirectionalLight sun = light::DirectionalLight(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.05f), glm::vec3(1.0f));

		World() {}

		virtual void loadWorld()
		{

		}

		virtual void renderWorld()
		{

		}
	};
}