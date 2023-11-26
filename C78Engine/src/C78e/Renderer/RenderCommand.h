#pragma once

#include "C78E/Renderer/RendererAPI.h"

namespace C78E {

	class RenderCommand
	{
	public:
		static void init() {
			s_RendererAPI->init();
		}

		static uint32_t getMaxTextureSlots(RendererAPI::ShaderStage stage) {
			return s_RendererAPI->getMaxTextureSlots(stage);
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

		static void enableDepthTest(bool enable) {
			s_RendererAPI->enableDepthTest(enable);
		}

		static void setDepthFunc(RendererAPI::DepthFunc func) {
			s_RendererAPI->setDepthFunc(func);
		}
		
		static void setDepthWrite(bool enable) {
			s_RendererAPI->setDepthWrite(enable);
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
