#include "C78EPCH.h"
#include "FontLoader.h"

#include <C78E/Utils/Image/ImageLoader.h>

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8

namespace C78E {

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	Ref<Texture2D> FontLoader::createAndCacheAtlas(float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
		const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height, const FilePath& cacheFile) {

		Ref<Texture2D> texture = nullptr;
		if(!cacheFile.empty() && FileSystem::exists(cacheFile)) {
			auto image = ImageLoader::loadImage(cacheFile);

			Texture2D::TextureSpecification spec;
			spec.width = image->width;
			spec.height = image->height;
			spec.format = (Image::ImageFormat)image->bytesPerPixel;
			spec.generateMips = false;

			texture = Texture2D::create(spec, image->data);
		} else {
			msdf_atlas::GeneratorAttributes attributes;
			attributes.config.overlapSupport = true;
			attributes.scanlinePass = true;

			msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
			generator.setAttributes(attributes);
			generator.setThreadCount(THREAD_COUNT);
			generator.generate(glyphs.data(), (int)glyphs.size());

			msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

			//if(!cacheFile.empty())
			//	msdfgen::savePng(bitmap, cacheFile.string().c_str());

			Texture2D::TextureSpecification spec;
			spec.width = bitmap.width;
			spec.height = bitmap.height;
			spec.format = Image::ImageFormat::RGB8;
			spec.generateMips = false;

			texture = Texture2D::create(spec);
			texture->setData((void*)bitmap.pixels, bitmap.width * bitmap.height * Image::imageFormatSize(spec.format));
		}
		
		return texture;
	}

	Ref<Font> FontLoader::loadFont(FilePath fontFile, float fontSize, const FilePath& cacheFile) {
		return loadFont(FileSystem::readFileBinary(fontFile), fontSize, cacheFile);
	}

	Ref<Font> FontLoader::loadFont(Ref<ScopedBuffer> fontBuffer, float fontSize, const FilePath& cacheFile) {
		C78E_CORE_TRACE("FontLoader::loadFont: Loading Font...");
		Timer timer;
		MSDFData* data = new MSDFData();

		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		C78E_CORE_VALIDATE(ft, return nullptr, "FontLoader::loadFont:   Failed to initialize FreeType!");

		msdfgen::FontHandle* font = msdfgen::loadFontData(ft, (msdfgen::byte*)fontBuffer->as<char>(), static_cast<int>(fontBuffer->size()));
		C78E_CORE_VALIDATE(font, return nullptr, "FontLoader::loadFont:   Failed to load Font!");

		struct CharsetRange { uint32_t begin, end; };
		static const CharsetRange charsetRanges[] = { { 0x0020, 0x00FF } }; //Latin alphapet and some special chars
		msdf_atlas::Charset charset;
		for (CharsetRange range : charsetRanges)
			for (uint32_t c = range.begin; c <= range.end; c++)
				charset.add(c);

		double fontScale = 1.0;
		data->fontGeometry = msdf_atlas::FontGeometry(&data->glyphs);
		int glyphsLoaded = data->fontGeometry.loadCharset(font, fontScale, charset);
		C78E_CORE_TRACE("FontLoader::loadFont:   Glyphs: {} of {}", glyphsLoaded, charset.size());
		C78E_CORE_TRACE("FontLoader::loadFont:   Size: {}", fontSize);

		msdf_atlas::TightAtlasPacker atlasPacker;
		//atlasPacker.setDimensionsConstraint()
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		//atlasPacker.setPadding(0);
		atlasPacker.setScale(static_cast<double>(fontSize));
		int remaining = atlasPacker.pack(data->glyphs.data(), (int)data->glyphs.size());
		C78E_CORE_VALIDATE(remaining == 0, return nullptr, "FontLoader::loadFont:   Failed to arrange Glyphs to an Atlas!");

		int width, height;
		atlasPacker.getDimensions(width, height);
		fontSize = atlasPacker.getScale();

		// if MSDF || MTSDF TODO

		uint64_t coloringSeed = 0;
		bool expensiveColoring = false; //TODO use
		if (expensiveColoring) {
			msdf_atlas::Workload(
				[&glyphs = data->glyphs, &coloringSeed](int i, int threadNo) -> bool {
					unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
					glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
					return true;
				},
				static_cast<int>(data->glyphs.size())
			).finish(THREAD_COUNT);
		}
		else {
			unsigned long long glyphSeed = coloringSeed;
			for (msdf_atlas::GlyphGeometry& glyph : data->glyphs) {
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}


		Ref<Texture2D> fontAtlas = createAndCacheAtlas<msdfgen::byte, float, 3, msdf_atlas::msdfGenerator>(
			(float)fontSize, data->glyphs, data->fontGeometry, width, height, cacheFile
		);
		C78E_CORE_TRACE("FontLoader::loadFont:   AtlasTexture:");
		C78E_CORE_TRACE("FontLoader::loadFont:     Width: {}", fontAtlas->getWidth());
		C78E_CORE_TRACE("FontLoader::loadFont:     Height: {}", fontAtlas->getHeight());
		C78E_CORE_TRACE("FontLoader::loadFont:     Format: {}", C78E::Image::imageFormatToString(fontAtlas->getSpecification().format));


		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);

		C78E_CORE_TRACE("FontLoader::loadFont: Loading Font Successful, Took: {} ms", std::to_string(timer.elapsedMillis()));
		return createRef<Font>(data, fontAtlas);
	}

}
