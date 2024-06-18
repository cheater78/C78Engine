#pragma once

#include <chrono>

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

		float elapsedSeconds() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		float elapsedMillis() {
			return elapsedSeconds() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}
