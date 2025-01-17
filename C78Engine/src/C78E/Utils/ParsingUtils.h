#pragma once
#include <C78E/Utils/StdUtils.h>
#include <C78E/Utils/MathUtils.h>

namespace C78E {

	namespace TextParser {

		inline bool parseFloat(std::string_view str, float& value) {
			const bool valid = std::isFloat(str.data());
			if (valid)
				value = std::stof(str.data());
			return valid;
		}

		inline bool parseUnsignedInt(std::string_view str, size_t& value) {
			const bool valid = std::isNumerical(str.data());
			if (valid)
				value = std::stoull(str.data());
			return valid;
		}

		inline bool parseInfix(std::string_view line, std::string_view& infix, const std::string& prefix, const std::string& suffix) {
			const bool valid =
				(prefix.empty() || (prefix.size() < line.size() && line.substr(0, prefix.size()) == prefix)) &&
				(suffix.empty() || (suffix.size() < line.size() && line.substr(line.size() - suffix.size(), suffix.size()) == suffix));
			if (valid)
				infix = line.substr(prefix.size(), line.size() - prefix.size() - suffix.size());
			return valid;
		}

		template<typename Type, size_t parseElements, char vecDelimiter, bool(*parseFunction)(std::string_view, Type&)>
		inline bool parseVec(std::string_view str, glm::vec<parseElements, Type>& result, size_t& elements) {
			std::vector<std::string_view> chunks = std::split(str, vecDelimiter);
			if (chunks.size() > parseElements) return false;
			for (elements = 0; elements < chunks.size(); elements++) {
				Type value;
				if (!chunks[elements].empty() && parseFunction(chunks[elements], value))
					result[static_cast<glm::vec<parseElements, Type>::length_type>(elements)] = value;
			}
			return elements;
		}

		template<typename Type, size_t parseElements, const std::string& prefix, const std::string& suffix, char vecDelimiter, bool(*parseFunction)(std::string_view, Type&)>
		inline bool parseInfixVec(std::string_view line, glm::vec<parseElements, Type>& value) {
			std::string_view valueField;
			size_t elements;
			return parseInfix(line, valueField, prefix, suffix) && parseVec<Type, parseElements, vecDelimiter, parseFunction>(valueField, value, elements) && elements == parseElements;
		}

	}

}