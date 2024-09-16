#include "C78Editor.h"

#define C78E_ENTRYPOINT
#include <C78E.h>
#include <C78Elibs.h>

#include "Config/WindowConfig.h"
#include "EditorLayer.h"


namespace C78Editor {

	class C78Editor : public C78E::Application {
	public:

		C78Editor(C78E::WindowProps props)
			: Application(props),
			m_EditorLayer(C78E::createRef<EditorLayer>(this->getWindow())) {
			pushLayer(m_EditorLayer);
			getImGuiLayer()->blockEvents(false);
		}

		C78Editor(const C78Editor&) = delete;
		~C78Editor() {
			::C78Editor::WindowConfig::s_LastWindowSize = { getWindow().getWidth(), getWindow().getHeight() };
			::C78Editor::WindowConfig::save();
		}

	private:
		C78E::Ref<C78E::Layer> m_EditorLayer;
	};

}

// Register App in Engine
C78E::Application* C78E::createApplication() {
	::C78Editor::WindowConfig::load();

	C78E::WindowProps props{
		"C78Editor",
		::C78Editor::WindowConfig::s_LastWindowSize.x,
		::C78Editor::WindowConfig::s_LastWindowSize.y
	};

	return new C78Editor::C78Editor(props);
}
