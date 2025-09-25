#include "C78EPCH.h"
#include "MultiSample.h"

namespace C78E {

	MultiSample::MultiSample(MultisampleCount count)
		: m_SampleCount(count) {
	}

	uint32_t MultiSample::getSampleCount() const {
		return m_SampleCount;
	}

	MultiSample::operator uint32_t() const {
		return getSampleCount();
	}

	MultiSample::operator uint32_t& () {
		return m_SampleCount;
	}

}
