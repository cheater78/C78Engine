#include "C78ePCH.h"
#include "UUID.h"

#include <random>

namespace C78e {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_RandEngine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID() : m_UUID(s_UniformDistribution(s_RandEngine))
	{
	}

	UUID::UUID(uint64_t uuid) : m_UUID(uuid)
	{
	}
}



