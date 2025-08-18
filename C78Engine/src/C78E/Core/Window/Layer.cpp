#include "C78EPCH.h"
#include "Layer.h"

#include <C78E/Core/Window/Window.h>

namespace C78E {

	Layer::Layer(Window& window, const std::string& name)
		: m_Window(window), m_Name(name) {
	}
	
}