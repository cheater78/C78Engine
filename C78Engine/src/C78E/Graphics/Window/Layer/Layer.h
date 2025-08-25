#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Events/Event.h>
#include <C78E/Core/Timestep.h>
#include <C78E/Utils/StdUtils.h>

namespace C78E {

	class Window;

	/**
	 * @brief Layer interface for window layers.
	 */
	class Layer {
	public:
		Layer(Window& window, const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onDebugRender() {}
		virtual void onEvent(Event& event) { }

		const std::string& getName() const { return m_Name; }
	protected:
		Window& m_Window; // Reference to the window this layer is on
		std::string m_Name;
	};

}
