#pragma once
#include <C78E/Core/Window/Layer.h>
#include <C78E/Core/Events/Event.h>
#include <C78E/Core/Window/DebugLayer/Console.h>

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
		Console m_Console;

	};

}
