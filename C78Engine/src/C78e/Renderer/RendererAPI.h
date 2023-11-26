#pragma once

#include "C78E/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace C78E {

	class RendererAPI {
	public:
		enum class API {
			None = 0, OpenGL = 1, Vulkan = 2
		};
		enum DepthFunc : uint32_t {
			NEVER = 0, LESS = 1, EQUAL = 2, LEQUAL = 3, GREATER = 4, NOTEQUAL = 5, GEQUAL = 6, ALWAYS = 7
		};
		enum ShaderStage : uint32_t {
			VERTEX = 0, TESSELATION = 1, TESSEVALUATION = 2, GEOMETRY = 3, FRAGMENT = 4, COMPUTE = 5
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void init() = 0;
		virtual uint32_t getMaxTextureSlots(ShaderStage stage) = 0;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;

		virtual void enableDepthTest(bool enable) = 0;
		virtual void setDepthFunc(DepthFunc func) = 0;
		virtual void setDepthWrite(bool enable) = 0;

		virtual void clear() = 0;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		
		virtual void setLineWidth(float width) = 0;
		
		static API getAPI() { return s_API; }
		static Scope<RendererAPI> create();
	private:
		static API s_API;
	};

}
