#pragma once

#include <C78E/Core/Core.h>
#include <C78E/Events/Event.h>
#include <C78E/Core/Timestep.h>

namespace C78E {

	class Layer {
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual bool onEvent(Event& event) { return false; }

		const std::string& getName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}