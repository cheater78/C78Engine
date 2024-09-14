-- C78E Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb"] = 		"%{wks.location}/C78Engine/vendor/stb"
IncludeDir["GLFW"] = 		"%{wks.location}/C78Engine/vendor/GLFW/include"
IncludeDir["Glad"] = 		"%{wks.location}/C78Engine/vendor/Glad"
IncludeDir["ImGui"] = 		"%{wks.location}/C78Engine/vendor/ImGui"
IncludeDir["glm"] = 		"%{wks.location}/C78Engine/vendor/glm"
IncludeDir["entt"] = 		"%{wks.location}/C78Engine/vendor/entt/single_include"
IncludeDir["spdlog"] = 		"%{wks.location}/C78Engine/vendor/spdlog/include"
IncludeDir["tinobj"] = 		"%{wks.location}/C78Engine/vendor/tinyobjloader"
IncludeDir["yaml_cpp"] = 	"%{wks.location}/C78Engine/vendor/yaml-cpp/include"
IncludeDir["msdfgen"] =	"%{wks.location}/C78Engine/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["freetype"] =	"%{wks.location}/C78Engine/vendor/msdf-atlas-gen/msdfgen/freetype/include"
IncludeDir["msdf_atlas_gen"] =	"%{wks.location}/C78Engine/vendor/msdf-atlas-gen/msdf-atlas-gen"

IncludeDir["VulkanSDK"] = 	"%{VULKAN_SDK}/Include"
IncludeDir["shaderc"] = 	"%{VULKAN_SDK}/Include/shaderc"
IncludeDir["SPIRV_Cross"] = "%{VULKAN_SDK}/Include/spirv_cross"



LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"



Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"
