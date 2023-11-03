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

namespace C78E {

	C78E::Renderer3D::Renderer3DData Renderer3D::s_Data;

	void Renderer3D::init() {
		
		s_Data.whiteTexture = Texture2D::create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureSlotIndex = 0;
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = s_Data.whiteTexture;
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = C78E::TextureManager::get()->load("assets/textures/Top.png");
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = C78E::TextureManager::get()->load("assets/textures/Test000.png");
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = C78E::TextureManager::get()->load("assets/textures/WoodenCrate.jpg");
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = C78E::TextureManager::get()->load("assets/textures/GravelFloor.jpg");

		s_Data.skyBox = std::make_shared<RawImage>("assets/textures/SkyBox/MoonSpace.png", false);

		s_Data.skyBoxCubeMap = CubeMap::create(s_Data.skyBox);
		
	}

	void Renderer3D::shutdown() {
	}

	void Renderer3D::beginScene(Entity camera)
	{
		resetStats();

		s_Data.cameraUniformBuffer = UniformBuffer::Create(sizeof(CameraUniform), 0);
		s_Data.cameraUniform.projMat = camera.getComponent<CameraComponent>().Camera.GetProjection();
		s_Data.cameraUniform.invViewMat = camera.getComponent<TransformComponent>().getTransform();
		s_Data.cameraUniform.viewMat = glm::inverse(s_Data.cameraUniform.invViewMat);
		s_Data.cameraUniformBuffer->SetData(&s_Data.cameraUniform, sizeof(CameraUniform));
	}

