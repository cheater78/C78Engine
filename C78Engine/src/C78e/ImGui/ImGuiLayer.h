#pragma once

#include "C78E/Core/Layer.h"

#include "C78E/Events/ApplicationEvent.h"
#include "C78E/Events/KeyEvent.h"
#include "C78E/Events/MouseEvent.h"

namespace C78E {

	class C78_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event& e) override;

		void begin();
		void end();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		
		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;

	};

}
