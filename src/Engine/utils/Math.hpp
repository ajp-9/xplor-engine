#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace xplor
{
	struct Math
	{
		static glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, float scale);
	};
}