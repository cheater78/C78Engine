#pragma once

namespace C78E {

	class Timestep {
	public:
		Timestep(float timeSeconds = 0.0f)
			: m_Time(timeSeconds)
		{ }

		float getSeconds() const { return m_Time; }
		float getMilliSeconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time; // time in Seconds
	};

}