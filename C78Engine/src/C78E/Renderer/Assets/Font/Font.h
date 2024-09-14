#pragma once

#include "C78E/Renderer/API/Texture.h"

namespace C78E {

	struct MSDFData;

	class Font : public Asset {
	public:
		Font(MSDFData* data, Ref<C78E::Texture2D> atlasTexture);
		~Font();

		const MSDFData* getMSDFData() const { return m_Data; }
		Ref<C78E::Texture2D> getAtlasTexture() const { return m_AtlasTexture; }

		virtual Asset::AssetType getType() const override { return Asset::AssetType::Font; }
		static Asset::AssetType getClassType() { return Asset::AssetType::Font; }
	private:
		MSDFData* m_Data;
		Ref<C78E::Texture2D> m_AtlasTexture;
	};

}
