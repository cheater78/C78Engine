#pragma once

#include "C78E/Core/Core.h"
#include "C78E/Core/KeyCodes.h"
#include "C78E/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace C78E {

	class C78_API Input
	{
	public:
		static bool isKeyPressed(KeyCode key);

		static bool isMouseButtonPressed(MouseCode button);
		static glm::vec2 getMousePosition();
		static void setMousePosition(glm::vec2 position) { setMousePosition(position.x, position.y); }
		static void setMousePosition(float x, float y);
		static float getMouseX();
		static float getMouseY();
	};
}
