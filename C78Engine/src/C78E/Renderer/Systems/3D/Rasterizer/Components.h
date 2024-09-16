#pragma once
#include "C78EPCH.h"

#include "3DRasterizer.h"

namespace C78E {
	
	Ref<UniformBuffer> camUBO(Ref<Scene> scene) {
		Ref<CameraUniform> camUni = createRef<CameraUniform>();
		camUni->projMat = scene->getPrimaryCamera().getComponent<CameraComponent>().Camera.getProjection();
		camUni->invViewMat = scene->getPrimaryCamera().getTransform().getTransform();
		camUni->viewMat = glm::inverse(camUni->invViewMat);

		Ref<UniformBuffer> camUniformBuffer = UniformBuffer::create(sizeof(CameraUniform));
		camUniformBuffer->setData(camUni.get(), sizeof(CameraUniform));

		return camUniformBuffer;
	}

	Ref<UniformBuffer> lightUBO(Ref<Scene> scene) {
		SceneLightUniform sceneLightUniform{};

		auto ambientLights = scene->getAllEntitiesWith<AmbientLightComponent>();
		C78_ASSERT(ambientLights.size() == 1, "Every Scene needs exactly one AmbientLight!");
		Entity ambientLightEntity(*ambientLights.begin(), scene.get());
		sceneLightUniform.ambientLight = ambientLightEntity.getComponent<AmbientLightComponent>();


		for (auto& enttity : scene->getAllEntitiesWith<DirectLightComponent>()) {
			Entity entity(enttity, scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& dLight = entity.getComponent<DirectLightComponent>();
			sceneLightUniform.directLights[sceneLightUniform.directLightCount++] = dLight;
		}

		for (auto& enttity : scene->getAllEntitiesWith<PointLightComponent>()) {
			Entity entity(enttity, scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& pLight = entity.getComponent<PointLightComponent>();
			pLight.position = entity.getComponent<TransformComponent>().translation;
			sceneLightUniform.pointLights[sceneLightUniform.pointLightCount++] = pLight;
		}

		for (auto& enttity : scene->getAllEntitiesWith<SpotLightComponent>()) {
			Entity entity(enttity, scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& sLight = entity.getComponent<SpotLightComponent>();
			sLight.position = entity.getComponent<TransformComponent>().translation;
			sceneLightUniform.spotLights[sceneLightUniform.spotLightCount++] = sLight;
		}

		auto lightsUniformBuffer = C78E::UniformBuffer::create(sizeof(SceneLightUniform));
		lightsUniformBuffer->setData(&sceneLightUniform, sizeof(SceneLightUniform));
		return lightsUniformBuffer;
	}


	void skyBoxPass(Ref<Scene> scene) {
		auto skyBoxEntts = scene->getAllEntitiesWith<SkyBoxComponent>();

		if (!skyBoxEntts.size()) return;

		for (auto& skyBoxEntt : skyBoxEntts) {
			Entity skyBoxEntity(skyBoxEntts.front(), scene.get());
			if (!skyBoxEntity.getComponent<StateComponent>().enable) continue;
			auto& skyBoxTex = skyBoxEntity.getComponent<SkyBoxComponent>().skyboxes.at(0);

			auto vertecies = GenericShape::CubeMap::getVertexData();
			auto indicies = GenericShape::CubeMap::getIndexData();

			uint32_t passes = 1;
			Ref<Shader> shader = skyBoxEntity.getComponent<MaterialComponent>().getShader();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(vertecies.data(), static_cast<uint32_t>(vertecies.size() * sizeof(float)));
			vertexBuffer->setLayout({ { ShaderDataType::Float3, "a_Position"} });
			Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indicies.data(), static_cast<uint32_t>(indicies.size()));
			Ref<VertexArray> vertexArray = VertexArray::create();
			vertexArray->addVertexBuffer(vertexBuffer);
			vertexArray->setIndexBuffer(indexBuffer);

			Renderer3DPassConfig config{
				true,
				RendererAPI::LEQUAL
			};

			Renderer3DPass skyBoxPass{
				passes,
				shader,
				vertexBuffer,
				indexBuffer,
				vertexArray,
				{ (Ref<CubeMap>)skyBoxTex },
				{ currentCameraUniformBuffer() },
				config
			};
			renderPasses.push_back(skyBoxPass);
			break;
		}
	}

	void submitTexMeshes() {
		auto meshEntts = scene->getAllEntitiesWith<MeshComponent, TextureComponent, MaterialComponent>();
		if (!meshEntts) return;

		for (auto& enttity : meshEntts) {
			Entity entity(enttity, scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			Mesh& mesh = entity.getComponent<MeshComponent>().mesh.get();
			Material& material = entity.getComponent<MaterialComponent>();


			uint32_t passes = 1;
			Ref<Shader> shader = material.getShader();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::create((float*)mesh.m_Vertecies.data(), static_cast<uint32_t>(mesh.m_Vertecies.size() * sizeof(Vertex)));
			vertexBuffer->setLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float3, "a_Normal"       },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     }
				});
			Ref<IndexBuffer> indexBuffer = IndexBuffer::create(mesh.m_Indicies.data(), static_cast<uint32_t>(mesh.m_Indicies.size()));
			Ref<VertexArray> vertexArray = VertexArray::create();
			vertexArray->addVertexBuffer(vertexBuffer);
			vertexArray->setIndexBuffer(indexBuffer);

			std::vector<Ref<Texture>> textures;
			for (Asset<Texture2D> asset : entity.getComponent<TextureComponent>().textures) {
				textures.push_back((Ref<Texture2D>)asset);
			}

			//MAterial -------------------------------------------------------------------------------------------------------------------------------

			EntityModelUniform trans{ entity.getComponent<TransformComponent>().getTransform(),
									entity.getComponent<TransformComponent>().normalMat() };

			Ref<UniformBuffer> entityUniform = C78E::UniformBuffer::create(sizeof(EntityModelUniform));
			entityUniform->setData(&trans, sizeof(EntityModelUniform));

			Renderer3DPassConfig config{
				true,
				RendererAPI::LESS
			};

			Renderer3DPass meshPass{
				passes,
				shader,
				vertexBuffer,
				indexBuffer,
				vertexArray,
				textures,
				{ currentCameraUniformBuffer(), currentLightsUniformBuffer(), C78E::UniformBuffer::create(1), entityUniform},
				config
			};

			stats.vertecies += static_cast<uint32_t>(mesh.m_Vertecies.size());
			stats.indicies += static_cast<uint32_t>(mesh.m_Indicies.size());

			renderPasses.push_back(meshPass);
		}
	}

	void submitModels() {
		auto modelEntts = scene->getAllEntitiesWith<C78E::ModelComponent>();
		if (!modelEntts.size()) return;

		for (auto& enttity : modelEntts) {
			Entity entity(enttity, scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;

			std::vector<Asset<Model>> models = entity.getComponent<ModelComponent>().models;
			for (Asset<Model> model : models) {
				uint32_t passes = 1;

				Asset<Shader> shader = model.get().m_Material.get().getShader();
				Asset<Material> material = model.get().m_Material;
				Material::MaterialTextures materialTextures = model.get().m_Material.get().getTextures();
				Asset<Mesh> mesh = model.get().m_Mesh;

				Ref<VertexBuffer> vertexBuffer = VertexBuffer::create((float*)mesh.get().m_Vertecies.data(), static_cast<uint32_t>(mesh.get().m_Vertecies.size() * sizeof(Vertex)));
				vertexBuffer->setLayout({
					{ ShaderDataType::Float3, "a_Position"     },
					{ ShaderDataType::Float4, "a_Color"        },
					{ ShaderDataType::Float3, "a_Normal"       },
					{ ShaderDataType::Float2, "a_TexCoord"     },
					{ ShaderDataType::Float,  "a_TexIndex"     }
					});
				Ref<IndexBuffer> indexBuffer = IndexBuffer::create(mesh.get().m_Indicies.data(), static_cast<uint32_t>(mesh.get().m_Indicies.size()));
				Ref<VertexArray> vertexArray = VertexArray::create();
				vertexArray->addVertexBuffer(vertexBuffer);
				vertexArray->setIndexBuffer(indexBuffer);


				//TODO ORDER!---------------------------------------------------------------------------------------------------
				// else dummy
				std::vector<Ref<Texture>> textures;
				if (materialTextures.ambient != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.ambient));
				if (materialTextures.diffuse != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.diffuse));
				if (materialTextures.specular != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.specular));
				if (materialTextures.specularHighlight != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.specularHighlight));
				if (materialTextures.displacement != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.displacement));
				if (materialTextures.reflection != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.reflection));
				if (materialTextures.alpha != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.alpha));
				if (materialTextures.bump != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.bump));


				Ref<UniformBuffer> materialUniform = C78E::UniformBuffer::create(sizeof(Material::MaterialProperties));
				materialUniform->setData(&material.get().getProperties(), sizeof(Material::MaterialProperties));

				EntityModelUniform trans{ entity.getComponent<TransformComponent>().getTransform(),
										entity.getComponent<TransformComponent>().normalMat() };

				Ref<UniformBuffer> entityUniform = C78E::UniformBuffer::create(sizeof(EntityModelUniform));
				entityUniform->setData(&trans, sizeof(EntityModelUniform));

				Renderer3DPassConfig config{
					true,
					RendererAPI::LESS
				};

				Renderer3DPass meshPass{
					passes,
					shader,
					vertexBuffer,
					indexBuffer,
					vertexArray,
					textures,
					{ currentCameraUniformBuffer(), currentLightsUniformBuffer(), materialUniform, entityUniform},
					config
				};

				stats.vertecies += static_cast<uint32_t>(mesh.get().m_Vertecies.size());
				stats.indicies += static_cast<uint32_t>(mesh.get().m_Indicies.size());

				renderPasses.push_back(meshPass);
			}

		}
	}

	void submitPointLights() {
		auto pointLightEntts = scene->getAllEntitiesWith<PointLightComponent>();
		if (!pointLightEntts.size()) return;

		uint32_t passes = 1;
		Ref<Shader> shader = C78E::AssetManager::getShader("PointLight");

		std::vector<uint32_t> indicies = {  };
		for (auto& pointLightEntt : pointLightEntts) {
			Entity pointLightEnt(pointLightEntt, scene.get());
			if (!pointLightEnt.getComponent<StateComponent>().enable) continue;
			uint32_t offset = static_cast<uint32_t>(indicies.size());
			for (uint32_t i = 0; i < 6; i++)
				indicies.push_back(offset + i);
		}
		if (!indicies.size()) return;

		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indicies.data(), static_cast<uint32_t>(indicies.size()));
		Ref<VertexArray> vertexArray = VertexArray::create();
		vertexArray->setIndexBuffer(indexBuffer);

		Renderer3DPassConfig config{
			true,
			RendererAPI::LESS
		};

		Renderer3DPass pointLightPass{
			passes,
			shader,
			nullptr,
			indexBuffer,
			vertexArray,
			{},
			{ currentCameraUniformBuffer(), currentLightsUniformBuffer() },
			config
		};
		stats.indicies += static_cast<uint32_t>(indicies.size());
		renderPasses.push_back(pointLightPass);
	}

	void submitSpotLights() {
		auto spotLightEntts = scene->getAllEntitiesWith<PointLightComponent>();
		if (!spotLightEntts.size()) return;

		uint32_t passes = 1;
		Ref<Shader> shader = C78E::AssetManager::getShader("SpotLight");

		std::vector<uint32_t> indicies = {  };
		for (auto& spotLightEntt : spotLightEntts) {
			Entity pointLightEnt(spotLightEntt, scene.get());
			if (!pointLightEnt.getComponent<StateComponent>().enable) continue;
			uint32_t offset = static_cast<uint32_t>(indicies.size());
			for (uint32_t i = 0; i < 6; i++)
				indicies.push_back(offset + i);
		}
		if (!indicies.size()) return;

		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indicies.data(), static_cast<uint32_t>(indicies.size()));
		Ref<VertexArray> vertexArray = VertexArray::create();
		vertexArray->setIndexBuffer(indexBuffer);

		Renderer3DPassConfig config{
			true,
			RendererAPI::LESS
		};

		Renderer3DPass spotLightPass{
			passes,
			shader,
			nullptr,
			indexBuffer,
			vertexArray,
			{},
			{ currentCameraUniformBuffer(), currentLightsUniformBuffer() },
			config
		};
		stats.indicies += static_cast<uint32_t>(indicies.size());
		renderPasses.push_back(spotLightPass);
	}


}
