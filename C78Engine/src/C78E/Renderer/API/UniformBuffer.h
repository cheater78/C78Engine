#pragma once

#include "C78E/Core/Core.h"

namespace C78E {

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		virtual void bind(uint32_t binding) = 0;
		
		static Ref<UniformBuffer> create(uint32_t size);
		static Ref<UniformBuffer> create(uint32_t size, uint32_t binding);
	};

}
