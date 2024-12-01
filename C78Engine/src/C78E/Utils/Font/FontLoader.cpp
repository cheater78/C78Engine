#include "C78EPCH.h"
#include "FontLoader.h"

namespace C78E {

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	Ref<Texture2D> FontLoader::createAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
		const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height) {
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

		//msdfgen::savePng(bitmap, std::string("assets/cache/font/" + fontName + ".png").c_str()); //TODO: directory, caching

		Texture2D::TextureSpecification spec;
		spec.width = bitmap.width;
		spec.height = bitmap.height;
		spec.format = Image::ImageFormat::RGB8;
		spec.generateMips = false;

		Ref<Texture2D> texture = Texture2D::create(spec);
		texture->setData((void*)bitmap.pixels, bitmap.width * bitmap.height * 3);
		return texture;
	}

	Ref<Font> FontLoader::loadFont(FilePath fontFile) {
		C78E_CORE_TRACE("FontLoader::loadFont: Loading Font File: {}", fontFile.string());
		Timer timer;
		MSDFData* data = new MSDFData();

		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		C78E_CORE_ASSERT(ft, "FontLoader::loadFont:   Failed to initialize FreeType!");

		std::string fileString = fontFile.string();

		// TODO: msdfgen::loadFontData loads from memory buffer which we'll need 
		msdfgen::FontHandle* font = msdfgen::loadFont(ft, fileString.c_str());
		if (!font) {
			C78E_CORE_ERROR("FontLoader::loadFont:   Failed to load font: {}", fileString);
			return nullptr;
		}

		struct CharsetRange {
			uint32_t Begin, End;
		};

		// From imgui_draw.cpp
		static const CharsetRange charsetRanges[] = {
			{ 0x0020, 0x00FF }
		};

		msdf_atlas::Charset charset;
		for (CharsetRange range : charsetRanges) {
			for (uint32_t c = range.Begin; c <= range.End; c++)
				charset.add(c);
		}

		double fontScale = 1.0;
		data->fontGeometry = msdf_atlas::FontGeometry(&data->glyphs);
		int glyphsLoaded = data->fontGeometry.loadCharset(font, fontScale, charset);
		C78E_CORE_TRACE("FontLoader::loadFont:   Glyphs: {} of {}", glyphsLoaded, charset.size());


		double emSize = 40.0;
		C78E_CORE_TRACE("FontLoader::loadFont:   Size: {} (hardcoded)", emSize);

		msdf_atlas::TightAtlasPacker atlasPacker;
		// atlasPacker.setDimensionsConstraint()
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		//atlasPacker.setPadding(0);
		atlasPacker.setScale(emSize);
		int remaining = atlasPacker.pack(data->glyphs.data(), (int)data->glyphs.size());
		C78E_CORE_ASSERT(remaining == 0, "FontLoader::loadFont:   Failed to arrange Glyphs to an Atlas!");

		int width, height;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8
		// if MSDF || MTSDF

		uint64_t coloringSeed = 0;
		bool expensiveColoring = false;
		if (expensiveColoring) {
			msdf_atlas::Workload([&glyphs = data->glyphs, &coloringSeed](int i, int threadNo) -> bool {
				unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				return true;
				}, static_cast<int>(data->glyphs.size())).finish(THREAD_COUNT);
		}
		else {
			unsigned long long glyphSeed = coloringSeed;
			for (msdf_atlas::GlyphGeometry& glyph : data->glyphs)
			{
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}


		Ref<Texture2D> fontAtlas = createAndCacheAtlas<msdfgen::byte, float, 3, msdf_atlas::msdfGenerator>(
			fontFile.filename().string(), (float)emSize, data->glyphs, data->fontGeometry, width, height
		);
		C78E_CORE_TRACE("FontLoader::loadFont:   AtlasTexture:");
		C78E_CORE_TRACE("FontLoader::loadFont:     Width: {}", fontAtlas->getWidth());
		C78E_CORE_TRACE("FontLoader::loadFont:     Heigt: {}", fontAtlas->getHeight());
		C78E_CORE_TRACE("FontLoader::loadFont:     Heigt: {}", C78E::Image::imageFormatToString(fontAtlas->getSpecification().format));


		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);

		C78E_CORE_TRACE("FontLoader::loadFont: Loading Font Successful, Took: {} ms", std::to_string(timer.elapsedMillis()));
		return createRef<Font>(data, fontAtlas);
	}

}
