#pragma once

#include "Core.h"
#include "C78e/Events/Event.h"
#include "C78e/Core/Timestep.h"

namespace C78e {

	class C78_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		const std::string& getName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}