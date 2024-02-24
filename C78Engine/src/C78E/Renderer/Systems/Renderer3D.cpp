#include "C78EPCH.h"
#include "Renderer3D.h"

#include "C78E/Core/Types.h"
#include <C78E/Scene/Entity.h>
#include "C78E/Assets/AssetManager.h"

#include "C78E/Renderer/vertexArray.h"
#include "C78E/Renderer/UniformBuffer.h"
#include "C78E/Renderer/RendererAPI.h"
#include "C78E/Renderer/RenderCommand.h"
#include "C78E/Renderer/Framebuffer.h"
#include "C78E/Renderer/GenericShape.h"

namespace C78E {
	uint32_t Renderer3D::s_MaxTextureSlots = 0;
	uint32_t Renderer3D::s_Width = 1, Renderer3D::s_Height = 1;
	Ref<Texture2D> Renderer3D::s_Result = nullptr;
	Ref<Framebuffer> Renderer3D::s_FrameBuffer = nullptr;

	void Renderer3D::init(uint32_t width, uint32_t height) {
		s_MaxTextureSlots = std::min<uint32_t>(RenderCommand::getMaxTextureSlots(RendererAPI::ShaderStage::VERTEX), RenderCommand::getMaxTextureSlots(RendererAPI::ShaderStage::FRAGMENT));
		
		FramebufferSpecification spec{
			width, height,
			{{{ FramebufferTextureFormat::DEPTH24STENCIL8 } ,{ FramebufferTextureFormat::RGBA8 }}},
			1,
			false
		};
		s_FrameBuffer = Framebuffer::Create(spec);
		s_FrameBuffer->Unbind();

		onWindowResize(width, height);
	}

	void Renderer3D::shutdown() {
	}

	void Renderer3D::onWindowResize(uint32_t width, uint32_t height) {
		s_Width = width; s_Height = height;

		float* data = new float[s_Width * s_Height * 4];
		RawImage image{ s_Width, s_Height, ImageFormat::RGBA32F, (void*)data };
		delete[] data;
		s_Result = Texture2D::create(image);
		s_FrameBuffer->Resize(width, height);
	}

	Ref<UniformBuffer> Renderer3D::currentCameraUniformBuffer(Renderer3DScene& rScene){
		Ref<CameraUniform> camUni = createRef<CameraUniform>();
		camUni->projMat = rScene.scene->getPrimaryCamera().getComponent<CameraComponent>().Camera.GetProjection();
		camUni->invViewMat = rScene.scene->getPrimaryCamera().getTransform().getTransform();
		camUni->viewMat = glm::inverse(camUni->invViewMat);

		Ref<UniformBuffer> camUniformBuffer = UniformBuffer::create(sizeof(CameraUniform));
		camUniformBuffer->setData(camUni.get(), sizeof(CameraUniform));

		return camUniformBuffer;
	}

	Ref<UniformBuffer> Renderer3D::currentLightsUniformBuffer(Renderer3DScene& rScene) {
		SceneLightUniform sceneLightUniform{};

		auto ambientLights = rScene.scene->getAllEntitiesWith<AmbientLightComponent>();
		C78_ASSERT(ambientLights.size() == 1, "Every Scene needs exactly one AmbientLight!");
		Entity ambientLightEntity(*ambientLights.begin(), rScene.scene.get());
		sceneLightUniform.ambientLight = ambientLightEntity.getComponent<AmbientLightComponent>();


		for (auto& enttity : rScene.scene->getAllEntitiesWith<DirectLightComponent>()) {
			Entity entity(enttity, rScene.scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& dLight = entity.getComponent<DirectLightComponent>();
			sceneLightUniform.directLights[sceneLightUniform.directLightCount++] = dLight;
		}

		for (auto& enttity : rScene.scene->getAllEntitiesWith<PointLightComponent>()) {
			Entity entity(enttity, rScene.scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& pLight = entity.getComponent<PointLightComponent>();
			pLight.position = entity.getComponent<TransformComponent>().Translation;
			sceneLightUniform.pointLights[sceneLightUniform.pointLightCount++] = pLight;
		}

		for (auto& enttity : rScene.scene->getAllEntitiesWith<SpotLightComponent>()) {
			Entity entity(enttity, rScene.scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& sLight = entity.getComponent<SpotLightComponent>();
			sLight.position = entity.getComponent<TransformComponent>().Translation;
			sceneLightUniform.spotLights[sceneLightUniform.spotLightCount++] = sLight;
		}

		auto lightsUniformBuffer = C78E::UniformBuffer::create(sizeof(SceneLightUniform));
		lightsUniformBuffer->setData(&sceneLightUniform, sizeof(SceneLightUniform));
		return lightsUniformBuffer;
	}


	void Renderer3D::submitSkyBox(Renderer3DScene& rScene) {
		auto skyBoxEntts = rScene.scene->getAllEntitiesWith<SkyBoxComponent>();

		if (!skyBoxEntts.size()) return;

		for (auto& skyBoxEntt : skyBoxEntts) {
			Entity skyBoxEntity(skyBoxEntts.front(), rScene.scene.get());
			if (!skyBoxEntity.getComponent<StateComponent>().enable) continue;
			auto& skyBoxTex = skyBoxEntity.getComponent<SkyBoxComponent>().skyboxes.at(0);

			auto vertecies = GenericShape::CubeMap::getVertexData();
			auto indicies = GenericShape::CubeMap::getIndexData();

			uint32_t passes = 1;
			Ref<Shader> shader = skyBoxEntity.getComponent<MaterialComponent>().getShader();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertecies.data(), static_cast<uint32_t>(vertecies.size() * sizeof(float)));
			vertexBuffer->SetLayout({ { ShaderDataType::Float3, "a_Position"} });
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indicies.data(), static_cast<uint32_t>(indicies.size()));
			Ref<VertexArray> vertexArray = VertexArray::Create();
			vertexArray->AddVertexBuffer(vertexBuffer);
			vertexArray->SetIndexBuffer(indexBuffer);

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
				{ currentCameraUniformBuffer(rScene) },
				config
			};
			rScene.renderPasses.push_back(skyBoxPass);
			break;
		}
	}

