#pragma once
#include "GLM.h"
#include <C78E/Utils/StdUtils.h>

namespace C78E::Math {

	/**
	 * @brief Dimension type, to specify the dimensionality of Math objects
	 */
	using Dimension = glm::length_t;

	/**
	 * @brief Dimensions enum, having predefined dimensionalities
	 */
	enum Dimensions : Dimension {
		None = 0,
		D2 = 2,
		D3 = 3,
		D4 = 4
	};

}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(const ::C78E::Math::Dimensions& dimension) {
		return "Dimension::" + to_string(static_cast<::C78E::Math::Dimension>(dimension)) + "D"; // requires to_string for Dimension -> size_t exists!
	}

	template<>
	struct hash<::C78E::Math::Dimensions> {
		size_t operator()(const ::C78E::Math::Dimensions& dimension) const {
			return static_cast<size_t>(dimension);
		}
	};

}