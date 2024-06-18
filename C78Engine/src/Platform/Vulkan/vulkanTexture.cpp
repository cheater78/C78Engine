#include "C78EPCH.h"
#include "VulkanTexture.h"

namespace C78E {

	VulkanTexture2D::VulkanTexture2D(const Texture2D::TextureSpecification& specification)
		: m_RendererID(0), m_Specification(specification), m_IsLoaded(false) {
	}

	VulkanTexture2D::VulkanTexture2D(const Texture2D::TextureSpecification& specification, const Buffer& data)
		: m_Specification(specification) {
	}

	VulkanTexture2D::VulkanTexture2D(Image& image) {
	}

	VulkanTexture2D::~VulkanTexture2D() {
	}

	void VulkanTexture2D::setData(void* data, uint32_t size) {
	}

	void VulkanTexture2D::bind(uint32_t slot) const {
	}

	void VulkanTexture2D::bindImage(uint32_t binding) const {
	}


	VulkanCubeMap::VulkanCubeMap(CubeMap::TextureSpecification& specification)
		: m_IsLoaded(false), m_Specification(specification) {
	}

	VulkanCubeMap::VulkanCubeMap(std::vector<Image>& images)
		: m_IsLoaded(true) {
	}

	VulkanCubeMap::VulkanCubeMap(Ref<Image> crossCubeMap)
		: m_IsLoaded(true) {
	}

	VulkanCubeMap::~VulkanCubeMap() {
	}
	
	void VulkanCubeMap::setData(void* data, uint32_t size) {
	}

	void VulkanCubeMap::bind(uint32_t slot) const {
	}
	
}
