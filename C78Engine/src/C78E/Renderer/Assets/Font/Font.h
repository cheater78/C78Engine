#pragma once

#include "C78E/Renderer/API/Texture.h"

namespace C78E {

	struct MSDFData;

	class Font {
	public:
		Font(const FilePath& font);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();
	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};

}
