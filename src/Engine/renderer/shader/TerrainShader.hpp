#pragma once

#include "LightingShader.hpp"

namespace xplor
{
	namespace terrain
	{
		struct TerrainShader : public LightingShader
		{
			TerrainShader() {}
			TerrainShader(const glm::mat4x4 projection)
			{
				Init(std::string("res/shaders/terrain_lighting.vs").c_str(), std::string("res/shaders/terrain_lighting.fs").c_str());
				setLocations();
				bind();
				setTerrainSamplers();
				glm::mat4 thisProjection = glm::mat4(1.0f); // Only needs to change when window changes.
				thisProjection = projection;
				setProjectionMatrix(projection);
			}

			inline void setTerrainSamplers()
			{
				setInt("material.blendMap", 0);
				setInt("material.backDiffuseMap", 1);
				setInt("material.rDiffuseMap", 2);
				setInt("material.gDiffuseMap", 3);
				setInt("material.bDiffuseMap", 4);
			}
		};
	}
}