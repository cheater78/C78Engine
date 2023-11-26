#include "C78EPCH.h"
#include "C78E/Renderer/Renderer3D.h"

#include "C78E/Core/Types.h"
#include "C78E/Renderer/Assets/Model/Mesh.h"
#include "C78E/Renderer/Assets/Texture/Texture.h"
#include "C78E/Renderer/Assets/Shader/Shader.h"
#include "C78E/Renderer/vertexArray.h"
#include "C78E/Renderer/UniformBuffer.h"
#include "C78E/Renderer/RendererAPI.h"
#include "C78E/Renderer/RenderCommand.h"
#include "C78E/Renderer/Assets/Texture/TextureManager.h"
#include "C78E/Renderer/GenericShape.h"
#include "Assets/Shader/ShaderLibrary.h"
#include <glad.h>
#include "Framebuffer.h"

namespace C78E {
	std::string Renderer3D::s_ActiveScene;
	std::unordered_map<std::string, Ref<C78E::Renderer3D::Renderer3DScene>> Renderer3D::s_Scenes;
	uint32_t Renderer3D::s_MaxTextureSlots = 0;
	Ref<Texture2D> Renderer3D::s_WhiteTexture;

	void Renderer3D::init() {
		s_MaxTextureSlots = std::min<uint32_t>(RenderCommand::getMaxTextureSlots(RendererAPI::ShaderStage::VERTEX), RenderCommand::getMaxTextureSlots(RendererAPI::ShaderStage::FRAGMENT));
		s_WhiteTexture = Texture2D::create(Texture2D::TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_WhiteTexture->setData(&whiteTextureData, sizeof(uint32_t));
	}

	void Renderer3D::shutdown() {
	}

	void Renderer3D::beginScene(std::string scene) {
		s_Scenes.emplace(scene, createRef<Renderer3DScene>());
		s_ActiveScene = scene;
		resetStats(scene);
		curr()->renderPasses.clear();
	}

	void Renderer3D::submit(Entity entity) {
		
	}

	void Renderer3D::submit(Ref<Scene>& scene) {
		curr()->scene = Scene::copy(scene);
	}

	Ref<UniformBuffer> Renderer3D::currentCameraUniformBuffer(){
		Ref<CameraUniform> camUni = createRef<CameraUniform>();
		camUni->projMat = curr()->scene->getPrimaryCamera().getComponent<CameraComponent>().Camera.GetProjection();
		camUni->invViewMat = curr()->scene->getPrimaryCamera().getTransform().getTransform();
		camUni->viewMat = glm::inverse(camUni->invViewMat);

		Ref<UniformBuffer> camUniformBuffer = UniformBuffer::create(sizeof(CameraUniform));
		camUniformBuffer->setData(camUni.get(), sizeof(CameraUniform));

		return camUniformBuffer;
	}

	Ref<UniformBuffer> Renderer3D::currentLightsUniformBuffer() {
		SceneLightUniform sceneLightUniform{};

		auto ambientLights = curr()->scene->getAllEntitiesWith<AmbientLightComponent>();
		C78_ASSERT(ambientLights.size() == 1, "Every Scene needs exactly one AmbientLight!");
		Entity ambientLightEntity(*ambientLights.begin(), curr()->scene.get());
		sceneLightUniform.ambientLight = ambientLightEntity.getComponent<AmbientLightComponent>();


		for (auto& enttity : curr()->scene->getAllEntitiesWith<DirectLightComponent>()) {
			Entity entity(enttity, curr()->scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& dLight = entity.getComponent<DirectLightComponent>();
			sceneLightUniform.directLights[sceneLightUniform.directLightCount++] = dLight;
		}

		for (auto& enttity : curr()->scene->getAllEntitiesWith<PointLightComponent>()) {
			Entity entity(enttity, curr()->scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& pLight = entity.getComponent<PointLightComponent>();
			pLight.position = entity.getComponent<TransformComponent>().Translation;
			sceneLightUniform.pointLights[sceneLightUniform.pointLightCount++] = pLight;
		}

		for (auto& enttity : curr()->scene->getAllEntitiesWith<SpotLightComponent>()) {
			Entity entity(enttity, curr()->scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			auto& sLight = entity.getComponent<SpotLightComponent>();
			sLight.position = entity.getComponent<TransformComponent>().Translation;
			sceneLightUniform.spotLights[sceneLightUniform.spotLightCount++] = sLight;
		}

		auto& lightsUniformBuffer = C78E::UniformBuffer::create(sizeof(SceneLightUniform));
		lightsUniformBuffer->setData(&sceneLightUniform, sizeof(SceneLightUniform));
		return lightsUniformBuffer;
	}


	void Renderer3D::submitSkyBox() {
		auto& skyBoxEntts = curr()->scene->getAllEntitiesWith<SkyBoxComponent>();

		if (!skyBoxEntts.size()) return;

		for (auto& skyBoxEntt : skyBoxEntts) {
			Entity skyBoxEntity(skyBoxEntts.front(), curr()->scene.get());
			if (!skyBoxEntity.getComponent<StateComponent>().enable) continue;
			auto& skyBoxTex = skyBoxEntity.getComponent<SkyBoxComponent>().textures.at(0);

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
				{ skyBoxTex },
				{ currentCameraUniformBuffer() },
				config
			};
			curr()->renderPasses.push_back(skyBoxPass);
			break;
		}
	}

	void Renderer3D::submitTexMeshes() {
		auto& meshEntts = curr()->scene->getAllEntitiesWith<MeshComponent, TextureComponent>();
		if (!meshEntts) return;

		for (auto& enttity : meshEntts) {
			Entity entity(enttity, curr()->scene.get());
			if (!entity.getComponent<StateComponent>().enable) continue;
			Mesh& mesh = *entity.getComponent<MeshComponent>().mesh.get();
			Material& material = entity.getComponent<MaterialComponent>();


			uint32_t passes = 1;
			Ref<Shader> shader = material.getShader();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)mesh.getVertexData().data(), static_cast<uint32_t>(mesh.getVertexData().size() * sizeof(Vertex)));
			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float3, "a_Normal"       },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     }
				});
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(mesh.getIndexData().data(), static_cast<uint32_t>(mesh.getIndexData().size()));
			Ref<VertexArray> vertexArray = VertexArray::Create();
			vertexArray->AddVertexBuffer(vertexBuffer);
			vertexArray->SetIndexBuffer(indexBuffer);

			std::vector<Ref<Texture>>& textures = entity.getComponent<TextureComponent>().textures;

			Ref<UniformBuffer> entityMaterialUniform = C78E::UniformBuffer::create(sizeof(EntityMaterialUniform));
			entityMaterialUniform->setData(&material.getUniform(), sizeof(EntityMaterialUniform));

			EntityUniform trans{ entity.getComponent<TransformComponent>().getTransform(),
									entity.getComponent<TransformComponent>().normalMat() };

			Ref<UniformBuffer> entityUniform = C78E::UniformBuffer::create(sizeof(EntityUniform));
			entityUniform->setData(&trans, sizeof(EntityUniform));

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
				{ currentCameraUniformBuffer(), currentLightsUniformBuffer(), entityMaterialUniform, entityUniform },
				config
			};

			curr()->stats.vertecies += static_cast<uint32_t>(mesh.getVertexData().size());
			curr()->stats.indicies += static_cast<uint32_t>(mesh.getIndexData().size());

			curr()->renderPasses.push_back(meshPass);
		}
	}

	void Renderer3D::submitPointLights() {
		auto& pointLightEntts = curr()->scene->getAllEntitiesWith<PointLightComponent>();
		if (!pointLightEntts.size()) return;

		uint32_t passes = 1;
		Ref<Shader> shader = C78E::ShaderLibrary::get()->get("assets/shaders/Renderer3D_PointLight.glsl");
		
		std::vector<uint32_t> indicies = {  };
		for (auto& pointLightEntt : pointLightEntts) {
			Entity pointLightEnt(pointLightEntt, curr()->scene.get());
			if (!pointLightEnt.getComponent<StateComponent>().enable) continue;
			uint32_t offset = indicies.size();
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
			{ currentCameraUniformBuffer(), currentLightsUniformBuffer() },
			config
		};
		curr()->stats.indicies += static_cast<uint32_t>(indicies.size());
		curr()->renderPasses.push_back(pointLightPass);
	}

	void Renderer3D::submitSpotLights() {
		auto& spotLightEntts = curr()->scene->getAllEntitiesWith<PointLightComponent>();
		if (!spotLightEntts.size()) return;

		uint32_t passes = 1;
		Ref<Shader> shader = C78E::ShaderLibrary::get()->get("assets/shaders/Renderer3D_SpotLight.glsl");

		std::vector<uint32_t> indicies = {  };
		for (auto& spotLightEntt : spotLightEntts) {
			Entity pointLightEnt(spotLightEntt, curr()->scene.get());
			if (!pointLightEnt.getComponent<StateComponent>().enable) continue;
			uint32_t offset = indicies.size();
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
			{ currentCameraUniformBuffer(), currentLightsUniformBuffer() },
			config
		};
		curr()->stats.indicies += static_cast<uint32_t>(indicies.size());
		curr()->renderPasses.push_back(spotLightPass);
	}


	void Renderer3D::endScene() {
		auto& r3Dscene = curr();

		r3Dscene->renderPasses.clear();

		if(r3Dscene->elements.skyBox)
			submitSkyBox();

		if(r3Dscene->elements.texMesh)
			submitTexMeshes();

		if(r3Dscene->elements.pointLightSprites)
			submitPointLights();
		if(r3Dscene->elements.spotLightSprites)
			submitSpotLights();
		
		/*
		//Shadow Map Render
		//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
		{
			auto pointLightsEntts = curr().scene->getAllEntitiesWith<PointLightComponent>();
			for (uint32_t i = 0; i < pointLightsEntts.size(); i++) {
				uint32_t res = 1024;
				FramebufferSpecification spec{
					res, res,
					{{{ FramebufferTextureFormat::DEPTH24STENCIL8 }}},
					1,
					false
				};
				auto fbo = Framebuffer::Create(spec);
				fbo->Bind();
				RenderCommand::setViewport(0, 0, res, res);
				RenderCommand::clear();


				// 1. first render to depth map
				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
				glClear(GL_DEPTH_BUFFER_BIT);
				ConfigureShaderAndMatrices();
				Rendercurr();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				// 2. then render scene as normal with shadow mapping (using depth map)
				glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				ConfigureShaderAndMatrices();
				glBindTexture(GL_TEXTURE_2D, depthMap);
				Rendercurr();
			}
		}
		*/
		s_ActiveScene = "";
	}

	void Renderer3D::render(std::string scene) {
		
		for (auto& pass : curr(scene)->renderPasses) {
			if (!pass.passes) continue;
			pass.shader->Bind();
			pass.vertexArray->Bind();

			C78_CORE_ASSERT(pass.textureSlots.size() <= s_MaxTextureSlots, "RenderPass cannot have this much Textures bound!");
			uint32_t texID = 0;
			for (auto& tex : pass.textureSlots)
				tex->bind(texID++);
			
			for (uint32_t i = 0; i < pass.uniformBuffers.size(); i++)
				pass.uniformBuffers.at(i)->bind(i);

			RenderCommand::setDepthWrite(pass.config.writeDepthBuffer);
			RenderCommand::setDepthFunc(pass.config.depthFunc);

			RenderCommand::drawIndexed(pass.vertexArray);
			curr(scene)->stats.drawCalls++;
			pass.passes--;
		}
		
	}

	void Renderer3D::resetStats() { memset(&curr()->stats, 0, sizeof(Statistics)); }
	void Renderer3D::resetStats(std::string scene) { memset(&curr(scene)->stats, 0, sizeof(Statistics)); }

	Renderer3D::Statistics Renderer3D::getStats() { return curr()->stats; }
	Renderer3D::Statistics Renderer3D::getStats(std::string scene) { return curr(scene)->stats; }
}
