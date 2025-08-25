#pragma once

namespace C78E {
    /*
    VkSurfaceCapabilitiesKHR getSurfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        VkSurfaceCapabilitiesKHR capabilities{};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);
        return capabilities;
    }
    uint32_t getSurfaceFormatCount(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
        return formatCount;
    }
    std::vector<VkSurfaceFormatKHR> getSurfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        uint32_t formatCount = getSurfaceFormatCount(physicalDevice, surface);
        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        if (formatCount) {
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());
        }
        return surfaceFormats;
    }
    uint32_t getPresentModeCount(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
        return presentModeCount;
    }
    std::vector<VkPresentModeKHR> getPresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        uint32_t presentModeCount = getPresentModeCount(physicalDevice, surface);
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        if (presentModeCount) {
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
        }
        return presentModes;
    }
    */
}
