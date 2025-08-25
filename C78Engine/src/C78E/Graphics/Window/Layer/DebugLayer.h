#pragma once
#include "Layer.h"

namespace C78E {
	
	// Window itself is depended on DebugLayer
	class Window;

	class DebugLayer : public Layer {
	public:
		DebugLayer(Window& window);
		virtual ~DebugLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onDebugRender() override;
		virtual void onEvent(Event& e) override;

		void begin();
		void end();

		void blockEvents(bool block) { m_BlockEvents = block; }
		
		void setDarkThemeColors();
	private:
		bool m_BlockEvents = true;
	};

}
