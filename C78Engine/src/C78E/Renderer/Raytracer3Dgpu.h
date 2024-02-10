#pragma once

#include <C78E/Assets/Texture/RawImage.h>
#include <C78E/Assets/AssetManager.h>

#include <C78E/Renderer/Buffer.h>
#include <C78E/Renderer/VertexArray.h>

#include <C78E/Scene/Scene.h>


namespace C78E {
	class RayTracer3Dgpu {
	public:

		static void init();

		static void setViewport(uint32_t width, uint32_t height);

		static void submit(Ref<Scene> scene);

		static void compute();

		static void display();

	private:
		static float s_FOV;
		static uint32_t s_Width, s_Height;

		static Ref<Scene> s_Scene;

		static Asset<Shader> s_ShaderAsset;
		static Ref<VertexBuffer> s_VertexBuffer;
		static Ref<IndexBuffer> s_IndexBuffer;
		static Ref<VertexArray> s_VertexArray;
		static Ref<Texture2D> s_Texture;

		static Asset<Shader> s_ComputeShaderAsset;
	};
}
