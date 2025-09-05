#include "C78EPCH.h"
#include "VulkanGraphicsInstance.h"

namespace C78E {

    static const std::array<const char*, 1> c_RequiredDebugExtensions = {
        // Add more extensions here if needed
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    };
    static const std::array<const char*, 1> c_RequiredExtensions = {
        // Add more extensions here if needed
        VK_KHR_SURFACE_EXTENSION_NAME,
    };
    static const std::array<const char*, 1> c_RequiredDeviceExtensions = {
        // Add more extensions here if needed
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };
    static const std::array<const char*, 3> c_RTExtensions = {
        // Add more extensions here if needed
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    };
    static const std::array<const char*, 1> c_RequiredDebugLayers = {
        // Add more layers here if needed
        "VK_LAYER_KHRONOS_validation",
    };
    static const std::array<const char*, 0> c_RequiredLayers = {
        // Add more layers here if needed
    };

    static std::vector<const char*> getRequiredExtensionNames() {
        std::vector<const char*> extensions;

#ifdef C78E_DEBUG
        for (const char* extension : c_RequiredDebugExtensions) {
            extensions.emplace_back(extension);
        }
#endif // C78E_DEBUG

        for (const char* extension : c_RequiredExtensions) {
            extensions.emplace_back(extension);
        }

        // GLFW extensions required for Vulkan
#ifdef C78E_PLATFORM_GLFW
        int vukanAvailable = glfwVulkanSupported();
        C78E_CORE_ASSERT(vukanAvailable, "getRequiredExtensionNames: Vulkan support not found by glfw!");

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        C78E_CORE_ASSERT(glfwExtensions, "VulkanGraphicsContext::getRequiredExtensionNames: Failed to get required instance extensions!");

        // extensions may contain extentions multiple times, e.g. VK_KHR_SURFACE_EXTENSION_NAME is required by glfw and alr in there
        extensions.reserve(extensions.size() + glfwExtensionCount);
        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            extensions.emplace_back(glfwExtensions[i]);
        }
#endif

        // Add more extensions here if needed by a Platform

