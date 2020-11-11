#include "Keyboard.h"

namespace input
{
	Keyboard::Keyboard() {
		//Init the arrays with false values
		//Zero is for non supported keys
		const unsigned short qtdKeys = 12;
		pressedKeys.reserve(qtdKeys); pressedKeys.assign(qtdKeys, false);
		clickedKeys.reserve(qtdKeys); clickedKeys.assign(qtdKeys, false);
		clickedFunction.reserve(qtdKeys); clickedFunction.assign(qtdKeys, [](){});
	}
	Keyboard::~Keyboard() {}

	void Keyboard::handleInputs(GLFWwindow* window, int key, int scancode, int action, int mods) {
		switch (action) {

			case GLFW_PRESS:
				pressedKeys[keyAssociated(key)] = true;

				if (clickedFunction[keyAssociated(key)] && !clickedKeys[keyAssociated(key)]) {
					clickedKeys[keyAssociated(key)] = true;
					clickedFunction[keyAssociated(key)]();
				}
				clickedFunction[keyAssociated(key)] = []() {};
			break;

			case GLFW_RELEASE:
				pressedKeys[keyAssociated(key)] = false;
				clickedKeys[keyAssociated(key)] = false;
			break;
		}
	}

	bool Keyboard::isPress(const ky& k)
	{
		return clickedKeys[k];
	}

	void Keyboard::clicked(const ky& k, const std::function<void()>& func)
	{
		clickedFunction[k] = func;
	}

	int Keyboard::keyAssociated(int key)
	{
		switch (key) {
		case GLFW_KEY_RIGHT:
			return (int)ARROW_RIGHT;
			break;
		case GLFW_KEY_LEFT:
			return (int)ARROW_LEFT;
			break;
		case GLFW_KEY_UP:
			return (int)ARROW_UP;
			break;
		case GLFW_KEY_DOWN:
			return (int)ARROW_DOWN;
			break;
		case GLFW_KEY_W:
			return (int)W_KEY;
			break;
		case GLFW_KEY_S:
			return (int)S_KEY;
			break;
		case GLFW_KEY_SPACE:
			return (int)SPACEBAR;
			break;
		case GLFW_KEY_KP_ADD:
			return (int)PLUS_KEY;
			break;
		case GLFW_KEY_KP_SUBTRACT:
			return (int)MINUS_KEY;
			break;
		case GLFW_KEY_ESCAPE:
			return (int)ESC_KEY;
			break;
		case GLFW_KEY_F11:
			return (int)F11_KEY;
			break;
		default:
			return 0;
			break;

		}
	}
}