#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <array>
#include "texture/Texture.hpp"

namespace xplor
{
	struct Material
	{
		Texture diffuseMap;
		Texture specularMap;
		bool specular = false;
		float shininess;
		Material() { shininess = 0; }
		Material(Texture diffuseMap, Texture specularMap, bool specular, float shininess = 0)
			: diffuseMap(diffuseMap), specularMap(specularMap), specular(specular), shininess(shininess)
		{}
	};

	namespace terrain
	{
		struct TerrainMaterial
		{
			Texture blendMap;
			Texture backDiffuseMap;
			Texture rDiffuseMap;
			Texture gDiffuseMap;
			Texture bDiffuseMap;

			TerrainMaterial() {}
			TerrainMaterial(Texture& blendMap, Texture& backDiffuseMap, Texture& rDiffuseMap, Texture& gDiffuseMap, Texture& bDiffuseMap)
				: blendMap(blendMap), backDiffuseMap(backDiffuseMap), rDiffuseMap(rDiffuseMap), gDiffuseMap(gDiffuseMap), bDiffuseMap(bDiffuseMap)
			{}
		};
	}

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;

		Vertex() {}
		Vertex(glm::vec3 position, glm::vec2 uv, glm::vec3 normal)
			: position(position), uv(uv), normal(normal)
		{}
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		unsigned int VAO = -1;
		unsigned int vertexCount = -1;

		Mesh() {}
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

		void draw();

	protected:

		unsigned int VBO = -1, EBO = -1;

		void upload();
	};

	struct TerrainMesh : public Mesh
	{
		void setVertices(std::vector<Vertex>& vertices);
		void setIndices(std::vector<unsigned int>& indices);

		void finish();
	};
}