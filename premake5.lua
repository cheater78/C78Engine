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
IncludeDir["IMGUI"] = "C78Engine/vendor/imgui/"

include "C78Engine/vendor/glfw"
include "C78Engine/vendor/glad"
include "C78Engine/vendor/imgui"

filter "system:windows"
	argsym = "/"
filter "system:linux"
	argsym = "-"



project "C78Engine"
	location "C78Engine"
	kind "SharedLib"
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
		"%{prj.name}/src/",
		"%{prj.name}/vendor/entt/single_include",
		"%{prj.name}/vendor/glm/glm",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/stb",
		"%{prj.name}/vendor/tinyobjloader",
		"%{vulkanSDKdir}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.IMGUI}"
	}
	
	filter "action:vs*"
	    pchheader "C78ePCH.h"
		pchsource "C78Engine/src/C78ePCH.cpp"


	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines{
			"C78_PLATFORM_WINDOWS",
			"C78_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		links{
			"GLFW",
			"GLAD",
			"opengl32.lib",
			"ImGui"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/")
		}
		
	filter "system:linux"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines{
			"C78_PLATFORM_LINUX",
			"C78_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		links{
			"GLFW",
			"GLAD",
			"GL",
			"ImGui"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/C78TestApp")
		}

	filter "configurations:Debug"
		defines{ "C78_DEBUG", "C78_ENABLE_ASSERTS" }
		buildoptions "%{argsym}MD"
		symbols "On"

	filter "configurations:Release"
		defines "C78_RELEASE"
		buildoptions "%{argsym}MD"
		optimize "speed"

	filter "configurations:Dist"
		defines "C78_DIST"
		buildoptions "%{argsym}MD"
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
		"C78Engine/vendor/",
		"C78Engine/src"
	}

	links
	{
		"C78Engine",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		

		defines{
			"C78_PLATFORM_WINDOWS"
		}
		
	filter "system:linux"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines{
			"C78_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines "C78_DEBUG"
		buildoptions "%{argsym}MD"
		symbols "On"

	filter "configurations:Release"
		defines "C78_RELEASE"
		buildoptions "%{argsym}MD"
		optimize "On"

	filter "configurations:Dist"
		defines "C78_DIST"
		buildoptions "%{argsym}MD"
		optimize "On"


