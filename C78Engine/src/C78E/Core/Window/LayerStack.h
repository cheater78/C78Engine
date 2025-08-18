#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Window/Layer.h>

namespace C78E {

	/**
	 * @brief Stack of layers for a window.
	 * Features two types of layers:
	 * Layers that are pushed to the stack and overlays that are always on top of all Layers.
	 */
	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack();

		void pushLayer(Ref<Layer> layer);
		void pushOverlay(Ref<Layer> overlay);
		void popLayer(Ref<Layer> layer);
		void popOverlay(Ref<Layer> overlay);

		std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }
		std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Ref<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Ref<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Ref<Layer>>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Ref<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};

}