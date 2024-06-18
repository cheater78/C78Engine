#include "C78Editor.h"

#define C78E_ENTRYPOINT
#include <C78E.h>
#include <C78Elibs.h>

#include "Config/WindowConfig.h"
#include "EditorLayer.h"


namespace C78Editor {

	class C78Editor : public C78E::Application {
	public:

		C78Editor() : Application({"C78Editor", 1920, 1080 }), m_EditorLayer(C78E::createRef<EditorLayer>(this->getWindow())) {
			pushLayer(m_EditorLayer);
		}

		C78Editor(const C78Editor&) = delete;
		~C78Editor() = default;

	private:
		C78E::Ref<C78E::Layer> m_EditorLayer;
	};

}

// Register App in Engine
C78E::Application* C78E::createApplication() {
	C78Editor::WindowConfig::load(); //TODO: load real Window settings
	return new C78Editor::C78Editor();
}
