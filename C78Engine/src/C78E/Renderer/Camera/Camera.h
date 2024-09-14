#pragma once
#include <C78E/Renderer/API/RendererAPI.h>
#include <glm/glm.hpp>

namespace C78E {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& getProjection() const { return m_Projection; }
	protected:
		static glm::mat4 perspective(float fov, float aspectRatio, float nearClip, float farClip) {
			switch (RendererAPI::getAPI()) {
			case RendererAPI::API::Vulkan:
				return glm::perspectiveRH(fov, aspectRatio, nearClip, farClip);
			default:
				return glm::perspectiveLH(fov, aspectRatio, nearClip, farClip);
			}
		}
		static glm::mat4 orthogonal(float left, float right, float top, float bottom, float nearClip, float farClip) {
			switch (RendererAPI::getAPI()) {
			case RendererAPI::API::Vulkan:
				return glm::orthoRH(left, right, top, bottom, nearClip, farClip);
			default:
				return glm::orthoLH(left, right, top, bottom, nearClip, farClip);
			}
		}
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};

}