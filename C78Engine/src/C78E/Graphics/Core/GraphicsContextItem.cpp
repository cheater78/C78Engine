#include "C78EPCH.h"
#include "GraphicsContextItem.h"
#include <C78E/Graphics/API/GraphicsContext.h>

namespace C78E {



	GraphicsContextItem::GraphicsContextItem(GraphicsContext& ctx)
		: m_GraphicsContext(ctx) {
	}

	GraphicsContextItem::~GraphicsContextItem() {
	}

	GraphicsContext& GraphicsContextItem::getGraphicsContext() const {
		return m_GraphicsContext;
	}

}
