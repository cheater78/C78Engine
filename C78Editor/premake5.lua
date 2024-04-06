project "C78Editor"
	location "./"
	kind "ConsoleApp"
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
		"vendor/",
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

