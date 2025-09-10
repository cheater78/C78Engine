#pragma once
#include "Matrix.h"

namespace C78E::Math {

	/**
	 * @brief Pad a Transform Matrix from native Dimension to target Dimension, keeps Translation in the correct Matrix location,
	 * assumes Identity transform for new Dimensions
	 * @tparam native Dimension of the given TransformMatrix
	 * @tparam target Dimension of the padded TransformMatrix
	 * @param nativeMatrix the TransformMatrix to provide
	 * @return the padded TransformMatrix
	 */
	template<Dimension native, Dimension target>
		requires (native <= target && native > 0)
	mat<target> padTransformMatrix(const mat<native>& nativeMatrix) {
		constexpr Dimension coordinateDimension = native - 1; //Transform Matrix for Homogeneous Coordinates is dim + 1
		using CoreMatrix = mat<coordinateDimension>;
		using TranslationVector = vec<coordinateDimension>;

		if constexpr(native == target) return nativeMatrix;

		mat<target> targetMatrix = mat<target>(0.f);
		const CoreMatrix coreMatrix = CoreMatrix(nativeMatrix); //TODO: this cast works only with glm
		const TranslationVector translationVector = TranslationVector(nativeMatrix[native - 1]);

		targetMatrix = padMatrix<coordinateDimension, target>(coreMatrix);
		targetMatrix[target - 1] = vec<target>(padVector<coordinateDimension, target - 1>(translationVector), 1.f); // pad vec to target - 1 only to keep 1.f in lower Corner
		return targetMatrix;
	}

	/**
	 * @brief Collapse a Transform Matrix from native Dimension to target Dimension, keeps Translation in the correct Matrix location,
	 * cuts lost Dimensions
	 * @tparam native Dimension of the given TransformMatrix
	 * @tparam target Dimension of the collapsed TransformMatrix
	 * @param nativeMatrix the TransformMatrix to provide
	 * @return the collapsed TransformMatrix
	 */
	template<Dimension native, Dimension target>
		requires (native >= target && target > 0)
	mat<target> collapseTransformMatrix(const mat<native>& nativeMatrix) {
		constexpr Dimension coordinateDimension = target - 1; //Transform Matrix for Homogeneous Coordinates is dim + 1
		using CoreMatrix = mat<coordinateDimension>;
		using TranslationVector = vec<coordinateDimension>;

		if constexpr(native == target) return nativeMatrix;

		mat<target> targetMatrix = mat<target>(0.f);
		const CoreMatrix coreMatrix = CoreMatrix(nativeMatrix); //TODO: this cast works only with glm
		const TranslationVector translationVector = TranslationVector(nativeMatrix[native - 1]);

		targetMatrix = padMatrix<coordinateDimension, target>(coreMatrix);
		targetMatrix[target - 1] = vec<target>(padVector<coordinateDimension, target - 1>(translationVector), 1.f); // pad vec to target - 1 only to keep 1.f in lower Corner
		return targetMatrix;
	}

	/**
	 * @brief Refit a Transform Matrix from native Dimension to target Dimension, keeps Translation in the correct Matrix location,
	 * (native < target) assumes Identity transform for new Dimensions, (native > target) cuts lost Dimensions
	 * @tparam native Dimension of the given TransformMatrix
	 * @tparam target Dimension of the refit TransformMatrix
	 * @param nativeMatrix the TransformMatrix to provide
	 * @return the refit TransformMatrix
	 */
	template<Dimension native, Dimension target>
		requires (native >= target && target > 0)
	mat<target> refitTransformMatrix(const mat<native>& nativeMatrix) {
		if constexpr(native < target) {
			return padTransformMatrix<native, target>(nativeMatrix);
		} else {
			return collapseTransformMatrix<native, target>(nativeMatrix);
		}
	}

	/**
	 * @brief Pad a Transform Matrix from native Dimension to native + 1 Dimension, keeps Translation in the correct Matrix location,
	 * assumes Identity transform for the new Dimension
	 * @tparam native Dimension of the given TransformMatrix
	 * @param nativeMatrix the TransformMatrix to provide
	 * @return  the padded TransformMatrix
	 */
	template<Dimension native>
		requires (native > 0)
	mat<native + 1> padTransformMatrixOnce(const mat<native>& nativeMatrix) {
		return padTransformMatrix<native, native + 1>(nativeMatrix);
	}
	/**
	 * @brief Collapse a Transform Matrix from native Dimension to native - 1 Dimension, keeps Translation in the correct Matrix location,
	 * simply cuts the lost Dimension
	 * @tparam native Dimension of the given TransformMatrix
	 * @param nativeMatrix the TransformMatrix to provide
	 * @return the collapsed TransformMatrix
	 */
	template<Dimension native>
		requires (native > 1)
	mat<native - 1> collapseTransformMatrixOnce(const mat<native>& nativeMatrix) {
		return collapseTransformMatrix<native, native - 1>(nativeMatrix);
	}


	/**
	 * @brief Transforms a Vector given a Matrix, Vector is padded to use Homogeneous Coordinates according to isPoint,
	 * transformation is performed on the largest Dimension between vector and transform,
	 * if vector does not specify a component for a given Dimension the nullvector is assumed,
	 * if transform does not specify a Transform for a given Dimension Identity is assumend
	 * @tparam native the vectors native Dimension
	 * @tparam transforming the transforms Dimension
	 * @param vector the vector to transform
	 * @param transform the transform to apply to the vector
	 * @param isPoint {0.f, 1.f} whether the given vector should be treated as a Vector or a Point
	 * @return the transformed Vector
	 */
	template<Dimension native, Dimension transforming>
	vec<native> transformHomogeneous(const vec<native>& vector, const mat<transforming>& transform, const scalar isPoint) {
		if constexpr(native == transforming - 1) {
			return vec<native>(transform * vec<transforming>(vector, isPoint));
		} else if constexpr(native < transforming) {
			return vec<native>(transform * vec<transforming>(padVector<native, transforming - 1>(vector), isPoint));
		} else {
			return vec<native>(padTransformMatrix<transforming, native + 1>(transform) * vec<native + 1>(vector, isPoint));
		}
	}

	/**
	 * @brief Transforms a Position given a Matrix, position is padded to use Homogeneous Coordinates,
	 * transformation is performed on the largest Dimension between position and transform,
	 * if position does not specify a component for a given Dimension the nullvector is assumed,
	 * if transform does not specify a Transform for a given Dimension Identity is assumend
	 * @tparam native the positions native Dimension
	 * @tparam transforming the transforms Dimension
	 * @param position the position to transform
	 * @param transform the transform to apply to the position
	 * @return the transformed position
	 */
	template<Dimension native, Dimension transforming>
	vec<native> transformPosition(const vec<native>& position, const mat<transforming>& transform) {
		return transformHomogeneous<native, transforming>(position, transform, 1.f);
	}

	/**
	 * @brief Transforms a Vector given a Matrix, vector is padded to use Homogeneous Coordinates,
	 * transformation is performed on the largest Dimension between vector and transform,
	 * if vector does not specify a component for a given Dimension the nullvector is assumed,
	 * if transform does not specify a Transform for a given Dimension Identity is assumend
	 * @tparam native the vectors native Dimension
	 * @tparam transforming the transforms Dimension
	 * @param vector the vector to transform
	 * @param transform the transform to apply to the vector
	 * @return the transformed vector
	 */
	template<Dimension native, Dimension transforming>
	vec<native> transformVector(const vec<native>& vector, const mat<transforming>& transform) {
		return transformHomogeneous<native, transforming>(vector, transform, 0.f);
	}

}