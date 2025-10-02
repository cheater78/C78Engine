#pragma once
#include <C78E/Utils/StdUtils.h>

namespace C78E {

	template <typename T>
	concept Iterable = requires(T t) {
		{ std::begin(t) } -> std::input_or_output_iterator; // must have std::begin
		{ std::end(t) }   -> std::sentinel_for<decltype(std::begin(t))>; // must have std::end
	};

	template<typename T>
	struct MemoryRange {
	public:
		using IteratorDifference = size_t;
		struct Iterator {
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			T* ptr;

			T operator*() const { return *ptr; }
			Iterator& operator++() {
				ptr++; 
				return *this;
			}
			bool operator!=(const Iterator& other) const {
				return ptr != other.ptr;
			}
			IteratorDifference operator-(const Iterator& other) const {
				return (ptr - other.ptr) / sizeof(T);
			}
		};
		struct ReverseIterator {
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			T* ptr;

			T operator*() const { return *ptr; }
			ReverseIterator& operator++() {
				ptr--;
				return *this;
			}
			bool operator!=(const ReverseIterator& other) const {
				return ptr != other.ptr;
			}
			IteratorDifference operator-(const ReverseIterator& other) const {
				return (ptr - other.ptr) / sizeof(T);
			}
		};
	public:
		MemoryRange(T* begin, T* end)
			: m_Begin(begin), m_ElementCount((end - begin) / sizeof(T)) {
			C78E_CORE_ASSERT(begin && end, "MemoryRange::MemoryRange: begin or end pointer was nullptr!");
			C78E_CORE_ASSERT((end - begin) % sizeof(T), "MemoryRange::MemoryRange: begin and end do not fit a whole number of {}", typeid(T).name());
		}
		MemoryRange(T* begin, size_t elementCount)
			: m_Begin(begin), m_ElementCount(elementCount) {
		}

		virtual Iterator begin() const {
			return Iterator(m_Begin);
		}
		virtual Iterator end() const {
			return Iterator(m_Begin + m_ElementCount * sizeof(T));
		}

		virtual ReverseIterator rend() const {
			return ReverseIterator(m_Begin + m_ElementCount * sizeof(T));
		}
		virtual ReverseIterator rbegin() const {
			return ReverseIterator(m_Begin);
		}

		virtual T& at(size_t index) const {
			C78E_CORE_ASSERT(index < m_ElementCount, "MemoryRange::at: Index out of bounds!");
			return m_Begin[index];
		}

	public:
		size_t size() const {
			return end() - begin();
		}
		size_t elementCount() const {
			return m_ElementCount;
		}
		size_t byteSize() const {
			return m_ElementCount * sizeof(T);
		}

		inline T& operator[](size_t index) {
			return at(index);
		}

		inline T* data() {
			return m_Begin;
		}
	private:
		T* const m_Begin;
		size_t m_ElementCount;
	};

}
