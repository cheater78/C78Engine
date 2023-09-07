#include "C78ePCH.h"
#include "C78e/Renderer/Renderer3D.h"

#include "C78e/Core/Types.h"
#include "C78e/Renderer/Model.h"
#include "C78e/Renderer/Texture.h"
#include "C78e/Renderer/Shader.h"
#include "C78e/Renderer/vertexArray.h"
#include "C78e/Renderer/UniformBuffer.h"
#include "C78e/Renderer/RenderCommand.h"

#include "MSDFData.h"

namespace C78e {

	C78e::Renderer3D::Renderer3DData Renderer3D::s_Data;

	void Renderer3D::Init() {

		s_Data.whiteTexture = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.shader = Shader::Create("assets/shaders/Renderer3D_Generic.glsl");

		s_Data.TextureSlotIndex = 0;
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = s_Data.whiteTexture;
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = C78e::Texture2D::Create("assets/textures/bird.png");
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = C78e::Texture2D::Create("assets/textures/Top.png");
		

		s_Data.cameraUniformBuffer = UniformBuffer::Create(sizeof(CameraUniform), 0);
	}

	void Renderer3D::Shutdown() {
	}

	void Renderer3D::BeginScene(Entity camera)
	{
		ResetStats();
		s_Data.cameraUniform.projMat = camera.getComponent<CameraComponent>().Camera.GetProjection();
		s_Data.cameraUniform.invViewMat = camera.getComponent<TransformComponent>().GetTransform();
		s_Data.cameraUniform.viewMat = glm::inverse(s_Data.cameraUniform.invViewMat);
		s_Data.cameraUniformBuffer->SetData(&s_Data.cameraUniform, sizeof(CameraUniform));
	}

	void Renderer3D::submit(Ref<Scene> scene) {
		auto entitiesToRender = scene->getAllEntitiesWith<ModelComponent>();
		s_Data.shader->Bind();
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);
			
		for (auto& enttity : entitiesToRender) {
			Entity entity(enttity, scene.get());
			Model& model = *entity.getComponent<ModelComponent>().model.get();

			s_Data.Stats.Vertecies = model.getVertexData().size();
			s_Data.Stats.Indicies = model.getIndexData().size();

			s_Data.vertexArray = VertexArray::Create();

			s_Data.vertexBuffer = VertexBuffer::Create((float*)model.getVertexData().data(), model.getVertexData().size() * sizeof(Vertex));
			s_Data.vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float3, "a_Normal"       },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     },
				{ ShaderDataType::Int,    "a_EntityID"     }
				});
			s_Data.vertexArray->AddVertexBuffer(s_Data.vertexBuffer);

			auto indexBuffer = C78e::IndexBuffer::Create(model.getIndexData().data(), static_cast<uint32_t>(model.getIndexData().size()));
			indexBuffer->Bind();
			s_Data.vertexArray->SetIndexBuffer(indexBuffer);

			EntityUniform trans{ entity.getComponent<TransformComponent>().GetTransform() };

			s_Data.entityUniformBuffer = C78e::UniformBuffer::Create(sizeof(EntityUniform), 1);
			s_Data.entityUniformBuffer->SetData(&trans, sizeof(EntityUniform));

			s_Data.shader->Bind();
			s_Data.vertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data.vertexArray);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer3D::EndScene()
	{
		Flush();
	}

	void Renderer3D::StartBatch()
	{
		
	}

	void Renderer3D::Flush()
	{
				
	}

	void Renderer3D::NextBatch() {
		Flush();
		StartBatch();
	}

	void Renderer3D::ResetStats() { memset(&s_Data.Stats, 0, sizeof(Statistics)); }
	Renderer3D::Statistics Renderer3D::GetStats() { return s_Data.Stats; }
}
