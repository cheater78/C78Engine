#pragma once

#include <C78E/Renderer/Assets/Font/MSDFData.h>
#include <C78E/Renderer/Assets/Texture/Image.h>
#include <C78E/Renderer/API/Texture.h>
#include <C78E/Renderer/Assets/Font/Font.h>

#undef INFINITE
#include "msdf-atlas-gen.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"

namespace C78E {

	class FontLoader {
	public:
		static Ref<Font> loadFont(FilePath fontFile);

	private:
		template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
		static Ref<Texture2D> createAndCacheAtlas(
			const std::string& fontName,
			float fontSize,
			const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
			const msdf_atlas::FontGeometry& fontGeometry,
			uint32_t width, uint32_t height
		);
	};

}
