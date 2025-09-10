#pragma once
#include <C78E/Math/Core/TransformMatrix.h>

namespace C78E::Math {

	template<Dimension dim, typename IntersectionResultType>
		requires (dim >= 2)
	struct Intersection {
	public:
		enum Type : uint32_t {
			None = 0, // A and B share no space
			Intersecting = 1 << 0, // A and B are intersecting
			Touching = 1 << 1, // A and B are touching each other, they share some primitive (dim - 1?)
			Inside = 1 << 2, // A is inside B
			Containing = 1 << 3, // A contains B, B is inside A
			Identical = 1 << 4 // A is identical to B
		};
	private:
		inline static bool typeHas(Type value, Type flag) {
			return (static_cast<uint32_t>(value) & static_cast<uint32_t>(flag)) != 0;
		}
	public:
		static Intersection createNone() {
			return Intersection();
		}
		static Intersection createIntersection(const IntersectionResultType& result) {
			return Intersection( Type::Intersecting, result);
		}
		static Intersection createTouching(const IntersectionResultType& result = IntersectionResultType()) {
			return Intersection(static_cast<Type>(Type::Intersecting | Type::Touching), result);
		}
		static Intersection createInside() {
			return Intersection(static_cast<Type>(Type::Intersecting | Type::Inside));
		}
		static Intersection createContaining() {
			return Intersection(static_cast<Type>(Type::Intersecting | Type::Containing));
		}
		static Intersection createIdentical() {
			return Intersection(static_cast<Type>(Type::Intersecting | Type::Inside | Type::Containing | Type::Identical));
		}
	public:
		Intersection() = default;
	private:
		Intersection(Type type, const IntersectionResultType& result = IntersectionResultType()) : type(type), result(result) { }
	public:
		Intersection(const Intersection& other) = default;
		~Intersection() = default;
		
		bool has(Type value) const {
			return typeHas(type, value);
		}

		bool hasNone() const {
			return !type;
		}

		bool isIdentical() const {
			return has(Type::Identical);
		}

		const IntersectionResultType& getResult() const {
			return result;
		}

	public:
		Type type = Type::None;
		IntersectionResultType result = IntersectionResultType();
	};

	template<Dimension dim, typename IntersectionResultType>
		requires (dim >= 2)
	inline typename Intersection<dim, IntersectionResultType>::Type operator|(typename Intersection<dim, IntersectionResultType>::Type a, typename Intersection<dim, IntersectionResultType>::Type b) {
		return static_cast<typename Intersection<dim, IntersectionResultType>::Type>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}

	template<Dimension dim, typename IntersectionResultType>
		requires (dim >= 2)
	inline typename Intersection<dim, IntersectionResultType>::Type operator|(bool a, typename Intersection<dim, IntersectionResultType>::Type b) {
		return static_cast<typename Intersection<dim, IntersectionResultType>::Type>((a) ? -1 : 0 | static_cast<uint32_t>(b));
	}

}