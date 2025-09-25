#pragma once

#include <C78E/Core/Image/ImageFormat.h>
#include <C78E/Core/Image/ColorSpace.h>
#include <C78E/Core/Image/MultiSample.h>

#include <C78E/Graphics/API/SwapChain/SwapInterval.h>

namespace C78E {
    
    // ImageFormat - VkFormat
    VkFormat toVkFormat(ImageFormat imageFormat);
    ImageFormat toImageFormat(VkFormat format);

    // ImageSize - VkExtent2D
    VkExtent2D toVkExtent(const ImageSize& size);
    ImageSize toImageSize(const VkExtent2D& extent);

    // SwapInterval - VkPresentModeKHR
    VkPresentModeKHR toVkPresentModeKHR(const SwapInterval& swapInterval);
    SwapInterval toSwapInterval(const VkPresentModeKHR& swapInterval);

    // ColorSpace - VkColorSpaceKHR
    VkColorSpaceKHR toVkColorSpaceKHR(const ColorSpace& colorSpace);
    ColorSpace toColorSpace(const VkColorSpaceKHR& colorSpace);

}
