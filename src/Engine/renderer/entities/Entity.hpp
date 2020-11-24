#pragma once

namespace xplor
{
	struct Entity
	{
		int modelID;

		glm::mat4 transformationMatrix;

		glm::vec3 position;
		glm::vec3 rotation;
		float scale;

	public:

		Entity(unsigned int modelID, glm::vec3 position, glm::vec3 rotation, float scale)
			: modelID(modelID), position(position), rotation(rotation), scale(scale)
		{
			updateTransformationMatrix(position, rotation, scale);
		}

		void render(xplor::LightingShader& shader, ModelManager& manager)
		{
			shader.setModelMatrix(transformationMatrix);
			manager.models[modelID].draw(shader);
		}

		// Updates matrix and all the positions for the entity.
		// Rotation in radians.
		void updateTransformationMatrix(const glm::vec3& position, const glm::vec3& rotation, float scale)
		{
			this->position = position; this->rotation = rotation; this->scale = scale;
			transformationMatrix = xplor::Math::createTransformationMatrix(position, rotation, scale);
		}
		// Updates matrix without anything else. (Should be most common)
		void updateTransformationMatrix()
		{
			transformationMatrix = xplor::Math::createTransformationMatrix(position, rotation, scale);
		}

	private:

		unsigned int shaderID;
	};
}