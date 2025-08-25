#pragma once

namespace C78E {

	class Timestep {
	public:
		Timestep(double timeSeconds = 0.0f)
			: m_Time(timeSeconds) {
		}

		double getSeconds() const { return m_Time; }
		double getMilliSeconds() const { return m_Time * 1000.0; }
		double getMicroSeconds() const { return m_Time * 1000000.0; }
		double getNanoSeconds() const { return m_Time * 1000000000.0; }
	private:
		double m_Time; // time in Seconds
	};

}
