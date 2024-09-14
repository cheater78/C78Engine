#include "C78EPCH.h"
#include "Font.h"

#include "MSDFData.h"
#include <C78E/Renderer/Assets/Texture/Image.h>
#include <C78E/Renderer/API/Texture.h>

#undef INFINITE
#include "msdf-atlas-gen.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"



namespace C78E {

	Font::Font(MSDFData* data, Ref<C78E::Texture2D> atlasTexture)
		: m_Data(data), m_AtlasTexture(atlasTexture) {
	}

	Font::~Font() {
		delete m_Data;
	}

}
