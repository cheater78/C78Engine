#pragma once

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
		MemoryRange(const T* begin, const T* end);
		MemoryRange(const T* begin, size_t elementCount);

		virtual Iterator begin() const;
		virtual Iterator end() const;

		virtual ReverseIterator rend() const;
		virtual ReverseIterator rbegin() const;

		virtual T& at(size_t index);
		virtual const T& at(size_t index) const;

	public:
		size_t size() const;
		size_t elementCount() const;
		size_t byteSize() const;

		inline T& operator[](size_t index) {
			return at(index);
		}
		inline const T& operator[](size_t index) const {
			return at(index);
		}

		inline T* data() {
			return m_Begin;
		}
	private:
		T* m_Begin;
		size_t m_ElementCount;
	};

}
