#include "C78App.h"

#define C78E_ENTRYPOINT
#include <C78E.h>

#include "TestLayer.h"

class C78App : public C78E::Application {

public:

	C78App() :m_AppLayer(C78E::createRef<TestLayer>(this->getWindow())) {
		pushLayer(m_AppLayer);
	}

	C78App(const C78App&) = delete;
	~C78App() = default;

private:
	C78E::Ref<C78E::Layer> m_AppLayer;
};

// Register App in Engine
C78E::Application* C78E::createApplication() { return new C78App(); }
