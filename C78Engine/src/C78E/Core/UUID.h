#pragma once

namespace C78E {

	class UUID {
	public:
		UUID();
		UUID(uint64_t upper, uint64_t lower);
		UUID(const UUID&) = default;

		bool isValid() const { return (bool)*this; }

		void* data() const { return (void*)&m_UUID[0]; }
		const size_t size() const { return 2 * sizeof(uint64_t); }

		operator bool() const { return (m_UUID[0] | m_UUID[1]); }
		friend bool operator<(const UUID& l, const UUID& r) { return l.m_UUID[1] < r.m_UUID[1] || (l.m_UUID[1] == r.m_UUID[1] && l.m_UUID[0] < r.m_UUID[0]); }
		friend bool operator==(const UUID& l, const UUID& r) { return l.m_UUID[1] == r.m_UUID[1] && l.m_UUID[0] == r.m_UUID[0]; }

	public:
		static bool decodesToUUID(const std::string& str);
		static bool decodesToValidUUID(const std::string& str);
		static UUID decodeFromString(const std::string& str);
		static std::string encodeToString(UUID uuid);

		static UUID invalid() { UUID uuid; uuid.m_UUID[0] = 0; uuid.m_UUID[1] = 0; return uuid; }
	private:
		uint64_t m_UUID[2] = { 0 };
	};

}

namespace std {
	
	template <>
	struct hash<C78E::UUID> {
		size_t operator()(const C78E::UUID& uuid) const {
			size_t seed = 0;
			hashCombine(seed, ((uint64_t*)uuid.data())[0], ((uint64_t*)uuid.data())[1]);
			return seed;
		}
	};

	_EXPORT_STD _NODISCARD inline string to_string(C78E::UUID uuid) {
		return C78E::UUID::encodeToString(uuid);
	}
}

#include <C78E/Utils/YamlUtils.h>
namespace YAML {

	// UUID
	inline Emitter& operator<<(Emitter& out, const C78E::UUID& id) {
		out << C78E::UUID::encodeToString(id);
		return out;
	}
	template <>
	struct convert<C78E::UUID> {
		static Node encode(const C78E::UUID& id) {
			Node node;
			node.push_back(C78E::UUID::encodeToString(id));
			return node;
		}

		static bool decode(const Node& node, C78E::UUID& id) {
			if(!C78E::UUID::decodesToUUID(node.as<std::string>())) return false;
			id = C78E::UUID::decodeFromString(node.as<std::string>());
			return true;
		}

	};

}