#include "C78EPCH.h"
#include "VulkanGPUBuffer.h"

namespace C78E {

	VulkanBuffer& VulkanGPUBuffer::getVulkanBuffer() const {
		C78E_CORE_ASSERT(m_VulkanBuffer, "VulkanGPUBuffer::getVulkanBuffer: m_VulkanBuffer was nullptr!");
		return *m_VulkanBuffer;
	}

}
