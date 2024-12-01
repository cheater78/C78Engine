#include "C78App.h"

#define C78E_ENTRYPOINT
#include <C78E.h>

#include "AppLayer.h"

class C78App : public C78E::Application {

public:

	C78App() :m_Layer(C78E::createRef<Applayer>(this->getWindow())) {
		pushLayer(m_Layer);
	}

	C78App(const C78App&) = delete;
	~C78App() = default;

private:
	C78E::Ref<C78E::Layer> m_Layer;
};

// Register App in Engine
C78E::Application* C78E::createApplication() { return new C78App(); }