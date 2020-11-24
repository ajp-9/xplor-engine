#include "Model.hpp"

#include <glad/glad.h>

#include <iostream>
#include <fstream>

#include "../../utils/Math.hpp"

namespace xplor
{
	void terrain::TerrainModel::draw(TerrainShader& shader)
	{
		glActiveTexture(GL_TEXTURE0);
		material.blendMap.bind();
		glActiveTexture(GL_TEXTURE1);
		material.backDiffuseMap.bind();
		glActiveTexture(GL_TEXTURE2);
		material.rDiffuseMap.bind();
		glActiveTexture(GL_TEXTURE3);
		material.gDiffuseMap.bind();
		glActiveTexture(GL_TEXTURE4);
		material.bDiffuseMap.bind();

		mesh.draw();
	}

	Model::Model(const std::string& name)
	{
		this->name = name;
		// read file via ASSIMP
		Assimp::Importer importer;
		std::ifstream finder;
		finder.open("res/models/" + name + "/" + name + ".obj");
		if (finder)
		{
			const aiScene* scene = importer.ReadFile("res/models/" + name + "/" + name + ".obj", aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices);

			// check for errors
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
				return;
			}

			// process ASSIMP's root node recursively
			processNode(scene->mRootNode, scene);
		}
		else
		{
			std::cout << "ERROR::OBJ_NOT_FOUND" << std::endl;
		}

		finder.close();
		finder.open("res/models/" + name + "/" + name + ".jpg");
		if (finder)
		{
			material.diffuseMap = Texture("res/models/" + name + "/" + name + ".jpg");
		}
		else
		{
			finder.close();
			finder.open("res/models/" + name + "/" + name + ".png");
			if (finder)
			{
				material.diffuseMap = Texture("res/models/" + name + "/" + name + ".png");
			}
			else
			{
				std::cout << "ERROR::DIFFUSE_MAP_NOT_FOUND" << std::endl;
			}
		}
		finder.close();
		finder.open("res/models/" + name + "/" + name + "_specular.jpg");
		if (finder)
		{
			material.specularMap = Texture("res/models/" + name + "/" + name + "_specular.jpg");
			material.specular = true;
		}
		else
		{
			finder.close();
			finder.open("res/models/" + name + "/" + name + "_specular.png");
			if (finder)
			{
				material.specularMap = Texture("res/models/" + name + "/" + name + "_specular.png");
				material.specular = true;
			}
			else
			{
				//std::cout << "ERROR::SPECULAR_MAP_NOT_FOUND" << std::endl;
			}
		}
		finder.close();
	}
	 
	void Model::draw(LightingShader& shader)
	{
		glActiveTexture(GL_TEXTURE0);
		material.diffuseMap.bind();

		glActiveTexture(GL_TEXTURE1);
		if (material.specular)
			material.specularMap.bind();
		else
			glBindTexture(GL_TEXTURE_2D, 0); // unbinding last specular map

		for (auto& m : meshes)
			m.draw();
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// thru each vertex
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // temp vec
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
			// texture coordinates

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.uv = vec;
			}
			else
			{
				vertex.uv = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		return Mesh(vertices, indices);
	}
}