#include "C78EPCH.h"
#include "3DRasterizer.h"

namespace C78E {
	uint32_t Rasterizer3D::s_MaxTextureSlots = 0;


	Ref<UniformBuffer> getEntityUBO(Entity entity, uint32_t binding = 0) {
		TransformComponent& transformComponent = entity.getTransform();
		EntityModelUniform entityUniform{ transformComponent.getTransform(), transformComponent.normalMat() };
		Ref<UniformBuffer> entityUniformBuffer = C78E::UniformBuffer::create(sizeof(EntityModelUniform), binding);
		entityUniformBuffer->setData(&entityUniform, sizeof(EntityModelUniform));
		return entityUniformBuffer;
	}

	void Rasterizer3D::submitSprites(C78E::Ref<Scene> scene) {
		for (auto& entt : scene->getAllEntitiesWith<SpriteRendererComponent>()) {
			Entity entity(entt, scene.get());

			m_CurrentScene->renderPasses.push_back(RenderPass());
			auto& pass = m_CurrentScene->renderPasses.back();

			pass.material = C78E::createRef<C78E::Material>();
			pass.material->m_Shader = EditorAssetManager::Default::Shader_SpriteRenderComponent;

			// Default Texture
			pass.textureSlots.push_back(std::make_pair(EditorAssetManager::Default::Texture2D_White, m_AssetManager->getAssetAs<Texture2D>(EditorAssetManager::Default::Texture2D_White)));

			m_CurrentScene->addCamUBOToCurrentRenderPass(); // Camera UBO
			pass.uniformBuffers.push_back(getEntityUBO(entity, 1)); // Entity UBO

			SpriteRendererComponent& spriteComponent = entity.getComponent<SpriteRendererComponent>();

			C78E::Geometry::Quad spriteBase{};
			uint32_t indCount = spriteBase.getIndexCount();
			uint32_t* indecies = static_cast<uint32_t*>(spriteBase.getIndexData());


			{ //Vertex
				uint32_t vertSize = spriteBase.getVertexSize();
				float* vertecies = (float*)spriteBase.getVertexData();
				Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(vertecies, vertSize);
				vertexBuffer->setLayout({
						{ ShaderDataType::Float3, "position" },
						{ ShaderDataType::Float3, "normal" }
					}
				);
				pass.vertexBuffers.push_back(vertexBuffer);
			}

			glm::vec4 color = spriteComponent.color;
			{ //Vertex Color
				uint32_t colorVertSize = spriteBase.getVertexCount() * sizeof(glm::vec4);
				glm::vec4 colorVertecies[4] = { color, color, color, color };
				Ref<VertexBuffer> vertexColorBuffer = VertexBuffer::create(&colorVertecies[0].x, colorVertSize);
				vertexColorBuffer->setLayout({
						{ ShaderDataType::Float4, "color" }
					}
				);
				pass.vertexBuffers.push_back(vertexColorBuffer);
			}


			AssetHandle texture = spriteComponent.texture;
			{ //Vertex Texture
				int textureID = 0;
				if (texture.isValid()) {
					AssetHandle texHandle = texture;
					auto& texSlots = pass.textureSlots;

					C78_CORE_ASSERT(texSlots.size() <= RenderCommand::getMaxTextureSlots(RendererAPI::ShaderType::FRAGMENT));

					for (uint32_t i = 0; i < texSlots.size(); i++)
						if (texSlots.at(i).first == texHandle) {
							textureID = i;
							break;
						}

					if (!textureID) {
						textureID = texSlots.size();
						texSlots.push_back(std::make_pair(texHandle, m_AssetManager->getAssetAs<Texture2D>(texHandle)));
					}

				}

				float tf = spriteComponent.tilingFactor;
				uint32_t textureVertSize = spriteBase.getVertexCount() * C78E::Primitive::VertexTexture::getSize();
				C78E::Primitive::VertexTexture textureVertecies[4] = {
					{ { 0.f, 0.f }, textureID },
					{ { tf, 0.f }, textureID },
					{ { tf, tf }, textureID },
					{ { 0.f, tf }, textureID },
				};
				Ref<VertexBuffer> vertexColorBuffer = VertexBuffer::create(&textureVertecies[0].textureCoordinate.x, textureVertSize);
				vertexColorBuffer->setLayout({
						{ ShaderDataType::Float2, "textureCoordinate" },
						{ ShaderDataType::Int, "textureSlot" }
					}
				);
				pass.vertexBuffers.push_back(vertexColorBuffer);
			}

			{
				pass.indexBuffer = IndexBuffer::create(indecies, indCount);
			}

			{
				pass.vertexArray = VertexArray::create();
				for (auto vbo : pass.vertexBuffers)
					pass.vertexArray->addVertexBuffer(vbo);
				pass.vertexArray->setIndexBuffer(pass.indexBuffer);
			}


		}
	}


