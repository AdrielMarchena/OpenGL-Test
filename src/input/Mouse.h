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
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			mouse_pos_x = xpos - lastX;
			mouse_pos_y = lastY - ypos; // reversed since y-coordinates go from bottom to top

			lastX = xpos;
			lastY = ypos;
		}
	private:
		float lastX = 960.0f / 2.0f;
		float lastY = 540.0f / 2.0f;
		bool firstMouse = true;
	};
}