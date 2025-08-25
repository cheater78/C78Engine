#include "C78EPCH.h"
#include "GraphicsInstance.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsInstance.h>

namespace C78E {
	
	Ref<GraphicsInstance> GraphicsInstance::s_GraphicsInstance;

	void GraphicsInstance::create(API api) {
		switch (api) {
		case API::Vulkan:
			s_GraphicsInstance = createRef<VulkanGraphicsInstance>();
			return;
		default:
			C78E_CORE_ASSERT(false, "GraphicsInstance::create: Unsupported API type!");
			return;
		}
	}

	API GraphicsInstance::api() {
		C78E_CORE_ASSERT(s_GraphicsInstance, "GraphicsInstance::api: GraphicsInstance is not created yet!");
		return s_GraphicsInstance->getAPIType();
	}

}
