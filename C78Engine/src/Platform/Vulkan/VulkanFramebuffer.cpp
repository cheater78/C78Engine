#include "C78EPCH.h"
#include "VulkanFramebuffer.h"

namespace C78E {

	static const uint32_t s_MaxFramebufferSize = 8192;

	VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec) {
	}

	VulkanFramebuffer::~VulkanFramebuffer() {
	}

	void VulkanFramebuffer::bind() {
	}

	void VulkanFramebuffer::unbind() {
	}

	void VulkanFramebuffer::resize(uint32_t width, uint32_t height) {
	}

	uint32_t VulkanFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y) {

	}

	void VulkanFramebuffer::clearAttachment(uint32_t attachmentIndex, int value) {
	}

	Ref<Texture2D> VulkanFramebuffer::getColorAttachment(uint32_t id) {
	}

}
