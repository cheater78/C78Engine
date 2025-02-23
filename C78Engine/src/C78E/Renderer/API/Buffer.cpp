#include "C78EPCH.h"
#include "Buffer.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLBuffer.h>

namespace C78E {

	Ref<VertexBuffer> VertexBuffer::create(uint32_t size) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLVertexBuffer>(size);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLVertexBuffer>(vertices, size);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLIndexBuffer>(indices, size);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}