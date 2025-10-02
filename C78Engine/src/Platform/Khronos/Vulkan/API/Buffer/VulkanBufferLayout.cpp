#include "C78EPCH.h"
#include "VulkanBufferLayout.h"

namespace C78E {



	VkFormat toVkFormat(const ShaderDataType& shaderDataType) {
		const PrimitiveType type = shaderDataType;
		const size_t size = shaderDataType.elementCount();

		switch (type.raw()) {
		case PrimitiveType::None:				return VK_FORMAT_UNDEFINED;
		case PrimitiveType::Bool:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:
				switch (sizeof(bool)) {
				case 8u:				return VK_FORMAT_R8_UINT;
				case 16u:				return VK_FORMAT_R16_UINT;
				case 32u:				return VK_FORMAT_R32_UINT;
				case 64u:				return VK_FORMAT_R64_UINT;
				default:				return VK_FORMAT_UNDEFINED;
				}
			case 2u:
				switch (sizeof(bool)) {
				case 8u:				return VK_FORMAT_R8G8_UINT;
				case 16u:				return VK_FORMAT_R16G16_UINT;
				case 32u:				return VK_FORMAT_R32G32_UINT;
				case 64u:				return VK_FORMAT_R64G64_UINT;
				default:				return VK_FORMAT_UNDEFINED;
				}
			case 3u:
				switch (sizeof(bool)) {
				case 8u:				return VK_FORMAT_R8G8B8A8_UINT;
				case 16u:				return VK_FORMAT_R16G16B16_UINT;
				case 32u:				return VK_FORMAT_R32G32B32_UINT;
				case 64u:				return VK_FORMAT_R64G64B64_UINT;
				default:				return VK_FORMAT_UNDEFINED;
				}
			case 4u:
				switch (sizeof(bool)) {
				case 8u:				return VK_FORMAT_R8G8B8A8_UINT;
				case 16u:				return VK_FORMAT_R16G16B16A16_UINT;
				case 32u:				return VK_FORMAT_R32G32B32A32_UINT;
				case 64u:				return VK_FORMAT_R64G64B64A64_UINT;
				default:				return VK_FORMAT_UNDEFINED;
				}
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::Int8:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R8_SINT;
			case 2u:						return VK_FORMAT_R8G8_SINT;
			case 3u:						return VK_FORMAT_R8G8B8_SINT;
			case 4u:						return VK_FORMAT_R8G8B8A8_SINT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::Int16:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R16_SINT;
			case 2u:						return VK_FORMAT_R16G16_SINT;
			case 3u:						return VK_FORMAT_R16G16B16_SINT;
			case 4u:						return VK_FORMAT_R16G16B16A16_SINT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::Int32:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R32_SINT;
			case 2u:						return VK_FORMAT_R32G32_SINT;
			case 3u:						return VK_FORMAT_R32G32B32_SINT;
			case 4u:						return VK_FORMAT_R32G32B32A32_SINT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::Int64:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R64_SINT;
			case 2u:						return VK_FORMAT_R64G64_SINT;
			case 3u:						return VK_FORMAT_R64G64B64_SINT;
			case 4u:						return VK_FORMAT_R64G64B64A64_SINT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::UInt8:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R8_UINT;
			case 2u:						return VK_FORMAT_R8G8_UINT;
			case 3u:						return VK_FORMAT_R8G8B8_UINT;
			case 4u:						return VK_FORMAT_R8G8B8A8_UINT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::UInt16:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R16_UINT;
			case 2u:						return VK_FORMAT_R16G16_UINT;
			case 3u:						return VK_FORMAT_R16G16B16_UINT;
			case 4u:						return VK_FORMAT_R16G16B16A16_UINT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::UInt32:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R32_UINT;
			case 2u:						return VK_FORMAT_R32G32_UINT;
			case 3u:						return VK_FORMAT_R32G32B32_UINT;
			case 4u:						return VK_FORMAT_R32G32B32A32_UINT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::UInt64:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R64_UINT;
			case 2u:						return VK_FORMAT_R64G64_UINT;
			case 3u:						return VK_FORMAT_R64G64B64_UINT;
			case 4u:						return VK_FORMAT_R64G64B64A64_UINT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::Float16:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R16_SFLOAT;
			case 2u:						return VK_FORMAT_R16G16_SFLOAT;
			case 3u:						return VK_FORMAT_R16G16B16_SFLOAT;
			case 4u:						return VK_FORMAT_R16G16B16A16_SFLOAT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::Float32:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R32_SFLOAT;
			case 2u:						return VK_FORMAT_R32G32_SFLOAT;
			case 3u:						return VK_FORMAT_R32G32B32_SFLOAT;
			case 4u:						return VK_FORMAT_R32G32B32A32_SFLOAT;
			default:						return VK_FORMAT_UNDEFINED;
			}
		case PrimitiveType::Double64:
			switch (size) {
			case 0u:						return VK_FORMAT_UNDEFINED;
			case 1u:						return VK_FORMAT_R64_SFLOAT;
			case 2u:						return VK_FORMAT_R64G64_SFLOAT;
			case 3u:						return VK_FORMAT_R64G64B64_SFLOAT;
			case 4u:						return VK_FORMAT_R64G64B64A64_SFLOAT;
			default:						return VK_FORMAT_UNDEFINED;
			}

		default:								return VK_FORMAT_UNDEFINED;
		}


		return VkFormat();
	}

}
