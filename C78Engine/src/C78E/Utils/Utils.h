

#ifndef _EXPORT_STD
#define _EXPORT_STD
#endif
#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace std {

    template <typename T> struct hash;

    // from: https://stackoverflow.com/a/57595105
	template <typename T, typename... Rest>
	void hashCombine(size_t& seed, const T& v, const Rest&... rest) {
		seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		(hashCombine(seed, rest), ...);
	};
	//~

}