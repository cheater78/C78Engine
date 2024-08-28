#pragma once
// Core
#include "C78E/Core/Core.h"
#include "C78E/Core/Log.h"
#include "C78E/Core/Timestep.h"
#include "C78E/Core/UUID.h"
#include "C78E/Core/Types.h"
#include "C78E/Core/Application.h"
#include <C78E/Core/FileSystem/Extensions.h>


// ImGui
#include "C78E/Core/Layer.h"
#include "C78E/GUI/ImGui/ImGuiLayer.h"


// User Input
#include "C78E/Core/Input.h"
#include "C78E/Core/KeyCodes.h"
#include "C78E/Core/MouseCodes.h"


// Rendering

#include "C78E/Renderer/API/RendererAPI.h"
#include "C78E/Renderer/API/Buffer.h"
#include "C78E/Renderer/API/VertexArray.h"
#include "C78E/Renderer/API/UniformBuffer.h"

#include "C78E/Renderer/Camera/Camera.h"
#include "C78E/Renderer/Camera/EditorCamera.h"
#include "C78E/Renderer/Camera/OrthographicCamera.h"

#include "C78E/Renderer/RenderCommand.h"

#include "C78E/Renderer/Systems/Renderer.h"
#include "C78E/Renderer/Systems/3D/3DRenderer.h"
#include "C78E/Renderer/Systems/3D/Rasterizer/3DRasterizer.h"
#include "C78E/Renderer/Systems/3D/Raytracer/3DRaytracer.h"





// Project
#include <C78E/Project/Project.h>
#include <C78E/Project/ProjectManager.h>

// Assets

#include "C78E/Renderer/API/Texture.h"

#include "C78E/Renderer/Assets/AssetManager.h"


#include "C78E/Renderer/Assets/Texture/Image.h"
#include "C78E/Renderer/Assets/Texture/TextureImporter.h"

#include "C78E/Renderer/Assets/Scene/Entity/Component/Components.h"
#include "C78E/Renderer/Assets/Scene/Entity/Entity.h"
#include "C78E/Renderer/Assets/Scene/Scene.h"
#include "C78E/Renderer/Assets/Scene/SceneManager.h"
#include "C78E/Renderer/Assets/Scene/SceneSerializer.h"
#include "C78E/Renderer/Assets/Scene/Entity/ScriptableEntity.h"

//GUI
#include <C78E/GUI/GUIUtils.h>
#include <C78E/GUI/GUISystems.h>
#include <C78E/GUI/GUIElements.h>
#include <C78E/GUI/GUIExtensions.h>
#include <C78E/GUI/FileManager/FileManager.h>


// Utils
#include <C78E/Utils/PlatformUtils.h>
#include <C78E/Utils/Yaml/YamlUtils.h>
#include <C78E/Utils/Wavefront/WavefrontLoader.h>


#ifdef C78E_ENTRYPOINT
#include "C78E/Core/EntryPoint.h"
#endif