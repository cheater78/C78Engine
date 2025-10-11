#include "C78EApp.h"

#define C78E_ENTRYPOINT
#include <C78E.h>

#include "C78ESandboxLayer.h"

#include "Layers/Vk00HelloTriangle.h"
#include "Layers/Vk01VertexBufferTest.h"
#include "Layers/Vk02UniformBufferTest.h"
#include "Layers/Vk03InstanceBufferTest.h"

static const C78E::WindowProperties appWindowProperties{
	"C78ESandbox",
	C78E::Resolution::resolution(C78E::Resolution::FHD),
	C78E::WindowMode::Windowed,
	C78E::WindowMouseCursorMode::Normal
};

class C78App : public C78E::Application {
public:
	C78App() : C78E::Application() {
		C78E::Window& window1 = createWindow(appWindowProperties);
		window1.pushLayer(C78E::createRef<VkInstanceBufferTest>(window1));
	}

	C78App(const C78App&) = delete;
	~C78App() = default;

private:
};

// Register App in Engine
C78E::Application* C78E::createApplication() { return new C78App(); }
