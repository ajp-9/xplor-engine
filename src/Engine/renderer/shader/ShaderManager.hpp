#pragma once

#include <memory>
#include <vector>

#include "Shader.hpp"
#include "LightingShader.hpp"
#include "TerrainShader.hpp"

namespace xplor
{
	struct ShaderManager
	{
		int current_shader = -1;

		LightingShader lighting;
		terrain::TerrainShader terrain;

	public:

		ShaderManager() {}

		void Init(const glm::mat4x4 projection);

		void bindShader(unsigned char shader);

		void setDirectionalLight(const glm::vec3& directionalLight);
		void setPointLight(const light::PointLight& light);
		void setSpotLightActivated(const bool activated);

		void setSpotLightDirection(const glm::vec3& direction);

		void setCameraPosVec(const glm::vec3& position);
		// Should be called every frame.
		void setModelMatrix(const glm::mat4x4& model);
		// Should change when camera moves.
		void setViewMatrix(const glm::mat4x4& view);
		// Should only change when changing fov, aspect ratio, and clipping distance.
		void setProjectionMatrix(const glm::mat4x4& projection);
	};
}