        return extensions;
    }
    static std::vector<const char*> getRequiredDeviceExtensionNames() {
        std::vector<const char*> extensions;
        for (const char* extension : c_RequiredDeviceExtensions) {
            extensions.emplace_back(extension);
        }
        return extensions;
    }
    static std::vector<const char*> getRequiredLayerNames() {
        std::vector<const char*> layers;

#ifdef C78E_DEBUG
        for (const char* layer : c_RequiredDebugLayers) {
            layers.emplace_back(layer);
        }
#endif // C78E_DEBUG

        for (const char* layer : c_RequiredLayers) {
            layers.emplace_back(layer);
        }

        // Add more layers here if needed by a Platform

        return layers;
    }

    // Validation Layer Callback -> Logging
    static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallBack(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            C78E_CORE_ERROR("Vulkan Validation Layer: {0}", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            C78E_CORE_WARN("Vulkan Validation Layer: {0}", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            C78E_CORE_INFO("Vulkan Validation Layer: {0}", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
            C78E_CORE_TRACE("Vulkan Validation Layer: {0}", pCallbackData->pMessage);
        }
        return VK_FALSE;
    }
    static VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{}; // instantiate defaults
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = vulkanDebugCallBack;
        createInfo.pUserData = nullptr;  // Optional
        return createInfo;
    }

    void VulkanGraphicsInstance::init() {
		C78E_CORE_TRACE("Initializing Vulkan Graphics Instance...");
		C78E_CORE_ASSERT(!m_VkInstance, "VulkanGraphicsInstance::init: Vulkan instance already initialized!");

        fetchAvailableExtensions();
        fetchAvailableLayers();

        std::vector<const char*> requiredExtensions = getRequiredExtensionNames();
        std::vector<const char*> requiredLayers = getRequiredLayerNames();

        C78E_CORE_ASSERT(checkExtensionSupport(requiredExtensions), "VulkanGraphicsContext::init: Required extensions not supported!");
        C78E_CORE_ASSERT(checkLayerSupport(requiredLayers), "VulkanGraphicsContext::init: Required layers not supported!");

        // Application info setup
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = Application::get().getApplicationName();
        appInfo.applicationVersion = Application::get().getApplicationVersion();
        appInfo.pEngineName = C78E_TITLE;
        appInfo.engineVersion = C78E_VERSION_NUMBER;
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();

#ifdef C78E_DEBUG // Prepare Debug Messenger for Instance creation
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = debugUtilsMessengerCreateInfo();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
        createInfo.pNext = nullptr;
#endif // C78E_DEBUG

        // Create Vulkan instance
        VkResult instanceCreateResult = vkCreateInstance(&createInfo, nullptr, &m_VkInstance);
        C78E_CORE_VALIDATE(instanceCreateResult == VK_SUCCESS, return, "VulkanGraphicsContext::init: Failed to create instance!");

		VulkanDeviceManager::init(m_VkInstance); // Initialize Vulkan Device Manager with the created instance

#ifdef C78E_DEBUG
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VkInstance, "vkCreateDebugUtilsMessengerEXT");
        C78E_CORE_SOFT_VALIDATE(func, "VulkanGraphicsContext::VulkanInstance: Failed to load vkCreateDebugUtilsMessengerEXT function!");

        VkResult debugMessengerCreateResult = func(m_VkInstance, &debugCreateInfo, nullptr, &m_DebugMessenger);
        C78E_CORE_SOFT_VALIDATE(debugMessengerCreateResult == VK_SUCCESS, "VulkanGraphicsContext::init: Failed to set up debug messenger!");

#endif // C78E_DEBUG
    }

    void VulkanGraphicsInstance::shutdown() {
        C78E_CORE_TRACE("Shutting down Vulkan Graphics Instance...");
        if (m_DebugMessenger) { // DebugMessenger is enabled -> clean it up
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VkInstance, "vkDestroyDebugUtilsMessengerEXT");
            C78E_CORE_ASSERT(func, "VulkanInstance::~VulkanInstance: Failed to load vkDestroyDebugUtilsMessengerEXT function! - DebugMessenger cannot be cleaned up!");
            func(m_VkInstance, m_DebugMessenger, nullptr);
        }
        if (m_VkInstance) {
            vkDestroyInstance(m_VkInstance, nullptr);
			m_VkInstance = VK_NULL_HANDLE; // needed?
        }
    }


    void VulkanGraphicsInstance::fetchAvailableExtensions() {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        C78E_CORE_VALIDATE(extensionCount, return, "VulkanInstance::fetchAvailableExtensions: Failed to find ANY instance extensions!");
        C78E_CORE_TRACE("VulkanInstance::fetchAvailableExtensions: Instance extensions found: {0}", extensionCount);
        m_AvailableExtensions.resize(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_AvailableExtensions.data());
        for (const auto& extension : m_AvailableExtensions) {
            C78E_CORE_TRACE("VulkanInstance::fetchAvailableExtensions: Instance extension: {0}", extension.extensionName);
        }
    }
    void VulkanGraphicsInstance::fetchAvailableLayers() {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        C78E_CORE_VALIDATE(layerCount, return, "VulkanInstance::fetchAvailableLayers: Failed to find ANY instance layers!");
        C78E_CORE_TRACE("VulkanInstance::fetchAvailableLayers: Instance layers found: {0}", layerCount);
        m_AvailableLayers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, m_AvailableLayers.data());
        for (const auto& layer : m_AvailableLayers) {
            C78E_CORE_TRACE("VulkanInstance::fetchAvailableLayers: Instance layer: {0}", layer.layerName);
        }
    }
    bool VulkanGraphicsInstance::checkExtensionSupport(const char* extensionName) {
        if (m_AvailableExtensions.empty()) {
			fetchAvailableExtensions();
        }
        for (const auto& extension : m_AvailableExtensions) {
            if (std::strcmp(extension.extensionName, extensionName) == 0) {
                return true;
            }
        }
        return false;
    }
    bool VulkanGraphicsInstance::checkExtensionSupport(const std::vector<const char*>& extensionNames) {
        for (const auto& extensionName : extensionNames) {
            if (!checkExtensionSupport(extensionName)) {
                return false;
            }
        }
        return true;
    }
    bool VulkanGraphicsInstance::checkLayerSupport(const char* layerName) {
        if (m_AvailableLayers.empty()) {
			fetchAvailableLayers();
        }
        for (const auto& layerProperties : m_AvailableLayers) {
            if (std::strcmp(layerName, layerProperties.layerName) == 0) {
                return true;
            }
        }
        return false;
    }
    bool VulkanGraphicsInstance::checkLayerSupport(const std::vector<const char*>& layerNames) {
        for (const auto& layerName : layerNames) {
            if (!checkLayerSupport(layerName)) {
                return false;
            }
        }
        return true;
    }
    
    VulkanGraphicsInstance::VulkanGraphicsInstance()
        : GraphicsInstance() {
		init();
    }

    VulkanGraphicsInstance::~VulkanGraphicsInstance() {
		shutdown();
    }

    Ref<VulkanDevice> VulkanGraphicsInstance::pickDevice(VkSurfaceKHR surface) {
        return VulkanDeviceManager::pickDevice(surface, getRequiredDeviceExtensionNames());
    }

}
