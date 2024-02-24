#include "C78EPCH.h"
#include "Renderer.h"
#include <C78E/Renderer/Systems/Renderer2D.h>
#include <C78E/Renderer/Systems/Renderer3D.h>
#include <C78E/Renderer/Systems/Raytracer3D.h>

namespace C78E {

	Renderer::RenderMode Renderer::s_RenderMode = Renderer::RenderMode::NONE;
	Ref<Scene> Renderer::s_Scene = nullptr;
	Ref<Texture2D> Renderer::s_Result = nullptr;
	
	Renderer::DisplayAssets Renderer::s_DisplayAssets;

	void Renderer::init(uint32_t width, uint32_t height) {
		RenderCommand::init();
		Renderer3D::init(width, height);
		RayTracer3D::init(width, height);

		initDisplayAssets();
	}

	void Renderer::shutdown()
	{
		Renderer3D::shutdown();
		RayTracer3D::shutdown();
	}

	void Renderer::setDisplayShader(std::string name) {
		s_DisplayAssets.displayShader = AssetManager::getShaderAsset(name);
		s_DisplayAssets.init = true;
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
		Renderer3D::onWindowResize(width, height);
		RayTracer3D::onWindowResize(width, height);
	}

	void Renderer::beginScene(SceneCamera& camera) {
		
	}

	void Renderer::beginScene(EditorCamera& camera) {

	}

	void Renderer::submit(Ref<Scene> scene) {
		s_Scene = scene;
	}

	void Renderer::endScene()
	{
	}

	void Renderer::render() {
		if (!s_Scene) return;

		switch (s_RenderMode) {
		case C78E::Renderer::NONE:
			/* No Rendering */
			break;
		case C78E::Renderer::RASTERIZE_2D:
			//Renderer2D::Flush(); render
			break;
		case C78E::Renderer::RASTERIZE_3D:
			Renderer3D::render(s_Scene);
			s_Result = Renderer3D::getResult();
			break;
		case C78E::Renderer::RAYTRACE_3D:
			RayTracer3D::render(s_Scene);
			s_Result = RayTracer3D::getResult();
			break;
		default:
			break;
		}
	}

	void Renderer::display() {
		if (!s_DisplayAssets.init || !s_Result) return;
		RenderCommand::clear();

		s_DisplayAssets.displayShader.get().Bind();
		s_Result->bind(0);
		RenderCommand::drawIndexed(s_DisplayAssets.vertexArray);
	}



















	void Renderer::initDisplayAssets() {
		float vertecies[20] = {
			-1.f, -1.f, 0.f, 0.f, 0.f,
			+1.f, -1.f, 0.f, 1.f, 0.f,
			+1.f, +1.f, 0.f, 1.f, 1.f,
			-1.f, +1.f, 0.f, 0.f, 1.f
		};
		uint32_t indecies[6] = { 0,1,2,2,3,0 };

		s_DisplayAssets.vertexBuffer = VertexBuffer::Create(vertecies, 4*20);
		s_DisplayAssets.vertexBuffer->SetLayout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float2, "a_TexCoord" } });

		s_DisplayAssets.indexBuffer = IndexBuffer::Create(indecies, 6);

		s_DisplayAssets.vertexArray = VertexArray::Create();
		s_DisplayAssets.vertexArray->AddVertexBuffer(s_DisplayAssets.vertexBuffer);
		s_DisplayAssets.vertexArray->SetIndexBuffer(s_DisplayAssets.indexBuffer);
	}

}
