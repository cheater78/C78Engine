#pragma once
#include "ImageFormat.h"

namespace C78E {

	struct ImageArea {
	public:
		ImagePosition offset;
		ImageSize size;

	};

	using Viewport = ImageArea;
	using Scissor = ImageArea;
	
	struct RenderArea {
	public:
		Viewport viewport;
		Scissor scissor;
	};

}
