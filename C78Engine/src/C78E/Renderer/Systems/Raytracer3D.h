#pragma once

#include <C78E/Assets/Texture/Texture.h>
#include <C78E/Assets/AssetManager.h>

#include <C78E/Renderer/Buffer.h>
#include <C78E/Renderer/VertexArray.h>

#include <C78E/Scene/Scene.h>
#include <C78E/Renderer/Framebuffer.h>


namespace C78E {
	class RayTracer3D {
	public:
		static void init(uint32_t width, uint32_t height);
		static void shutdown();

		static void onWindowResize(uint32_t width, uint32_t height);

		static void render(Ref<Scene> scene);

		static Ref<Texture2D> getResult() { return s_Result; }

	private:
		static uint32_t s_Width, s_Height;
		static AssetHandle s_RayComputeShader;
		static Ref<Texture2D> s_Result;
	};
}
