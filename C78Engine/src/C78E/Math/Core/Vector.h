#pragma once
#include "Dimension.h"
#include "Scalar.h"

namespace C78E::Math {

	/**
	 * @brief Vector of size dim, provides a default Vector type to use, using scalar type and formatting glm::packed_highp
	 * "most commonly used vector"
	 * @tparam dim the Vector size
	 */
	template<Dimension dim>
	using vec = glm::vec<dim, scalar, glm::packed_highp>;
	using vec2 = vec<Dimensions::D2>;
	using vec3 = vec<Dimensions::D3>;
	using vec4 = vec<Dimensions::D4>;

	template<Dimension dim>
	using ivec = glm::vec<dim, int, glm::packed_highp>;
	using ivec2 = ivec<Dimensions::D2>;
	using ivec3 = ivec<Dimensions::D3>;
	using ivec4 = ivec<Dimensions::D4>;

	template<Dimension dim>
	using uvec = glm::vec<dim, uint32_t, glm::packed_highp>;
	using uvec2 = uvec<Dimensions::D2>;
	using uvec3 = uvec<Dimensions::D3>;
	using uvec4 = uvec<Dimensions::D4>;

	/**
	 * @brief Pad a Vector from native Dimension to target Dimension,
	 * assumes NullVector for new Dimensions
	 * @tparam native Dimension of the given Vector
	 * @tparam target Dimension of the padded Vector
	 * @param nativeVector the Vector to provide
	 * @param paddingValue the value which to pad the Vector with, default 0
	 * @return the padded Vector
	 */
	template<Dimension native, Dimension target>
	requires (target >= native && native > 0)
	vec<target> padVector(vec<native> nativeVector, float paddingValue = 0.f) {
		vec<target> targetVector = vec<target>(0.f);
		for(Dimension i = 0; i < target; i++) {
			if(i < native) {
				targetVector[i] = nativeVector[i];
			} else {
				targetVector[i] = paddingValue;
			}
		}
		return targetVector;
	}

	/**
	 * @brief Collapse a Vector from native Dimension to target Dimension,
	 * cuts lost Dimensions
	 * @tparam native Dimension of the given Vector
	 * @tparam target Dimension of the collapsed Vector
	 * @param nativeVector the Vector to provide
	 * @return the collapsed Vector
	 */
	template<Dimension native, Dimension target>
		requires (target <= native && target > 0)
	vec<target> collapseVector(vec<native> nativeVector) {
		vec<target> targetVector = vec<target>(0.f);
		for(Dimension i = 0; i < target; i++) {
			targetVector[i] = nativeVector[i];
		}
		return targetVector;
	}

	/**
	 * @brief Refit a Vector from native Dimension to target Dimension,
	 * (native < target) assumes NullVector for new Dimensions, (native > target) cuts lost Dimensions
	 * @tparam native Dimension of the given Vector
	 * @tparam target Dimension of the refit Vector
	 * @param nativeVector the Vector to provide
	 * @param paddingValue the value which to pad the Vector with, default 0
	 * @return the refit Vector
	 */
	template<Dimension native, Dimension target>
		requires (native > 0 && target > 0)
	vec<target> refitVector(vec<native> nativeVector, scalar paddingValue = 0.f) {
		if constexpr(native < target) {
			return padVector<native, target>(nativeVector, paddingValue);
		} else {
			return collapseVector<native, target>(nativeVector);
		}
	}

	/**
	 * @brief Pad a Matrix from native Dimension to native + 1 Dimension,
	 * assumes Identity for the new Dimension
	 * @tparam native Dimension of the given Vector
	 * @param nativeVector the Vector to provide
	 * @param paddingValue the value which to pad the Vector with, default 0
	 * @return the padded Vector
	 */
	template<Dimension native>
		requires (native > 0)
	vec<native + 1> padVectorOnce(const vec<native>& nativeVector, scalar paddingValue = 0.f) {
		return padVector<native, native + 1>(nativeVector, paddingValue);
	}

	/**
	 * @brief Collapse a Transform Matrix from native Dimension to native - 1 Dimension, keeps Translation in the correct Matrix location,
	 * simply cuts the lost Dimension
	 * @tparam native Dimension of the given Vector
	 * @param nativeVector the Vector to provide
	 * @return the collapsed Vector
	 */
	template<Dimension native>
		requires (native > 1)
	vec<native - 1> collapseVectorOnce(const vec<native>& nativeVector) {
		return collapseVector<native, native - 1>(nativeVector);
	}

	/**
	 * @brief Element wise operation on 2 Vectors
	 * @tparam native Dimension of the given Vectors
	 * @param v0 first Vector
	 * @param v1 second Vector
	 * @param op operation to perform on the Vectors
	 * @return the result of the operation
	 */
	template<Dimension native>
		requires (native >= 2)
	vec<native> elementWiseOperation(const vec<native>& v0, const vec<native>& v1, std::function<scalar(scalar, scalar)> op) {
		vec<native> result = vec<native>(0.f);
		for(Dimension i = 0; i < native; i++) {
			result[i] = op(v0[i], v1[i]);
		}
		return result;
	}

