#pragma once

#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "../entities/Light.hpp"
#include "../model/Mesh.hpp"

namespace xplor
{
	class LightingShader : public Shader
	{
		unsigned int dirLightDirection;
		unsigned int spotLightDirLocation;
		unsigned int cameraPosVecLocation;
		unsigned int inverseModelMatLocation;
		unsigned int modelMatLocation;
		unsigned int viewMatLocation;
		unsigned int projectionMatLocation;

	public:

		LightingShader() {}

		LightingShader(const glm::mat4x4 projection)
		{
			Init(std::string("res/shaders/lighting.vs").c_str(), std::string("res/shaders/lighting.fs").c_str());
			setLocations();
			bind();
			setSamplers();
			glm::mat4 thisProjection = glm::mat4(1.0f); // only needs to change when window changes
			thisProjection = projection;
			setProjectionMatrix(projection);
		}

		inline void setLocations()
		{
			dirLightDirection = glGetUniformLocation(ID, "dirLight.direction");
			spotLightDirLocation = glGetUniformLocation(ID, "spotLight.direction");
			cameraPosVecLocation = glGetUniformLocation(ID, "camPos");
			inverseModelMatLocation = glGetUniformLocation(ID, "inverseModel");
			modelMatLocation = glGetUniformLocation(ID, "model");
			viewMatLocation = glGetUniformLocation(ID, "view");
			projectionMatLocation = glGetUniformLocation(ID, "projection");
		}

		inline void setSamplers()
		{
			setInt("material.diffuse", 0);
			setInt("material.specular", 1);
		}

		void setDirectionalLight(const glm::vec3& directionalLight)
		{
			glUniform3fv(dirLightDirection, 1, glm::value_ptr(directionalLight));
		}

		// Point Light IDs are 0-4.
		void setPointLight(const light::PointLight& light)
		{
			if (light.ID <= 4 || light.ID >= 0)
			{
				setBool("pointLights[" + std::to_string(light.ID) + "].activated", true);
				setVec3("pointLights[" + std::to_string(light.ID) + "].position", light.position);
				setVec3("pointLights[" + std::to_string(light.ID) + "].diffuse", light.diffuse);
			}
			else
			{
				printf("ERROR::POINT_LIGHT_ID_OUT_OF_BOUNDS\n");
			}
		}

		void setSpotLightActivated(const bool activated)
		{
			setBool("spotLight.activated", activated);
		}

		void setSpotLightDirection(const glm::vec3& direction)
		{
			glUniform3fv(spotLightDirLocation, 1, glm::value_ptr(direction));
		}

		void setCameraPosVec(const glm::vec3& position)
		{
			glUniform3fv(cameraPosVecLocation, 1, glm::value_ptr(position));
		}

		inline void setInverseModelMatrix(const glm::mat3x3& model)
		{
			glm::mat3x3 inverseModel = glm::transpose(glm::inverse(model));
			glUniformMatrix3fv(inverseModelMatLocation, 1, GL_FALSE, glm::value_ptr(inverseModel));
		}

		// Should be called every frame.
		void setModelMatrix(const glm::mat4x4& model)
		{
			glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
			setInverseModelMatrix(glm::mat3x3(model));
		}

		// Should change when camera moves.
		void setViewMatrix(const glm::mat4x4& view)
		{
			glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, glm::value_ptr(view));
		}

		// Should only change when changing fov, aspect ratio, and clipping distance.
		void setProjectionMatrix(const glm::mat4x4& projection)
		{
			glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, glm::value_ptr(projection));
		}
	};
}