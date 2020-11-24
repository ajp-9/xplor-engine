#pragma once

struct World
{
	std::shared_ptr<xplor::terrain::Terrain> terrain = std::make_shared<xplor::terrain::Terrain>();

	std::vector<xplor::light::PointLight> pointLights;
	std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>> entities = std::make_shared<std::vector<std::shared_ptr<xplor::Entity>>>();

	xplor::light::DirectionalLight sun = xplor::light::DirectionalLight(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.05f), glm::vec3(1.0f));

	World() {}

	void loadWorld()
	{

	}

	void renderWorld(xplor::ShaderManager& shader_manager, ModelManager& model_manager, Player& player)
	{
		shader_manager.terrain.bind();
		terrain->render(shader_manager.terrain);
		shader_manager.setSpotLightDirection(player.front);

		shader_manager.lighting.bind();
		for (auto& e : *entities)
			e->render(shader_manager.lighting, model_manager);

		glm::mat4 view = player.getViewMatrix();
		shader_manager.setViewMatrix(view);
	}
};