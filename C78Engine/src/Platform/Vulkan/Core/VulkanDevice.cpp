#include "C78ePCH.h"
#include "VulkanDevice.h"

namespace C78E {

    #ifndef C78_DEBUG
    bool VulkanDevice::s_EnableValidationLayers = false;
    #else
    bool VulkanDevice::s_EnableValidationLayers = true;
    #endif

    const std::vector<const char*> c_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> c_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };



    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {
        C78_CORE_ERROR("VulkanValidationlayer: {}", pCallbackData->pMessage);
        return VK_FALSE;
    }

    VkResult createDebugUtilsMessengerEXT(VkInstance m_Instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) { return func(m_Instance, pCreateInfo, pAllocator, pDebugMessenger); }
        else { return VK_ERROR_EXTENSION_NOT_PRESENT; }
    }

    void destroyDebugUtilsMessengerEXT(VkInstance m_Instance, VkDebugUtilsMessengerEXT m_DebugMessenger, const VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr( m_Instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) { func(m_Instance, m_DebugMessenger, pAllocator); }
    }


    VulkanDevice* VulkanDevice::s_Device = nullptr;

    VulkanDevice& VulkanDevice::get() { return *s_Device; }


    VulkanDevice::VulkanDevice(Window& window)
        : m_Window(window) {

        C78_CORE_ASSERT(!s_Device, "big Problem... redesign!");
        s_Device = this;
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createCommandPool();
    }

    VulkanDevice::~VulkanDevice() {
        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
        vkDestroyDevice(m_Device, nullptr);
        if (s_EnableValidationLayers) { destroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr); }
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }




    void VulkanDevice::createInstance() {
        if (s_EnableValidationLayers && !checkValidationLayerSupport()) {
            C78_CORE_ERROR("VulkanDevice::createInstance: ValidationLayerSupport missing!");
            C78_ASSERT(false);
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = m_Window.getWindowProperties().title.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = C78E_ENGINE_TITLE;
        appInfo.engineVersion = VK_MAKE_VERSION(C78E_VERSION_MAJOR, C78E_VERSION_MINOR, C78E_VERSION_PATCH);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if (s_EnableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(c_ValidationLayers.size());
            createInfo.ppEnabledLayerNames = c_ValidationLayers.data();

            debugCreateInfo = createDebugMessengerCreateInfo();
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createInstance: Failed to create Instance!");
            C78_ASSERT(false);
        }

        if (!verifyGflwRequiredInstanceExtensions()) {
            C78_CORE_ERROR("VulkanDevice::createInstance: Failed to match GLFW extenxion requirements!");
            C78_ASSERT(false);
        }
    }

    void VulkanDevice::setupDebugMessenger() {
        if (!s_EnableValidationLayers) return;
        VkDebugUtilsMessengerCreateInfoEXT createInfo = createDebugMessengerCreateInfo();
        if (createDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::setupDebugMessenger: Failed to create Vulkan Debug Messenger!");
            C78_ASSERT(false);
        }
    }

    void VulkanDevice::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            C78_CORE_ERROR("VulkanDevice::pickPhysicalDevice: No Vulkan supported GPUs found!");
            C78_ASSERT(false);
        }
        C78_CORE_TRACE("Vulkan supported GPUs found: {}", deviceCount);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

        for (const auto &device : devices) {
            if (isDeviceSuitable(device, m_Surface)) {
                m_PhysicalDevice = device;
                C78_CORE_WARN("   -> first suitable GPU selected!");
                break;
            }
        }
        if (m_PhysicalDevice == VK_NULL_HANDLE) {
            C78_CORE_ERROR("VulkanDevice::pickPhysicalDevice: No Vulkan supported and suitable GPU found!");
            C78_ASSERT(false);
        }

        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);
        C78_CORE_TRACE("   GPU selected: {}", properties.deviceName);
    }

    void VulkanDevice::createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(m_PhysicalDevice, m_Surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(c_DeviceExtensions.size());
        createInfo.ppEnabledExtensionNames = c_DeviceExtensions.data();

        // might not really be necessary anymore because device specific validation layers
        // have been deprecated
        if (s_EnableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(c_ValidationLayers.size());
            createInfo.ppEnabledLayerNames = c_ValidationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(m_Device, indices.graphicsFamily, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.presentFamily, 0, &m_PresentQueue);
    }

    void VulkanDevice::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createCommandPool: Failed to create CommandPool!");
            C78_ASSERT(false);
        }
    }

    void VulkanDevice::createSurface() {
        if (glfwCreateWindowSurface(m_Instance, Application::get().getWindow().getNativeWindow(), nullptr, &m_Surface) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createSurface: Failed to create Surface!");
            C78_ASSERT(false);
        }
    }

    

    

   

    VkFormat VulkanDevice::findSupportedFormat(
        const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
        C78_CORE_ERROR("VulkanDevice::findSupportedFormat: Failed to find supported Format!");
        C78_ASSERT(false);
    }

    uint32_t VulkanDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        C78_CORE_ERROR("VulkanDevice::findSupportedFormat: Failed to find suitable memory type!");
        C78_ASSERT(false);
    }

    void VulkanDevice::createBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer &buffer,
        VkDeviceMemory &bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createBuffer: Failed to create Buffer!");
            C78_ASSERT(false);
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_Device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createBuffer: Failed to allocate Buffer Memory!");
            C78_ASSERT(false);
        }

        vkBindBufferMemory(m_Device, buffer, bufferMemory, 0);
    }

    VkCommandBuffer VulkanDevice::beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_Device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        return commandBuffer;
    }

    void VulkanDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_GraphicsQueue);

        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &commandBuffer);
    }

    void VulkanDevice::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;  // Optional
        copyRegion.dstOffset = 0;  // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);
    }

    void VulkanDevice::copyBufferToImage(
        VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = layerCount;

        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region);
        endSingleTimeCommands(commandBuffer);
    }

    void VulkanDevice::createImageWithInfo(
        const VkImageCreateInfo &imageInfo,
        VkMemoryPropertyFlags properties,
        VkImage &image,
        VkDeviceMemory &imageMemory) {
        if (vkCreateImage(m_Device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createImageWithInfo: Failed to create Vulkan Image!");
            C78_ASSERT(false);
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_Device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createImageWithInfo: Failed to allocate Image Memory!");
            C78_ASSERT(false);
        }

        if (vkBindImageMemory(m_Device, image, imageMemory, 0) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createImageWithInfo: Failed to bind Image Memory!");
            C78_ASSERT(false);
        }
    }

    VkImageView VulkanDevice::createImageView(VkImage image, VkFormat format) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(m_Device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            C78_CORE_ERROR("VulkanDevice::createImageWithInfo: Failed to create Texture Image View!");
            C78_ASSERT(false);
        }

        return imageView;
    }











    bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices = findQueueFamilies(device, surface);
        bool extensionsSupported = checkDeviceExtensionSupport(device);
        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }
        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
        return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    std::vector<const char*> VulkanDevice::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if (s_EnableValidationLayers) { extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); }
        return extensions;
    }

    bool VulkanDevice::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : c_ValidationLayers) {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) { return false; }
        }
        return true;
    }

    VulkanDevice::QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
                indices.graphicsFamilyHasValue = true;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.presentFamily = i;
                indices.presentFamilyHasValue = true;
            }
            if (indices.isComplete()) { break; }
            i++;
        }

        return indices;
    }

    bool VulkanDevice::verifyGflwRequiredInstanceExtensions() {
        C78_CORE_TRACE("VulkanDevice Checking GLFW Required Extensions..");
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        bool passed = true;
        C78_CORE_TRACE("VulkanDevice GLFW Required Extensions: ");
        for (const auto& required : getRequiredExtensions()) {
            bool extAvail = false;
            for (const auto& extension : extensions) {
                if (required == extension.extensionName) {
                    extAvail = true;
                    break;
                }
            }
            if (extAvail) {
                C78_CORE_TRACE("  {} found supported!", required);
            } else {
                passed = false;
                C78_CORE_ERROR("  {} found not supported!", required);
            }

        }
        return passed;
    }

    bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
        C78_CORE_TRACE("VulkanDevice Checking Required Extensions(c_DeviceExtensions)..");
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(c_DeviceExtensions.begin(), c_DeviceExtensions.end());
        for (const auto& extension : availableExtensions) {
            C78_CORE_TRACE("  {} found supported!", extension.extensionName);
            requiredExtensions.erase(extension.extensionName);
        }
        for (auto& ext : requiredExtensions) {
            C78_CORE_ERROR("  {} found not supported!", ext);
        }
        return requiredExtensions.empty();
    }

    VulkanDevice::SwapChainSupportDetails VulkanDevice::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }
        return details;
    }



    VkDebugUtilsMessengerCreateInfoEXT VulkanDevice::createDebugMessengerCreateInfo() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;  // Optional
        return createInfo;
    }

}