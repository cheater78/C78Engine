#pragma once
#include <C78E/Graphics/API/Buffer/FrameBuffer.h>

namespace C78E {

	class GraphicsContext;

	enum RefreshMode : int {
		Unlimited = 0,
		Vsync = 1
	};

	enum class SwapChainBufferCount : uint32_t {
		SingleBuffer = 1,
		DoubleBuffer = 2,
		TripleBuffer = 3,
	};

	struct SwapChainConfig {
		uint32_t bufferCount = (uint32_t)SwapChainBufferCount::TripleBuffer;
		FrameBufferSpecification swapChainElementFrameBufferSpec;
		uint32_t swapChainColorAttachmentIndex = 0;
		Ref<RenderPass> renderPass;
		RefreshMode refreshMode = RefreshMode::Unlimited;
	};
	
	class SwapChain {
	public:
		using ImageIndex = uint32_t; // Index of the current image in the swap chain
	public:
		static Ref<SwapChain> create(GraphicsContext& ctx, const SwapChainConfig& config);
	public:
		SwapChain(GraphicsContext& ctx, const SwapChainConfig& config);
		SwapChain(const SwapChain&) = delete; // no copy
		SwapChain& operator=(const SwapChain&) = delete; // no assignment
		virtual ~SwapChain() = default;

		virtual bool recreate(SwapChainConfig config) = 0;
		virtual bool resize(ImageSize size) = 0;

		virtual Ref<FrameBuffer> aquireNextFramebuffer(uint32_t frameIndex) = 0;
		virtual Ref<FrameBuffer> getFrameBuffer(uint32_t frameIndex) = 0;

	public:
		uint32_t getFrameCount() const;
		const SwapChainConfig& getConfig() const { return m_Config; }
	protected:
		GraphicsContext& m_GraphicsContext;
		SwapChainConfig m_Config; // keep for recreation and readable current state
	};

} // namespace C78E

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::RefreshMode& mode) {
		switch (mode) {
		case C78E::RefreshMode::Unlimited: return "Unlimited";
		case C78E::RefreshMode::Vsync: return "Vsync";
			//case C78E::WindowRefreshMode::Gsync: return "Gsync";
		default: return "Unknown";
		}
	}

}
