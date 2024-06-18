#include "C78EPCH.h"
#include "VertexArray.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLVertexArray.h>
#include <Platform/Vulkan/VulkanVertexArray.h>

namespace C78E {

	Ref<VertexArray> VertexArray::create() {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLVertexArray>();
			case RendererAPI::API::Vulkan:  return createRef<VulkanVertexArray>();
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}