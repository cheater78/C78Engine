#include "C78EPCH.h"
#include "VulkanDevice.h"

//#include <Platform/Khronos/Vulkan/API/Command/VulkanCommandBuffer.h>

namespace C78E {

#define C78E_VULKAN_QUEUE_FAMILY_UNIVERSAL_QUEUE_FLAGS (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)
#define C78E_VULKAN_QUEUE_FAMILY_COMPUTE_QUEUE_FLAGS (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)
#define C78E_VULKAN_QUEUE_FAMILY_TRANSFER_QUEUE_FLAGS (VK_QUEUE_TRANSFER_BIT)
#define C78E_VULKAN_QUEUE_FAMILY_QUEUE_FLAGS_MASK (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)

    std::string vkQueueFlagsToString(VkQueueFlags flags) {
        std::string result = "[ ";
        if (flags & VK_QUEUE_GRAPHICS_BIT) result += "Graphics ";
        if (flags & VK_QUEUE_COMPUTE_BIT) result += "Compute ";
        if (flags & VK_QUEUE_TRANSFER_BIT) result += "Transfer ";
        if (flags & VK_QUEUE_SPARSE_BINDING_BIT) result += "SparseBinding ";
        if (flags & VK_QUEUE_PROTECTED_BIT) result += "Protected ";
        if (flags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) result += "VideoDecode ";
        // opt? if (flags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) result += "VideoEncode ";
        if (flags & VK_QUEUE_OPTICAL_FLOW_BIT_NV) result += "OpticalFlow ";
        return result + "]";
    }

    static const char* getDeviceTypeString(VkPhysicalDeviceType type) {
        switch (type) {
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated GPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "Discrete GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "Virtual GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
        case VK_PHYSICAL_DEVICE_TYPE_OTHER: return "Other";
        default: return "Unknown";
        }
    }

	// Physical Device
    
    VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice vkPhysicalDevice)
        : m_VkPhysicalDevice(vkPhysicalDevice) {
        C78E_CORE_INFO("VulkanPhysicalDevice::VulkanPhysicalDevice: Creating Vulkan device...");
		
        fetchPhysicalDeviceProperties();
        fetchPhysicalDeviceFeatures();
        fetchAvailableDeviceExtensions();

        fetchQueueFamilyProperties();
    }
    VulkanPhysicalDevice::~VulkanPhysicalDevice() {
		C78E_CORE_INFO("VulkanPhysicalDevice::~VulkanPhysicalDevice: Destroying Vulkan device...");
    }

    VkPhysicalDevice VulkanPhysicalDevice::getVkPhysicalDevice() const {
        return m_VkPhysicalDevice;
    }
    VkPhysicalDeviceProperties VulkanPhysicalDevice::getPhysicalDeviceProperties() const {
        return m_PhysicalDeviceProperties;
    }
    VkPhysicalDeviceFeatures VulkanPhysicalDevice::getPhysicalDeviceFeatures() const {
        return m_PhysicalDeviceFeatures;
    }
    const std::vector<VkQueueFamilyProperties>& VulkanPhysicalDevice::getQueueFamilyProperties() const {
        return m_QueueFamilyProperties;
    }
    
    bool VulkanPhysicalDevice::supportsExtensionByName(const char* extensionName) const {
        for (const auto& extension : m_AvailableExtensions) {
            if (std::strcmp(extension.extensionName, extensionName) == 0) {
                return true;
            }
        }
        return false;
    }
    bool VulkanPhysicalDevice::supportsExtensionsByName(const std::vector<const char*>& extensionNames) const {
        for (const auto& extensionName : extensionNames) {
            if (!supportsExtensionByName(extensionName)) {
                return false;
            }
        }
        return true;
    }


    bool VulkanPhysicalDevice::supportsUniversalPresent(const VkSurfaceKHR presentSurface) {
        fetchQueueFamilies(presentSurface);
        return m_UniversialFamily.queueFamilyIndex != -1 && m_UniversialFamily.supportsPresent;
    }

    bool VulkanPhysicalDevice::supportsDedicatedQueues() const {
        return m_ComputeFamily.queueFamilyIndex != -1 && m_TransferFamily.queueFamilyIndex != -1;
    }

    bool VulkanPhysicalDevice::supportsRayTracing() const {
        std::vector<const char*> rtextensions = { //TODO: also in Instance -> unify/centralize
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
        };
        if (!supportsExtensionsByName(rtextensions)) {
            return false;
        }

        VkPhysicalDeviceFeatures2 features2{};
        features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

        VkPhysicalDeviceAccelerationStructureFeaturesKHR accelFeatures{};
        accelFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        features2.pNext = &accelFeatures;

        VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeatures{};
        rtPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        accelFeatures.pNext = &rtPipelineFeatures;

        VkPhysicalDeviceBufferDeviceAddressFeatures bufferAddressFeatures{};
        bufferAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
        rtPipelineFeatures.pNext = &bufferAddressFeatures;

        vkGetPhysicalDeviceFeatures2(m_VkPhysicalDevice, &features2);

        return accelFeatures.accelerationStructure &&
            rtPipelineFeatures.rayTracingPipeline &&
            bufferAddressFeatures.bufferDeviceAddress;
    }

    void VulkanPhysicalDevice::fetchPhysicalDeviceProperties() {
		C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceProperties: Fetching physical device properties...");
        vkGetPhysicalDeviceProperties(m_VkPhysicalDevice, &m_PhysicalDeviceProperties);
        C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceProperties: Physical device: {0}", m_PhysicalDeviceProperties.deviceName);
		C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceProperties: Device type: {0}", getDeviceTypeString(m_PhysicalDeviceProperties.deviceType));
        C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceProperties: Device ID: {0}", m_PhysicalDeviceProperties.deviceID);
        C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceProperties: Driver version: {0}", m_PhysicalDeviceProperties.driverVersion);
        C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceProperties: API version: {0}", m_PhysicalDeviceProperties.apiVersion);
        C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceProperties: VendorID: {0}", m_PhysicalDeviceProperties.vendorID);
    }
    void VulkanPhysicalDevice::fetchPhysicalDeviceFeatures() {
		C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceFeatures: Fetching physical device features...");
		vkGetPhysicalDeviceFeatures(m_VkPhysicalDevice, &m_PhysicalDeviceFeatures);
		C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceFeatures: Geometry shader support: {0}", m_PhysicalDeviceFeatures.geometryShader ? "Yes" : "No");
		C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceFeatures: Tessellation shader support: {0}", m_PhysicalDeviceFeatures.tessellationShader ? "Yes" : "No");
		C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceFeatures: Anisotropic filtering support: {0}", m_PhysicalDeviceFeatures.samplerAnisotropy ? "Yes" : "No");
		C78E_CORE_TRACE("VulkanDevice::fetchPhysicalDeviceFeatures: Multi draw indirect support: {0}", m_PhysicalDeviceFeatures.multiDrawIndirect ? "Yes" : "No");
    }
    void VulkanPhysicalDevice::fetchAvailableDeviceExtensions() {
		C78E_CORE_TRACE("VulkanDevice::fetchDeviceExtensions: Fetching device extensions...");
		m_AvailableExtensions.clear();
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(m_VkPhysicalDevice, nullptr, &extensionCount, nullptr);
		C78E_CORE_VALIDATE(extensionCount, return, "VulkanDevice::fetchDeviceExtensions: Failed to find ANY device extensions!");
		C78E_CORE_TRACE("VulkanDevice::fetchDeviceExtensions: Device extensions found: {0}", extensionCount);
		m_AvailableExtensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(m_VkPhysicalDevice, nullptr, &extensionCount, m_AvailableExtensions.data());
		for(const auto& extension : m_AvailableExtensions) {
			C78E_CORE_TRACE("VulkanDevice::fetchDeviceExtensions: Device extension: {0}", extension.extensionName);
		}
    }

    void VulkanPhysicalDevice::fetchQueueFamilyProperties() {
        C78E_CORE_TRACE("queryQueueFamilyProperties: Fetching queue families...");
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_VkPhysicalDevice, &queueFamilyCount, nullptr);
        C78E_CORE_VALIDATE(queueFamilyCount, return, "queryQueueFamilyProperties: Failed to find ANY queue families!");
        C78E_CORE_TRACE("queryQueueFamilyProperties: Queue families found: {0}", queueFamilyCount);
        m_QueueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_VkPhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.data());
        for (const auto& queueFamily : m_QueueFamilyProperties) {
            C78E_CORE_TRACE("queryQueueFamilyProperties: Queue family: {0}", queueFamily.queueCount);
            C78E_CORE_TRACE("queryQueueFamilyProperties: Queue family flags: {0}", vkQueueFlagsToString(queueFamily.queueFlags));
        }
    }

    void VulkanPhysicalDevice::fetchQueueFamilies(const VkSurfaceKHR presentSurface) {
        if (m_UniversialFamily.queueFamilyIndex != -1) {
            C78E_CORE_TRACE("VulkanPhysicalDevice::fetchQueueFamilies: Universal queue family already fetched!");
			return; // Already fetched
        }
        m_UniversialFamily.queueFamilyIndex = getQueueFamilyIndexWith(
            C78E_VULKAN_QUEUE_FAMILY_UNIVERSAL_QUEUE_FLAGS,
            C78E_VULKAN_QUEUE_FAMILY_QUEUE_FLAGS_MASK,
            presentSurface);
        if (m_UniversialFamily.queueFamilyIndex != -1) {
            m_UniversialFamily.supportsPresent = (presentSurface != VK_NULL_HANDLE);
        }
        m_ComputeFamily.queueFamilyIndex = getQueueFamilyIndexWith(
            C78E_VULKAN_QUEUE_FAMILY_COMPUTE_QUEUE_FLAGS,
            C78E_VULKAN_QUEUE_FAMILY_QUEUE_FLAGS_MASK);
        m_TransferFamily.queueFamilyIndex = getQueueFamilyIndexWith(
            C78E_VULKAN_QUEUE_FAMILY_TRANSFER_QUEUE_FLAGS,
            C78E_VULKAN_QUEUE_FAMILY_QUEUE_FLAGS_MASK);
    }

    uint32_t VulkanPhysicalDevice::getQueueFamilyIndexWith(const VkQueueFlags queueFlags, const VkQueueFlags mask, const VkSurfaceKHR presentSurface) const {
        const bool requiresPresentSupport = presentSurface;

        for (auto it = m_QueueFamilyProperties.begin(); it != m_QueueFamilyProperties.end(); it++) {
            const uint32_t queueFamilyIndex = static_cast<uint32_t>(std::distance(m_QueueFamilyProperties.begin(), it));
            const VkQueueFamilyProperties& queueFamily = *it;
            if (!queueFamily.queueCount ||
                (queueFamily.queueFlags & mask) != (queueFlags & mask)) {
                continue;
            }
            if (requiresPresentSupport) {
                VkBool32 hasPresentSupport = VK_FALSE;
                vkGetPhysicalDeviceSurfaceSupportKHR(m_VkPhysicalDevice, queueFamilyIndex, presentSurface, &hasPresentSupport);
                if (!hasPresentSupport) {
                    continue;
                }
            }
            return queueFamilyIndex;
        }
        return -1;
    }
    
	// Logical Device

    VulkanDevice::VulkanDevice(VulkanPhysicalDevice& physicalDevice, VkSurfaceKHR surface, const std::vector<const char*>& extensions)
        : VulkanPhysicalDevice(physicalDevice), m_Surface(surface) {
        C78E_CORE_INFO("VulkanPhysicalDevice::VulkanDevice: Creating Vulkan device...");

        C78E_CORE_TRACE("VulkanDevice::VulkanDevice: Creating logical device...");
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        buildDeviceQueueCreateInfos(queueCreateInfos);

		//TODO: implement wanted features more dynamically
        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkResult createResult = vkCreateDevice(getVkPhysicalDevice(), &createInfo, nullptr, &m_VkDevice);
        C78E_CORE_ASSERT(createResult == VK_SUCCESS, "VulkanDevice::createLogicalDevice: Failed to create logical device!");

		C78E_CORE_ASSERT(createDeviceQueues(), "VulkanDevice::createLogicalDevice: Failed to create device queues!");

    }
    VulkanDevice::~VulkanDevice() {
        C78E_CORE_INFO("VulkanDevice::~VulkanDevice: Destroying Vulkan device...");

        if (m_VkDevice) {
            vkDestroyDevice(m_VkDevice, nullptr);
        }
    }

    VkDevice VulkanDevice::getVkDevice() {
        return m_VkDevice;
    }
    bool VulkanDevice::waitIdle() const {
        const VkResult result = vkDeviceWaitIdle(m_VkDevice);
        C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanDevice::waitIdle: failed!");
        return true;
    }
    /*
    bool VulkanDevice::submitCommandBuffer(Ref<VulkanCommandBuffer> vulkanCommandBuffer) {
        const VkQueueFlags flags = vulkanCommandBuffer->getRequiredVkQueueFlags();
        if (m_TransferQueue && ((flags & C78E_VULKAN_QUEUE_FAMILY_QUEUE_FLAGS_MASK) == C78E_VULKAN_QUEUE_FAMILY_TRANSFER_QUEUE_FLAGS)) {
            return m_TransferQueue->submit(vulkanCommandBuffer);
		} else if (m_ComputeQueues[0] && ((flags & C78E_VULKAN_QUEUE_FAMILY_QUEUE_FLAGS_MASK) == C78E_VULKAN_QUEUE_FAMILY_COMPUTE_QUEUE_FLAGS)) {
            if (!m_ComputeQueues[0]->isReady() && m_ComputeQueues[1]) {
                return m_ComputeQueues[1]->submit(vulkanCommandBuffer);
            }
            return m_ComputeQueues[0]->submit(vulkanCommandBuffer);
		} else if (m_UniversalQueue && ((flags & C78E_VULKAN_QUEUE_FAMILY_QUEUE_FLAGS_MASK) == C78E_VULKAN_QUEUE_FAMILY_UNIVERSAL_QUEUE_FLAGS)) {
			return m_UniversalQueue->submit(vulkanCommandBuffer);
		}
        return false;
    }
    */

    bool VulkanDevice::buildDeviceQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& deviceQueueCreateInfos) {
        deviceQueueCreateInfos.clear();
        fetchQueueFamilies(m_Surface);
        if (m_UniversialFamily.queueFamilyIndex == -1) {
            C78E_CORE_ERROR("VulkanPhysicalDevice::buildDeviceQueueCreateInfos: No universal queue family found!");
            return false;
        }

        // Universal queue
        VkDeviceQueueCreateInfo universalQueueInfo = {};
        universalQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        universalQueueInfo.queueFamilyIndex = m_UniversialFamily.queueFamilyIndex;
        universalQueueInfo.queueCount = 1;
        universalQueueInfo.pQueuePriorities = &m_UniversalQueuePriority;
        deviceQueueCreateInfos.push_back(universalQueueInfo);

        // Compute queue(s)
        if (m_ComputeFamily.queueFamilyIndex != -1) {
            uint32_t computeQueueCount = 1; // Default to 1, can be changed later
            if (m_QueueFamilyProperties[m_ComputeFamily.queueFamilyIndex].queueCount > 1) {
                computeQueueCount = 2;
            }
            VkDeviceQueueCreateInfo computeQueueInfo = {};
            computeQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            computeQueueInfo.queueFamilyIndex = m_ComputeFamily.queueFamilyIndex;
            computeQueueInfo.queueCount = computeQueueCount;
            computeQueueInfo.pQueuePriorities = m_ComputeQueuesPriorities;
            deviceQueueCreateInfos.push_back(computeQueueInfo);
        }

        // Transfer queue
        if (m_TransferFamily.queueFamilyIndex != -1) {
            VkDeviceQueueCreateInfo transferQueueInfo = {};
            transferQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            transferQueueInfo.queueFamilyIndex = m_TransferFamily.queueFamilyIndex;
            transferQueueInfo.queueCount = 1;
            transferQueueInfo.pQueuePriorities = &m_TransferQueuePriority;
            deviceQueueCreateInfos.push_back(transferQueueInfo);
        }
        return true;
    }

    bool VulkanDevice::createDeviceQueues() {
        if (m_UniversialFamily.queueFamilyIndex != -1) {
            vkGetDeviceQueue(m_VkDevice, m_UniversialFamily.queueFamilyIndex, 0, &m_UniversalQueue);
            C78E_CORE_VALIDATE(m_UniversalQueue, return false, "VulkanDevice::createDeviceQueues: Failed to create Universal Device Queue!");
        }
        if (m_ComputeFamily.queueFamilyIndex != -1) {
            vkGetDeviceQueue(m_VkDevice, m_ComputeFamily.queueFamilyIndex, 0, &m_ComputeQueues[0]);
            C78E_CORE_VALIDATE(m_UniversalQueue, return false, "VulkanDevice::createDeviceQueues: Failed to create Compute Device Queue(0)!");
            if (m_QueueFamilyProperties[m_ComputeFamily.queueFamilyIndex].queueCount > 1) {
                vkGetDeviceQueue(m_VkDevice, m_ComputeFamily.queueFamilyIndex, 1, &m_ComputeQueues[1]);
                C78E_CORE_VALIDATE(m_UniversalQueue, return false, "VulkanDevice::createDeviceQueues: Failed to create Compute Device Queue(1)!");
            }
        }
        if (m_TransferFamily.queueFamilyIndex != -1) {
            vkGetDeviceQueue(m_VkDevice, m_TransferFamily.queueFamilyIndex, 0, &m_TransferQueue);
            C78E_CORE_VALIDATE(m_UniversalQueue, return false, "VulkanDevice::createDeviceQueues: Failed to create Transfer Device Queue!");
        }
        C78E_CORE_VALIDATE(m_UniversalQueue || m_ComputeQueues[0] || m_ComputeQueues[1] || m_TransferQueue,
            return false, "VulkanDevice::createDeviceQueues: No Queue was created!");
        return true;
    }

	// static Vulkan Device Manager

    VkInstance VulkanDeviceManager::s_VkInstance = VK_NULL_HANDLE;
    std::vector<VulkanPhysicalDevice> VulkanDeviceManager::s_PhysicalDevices;

    void VulkanDeviceManager::init(VkInstance vkInstance) {
        C78E_CORE_TRACE("VulkanDeviceManager::init: Initializing Vulkan Device Manager...");
        s_VkInstance = vkInstance;
        fetchPhysicalDevices();
		C78E_CORE_TRACE("VulkanDeviceManager::init: Initialized with {0} physical devices.", s_PhysicalDevices.size());
    }

    Ref<VulkanDevice> VulkanDeviceManager::pickDevice(const VkSurfaceKHR presentSurface, const std::vector<const char*>& extensions) {
		VulkanPhysicalDevice* compatibleDevice = nullptr;
        for (auto& device : s_PhysicalDevices) {
			const bool hasDeviceSupport =
                device.supportsExtensionsByName(extensions) &&
				device.supportsUniversalPresent(presentSurface);
            if (hasDeviceSupport) {
				compatibleDevice = &device;
            }
            const bool hasFullSupport =
                device.supportsDedicatedQueues();
            if (hasFullSupport) {
                return createRef<VulkanDevice>(device, presentSurface, extensions);
            }
        }
        if (compatibleDevice) {
            return createRef<VulkanDevice>(*compatibleDevice, presentSurface, extensions);
        }
		C78E_CORE_ASSERT(false, "VulkanDeviceManager::pickDevice: No suitable Vulkan device found!");
        return nullptr;
    }

    inline const std::vector<VulkanPhysicalDevice>& VulkanDeviceManager::getPhysicalDevices() {
        return s_PhysicalDevices;
    }

    inline const VulkanPhysicalDevice& VulkanDeviceManager::getPhysicalDevice(uint32_t index) {
		C78E_CORE_ASSERT(index < s_PhysicalDevices.size(), "VulkanDeviceManager::getPhysicalDevice: Index out of bounds!");
        return s_PhysicalDevices[index];
    }

    void VulkanDeviceManager::fetchPhysicalDevices() {
        C78E_CORE_TRACE("VulkanDeviceManager::fetchDevices: Fetching devices...");
        s_PhysicalDevices.clear();
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(s_VkInstance, &deviceCount, nullptr);
        C78E_CORE_ASSERT(deviceCount, "VulkanDeviceManager::fetchDevices: Failed to find GPUs with Vulkan support!");
        C78E_CORE_TRACE("VulkanDeviceManager::fetchDevices: PhysicalDevices found: {0}", deviceCount);
        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(s_VkInstance, &deviceCount, physicalDevices.data());
        for (const auto& physicalDevice : physicalDevices) {
            s_PhysicalDevices.emplace_back(physicalDevice);
        }
    }

}
