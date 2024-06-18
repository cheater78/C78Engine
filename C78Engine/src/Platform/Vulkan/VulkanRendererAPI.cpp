#include "C78EPCH.h"
#include "VulkanRendererAPI.h"

namespace C78E {

	void VulkanRendererAPI::init() {
	}

	uint32_t VulkanRendererAPI::getMaxTextureSlots(ShaderType stage) {
	}

	void VulkanRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	}

	void VulkanRendererAPI::setClearColor(const glm::vec4& color) {
	}

	void VulkanRendererAPI::clear() {
	}

	void VulkanRendererAPI::enableDepthTest(bool enable) {
	}

	void VulkanRendererAPI::setDepthFunc(DepthFunc func) {
	}

	void VulkanRendererAPI::setDepthWrite(bool enable) {
	}

	void VulkanRendererAPI::drawIndexed(const Ref<VertexArray> vertexArray, uint32_t indexCount) {
	}

	void VulkanRendererAPI::drawLines(const Ref<VertexArray> vertexArray, uint32_t vertexCount) {
	}

	void VulkanRendererAPI::setLineWidth(float width) {
	}

}
