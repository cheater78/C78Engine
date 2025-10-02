#pragma once
#include <C78E/Graphics/Core/GraphicsContextItem.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	class VulkanGraphicsContext;

	class VulkanGraphicsContextItem : public virtual GraphicsContextItem {
	public:
		VulkanGraphicsContextItem();
		virtual ~VulkanGraphicsContextItem();

	public:
		VulkanGraphicsContext& getVulkanGraphicsContext() const;
		Ref<VulkanDevice> getVulkanDevice() const;
		VkDevice getVkDevice() const;
	protected:
		Ref<VulkanDevice> m_Device;
	};

}
