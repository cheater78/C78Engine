#pragma once
#include <C78E/Core/Window/Window.h>

namespace C78E::Renderer {

	//TODO: just a dummy rn
	class GraphicsContext {
	public:
		static Scope<GraphicsContext> create(Window& window) {
			return Scope<GraphicsContext>();
		}

	public:
		GraphicsContext() = default;
		~GraphicsContext() = default;


	};
}