#pragma once
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
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

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

//C78E::CORE
#include <C78E/Core/Core.h>
#include <C78E/Core/Assert.h>
#include <C78E/Core/Log.h>
#include <C78E/Core/UUID.h>
#include <C78E/Core/Buffer.h>
#include <C78E/Core/Window.h>
#include "C78E/Core/Application.h"
#include <C78E/Core/FileSystem/FileSystem.h>
#include <C78E/Core/FileSystem/Extensions.h>
//C78E::UTILS
#include <C78E/Utils/StdUtils.h>


//PLATFORM
#ifdef C78_PLATFORM_WINDOWS
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <glad.h>
#elif defined C78_PLATFORM_LINUX
#include <GLFW/glfw3.h>
#include <glad.h>
#endif
