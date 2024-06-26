#include "C78EPCH.h"
#include "Shader.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLShader.h>
#include <Platform/Vulkan/VulkanShader.h>

namespace C78E {

	Ref<Shader> Shader::create(const std::string& filepath) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLShader>(filepath);
			case RendererAPI::API::Vulkan:  return createRef<VulkanShader>(filepath);
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
			case RendererAPI::API::Vulkan:  return createRef<VulkanShader>(name, vertexSrc, fragmentSrc);
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string& name, const std::string& computeSrc) {
		switch (RendererAPI::getAPI())
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLShader>(name, computeSrc);
			case RendererAPI::API::Vulkan:  return createRef<VulkanShader>(name, computeSrc);
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}