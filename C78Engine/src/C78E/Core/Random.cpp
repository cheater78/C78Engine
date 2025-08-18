#include "C78EPCH.h"
#include "Random.h"

namespace C78E {

	std::random_device Random::s_RandomDevice{};
	std::mt19937_64 Random::s_Engine(s_RandomDevice());

}