	/**
	 * @brief Element wise operation on 2 Vectors
	 * @tparam native Dimension of the given Vectors
	 * @param v0 first Vector
	 * @param v1 second Vector
	 * @return the result of the operation
	 */
	template<Dimension native>
		requires (native >= 2)
	vec<native> elementWiseMin(const vec<native>& v0, const vec<native>& v1) {
		return elementWiseOperation<native>(v0, v1, [](scalar e1, scalar e2) -> scalar { return glm::min(e1, e2); });
	}

	/**
	 * @brief Element wise maximum on 2 Vectors
	 * @tparam native Dimension of the given Vectors
	 * @param v0 first Vector
	 * @param v1 second Vector
	 * @return the elementwise maximum elements as Vector
	 */
	template<Dimension native>
		requires (native >= 2)
	vec<native> elementWiseMax(const vec<native>& v0, const vec<native>& v1) {
		return elementWiseOperation<native>(v0, v1, [](scalar e1, scalar e2) -> scalar { return glm::max(e1, e2); });
	}

	/**
	 * @brief Element wise absolute on a Vector
	 * @tparam native Dimension of the given Vectors
	 * @param v0 first Vector
	 * @return the elementwise absolute elements as Vector
	 */
	template<Dimension native>
		requires (native >= 2)
	vec<native> elementWiseAbs(const vec<native>& v0) {
		return elementWiseOperation<native>(v0, vec<native>(0.f), [](scalar e1, scalar e2) -> scalar { return glm::abs(e1); });
	}

	/**
	 * @brief Element wise multiplication on 2 Vectors
	 * @tparam native Dimension of the given Vectors
	 * @param v0 first Vector
	 * @param v1 second Vector
	 * @return the elementwise multiplied elements as Vector
	 */
	template<Dimension native>
		requires (native >= 2)
	vec<native> elementWiseMul(const vec<native>& v0, const vec<native>& v1) {
		return elementWiseOperation<native>(v0, v1, [](scalar e1, scalar e2) -> scalar { return e1 * e2; });
	}
	
	/**
	 * @brief Element wise division on 2 Vectors
	 * @tparam native Dimension of the given Vectors
	 * @param v0 first Vector
	 * @param v1 second Vector
	 * @return the elementwise divided elements as Vector
	 */
	template<Dimension native>
		requires (native >= 2)
	vec<native> elementWiseDiv(const vec<native>& v0, const vec<native>& v1) {
		return elementWiseOperation<native>(v0, v1, [](scalar e1, scalar e2) -> scalar { return e1 / e2; });
	}
	
	/**
	 * @brief Element wise Clamp a Vector between the values of 2 Vectors
	 * @tparam native Dimension of the given Vectors
	 * @param v the Vector to clamp
	 * @param b1 one Clamping Bound
	 * @param b2 the other Clamping Bound
	 * @return the clamped Vector
	 */
	template<Dimension native>
		requires (native >= 2)
	inline vec<native> elementWiseClamp(const vec<native>& v, const vec<native>& b1, const vec<native>& b2) {
		const vec<native> lowerBounds = elementWiseMin<native>(b1, b2);
		const vec<native> upperBounds = elementWiseMax<native>(b1, b2);
		return elementWiseMax<native>(elementWiseMin<native>(v, upperBounds), lowerBounds);
	}

	/**
	 * @brief Computes the triple Product of 3 Vectors
	 * @param a Vector
	 * @param b Vector
	 * @param c Vector
	 * @return the triple Product
	 */
	inline vec3 tripleProduct(const vec3& a, const vec3& b, const vec3& c) {
		return glm::cross(glm::cross(a, b), c);
	}

	/**
	 * @brief Determines whether 2 Vectors point in the same Direction
	 * @tparam dim the Dimension of the Vectors
	 * @param v0 one of the Vectors to provide
	 * @param v1 the other Vector to provide
	 * @return whether they point in the same Direction
	 */
	template<Dimension dim>
	inline bool doAlign(const vec<dim>& v0, const vec<dim>& v1) {
		return glm::dot(v0, v1) > 0;
	}

	/**
	 * @brief Determines whether 2 Vectors point are parallel
	 * parallel means that they point in the same or opposite Direction,
	 * while being below the isNearZero threshold cosine angle
	 * @tparam dim the Dimension of the Vectors
	 * @param v0 one of the Vectors to provide
	 * @param v1 the other Vector to provide
	 * @return whether they are parallel
	 */
	template<Dimension dim>
	inline bool inParallel(const vec<dim>& v0, const vec<dim>& v1) {
		return isNearZero( 1.f - glm::abs(glm::dot(v0, v1)) );
	}

