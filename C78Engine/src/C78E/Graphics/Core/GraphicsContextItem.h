#pragma once

namespace C78E {

	class GraphicsContext;

	class GraphicsContextItem {
	public:
		GraphicsContextItem(GraphicsContext& ctx);
		virtual ~GraphicsContextItem();

		virtual bool alive() = 0; // whether the GraphicsContextItem still has a backend
		virtual void free() = 0; // frees all backend resources (e.g. to destroy the GraphicsContext safely)
	public:
		GraphicsContext& getGraphicsContext() const;

	protected:
		GraphicsContext& m_GraphicsContext;
	};

}
