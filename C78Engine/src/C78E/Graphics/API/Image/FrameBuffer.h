#pragma once
#include "Texture.h"
#include <C78E/Graphics/API/Command/RenderPass.h>

namespace C78E {

	class GraphicsContext;

	struct FrameBufferAttachmentSpecification : public Texture::Specification, Texture2D::Option {
		FrameBufferAttachmentSpecification(ImageFormat format = ImageFormat::None, const Texture2D::Option& option = Texture2D::Option())
			: Texture::Specification(format), Texture2D::Option(option) { }
	};

	struct FrameBufferConfig {
		ImageSize size = ImageSize(1, 1);
		std::vector<FrameBufferAttachmentSpecification> attachmentSpecifications = {};
		uint32_t activeDepthAttachment = std::numeric_limits<uint32_t>::max();
		uint32_t multiSample = MultiSample::X1;
	};

	class FrameBuffer {
	public:
		static Ref<FrameBuffer> create(GraphicsContext& ctx, const FrameBufferConfig& cfg, Ref<RenderPass> renderPass);
	public:
		FrameBuffer(GraphicsContext& ctx, const FrameBufferConfig& spec, Ref<RenderPass> renderPass);
	public:
		virtual ~FrameBuffer() = default;

		virtual void resize(ImageSize size) = 0;
		virtual bool isSwapChainTarget() const = 0;
		virtual uint32_t getSwapChainImageIndex() const = 0;
		//TODO: some form of reading data to cpu memory -> provided a buffer
		//TODO: maybe clearing Attachments to a value -> needs to acknolage different data types..?

	public:
		const FrameBufferConfig& getFrameBufferConfig() const;
		const ImageSize getSize() const;
		const std::vector<FrameBufferAttachmentSpecification>& getAttachmentSpecifications() const;
		bool hasDepthAttachment() const;
		const FrameBufferAttachmentSpecification& getDepthAttachmentSpecification() const;
		bool isMultisampled() const;
		uint32_t getSamples() const;

		Ref<RenderPass> getRenderPass() const { return m_RenderPass; }
	protected:
		GraphicsContext& m_GraphicsContext;
		FrameBufferConfig m_Config;
		Ref<RenderPass> m_RenderPass;
	};


}
