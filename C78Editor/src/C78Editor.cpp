#include "C78Editor.h"

#define C78E_ENTRYPOINT
#include <C78E.h>
#include <C78Elibs.h>

#include "EditorLayer.h"

namespace C78Editor {

	class C78Editor : public C78E::Application {

	public:

		C78Editor() :m_EditorLayer(C78E::createRef<EditorLayer>(this->getWindow())) {
			pushLayer(m_EditorLayer);
		}

		C78Editor(const C78Editor&) = delete;
		~C78Editor() = default;

	private:
		C78E::Ref<C78E::Layer> m_EditorLayer;
	};

}

// Register App in Engine
C78E::Application* C78E::createApplication() { return new C78Editor::C78Editor(); }
