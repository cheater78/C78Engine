#pragma once

namespace C78E {

	class UUID {
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		bool isValid() const { return (bool)*this; }

		operator uint64_t() const { return m_UUID; }
		operator bool() const { return m_UUID; }
		friend bool operator<(const UUID& l, const UUID& r) { return l.m_UUID < r.m_UUID; }
		friend bool operator==(const UUID& l, const UUID& r) { return l.m_UUID == r.m_UUID; }
	private:
		uint64_t m_UUID; // Change to 128 or 256
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
		return to_string((uint64_t)uuid);
	}
}
