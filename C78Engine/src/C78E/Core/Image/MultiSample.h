#pragma once

namespace C78E {
	
	class MultiSample {
	public: 
		enum MultisampleCount : uint32_t {
			X1 = BIT(1), Off = X1, None = X1,
			X2 = BIT(2),
			X4 = BIT(3),
			X8 = BIT(4),
			X16 = BIT(5),
			X32 = BIT(6),
			X64 = BIT(7)
		};

	public:
		MultiSample(MultisampleCount count);
		~MultiSample() = default;

		uint32_t getSampleCount() const;

		operator uint32_t() const;
		operator uint32_t&();
		MultiSample& operator=(const MultiSample& other) {
			m_SampleCount = other.m_SampleCount;
			return *this;
		}

	private:
		uint32_t m_SampleCount = 0;
	};

}
