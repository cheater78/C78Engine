#pragma once
#include <C78E/Core/Log/SmartLog.h>
#include "GLM.h"


namespace C78E::Math {

	/**
	 * @brief scalar type, to represent real numbers
	 */
	using scalar = float;
	//TODO: unify and manage scalar, float / double, handle .0f / .0

	/**
	 * @brief scalar limit, to provide quick access to special floating point type values
	 */
	using scalar_limit = std::numeric_limits<scalar>;

	constexpr scalar nearZeroValue = 1e-6f;
	inline bool isNearZero(const scalar s) {
		return glm::abs(s) < nearZeroValue;
	}

	inline scalar clamp(scalar v, scalar lo, scalar hi) {
		C78E_CORE_VALIDATE(lo <= hi, return glm::clamp(v, hi, lo), "C78E::Math::clamp: lower bound was greater that upper bound!(swapped, moving on)");
		return glm::clamp(v, lo, hi);
	}

	inline scalar smoothStep(scalar lo, scalar hi, scalar interpolate) {
		C78E_CORE_VALIDATE(lo <= hi, return glm::smoothstep(hi, lo, interpolate), "C78E::Math::smoothStep: lower bound was greater that upper bound!(swapped, moving on)");
		return glm::smoothstep(lo, hi, interpolate);
	}

	inline scalar snap(scalar v, scalar lo, scalar hi) {
		const scalar distToLo = lo - v;
		const scalar distToHi = hi - v;
		return (glm::abs(distToLo) == glm::abs(distToHi)) ? v : ((glm::abs(distToLo) < glm::abs(distToHi)) ? lo : hi);
	}

}