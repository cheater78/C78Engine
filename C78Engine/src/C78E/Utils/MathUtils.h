#pragma once
#include "StdUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(glm::vec1 _Val, const uint32_t& _Dec = -1) {
		return "(" + to_string(_Val[0], _Dec) + ")";
	}
	_EXPORT_STD _NODISCARD inline string to_string(glm::vec2 _Val, const uint32_t& _Dec = -1) {
		return "(" + to_string(_Val[0], _Dec) + "," + to_string(_Val[1], _Dec) + ")";
	}
	_EXPORT_STD _NODISCARD inline string to_string(glm::vec3 _Val, const uint32_t& _Dec = -1) {
		return "(" + to_string(_Val[0], _Dec) + "," + to_string(_Val[1], _Dec) + "," + to_string(_Val[2], _Dec) + ")";
	}
	_EXPORT_STD _NODISCARD inline string to_string(glm::vec4 _Val, const uint32_t& _Dec = -1) {
		return "(" + to_string(_Val[0], _Dec) + "," + to_string(_Val[1], _Dec) + "," + to_string(_Val[2], _Dec) + "," + to_string(_Val[3], _Dec) + ")";
	}

    template <>
	struct hash<glm::vec1> {
		size_t operator()(glm::vec1 const& vec) const {
			size_t seed = 0;
			hashCombine(seed, vec);
			return seed;
		}
	};

	template <>
	struct hash<glm::vec2> {
		size_t operator()(glm::vec1 const& vec) const {
			size_t seed = 0;
			hashCombine(seed, vec[0], vec[1]);
			return seed;
		}
	};

	template <>
	struct hash<glm::vec3> {
		size_t operator()(glm::vec1 const& vec) const {
			size_t seed = 0;
			hashCombine(seed, vec[0], vec[1], vec[2]);
			return seed;
		}
	};

	template <>
	struct hash<glm::vec4> {
		size_t operator()(glm::vec1 const& vec) const {
			size_t seed = 0;
			hashCombine(seed, vec[0], vec[1], vec[2], vec[3]);
			return seed;
		}
	};
}