#pragma once

namespace C78E {

	class UUID {
	public:
		UUID();
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
	template <typename T> struct hash;

	template<>
	struct hash<C78E::UUID> {
		size_t operator()(const C78E::UUID& uuid) const {
			return (uint64_t)uuid;
		}
	};

	_EXPORT_STD _NODISCARD inline string to_string(C78E::UUID uuid) {
		return C78E::UUID::encodeToString(uuid);
	}
}
