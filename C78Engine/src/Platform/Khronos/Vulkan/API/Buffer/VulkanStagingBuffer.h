#pragma once
#include <C78E/Graphics/API/Buffer/StagingBuffer.h>
#include <Platform/Khronos/Vulkan/Core/VulkanGraphicsContextItem.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanGPUBuffer.h>

namespace C78E {

	class VulkanStagingBuffer : public VulkanGraphicsContextItem, public VulkanGPUBuffer, public StagingBuffer {
	public:
		VulkanStagingBuffer(GraphicsContext& ctx, size_t size);
		virtual ~VulkanStagingBuffer();

		virtual bool isMapped() const override;
		virtual bool map() override;
		virtual void unmap(bool writeBack = false) override;

		virtual void* data() override;
		virtual size_t size() override;

		bool alive() override { return false; }
		virtual void free() override { }

	protected:
		Ref<VulkanDevice> m_Device;
	};

}
