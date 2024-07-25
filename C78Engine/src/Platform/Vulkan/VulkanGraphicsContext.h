#pragma once
#include <C78E/Renderer/API/GraphicsContext.h>


#include "Core/VulkanDevice.h"

struct GLFWwindow;

namespace C78E {

	class VulkanGraphicsContext : public GraphicsContext {
	public:
		VulkanGraphicsContext(Window& window);

		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* getGLFWwindow() const;
	private:
		Window& m_Window;

	};

}