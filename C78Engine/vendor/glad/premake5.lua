project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	runtime "Release"
	warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"glad.h",
		"khrplatform.h",
		"glad.c"
	}

	includedirs
	{
		"."
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "system:macosx"
		pic "On"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
        symbols "off"
