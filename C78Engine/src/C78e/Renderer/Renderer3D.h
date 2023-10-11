#pragma once

#include "C78E/Scene/Scene.h"

#include "C78E/Core/Core.h"
#include "C78E/Renderer/Buffer.h"
#include "C78E/Renderer/VertexArray.h"
#include "C78E/Renderer/UniformBuffer.h"
#include "C78E/Renderer/Assets/Shader/Shader.h"

namespace C78E {

	class Renderer3D
	{
	public:
		static void init();
		static void shutdown();

		static void beginScene(Entity camera);

		static void submit(Ref<Scene> scene);

		static void setShader(std::string shader);

		static void endScene();
		static void flush();

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t Vertecies = 0;
			uint32_t Indicies = 0;
		};
		static void resetStats();
		static Statistics getStats();

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
			Ref<UniformBuffer> sceneLightUniformBuffer;
			CameraUniform cameraUniform;
			Ref<UniformBuffer> entityMaterialUniformBuffer;
			Ref<UniformBuffer> entityUniformBuffer;
		};

		static Renderer3DData s_Data;

		static void startBatch();
		static void nextBatch();
	};

}
