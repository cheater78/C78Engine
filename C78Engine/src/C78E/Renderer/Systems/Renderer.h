#pragma once
#include <C78E/Renderer/RenderCommand.h>

#include <C78E/Renderer/API/Buffer.h>
#include <C78E/Renderer/API/Framebuffer.h>
#include <C78E/Renderer/API/GraphicsContext.h>
#include <C78E/Renderer/API/RendererAPI.h>
#include <C78E/Renderer/API/Shader.h>
#include <C78E/Renderer/API/Texture.h>
#include <C78E/Renderer/API/UniformBuffer.h>
#include <C78E/Renderer/API/VertexArray.h>

#include <C78E/Renderer/Assets/AssetManager.h>
#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/Assets/Material/Material.h>
#include <C78E/Renderer/Assets/Mesh/Mesh.h>
#include <C78E/Renderer/Assets/Model/Model.h>
#include <C78E/Renderer/Assets/Scene/Entity/Component/Components.h>
#include <C78E/Renderer/Assets/Scene/Entity/Entity.h>
#include <C78E/Renderer/Assets/Scene/Scene.h>

#include <C78E/Renderer/Assets/Scene/Entity/Component/Uniform.h>

namespace C78E {

	class Renderer {
	public:
		enum RendererType : uint8_t {
			None = 0, // No Renderer -> TODO: Black Frame as result
			//2D
			Rasterizer2D,
			//3D
			Rasterizer3D,
			Raytracer3D
		};

		struct FrameInfo {
			uint32_t drawCalls = 0;
			uint32_t vertecies = 0;
			uint32_t indicies = 0;
		};

		struct TargetConfig {
			uint32_t width = 1;
			uint32_t height = 1;
			

		};

	public:
		Renderer(Ref<AssetManager> assetManager);
		Renderer(const Renderer& other) = delete;
		~Renderer();

		virtual bool render(Ref<Scene> scene) = 0;

		virtual bool display() = 0;

		virtual const bool isTargetReady() const;
		virtual const bool invalidateTarget();
		virtual Ref<Texture2D> getTargetTexture() const;

		virtual FrameInfo getTargetInfo() const;
		virtual void resizeTarget(uint32_t width, uint32_t height);

		virtual inline const RendererType getType() const;

	protected:
		virtual bool initTarget() final;

		virtual void resetTargetFrameInfo();

	protected:
		Ref<AssetManager> m_AssetManager = nullptr;

		
		Ref<std::thread> m_RenderThread = nullptr;

		FrameInfo m_TargetFrameInfo{};

		std::binary_semaphore m_BusyTarget{ 1 };
		Ref<Framebuffer> m_TargetFrame = nullptr;

		std::binary_semaphore m_BusyWorking{ 1 };
		Ref<Framebuffer> m_WorkingFrame = nullptr;

	};

}
