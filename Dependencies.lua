
-- C78 Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb"] = 		"%{wks.location}/C78Engine/vendor/stb"
--IncludeDir["yaml_cpp"] = 	"%{wks.location}/C78Engine/vendor/yaml-cpp/include"
--IncludeDir["Box2D"] = 	"%{wks.location}/C78Engine/vendor/Box2D/include"
--IncludeDir["filewatch"] = "%{wks.location}/C78Engine/vendor/filewatch"
IncludeDir["GLFW"] = 		"%{wks.location}/C78Engine/vendor/GLFW/include"
IncludeDir["Glad"] = 		"%{wks.location}/C78Engine/vendor/Glad"
IncludeDir["ImGui"] = 		"%{wks.location}/C78Engine/vendor/ImGui"
--IncludeDir["ImGuizmo"] = 	"%{wks.location}/C78Engine/vendor/ImGuizmo"
IncludeDir["glm"] = 		"%{wks.location}/C78Engine/vendor/glm"
IncludeDir["entt"] = 		"%{wks.location}/C78Engine/vendor/entt/single_include"
--IncludeDir["mono"] = 		"%{wks.location}/C78Engine/vendor/mono/include"
IncludeDir["shaderc"] = 	"%{VULKAN_SDK}/Include/shaderc"
IncludeDir["SPIRV_Cross"] = "%{VULKAN_SDK}/Include/spirv_cross"
IncludeDir["VulkanSDK"] = 	"%{VULKAN_SDK}/Include"
IncludeDir["msdfgen"] = 	"%{wks.location}/C78Engine/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/C78Engine/vendor/msdf-atlas-gen/msdf-atlas-gen"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
--LibraryDir["mono"] = "%{wks.location}/C78Engine/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
--Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

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
