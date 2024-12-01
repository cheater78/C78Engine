#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Input/KeyCodes.h>
#include <C78E/Core/Input/MouseCodes.h>
#include <C78E/Core/Input/KeyCombo.h>
#include <C78E/Utils/MathUtils.h>

namespace C78E {

	namespace Input {
		
		bool isKeyPressed(KeyCode key);
		bool isMouseButtonPressed(MouseCode button);
		glm::vec2 getMousePosition();
		void setMousePosition(float x, float y);
		void setMousePosition(glm::vec2 position);

	};
}
