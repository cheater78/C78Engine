#pragma once
// Core
#include "C78E/Core/Core.h"
#include "C78E/Core/Log.h"
#include "C78E/Core/Timestep.h"
#include "C78E/Core/UUID.h"
#include "C78E/Core/Types.h"
#include "C78E/Core/Application.h"


// ImGui
#include "C78E/Core/Layer.h"
#include "C78E/ImGui/ImGuiLayer.h"


// User Input
#include "C78E/Core/Input.h"
#include "C78E/Core/KeyCodes.h"
#include "C78E/Core/MouseCodes.h"


// Rendering
#include "C78E/Renderer/RendererAPI.h"
#include "C78E/Renderer/RenderCommand.h"
#include "C78E/Renderer/Renderer.h"

#include "C78E/Renderer/Buffer.h"
#include "C78E/Renderer/VertexArray.h"
#include "C78E/Renderer/UniformBuffer.h"

#include "C78E/Renderer/Camera.h"
#include "C78E/Renderer/EditorCamera.h"
#include "C78E/Renderer/OrthographicCamera.h"


// Assets
#include <C78E/Project/Project.h>

#include "C78E/Assets/AssetManager.h"


#include "C78E/Assets/Texture/Image.h"
#include "C78E/Assets/Texture/TextureLoader.h"

#include "C78E/Scene/Components.h"
#include "C78E/Scene/Entity.h"
#include "C78E/Scene/Scene.h"
#include "C78E/Scene/ScriptableEntity.h"
#include "C78E/Scene/SceneSerializer.h"
#include "C78E/Renderer/GenericShape.h"


// Utils
#include <C78E/Utils/PlatformUtils.h>
#include <C78E/Utils/yaml-cpp/YamlUtils.h>


#ifdef C78E_ENTRYPOINT
#include "C78E/Core/EntryPoint.h"
#endif