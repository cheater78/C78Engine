#pragma once

#include "C78E/Core/Layer.h"

#include "C78E/Events/ApplicationEvent.h"
#include "C78E/Events/KeyEvent.h"
#include "C78E/Events/MouseEvent.h"

namespace C78E::GUI {

	//TODO: ImGui Implementation missing!
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;
		virtual bool onEvent(Event& e) override;

		void begin();
		void end();

		void blockEvents(bool block) { m_BlockEvents = block; }
		
		void setDarkThemeColors();

		uint32_t getActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;

	};

}
