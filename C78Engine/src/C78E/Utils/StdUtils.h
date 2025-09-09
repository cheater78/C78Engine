#pragma once
#include "Utils.h"

//std lib
#include <iostream>
#include <memory>
#include <variant>
#include <utility>
#include <algorithm>
#include <functional>

#include <filesystem>
#include <fstream>

#include <limits>
#include <cstdint>
#include <string>
#include <cstring>
#include <sstream>
#include <regex>
#include <tuple>
#include <ranges>
#include <vector>
#include <array>
#include <list>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <cassert>

#include <semaphore>
#include <barrier>
#include <mutex>
#include <shared_mutex>
#include <latch>
#include <thread>

#include <chrono>
#include <random>

#include <concepts>

// std extension
namespace std {

#ifndef _EXPORT_STD
#define _EXPORT_STD
#endif
#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

	//String
	_EXPORT_STD _NODISCARD inline string to_string(const char* _Val) {
		return string(_Val);
	}
	_EXPORT_STD _NODISCARD inline string to_string(string _Val) {
		return _Val;
	}

	template <typename T>
		requires is_floating_point_v<T>
	_EXPORT_STD _NODISCARD inline string to_string(T _Val, const size_t& _Dec) {
		string out;
		if constexpr (is_same_v<T, float>) {
			out = to_string(static_cast<float>(_Val));
		}
		else if constexpr (is_same_v < T, double>) {
			out = to_string(static_cast<double>(_Val));
		}
		else if constexpr (is_same_v < T, long double>) {
			out = to_string(static_cast<long double>(_Val));
		}
		else {
			return "NaN";
		}
		if (!_Dec || (_Dec == -1 && _Val == static_cast<T>(static_cast<long long>(_Val))) ) {
			return to_string((long long)_Val);
		}
		else if (_Dec == -1) {
			out.erase(out.find_last_not_of('0') + 1, string::npos);
			out.erase(out.find_last_not_of('.') + 1, string::npos);
			return out;
		}
		else {
			const size_t targetSize = out.find('.') + _Dec + 2;
			return (targetSize >= out.size()) ? out : out.substr(0, targetSize - 1);
		}
	}

	template <typename T>
	_EXPORT_STD _NODISCARD inline string to_string(const vector<T>& vec) {
		string str;
		for (auto& e : vec)
			str += ((str.empty()) ? "{ " : " ; ") + to_string(e);
		return str + " }";
	}

	template<typename T, typename S>
	_EXPORT_STD _NODISCARD vector<T> split(const T& s, S seperator) {
		vector<T> output;
		string::size_type prevPos = 0, pos = 0;

		while ((pos = s.find(seperator, pos)) != string::npos) {
			output.push_back(s.substr(prevPos, pos - prevPos));
			prevPos = ++pos;
		}

		output.push_back(s.substr(prevPos, pos - prevPos)); // Last word
		return output;
	}
	template
	vector<string> split<string, char>(const string& s, char seperator);
	template
	vector<string> split<string, const string&>(const string& s, const string& seperator);
	template
	vector<string_view> split<string_view, char>(const string_view& s, char seperator);
	template
	vector<string_view> split<string_view, const string&>(const string_view& s, const string& seperator);


	/**
	 * @brief concatenates a vector of strings into one string
	 * @param strs the string to concatenate
	 * @param del an optional delimiter in between the strings (first one after the first string, last one before the last string)
	 * @param start an optional index to offset the start of concat
	 * @param count an optional element count to stop after concatenating <count> elements
	 * @return the concatenated string
	 */
	_EXPORT_STD _NODISCARD inline string join(const vector<string>& strs, const string& del = "", size_t start = 0, size_t count = -1) {
		string output;

		assert(start < strs.size()); //TODO: no SmartLog Assert avail, maybe rearrange/move
		assert(count == -1 || start + count < strs.size());

		const auto& begin = strs.begin() + start;
		const auto& end = (count == -1) ? strs.end() : begin + count;
		output = *begin; // write first element, so del + elem can be appended
		for (auto it = begin + 1; it != end; it++) { // start at second elem, skips if one elem only
			output += del + *it; // append elems with leading delimiters -> no del after last elem
		}
		return output;
	}
	
	_EXPORT_STD _NODISCARD inline string replace(const string& str, const string& from, const string& to) {
		string output;
		if (from.empty())
			return output;
		output = str; // copy
		size_t startPos = 0;
		while ((startPos = output.find(from, startPos)) != string::npos) {
			output.replace(startPos, from.length(), to);
			startPos += to.length(); // avoid recursion
		}
		return output;
	}


	// string compare with caseSensitivity
	template<typename charT>
	struct nonCaseSensitiveEqual {
		nonCaseSensitiveEqual(const locale& loc) : m_Locale(loc) {}
		bool operator()(charT ch1, charT ch2) {
			return toupper(ch1, m_Locale) == toupper(ch2, m_Locale);
		}
	private:
		const locale& m_Locale;
	};

	template<typename T>
	size_t findInString(const T& base, const T& pattern, bool caseSensitive = true, const locale& loc = locale()) {
		if (caseSensitive)
			return base.find(pattern);
		else {
			typename T::const_iterator it = search(base.begin(), base.end(),
				pattern.begin(), pattern.end(), nonCaseSensitiveEqual<typename T::value_type>(loc));
			if (it != base.end()) return it - base.begin();
			else return string::npos;
		}
	}

	const std::array<const char*, 4> c_LineEndings = {
		"\r\n",
		"\n\r",
		"\n",
		"\r",
	};

	size_t str_find_first_line_ending(std::string_view str) {
		size_t bestFound = std::string::npos;
		for (const char* pattern : c_LineEndings) {
			size_t currentFound = str.find_first_of(pattern);
			if (currentFound < bestFound) {
				bestFound = currentFound;
			}
		}
		return bestFound;
	}

	size_t str_find_first_not_line_ending(std::string_view str) {
		size_t bestFound = std::string::npos;
		for (const char* pattern : c_LineEndings) {
			size_t currentFound = str.find_first_not_of(pattern);
			if (currentFound > bestFound) {
				bestFound = currentFound;
			}
		}
		return bestFound;
	}

	//source: https://stackoverflow.com/a/447307
	_EXPORT_STD _NODISCARD inline bool isFloat(const string& str) {
		istringstream iss(str);
		float f;
		iss >> noskipws >> f; // noskipws considers leading whitespace invalid
		// Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}
	//~

	//source: https://stackoverflow.com/a/4654718
	_EXPORT_STD _NODISCARD inline bool isNumerical(const string& s) {
		string::const_iterator it = s.begin();
		while (it != s.end() && isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}
	//~
	

	// Filesystem
	namespace filesystem {
		_EXPORT_STD _NODISCARD inline string getName(string filepath) { //TODO: check whether path.filename() returns folder names too
			// Extract name from filepath
			auto lastSlash = filepath.find_last_of("/\\");
			lastSlash = lastSlash == string::npos ? 0 : lastSlash + 1;
			auto lastDot = filepath.rfind('.');
			auto count = lastDot == string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
			return filepath.substr(lastSlash, count);
		}

		_EXPORT_STD _NODISCARD inline string getExtension(string filepath) {
			auto lastDot = filepath.rfind('.') + 1;
			return filepath.substr(lastDot, filepath.size());
		}
	}
}

#include <C78E/Utils/YamlUtils.h>
namespace YAML {

	// String View
	inline Emitter& operator<<(Emitter& out, const std::string_view& v) {
		out << std::string(v.data(), v.size());
		return out;
	}

}
