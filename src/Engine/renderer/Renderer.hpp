#pragma once

#include <glm/mat4x4.hpp>

#include "shader/Shader.hpp"
#include "shader/ShaderManager.hpp"

struct Renderer
{
	xplor::ShaderManager shaderManager;

	void initRenderer();

	void renderAll(xplor::Shader& shader, glm::mat4 view, int fov, int width, int height);

	//~Renderer();
};