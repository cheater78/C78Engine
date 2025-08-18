#include "C78EPCH.h"
#include "Particle.h"

namespace C78E {
	/*
	void Particle::onUpdate(C78E::Timestep delta) {
		if (!isAlive()) return;
		// limit time step to 0 lifetime
		const float deltaSeconds = delta.getSeconds();
		float ltStep = deltaSeconds;
		if (lifeTime - deltaSeconds < 0.f) {
			ltStep = lifeTime;
			lifeTime = 0.f;
		} else {
			lifeTime -= ltStep;
		}
		// update y with dy
		y = y + dy * ltStep;
	}

	bool Particle::isAlive() const {
		return lifeTime > 0.f;
	}

	glm::mat4 Particle::getTransform() const {
		return y.toMat4();
	}

	Particle Particle::create(const glm::vec3& originPosition, const glm::vec3& deltaPosition, float lifeTime) {
		return Particle{ Transform(originPosition), DeltaTransform(deltaPosition), lifeTime };
	}

	Particle Particle::create(const glm::vec3& originPosition, const glm::vec3& originRotation, const glm::vec3& deltaPosition, const glm::vec3& deltaRotation, float lifeTime) {
		return Particle{ Transform(originPosition, originPosition), DeltaTransform(deltaPosition, deltaRotation), lifeTime };
	}

	Particle Particle::create(const Transform& originTransform, const DeltaTransform& deltaTransform, float lifeTime) {
		return Particle{ originTransform, deltaTransform, lifeTime };
	}

	Particle Particle::create2D(const glm::vec2& originPosition, const glm::vec2& deltaPosition, float lifeTime) {
		return Particle{ Transform(glm::vec3(originPosition, 0.f)), DeltaTransform(glm::vec3(deltaPosition, 0.f)), lifeTime };
	}

	Particle Particle::create2D(const glm::vec2& originPosition, float originRotation, const glm::vec2& deltaPosition, float deltaRotation, float lifeTime) {
		return Particle{ Transform(glm::vec3(originPosition, 0.f), glm::vec3(0.f, 0.f, originRotation)), DeltaTransform(glm::vec3(deltaPosition, 0.f), glm::vec3(0.f, 0.f, deltaRotation)), lifeTime };
	}
	*/
} // namespace C78E