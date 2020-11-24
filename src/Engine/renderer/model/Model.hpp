#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "texture/Texture.hpp"
#include "../shader/ShaderManager.hpp"

#include "Mesh.hpp"

namespace xplor
{
	namespace terrain
	{
		struct TerrainModel
		{
			TerrainMesh mesh;
			TerrainMaterial material;

		public:

			TerrainModel() {}
			TerrainModel(TerrainMesh mesh, TerrainMaterial material)
				: mesh(mesh), material(material)
			{}

			void draw(TerrainShader& shader);
		};
	}

	class Model
	{
		std::vector<Mesh> meshes;
		Material material;

	public:

		std::string name;

		Model() {}

		// Path to the folder "res/models/{object name}/{name}.obj/.jpg"
		Model(const std::string& name);

		void draw(LightingShader& shader);

	private:

		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	};
}