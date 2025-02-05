#pragma once

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
// Utils - Libs
#include <C78E/Utils/StdUtils.h>
#include <C78E/Utils/PlatformUtils.h>

#include <C78E/Utils/LogUtils.h>
#include <C78E/Utils/MathUtils.h>
#include <C78E/Utils/Yaml/YamlUtils.h>
#include <C78E/Utils/ImGui/ImGuiUtils.h>
#include <entt/entt.hpp>
#include <C78E/Utils/MSDFAtlasGen/MSDFUtils.h>

// Core
#include <C78E/Core/Base.h>
#include <C78E/Core/Core.h>
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Core/Timestep.h>
#include <C78E/Core/Timer.h>
#include <C78E/Core/UUID.h>
#include <C78E/Core/Buffer.h>
#include <C78E/Core/Application/Application.h>
#include <C78E/Core/FileSystem/FileSystem.h>

// User Input
#include <C78E/Core/Input/Input.h>

// Project
#include <C78E/Project/Project.h>
#include <C78E/Project/ProjectManager.h>

// Renderer API
#include <C78E/Renderer/RenderCommand.h>

#include <C78E/Renderer/API/RendererAPI.h>
#include <C78E/Renderer/API/GraphicsContext.h>
#include <C78E/Renderer/API/Buffer.h>
#include <C78E/Renderer/API/VertexArray.h>
#include <C78E/Renderer/API/Framebuffer.h>
#include <C78E/Renderer/API/UniformBuffer.h>
#include <C78E/Renderer/API/Texture.h>
#include <C78E/Renderer/API/Shader.h>

#include <C78E/Renderer/Camera/Camera.h>
#include <C78E/Renderer/Camera/OrthographicCamera.h>
#include <C78E/Renderer/Camera/EditorCamera.h>
#include <C78E/Renderer/Camera/Controller/CameraController.h>

#include <C78E/Renderer/Systems/Renderer.h>
#include <C78E/Renderer/Systems/3D/3DRenderer.h>
#include <C78E/Renderer/Systems/3D/Rasterizer/3DRasterizer.h>
#include <C78E/Renderer/Systems/3D/Raytracer/3DRaytracer.h>

// Assets
#include <C78E/Asset/Asset.h>
#include <C78E/Asset/AssetManager.h>

#include <C78E/Asset/Font/Font.h>
#include <C78E/Asset/Font/FontManager.h>

#include <C78E/Asset/Material/Material.h>
#include <C78E/Asset/Material/MaterialManager.h>

#include <C78E/Asset/Mesh/Mesh.h>
#include <C78E/Asset/Mesh/MeshManager.h>

#include <C78E/Asset/Scene/Scene.h>
#include <C78E/Asset/Scene/SceneManager.h>

#include <C78E/Asset/Shader/Shader.h>
#include <C78E/Asset/Shader/ShaderManager.h>

#include <C78E/Asset/Texture/Texture.h>
#include <C78E/Asset/Texture/TextureManager.h>

//GUI
#include <C78E/GUI/GUIUtils.h>


#ifdef C78E_ENTRYPOINT
#include "C78E/Core/EntryPoint.h"
#endif