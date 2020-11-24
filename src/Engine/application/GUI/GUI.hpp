#pragma once

namespace xplor
{
	namespace gui
	{
		struct GUI
		{
			bool active = true;

			virtual void update(std::vector<int>& events) = 0;
		};
	}
}