	/**
	 * @brief Computes the dot product of 2 Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @param a one of the Vectors to provide
	 * @param b the other Vector to provide
	 * @return the dot product of the Vectors
	 */
	template <Dimension dim>
	scalar dot(const vec<dim>& a, const vec<dim>& b) {
		scalar result = 0.0;
		for(Dimension i = 0; i < dim; ++i)
			result += a[i] * b[i];
		return result;
	}

	/**
	 * @brief Computes the projection of a Vector on another Vector, as a scale of the second Vector
	 * @tparam dim the Dimension of the Vectors
	 * @param a one of the Vectors to provide
	 * @param b the other Vector to provide
	 * @return the projection scale of a on b
	 */
	template<Dimension dim>
		requires (dim >= 2)
	scalar projectionScaleOn(const vec<dim>& a, const vec<dim>& b) {
		return glm::dot(a, b) / glm::length2(b);
	}

	/**
	 * @brief Computes the projection of a Vector on another Vector, as a scale of the second Vector
	 * @tparam dim the Dimension of the Vectors
	 * @param a one of the Vectors to provide
	 * @param b the other Vector to provide
	 * @return the projection scale of a on b
	 */
	template<Dimension dim>
		requires (dim >= 2)
	scalar projectionLengthOn(const vec<dim>& a, const vec<dim>& b) {
		return glm::dot(a, b) / glm::length(b);
	}

	/**
	 * @brief Computes the projection of a Vector on another Vector
	 * @tparam dim the Dimension of the Vectors
	 * @param a one of the Vectors to provide
	 * @param b the other Vector to provide
	 * @return the projection of a on b
	 */
	template<Dimension dim>
		requires (dim >= 2)
	vec<dim> projectOn(const vec<dim>& a, const vec<dim>& b) {
		return projectionScaleOn(a, b) * b;
	}

	/**
	 * @brief Computes the cross product of 2 Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @param a one of the Vectors to provide
	 * @param b the other Vector to provide
	 * @return the cross product of the Vectors
	 */
	template <Dimension dim>
	vec<dim> normalize(const vec<dim>& v) {
		scalar length = std::sqrt(dot(v, v));
		vec<dim> result{};
		for(Dimension i = 0; i < dim; ++i)
			result[i] = v[i] / length;
		return result;
	}

	/**
	 * @brief Computes the Gram-Schmidt orthonormalization of a set of Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @tparam count the number of Vectors to provide
	 * @param vectors the Vectors to provide
	 * @return the orthonormal basis of the Vectors
	 */
	template <Dimension dim, size_t count>
	std::array<vec<dim>, count> gramSchmidt(const std::array<vec<dim>, count>& vectors) { //TODO: unused
		std::array<vec<dim>, count> result{};
		for(size_t i = 0; i < count; ++i) {
			vec<dim> v = vectors[i];
			for(size_t j = 0; j < i; ++j) {
				double proj = dot(v, result[j]);
				for(size_t k = 0; k < dim; ++k)
					v[k] -= proj * result[j][k];
			}
			result[i] = glm::normalize(v);
		}
		return result;
	}

	
	template<Dimension native>
		requires (native >= 2)
	inline vec<native> smoothStep(const vec<native>& lo, const vec<native>& hi, scalar interpolate) {
		const scalar interpolateClamped = clamp(interpolate, 0.f, 1.f);
		return lo + interpolateClamped * (hi - lo);
	}

	template<Dimension native>
		requires (native >= 2)
	inline vec<native> snapToBounds(const vec<native>& v, const vec<native>& lo, const vec<native>& hi) {
		const vec<native> lowerBound = elementWiseMin<native>(lo, hi);
		const vec<native> upperBound = elementWiseMax<native>(lo, hi);

		const vec<native> center = 0.5f * (upperBound + lowerBound);
		const vec<native> halfExtent = 0.5f * (upperBound - lowerBound);
		if(isNearZero(glm::length(halfExtent))) return v; // degenerate Bounds

		const vec<native> vCenterLocal = v - center;
		const vec<native> vAbsCenterLocal = glm::abs(vCenterLocal); // kill quadrant directionality
		const vec<native> distToBounds = vAbsCenterLocal - halfExtent; // any elem > 0 -> oustide / all elem < 0 -> inside

		scalar correction = -scalar_limit::infinity(); // greatest signed dimension distance to bound, negative if inside -> still greatest(smallest abs)
		Dimension closestDim = 0;
		for(Dimension i = 0; i < native; i++) {
			if(!isNearZero(halfExtent[i]) && distToBounds[i] > correction) {
				correction = distToBounds[i];
				closestDim = i;
			}
		}
		vec<native> onBoundsCenterLocal = vCenterLocal;
		onBoundsCenterLocal[closestDim] += (vCenterLocal[closestDim] < 0.f) ? correction : -correction; // reconstruct quadrant directionality
		return elementWiseClamp<native>(center + onBoundsCenterLocal, lowerBound, upperBound);
	}

}

#include <C78E/Utils/StdUtils.h>
namespace std {

	// alr defined in GLM.h

}

#include <C78E/Utils/YamlUtils.h>
namespace YAML {

	// alr defined in GLM.h

}