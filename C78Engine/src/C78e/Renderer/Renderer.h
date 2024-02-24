#pragma once

#include <C78E/Renderer/RenderCommand.h>
#include <C78E/Assets/AssetManager.h>
#include <C78E/Scene/SceneCamera.h>
#include <C78E/Scene/Scene.h>

namespace C78E {

	class Renderer {
	public:
		enum RenderMode {
			NONE,
			RASTERIZE_2D,
			RASTERIZE_3D,
			RAYTRACE_3D
		};
	public:
		static void init(uint32_t width, uint32_t height);
		static void shutdown();
		
		static void setDisplayShader(std::string name);

		static void onWindowResize(uint32_t width, uint32_t height);

		static void beginScene(SceneCamera& camera);
		static void beginScene(EditorCamera& camera);
		static void submit(Ref<Scene> scene);
		static void submit(Ref<Entity> entity);
		static void endScene();

		static void render();
		static void display();

		static Ref<Texture2D> getResult() { return s_Result; }

		static void setRenderMode(RenderMode mode) { s_RenderMode = mode; }
		static RenderMode getRenderMode() { return s_RenderMode; }

		static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	private:
		struct DisplayAssets {
			bool init = false;
			Asset<Shader> displayShader;
			Ref<VertexBuffer> vertexBuffer = nullptr;
			Ref<IndexBuffer> indexBuffer = nullptr;
			Ref<VertexArray> vertexArray = nullptr;
		};
	private:
		static void initDisplayAssets();

	private:
		static RenderMode s_RenderMode;
		static Ref<Scene> s_Scene;
		static Ref<Texture2D> s_Result;
		
		static DisplayAssets s_DisplayAssets;

	};
}
