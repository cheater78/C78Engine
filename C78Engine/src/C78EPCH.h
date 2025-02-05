#pragma once

namespace C78E { }

//PLATFORM
#ifdef C78E_PLATFORM_WINDOWS
#include <Windows.h>
#define C78E_PLATFORM_GLFW 1
#define GLFW_EXPOSE_NATIVE_WIN32

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#elif defined C78E_PLATFORM_LINUX
#define C78E_PLATFORM_GLFW 1
#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#endif

//LIB::STD
#include <C78E/Utils/StdUtils.h>

//LIB::SPDLOG
#include <C78E/Utils/LogUtils.h>

//LIB::GLM
#include <C78E/Utils/MathUtils.h>

//LIB::ENTT
#include <entt/entt.hpp>

//LIB::IMGUI
#include <C78E/Utils/ImGui/ImGuiUtils.h>

//LIB::YAML
#include <C78E/Utils/Yaml/YamlUtils.h>

//C78E::CORE
#include <C78E/Core/Base.h>
#include <C78E/Core/Core.h>
#include <C78E/Core/Log/Log.h>
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Core/UUID.h>
#include <C78E/Core/Timer.h>
#include <C78E/Core/Timestep.h>
#include <C78E/Core/Buffer.h>
#include <C78E/Core/Application/Window.h>
#include <C78E/Core/Application/Application.h>
#include <C78E/Core/FileSystem/FileSystem.h>
//C78E::UTILS
#include <C78E/Utils/PlatformUtils.h>



