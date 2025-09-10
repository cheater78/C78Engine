#pragma once
#include <C78E/Utils/StdUtils.h>
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Core/Buffer/Iterator.h>

namespace C78E {

	/**
	 * @brief Non owning Buffer, used for temporary data storage
	 * don't forget to release the buffer after usage
	 */
	struct Buffer {
	public:
		static bool copy(Buffer& from, Buffer& to);
	public:
		Buffer() = default;
		Buffer(size_t _size, const void* _data = nullptr);
		Buffer(const Buffer&) = delete;
		~Buffer();

		void allocate(size_t _size);
		void release();

		template<typename T>
		T* as() const {
			return (T*)data;
		}
		
		template<typename T>
		void clear(const T& value) {
			C78E_CORE_ASSERT(size % sizeof(T) == 0, "Buffer::clear: Buffers size({}bytes) is not a multiple of the given Types size({}bytes)!", size, sizeof(T));
			for (T* elem = (T*)data; (size_t)elem < (size_t)data + size; elem++)
				*elem = value;
		}

		template<typename T>
		T& at(size_t index) {
			C78E_CORE_ASSERT(size % sizeof(T) == 0, "Buffer::at: Buffers size({}bytes) is not a multiple of the given Types size({}bytes)!", size, sizeof(T));
			C78E_CORE_ASSERT(index * sizeof(T) < size, "Buffer::at: Index({}) is out of bounds! Buffer has {} elements.", index, size / sizeof(T));
			return *((T*)data + index);
		}

		template<typename T>
		MemoryRange<T> range() const {
			return MemoryRange(data, size);
		}

		operator bool() const {
			return (bool)data;
		}

	public:
		uint8_t* data = nullptr;
		size_t size = 0;
	};

	struct ScopedBuffer {
	public:
		ScopedBuffer(size_t size, const void* data = nullptr);
		ScopedBuffer(ScopedBuffer& other);
		~ScopedBuffer();

		uint8_t* data();
		size_t size() const;

		template<typename T>
		T* as() const {
			return m_Buffer.as<T>();
		}
		
		template<typename T>
		void clear(const T& value) {
			m_Buffer.clear<T>(value);
		}

		template<typename T>
		T& at(size_t index) {
			return m_Buffer.at<T>(index);
		}

		template<typename T>
		MemoryRange<T> range() const {
			return MemoryRange<T>(data, size);
		}

		operator bool() const { return m_Buffer; }
	private:
		Buffer m_Buffer;
	};


}
