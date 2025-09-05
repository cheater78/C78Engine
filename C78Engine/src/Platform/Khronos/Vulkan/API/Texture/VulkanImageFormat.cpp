#include "C78EPCH.h"
#include "VulkanImageFormat.h"

namespace C78E {

	VkFormat toVkFormat(ImageFormat imageFormat) {
		switch(imageFormat.format()) {
		case ImageFormat::R8: return VK_FORMAT_R8_UINT;
		case ImageFormat::R16F: return VK_FORMAT_R16_SFLOAT;
		case ImageFormat::R32: return VK_FORMAT_R32_UINT;
		case ImageFormat::R32F: return VK_FORMAT_R32_SFLOAT;
		case ImageFormat::D24S8: return VK_FORMAT_D24_UNORM_S8_UINT;

		case ImageFormat::RG8: return VK_FORMAT_R8G8_UINT;
		case ImageFormat::RG16F: return VK_FORMAT_R16G16_SFLOAT;
		case ImageFormat::RG32F: return VK_FORMAT_R32G32_SFLOAT;


		case ImageFormat::RGB8: return VK_FORMAT_R8G8B8_UINT;
		case ImageFormat::RGB16F: return VK_FORMAT_R16G16B16_SFLOAT;
		case ImageFormat::RGB32F: return VK_FORMAT_R32G32B32_SFLOAT;


		case ImageFormat::RGBA8: return VK_FORMAT_R8G8B8A8_UINT;
		case ImageFormat::RGBA16F: return VK_FORMAT_R16G16B16A16_SFLOAT;
		case ImageFormat::RGBA32F: return VK_FORMAT_R32G32B32A32_SFLOAT;
		default:
			C78E_CORE_ASSERT(false, "toVkFormat: Unsupported ShaderDataType!");
		}
		return VkFormat();
	}

}