#include <C78ePCH.h>
#include "RayTracer3Dgpu.h"

#include <C78E/Scene/Entity.h>
#include <C78E/Renderer/RenderCommand.h>
#include <C78E/Renderer/UniformBuffer.h>

#include <C78E/Assets/AssetManager.h>

namespace C78E {

	float RayTracer3Dgpu::s_FOV = 45.f;

	uint32_t RayTracer3Dgpu::s_Width = 1;
	uint32_t RayTracer3Dgpu::s_Height = 1;

	Ref<Scene> RayTracer3Dgpu::s_Scene = nullptr;

	Asset<Shader> RayTracer3Dgpu::s_ShaderAsset;
	Ref<VertexBuffer> RayTracer3Dgpu::s_VertexBuffer = nullptr;
	Ref<IndexBuffer> RayTracer3Dgpu::s_IndexBuffer = nullptr;
	Ref<VertexArray> RayTracer3Dgpu::s_VertexArray = nullptr;
	Ref<Texture2D> RayTracer3Dgpu::s_Texture = nullptr;

	Asset<Shader> RayTracer3Dgpu::s_ComputeShaderAsset;

	void RayTracer3Dgpu::init() {

		{
			std::vector<float> vertecies = {
			-1.f, -1.f, 0.f, 0.f, 0.f,
			+1.f, -1.f, 0.f, 1.f, 0.f,
			+1.f, +1.f, 0.f, 1.f, 1.f,
			-1.f, +1.f, 0.f, 0.f, 1.f
			};

			std::vector<uint32_t> indecies = {
				0,1,2,2,3,0
			};
			s_ShaderAsset = C78E::AssetManager::getShaderAsset("DisplayTex");
			s_VertexBuffer = VertexBuffer::Create(vertecies.data(), static_cast<uint32_t>(sizeof(float) * vertecies.size()));
			s_VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" }
				});
			s_IndexBuffer = IndexBuffer::Create(indecies.data(), static_cast<uint32_t>(indecies.size()));
			s_VertexArray = VertexArray::Create();
			s_VertexArray->AddVertexBuffer(s_VertexBuffer);
			s_VertexArray->SetIndexBuffer(s_IndexBuffer);
		}

		{
			s_ComputeShaderAsset = C78E::AssetManager::getShaderAsset("RayCompute");
		}

	}

	void RayTracer3Dgpu::setViewport(uint32_t width, uint32_t height) {
		s_Width = width;
		s_Height = height;
	}

	void RayTracer3Dgpu::submit(Ref<Scene> scene) {
		s_Scene = scene;
	}

	void RayTracer3Dgpu::compute() {
		if (s_Scene == nullptr)
			return;

		float* data = new float[s_Width * s_Height * 4];
		RawImage image{ s_Width, s_Height, ImageFormat::RGBA32F, (void*)data };
		delete[] data;
		s_Texture = Texture2D::create(image);
		
		Ref<CameraUniform> camUni = createRef<CameraUniform>();
		camUni->projMat = s_Scene->getPrimaryCamera().getComponent<CameraComponent>().Camera.GetProjection();
		camUni->invViewMat = s_Scene->getPrimaryCamera().getTransform().getTransform();
		camUni->viewMat = glm::inverse(camUni->invViewMat);

		Ref<UniformBuffer> camUniformBuffer = UniformBuffer::create(sizeof(CameraUniform));
		camUniformBuffer->setData(camUni.get(), sizeof(CameraUniform));

		{
			s_Texture->bindImage(0);
			camUniformBuffer->bind(1);
			s_ComputeShaderAsset.get().BindCompute(static_cast<uint32_t>(std::ceil(s_Width / 8)), static_cast<uint32_t>(std::ceil(s_Height / 4)));
			
		}
	}

	void RayTracer3Dgpu::display() {
		RenderCommand::clear();
		
		s_ShaderAsset.get().Bind();
		s_Texture->bind(0);
		RenderCommand::drawIndexed(s_VertexArray);
	}

}