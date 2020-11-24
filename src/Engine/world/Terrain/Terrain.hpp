#pragma once
  
#include <stb_image.h>

#include <array>
#include <memory>

#include "../../renderer/model/Model.hpp"
#include "../../renderer/shader/TerrainShader.hpp"
#include "TerrainMeshGenerator.hpp"

namespace xplor
{
	namespace terrain
	{
		struct Terrain
		{
			TerrainModel model;

			glm::mat4 transformationMatrix = glm::mat4();
			glm::vec3 position = glm::vec3(0);
			float scale = 1;

			const float SCALE = 100;
			const int UV_MULTIPLYER = 75;
			unsigned int vertex_count = 0; // vertices width or height

			std::shared_ptr<float[]> heights;

		public:

			Terrain() {}

			// Paths of different multitextured diffuse maps used in the blendmap.
			// Image file name in "res/terrain" folder
			void generateTerrain(std::string backDiffuseMap, std::string rDiffuseMap, std::string gDiffuseMap, std::string bDiffuseMap);

			void render(TerrainShader& shader);

			void updateTerrainMesh();

			// Rotation in radians.
			void updateTransformationMatrix(const glm::vec3& position, const glm::vec3& rotation, float scale);
			void updateTransformationMatrix();

			void generateMesh();
				
			inline glm::vec3 calculateNormal(glm::ivec2 vertex);
			inline float getHeight(glm::ivec2 position);

			std::vector<unsigned int> loadImage(std::string path, unsigned int& vertex_count);
		};
	}
}