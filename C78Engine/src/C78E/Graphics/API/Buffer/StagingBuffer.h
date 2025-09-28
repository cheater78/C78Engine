#pragma once
#include <C78E/Core/Buffer/Iterator.h>
#include <C78E/Core/Log/SmartLog.h>

namespace C78E {

	/**
	 * @brief CPU Memory Buffer that holds data, to be uploaded to the GPU
	 */
	class StagingBuffer {
	public:
		static Ref<StagingBuffer> create(size_t size);
	public:
		virtual ~StagingBuffer() = default;

		virtual bool isMapped() const = 0;
		virtual void map() = 0;
		virtual void unmap(bool writeBack = false) = 0;

		virtual void* data() = 0;
		virtual size_t size() = 0;

	public:
		template<typename T>
		MemoryRange<T> range() {
			return MemoryRange<T>(reinterpret_cast<T*>(data()), size() / sizeof(T));
		}

		template<typename T>
		MemoryRange<T> range(size_t firstElementIndex, size_t elementCount = -1) {
			C78E_CORE_ASSERT(firstElementIndex * sizeof(T) < size(), "StagingBuffer::range: firstElementIndex was out of bounds!");
			C78E_CORE_ASSERT(elementCount == -1 || (firstElementIndex + elementCount) * sizeof(T) < size(), "StagingBuffer::range: firstElementIndex was out of bounds!");
			const uint8_t* bufferBegin = reinterpret_cast<const uint8_t*>(data());
			return MemoryRange<T>(reinterpret_cast<T*>(bufferBegin + firstElementIndex * sizeof(T)), elementCount);
		}
	private:
	};

}
