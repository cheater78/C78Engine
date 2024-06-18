#include "C78EPCH.h"
#include "VulkanGraphicsContext.h"

namespace C78E {

	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle) {
		C78_CORE_ASSERT(m_WindowHandle, "VulkanGraphicsContext::VulkanGraphicsContext: m_WindowHandle handle is null!");
	}

	void VulkanGraphicsContext::init() {
	}

	void VulkanGraphicsContext::swapBuffers() {
	}

}
