#include "C78EPCH.h"
#include "LayerStack.h"

namespace C78E {

	LayerStack::~LayerStack()
	{
		for (Ref<Layer> layer : m_Layers)
		{
			layer->onDetach();
		}
	}

	void LayerStack::pushLayer(Ref<Layer> layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::pushOverlay(Ref<Layer> overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Ref<Layer> layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->onDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::popOverlay(Ref<Layer> overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->onDetach();
			m_Layers.erase(it);
		}
	}

}
