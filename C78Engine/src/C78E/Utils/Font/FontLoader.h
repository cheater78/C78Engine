#pragma once
#include <C78E/Renderer/Assets/Texture/Image.h>
#include <C78E/Renderer/API/Texture.h>
#include <C78E/Renderer/Assets/Font/Font.h>

#include "FontUtils.h"


namespace C78E {

	class FontLoader {
	public:
		static Ref<Font> loadFont(FilePath fontFile, float fontSize, const FilePath& cacheFile = "");
		static Ref<Font> loadFont(Ref<ScopedBuffer> fontBuffer, float fontSize, const FilePath& cacheFile = "");

	private:
		template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
		static Ref<Texture2D> createAndCacheAtlas(
			float fontSize,
			const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
			const msdf_atlas::FontGeometry& fontGeometry,
			uint32_t width, uint32_t height,
			const FilePath& cacheFile
		);
	};

}
