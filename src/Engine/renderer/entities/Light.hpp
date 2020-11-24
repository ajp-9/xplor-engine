#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace xplor
{
	namespace light
	{
		struct DirectionalLight
		{
			glm::vec3 direction;
			glm::vec3 ambient;
			glm::vec3 diffuse;

			DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse)
				: direction(direction), ambient(ambient), diffuse(diffuse)
			{}
		};

		struct PointLight
		{
			unsigned int ID;
			glm::vec3 position;
			glm::vec3 diffuse;

			PointLight(unsigned int ID, glm::vec3 position, glm::vec3 diffuse)
				: ID(ID), position(position), diffuse(diffuse)
			{}
		};

		struct SpotLight // Calculated at camera position.
		{
			glm::vec3 direction;
			glm::vec2 cutoff; // first is cutoff, second is outer cutoff
			glm::vec3 diffuse;
			
			SpotLight(glm::vec3 direction, glm::vec2 cutoff, glm::vec3 diffuse)
				: direction(direction), cutoff(cutoff), diffuse(diffuse)
			{}

			void shaderUniform()
			{

			}
		};
	}
}