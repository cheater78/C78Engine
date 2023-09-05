#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <limits>
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#ifdef C78_PLATFORM_WINDOWS
	#include <Windows.h>

#endif // C78_PLATFORM_WINDOWS

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <entt/entt.hpp>

#include "C78e/Core/Core.h"
#include "C78e/Core/Assert.h"
#include "C78e/Core/Log.h"