#include "C78EPCH.h"
#include "VulkanRenderPass.h"

#include <Platform/Khronos/Vulkan/Core/VulkanConversions.h>
#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanCommandBuffer.h>

namespace C78E {

    static VkSampleCountFlagBits toVkSampleCountFlagBits(uint32_t samples) {
        switch (samples) {
        case 1: return VK_SAMPLE_COUNT_1_BIT;
        case 2: return VK_SAMPLE_COUNT_2_BIT;
        case 4: return VK_SAMPLE_COUNT_4_BIT;
        case 8: return VK_SAMPLE_COUNT_8_BIT;
        case 16: return VK_SAMPLE_COUNT_16_BIT;
        case 32: return VK_SAMPLE_COUNT_32_BIT;
        case 64: return VK_SAMPLE_COUNT_64_BIT;
        default:
            C78E_CORE_WARN("Sample Count was not a power of 2 or <64! (it was {}) -> using 1x", samples);
            return VK_SAMPLE_COUNT_1_BIT;
        }
    }

    static VkAttachmentDescription toVkAttachmentDescription(const FrameBufferAttachmentSpecification& framebufferAttachmentSpecification, uint32_t samples) {
        VkAttachmentDescription attachmentDescription{};
        attachmentDescription.format = toVkFormat(framebufferAttachmentSpecification.format);
        attachmentDescription.samples = toVkSampleCountFlagBits(samples);
        // TODO: set for framebufferAttachmentSpecification
        attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // TODO: change based on usage
        return attachmentDescription;
    }

    static VkRenderPassCreateInfo toVkRenderPassCreateInfo(
        const std::vector<VkAttachmentDescription> attachmentDescription,
        const std::vector<VkSubpassDescription> subPassDescription,
        const std::vector<VkSubpassDependency> subPassDependencies) {
        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.pNext = VK_NULL_HANDLE;
        renderPassCreateInfo.flags = 0;
        renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentDescription.size());
        renderPassCreateInfo.pAttachments = attachmentDescription.data();
        renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subPassDescription.size());
        renderPassCreateInfo.pSubpasses = subPassDescription.data();
        renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(subPassDependencies.size());
        renderPassCreateInfo.pDependencies = subPassDependencies.data();
        return renderPassCreateInfo;
    }


	VulkanRenderPass::VulkanRenderPass(GraphicsContext& ctx)
        : RenderPass(ctx) {

		VulkanGraphicsContext& vulkanContext = ctx.getAs<VulkanGraphicsContext>();
        m_Device = vulkanContext.getDevice();
        
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = toVkFormat(ImageFormat::ABGR8);
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VkResult renderPassCreateResult = vkCreateRenderPass(m_Device->getVkDevice(), &renderPassInfo, nullptr, &m_VkRenderPass);
        C78E_CORE_ASSERT(renderPassCreateResult == VK_SUCCESS, "VulkanRenderPass::VulkanRenderPass: Failed to create VkRenderpass!");
	}

    VulkanRenderPass::~VulkanRenderPass() {
        if (m_VkRenderPass) {
            vkDestroyRenderPass(m_Device->getVkDevice(), m_VkRenderPass, nullptr);
        }
    }

} // namespace C78E::Renderer
