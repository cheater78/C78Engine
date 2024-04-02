#pragma once

#include "C78E/Scene/Scene.h"

#include "C78E/Core/Core.h"
#include "C78E/Renderer/RendererAPI.h"
#include "C78E/Renderer/Buffer.h"
#include "C78E/Renderer/VertexArray.h"
#include "C78E/Renderer/UniformBuffer.h"
#include "C78E/Assets/Shader/Shader.h"
#include "C78E/Assets/Texture/Texture.h"
#include "C78E/Assets/Texture/Image.h"
#include "C78E/Renderer/Framebuffer.h"

namespace C78E {

	class Renderer3D {
	public:
		static void init(uint32_t width, uint32_t height);
		static void shutdown();

		static void onWindowResize(uint32_t width, uint32_t height);

		static void render(Ref<Scene> scene);

		static Ref<Texture2D> getResult() { return s_Result; }
	private:
		struct Renderer3DPassConfig {
			bool writeDepthBuffer = true;
			RendererAPI::DepthFunc depthFunc = RendererAPI::DepthFunc::LESS;
		};
		struct Renderer3DPass {
			uint32_t passes = 1;
			Ref<Shader> shader = nullptr;
			Ref<VertexBuffer> vertexBuffer = nullptr;
			Ref<IndexBuffer> indexBuffer = nullptr;
			Ref<VertexArray> vertexArray = nullptr;
			std::vector<Ref<Texture>> textureSlots{};
			std::vector<Ref<UniformBuffer>> uniformBuffers{};
			Renderer3DPassConfig config{};
		};
		struct Renderer3DScene {
			Ref<Scene> scene = nullptr;
			std::vector<Renderer3DPass> renderPasses{};
		};
	private:
		static void submitSkyBox(Renderer3DScene& rScene);
		static void submitTexMeshes(Renderer3DScene& rScene);
		static void submitModels(Renderer3DScene& rScene);
		static void submitPointLights(Renderer3DScene& rScene);
		static void submitSpotLights(Renderer3DScene& rScene);

		static Ref<UniformBuffer> currentCameraUniformBuffer(Renderer3DScene& rScene);
		static Ref<UniformBuffer> currentLightsUniformBuffer(Renderer3DScene& rScene);
	private:
		static uint32_t s_MaxTextureSlots;
		static Ref<Texture2D> s_WhiteTexture;

		static uint32_t s_Width, s_Height;
		static Ref<Texture2D> s_Result;
		static Ref<Framebuffer> s_FrameBuffer;
	};

}
