#pragma once
#include <C78E.h>

using namespace C78E; // not great, but im lazy

class C78ESandboxLayer : public C78E::Layer {
public:
    C78ESandboxLayer(C78E::Window& window)
        : Layer(window, "C78ESandboxLayer") {

    }
	virtual ~C78ESandboxLayer() = default;

    void onAttach() {
        C78E_INFO("C78ESandboxLayer attached!");
    }

    void onDetach() {
        C78E_INFO("C78ESandboxLayer detached!");
    }

    void onUpdate(C78E::Timestep delta) override {


    }

    void onEvent(C78E::Event& e) override {
        C78E::EventDispatcher dispatcher(e);
        dispatcher.dispatch<C78E::KeyPressedEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onKeyPressed));
        dispatcher.dispatch<C78E::MouseButtonPressedEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onMouseButtonPressed));
        dispatcher.dispatch<C78E::WindowResizeEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onWindowResize));
    }

    void onKeyPressed(C78E::KeyPressedEvent e) {
        if (e.getKeyCode() == C78E::Input::Key::F11) {
            m_Window.setWindowMode((m_Window.getWindowMode() == C78E::WindowMode::Windowed) ? C78E::WindowMode::FullScreen : C78E::WindowMode::Windowed);
            e.handled = true;
        }
        if(e.getKeyCode() == C78E::Input::Key::Insert) {

            static const C78E::WindowProperties propBase{
                "C78ESandboxWindow",
                C78E::Resolution::resolution(C78E::Resolution::FHD),
                C78E::WindowMode::Windowed,
                C78E::WindowRefreshMode::Unlimited,
                C78E::WindowMouseCursorMode::Hidden //
            };

			C78E::WindowProperties props = propBase;
			props.title += " " + std::to_string(C78E::UUID());
            C78E::Application::get().createWindow(props);
            e.handled = true;
		}
    }

    bool onMouseButtonPressed(C78E::MouseButtonPressedEvent e) {
        return false;
    }

    void onWindowResize(C78E::WindowResizeEvent e) {
        //m_Window.getGraphicsContext().resize(e.getSize());
    }

    virtual void onDebugRender() override {

    }

private:
    // Order of operations:
    // 1. Begin recording
    // (2. Optionally) bind a offscreen framebuffer
    // 3. Begin render pass, either by creating a new one, providing a framebuffer or using an existing render pass
    // 4. Bind a pipeline
    // 5. Bind Shader elements (vertex buffers, index buffers, uniform buffers, storage buffers, storage images)
    //   this entails the correct Descriptors
    // 6. Draw calls (indexed or not)
    // (7. Optionally) Bind another pipeline, and repeat steps
    // 7. End render pass
    // (8. Optionally) bind a different framebuffer or start a new render pass, then repeat steps
    // 8. End recording
	bool setupTestScene() {

		return true;
	}

private:

};