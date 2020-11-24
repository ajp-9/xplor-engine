#include "ShaderManager.hpp"


#include <iostream>

namespace xplor
{
	void ShaderManager::Init(const glm::mat4x4 projection)
	{
		lighting = LightingShader(projection);
		terrain = terrain::TerrainShader(projection);
	}

	void ShaderManager::bindShader(unsigned char shader)
	{
		/*if (current_shader != shader)
		{
			if (shader <= shaders.size())
				shaders[shader].bind();
			else
				std::cout << "ERROR::SHADER_ID_OUT_OF_RANGE" << std::endl;
		}*/
	}

	void ShaderManager::setDirectionalLight(const glm::vec3& directionalLight)
	{
		lighting.bind();
		lighting.setDirectionalLight(directionalLight);
		terrain.bind();
		terrain.setDirectionalLight(directionalLight);
	}
	
	void ShaderManager::setPointLight(const light::PointLight& light)
	{
		lighting.bind();
		lighting.setPointLight(light);
		terrain.bind();
		terrain.setPointLight(light);
	}

	void ShaderManager::setSpotLightActivated(const bool activated)
	{
		lighting.bind();
		lighting.setSpotLightActivated(activated);
		terrain.bind();
		terrain.setSpotLightActivated(activated);
	}

	void ShaderManager::setSpotLightDirection(const glm::vec3& direction)
	{
		lighting.bind();
		lighting.setSpotLightDirection(direction);
		terrain.bind();
		terrain.setSpotLightDirection(direction);
	}

	void ShaderManager::setCameraPosVec(const glm::vec3& position)
	{
		lighting.bind();
		lighting.setCameraPosVec(position);
		terrain.bind();
		terrain.setCameraPosVec(position);
	}

	
	void ShaderManager::setModelMatrix(const glm::mat4x4& model)
	{
		lighting.bind();
		lighting.setModelMatrix(model);
		terrain.bind();
		terrain.setModelMatrix(model);
	}

	void ShaderManager::setViewMatrix(const glm::mat4x4& view)
	{
		lighting.bind();
		lighting.setViewMatrix(view);
		terrain.bind();
		terrain.setViewMatrix(view);
	}

	void ShaderManager::setProjectionMatrix(const glm::mat4x4& projection)
	{
		lighting.bind();
		lighting.setProjectionMatrix(projection);
		terrain.bind();
		terrain.setProjectionMatrix(projection);
	}
}