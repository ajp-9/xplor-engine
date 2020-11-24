#pragma once

#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace xplor
{
	class TextureShader : public Shader
	{
		unsigned int modelMatLocation;
		unsigned int viewMatLocation;
		unsigned int projectionMatLocation;

	public:

		TextureShader() { modelMatLocation = -1; viewMatLocation = -1; projectionMatLocation = -1; }
		TextureShader(glm::mat4x4 projection)
		{
			Init(std::string("res/shaders/texture.vs").c_str(), std::string("res/shaders/texture.fs").c_str());
			setLocations();
			bind();
			glm::mat4 thisProjection = glm::mat4(1.0f); // only needs to change when window changes
			thisProjection = projection;
			setProjectionMatrix(projection);
		}

		void setLocations()
		{
			modelMatLocation = glGetUniformLocation(ID, "model");
			viewMatLocation = glGetUniformLocation(ID, "view");
			projectionMatLocation = glGetUniformLocation(ID, "projection");
		}

		// Should be called every frame.
		void setModelMatrix(glm::mat4x4& model)
		{
			glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
		}

		// Should change when camera moves.
		void setViewMatrix(glm::mat4x4& view)
		{
			glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, glm::value_ptr(view));
		}

		// Should only change when changing fov, aspect ratio, and clipping distance.
		void setProjectionMatrix(glm::mat4x4& projection)
		{
			glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, glm::value_ptr(projection));
		}
	};
}