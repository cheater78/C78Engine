#pragma once

#include <C78E/Core/Types.h>

//std lib
#include <string>
#include <vector>

// external Libs
#include <glm/glm.hpp>


// std extension
namespace std {

	//String
	_EXPORT_STD _NODISCARD inline string to_string(const char* _Val) {
		std::string ret = _Val;
		return ret;
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

	_EXPORT_STD _NODISCARD inline std::vector<std::string> split(const std::string& s, char seperator)
	{
		std::vector<std::string> output;

		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = s.find(seperator, pos)) != std::string::npos)
		{
			std::string substring(s.substr(prev_pos, pos - prev_pos));

			output.push_back(substring);

			prev_pos = ++pos;
		}

		output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

		return output;
	}

	_EXPORT_STD _NODISCARD inline std::string join(const std::vector<std::string>& strs, std::string del = "", uint32_t start = 0, uint32_t count = -1)
	{
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

	// Filesystem
	namespace filesystem {
		_EXPORT_STD _NODISCARD inline string getName(string filepath) {
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

	// Hash for Vertex

	// from: https://stackoverflow.com/a/57595105
	template <typename T, typename... Rest>
	void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
		seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		(hashCombine(seed, rest), ...);
	};
	//~

	template <>
	struct hash<glm::vec1> {
		size_t operator()(glm::vec1 const& vec) const {
			size_t seed = 0;
			hashCombine(seed, vec);
			return seed;
		}
	};

	template <>
	struct hash<glm::vec2> {
		size_t operator()(glm::vec1 const& vec) const {
			size_t seed = 0;
			hashCombine(seed, vec[0], vec[1]);
			return seed;
		}
	};

	template <>
	struct hash<glm::vec3> {
		size_t operator()(glm::vec1 const& vec) const {
			size_t seed = 0;
			hashCombine(seed, vec[0], vec[1], vec[2]);
			return seed;
		}
	};

	template <>
	struct hash<glm::vec4> {
		size_t operator()(glm::vec1 const& vec) const {
			size_t seed = 0;
			hashCombine(seed, vec[0], vec[1], vec[2], vec[3]);
			return seed;
		}
	};

	template <>
	struct hash<C78E::Vertex> {
		size_t operator()(C78E::Vertex const& vertex) const {
			size_t seed = 0;
			hashCombine(seed, vertex.Position, vertex.Color, vertex.Normal, vertex.TexCoord, vertex.TexIndex);
			return seed;
		}
	};
	
	

	// GLM
	_EXPORT_STD _NODISCARD inline string to_string(glm::vec1 _Val, const uint32_t& _Dec = -1) {
		return "(" + to_string(_Val[0], _Dec) + ")";
	}
	_EXPORT_STD _NODISCARD inline string to_string(glm::vec2 _Val, const uint32_t& _Dec = -1) {
		return "(" + to_string(_Val[0], _Dec) + "," + to_string(_Val[1], _Dec) + ")";
	}
	_EXPORT_STD _NODISCARD inline string to_string(glm::vec3 _Val, const uint32_t& _Dec = -1) {
		return "(" + to_string(_Val[0], _Dec) + "," + to_string(_Val[1], _Dec) + "," + to_string(_Val[2], _Dec) + ")";
	}
	_EXPORT_STD _NODISCARD inline string to_string(glm::vec4 _Val, const uint32_t& _Dec = -1) {
		return "(" + to_string(_Val[0], _Dec) + "," + to_string(_Val[1], _Dec) + "," + to_string(_Val[2], _Dec) + "," + to_string(_Val[3], _Dec) + ")";
	}
}