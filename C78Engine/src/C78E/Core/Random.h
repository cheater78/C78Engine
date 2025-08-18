#pragma once
#include <C78E/Utils/StdUtils.h>

namespace C78E {

	class Random {
	public:
		template<typename T>
		static T getInt(T min, T max) {
			std::uniform_int_distribution<T> s_UniformDistribution{ min, max };
			return s_UniformDistribution(s_Engine);
		}

		static float getFloat(float min = 0.f, float max = 1.f) {
			std::uniform_real_distribution<float> s_UniformDistribution{min, max};
			return s_UniformDistribution(s_Engine);
		}

		static double getDouble(double min = 0.f, double max = 1.f) {
			std::uniform_real_distribution<double> s_UniformDistribution{ min, max };
			return s_UniformDistribution(s_Engine);
		}

	private:
		static std::random_device s_RandomDevice;
		static std::mt19937_64 s_Engine;
	};

}