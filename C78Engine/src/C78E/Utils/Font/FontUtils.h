#pragma once

#undef INFINITE
#define MSDFGEN_USE_LIBPNG
#include "msdf-atlas-gen.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"

#include <vector>

namespace C78E {

	struct MSDFData {
		std::vector<msdf_atlas::GlyphGeometry> glyphs;
		msdf_atlas::FontGeometry fontGeometry;
	};
}
