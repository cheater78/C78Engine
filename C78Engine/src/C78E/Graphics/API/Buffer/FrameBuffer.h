#pragma once
#include <C78E/Graphics/API/Texture/Texture.h>
#include <C78E/Graphics/API/Command/RenderPass.h>

namespace C78E {

	class GraphicsContext;

	struct FrameBufferAttachmentSpecification : public Texture::Specification, Texture2D::Option {
		FrameBufferAttachmentSpecification(ImageFormat format, const Texture2D::Option& option = Texture2D::Option())
			: Texture::Specification(format), Texture2D::Option(option) { }
	};

	struct FrameBufferSpecification {
		ImageSize size = ImageSize(0, 0);
		bool hasDepthAttachment = false;
		FrameBufferAttachmentSpecification depthAttachmentSpecification = FrameBufferAttachmentSpecification(ImageFormat::D24S8);
		std::vector<FrameBufferAttachmentSpecification> colorAttachmentSpecifications = { FrameBufferAttachmentSpecification(ImageFormat::RGBA8) };
		uint32_t swapChainColorAttachmentIndex = (uint32_t)-1;
		uint32_t samples = 1;
	};

	class FrameBuffer {
	public:
		static Ref<FrameBuffer> create(GraphicsContext& ctx, const FrameBufferSpecification& spec, Ref<RenderPass> renderPass);
	public:
		FrameBuffer(GraphicsContext& ctx, const FrameBufferSpecification& spec, Ref<RenderPass> renderPass);
	public:
		virtual ~FrameBuffer() = default;

		virtual void resize(ImageSize size) = 0;
		virtual bool isSwapChainTarget() const = 0;
		//TODO: some form of reading data to cpu memory -> provided a buffer
		//TODO: maybe clearing Attachments to a value -> needs to acknolage different data types..?
	public:
		const FrameBufferSpecification& getFrameBufferSpecification() const;
		const std::vector<FrameBufferAttachmentSpecification>& getColorAttachmentSpecifications() const;
		bool hasDepthAttachment() const;
		const FrameBufferAttachmentSpecification& getDepthAttachmentSpecifications() const;
		bool isMultisampled() const;
		uint32_t getSamples() const;

		Ref<RenderPass> getRenderPass() const { return m_RenderPass; }
	protected:
		GraphicsContext& m_GraphicsContext;
		FrameBufferSpecification m_Specification;
		Ref<RenderPass> m_RenderPass;
	};


}
