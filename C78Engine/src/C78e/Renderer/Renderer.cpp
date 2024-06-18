#include "C78EPCH.h"
#include "Renderer.h"

namespace C78E {

	Renderer::RenderMode Renderer::s_RenderMode = Renderer::RenderMode::NONE;
	Ref<Scene> Renderer::s_Scene = nullptr;
	Ref<Texture2D> Renderer::s_Result = nullptr;
	
	Renderer::DisplayAssets Renderer::s_DisplayAssets;

	void Renderer::init(uint32_t width, uint32_t height) {

		initDisplayAssets();
	}

	void Renderer::shutdown()
	{
	}

	void Renderer::setDisplayShader(std::string name) {
		C78_CORE_ERROR("impl!");
		//s_DisplayAssets.displayShader = AssetManager::getShaderAsset(name);
		s_DisplayAssets.init = true;
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
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
	}

	void Renderer::display() {
		if (!s_DisplayAssets.init || !s_Result) return;
		RenderCommand::clear();

		C78_CORE_ERROR("impl!");
		//s_DisplayAssets.displayShader.get().Bind();
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

		s_DisplayAssets.vertexBuffer = VertexBuffer::create(vertecies, 4*20);
		s_DisplayAssets.vertexBuffer->setLayout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float2, "a_TexCoord" } });

		s_DisplayAssets.indexBuffer = IndexBuffer::create(indecies, 6);

		s_DisplayAssets.vertexArray = VertexArray::create();
		s_DisplayAssets.vertexArray->addVertexBuffer(s_DisplayAssets.vertexBuffer);
		s_DisplayAssets.vertexArray->setIndexBuffer(s_DisplayAssets.indexBuffer);
	}

}
