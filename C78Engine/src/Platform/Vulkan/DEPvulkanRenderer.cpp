#include "vulkanRenderer.h"

#include <stdexcept>
#include <string>
#include <array>

namespace C78E {

	VulkanRenderer::VulkanRenderer(VulkanWindow& window, VulkanDevice& device) : m_Window{ window }, m_Device{ device } {
		recreateSwapChain();
		createCommandBuffers();
	}

	VulkanRenderer::~VulkanRenderer() {
		freeCommandBuffers();
	}

	void VulkanRenderer::recreateSwapChain() {
		auto extent = m_Window.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = m_Window.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(m_Device.device());
		if (m_SwapChain == nullptr) {
			m_SwapChain = std::make_unique<VulkanSwapChain>(m_Device, extent);
		}
		else {
			shared_ptr<VulkanSwapChain> oldSwapChain = move(m_SwapChain);
			m_SwapChain = std::make_unique<VulkanSwapChain>(m_Device, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*m_SwapChain.get())) {
				throw runtime_error("SwapChain image or depth format has changed");
			}
		}

	}

	void VulkanRenderer::createCommandBuffers() {
		m_CommandBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_Device.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS) {
			throw runtime_error("failed to alloc cmd buffs");
		}

	}

	void VulkanRenderer::freeCommandBuffers() {
		vkFreeCommandBuffers(m_Device.device(), m_Device.getCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	VkCommandBuffer VulkanRenderer::beginFrame() {
		assert(!m_IsFrameStarted && "cant beginFrame, alr in Prog");

		auto result = m_SwapChain->acquireNextImage(&m_CurrentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw runtime_error("failed to acqu swan chain img");
		}

		m_IsFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw runtime_error("failed to begin rec cmd buff");
		}

		return commandBuffer;
	}

	void VulkanRenderer::endFrame() {
		assert(m_IsFrameStarted && "cant endFrame, not in Prog");

		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw runtime_error("failed to stop rec cmd buff");
		}

		VkResult result = m_SwapChain->submitCommandBuffers(&commandBuffer, &m_CurrentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window.wasWindowResized()) {
			m_Window.resetWindowResizedFlag();
			recreateSwapChain();
		}else if (result != VK_SUCCESS) {
			throw runtime_error("failed to present swap chain img" + to_string(result));
		}

		m_IsFrameStarted = false;
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(m_IsFrameStarted && "cant beginm_SwapChainRenderPass, not in Prog");
		assert(commandBuffer == getCurrentCommandBuffer() && "cant begin render pass on cmd buf from diff frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(m_CurrentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

		array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f,0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	}

	void VulkanRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(m_IsFrameStarted && "cant endm_SwapChainRenderPass, not in Prog");
		assert(commandBuffer == getCurrentCommandBuffer() && "cant end render pass on cmd buf from diff frame");

		vkCmdEndRenderPass(commandBuffer);
	}

}