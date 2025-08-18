#pragma once

namespace C78E {

    /**
	 * @brief Particle struct for a ParticleSystem, having a Transform, DeltaTransform to perform per second, and lifetime in Seconds
     */
	/*
    struct Particle {
        C78E::Transform y;
        C78E::DeltaTransform dy;
		float lifeTime = 0.f; // in seconds, default Particles are dead

		void onUpdate(C78E::Timestep delta);
		bool isAlive() const;
		glm::mat4 getTransform() const;

		static Particle create(const glm::vec3& originPosition, const glm::vec3& deltaPosition, float lifeTime);
		static Particle create(const glm::vec3& originPosition, const glm::vec3& originRotation, const glm::vec3& deltaPosition, const glm::vec3& deltaRotation, float lifeTime);
		static Particle create(const Transform& originTransform, const DeltaTransform& deltaTransform, float lifeTime);
		static Particle create2D(const glm::vec2& originPosition, const glm::vec2& deltaPosition, float lifeTime);
		static Particle create2D(const glm::vec2& originPosition, float originRotation, const glm::vec2& deltaPosition, float deltaRotation, float lifeTime);
    };
	*/

} // namespace C78E