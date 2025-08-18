#pragma once
/*
#ifdef C78E_DEBUG
#include <C78E/Core/Timer.h>

#ifndef PROFILER_INSTANCE_COUNT
#define PROFILER_INSTANCE_COUNT 0
#endif
#define INC_FUNCTION_COUNT (PROFILER_INSTANCE_COUNT + 1)
#undef PROFILER_INSTANCE_COUNT
#define PROFILER_INSTANCE_COUNT INC_FUNCTION_COUNT


#ifndef C78E_PROFILER_NAMES
#define C78E_PROFILER_NAMES ""
#endif
#define C78E_ADD_PROFILER_NAME(name) C78E_PROFILER_NAMES, name
#undef C78E_PROFILER_NAMES
#define PROFILER_INSTANCE_COUNT INC_FUNCTION_COUNT


namespace C78E {
	class GlobalProfiler {
	public:
		static void writeProfilerData(const size_t instanceID, const float time) {
			m_ProfilerFunctionTimes[instanceID] = time;
		}

	private:
		static constexpr std::array<const char*, PROFILER_INSTANCE_COUNT> m_ProfilerFunctionNames = {

		};
		static std::array<float, PROFILER_INSTANCE_COUNT> m_ProfilerFunctionTimes;
	};

#define C78E_PROFILE_FUNC() C78E::ScopeProfiler profiler(__func__, PROFILER_INSTANCE_COUNT);
#define C78E_PROFILE_SCOPE(name) C78E::ScopeProfiler profiler(name, PROFILER_INSTANCE_COUNT);

	class ScopeProfiler : public Timer {
		ScopeProfiler(const char* name, const size_t instanceID)
			: Timer(), m_Name(name), m_InstanceID(instanceID) {
		}
		~ScopeProfiler() {
			float elapsed = elapsedMillis();
			// write to GlobalProfiler
			GlobalProfiler::writeProfilerData(m_InstanceID, elapsed);
		}


	private:
		const char* m_Name;
		const size_t m_InstanceID;
	};

}
#endif
*/