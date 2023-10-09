#pragma once

#include "C78E/Renderer/RendererAPI.h"

namespace C78E {

	class RenderCommand
	{
	public:
		static void init() {
			s_RendererAPI->init();
		}

		static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->setViewport(x, y, width, height);
		}

		static void setClearColor(const glm::vec4& color) {
			s_RendererAPI->setClearColor(color);
		}

		static void clear() {
			s_RendererAPI->clear();
		}

		static void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
			s_RendererAPI->drawIndexed(vertexArray, indexCount);
		}

		static void drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
			s_RendererAPI->drawLines(vertexArray, vertexCount);
		}

		static void setLineWidth(float width) {
			s_RendererAPI->setLineWidth(width);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}
