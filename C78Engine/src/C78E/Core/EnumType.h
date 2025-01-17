#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Utils/StdUtils.h>

#include <stdint.h>
#include <string>

namespace C78E {

#define C78E_ENUMTYPE_BEGIN(BaseType, EnumTypeName, ...) 																				\
	class EnumTypeName {																												\
	public:																																\
		enum : BaseType {																												\
			__VA_ARGS__,																												\
			TYPE_SIZE																													\
		};																																\
		EnumTypeName() : m_EnumTypeName{ 0 } {}																							\
		EnumTypeName(BaseType type) : m_EnumTypeName(type) {}																			\
		EnumTypeName(EnumTypeName& other) = default;																					\
		~EnumTypeName() = default;																										\
																																		\
		bool operator==(const EnumTypeName& other) const { return this->m_EnumTypeName == other.m_EnumTypeName; }						\
		bool operator==(BaseType other) const { return this->m_EnumTypeName == other; }													\
		bool operator!=(const EnumTypeName& other) const { return this->m_EnumTypeName != other.m_EnumTypeName; }						\
		bool operator!=(uint8_t other) const { return this->m_EnumTypeName != other; }													\
		bool operator<(const EnumTypeName& other) const { return this->m_EnumTypeName < other.m_EnumTypeName; }							\
		bool operator<(BaseType other) const { return this->m_EnumTypeName < other; }													\
		operator BaseType() const { return m_EnumTypeName; }																			\
		operator bool() const { return m_EnumTypeName; }																				\
// ~C78E_ENUMTYPE_BEGIN

#define C78E_ENUMTYPE_END(BaseType, EnumTypeName, ...)																					\
	private:																															\
		static std::array<std::string, static_cast<size_t>(TYPE_SIZE)> createLables(const std::string& csl) {							\
			std::array<std::string, static_cast<size_t>(TYPE_SIZE)> labels;																\
			std::vector<std::string> rawLabels = std::split(csl, ",");																	\
			C78E_CORE_ASSERT(rawLabels.size() == TYPE_SIZE, "EnumTypeName:initLabels: Enum Count does not match!");						\
			for (size_t i = 0; i < TYPE_SIZE; i++) {																					\
				C78E_CORE_ASSERT(!rawLabels[i].empty(), "EnumTypeName:initLabels: Label was empty!");									\
				labels[i] = std::string(#EnumTypeName "::");																			\
				for (char c : rawLabels[i])																								\
					if (c != ' ' && c != '\n')																							\
						labels[i] += c;																									\
			}																															\
		}																																\
		static const std::array<std::string, static_cast<size_t>(TYPE_SIZE)> c_Labels;													\
	public:																																\
		static std::string EnumTypeNameToString(const EnumTypeName& type) {																\
			return c_Labels[type];																										\
		}																																\
		static EnumTypeName EnumTypeNameFromString(const std::string& typeString) {														\
			for (size_t i = 0; i < TYPE_SIZE; i++)																						\
				if (c_Labels[i] == typeString)																							\
					return EnumTypeName(i);																								\
			return BaseType(-1);																										\
		}																																\
	private:																															\
		BaseType m_EnumTypeName;																										\
	};																																	\
	const std::array<std::string, static_cast<size_t>(EnumTypeName::TYPE_SIZE)> EnumTypeName::c_Labels = EnumTypeName::createLables(#__VA_ARGS__);			\
// ~C78E_ENUMTYPE_END


#define C78E_ENUMTYPE(BaseType, EnumTypeName, ...)	\
C78E_ENUMTYPE_BEGIN(BaseType, EnumTypeName, ...)	\
C78E_ENUMTYPE_END(BaseType, EnumTypeName, ...)		\
// ~C78E_ENUMTYPE																												

#define C78E_CREATE_ENUMTYPE8(EnumTypeName, ...) C78E_CREATE_ENUMTYPE(uint8_t, EnumTypeName, __VA_ARGS__)
#define C78E_CREATE_ENUMTYPE16(EnumTypeName, ...) C78E_CREATE_ENUMTYPE(uint16_t, EnumTypeName, __VA_ARGS__)
#define C78E_CREATE_ENUMTYPE32(EnumTypeName, ...) C78E_CREATE_ENUMTYPE(uint32_t, EnumTypeName, __VA_ARGS__)
#define C78E_CREATE_ENUMTYPE64(EnumTypeName, ...) C78E_CREATE_ENUMTYPE(uint64_t, EnumTypeName, __VA_ARGS__)

}