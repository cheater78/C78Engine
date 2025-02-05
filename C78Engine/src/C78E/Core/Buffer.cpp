#include "C78EPCH.h"
#include "Buffer.h"

namespace C78E {

	Buffer::Buffer(size_t _size) {
		allocate(_size);
	}

	Buffer::Buffer(size_t _size, const void* _data) {
		allocate(_size);
		std::memcpy(data, _data, size);
	}

	Buffer::~Buffer() {
		// non owning buffer, release manually
	}

	bool Buffer::copy(Buffer& from, Buffer& to) {
		to.allocate(from.size);
		std::memcpy(to.data, from.data, from.size);
		return to.size;
	}

	void Buffer::allocate(size_t _size) {
		release();

		data = new uint8_t[_size];
		size = _size;
	}

	void Buffer::release() {
		if (data) {
			delete[] data;
			data = nullptr;
		}
		size = 0;
	}

	ScopedBuffer::ScopedBuffer(size_t size)
		: m_Buffer(size) {
	}

	ScopedBuffer::ScopedBuffer(size_t size, const void* data)
		: m_Buffer(size, data) {
	}

	ScopedBuffer::ScopedBuffer(ScopedBuffer& other) {
		Buffer::copy(other.m_Buffer, m_Buffer);
	}

	ScopedBuffer::~ScopedBuffer() {
		m_Buffer.release();
	}

	uint8_t* ScopedBuffer::data() {
		return m_Buffer.data;
	}

	size_t ScopedBuffer::size() const {
		return m_Buffer.size;
	}

}