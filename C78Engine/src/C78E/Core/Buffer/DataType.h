#pragma once
#include <C78E/Utils/StdUtils.h>
#include "Iterator.h"

namespace C78E {

	/**
	 * @brief Generic DataType interface from a memory perspective.
	 * A DataType is defined by its size and alignment(natural alignment).
	 */
	class DataType {
	public:
		DataType() = default;
		DataType(const DataType&) = default;
		DataType(DataType&&) = default;
		DataType& operator=(const DataType&) = default;
		virtual ~DataType() = default;
		
		virtual size_t size() const = 0;
		virtual size_t alignment() const = 0;
	protected:
		static size_t naturalAligmentOf(size_t size);
	};

	/**
	 * @brief PrimitiveType is a DataType that represents a single value type.
	 * It can be used to represent basic types like int, float, bool, etc.
	 */
	class PrimitiveType : public DataType {
	public:
		enum Type {
			None = 0,
			Bool,
			Int8,
			Int16,
			Int32,
			Int64,
			UInt8,
			UInt16,
			UInt32,
			UInt64,
			Float16,
			Float32,
			Double64
		};
	public:
		PrimitiveType(Type type);
		PrimitiveType(const PrimitiveType&) = default;
		PrimitiveType(PrimitiveType&&) = default;
		PrimitiveType& operator=(const PrimitiveType&) = default;
		virtual ~PrimitiveType() = default;

		virtual size_t size() const override;
		virtual size_t alignment() const override;
	public:
		Type getType() const;
	protected:
		Type m_Type = None;
	};

	/**
	 * @brief CompositeType is a DataType interface that represents a collection of DataTypes.
	 * It can be used to represent structs, arrays, and lists.
	 */
	class CompositeType : public DataType {
	public:
		using Index = size_t;
	public:
		virtual ~CompositeType() = default;

		virtual size_t size() const = 0;
		virtual size_t alignment() const = 0;

		virtual size_t elementSize(Index elementIndex) const = 0;
		virtual size_t elementAlignment(Index elementIndex) const = 0;
		virtual size_t elementCount() const = 0;

	};

	/**
	 * @brief VectorType is a CompositeType that represents a vector of PrimitiveTypes.
	 * It can be used to represent vectors, matrices, and other composite types of single typed elements.
	 */
	class VectorType : public PrimitiveType, public CompositeType {
	public:
		using Index = size_t;
	public: // Type alias for convenience
		static inline VectorType Float2F();
		static inline VectorType Float3F();
		static inline VectorType Float4F();
		static inline VectorType Mat3F();
		static inline VectorType Mat4F();
	public:
		VectorType(PrimitiveType::Type type = PrimitiveType::Type::None, size_t count = 1);
		VectorType(const VectorType&) = default;
		VectorType(VectorType&&) = default;
		VectorType& operator=(const VectorType&) = default;
		virtual ~VectorType() = default;

		virtual size_t size() const override;
		virtual size_t alignment() const override;

		virtual size_t elementSize(Index elementIndex = 0) const override;
		virtual size_t elementAlignment(Index elementIndex = 0) const override;
		virtual size_t elementCount() const override;
	protected:
		size_t m_Count = 0;
	};


	/**
	 * @brief StructType is a fixed-size CompositeType containing a fixed number of VectorTypes.
	 * @tparam N (fixed) number of elements/fields in the struct.
	 */
	template<size_t N>
	class StructType : public CompositeType, private std::array<VectorType, N> {
	public:
		using StructTypeRange = MemoryRange<VectorType>;
		using StructTypeIterator = StructTypeRange::Iterator;
		using Index = size_t;
	public:
		template<typename... Args>
		StructType(Args&&... args)
			: std::array<VectorType, N>{ std::forward<Args>(args)... } {
			C78E_CORE_STATIC_ASSERT(sizeof...(args) == N, "StructType: Number of arguments must match the size of the struct.");
		}

		virtual ~StructType() = default;

