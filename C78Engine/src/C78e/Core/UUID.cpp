#include "C78EPCH.h"
#include "UUID.h"

#include <random>
#include <unordered_map>

namespace C78E {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{ }

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{ }

	UUID UUID::fromString(std::string string) {
		C78_CORE_FATAL("UUID::fromString not impl!");
		return UUID();
	}

	std::string UUID::toString(UUID uuid) {
		C78_CORE_FATAL("UUID::toString not impl!");
		return std::string();
	}

}
