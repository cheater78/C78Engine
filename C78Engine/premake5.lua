project "C78Engine"
	location "./"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir  ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir  ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.c",
		"src/**.cpp"
	}

	includedirs
	{
		"src/",
		"../C78Engine/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.tinobj}",
		"%{IncludeDir.yaml_cpp}"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp"
	}
	
	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}
	
	filter "action:vs*"
	    pchheader "C78ePCH.h"
		pchsource "src/C78ePCH.cpp"


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