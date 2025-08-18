#include "C78EPCH.h"
#include "Hash.h"

namespace C78E {

	inline const size_t& Hash::raw() const {
		return m_Hash;
	}
	inline size_t Hash::combine(size_t hash0, size_t hash1) {
		return hash0 ^ hash1 + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2);
	}
	inline size_t Hash::hashByte(const uint8_t& byte) {
		return std::hash<uint8_t>{}(byte);
	}
	inline size_t Hash::hashBytes(const void* data, size_t size) {
		const uint8_t* bytes = static_cast<const uint8_t*>(data);
		size_t seed = 0;
		for (size_t i = 0; i < size; i++) {
			seed = combine(seed, hashByte(bytes[i]));
		}
		return seed;
	}
}