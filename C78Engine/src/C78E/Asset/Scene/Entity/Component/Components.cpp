#include "C78EPCH.h"
#include "Components.h"

#include <C78E/Asset/Scene/Entity/ScriptableEntity.h>

namespace C78E {

	IDComponent::IDComponent(UUID _id)
		: id(_id) {
	}

	TransformComponent::TransformComponent(const glm::vec3& translation)
		: translation(translation) {
	}

	glm::mat4 TransformComponent::getTransform() const {
		return glm::translate(glm::mat4(1.0f), translation)
			* glm::toMat4(glm::quat(rotation))
			* glm::scale(glm::mat4(1.0f), scale);
	}

	glm::mat4 TransformComponent::normalMat() {
		return glm::mat4(glm::inverseTranspose(getTransform()));
	}

	template<typename T>
	void NativeScriptComponent::bind() {
		instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
	}

	// Explicit template instantiation
	template void NativeScriptComponent::bind<ScriptableEntity>();

}