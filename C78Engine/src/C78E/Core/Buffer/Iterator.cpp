#include "C78EPCH.h"
#include "Iterator.h"

namespace C78E {
	
	template<typename T>
	inline MemoryRange<T>::MemoryRange(const T* begin, const T* end)
		: m_Begin(begin), m_ElementCount((end - begin) / sizeof(T)) {
		C78E_CORE_ASSERT(begin && end, "MemoryRange::MemoryRange: begin or end pointer was nullptr!");
		C78E_CORE_ASSERT((end - begin) % sizeof(T), "MemoryRange::MemoryRange: begin and end do not fit a whole number of {}", typeid(T).name());
	}
	template<typename T>
	inline MemoryRange<T>::MemoryRange(const T* begin, size_t elementCount)
		: m_Begin(begin), m_ElementCount(elementCount) {
	}
	template<typename T>
	inline MemoryRange<T>::Iterator MemoryRange<T>::begin() const {
		return Iterator(m_Begin);
	}
	template<typename T>
	inline MemoryRange<T>::Iterator MemoryRange<T>::end() const {
		return Iterator(m_Begin + m_ElementCount * sizeof(T));
	}
	template<typename T>
	inline MemoryRange<T>::ReverseIterator MemoryRange<T>::rend() const {
		return ReverseIterator(m_Begin + m_ElementCount * sizeof(T));
	}
	template<typename T>
	inline MemoryRange<T>::ReverseIterator MemoryRange<T>::rbegin() const {
		return ReverseIterator(m_Begin);
	}
	template<typename T>
	inline T& MemoryRange<T>::at(size_t index) {
		C78E_CORE_ASSERT(index < m_ElementCount, "MemoryRange::at: Index out of bounds!");
		return m_Begin[index];
	}
	template<typename T>
	inline const T& MemoryRange<T>::at(size_t index) const {
		C78E_CORE_ASSERT(index < m_ElementCount, "MemoryRange::at: Index out of bounds!");
		return m_Begin[index];
	}
	template<typename T>
	inline size_t MemoryRange<T>::size() const {
		return end() - begin();
	}

	template<typename T>
	inline size_t MemoryRange<T>::elementCount() const {
		return m_ElementCount;
	}

	template<typename T>
	inline size_t MemoryRange<T>::byteSize() const {
		return m_ElementCount * sizeof(T);
	}

}