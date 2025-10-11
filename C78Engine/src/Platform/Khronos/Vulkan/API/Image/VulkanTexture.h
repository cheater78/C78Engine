#pragma once
#include <C78E/Graphics/API/Image/Texture.h>

namespace C78E {

	VkFilter toVkFilter(Texture::Filter filter);
	VkSamplerMipmapMode toVkSamplerMipmapMode(Texture::Filter filter);
	VkSamplerAddressMode toVkSamplerAddressMode(Texture::Wrap wrap);

	
	class VulkanTexture : public Texture {
	public:
		VulkanTexture();
		virtual ~VulkanTexture();
		
		void copyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

		void createImageWithInfo(
			const VkImageCreateInfo& imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory);

	private:
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
		uint32_t m_Width, m_Height;
	};
	
}
