#include "C78EPCH.h"
#include "Framebuffer.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLFramebuffer.h>
#include <Platform/Vulkan/VulkanFramebuffer.h>

namespace C78E {
	
	Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLFramebuffer>(spec);
			case RendererAPI::API::Vulkan:  return createRef<VulkanFramebuffer>(spec);
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

