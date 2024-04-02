#pragma once

// std lib
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

// Platform dependent
#ifdef C78_PLATFORM_WINDOWS
	#include <Windows.h>

#endif


// libs
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

//C78E core
#include <C78E/Core/Core.h>
#include <C78E/Core/Assert.h>
#include <C78E/Core/Log.h>
#include <C78E/Core/UUID.h>

//C78E utils
#include <C78E/Utils/StdUtils.h>