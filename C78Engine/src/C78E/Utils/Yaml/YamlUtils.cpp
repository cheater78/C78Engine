#include "C78ePCH.h"
#include "YamlUtils.h"

namespace YAML {

	// String
	Emitter& operator<<(Emitter& out, const std::string_view& v) {
		out << std::string(v.data(), v.size());
		return out;
	}


	// UUID
	Emitter& operator<<(Emitter& out, const C78E::UUID& id) {
		out << C78E::UUID::encodeToString(id);
		return out;
	}


	// GLM::vec2
	Emitter& operator<<(Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}


	// GLM::vec3
	Emitter& operator<<(Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}


	// GLM::vec4
	Emitter& operator<<(Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	// GLM::vec<2, uint32_t>
	Emitter& operator<<(Emitter& out, const glm::vec<2, uint32_t>& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

}
