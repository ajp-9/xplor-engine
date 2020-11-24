#pragma once

#include <vector>
#include <iostream>

class Entity
{
	glm::mat4 transformationMatrix;

	glm::vec3 position;
	glm::vec3 rotation;
	float scale;

public:

	Entity(unsigned int modelID, glm::vec3 position, glm::vec3 rotation, float scale)
		: modelID(modelID), position(position), rotation(rotation), scale(scale)
	{}
	
	void render(xplor::LightingShader& shader, ModelManager& manager)
	{
		shader.setModelMatrix(transformationMatrix);
		manager.models[modelID].draw(shader);
	}

	// Rotation in radians.
	void updateTransformationMatrix(const glm::vec3& position, const glm::vec3& rotation, float scale)
	{
		transformationMatrix = xplor::Math::createTransformationMatrix(position, rotation, scale);
	}

private:

	unsigned int modelID;
	unsigned int shaderID;
};