#include "Terrain.hpp"

#include <iostream>

#include "../../utils/Math.hpp"

namespace xplor
{

	namespace terrain
	{
		void Terrain::generateTerrain(std::string backDiffuseMap, std::string rDiffuseMap, std::string gDiffuseMap, std::string bDiffuseMap)
		{
			{
				std::vector<unsigned int> image = loadImage("res/terrain/heights.png", vertex_count); // only pngs

				heights = std::shared_ptr<float[]>(new float[vertex_count * vertex_count]);

				unsigned int count = 0;
				for (unsigned int y = 0; y < vertex_count; y++)
				{
					for (unsigned int x = 0; x < vertex_count; x++)
					{
						heights[x + y * vertex_count] = float(image[count]);
						count += 4;
					}
				}
			}

			generateMesh();

			model.material.blendMap = Texture("res/terrain/blendMap.png");
			model.material.backDiffuseMap = Texture("res/terrain/" + backDiffuseMap);
			model.material.rDiffuseMap = Texture("res/terrain/" + rDiffuseMap);
			model.material.gDiffuseMap = Texture("res/terrain/" + gDiffuseMap);
			model.material.bDiffuseMap = Texture("res/terrain/" + bDiffuseMap);

			updateTransformationMatrix(position, glm::vec3(0), 1);
		}

		void Terrain::render(TerrainShader& shader)
		{
			shader.setModelMatrix(transformationMatrix);
			model.draw(shader);
		}

		void Terrain::updateTerrainMesh() {}
		   
		void Terrain::updateTransformationMatrix(const glm::vec3& position, const glm::vec3& rotation, float scale)
		{
			this->position = position; this->scale = scale;
			transformationMatrix = Math::createTransformationMatrix(position, rotation, scale);
		}

		void Terrain::updateTransformationMatrix()
		{
			transformationMatrix = Math::createTransformationMatrix(position, glm::vec3(0), scale);
		}

		void Terrain::generateMesh()
		{
			// putting things in scope so it can quickly free memory
			try
			{
				{
					std::vector<Vertex> vertices;
					vertices.resize(vertex_count * vertex_count);

					unsigned int vIterator = 0;
					for (unsigned int y = 0; y < vertex_count; y++)
					{
						for (unsigned int x = 0; x < vertex_count; x++)
						{
							vertices[vIterator++] = xplor::Vertex
							(
								glm::vec3(-float(x) / (float(vertex_count) - 1) * SCALE, -float(y) / (float(vertex_count) - 1) * SCALE, getHeight(glm::ivec2(x, y))),
								glm::vec2(float(x) / (float(vertex_count) - 1) * UV_MULTIPLYER, float(y) / (float(vertex_count) - 1) * UV_MULTIPLYER),
								calculateNormal(glm::ivec2(x, y))
							);
						}
					}

					model.mesh.setVertices(vertices);
				}

				{
					std::vector<unsigned int> indices;
					indices.resize(6 * (vertex_count - 1) * (vertex_count * 1));

					unsigned int iIterator = 0;
					for (unsigned int gz = 0; gz < vertex_count - 1; gz++) {
						for (unsigned int gx = 0; gx < vertex_count - 1; gx++) {
							unsigned int topLeft = (gz * vertex_count) + gx;
							unsigned int topRight = topLeft + 1;
							unsigned int bottomLeft = ((gz + 1) * vertex_count) + gx;
							unsigned int bottomRight = bottomLeft + 1;
							indices[iIterator++] = topRight;
							indices[iIterator++] = bottomLeft;
							indices[iIterator++] = topLeft;
							indices[iIterator++] = bottomRight;
							indices[iIterator++] = bottomLeft;
							indices[iIterator++] = topRight;
						}
					}

					model.mesh.setIndices(indices);
				}

				model.mesh.finish();
			}
			catch (const std::bad_alloc& e)
			{
				std::cout << "Allocation for Terrain Mesh Failed: " << e.what() << std::endl;
			}
		}

		inline glm::vec3 Terrain::calculateNormal(glm::ivec2 vertex)
		{
			float heightL = getHeight(glm::ivec2(vertex.x - 1, vertex.y));
			float heightR = getHeight(glm::ivec2(vertex.x + 1, vertex.y));
			float heightD = getHeight(glm::ivec2(vertex.x, vertex.y - 1));
			float heightU = getHeight(glm::ivec2(vertex.x, vertex.y + 1));

			return glm::vec3(heightR - heightL, heightU - heightD, .5);
		}

		inline float Terrain::getHeight(glm::ivec2 position)
		{
			if (position.x < 0 || position.x >= int(vertex_count) ||
				position.y < 0 || position.y >= int(vertex_count))
				return 0;
			else
				return (heights[position.x + position.y * vertex_count] - 127) / 12;
		}

		std::vector<unsigned int> Terrain::loadImage(std::string path, unsigned int& vertex_count)
		{
			int width, height, nrChannels;
			unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4); // maybe change to 0 from 4
			if (data)
			{
				if (width == height)
				{
					vertex_count = width;
					auto heights = std::vector<unsigned int>(data, data + width * height * 4);
					stbi_image_free(data);
					return heights;
				}
				else
				{
					printf("ERROR::WIDTH_NOT_EQUAL_TO_HEIGHT");
					stbi_image_free(data);
					return std::vector<unsigned int>(0);
				}
			}
			else
			{
				printf("ERROR::LOADING_HEIGHT_MAP");
				stbi_image_free(data);
				return std::vector<unsigned int>(0);
			}
		}
	}
}