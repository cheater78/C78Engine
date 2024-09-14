#pragma once
#include "Model.h"

namespace C78E {

	class ModelComposite {
	public:
		ModelComposite(const std::vector<Ref<Model>>& parts) : m_Parts(parts) { }
		ModelComposite(const ModelComposite& other) = delete;
		~ModelComposite() = default;

		const std::vector<Ref<Model>>& models() { return m_Parts; }
	private:
		std::vector<Ref<Model>> m_Parts;

	};

}

