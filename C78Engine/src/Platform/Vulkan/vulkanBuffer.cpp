#include "C78EPCH.h"
#include "VulkanBuffer.h"

namespace C78E {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) {
	}

	VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size) {
	}

	VulkanVertexBuffer::~VulkanVertexBuffer() {
	}

	void VulkanVertexBuffer::bind() const {
	}

	void VulkanVertexBuffer::unbind() const {
	}

	void VulkanVertexBuffer::setData(const void* data, uint32_t size) {
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count) {
	}

	VulkanIndexBuffer::~VulkanIndexBuffer() {
	}

	void VulkanIndexBuffer::bind() const {
	}

	void VulkanIndexBuffer::unbind() const {
	}

}
