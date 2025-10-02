#pragma once
#include <C78E/Graphics/API/Buffer/GPUBuffer.h>
#include <Platform/Khronos/Vulkan/Core/Buffer/VulkanBuffer.h>

namespace C78E {

	class VulkanGPUBuffer : public virtual GPUBuffer {
	public:
		VulkanGPUBuffer() = default;
		virtual ~VulkanGPUBuffer() = default;

		virtual bool alive() = 0;
		virtual void free() = 0;
	public:
		VulkanBuffer& getVulkanBuffer() const;
	protected:
		Scope<VulkanBuffer> m_VulkanBuffer = nullptr;
	};

}
