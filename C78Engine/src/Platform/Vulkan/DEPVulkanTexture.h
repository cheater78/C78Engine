#pragma once



namespace C78e {

	struct TextureInfo {
		VulkanDevice& device;
		std::string filename;
		VkCommandBuffer commandBuffer;
		VkQueue queue;
		VkDescriptorSetLayout layout;
		VkDescriptorPool descriptorPool;
	};

	class VulkanTexture : public Texture2D {
	public:

		VulkanTexture(VulkanDevice& device, std::string textureFile);
		~VulkanTexture();

		VkDescriptorImageInfo descriptorInfo();
		VkDescriptorImageInfo textureImageInfo;
	private:
		VulkanDevice& m_Device;

		RawImage m_RawImg;
		std::string m_File;

		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;
		VkImageView m_TextureImageView;
		VkSampler m_TextureSampler;

		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void createTextureImageView();
		void createTextureSampler();

	};
}
