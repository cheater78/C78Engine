#include "C78EPCH.h"
#include "3DRasterizer.h"

#include <C78E/Renderer/Assets/Font/MSDFData.h>

namespace C78E {
	uint32_t Rasterizer3D::s_MaxTextureSlots = 0;


	Ref<UniformBuffer> getEntityUBO(Entity entity, uint32_t binding = 0) {
		TransformComponent& transformComponent = entity.getTransform();
		EntityModelUniform entityUniform{ transformComponent.getTransform(), transformComponent.normalMat() };
		Ref<UniformBuffer> entityUniformBuffer = C78E::UniformBuffer::create(sizeof(EntityModelUniform), binding);
		entityUniformBuffer->setData(&entityUniform, sizeof(EntityModelUniform));
		return entityUniformBuffer;
	}

	Ref<UniformBuffer> getMaterialUBO(Ref<Material> material, uint32_t binding = 0) {
		Ref<UniformBuffer> entityUniformBuffer = C78E::UniformBuffer::create(sizeof(MaterialUniform), binding);
		entityUniformBuffer->setData(&material->m_MaterialProperties, sizeof(MaterialUniform));
		return entityUniformBuffer;
	}

	void Rasterizer3D::submitSpriteComponents(C78E::Ref<Scene> scene) {
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
				uint32_t vertSize = static_cast<uint32_t>(spriteBase.getVertexSize());
				float* vertecies = (float*)spriteBase.getVertexData();
				Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(vertecies, vertSize);
				vertexBuffer->setLayout({
						{ ShaderDataType::Float3, "a_Position" },
						{ ShaderDataType::Float3, "a_Normal" }
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
						{ ShaderDataType::Float4, "a_Color" }
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
				Ref<VertexBuffer> vertexTextureBuffer = VertexBuffer::create(&textureVertecies[0].textureCoordinate.x, textureVertSize);
				vertexTextureBuffer->setLayout({
						{ ShaderDataType::Float2, "a_TextureCoordinate" },
						{ ShaderDataType::Int, "a_TextureSlot" }
					}
				);
				pass.vertexBuffers.push_back(vertexTextureBuffer);
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

	void Rasterizer3D::submitTextComponents(C78E::Ref<Scene> scene) {
		for (auto& entt : scene->getAllEntitiesWith<TextComponent>()) {
			Entity entity(entt, scene.get());
			TransformComponent& transformCompoent = entity.getTransform();
			TextComponent& textComponent = entity.getComponent<TextComponent>();
			if (!textComponent.fontAsset.isValid()) continue;
			if (textComponent.textString.empty()) continue;
			Ref<Font> font = m_AssetManager->getAssetAs<Font>(textComponent.fontAsset);
			const auto& fontGeometry = font->getMSDFData()->fontGeometry;
			const auto& metrics = fontGeometry.getMetrics();
			Ref<Texture2D> fontAtlas = font->getAtlasTexture();


			m_CurrentScene->renderPasses.push_back(RenderPass());
			auto& pass = m_CurrentScene->renderPasses.back();

			pass.material = C78E::createRef<C78E::Material>();
			pass.material->m_Shader = EditorAssetManager::Default::Shader_TextCompoent;

			// Default Texture
			pass.textureSlots = { { textComponent.fontAsset , fontAtlas } };

			m_CurrentScene->addCamUBOToCurrentRenderPass(); // Camera UBO
			pass.uniformBuffers.push_back(getEntityUBO(entity, 1)); // Entity UBO


			double x = 0.0;
			double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
			double y = 0.0;

			const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

			std::vector<glm::vec3> vertecies;
			std::vector<C78E::Primitive::VertexColor> colorVertecies;
			std::vector<glm::vec2> textureVertecies;
			std::vector<uint32_t> indecies;

			for (size_t i = 0; i < textComponent.textString.size(); i++) {
				char character = textComponent.textString[i];
				if (character == '\r')
					continue;

				if (character == '\n') {
					x = 0;
					y -= fsScale * metrics.lineHeight + textComponent.lineSpacing;
					continue;
				}

				if (character == ' ') {
					float advance = spaceGlyphAdvance;
					if (i < textComponent.textString.size() - 1)
					{
						char nextCharacter = textComponent.textString[i + 1];
						double dAdvance;
						fontGeometry.getAdvance(dAdvance, character, nextCharacter);
						advance = (float)dAdvance;
					}

					x += fsScale * advance + textComponent.kerning;
					continue;
				}

				if (character == '\t') {
					// NOTE(Yan): is this right?
					x += 4.0f * (fsScale * spaceGlyphAdvance + textComponent.kerning);
					continue;
				}

				auto glyph = fontGeometry.getGlyph(character);
				if (!glyph)
					glyph = fontGeometry.getGlyph('?');
				C78_CORE_ASSERT(glyph);

				double al, ab, ar, at;
				glyph->getQuadAtlasBounds(al, ab, ar, at);
				glm::vec2 texCoordMin(static_cast<float>(al), static_cast<float>(ab));
				glm::vec2 texCoordMax(static_cast<float>(ar), static_cast<float>(at));

				double pl, pb, pr, pt;
				glyph->getQuadPlaneBounds(pl, pb, pr, pt);
				glm::vec2 quadMin((float)pl, (float)pb);
				glm::vec2 quadMax((float)pr, (float)pt);

				quadMin *= fsScale, quadMax *= fsScale;
				quadMin += glm::vec2(x, y);
				quadMax += glm::vec2(x, y);

				float texelWidth = 1.0f / fontAtlas->getWidth();
				float texelHeight = 1.0f / fontAtlas->getHeight();
				texCoordMin *= glm::vec2(texelWidth, texelHeight);
				texCoordMax *= glm::vec2(texelWidth, texelHeight);

				const uint32_t indexOffset = static_cast<uint32_t>(vertecies.size());
				indecies.push_back(indexOffset + 0);
				indecies.push_back(indexOffset + 1);
				indecies.push_back(indexOffset + 2);
				indecies.push_back(indexOffset + 2);
				indecies.push_back(indexOffset + 3);
				indecies.push_back(indexOffset + 0);

				vertecies.push_back(glm::vec3(quadMin, 0.0f));
				vertecies.push_back(glm::vec3(quadMin.x, quadMax.y, 0.0f));
				vertecies.push_back(glm::vec3(quadMax, 0.0f));
				vertecies.push_back(glm::vec3(quadMax.x, quadMin.y, 0.0f));

				glm::vec4 color = textComponent.color;
				colorVertecies.push_back({ color });
				colorVertecies.push_back({ color });
				colorVertecies.push_back({ color });
				colorVertecies.push_back({ color });

				textureVertecies.push_back( texCoordMin );
				textureVertecies.push_back( { texCoordMin.x, texCoordMax.y } );
				textureVertecies.push_back( texCoordMax );
				textureVertecies.push_back( { texCoordMax.x, texCoordMin.y } );

				if (i < textComponent.textString.size() - 1) {
					double advance = glyph->getAdvance();
					char nextCharacter = textComponent.textString[i + 1];
					fontGeometry.getAdvance(advance, character, nextCharacter);

					x += fsScale * advance + textComponent.kerning;
				}
			}


			const size_t vertexCount = vertecies.size();
			C78_CORE_ASSERT(vertexCount % 4 == 0 && vertexCount == colorVertecies.size() && vertexCount == textureVertecies.size());

			{ //Vertex
				Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(&((*vertecies.data())[0]), vertexCount * sizeof(glm::vec3));
				vertexBuffer->setLayout({
						{ ShaderDataType::Float3, "a_Position" }
					}
				);
				pass.vertexBuffers.push_back(vertexBuffer);
			}

			glm::vec4 color = textComponent.color;
			{ //Vertex Color
				Ref<VertexBuffer> vertexColorBuffer = VertexBuffer::create(&colorVertecies[0].color.r, vertexCount * sizeof(C78E::Primitive::VertexColor));
				vertexColorBuffer->setLayout({
						{ ShaderDataType::Float4, "a_Color" }
					}
				);
				pass.vertexBuffers.push_back(vertexColorBuffer);
			}

			{ //Vertex Texture
				Ref<VertexBuffer> vertexTextureBuffer = VertexBuffer::create(&textureVertecies[0].x, vertexCount * sizeof(C78E::Primitive::VertexTexture));
				vertexTextureBuffer->setLayout({
						{ ShaderDataType::Float2, "a_TextureCorrdinate" }
					}
				);
				pass.vertexBuffers.push_back(vertexTextureBuffer);
			}


			{
				pass.indexBuffer = IndexBuffer::create(indecies.data(), indecies.size());
			}

			{
				pass.vertexArray = VertexArray::create();
				for (auto vbo : pass.vertexBuffers)
					pass.vertexArray->addVertexBuffer(vbo);
				pass.vertexArray->setIndexBuffer(pass.indexBuffer);
			}


		}
	}

	void Rasterizer3D::submitModelComponents(C78E::Ref<Scene> scene) {
		// submit ModelComponent
		for (auto& entt : scene->getAllEntitiesWith<ModelComponent>()) {
			Entity entity(entt, scene.get());
			ModelComponent& modelComponent = entity.getComponent<ModelComponent>();
			if (!modelComponent.model.isValid()) continue;
			Ref<Model> model = m_AssetManager->getAssetAs<Model>(modelComponent.model);

			m_CurrentScene->renderPasses.push_back(RenderPass());
			auto& pass = m_CurrentScene->renderPasses.back();

			pass.material = model->material();
			pass.material->m_Shader = EditorAssetManager::Default::Shader_ModelCompoent;

			// Default Texture
			pass.textureSlots.push_back(std::make_pair(EditorAssetManager::Default::Texture2D_White, m_AssetManager->getAssetAs<Texture2D>(EditorAssetManager::Default::Texture2D_White)));

			m_CurrentScene->addCamUBOToCurrentRenderPass(); // Camera UBO
			pass.uniformBuffers.push_back(getMaterialUBO(model->material(), 1)); // Entity UBO
			pass.uniformBuffers.push_back(getEntityUBO(entity, 2)); // Entity UBO



			{ //Vertex
				Buffer meshBuffer{ model->mesh()->getVertexByteSize() };
				meshBuffer.data = (uint8_t*)model->mesh()->getVertexData();

				Buffer buffer = Buffer::copy(meshBuffer);

				for (glm::vec3* vertex = buffer.as<glm::vec3>(); (size_t)vertex - (size_t)buffer.data < (size_t)buffer.size; vertex += 2) {
					vertex[0] += modelComponent.offset;
				}

				Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(buffer.as<float>(), buffer.size);
				vertexBuffer->setLayout({
						{ ShaderDataType::Float3, "a_Position" },
						{ ShaderDataType::Float3, "a_Normal" }
					});
				pass.vertexBuffers.push_back(vertexBuffer);
				buffer.release();
			}

			if (model->mesh()->hasVertexColor()) { //Vertex Color
				uint32_t colorVertSize = model->mesh()->getVertexColorByteSize();
				float* colorVertecies = (float*)model->mesh()->getVertexColorData();
				Ref<VertexBuffer> vertexColorBuffer = VertexBuffer::create(colorVertecies, colorVertSize);
				vertexColorBuffer->setLayout({
						{ ShaderDataType::Float4, "a_Color" }
					});
				pass.vertexBuffers.push_back(vertexColorBuffer);
			}


			{
				auto& textureSlots = pass.textureSlots;
				Material::MaterialTextures& matProps = model->material()->m_MaterialTextures;

				const std::vector<AssetHandle> textures = {
					matProps.ambient,
					matProps.diffuse,
					matProps.specular,
					matProps.specularHighlight,
					matProps.bump,
					matProps.displacement,
					matProps.alpha,
					matProps.reflection
				};

				for (AssetHandle textureHandle : textures) {
					if (textureSlots.size() >= RenderCommand::getMaxTextureSlots(RendererAPI::ShaderType::FRAGMENT)) break;
					if (!model->mesh()->hasVertexTexture() || !textureHandle.isValid()) {
						textureSlots.push_back(std::make_pair(EditorAssetManager::Default::Texture2D_White, m_AssetManager->getAssetAs<Texture2D>(EditorAssetManager::Default::Texture2D_White)));
					}
					else if (auto texture = m_AssetManager->getAssetAs<Texture2D>(textureHandle)) {
						textureSlots.push_back(std::make_pair(textureHandle, texture));
					}
					else {
						textureSlots.push_back(std::make_pair(EditorAssetManager::Default::Texture2D_White, m_AssetManager->getAssetAs<Texture2D>(EditorAssetManager::Default::Texture2D_White)));
					}
				}
				if (model->mesh()->hasVertexTexture()) { //Vertex Texture
					float* textureVertecies = (float*)model->mesh()->getVertexTextureData();
					uint32_t textureVertSize = model->mesh()->getVertexTextureByteSize();
					Ref<VertexBuffer> vertexTextureBuffer = VertexBuffer::create(textureVertecies, textureVertSize);
					vertexTextureBuffer->setLayout({
							{ ShaderDataType::Float2, "a_TextureCoordinate" },
							{ ShaderDataType::Int, "a_TextureSlot" }
						}
					);
					pass.vertexBuffers.push_back(vertexTextureBuffer);
				}
			}

			{
				uint32_t* indecies = model->mesh()->getIndexData();
				uint32_t indCount = model->mesh()->getIndexCount();
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

	void Rasterizer3D::submitSkyBoxComponents(C78E::Ref<Scene> scene) {
		for (auto& enttity : scene->getAllEntitiesWith<SkyBoxComponent>()) {
			Entity entity(enttity, scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			AssetHandle skyBoxTex = entity.getComponent<SkyBoxComponent>().skybox;
			if (!skyBoxTex.isValid()) continue;

			m_CurrentScene->renderPasses.push_back(RenderPass());
			RenderPass& pass = m_CurrentScene->renderPasses.back();
			pass.material = createRef<Material>();
			m_CurrentScene->addCamUBOToCurrentRenderPass(); // Camera UBO
			pass.material->m_Shader = EditorAssetManager::Default::Shader_SkyBoxComponent;

			// Default Texture
			pass.textureSlots.push_back(std::make_pair(EditorAssetManager::Default::Texture2D_White, m_AssetManager->getAssetAs<Texture2D>(EditorAssetManager::Default::Texture2D_White)));
			pass.textureSlots.push_back(std::make_pair(skyBoxTex, m_AssetManager->getAssetAs<CubeMap>(skyBoxTex)));

			Primitive::CubeMap cubeMap;

			Ref<VertexBuffer> vertexBuffer = VertexBuffer::create((float*)cubeMap.getVertexData(), cubeMap.getVertexSize());
			vertexBuffer->setLayout({ { ShaderDataType::Float3, "a_Position"} });
			pass.vertexBuffers.push_back(vertexBuffer);
			pass.indexBuffer = IndexBuffer::create((uint32_t*)cubeMap.getIndexData(), cubeMap.getIndexCount());

			pass.vertexArray = VertexArray::create();
			for (auto vbo : pass.vertexBuffers)
				pass.vertexArray->addVertexBuffer(vbo);
			pass.vertexArray->setIndexBuffer(pass.indexBuffer);

			pass.depthFunc = RendererAPI::DepthFunc::LEQUAL;
			pass.writeDepthBuffer = true;
		}
	}

	Rasterizer3D::Rasterizer3D(Ref<AssetManager> assetManager)
		: Renderer3D(assetManager){
	}

	Rasterizer3D::~Rasterizer3D() { }

	bool Rasterizer3D::beginScene(Camera& camera, const glm::mat4& viewMatrix) {
		m_CurrentScene = createRef<RenderScene>();
		m_CurrentScene->cameraUniform = CameraUniform{ camera.getProjection(), viewMatrix, glm::inverse(viewMatrix) };
		m_CurrentScene->renderPasses.clear();
		return false;
	}

	void Rasterizer3D::submit(Ref<Scene> scene) {
		if (!scene) return;

		submitSpriteComponents(scene);
		submitTextComponents(scene);
		submitModelComponents(scene);
		submitSkyBoxComponents(scene);

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
