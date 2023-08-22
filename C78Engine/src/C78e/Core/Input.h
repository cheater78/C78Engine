#pragma once

#include "C78e/Core/Core.h"
#include "C78e/Core/KeyCodes.h"
#include "C78e/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace C78e {

	class C78_API Input
	{
	public:
		static bool isKeyPressed(KeyCode key);

		static bool isMouseButtonPressed(MouseCode button);
		static glm::vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}
