#include "C78EPCH.h"
#include "C78E/Renderer/Renderer.h"
#include "C78E/Renderer/Renderer2D.h"

namespace C78E {

	Scope<Renderer::SceneData> Renderer::s_SceneData = createScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		//C78_PROFILE_FUNCTION();

		RenderCommand::init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::setViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		//shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		//shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::drawIndexed(vertexArray);
	}

}
