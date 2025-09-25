#pragma once
#include "SwapInterval.h"
#include <C78E/Graphics/API/Buffer/FrameBuffer.h>

namespace C78E {

	class GraphicsContext;

	enum SwapChainBufferCount : uint32_t {
		SingleBuffer = 1,
		DoubleBuffer = 2,
		TripleBuffer = 3,
	};

	// size, format is determined by the Context(Surface-Capabilities)
	struct SwapChainConfig {
		Ref<RenderPass> frameBufferRenderPass;
		uint32_t frameBufferCount = (uint32_t)SwapChainBufferCount::TripleBuffer;
		std::vector<FrameBufferAttachmentSpecification> frameBufferAttachmentSpecifications;
		uint32_t frameBufferActiveDepthAttachment = std::numeric_limits<uint32_t>::max();
		uint32_t frameBufferMultiSample = MultiSample::X1;
		SwapInterval swapInterval = SwapInterval::Immediate;
	};
	
	struct SwapChainState {
		Ref<RenderPass> frameBufferRenderPass;
		uint32_t frameBufferCount = (uint32_t)SwapChainBufferCount::TripleBuffer;
		FrameBufferConfig frameBufferConfig;
		uint32_t ownedAttachmentIndex = std::numeric_limits<uint32_t>::max();

		SwapInterval swapInterval = SwapInterval::Immediate;
	};

	
	class SwapChain {
	public:
		using FrameIndex = uint32_t;
		using FrameCount = uint32_t;
	public:
		static Ref<SwapChain> create(GraphicsContext& ctx, const SwapChainConfig& config);
	public:
		SwapChain(GraphicsContext& ctx);
		SwapChain(const SwapChain&) = delete; // no copy
		SwapChain& operator=(const SwapChain&) = delete; // no assignment
		virtual ~SwapChain() = default;

		virtual bool recreate() = 0;
		virtual bool recreate(SwapChainConfig config) = 0;

		virtual Ref<FrameBuffer> aquireNextFramebuffer(FrameIndex frameIndex) = 0;
		virtual Ref<FrameBuffer> getFrameBuffer(FrameIndex frameIndex) = 0;

	public:
		const SwapChainState& getState() const { return m_State; }
		FrameCount getFrameCount() const {
			return static_cast<uint32_t>(m_State.frameBufferCount);
		}
		uint32_t getSwapChainOwnedAttachmentIndex() const {
			return m_State.ownedAttachmentIndex;
		}
		ImageFormat getSwapChainTargetFormat() {
			return m_State.frameBufferConfig
				.attachmentSpecifications[getSwapChainOwnedAttachmentIndex()].format;
		}
		ImageSize getTargetSize() const {
			return m_State.frameBufferConfig.size;
		}
		RenderArea getFullRenderArea() const {
			return RenderArea{
				.viewport = Viewport {
					.offset = {0, 0},
					.size = m_State.frameBufferConfig.size
				},
				.scissor = Scissor {
					.offset = {0, 0},
					.size = m_State.frameBufferConfig.size
				}
			};
		}
	protected:
		GraphicsContext& m_GraphicsContext;
		SwapChainState m_State;
	};

} // namespace C78E
