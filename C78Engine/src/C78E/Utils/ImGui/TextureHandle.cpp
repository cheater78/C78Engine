#include "C78EPCH.h"
#include "TextureHandle.h"
#include "ImGuiUtils.h"

namespace C78E {

	TextureHandle::TextureHandle(uint32_t handle)
		: m_RendererID(handle) {
	}

	TextureHandle::TextureHandle(ImTextureID handle)
		: m_RendererID(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle))) {
	}

	TextureHandle::TextureHandle(const TextureHandle& other) 
		: m_RendererID(other.m_RendererID) {
	}

	TextureHandle::~TextureHandle() {
	}

	TextureHandle::operator uint32_t() const {
		return m_RendererID;
	}

	TextureHandle::operator ImTextureID() const {
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(m_RendererID));
	}

	TextureHandle& TextureHandle::operator=(uint32_t handle) {
		m_RendererID = handle;
		return *this;
	}

	TextureHandle& TextureHandle::operator=(ImTextureID handle) {
		m_RendererID = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
		return *this;
	}

	bool TextureHandle::operator==(const TextureHandle& other) const {
		return m_RendererID == other.m_RendererID;
	}

}