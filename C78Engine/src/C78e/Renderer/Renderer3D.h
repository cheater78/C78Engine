#pragma once

#include "C78e/Renderer/OrthographicCamera.h"

#include "C78e/Renderer/Texture.h"

#include "C78e/Renderer/Camera.h"
#include "C78e/Renderer/EditorCamera.h"
#include "C78e/Renderer/Font.h"

#include "C78e/Scene/Components.h"

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
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};

}
