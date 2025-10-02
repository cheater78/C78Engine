#include "C78EPCH.h"
#include "VulkanGraphicsContextItem.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>

namespace C78E {

	VulkanGraphicsContextItem::VulkanGraphicsContextItem() {
		VulkanGraphicsContext& vulkanGraphicsContext = getGraphicsContext().getAs<VulkanGraphicsContext>();
		m_Device = vulkanGraphicsContext.getDevice();
	}

	VulkanGraphicsContextItem::~VulkanGraphicsContextItem() {
	}

	VulkanGraphicsContext& VulkanGraphicsContextItem::getVulkanGraphicsContext() const {
		return getGraphicsContext().getAs<VulkanGraphicsContext>();
	}

	Ref<VulkanDevice> VulkanGraphicsContextItem::getVulkanDevice() const {
		return m_Device;
	}

	VkDevice VulkanGraphicsContextItem::getVkDevice() const {
		return m_Device->getVkDevice();
	}

}
