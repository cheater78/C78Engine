#pragma once
#include <C78E/Renderer/API/Texture.h>

namespace C78E {

	class VulkanTexture2D : public Texture2D {
	public:
		VulkanTexture2D(const Texture2D::TextureSpecification& specification);
		VulkanTexture2D(const Texture2D::TextureSpecification& specification, const Buffer& data);
		VulkanTexture2D(Image& image);
		VulkanTexture2D(const VulkanTexture2D&) = default; //TODO: Maybe needed!
		virtual ~VulkanTexture2D();

		virtual uint32_t getWidth() const override { return m_Specification.width;  }
		virtual uint32_t getHeight() const override { return m_Specification.height; }
		virtual uint32_t getRendererID() const override { return m_RendererID; }
		
		virtual void setData(void* data, uint32_t size) override;
		virtual void bind(uint32_t slot = 0) const override;
		virtual void bindImage(uint32_t binding = 0) const override;

		virtual bool isLoaded() const override { return m_IsLoaded; }
		virtual const Texture2D::TextureSpecification& getSpecification() const override { return m_Specification; }

		virtual bool operator==(const Texture& other) const override { return m_RendererID == other.getRendererID(); }
	private:
		uint32_t m_RendererID;
		Texture2D::TextureSpecification m_Specification;

		bool m_IsLoaded = false;

	};

	class VulkanCubeMap : public CubeMap {
	public:
		VulkanCubeMap(CubeMap::TextureSpecification& specification);
		VulkanCubeMap(std::vector<Image>& images);
		VulkanCubeMap(Ref<Image> crossCubeMap);
		~VulkanCubeMap();

		uint32_t getSize() const override { return m_Specification.size; };
		virtual uint32_t getRendererID() const override { return m_RendererID; };

		virtual void setData(void* data, uint32_t size) override;
		virtual void bind(uint32_t slot = 0) const override;

		virtual bool isLoaded() const override { return m_IsLoaded; };
		const CubeMap::TextureSpecification& getSpecification() const override { return m_Specification; };

		bool operator==(const Texture& other) const override { return m_RendererID == other.getRendererID(); }
	private:
		uint32_t m_RendererID = 0;
		CubeMap::TextureSpecification m_Specification;

		bool m_IsLoaded = false;
	};

}
