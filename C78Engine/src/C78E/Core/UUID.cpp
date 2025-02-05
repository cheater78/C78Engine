#include "C78EPCH.h"
#include "UUID.h"

namespace C78E {

	static const std::regex uuid_regex{ "UUID::([[:xdigit:]]{4}):([[:xdigit:]]{4}):([[:xdigit:]]{4}):([[:xdigit:]]{4}):([[:xdigit:]]{4}):([[:xdigit:]]{4}):([[:xdigit:]]{4}):([[:xdigit:]]{4})" };
	static const char* uuid_invalid{ "UUID::Invalid" };

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_EngineL(s_RandomDevice());
	static std::mt19937_64 s_EngineU(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID() { 
		m_UUID[0] = s_UniformDistribution(s_EngineL);
		m_UUID[1] = s_UniformDistribution(s_EngineU);
	}

	bool UUID::decodesToUUID(const std::string& str) {
		if (str == uuid_invalid) return true;
		else return decodesToValidUUID(str);
	}

	bool UUID::decodesToValidUUID(const std::string& str) {
		std::smatch match;
		return std::regex_match(str, match, uuid_regex);
	}

	UUID UUID::decodeFromString(const std::string& str) {
		UUID uuid = UUID::invalid();
		if (str == uuid_invalid) return uuid;
		std::smatch match;
		if (std::regex_match(str, match, uuid_regex) && match.size() == 9) { // match[0] = uuid.encodeToString(), match[1-8] are the fragments
			std::stringstream lo;
			lo << std::hex << match[1] << match[2] << match[3] << match[4];
			lo >> uuid.m_UUID[1];

			std::stringstream up;
			up << std::hex  << match[5] << match[6] << match[7] << match[8];
			up >> uuid.m_UUID[0];
		}
		else {
			C78E_CORE_ERROR("UUID::decodeFromString: UUID Decode failed!");
			C78E_CORE_TRACE("  {}", str);
		}
		return uuid;
	}

	std::string UUID::encodeToString(UUID uuid) {
		char str[] = "UUID::0000:0000:0000:0000:0000:0000:0000:0000";
		if (uuid == UUID::invalid()) return "UUID::Invalid";
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
