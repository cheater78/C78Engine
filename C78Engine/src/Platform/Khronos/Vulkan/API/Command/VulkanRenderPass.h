#pragma once
#include <C78E/Graphics/API/Command/RenderPass.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanFrameBuffer.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipeline.h>


namespace C78E {

	class VulkanRenderPass : public RenderPass {
	public:
		VulkanRenderPass(GraphicsContext& ctx);
		virtual ~VulkanRenderPass();

	public:
		VkRenderPass getVkRenderPass() const {
			return m_VkRenderPass;
		}

	protected:
		Ref<VulkanDevice> m_Device;
		VkRenderPass m_VkRenderPass;
		// VkRenderPassBeginInfo - VkRenderPassCreateInfo

	};

} // namespace C78E::Renderer
