#pragma once

#include <stdint.h>
#include <cstring>

namespace C78E {

	// Non-owning raw buffer class
	struct Buffer {
		uint8_t* data = nullptr;
		uint64_t size = 0;

		Buffer() = default;

		Buffer(uint64_t size) {
			allocate(size);
		}

		Buffer(const Buffer&) = default;

		static Buffer copy(Buffer other) {
			Buffer result(other.size);
			memcpy(result.data, other.data, other.size);
			return result;
		}

		void allocate(uint64_t _size) {
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
		T* as() {
			return (T*)data;
		}

		operator bool() const {
			return (bool)data;
		}

	};

	struct ScopedBuffer {
		ScopedBuffer(Buffer buffer)
			: m_Buffer(buffer)
		{ }

		ScopedBuffer(uint64_t size)
			: m_Buffer(size)
		{ }

		~ScopedBuffer() {
			m_Buffer.release();
		}

		uint8_t* data() { return m_Buffer.data; }
		uint64_t size() { return m_Buffer.size; }

		template<typename T>
		T* as() {
			return m_Buffer.as<T>();
		}

		operator bool() const { return m_Buffer; }
	private:
		Buffer m_Buffer;
	};


}
