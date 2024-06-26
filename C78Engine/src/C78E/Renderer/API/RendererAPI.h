#pragma once
#include <C78E/Renderer/API/VertexArray.h>

namespace C78E {

	class RendererAPI {
	public:
		enum class API { None = 0, OpenGL, Vulkan, DIRECTX, METAL };
		enum DepthFunc : uint32_t { NEVER = 0, LESS, EQUAL, LEQUAL, GREATER, NOTEQUAL, GEQUAL, ALWAYS };
		enum ShaderType : uint32_t { VERTEX, TESSELATION, TESSEVALUATION, GEOMETRY, FRAGMENT, COMPUTE };
	public:
		static API getAPI() { return s_API; }
		static Scope<RendererAPI> create();
	private:
		static API s_API;
	public:
		virtual ~RendererAPI() = default;

		virtual void init() = 0;
		virtual uint32_t getMaxTextureSlots(ShaderType stage) = 0;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;

		virtual void enableDepthTest(bool enable) = 0;
		virtual void setDepthFunc(DepthFunc func) = 0;
		virtual void setDepthWrite(bool enable) = 0;

		virtual void clear() = 0;

		virtual void drawIndexed(Ref<VertexArray> vertexArray, uint32_t indexCount = 0) = 0;
		virtual void drawLines(Ref<VertexArray> vertexArray, uint32_t vertexCount) = 0;
		
		virtual void setLineWidth(float width) = 0;
	};

}
