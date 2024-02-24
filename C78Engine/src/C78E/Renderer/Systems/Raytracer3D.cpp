#include <C78ePCH.h>
#include "RayTracer3D.h"

#include <C78E/Scene/Entity.h>
#include <C78E/Renderer/RenderCommand.h>
#include <C78E/Renderer/UniformBuffer.h>

#include <C78E/Assets/AssetManager.h>

namespace C78E {
	uint32_t RayTracer3D::s_Width = 1;
	uint32_t RayTracer3D::s_Height = 1;
	Asset<Shader> RayTracer3D::s_RayComputeShader;
	Ref<Texture2D> RayTracer3D::s_Result = nullptr;

	void RayTracer3D::init(uint32_t width, uint32_t height) {
		s_RayComputeShader = C78E::AssetManager::getShaderAsset("RayCompute");

		onWindowResize(width, height);
	}

	void RayTracer3D::shutdown() { }

	void RayTracer3D::onWindowResize(uint32_t width, uint32_t height) {
		s_Width = width; s_Height = height;

		float* data = new float[s_Width * s_Height * 4];
		RawImage image{ s_Width, s_Height, ImageFormat::RGBA32F, (void*)data };
		delete[] data;
		s_Result = Texture2D::create(image);
	}
	
	void RayTracer3D::render(Ref<Scene> scene) {
		if (scene == nullptr)
			return;

		Ref<CameraUniform> camUni = createRef<CameraUniform>();
		camUni->projMat = scene->getPrimaryCamera().getComponent<CameraComponent>().Camera.GetProjection();
		camUni->invViewMat = scene->getPrimaryCamera().getTransform().getTransform();
		camUni->viewMat = glm::inverse(camUni->invViewMat);

		Ref<UniformBuffer> camUniformBuffer = UniformBuffer::create(sizeof(CameraUniform));
		camUniformBuffer->setData(camUni.get(), sizeof(CameraUniform));

		/* Scene to Storage Buffer */

		{
			s_Result->bindImage(0);
			camUniformBuffer->bind(1);
			s_RayComputeShader.get().BindCompute(static_cast<uint32_t>(std::ceil(s_Width / 8)), static_cast<uint32_t>(std::ceil(s_Height / 4)));
		}
	}

}