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
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.freetype}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.tinygltf}",
	}
	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"msdf-atlas-gen",
		"msdfgen",
		"freetype",
		"%{Library.Vulkan}"
	}
	
	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}
	
	filter "action:vs*"
	    pchheader "C78EPCH.h"
		pchsource "src/C78EPCH.cpp"


	filter "system:Windows"
		systemversion "latest"
		buildoptions {
			"/Zc:preprocessor"
		}

		defines{
			"C78E_PLATFORM_WINDOWS",
			"C78E_PLATFORM_GLFW",
			"GLFW_INCLUDE_NONE"
		}
		
		links{
			"opengl32.lib"
		}
		
	filter "system:Linux"
		systemversion "latest"

		defines{
			"C78E_PLATFORM_LINUX",
			"C78E_PLATFORM_GLFW",
			"GLFW_INCLUDE_NONE"
		}

		links{
			"GL",
			"X11",
			"Xcursor",
			"Xxf86vm",
			"Xinerama"
		}

	filter "configurations:Debug"
		defines{ "C78E_DEBUG", "C78E_ENABLE_ASSERTS" }
		runtime "Debug"
		symbols "on"
		
		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}",
			"%{Library.SPIRV_Tools_Debug}"
		}

	filter "configurations:Release"
		defines "C78E_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "C78E_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}