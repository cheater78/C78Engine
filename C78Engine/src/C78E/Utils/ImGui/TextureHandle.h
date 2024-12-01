#pragma once
#include <imgui.h>
#include <cstdint>

namespace C78E::GUI {

	class TextureHandle {
	public:
		TextureHandle(uint32_t handle) : m_RendererID(static_cast<size_t>(handle)) {}
		TextureHandle(ImTextureID handle) : m_RendererID((size_t)handle) {}
		TextureHandle(const TextureHandle& other) : m_RendererID(other.m_RendererID) {}
		~TextureHandle() = default;
		
		operator uint32_t() const { return static_cast<uint32_t>(m_RendererID); }
		operator ImTextureID() const { return (ImTextureID)m_RendererID; }
	private:
		size_t m_RendererID = 0;
	};

}
