#pragma once
#include "Utils.h"

//std lib
#include <iostream>
#include <memory>
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
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <cassert>

#include <semaphore>
#include <thread>


// std extension
namespace std {

	//String
	_EXPORT_STD _NODISCARD inline string to_string(const char* _Val) {
		return string(_Val);
	}
	_EXPORT_STD _NODISCARD inline string to_string(string _Val) {
		return _Val;
	}

	_EXPORT_STD _NODISCARD inline string to_string(double _Val, const uint32_t& _Dec) {
		string _out = to_string(_Val);
		if (!_Dec || (_Dec == -1 && _Val == static_cast<double>(static_cast<long long>(_Val))) )
			return to_string((long long)_Val);
		else if (_Dec == -1) {
			_out.erase(_out.find_last_not_of('0') + 1, std::string::npos);
			_out.erase(_out.find_last_not_of('.') + 1, std::string::npos);
			return _out;
		}
		else {
			const size_t _TargetSize = _out.find('.') + _Dec + 2;
			return (_TargetSize >= _out.size()) ? _out : _out.substr(0, _TargetSize - 1);
		}
	}

	_EXPORT_STD _NODISCARD inline string to_string(float _Val, const uint32_t& _Dec) {
		return to_string(static_cast<double>(_Val), _Dec);
	}

	template <typename T>
	_EXPORT_STD _NODISCARD inline string to_string(const std::vector<T>& vec) {
		std::string str;
		for (auto& e : vec)
			str += ((str.empty()) ? "{ " : " ; ") + std::to_string(e);
		return str + " }";
	}

	template<typename T>
	_EXPORT_STD _NODISCARD inline std::vector<std::string> split(const std::string& s, T seperator) {
		std::vector<std::string> output;
		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = s.find(seperator, pos)) != std::string::npos) {
			output.push_back(s.substr(prev_pos, pos - prev_pos));
			prev_pos = ++pos;
		}

		output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
		return output;
	}
	template
	std::vector<std::string> split<char>(const std::string& s, char seperator);
	template
	std::vector<std::string> split<const std::string&>(const std::string& s, const std::string& seperator);


	_EXPORT_STD _NODISCARD inline std::string join(const std::vector<std::string>& strs, std::string del = "", uint32_t start = 0, uint32_t count = -1) {
		std::string output = del;

		uint32_t wordInd = 0;
		for (std::string part : strs) {
			if (wordInd < start) continue;
			if (wordInd + 1 > count) break;
			if (wordInd + 1 == strs.size() || wordInd + 2 > count)
				output += part;
			else
				output += part + del;
			wordInd++;
		}

		return output;
	}
	

	// string compare with caseSensitivity
	template<typename charT>
	struct nonCaseSensitiveEqual {
		nonCaseSensitiveEqual(const std::locale& loc) : locale(loc) {}
		bool operator()(charT ch1, charT ch2) {
			return std::toupper(ch1, locale) == std::toupper(ch2, locale);
		}
	private:
		const std::locale& locale;
	};

	template<typename T>
	size_t findInString(const T& base, const T& pattern, bool caseSensitive = true, const std::locale& loc = std::locale()) {
		if (caseSensitive)
			return base.find(pattern);
		else {
			typename T::const_iterator it = std::search(base.begin(), base.end(),
				pattern.begin(), pattern.end(), nonCaseSensitiveEqual<typename T::value_type>(loc));
			if (it != base.end()) return it - base.begin();
			else return std::string::npos;
		}
	}

	

	// Filesystem
	namespace filesystem {
		_EXPORT_STD _NODISCARD inline string getName(string filepath) { //TODO: check whether path.filename() returns folder names too
			// Extract name from filepath
			auto lastSlash = filepath.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			auto lastDot = filepath.rfind('.');
			auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
			return filepath.substr(lastSlash, count);
		}

		_EXPORT_STD _NODISCARD inline string getExtension(string filepath) {
			auto lastDot = filepath.rfind('.') + 1;
			return filepath.substr(lastDot, filepath.size());
		}
	}
}