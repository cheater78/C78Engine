#pragma once

#include <stdint.h>
#include <cstring>

namespace C78E {

	// Non-owning raw buffer
	struct Buffer {
	public:
		uint8_t* data = nullptr;
		size_t size = 0;

		Buffer() = default;

		Buffer(size_t size) {
			allocate(size);
		}

		Buffer(const Buffer&) = default;

		static Buffer copy(Buffer other) {
			Buffer result(other.size);
			memcpy(result.data, other.data, other.size);
			return result;
		}

		void allocate(size_t _size) {
			release();

			data = new uint8_t[_size];
			size = _size;
		}

		void release() {
			delete[] data;
			data = nullptr;
			size = 0;
		}

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

		operator bool() const {
			return (bool)data;
		}

	};

	struct ScopedBuffer {
	public:
		ScopedBuffer(size_t size)
			: m_Buffer(size)
		{ }

		ScopedBuffer(ScopedBuffer& other)
			: m_Buffer{other.size()} {
			memcpy(m_Buffer.data, other.data(), other.size());
		};

		~ScopedBuffer() {
			m_Buffer.release();
		}

		uint8_t* data() { return m_Buffer.data; }
		size_t size() const { return m_Buffer.size; }

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

		operator bool() const { return m_Buffer; }
	private:
		Buffer m_Buffer;
	};


}
