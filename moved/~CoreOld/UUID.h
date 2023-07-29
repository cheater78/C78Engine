#pragma once

#include <cstdint>
#include <xhash>

namespace C78e {

	class UUID {
	public:

		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }

		bool operator==(const UUID& other) { return m_UUID == other.m_UUID; }

	private:
		uint64_t m_UUID;
	};

}

namespace std {

	template<>
	struct hash<C78e::UUID>
	{
		std::size_t operator()(const C78e::UUID& uuid) const {
			return std::hash<uint64_t>()((uint64_t)uuid);
		}
	};

}