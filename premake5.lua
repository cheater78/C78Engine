workspace "C78Engine"
	architecture "x64"
	startproject "C78TestApp"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

vulkanSDKdir = "%{prj.name}/vendor/VulkanSDK/Include"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "C78Engine/vendor/glfw/include"
IncludeDir["GLAD"] = "C78Engine/vendor/glad/"

include "C78Engine/vendor/glfw"
include "C78Engine/vendor/glad"

project "C78Engine"
	location "C78Engine"
	kind "SharedLib"
	language "C++"

	targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
	objdir  ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "C78ePCH.h"
	pchsource "C78Engine/src/C78ePCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src/",
		"%{prj.name}/vendor/entt/single_include",
		"%{prj.name}/vendor/glm/glm",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/stb",
		"%{prj.name}/vendor/tinyobjloader",
		"%{vulkanSDKdir}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}"
	}

	links{
		"GLFW",
		"GLAD",
		"opengl32.lib"
	}


	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines{
			"C78_PLATFORM_WINDOWS",
			"C78_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/C78TestApp")
		}

	filter "configurations:Debug"
		defines{ "C78_DEBUG", "C78_ENABLE_ASSERTS" }
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "C78_RELEASE"
		buildoptions "/MD"
		optimize "speed"

	filter "configurations:Dist"
		defines "C78_DIST"
		buildoptions "/MD"
		optimize "speed"




project "C78TestApp"
	location "C78TestApp"
	kind "ConsoleApp"
	language "C++"

	targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
	objdir  ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/",
		"C78Engine/vendor/spdlog/include",
		"C78Engine/src"
	}

	links
	{
		"C78Engine"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines{
			"C78_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "C78_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "C78_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "C78_DIST"
		buildoptions "/MD"
		optimize "On"


