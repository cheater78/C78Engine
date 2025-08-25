#include "C78EApp.h"

#define C78E_ENTRYPOINT
#include <C78E.h>

#include "C78ESandboxLayer.h"

using namespace C78E; // not great, but im lazy
using namespace C78E; // not great, but im lazy

static const WindowCreateInfo appWindowProperties{
	"C78ESandbox",
	Resolution::resolution(Resolution::FHD),
	WindowMode::Windowed,
	WindowRefreshMode::Vsync
};

class C78App : public C78E::Application {
public:
	C78App() : C78E::Application() {
		Window& window1 = getWindowSystem().createWindow(appWindowProperties);
		window1.pushLayer(C78E::createRef<C78ESandboxLayer>(window1));

	}

	C78App(const C78App&) = delete;
	~C78App() = default;

private:
};

// Register App in Engine
C78E::Application* C78E::createApplication() { return new C78App(); }