		virtual size_t size() const override {
			size_t size = 0;
			for (const VectorType& elem : *this) {
				size += elem.size();
			}
			return size;
		}
		virtual size_t alignment() const override {
			size_t alignment = 0;
			for (const VectorType& elem : *this) {
				alignment += elem.alignment();
			}
			return naturalAligmentOf(alignment);
		}

		virtual inline size_t elementSize(Index elementIndex) const override {
			C78E_CORE_VALIDATE(elementIndex < N, return std::numeric_limits<size_t>::max(), "StructType::fieldSize: fieldIndex out of bounds.");
			return (*this)[elementIndex].size();
		}
		virtual inline size_t elementAlignment(Index elementIndex) const override {
			C78E_CORE_VALIDATE(elementIndex < N, return std::numeric_limits<size_t>::max(), "StructType::fieldAlignment: fieldIndex out of bounds.");
			return (*this)[elementIndex].alignment();
		}
		virtual inline size_t elementCount() const override {
			return N;
		}
	public:
		StructTypeRange elements() {
			return StructTypeRange(std::array<VectorType, N>::data(), std::array<VectorType, N>::size());
		}
		const StructTypeRange elements() const {
			return StructTypeRange(std::array<VectorType, N>::data(), std::array<VectorType, N>::size());
		}
		StructTypeIterator begin() {
			return elements().begin();
		}
		StructTypeIterator end() {
			return elements().end();
		}
	};
	/**
	 * @brief ListType is a CompositeType that represents a collection of VectorTypes.
	 * It can be used to represent any Type that consists of multiple VectorTypes.
	 */
	template<typename T>
	requires std::is_convertible_v<T, VectorType>
	class ExtListType : public CompositeType, private std::vector<T> {
	public:
		using ListTypeRange = MemoryRange<T>;
		using ListTypeIterator = ListTypeRange::Iterator;
		using Index = size_t;
	public:
		ExtListType() = default;
		template<typename... Args>
		ExtListType(Args&&... args)
			: std::vector<T>{ std::forward<Args>(args)... } {
		}
		virtual ~ExtListType() = default;

		virtual size_t size() const override {
			size_t size = 0;
			for (const T& elem : elements()) {
				size += elem.size();
			}
			return size;
		}
		virtual size_t alignment() const override {
			size_t alignment = 0;
			for (const T& elem : elements()) {
				alignment += elem.alignment();
			}
			return naturalAligmentOf(alignment);
		}

		virtual inline size_t elementSize(Index elementIndex) const override {
			C78E_CORE_ASSERT(elementIndex < std::vector<T>::size(), "ExtListType::fieldSize: elementIndex out of bounds.");
			return (*this)[elementIndex].size();
		}
		virtual inline size_t elementAlignment(Index elementIndex) const override {
			C78E_CORE_ASSERT(elementIndex < std::vector<T>::size(), "ExtListType::fieldAlignment: elementIndex out of bounds.");
			return (*this)[elementIndex].alignment();
		}
		virtual inline size_t elementCount() const override {
			return std::vector<T>::size();
		}

		ListTypeRange elements() {
			return ListTypeRange(std::vector<T>::data(), std::vector<T>::size());
		}
		const ListTypeRange elements() const {
			return ListTypeRange(std::vector<T>::data(), std::vector<T>::size());
		}
		ListTypeIterator begin() {
			return elements().begin();
		}
		ListTypeIterator end() {
			return elements().end();
		}
		const ListTypeIterator begin() const {
			return elements().begin();
		}
		const ListTypeIterator end() const {
			return elements().end();
		}

		T& pushField(const T& element) {
			std::vector<T>::push_back(element);
			return std::vector<T>::back();
		}

		template<typename O, std::enable_if_t<std::is_convertible<T, VectorType>::value, bool> = true>
		ExtListType& operator=(const ExtListType<O>& other) {
			std::vector<VectorType>::clear();
			std::vector<VectorType>::reserve(other.elementCount());
			for (const O& elem : other) {
				std::vector<VectorType>::emplace_back(static_cast<VectorType>(elem));
			}
			return *this;
		}

	};
	using ListType = ExtListType<VectorType>;


	//TODO: GenericType that is a Composite of Composites, therefor can represent any DataType. (if needed)

}
