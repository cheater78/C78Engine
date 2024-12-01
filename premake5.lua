include "Dependencies.lua"

workspace "C78Engine"
	architecture "x64"
	startproject "C78Editor"

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
include "C78Engine/vendor/yaml-cpp"
include "C78Engine/vendor/msdf-atlas-gen"
include "C78Engine/vendor/msdf-atlas-gen/msdfgen"

include "C78Engine"
include "C78Editor"
include "C78Project"

filter "system:windows"
	argsym = "/"
filter "system:linux"
	argsym = "-"