#pragma once
#include <C78E/Graphics/API/Buffer/StagingBuffer.h>

namespace C78E {

	class VulkanStagingBuffer : public StagingBuffer {
	public:
		VulkanStagingBuffer(size_t size);
		virtual ~VulkanStagingBuffer();

		virtual bool isMapped() const override;
		virtual void map() override;
		virtual void unmap(bool writeBack = false) override;

		virtual void* data() override;
		virtual size_t size() override;

	protected:

	};

}