	void Renderer3D::submitTexMeshes(Renderer3DScene& rScene) {
		auto meshEntts = rScene.scene->getAllEntitiesWith<MeshComponent, TextureComponent, MaterialComponent>();
		if (!meshEntts) return;

		for (auto& enttity : meshEntts) {
			Entity entity(enttity, rScene.scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			Mesh& mesh = entity.getComponent<MeshComponent>().mesh.get();
			Material& material = entity.getComponent<MaterialComponent>();


			uint32_t passes = 1;
			Ref<Shader> shader = material.getShader();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)mesh.m_Vertecies.data(), static_cast<uint32_t>(mesh.m_Vertecies.size() * sizeof(Vertex)));
			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float3, "a_Normal"       },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     }
				});
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(mesh.m_Indicies.data(), static_cast<uint32_t>(mesh.m_Indicies.size()));
			Ref<VertexArray> vertexArray = VertexArray::Create();
			vertexArray->AddVertexBuffer(vertexBuffer);
			vertexArray->SetIndexBuffer(indexBuffer);

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
				{ currentCameraUniformBuffer(rScene), currentLightsUniformBuffer(rScene), C78E::UniformBuffer::create(1), entityUniform},
				config
			};

			rScene.renderPasses.push_back(meshPass);
		}
	}

	void Renderer3D::submitModels(Renderer3DScene& rScene) {
		auto modelEntts = rScene.scene->getAllEntitiesWith<C78E::ModelComponent>();
		if (!modelEntts.size()) return;

		for (auto& enttity : modelEntts) {
			Entity entity(enttity, rScene.scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			
			std::vector<Asset<Model>> models = entity.getComponent<ModelComponent>().models;
			for (Asset<Model> model : models) {
				uint32_t passes = 1;

				Asset<Shader> shader = model.get().m_Material.get().getShader();
				Asset<Material> material = model.get().m_Material;
				Material::MaterialTextures materialTextures = model.get().m_Material.get().getTextures();
				Asset<Mesh> mesh = model.get().m_Mesh;

				Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)mesh.get().m_Vertecies.data(), static_cast<uint32_t>(mesh.get().m_Vertecies.size() * sizeof(Vertex)));
				vertexBuffer->SetLayout({
					{ ShaderDataType::Float3, "a_Position"     },
					{ ShaderDataType::Float4, "a_Color"        },
					{ ShaderDataType::Float3, "a_Normal"       },
					{ ShaderDataType::Float2, "a_TexCoord"     },
					{ ShaderDataType::Float,  "a_TexIndex"     }
					});
				Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(mesh.get().m_Indicies.data(), static_cast<uint32_t>(mesh.get().m_Indicies.size()));
				Ref<VertexArray> vertexArray = VertexArray::Create();
				vertexArray->AddVertexBuffer(vertexBuffer);
				vertexArray->SetIndexBuffer(indexBuffer);
				
				std::vector<Ref<Texture>> textures;
				if (materialTextures.ambient != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.ambient));
				else textures.push_back(nullptr);
				if (materialTextures.diffuse != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.diffuse));
				else textures.push_back(nullptr);
				if (materialTextures.specular != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.specular));
				else textures.push_back(nullptr);
				if (materialTextures.specularHighlight != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.specularHighlight));
				else textures.push_back(nullptr);
				if (materialTextures.bump != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.bump));
				else textures.push_back(nullptr);
				if (materialTextures.displacement != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.displacement));
				else textures.push_back(nullptr);
				if (materialTextures.alpha != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.alpha));
				else textures.push_back(nullptr);
				if (materialTextures.reflection != "")
					textures.push_back(AssetManager::getTexture2D(materialTextures.reflection));
				else textures.push_back(nullptr);
				
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
					{ currentCameraUniformBuffer(rScene), currentLightsUniformBuffer(rScene), materialUniform, entityUniform},
					config
				};

				rScene.renderPasses.push_back(meshPass);
			}

		}
	}

	void Renderer3D::submitPointLights(Renderer3DScene& rScene) {
		auto pointLightEntts = rScene.scene->getAllEntitiesWith<PointLightComponent>();
		if (!pointLightEntts.size()) return;

		uint32_t passes = 1;
		Ref<Shader> shader = C78E::AssetManager::getShader("PointLight");
		
		std::vector<uint32_t> indicies = {  };
		for (auto& pointLightEntt : pointLightEntts) {
			Entity pointLightEnt(pointLightEntt, rScene.scene.get());
			if (!pointLightEnt.getComponent<StateComponent>().enable) continue;
			uint32_t offset = static_cast<uint32_t>(indicies.size());
			for (uint32_t i = 0; i < 6; i++)
				indicies.push_back(offset + i);
		}
		if (!indicies.size()) return;
		
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indicies.data(), static_cast<uint32_t>(indicies.size()));
		Ref<VertexArray> vertexArray = VertexArray::Create();
		vertexArray->SetIndexBuffer(indexBuffer);
		
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
			{ currentCameraUniformBuffer(rScene), currentLightsUniformBuffer(rScene) },
			config
		};
		rScene.renderPasses.push_back(pointLightPass);
	}

	void Renderer3D::submitSpotLights(Renderer3DScene& rScene) {
		auto spotLightEntts = rScene.scene->getAllEntitiesWith<PointLightComponent>();
		if (!spotLightEntts.size()) return;

		uint32_t passes = 1;
		Ref<Shader> shader = C78E::AssetManager::getShader("SpotLight");

		std::vector<uint32_t> indicies = {  };
		for (auto& spotLightEntt : spotLightEntts) {
			Entity pointLightEnt(spotLightEntt, rScene.scene.get());
			if (!pointLightEnt.getComponent<StateComponent>().enable) continue;
			uint32_t offset = static_cast<uint32_t>(indicies.size());
			for (uint32_t i = 0; i < 6; i++)
				indicies.push_back(offset + i);
		}
		if (!indicies.size()) return;

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indicies.data(), static_cast<uint32_t>(indicies.size()));
		Ref<VertexArray> vertexArray = VertexArray::Create();
		vertexArray->SetIndexBuffer(indexBuffer);

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
			{ currentCameraUniformBuffer(rScene), currentLightsUniformBuffer(rScene) },
			config
		};
		rScene.renderPasses.push_back(spotLightPass);
	}

	void Renderer3D::render(Ref<Scene> scene) {

		Renderer3DScene rScene = { scene, {} };

		submitSkyBox(rScene);
		submitModels(rScene);
		submitPointLights(rScene);
		submitSpotLights(rScene);

		s_FrameBuffer->Bind();
		RenderCommand::clear();

		for (Renderer3DPass pass : rScene.renderPasses) {
			if (!pass.passes) continue;
			pass.shader->Bind();
			pass.vertexArray->Bind();

			C78_CORE_ASSERT(pass.textureSlots.size() <= s_MaxTextureSlots, "RenderPass cannot have this much Textures bound!");
			uint32_t texID = 0;
			for (auto& tex : pass.textureSlots)
				if(tex != nullptr)
					tex->bind(texID++);
			
			for (uint32_t i = 0; i < pass.uniformBuffers.size(); i++)
				pass.uniformBuffers.at(i)->bind(i);

			RenderCommand::setDepthWrite(pass.config.writeDepthBuffer);
			RenderCommand::setDepthFunc(pass.config.depthFunc);

			RenderCommand::drawIndexed(pass.vertexArray);
			pass.passes--;
		}
		s_Result = s_FrameBuffer->getColorAttachment(0);
		s_FrameBuffer->Unbind();
	}

}
