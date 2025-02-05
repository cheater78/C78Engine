#pragma once

namespace C78E {

	struct TextureHandle {
	public:
		TextureHandle(uint32_t handle);
		TextureHandle(ImTextureID handle);
		TextureHandle(const TextureHandle& other);
		~TextureHandle();

		// Cast operators
		operator uint32_t() const;
		operator ImTextureID() const;

		// Assignment operators
		TextureHandle& operator=(uint32_t handle);
		TextureHandle& operator=(ImTextureID handle);

		bool operator==(const TextureHandle& other) const;

	private:
		uint32_t m_RendererID = 0;
	};

}