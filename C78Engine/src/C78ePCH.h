#pragma once

//PLATFORM
#ifdef C78_PLATFORM_WINDOWS
#include <Windows.h>
#define C78_PLATFORM_GLFW 1
#define GLFW_EXPOSE_NATIVE_WIN32

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#elif defined C78_PLATFORM_LINUX
#define C78_PLATFORM_GLFW 1
#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#endif

//LIB::STD
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <filesystem>
#include <fstream>

#include <limits>
#include <cstdint>
#include <string>
#include <cstring>
#include <sstream>
#include <regex>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <cassert>

#include <semaphore>
#include <thread>

//LIB::GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//LIB::ENTT
#include <entt/entt.hpp>

namespace C78E { }
//C78E::CORE
#include <C78E/Core/Base.h>
#include <C78E/Core/Core.h>
#include <C78E/Core/Assert.h>
#include <C78E/Core/Log.h>
#include <C78E/Core/UUID.h>
#include <C78E/Core/Timer.h>
#include <C78E/Core/Timestep.h>
#include <C78E/Core/Buffer.h>
#include <C78E/Core/Window.h>
#include "C78E/Core/Application.h"
#include <C78E/Core/FileSystem/FileSystem.h>
#include <C78E/Core/FileSystem/Extensions.h>
//C78E::UTILS
#include <C78E/Utils/StdUtils.h>
#include <C78E/Utils/PlatformUtils.h>



