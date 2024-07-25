#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include "glm/glm.hpp"

#include "vulkanDevice.h"
#include "vulkanBuffer.h"
#include "types.h"

#include <memory>
#include <vector>

namespace C78E {

	class VulkanModel {

	public:

		VulkanModel(VulkanDevice& device, std::vector<Vertex>& vertecies, std::vector<uint32_t>& indecies);
		~VulkanModel();
		VulkanModel(const VulkanModel&) = delete;
		VulkanModel& operator=(const VulkanModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		VulkanDevice& m_Device;

		std::unique_ptr<VulkanBuffer> m_VertexBuffer;
		uint32_t m_VertexCount;

		std::unique_ptr<VulkanBuffer> m_IndexBuffer;
		uint32_t m_IndexCount = 0;
		bool hasIndexBuffer() { return m_IndexCount > 0; }
		
		void createBuffers(std::vector<Vertex>& vertecies, std::vector<uint32_t>& indecies);
	};

}