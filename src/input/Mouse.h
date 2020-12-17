#pragma once
#include "GLFW/glfw3.h"

namespace input
{
	class Mouse
	{
	public:
		static double mouse_pos_x;
		static double mouse_pos_y;

		Mouse() {};
		~Mouse() {};

		void handleInput(double xpos, double ypos)
		{
			mouse_pos_x = xpos;
			mouse_pos_y = ypos;
		}
	private:
	};
}