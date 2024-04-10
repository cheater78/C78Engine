#pragma once
#include <cstdint>

#include "imgui.h"

namespace C78E {

	class TextureHandle {
	public:
		TextureHandle(uint32_t handle) : m_RendererID(handle) {}
		TextureHandle(ImTextureID handle) : m_RendererID((uint32_t)handle) {}
		TextureHandle(const TextureHandle& other) : m_RendererID(other.m_RendererID) {}
		~TextureHandle() = default;

		operator uint32_t() const { return m_RendererID; }
		operator ImTextureID() const { return (ImTextureID)m_RendererID; }
	private:
		uint32_t m_RendererID = 0;
	};

}
