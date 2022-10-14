workspace "C78Engine"
	architecture "x64"
	
	configurations{
		"Debug",
		"Release",
		"Dist"
	}
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "C78Engine"
	location "C78Engine"
	kind "SharedLib"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h"
	}
	
	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}
	
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "10.0.19041.0"
		
		defines
		{
			"C78E_PLATFORM_WINDOWS",
			"C78E_BUILD_DLL"
		}
		
		postbuildcommands
		{
			"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"
		}
		
	filter "configurations:Debug"
		defines "C78E_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "C78E_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "C78E_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h"
	}
	
	includedirs
	{
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
		systemversion "10.0.19041.0"
		
		defines
		{
			"C78E_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "C78E_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "C78E_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "C78E_DIST"
		optimize "On"