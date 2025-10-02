#pragma once

namespace C78E {

    struct VulkanQueueFamily {
        uint32_t queueFamilyIndex = -1;
        VkBool32 supportsPresent = VK_FALSE;
    };

    class VulkanPhysicalDevice {
    public:
        // Physical device
        VulkanPhysicalDevice(VkPhysicalDevice vkPhysicalDevice);
        ~VulkanPhysicalDevice();

        VkPhysicalDevice getVkPhysicalDevice() const;
        VkPhysicalDeviceProperties getPhysicalDeviceProperties() const;
        VkPhysicalDeviceFeatures getPhysicalDeviceFeatures() const;
        const std::vector<VkQueueFamilyProperties>& getQueueFamilyProperties() const;

        bool supportsExtensionByName(const char* extensionName) const;
        bool supportsExtensionsByName(const std::vector<const char*>& extensionNames) const;

        bool supportsUniversalPresent(const VkSurfaceKHR presentSurface = VK_NULL_HANDLE);
        bool supportsDedicatedQueues() const;
        bool supportsRayTracing() const;
        
	protected:
        void fetchQueueFamilies(const VkSurfaceKHR presentSurface = VK_NULL_HANDLE);
    private:
        void fetchPhysicalDeviceProperties();
        void fetchPhysicalDeviceFeatures();
        void fetchAvailableDeviceExtensions();

		void fetchQueueFamilyProperties();
        uint32_t getQueueFamilyIndexWith(const VkQueueFlags queueFlags, const VkQueueFlags mask = VK_QUEUE_FLAG_BITS_MAX_ENUM, const VkSurfaceKHR presentSurface = VK_NULL_HANDLE) const;

    protected:
        VkPhysicalDevice m_VkPhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties m_PhysicalDeviceProperties;
        VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures;
        std::vector<VkExtensionProperties> m_AvailableExtensions;

		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
        VulkanQueueFamily m_UniversialFamily; // main queue family that supports all operations
        VulkanQueueFamily m_ComputeFamily; // compute queue family
        VulkanQueueFamily m_TransferFamily; // transfer queue family
    };

    class VulkanDevice : public VulkanPhysicalDevice {
    public:
        VulkanDevice(VulkanPhysicalDevice& physicalDevice, VkSurfaceKHR surface, const std::vector<const char*>& extensions);
        ~VulkanDevice();

		VkDevice getVkDevice();
        //bool submitCommandBuffer(Ref<VulkanCommandBuffer> vulkanCommandBuffer);

        bool waitIdle() const;

        uint32_t getUniversalQueueFamilyIndex() const;
        uint32_t getComputeQueueFamilyIndex() const;
        uint32_t getTransferQueueFamilyIndex() const;

		VkQueue getUniversalVkQueue() const;
        VkQueue getPresentVkQueue() const;
        VkQueue getComputeVkQueue(uint32_t index = 0) const;
		VkQueue getTransferVkQueue() const;

        bool waitUniversalQueueIdle() const;
        bool waitComputeQueueIdle(uint32_t index = 0) const;
        bool waitTransferQueueIdle() const;

        VkCommandPool getUniversalVkCommandPool() const;
        VkCommandPool getPresentVkCommandPool() const;
        VkCommandPool getComputeVkCommandPool() const;
        VkCommandPool getTransferVkCommandPool() const;

    private:
        bool buildDeviceQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& deviceQueueCreateInfos);
        bool createDeviceQueues();
        bool createCommandPools();
        void destroyCommandPools();
    private:
        VkDevice m_VkDevice = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

        VkQueue m_UniversalQueue; // main queue
        float m_UniversalQueuePriority = 1.f;
        VkQueue m_ComputeQueues[2]; // compute queues
        float m_ComputeQueuesPriorities[2] = { 1.f, 1.f };
        VkQueue m_TransferQueue; // transfer queue
        float m_TransferQueuePriority = 1.f;

        VkCommandPool m_UniversalPool;
        VkCommandPool m_ComputePool;
        VkCommandPool m_TransferPool;

    };

    /**
	 * @brief static class to manage Vulkan devices. directly used by VulkanGraphicsInstance.
     */
    class VulkanDeviceManager {
    public:
        static void init(VkInstance vkInstance);
        
        static Ref<VulkanDevice> pickDevice(const VkSurfaceKHR presentSurface, const std::vector<const char*>& extensions);
		static const std::vector<VulkanPhysicalDevice>& getPhysicalDevices();
		static const VulkanPhysicalDevice& getPhysicalDevice(uint32_t index);

    private:
        static void fetchPhysicalDevices();
    private:
        static VkInstance s_VkInstance;
        static std::vector<VulkanPhysicalDevice> s_PhysicalDevices;
    };

}
