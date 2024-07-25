#pragma once

namespace C78E {

    class VulkanDevice {
    public:
        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        struct QueueFamilyIndices {
            uint32_t graphicsFamily;
            uint32_t presentFamily;
            bool graphicsFamilyHasValue = false;
            bool presentFamilyHasValue = false;
            bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
        };
    public:

        VulkanDevice(Window& window);
        ~VulkanDevice();
        
        VulkanDevice(const VulkanDevice &) = delete;
        VulkanDevice &operator=(const VulkanDevice &) = delete;
        VulkanDevice(VulkanDevice &&) = delete;
        VulkanDevice &operator=(VulkanDevice &&) = delete;

        VkCommandPool getCommandPool() { return m_CommandPool; }
        VkDevice device() { return m_Device; }
        VkSurfaceKHR surface() { return m_Surface; }
        VkQueue graphicsQueue() { return m_GraphicsQueue; }
        VkQueue presentQueue() { return m_PresentQueue; }

        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(m_PhysicalDevice); }
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(m_PhysicalDevice); }
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        // Buffer Helper Functions
        void createBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer &buffer,
            VkDeviceMemory &bufferMemory);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void copyBufferToImage(
            VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

        void createImageWithInfo(
            const VkImageCreateInfo &imageInfo,
            VkMemoryPropertyFlags properties,
            VkImage &image,
            VkDeviceMemory &imageMemory);

        VkPhysicalDeviceProperties properties;

        VkImageView createImageView(VkImage image, VkFormat format);

    private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();

    private:
        Window& m_Window;

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkCommandPool m_CommandPool;

        VkDevice m_Device;
        VkSurfaceKHR m_Surface;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;

    private:
        static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        static std::vector<const char*> getRequiredExtensions();
        static bool checkValidationLayerSupport();
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
        static bool verifyGflwRequiredInstanceExtensions();
        static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

        static VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo();

    private:
        static bool s_EnableValidationLayers;


    public:
        static VulkanDevice& get();
    private:
        static VulkanDevice* s_Device;
    };

}
