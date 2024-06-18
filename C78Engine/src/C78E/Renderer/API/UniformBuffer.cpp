#include "C78EPCH.h"
#include "UniformBuffer.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLUniformBuffer.h>
#include <Platform/Vulkan/VulkanUniformBuffer.h>

namespace C78E {

	Ref<UniformBuffer> UniformBuffer::create(uint32_t size) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLUniformBuffer>(size);
			case RendererAPI::API::Vulkan:  return createRefVulkanUniformBuffer>(size);
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLUniformBuffer>(size, binding);
		case RendererAPI::API::Vulkan:  return createRef<VulkanUniformBuffer>(size, binding);
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
