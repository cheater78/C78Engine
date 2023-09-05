#include "C78App.h"

#include <C78e.h>

#include "TestLayer.h"


class C78App : public C78e::Application {

public:

	C78App() :m_AppLayer(C78e::createRef<TestLayer>(this->getWindow())) {
		pushLayer(m_AppLayer);
		
		
	}

	C78App(const C78App&) = delete;

	~C78App() {

	}

private:
	C78e::Ref<C78e::Layer> m_AppLayer;
};



// Register App in Engine
C78e::Application* C78e::createApplication() { return new C78App(); }
















/*
Scene& genTestScene(float ascpectRatio) {
	static C78e::Scene firstScene;

	C78e::Entity camEnt = firstScene.createEntity("Cam");
	C78e::Camera cam(camEnt.getEnttity(), &firstScene);
	firstScene.setActiveCam(cam);
	cam.setPerspectiveProj(
		70.f,
		ascpectRatio,
		.001f,
		1000.f
	);
	cam.setPosRot(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f, 0.f, 0.f));

	C78e::Entity ent1 = firstScene.createEntity("Vase");
	C78e::TransformComponent& ent1trans = ent1.getComponent<C78e::TransformComponent>();
	ent1trans.translation = glm::vec3(0.0f, 0.f, 0.f);
	ent1trans.scale = glm::vec3(2.f, 1.f, 2.f);
	C78e::MeshComponent& ent1mesh = ent1.addComponent<C78e::MeshComponent>("/res/models/smooth_vase.obj");

	C78e::Entity ent2 = firstScene.createEntity("Floor");
	C78e::TransformComponent& ent2trans = ent2.getComponent<C78e::TransformComponent>();
	ent2trans.translation = glm::vec3(0.0f, 0.f, 0.f);
	ent2trans.scale = glm::vec3(1.f, 1.f, 1.f);
	C78e::MeshComponent& ent2mesh = ent2.addComponent<C78e::MeshComponent>("/res/models/quad.obj");

	std::vector<glm::vec3> lightColors{
		{1.f, .1f, .1f},
		{ .1f, .1f, 1.f },
		{ .1f, 1.f, .1f },
		{ 1.f, 1.f, .1f },
		{ .1f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f }
	};

	for (int i = 0; i < lightColors.size(); i++) {
		C78e::Entity plight = firstScene.createEntity("PLight");
		C78e::PointLightComponent& plightc = plight.addComponent<C78e::PointLightComponent>(lightColors[i], 1.3f, 0.1f);
		C78e::TransformComponent& plightt = plight.getComponent<C78e::TransformComponent>();
		plightt.translation = glm::vec3(0.f, .0f, 2.f);
		plightt.scale = glm::vec3(.01f, .01f, .01f);

		auto rotateLight = glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(), { .0f, -1.f, .0f });
		plightt.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
		plightt.translation += glm::vec3(0.f, -1.f, 0.f);
	}
	return firstScene;
}
*/


