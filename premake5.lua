include "Dependencies.lua"

workspace "C78Engine"
	architecture "x64"
	startproject "C78TestApp"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "C78Engine/vendor/GLFW"
include "C78Engine/vendor/Glad"
include "C78Engine/vendor/imgui"

filter "system:windows"
	argsym = "/"
filter "system:linux"
	argsym = "-"



project "C78Engine"
	location "C78Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.VulkanSDK}",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/tinyobjloader"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
	}
	
	filter "action:vs*"
	    pchheader "C78ePCH.h"
		pchsource "C78Engine/src/C78ePCH.cpp"


	filter "system:windows"
		systemversion "latest"

		defines{
			"C78_PLATFORM_WINDOWS",
			"C78_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
		links{
			"opengl32.lib"
		}
		
	filter "system:linux"
		systemversion "latest"

		defines{
			"C78_PLATFORM_LINUX",
			"C78_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		links{
			"GL"
		}

	filter "configurations:Debug"
		defines{ "C78_DEBUG", "C78_ENABLE_ASSERTS" }
		runtime "Debug"
		symbols "on"
		
		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "C78_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "C78_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

project "C78App"
	location "C78App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		"C78Engine/vendor/glm",
		"%{IncludeDir.entt}",
		"%{IncludeDir.stb}",
		"C78Engine/vendor/",
		"%{IncludeDir.VulkanSDK}",
		"C78Engine/src"
	}

	links
	{
		"C78Engine"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"C78_PLATFORM_WINDOWS"
		}
		
	filter "system:linux"
		systemversion "latest"

		defines{
			"C78_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines{ "C78_DEBUG", "C78_ENABLE_ASSERTS" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "C78_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "C78_DIST"
		runtime "Release"
		optimize "on"

