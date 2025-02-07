#include "C78EPCH.h"
#include "Model.h"

namespace C78E {
	Ref<Model> Model::create(const DrawableMesh& drawableMesh) {
		Ref<Model> model = createRef<Model>();
		model->m_DrawableMeshes.push_back(drawableMesh);
		return model;
	}
}	