-- C78E Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Include Directories
IncludeDir = {}
IncludeDir["stb"] = 		"%{wks.location}/C78Engine/vendor/stb"
IncludeDir["GLFW"] = 		"%{wks.location}/C78Engine/vendor/GLFW/include"
IncludeDir["Glad"] = 		"%{wks.location}/C78Engine/vendor/Glad"
IncludeDir["ImGui"] = 		"%{wks.location}/C78Engine/vendor/imgui"
IncludeDir["glm"] = 		"%{wks.location}/C78Engine/vendor/glm"
IncludeDir["entt"] = 		"%{wks.location}/C78Engine/vendor/entt/single_include"
IncludeDir["spdlog"] = 		"%{wks.location}/C78Engine/vendor/spdlog/include"
IncludeDir["tinobj"] = 		"%{wks.location}/C78Engine/vendor/tinyobjloader"
IncludeDir["yaml_cpp"] = 	"%{wks.location}/C78Engine/vendor/yaml-cpp/include"
IncludeDir["msdfgen"] =	"%{wks.location}/C78Engine/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["freetype"] =	"%{wks.location}/C78Engine/vendor/msdf-atlas-gen/msdfgen/freetype/include"
IncludeDir["msdf_atlas_gen"] =	"%{wks.location}/C78Engine/vendor/msdf-atlas-gen/msdf-atlas-gen"
IncludeDir["tinygltf"] =	"%{wks.location}/C78Engine/vendor/tinygltf"

if os.host() == "windows" then
	IncludeDir["VulkanSDK"] = 	"%{VULKAN_SDK}/Include"
    IncludeDir["shaderc"] = 	"%{VULKAN_SDK}/Include/shaderc"
    IncludeDir["SPIRV_Cross"] = "%{VULKAN_SDK}/Include/spirv_cross"
elseif os.host() == "linux" then
	IncludeDir["VulkanSDK"] = 	"%{VULKAN_SDK}/include"
    IncludeDir["shaderc"] = 	"%{VULKAN_SDK}/include/shaderc"
    IncludeDir["SPIRV_Cross"] = "%{VULKAN_SDK}/include/spirv_cross"
end

-- Static Libs
Library = {}
if os.host() == "windows" then
    Library["Vulkan"] = "%{VULKAN_SDK}/Lib/vulkan-1.lib"
    --TODO: VKLayerUtils does not exist any more!, check if needed -> update

    Library["ShaderC_Debug"] = "%{VULKAN_SDK}/Lib/shaderc_sharedd.lib"
    Library["SPIRV_Cross_Debug"] = "%{VULKAN_SDK}/Lib/spirv-cross-cored.lib"
    Library["SPIRV_Cross_GLSL_Debug"] = "%{VULKAN_SDK}/Lib/spirv-cross-glsld.lib"
    Library["SPIRV_Tools_Debug"] = "%{VULKAN_SDK}/Lib/SPIRV-Toolsd.lib"

    Library["ShaderC_Release"] = "%{VULKAN_SDK}/Lib/shaderc_shared.lib"
    Library["SPIRV_Cross_Release"] = "%{VULKAN_SDK}/Lib/spirv-cross-core.lib"
    Library["SPIRV_Cross_GLSL_Release"] = "%{VULKAN_SDK}/Lib/spirv-cross-glsl.lib"

    -- Windows
    Library["WinSock"] = "Ws2_32.lib"
    Library["WinMM"] = "Winmm.lib"
    Library["WinVersion"] = "Version.lib"
    Library["BCrypt"] = "Bcrypt.lib"

elseif os.host() == "linux" then
    Library["Vulkan"] = "%{VULKAN_SDK}/lib/vulkan"
    --TODO: VKLayerUtils does not exist any more!, check if needed -> update

    Library["ShaderC_Debug"] = "%{VULKAN_SDK}/lib/shaderc_shared"
    Library["SPIRV_Cross_Debug"] = "%{VULKAN_SDK}/lib/spirv-cross-core"
    Library["SPIRV_Cross_GLSL_Debug"] = "%{VULKAN_SDK}/lib/spirv-cross-glsl"
    Library["SPIRV_Tools_Debug"] = "%{VULKAN_SDK}/lib/SPIRV-Tools"

    Library["ShaderC_Release"] = "%{VULKAN_SDK}/lib/shaderc_shared"
    Library["SPIRV_Cross_Release"] = "%{VULKAN_SDK}/lib/spirv-cross-core"
    Library["SPIRV_Cross_GLSL_Release"] = "%{VULKAN_SDK}/lib/spirv-cross-glsl"
end