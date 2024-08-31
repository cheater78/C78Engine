#include "C78EPCH.h"
#include "UUID.h"

#include <random>
#include <unordered_map>

namespace C78E {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_EngineL(s_RandomDevice());
	static std::mt19937_64 s_EngineU(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID() { 
		m_UUID[0] = s_UniformDistribution(s_EngineL);
		m_UUID[1] = s_UniformDistribution(s_EngineU);
	}


	UUID UUID::fromString(std::string str) {
		UUID uuid = UUID::invalid();
		if ( str.length() != 45 || str.find("UUID::") == std::string::npos ) {
			C78_CORE_ERROR("UUID::fromString: UUID string provided was invalid!");
			C78_CORE_TRACE("  {}", str);
		}
		else {
			auto chunks = std::split(str.substr(6), ':');
			if(chunks.size() != 8) {
				C78_CORE_ERROR("UUID::fromString: UUID Decode failed!");
				C78_CORE_TRACE("  {}", str);
				C78_CORE_TRACE("     -> {}", std::to_string(chunks));
			}
			else {
				std::stringstream lo;
				lo << std::hex << chunks.at(4) << chunks.at(5) << chunks.at(6) << chunks.at(7);
				lo >> uuid.m_UUID[0];

				std::stringstream up;
				up << std::hex << chunks.at(0) << chunks.at(1) << chunks.at(2) << chunks.at(3);
				up >> uuid.m_UUID[1];
			}
		}
		return uuid;
	}

	std::string UUID::toString(UUID uuid) {
		char str[] = "UUID::0000:0000:0000:0000:0000:0000:0000:0000";
		const std::string lo = (std::stringstream() << std::uppercase << std::hex << uuid.m_UUID[0] << std::dec).str();
		const std::string up = (std::stringstream() << std::uppercase << std::hex << uuid.m_UUID[1] << std::dec).str();
		const size_t loSiz = lo.length();
		const size_t upSiz = up.length();
		const size_t loPad = 16 - loSiz;
		const size_t upPad = 16 - upSiz;

		for (uint8_t i = 44; i > 5; i--)
			if (i % 5 == 0 || ((i < 25) && (i - 5 < 1 + ((5 * upPad) / 4)) || ((i > 25) && (i - 25 < 1 + ((5 * loPad) / 4)))))
				continue;
			else *(str + i) = (i < 25) ? *((char*)up.c_str() + ((4 * (i - 05) / 5) - upPad)) : *((char*)lo.c_str() + ((4 * (i - 25) / 5) - loPad));
		return str;
	}

}
