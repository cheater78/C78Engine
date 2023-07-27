#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "vulkanWindow.h"
#include "vulkanDevice.h"
#include "vulkanSwapChain.h"

#include <memory>
#include <vector>
#include <cassert>

using namespace std;

namespace C78e {

	class VulkanRenderer {
	public:

		VulkanRenderer(VulkanWindow& window, VulkanDevice& device);
		~VulkanRenderer();

		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
		float getAscpectRatio() const { return m_SwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return m_IsFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(m_IsFrameStarted && "Frame not in Progress, cant get Cmd Buffer");
			return m_CommandBuffers[m_CurrentFrameIndex];
		}

		VkCommandBuffer beginFrame();
		void endFrame();

		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
		
		int getFrameIndex() const {
			assert(m_IsFrameStarted && "Frame not in Progress, cant get FrameIndex");
			return m_CurrentFrameIndex;
		}

	private:

		VulkanWindow& m_Window;
		VulkanDevice& m_Device;
		unique_ptr<VulkanSwapChain> m_SwapChain;
		vector<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_CurrentImageIndex = 0;
		int m_CurrentFrameIndex = 0;
		bool m_IsFrameStarted = false;

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

	};
}