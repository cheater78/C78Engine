#pragma once

#include <glm/glm.hpp>

//std lib
#include <string>


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
}

// glm impl
namespace std {
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