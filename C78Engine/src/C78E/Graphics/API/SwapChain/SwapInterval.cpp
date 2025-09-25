#include "C78EPCH.h"
#include "SwapInterval.h"

namespace C78E {

	std::string SwapInterval::swapIntervalToString(SwapInterval swapInterval) {
		switch (swapInterval.raw()) {
		case SwapIntervalType::Immediate: return "SwapIntervalType::Immediate";
		case SwapIntervalType::Vsync: return "SwapIntervalType::Vsync";
		default: return "SwapIntervalType::Immediate";
		}
	}

	SwapInterval SwapInterval::swapIntervalFromString(const std::string& swapIntervalString) {
		if (swapIntervalString == "SwapIntervalType::Immediate") return SwapIntervalType::Immediate;
		if (swapIntervalString == "SwapIntervalType::Vsync") return SwapIntervalType::Vsync;
		return SwapIntervalType::Immediate;
	}

	SwapInterval::SwapInterval()
		: m_SwapInterval(SwapIntervalType::Immediate) {
	}

	SwapInterval::SwapInterval(SwapIntervalType swapInterval)
		: m_SwapInterval(swapInterval) {
	}

	SwapInterval::~SwapInterval() {
	}

	SwapInterval::SwapIntervalType SwapInterval::raw() const {
		return m_SwapInterval;
	}

	bool SwapInterval::operator==(const SwapInterval& other) const {
		return raw() == other.raw();
	}
	bool SwapInterval::operator!=(const SwapInterval& other) const {
		return !operator==(other);
	}

}
