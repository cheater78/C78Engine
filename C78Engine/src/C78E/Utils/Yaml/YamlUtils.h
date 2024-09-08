#pragma once
#include <C78E/Core/UUID.h>

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML {

	// String
	Emitter& operator<<(Emitter& out, const std::string_view& v);


	// UUID
	Emitter& operator<<(Emitter& out, const C78E::UUID& id);
	template <>
	struct convert<C78E::UUID> {
		static Node encode(const C78E::UUID& id) {
			Node node;
			node.push_back(C78E::UUID::encodeToString(id));
			return node;
		}

		static bool decode(const Node& node, C78E::UUID& id) {
			id = C78E::UUID::decodeFromString(node.as<std::string>());
			return true;
		}

	};

	// GLM::vec2
	Emitter& operator<<(Emitter& out, const glm::vec2& v);
	template <>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& v) {
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& v) {
			if (!node.IsSequence() || node.size() != 2)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			return true;
		}

	};


	// GLM::vec3
	Emitter& operator<<(Emitter& out, const glm::vec3& v);
	template <>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& v) {
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& v) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			return true;
		}

	};


	// GLM::vec4
	Emitter& operator<<(Emitter& out, const glm::vec4& v);
	template <>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& v) {
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.push_back(v.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& v) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			v.w = node[3].as<float>();
			return true;
		}

	};


	// GLM::vec<2, uint32_t>
	Emitter& operator<<(Emitter& out, const glm::vec<2, uint32_t>& v);
	template <>
	struct convert<glm::vec<2, uint32_t>> {
		static Node encode(const glm::vec<2, uint32_t>& v) {
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec<2, uint32_t>& v) {
			if (!node.IsSequence() || node.size() != 2)
				return false;

			v.x = node[0].as<uint32_t>();
			v.y = node[1].as<uint32_t>();
			return true;
		}

	};

}

#define WRITE_SCRIPT_FIELD(FieldType, Type) case ScriptFieldType::FieldType: out << scriptField.GetValue<Type>(); break

#define READ_SCRIPT_FIELD(FieldType, Type)             \
	case ScriptFieldType::FieldType:                   \
	{                                                  \
		Type data = scriptField["Data"].as<Type>();    \
		fieldInstance.SetValue(data);                  \
		break;                                         \
	}