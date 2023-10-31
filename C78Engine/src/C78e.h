#pragma once

#include "C78E/Core/Core.h"
#include "C78E/Core/Log.h"
#include "C78E/Core/Timestep.h"
#include "C78E/Core/Application.h"
#include "C78E/Core/Types.h"
#include "C78E/Core/UUID.h"

#include "C78E/Core/Layer.h"
#include "C78E/ImGui/ImGuiLayer.h"

#include "C78E/Core/Input.h"
#include "C78E/Core/KeyCodes.h"
#include "C78E/Core/MouseCodes.h"

#include "C78E/Renderer/RendererAPI.h"
#include "C78E/Renderer/RenderCommand.h"
#include "C78E/Renderer/Renderer.h"
#include "C78E/Renderer/Renderer2D.h"
#include "C78E/Renderer/Renderer3D.h"

#include "C78E/Renderer/Buffer.h"
#include "C78E/Renderer/VertexArray.h"
#include "C78E/Renderer/UniformBuffer.h"

#include "C78E/Renderer/Assets/Material/Material.h"
#include "C78E/Renderer/Assets/Material/MaterialSystem.h"
#include "C78E/Renderer/Assets/Model/Mesh.h"
#include "C78E/Renderer/Assets/Model/ModelManager.h"
#include "C78E/Renderer/Assets/Shader/Shader.h"
#include "C78E/Renderer/Assets/Shader/ShaderLibrary.h"
#include "C78E/Renderer/Assets/Texture/RawImage.h"
#include "C78E/Renderer/Assets/Texture/Texture.h"
#include "C78E/Renderer/Assets/Texture/TextureManager.h"

#include "C78E/Renderer/Camera.h"
#include "C78E/Renderer/EditorCamera.h"
#include "C78E/Renderer/OrthographicCamera.h"

#include "C78E/Scene/Components.h"
#include "C78E/Scene/Entity.h"
#include "C78E/Scene/Scene.h"
#include "C78E/Scene/ScriptableEntity.h"
#include "C78E/Renderer/GenericShape.h"
#include "C78E/Renderer/Assets/Texture/TextureManager.h"
#include "C78E/Renderer/Assets/Model/ModelManager.h"


#ifdef C78E_ENTRYPOINT
#include "C78E/Core/EntryPoint.h"
#endif