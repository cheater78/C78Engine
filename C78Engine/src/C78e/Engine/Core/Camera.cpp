#include "Camera.h"

#include "Scene.h"
#include "Entity.h"
#include "EntityComponents.h"

namespace C78e {
	
	Camera::Camera(entt::entity enttity, Scene* scene) : Entity(enttity, scene) {
		if (!hasComponent<CameraComponent>())
			throw std::runtime_error("Entity has no CameraComponent");
	}

	Camera::Camera(uint32_t enttID, Scene* scene) : Entity(enttID, scene) {
		if (!hasComponent<CameraComponent>())
			throw std::runtime_error("Entity has no CameraComponent");
	}

	Camera::~Camera() {
	}
	
	void Camera::setOrthographicProj(float left, float right, float top, float bottom, float near, float far) {
		auto& projMat = getProjMat();
		projMat = glm::mat4{ 1.0f };
		projMat[0][0] = 2.f / (right - left);
		projMat[1][1] = 2.f / (bottom - top);
		projMat[2][2] = 1.f / (far - near);
		projMat[3][0] = -(right + left) / (right - left);
		projMat[3][1] = -(bottom + top) / (bottom - top);
		projMat[3][2] = -near / (far - near);
	}

	void Camera::setPerspectiveProj(float fovy, float aspect, float near, float far) {
		assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.f);
		auto& projMat = getProjMat();
		projMat = glm::mat4{ 0.0f };
		projMat[0][0] = 1.f / (aspect * tanHalfFovy);
		projMat[1][1] = 1.f / (tanHalfFovy);
		projMat[2][2] = far / (far - near);
		projMat[2][3] = 1.f;
		projMat[3][2] = -(far * near) / (far - near);
	}

	void Camera::setPosDir(glm::vec3 position, glm::vec3 direction) {
		auto& trans = getComponent<TransformComponent>();
		glm::vec3 up = glm::vec3( 0.f, 1.f, 0.f );
		direction = glm::normalize(direction);
		trans.translation = position;
		trans.rotation = { glm::asin(glm::normalize(glm::cross(up, direction)).y), asin(direction.z), asin(direction.y)};
	}

	void Camera::setPosTarget(glm::vec3 position, glm::vec3 target) {
		if (target - position == glm::vec3{ .0f }) return;
		setPosDir(position, target - position);
	}

	void Camera::setPosRot(glm::vec3 position, glm::vec3 rotation) {
		auto& trans = getComponent<TransformComponent>();
		trans.translation = position;
		trans.rotation = rotation;
	}

	glm::mat4& Camera::getProjMat() { return getComponent<CameraComponent>().projMat; }

	glm::mat4 Camera::getViewMat() {
		auto& trans = getComponent<TransformComponent>();
		glm::vec3& position = trans.translation;
		glm::vec3& rotation = trans.rotation;

		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
		const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
		const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
		glm::mat4 viewMat{ 1.f };
		viewMat[0][0] = u.x;
		viewMat[1][0] = u.y;
		viewMat[2][0] = u.z;
		viewMat[0][1] = v.x;
		viewMat[1][1] = v.y;
		viewMat[2][1] = v.z;
		viewMat[0][2] = w.x;
		viewMat[1][2] = w.y;
		viewMat[2][2] = w.z;
		viewMat[3][0] = -glm::dot(u, position);
		viewMat[3][1] = -glm::dot(v, position);
		viewMat[3][2] = -glm::dot(w, position);

		return viewMat;
	}
}
