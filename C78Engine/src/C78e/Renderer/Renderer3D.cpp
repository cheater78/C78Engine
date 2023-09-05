#include "C78ePCH.h"
#include "C78e/Core/Types.h"
#include "C78e/Renderer/Renderer3D.h"

#include "C78e/Renderer/vertexArray.h"
#include "C78e/Renderer/Shader.h"
#include "C78e/Renderer/UniformBuffer.h"
#include "C78e/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MSDFData.h"

namespace C78e {

	struct Renderer3DData {
		static const uint32_t MaxVertices = 1048576;
		static const uint32_t MaxIndices = 16777216;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Renderer3D::Statistics Stats;

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> shader;
		Ref<Texture2D> whiteTexture;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		std::vector<Vertex> vertecies{};
		std::vector<uint32_t> indicies{};

		Ref<Scene> Scene;
		Ref<UniformBuffer> cameraUniformBuffer;
		CameraUniform cameraUniform;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init() {
		s_Data.vertexArray = VertexArray::Create();

		s_Data.vertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(Vertex));
		s_Data.vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float3, "a_Normal"       },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Int,    "a_EntityID"     }
		});
		s_Data.vertexArray->AddVertexBuffer(s_Data.vertexBuffer);

		s_Data.whiteTexture = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.shader = Shader::Create("assets/shaders/Renderer3D_Generic.glsl");

		// Set first texture slot to 0
		s_Data.TextureSlots[0] = s_Data.whiteTexture;

		s_Data.cameraUniformBuffer = UniformBuffer::Create(sizeof(CameraUniform), 0);
	}

	void Renderer3D::Shutdown() {
	}

	void Renderer3D::BeginScene(Entity camera)
	{
		s_Data.cameraUniform.projMat = camera.getComponent<CameraComponent>().Camera.GetProjection();
		s_Data.cameraUniform.invViewMat = camera.getComponent<TransformComponent>().GetTransform();
		s_Data.cameraUniform.viewMat = glm::inverse(s_Data.cameraUniform.invViewMat);
		s_Data.shader->Bind();
		s_Data.cameraUniformBuffer->SetData(&s_Data.cameraUniform, sizeof(CameraUniform));
	}

	void Renderer3D::submit(Ref<Scene> scene) {
		auto entitiesToRender = scene->getAllEntitiesWith<ModelComponent>();
		s_Data.whiteTexture->Bind(0);
		for (auto& enttity : entitiesToRender) {
			Entity entity(enttity, scene.get());
			Model& model = *entity.getComponent<ModelComponent>().model.get();


			
			s_Data.shader->Bind();
			s_Data.vertexBuffer->SetData(model.getVertexData().data(), model.getVertexData().size());
			auto indexBuffer = C78e::IndexBuffer::Create(model.getIndexData().data(), model.getIndexData().size());
			indexBuffer->Bind();
			s_Data.vertexArray->SetIndexBuffer(indexBuffer);

			EntityUniform trans{
				entity.getComponent<TransformComponent>().GetTransform()
			};

			C78e::Ref<C78e::UniformBuffer> entityUniformBuffer = C78e::UniformBuffer::Create(sizeof(EntityUniform), 1);
			entityUniformBuffer->SetData(&trans, sizeof(EntityUniform));

			s_Data.vertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data.vertexArray);
			s_Data.Stats.DrawCalls++;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(s_Data.indicies.data(), s_Data.indicies.size());
		s_Data.vertexArray->SetIndexBuffer(quadIB);
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

	void Renderer3D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats() {
		return s_Data.Stats;
	}

}
