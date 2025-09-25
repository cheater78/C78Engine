#include "C78EPCH.h"
#include "VulkanConversions.h"

namespace C78E {




    // ImageFormat - VkFormat
    VkFormat toVkFormat(ImageFormat imageFormat) {
        switch (imageFormat.format()) {
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

        case ImageFormat::ABGR8: return VK_FORMAT_B8G8R8A8_SRGB;
        default:
            C78E_CORE_ASSERT(false, "toVkFormat: Unsupported ShaderDataType!");
        }
        return VkFormat();
    }

    ImageFormat toImageFormat(VkFormat format) {
        switch (format) {
        case VK_FORMAT_R8_UINT:             return ImageFormat::R8;
        case VK_FORMAT_R16_SFLOAT:          return ImageFormat::R16F;
        case VK_FORMAT_R32_UINT:            return ImageFormat::R32;
        case VK_FORMAT_R32_SFLOAT:          return ImageFormat::R32F;
        case VK_FORMAT_D24_UNORM_S8_UINT:   return ImageFormat::D24S8;
        case VK_FORMAT_R8G8_UINT:           return ImageFormat::RG8;
        case VK_FORMAT_R16G16_SFLOAT:       return ImageFormat::RG16F;
        case VK_FORMAT_R32G32_SFLOAT:       return ImageFormat::RG32F;
        case VK_FORMAT_R8G8B8_UINT:         return ImageFormat::RGB8;
        case VK_FORMAT_R16G16B16_SFLOAT:    return ImageFormat::RGB16F;
        case VK_FORMAT_R32G32B32_SFLOAT:    return ImageFormat::RGB32F;
        case VK_FORMAT_R8G8B8A8_UINT:       return ImageFormat::RGBA8;
        case VK_FORMAT_R16G16B16A16_SFLOAT: return ImageFormat::RGBA16F;
        case VK_FORMAT_R32G32B32A32_SFLOAT: return ImageFormat::RGBA32F;
        case VK_FORMAT_B8G8R8A8_SRGB:       return ImageFormat::ABGR8;
        default: return ImageFormat::None;
        }
    }

    // ImageSize - VkExtent2D
    VkExtent2D toVkExtent(const ImageSize& size) {
        return VkExtent2D{
            static_cast<uint32_t>(size.x),
            static_cast<uint32_t>(size.y)
        };
    }

    ImageSize toImageSize(const VkExtent2D& extent) {
        return ImageSize{
            static_cast<glm::uint>(extent.width),
            static_cast<glm::uint>(extent.height)
        };
    }

    // SwapInterval - VkPresentModeKHR
    VkPresentModeKHR toVkPresentModeKHR(const SwapInterval& swapInterval) {
        switch (swapInterval.raw()) {
        case SwapInterval::Immediate:
            return VK_PRESENT_MODE_IMMEDIATE_KHR;
        case SwapInterval::Vsync:
            return VK_PRESENT_MODE_MAILBOX_KHR;
        default:
            return VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    SwapInterval toSwapInterval(const VkPresentModeKHR& swapInterval) {
        switch (swapInterval) {
        case VK_PRESENT_MODE_IMMEDIATE_KHR:
            return SwapInterval::Immediate;
        case VK_PRESENT_MODE_MAILBOX_KHR:
            return SwapInterval::Vsync;
        default:
            return SwapInterval::Immediate;
        }
    }

    // ColorSpace - VkColorSpaceKHR
    VkColorSpaceKHR toVkColorSpaceKHR(const ColorSpace& colorSpace) {
        switch (colorSpace.raw()) {
        case ColorSpace::sRGB:
            return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        case ColorSpace::sRGBL:
            return VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
        default:
            return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        }
    }

    ColorSpace toColorSpace(const VkColorSpaceKHR& colorSpace) {
        switch (colorSpace) {
        case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
            return ColorSpace::sRGB;
        case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:
            return ColorSpace::sRGBL;
        default:
            return ColorSpace::sRGB;
        }
    }

}
