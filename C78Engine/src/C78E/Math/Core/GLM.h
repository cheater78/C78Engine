#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/extended_min_max.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace C78E::Math {
	
	template <typename T>
	T log10(const T& x) {
		return glm::log(x) / glm::log(T(10));
	}

}

#include <C78E/Utils/StdUtils.h>
namespace std {

	// GLM::vec
	template<glm::length_t L, typename T, glm::qualifier Q>
		requires std::is_floating_point_v<T>
	_EXPORT_STD _NODISCARD inline string to_string(const glm::vec<L, T, Q>& vector, const size_t& decimals = -1) {
		string out;
		out.reserve(3 + static_cast<size_t>(log10(L)) + 2 + (L * (1 + 1 + decimals)) + 2); // reserve prefix, (one digit, comma and decimals) for each dim, suffix
		out += "vec" + to_string(L) + "( " + to_string(static_cast<T>(vector[0]), decimals);
		for(glm::length_t i = 1; i < L; i++) {
			out += ", " + to_string(static_cast<T>(vector[i]), decimals);
		}
		return out + " )";
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
		requires std::is_integral_v<T>
	_EXPORT_STD _NODISCARD inline string to_string(const glm::vec<L, T, Q>& vector) {
		string out;
		out.reserve(3 + static_cast<size_t>(log10(L)) + 2); // reserve prefix, suffix
		out += "vec" + to_string(L) + "( " + to_string(static_cast<T>(vector[0]));
		for (glm::length_t i = 1; i < L; i++) {
			out += ", " + to_string(static_cast<T>(vector[i]));
		}
		return out + " )";
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	struct hash<glm::vec<L, T, Q>> {
		size_t operator()(const glm::vec<L, T, Q>& vec) const {
			size_t seed = 0;
			for(glm::length_t i = 0; i < L; i++) {
				hashCombine(seed, vec[i]);
			}
			return seed;
		}
	};

	// GLM::quat
	_EXPORT_STD _NODISCARD inline string to_string(const glm::quat& quat, const size_t& decimals = -1) {
		string out;
		out.reserve(6 + (4 * (1 + 1 + decimals)) + 2); // reserve prefix, (one digit, comma and decimals) for each dim, suffix
		out += "quat( " + to_string(quat[0], decimals);
		for(glm::length_t i = 1; i < 4; i++) {
			out += ", " + to_string(quat[i], decimals);
		}
		return out + " )";
	}

	template<>
	struct hash<glm::quat> {
		size_t operator()(const glm::quat& quat) const {
			size_t seed = 0;
			for(glm::length_t i = 0; i < 4; i++) {
				hashCombine(seed, quat[i]);
			}
			return seed;
		}
	};

}

#include <C78E/Utils/YamlUtils.h>
namespace YAML {

	// GLM::vec
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Emitter& operator<<(Emitter& out, const glm::vec<L, T, Q>& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for(glm::length_t i = 0; i < L; i++) {
			out << v[i];
		}
		out << YAML::EndSeq;
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	struct convert<glm::vec<L, T, Q>> {
		static Node encode(const glm::vec<L, T, Q>& v) {
			Node node;
			for(glm::length_t i = 0; i < L; i++) {
				node.push_back(v[i]);
			}
			return node;
		}
		static bool decode(const Node& node, glm::vec<L, T, Q>& v) {
			if(!node.IsSequence() || node.size() != L)
				return false;
			for(glm::length_t i = 0; i < L; i++) {
				v[i] = node[i].as<T>();
			}
			return true;
		}

	};

	// GLM::quat
	inline Emitter& operator<<(Emitter& out, const glm::quat& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	template <>
	struct convert<glm::quat> {
		static Node encode(const glm::quat& v) {
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.push_back(v.w);
			return node;
		}

		static bool decode(const Node& node, glm::quat& v) {
			if(!node.IsSequence() || node.size() != 4)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			v.w = node[3].as<float>();
			return true;
		}

	};
}