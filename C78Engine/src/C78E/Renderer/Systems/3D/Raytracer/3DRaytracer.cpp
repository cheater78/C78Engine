#include "C78EPCH.h"
#include "3DRaytracer.h"

#include <C78E/Renderer/Assets/Scene/Entity/Component/Uniform.h>

namespace C78E {

	Raytracer3D::Raytracer3D(Ref<AssetManager> assetManager)
		: Renderer3D(assetManager){

	}

	Raytracer3D::~Raytracer3D() {
	}

	

	struct Pass {
		Ref<C78E::UniformBuffer> camUBO = nullptr;
		Ref<C78E::UniformBuffer> entUBO = nullptr;

		Ref<VertexBuffer> vbo = nullptr;
		Ref<IndexBuffer> ibo = nullptr;
		Ref<VertexArray> va = nullptr;
	};

	std::vector<Pass> passes(10);
	uint32_t current = 0;

	bool Raytracer3D::render(Ref<Scene> scene) {
		if (!scene) return false;

		passes[current % 10] = Pass();

		SceneCamera camera = scene->getPrimaryCamera().getComponent<CameraComponent>().camera;
		TransformComponent cameraTransform = scene->getPrimaryCamera().getTransform();
		CameraUniform cameraUniform{ camera.getProjection(), glm::inverse(cameraTransform.getTransform()), cameraTransform.getTransform() };
		passes.at(current % 10).camUBO = C78E::UniformBuffer::create(sizeof(CameraUniform), 0);
		passes.at(current % 10).camUBO->setData(&cameraUniform, sizeof(CameraUniform));


		m_BusyWorking.acquire();
		m_WorkingFrame->bind();

		RenderCommand::setClearColor({0.3f, 0.05f, 0.4f, 1.0f});
		RenderCommand::clear();

		auto meshEnts = scene->getAllEntitiesWith<ModelComponent>();

		for (auto& entt : meshEnts) {
			Entity entity(entt, scene.get());

			AssetHandle modelHandle = entity.getComponent<ModelComponent>().model;
			for (auto& part : std::static_pointer_cast<Model>(m_AssetManager->getAsset(modelHandle))->m_Parts) {
				AssetHandle meshHandle = part.m_Mesh;
				AssetHandle materialHandle = part.m_Material;

				Ref<Mesh> mesh = std::static_pointer_cast<Mesh>(m_AssetManager->getAsset(meshHandle));
				Ref<Material> material = std::static_pointer_cast<Material>(m_AssetManager->getAsset(materialHandle));

				AssetHandle shaderHandle = material->m_Shader;

				uint32_t vertSize = mesh->getVertexByteSize();
				float* vertecies = (float*)mesh->getVertexData();

				uint32_t indCount = mesh->getIndexCount();
				uint32_t* indecies = mesh->getIndexData();


				passes.at(current % 10).vbo = VertexBuffer::create(vertecies, vertSize);
				passes.at(current % 10).vbo->setLayout({
					{ ShaderDataType::Float3, "position" },
					{ ShaderDataType::Float3, "normal" }
					});
				passes.at(current % 10).ibo = IndexBuffer::create(indecies, indCount);
				passes.at(current % 10).va = VertexArray::create();
				passes.at(current % 10).va->addVertexBuffer(passes.at(current % 10).vbo);
				passes.at(current % 10).va->setIndexBuffer(passes.at(current % 10).ibo);


				


				TransformComponent transform = entity.getComponent<TransformComponent>();
				EntityModelUniform entityUniform{ transform.getTransform(), transform.normalMat() };
				passes.at(current % 10).entUBO = C78E::UniformBuffer::create(sizeof(EntityModelUniform), 1);
				passes.at(current % 10).entUBO->setData(&entityUniform, sizeof(EntityModelUniform));



				Ref<Shader> shader = std::static_pointer_cast<Shader>(m_AssetManager->getAsset(shaderHandle));

				shader->bind();

				RenderCommand::drawIndexed(passes.at(current % 10).va, indCount);
				//RenderCommand::drawLines(passes.at(current % 10).va, vertSize);
				shader->unbind();

				current++;
				break;
			}

			break;
		}

		m_WorkingFrame->unbind();
		
		m_BusyTarget.acquire();
		m_TargetFrame.swap(m_WorkingFrame);

		m_BusyTarget.release();
		m_BusyWorking.release();


		uint32_t id = getTargetTexture()->getRendererID();

		return false;
	}

	bool Raytracer3D::display() {
		return false;
	}

	inline const Renderer::RendererType Raytracer3D::getType() const { return RendererType::Raytracer3D; }

}