	Rasterizer3D::Rasterizer3D(Ref<AssetManager> assetManager)
		: Renderer3D(assetManager){
	}

	Rasterizer3D::~Rasterizer3D() {
	}

	bool Rasterizer3D::beginScene(Camera& camera, const glm::mat4& viewMatrix) {
		m_CurrentScene = createRef<RenderScene>();
		m_CurrentScene->cameraUniform = CameraUniform{ camera.getProjection(), viewMatrix, glm::inverse(viewMatrix) };
		m_CurrentScene->renderPasses.clear();
		return false;
	}

	void Rasterizer3D::submit(Ref<Scene> scene) {
		if (!scene) return;

		submitSprites(scene);

	}

	bool Rasterizer3D::endScene() {
		render(m_CurrentScene);
		m_CurrentScene.reset();
		return false;
	}

	



	void Rasterizer3D::render(Ref<RenderScene> currentScene) {
		

		m_BusyWorking.lock();
		m_WorkingFrame->bind();

		RenderCommand::setClearColor({ 0.3f, 0.05f, 0.4f, 1.0f });
		RenderCommand::clear();

		FrameInfo info;

		for (auto& pass : currentScene->renderPasses) {
			C78_CORE_ASSERT(pass.textureSlots.size() <= RenderCommand::getMaxTextureSlots(RendererAPI::ShaderType::FRAGMENT));

			Ref<Shader> shader = m_AssetManager->getAssetAs<Shader>(pass.material->m_Shader);

			shader->bind();

			{
				uint32_t binding = 0;
				for (auto ubo : pass.uniformBuffers)
					ubo->bind(binding++);
			}

			{
				uint32_t binding = 0;
				for (auto tex : pass.textureSlots)
					tex.second->bind(binding++);
			}

			RenderCommand::enableDepthTest(pass.writeDepthBuffer);
			RenderCommand::setDepthFunc(pass.depthFunc);


			info.vertecies += 0;
			info.indicies += pass.indexBuffer->getCount();
			RenderCommand::drawIndexed(pass.vertexArray, pass.indexBuffer->getCount());
			info.drawCalls++;
			shader->unbind();
		}

		m_WorkingFrame->unbind();

		m_BusyTarget.lock();
		m_TargetFrame.swap(m_WorkingFrame);

		m_TargetFrameInfo = info;

		m_BusyTarget.unlock();
		m_BusyWorking.unlock();
	}

	bool Rasterizer3D::display() {
		return false;
	}

	inline const Renderer::RendererType Rasterizer3D::getType() const { return Renderer::RendererType::Rasterizer3D; }

	void Rasterizer3D::RenderScene::addCamUBOToCurrentRenderPass() {
		if (renderPasses.empty()) return;
		Ref<UniformBuffer> cameraUniformBuffer = C78E::UniformBuffer::create(sizeof(CameraUniform), renderPasses.back().uniformBuffers.size());
		cameraUniformBuffer->setData(&cameraUniform, sizeof(CameraUniform));
		renderPasses.back().uniformBuffers.push_back(cameraUniformBuffer);
	}

}
