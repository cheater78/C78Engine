#pragma once
#include <C78E/Graphics/Window/WindowSystem.h>
#include "GLFWMonitor.h"

namespace C78E {

	class GLFWWindowSystem : public WindowSystem {
	public:
		GLFWWindowSystem();
		virtual ~GLFWWindowSystem();

		virtual void waitEvents() override;
		virtual void callEvents() override;

		virtual void fetchMonitors() override;

	private:

	};

}
