#pragma once

#include "C78E/Renderer/RendererAPI.h"

namespace C78E {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void init() override;
		virtual uint32_t getMaxTextureSlots(ShaderStage stage) override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void enableDepthTest(bool enable) override;
		virtual void setDepthFunc(DepthFunc func) override;
		virtual void setDepthWrite(bool enable) override;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		
		virtual void setLineWidth(float width) override;
	};


}
