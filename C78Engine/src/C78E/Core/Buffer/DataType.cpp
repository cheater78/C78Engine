#include "C78EPCH.h"
#include "DataType.h"

namespace C78E {

	inline size_t DataType::naturalAligmentOf(size_t size) {
		const size_t upperLog2 = static_cast<size_t>(std::log2(size)) + 1;
		return static_cast<size_t>(std::pow(2, upperLog2));
	}


	inline PrimitiveType::PrimitiveType(Type type)
		: m_Type(type) {
	}

	inline size_t PrimitiveType::size() const {
		switch (m_Type) {
		case None: return 0;
		case Bool: return sizeof(bool);
		case Int8: return 1;
		case Int16: return 2;
		case Int32: return 4;
		case Int64: return 8;
		case UInt8: return 1;
		case UInt16: return 2;
		case UInt32: return 4;
		case UInt64: return 8;
		case Float16: return 2; // Assuming Float16 is represented as float
		case Float32: return 4;
		case Double64: return 8;
		}
		return 0; // Default case
	}

	inline size_t PrimitiveType::alignment() const {
		return size(); // for primitives, alignment size is the same as size -> pow of 2 is natural alignment
	}

	inline PrimitiveType::Type PrimitiveType::getType() const {
		return m_Type;
	}

	inline VectorType VectorType::Float2F() {
		return VectorType(PrimitiveType::Float32, 2);
	}

	inline VectorType VectorType::Float3F() {
		return VectorType(PrimitiveType::Float32, 3);
	}

	inline VectorType VectorType::Float4F() {
		return VectorType(PrimitiveType::Float32, 4);
	}

	inline VectorType VectorType::Mat3F() {
		return VectorType(PrimitiveType::Float32, 3 * 3);
	}

	inline VectorType VectorType::Mat4F() {
		return VectorType(PrimitiveType::Float32, 4 * 4);
	}

	inline VectorType::VectorType(PrimitiveType::Type type, size_t count)
		: PrimitiveType(type), m_Count(count) {
	}

	inline size_t VectorType::size() const {
		return PrimitiveType::size() * m_Count;
	}

	inline size_t VectorType::alignment() const {
		return naturalAligmentOf(size());
	}

	inline size_t VectorType::elementSize(Index fieldIndex) const {
		return PrimitiveType::size();
	}

	inline size_t VectorType::elementAlignment(Index fieldIndex) const {
		return PrimitiveType::alignment();
	}

	inline size_t VectorType::elementCount() const {
		return m_Count;
	}


	inline size_t ListType::size() const {
		size_t size = 0;
		for (const VectorType& elem : elements()) {
			size += elem.size();
		}
		return size;
	}

	inline size_t ListType::alignment() const {
		size_t alignment = 0;
		for (const VectorType& elem : elements()) {
			alignment += elem.alignment();
		}
		return naturalAligmentOf(alignment);
	}

	inline size_t ListType::elementSize(Index elementIndex) const {
		C78E_CORE_VALIDATE(elementIndex < std::vector<VectorType>::size(), "ListType::fieldSize: elementIndex out of bounds.");
		return (*this)[elementIndex].size();
	}

	inline size_t ListType::elementAlignment(Index elementIndex) const {
		C78E_CORE_VALIDATE(elementIndex < std::vector<VectorType>::size(), "ListType::fieldAlignment: elementIndex out of bounds.");
		return (*this)[elementIndex].alignment();
	}

	inline size_t ListType::elementCount() const {
		return std::vector<VectorType>::size();
	}

	inline ListType::ListTypeRange ListType::elements() {
		return ListTypeRange(std::vector<VectorType>::data(), std::vector<VectorType>::size());
	}

	inline const ListType::ListTypeRange ListType::elements() const {
		return ListTypeRange(std::vector<VectorType>::data(), std::vector<VectorType>::size());
	}

	inline ListType::ListTypeIterator ListType::begin() {
		return elements().begin();
	}

	inline ListType::ListTypeIterator ListType::end() {
		return elements().end();
	}

	inline VectorType& ListType::pushField(const VectorType& field) {
		std::vector<VectorType>::push_back(field);
		return std::vector<VectorType>::back();
	}

}