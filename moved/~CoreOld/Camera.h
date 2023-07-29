#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "Entity.h"

namespace C78e {

	class Camera : public Entity {
	public:

		Camera(entt::entity enttity, Scene* scene);
		Camera(uint32_t enttID, Scene* scene);
		Camera(const Camera&) = default;
		~Camera();

		void setOrthographicProj(float left, float right, float top, float bottom, float near, float far);
		void setPerspectiveProj(float fovy, float aspect, float near, float far);

		void setPosDir(glm::vec3 position, glm::vec3 direction);
		void setPosTarget(glm::vec3 position, glm::vec3 target);
		void setPosRot(glm::vec3 position, glm::vec3 rotation);

		glm::mat4& getProjMat();
		glm::mat4 getViewMat();
		glm::mat4 getIViewMat() { return glm::inverse(getViewMat()); }
	};
}