#pragma once

#include "C78e/Scene/Scene.h"

#include "C78e/Core/Core.h"
#include "C78e/Renderer/Buffer.h"
#include "C78e/Renderer/VertexArray.h"
#include "C78e/Renderer/UniformBuffer.h"
#include "C78e/Renderer/Shader.h"

namespace C78e {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(Entity camera);

		static void submit(Ref<Scene> scene);

		static void EndScene();
		static void Flush();

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t Vertecies = 0;
			uint32_t Indicies = 0;
		};
		static void ResetStats();
		static Statistics GetStats();

	private:

		struct Renderer3DData {
			static const uint32_t MaxVertices = 1048576;
			static const uint32_t MaxIndices = 16777216;
			static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

			Renderer3D::Statistics Stats;

			Ref<VertexArray> vertexArray;
			Ref<VertexBuffer> vertexBuffer;
			Ref<Shader> shader;
			Ref<Texture2D> whiteTexture;

			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			uint32_t TextureSlotIndex = 1; // 0 = white texture

			Ref<Scene> Scene;
			Ref<UniformBuffer> cameraUniformBuffer;
			CameraUniform cameraUniform;
			Ref<UniformBuffer> entityUniformBuffer;
		};

		static Renderer3DData s_Data;

		static void StartBatch();
		static void NextBatch();
	};

}
