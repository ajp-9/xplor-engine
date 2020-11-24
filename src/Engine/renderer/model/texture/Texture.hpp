#pragma once

#include <string>

namespace xplor
{
	struct Texture
	{
		unsigned int ID;

		Texture() { ID = -1; }
		Texture(std::string path);

		void bind();

		~Texture();
	};
}