#include "C78EPCH.h"
#include "Font.h"

#include <C78E/Renderer/Assets/Texture/Image.h>
#include <C78E/Renderer/API/Texture.h>
#include <C78E/Utils/Font/FontUtils.h>

namespace C78E {

	Font::Font(MSDFData* data, Ref<C78E::Texture2D> atlasTexture)
		: m_Data(data), m_AtlasTexture(atlasTexture) {
	}

	Font::~Font() {
		delete m_Data;
	}

}
