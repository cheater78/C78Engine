#pragma once

namespace C78E {

	class SwapInterval {
	public:
		enum SwapIntervalType {
			Immediate = 0,
			Vsync = 1,
		};
	public:
		static std::string swapIntervalToString(SwapInterval swapInterval);
		static SwapInterval swapIntervalFromString(const std::string& swapIntervalString);
	public:
		SwapInterval();
		SwapInterval(SwapIntervalType swapInterval);
		~SwapInterval();

		SwapIntervalType raw() const;

		bool operator==(const SwapInterval& other) const;
		bool operator!=(const SwapInterval& other) const;

	private:
		SwapIntervalType m_SwapInterval = SwapIntervalType::Immediate;
	};

}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::SwapInterval& swapInterval) {
		return C78E::SwapInterval::swapIntervalToString(swapInterval);
	}

}

template<>
DefineFormatter(C78E::SwapInterval, std::to_string(input));
