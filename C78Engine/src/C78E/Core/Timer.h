#pragma once
#include <C78E/Utils/StdUtils.h>
#include "Timestep.h"

namespace C78E {

	class Timer {
	public:
		Timer() {
			reset();
		}
		~Timer() = default;

		void reset() {
			m_Start = std::chrono::high_resolution_clock::now();
		}

		Timestep elapsed() const {
			return Timestep(static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count()) * 0.000000001);
		}

		inline double elapsedSeconds() const {
			return elapsed().getSeconds();
		}

		inline double elapsedMilliSeconds() const {
			return elapsed().getMilliSeconds();
		}

		inline double elapsedMicroSeconds() const {
			return elapsed().getMicroSeconds();
		}

		inline double elapsedNanoSeconds() const {
			return elapsed().getNanoSeconds();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}