	void Renderer3D::submit(Ref<Scene> scene) {
		

		//SkyBox
		{
			RenderCommand::setDepthFunc(RendererAPI::DepthFunc::LEQUAL);
			C78E::ShaderLibrary::get()->get("Renderer3D_SkyBox")->Bind();

			s_Data.skyBoxCubeMap->bind(0);

			auto vertecies = GenericShape::CubeMap::getVertexData();
			auto indicies = GenericShape::CubeMap::getIndexData();

			auto vbo = VertexBuffer::Create(vertecies.data(), vertecies.size() * sizeof(float));
			vbo->Bind();
			vbo->SetLayout({ { ShaderDataType::Float3, "a_Position"} });
			auto ibo = IndexBuffer::Create(indicies.data(), indicies.size() );
			ibo->Bind();
			auto vao = VertexArray::Create();
			vao->AddVertexBuffer(vbo);
			vao->SetIndexBuffer(ibo);
			vao->Bind();
			RenderCommand::drawIndexed(vao);
			RenderCommand::setDepthFunc(RendererAPI::LESS);
			C78E::ShaderLibrary::get()->get("Renderer3D_SkyBox")->Unbind();
		}

		//Light UBO at loc=1
		{
			SceneLightUniform sceneLightUniform{};

			auto ambientLights = scene->getAllEntitiesWith<AmbientLightComponent>();
			C78_ASSERT(ambientLights.size() == 1, "Every Scene needs exactly one AmbientLight!");

			Entity ambientLightEntity(*ambientLights.begin(), scene.get());
			sceneLightUniform.ambientLight = ambientLightEntity.getComponent<AmbientLightComponent>();


			auto directLights = scene->getAllEntitiesWith<DirectLightComponent>();

			for (auto& enttity : directLights) {
				Entity entity(enttity, scene.get());
				auto& dLight = entity.getComponent<DirectLightComponent>();
				sceneLightUniform.directLights[sceneLightUniform.directLightCount++] = dLight;
			}


			auto pointLights = scene->getAllEntitiesWith<PointLightComponent>();

			for (auto& enttity : pointLights) {
				Entity entity(enttity, scene.get());
				auto& pLight = entity.getComponent<PointLightComponent>();
				pLight.position = entity.getComponent<TransformComponent>().Translation;
				sceneLightUniform.pointLights[sceneLightUniform.pointLightCount++] = pLight;
			}

			auto spotLights = scene->getAllEntitiesWith<SpotLightComponent>();

			for (auto& enttity : spotLights) {
				Entity entity(enttity, scene.get());
				auto& sLight = entity.getComponent<SpotLightComponent>();
				sLight.position = entity.getComponent<TransformComponent>().Translation;
				sceneLightUniform.spotLights[sceneLightUniform.spotLightCount++] = sLight;
			}

			s_Data.sceneLightUniformBuffer = C78E::UniformBuffer::Create(sizeof(SceneLightUniform), 1);
			s_Data.sceneLightUniformBuffer->SetData(&sceneLightUniform, sizeof(SceneLightUniform));

		}

		//Scene meshes
		{

			auto entitiesToRender = scene->getAllEntitiesWith<MeshComponent, MaterialComponent>();

			for (auto& enttity : entitiesToRender) {
				Entity entity(enttity, scene.get());
				Mesh& mesh = *entity.getComponent<MeshComponent>().mesh.get();
				Material& material = entity.getComponent<MaterialComponent>();

				material.getShader().get()->Bind();
				for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
					s_Data.TextureSlots[i]->Bind(i);

				s_Data.Stats.Vertecies += static_cast<uint32_t>(mesh.getVertexData().size());
				s_Data.Stats.Indicies += static_cast<uint32_t>(mesh.getIndexData().size());

				s_Data.vertexArray = VertexArray::Create();

				s_Data.vertexBuffer = VertexBuffer::Create((float*)mesh.getVertexData().data(), static_cast<uint32_t>(mesh.getVertexData().size() * sizeof(Vertex)));
				s_Data.vertexBuffer->SetLayout({
					{ ShaderDataType::Float3, "a_Position"     },
					{ ShaderDataType::Float4, "a_Color"        },
					{ ShaderDataType::Float3, "a_Normal"       },
					{ ShaderDataType::Float2, "a_TexCoord"     },
					{ ShaderDataType::Float,  "a_TexIndex"     }
					});
				s_Data.vertexArray->AddVertexBuffer(s_Data.vertexBuffer);

				auto indexBuffer = C78E::IndexBuffer::Create(mesh.getIndexData().data(), static_cast<uint32_t>(mesh.getIndexData().size()));
				indexBuffer->Bind();
				s_Data.vertexArray->SetIndexBuffer(indexBuffer);


				s_Data.entityMaterialUniformBuffer = C78E::UniformBuffer::Create(sizeof(EntityMaterialUniform), 2);
				s_Data.entityMaterialUniformBuffer->SetData(&material.getUniform(), sizeof(EntityMaterialUniform));

				EntityUniform trans{ entity.getComponent<TransformComponent>().getTransform(),
										entity.getComponent<TransformComponent>().normalMat() };

				s_Data.entityUniformBuffer = C78E::UniformBuffer::Create(sizeof(EntityUniform), 3);
				s_Data.entityUniformBuffer->SetData(&trans, sizeof(EntityUniform));

				s_Data.vertexArray->Bind();
				RenderCommand::drawIndexed(s_Data.vertexArray);
				s_Data.Stats.DrawCalls++;
				material.getShader().get()->Unbind();
			}
		}

		//Translusent Objects
		RenderCommand::setDepthWrite(false);

		//PointLightSprites
		{	
			C78E::ShaderLibrary::get()->get("Renderer3D_PointLight")->Bind();

			std::vector<uint32_t> indicies = {  };

			auto pointLightsEntts = scene->getAllEntitiesWith<PointLightComponent>();
			for (uint32_t i = 0; i < 6 * pointLightsEntts.size(); i++)
				indicies.push_back(i);

			auto ibo = IndexBuffer::Create(indicies.data(), indicies.size());
			ibo->Bind();
			auto vao = VertexArray::Create();
			vao->SetIndexBuffer(ibo);
			vao->Bind();
			RenderCommand::drawIndexed(vao);
			s_Data.Stats.DrawCalls++;
			C78E::ShaderLibrary::get()->get("Renderer3D_PointLight")->Unbind();
		}

		//PointLightSprites
		{
			C78E::ShaderLibrary::get()->get("Renderer3D_SpotLight")->Bind();

			std::vector<uint32_t> indicies = {  };

			auto spotLightEntts = scene->getAllEntitiesWith<SpotLightComponent>();
			for (uint32_t i = 0; i < 6 * spotLightEntts.size(); i++)
				indicies.push_back(i);

			auto ibo = IndexBuffer::Create(indicies.data(), indicies.size());
			ibo->Bind();
			auto vao = VertexArray::Create();
			vao->SetIndexBuffer(ibo);
			vao->Bind();
			RenderCommand::drawIndexed(vao);
			s_Data.Stats.DrawCalls++;
			C78E::ShaderLibrary::get()->get("Renderer3D_SpotLight")->Unbind();
		}

		RenderCommand::setDepthWrite(true);

	}

	void Renderer3D::setShader(std::string shader) {
		s_Data.shader = Shader::create(shader);
	}

	void Renderer3D::endScene()
	{
		flush();
	}

	void Renderer3D::startBatch()
	{
		
	}

	void Renderer3D::flush()
	{
				
	}

	void Renderer3D::nextBatch() {
		flush();
		startBatch();
	}

	void Renderer3D::resetStats() { memset(&s_Data.Stats, 0, sizeof(Statistics)); }
	Renderer3D::Statistics Renderer3D::getStats() { return s_Data.Stats; }
}
