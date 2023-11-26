#pragma once

#include "C78E/Scene/Scene.h"

#include "C78E/Core/Core.h"
#include "C78E/Renderer/RendererAPI.h"
#include "C78E/Renderer/Buffer.h"
#include "C78E/Renderer/VertexArray.h"
#include "C78E/Renderer/UniformBuffer.h"
#include "C78E/Renderer/Assets/Shader/Shader.h"
#include "C78E/Renderer/Assets/Texture/Texture.h"
#include "C78E/Renderer/Assets/Texture/RawImage.h"

namespace C78E {

	class Renderer3D {
	public:
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t vertecies = 0;
			uint32_t indicies = 0;
			float frameTime = 0.f;
		};

		struct Renderer3DSceneElements {
			bool skyBox = true;
			bool texMesh = true;
			bool pointLightSprites = true;
			bool spotLightSprites = true;
		};

	public:
		static void init();
		static void shutdown();

		static void beginScene(std::string scene);
		static void submit(Ref<Scene>& scene);
		static void submit(Entity entity);
		static void endScene();
		static void render(std::string scene);

		static void setSceneElements(Renderer3DSceneElements elements) { curr()->elements = elements; }
		static Ref<Scene> getScene(std::string name) { return curr(name)->scene; }

		static void resetStats();
		static void resetStats(std::string scene);
		static Statistics getStats();
		static Statistics getStats(std::string scene);
	

	private:
		static uint32_t s_MaxTextureSlots;
		static Ref<Texture2D> s_WhiteTexture;

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
			Renderer3D::Statistics stats{};
			Ref<Scene> scene = nullptr;
			Renderer3D::Renderer3DSceneElements elements{};
			std::vector<Renderer3DPass> renderPasses{};
		};

		

		static std::string s_ActiveScene;
		static std::unordered_map<std::string, Ref<Renderer3DScene>> s_Scenes;

		static Ref<Renderer3DScene> curr() { C78_CORE_ASSERT(!s_ActiveScene.empty(), ""); return s_Scenes.at(s_ActiveScene); }
		static Ref<Renderer3DScene> curr(std::string scene) { C78_CORE_ASSERT(s_Scenes.find(scene) != s_Scenes.end(), ""); return s_Scenes.at(scene); }

		static Ref<UniformBuffer> currentCameraUniformBuffer();
		static Ref<UniformBuffer> currentLightsUniformBuffer();

		static void submitSkyBox();
		static void submitTexMeshes();
		static void submitPointLights();
		static void submitSpotLights();
	};

}
