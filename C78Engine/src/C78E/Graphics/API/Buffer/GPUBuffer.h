#pragma once
#include <C78E/Graphics/Core/GraphicsContextItem.h>

namespace C78E {

	class GPUBuffer : public virtual GraphicsContextItem {
	public:
		GPUBuffer() = default;
		virtual ~GPUBuffer() = default;

		virtual bool alive() = 0;
		virtual void free() = 0;
	};

}
