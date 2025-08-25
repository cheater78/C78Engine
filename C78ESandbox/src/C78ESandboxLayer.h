#pragma once
#include <C78E.h>

using namespace C78E; // not great, but im lazy
using namespace C78E; // not great, but im lazy

class C78ESandboxLayer : public Layer {
public:
    C78ESandboxLayer(Window& window)
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
        dispatcher.dispatch<KeyPressedEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onKeyPressed));
        dispatcher.dispatch<MouseButtonPressedEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onMouseButtonPressed));
        dispatcher.dispatch<WindowResizeEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onWindowResize));
    }

    void onKeyPressed(KeyPressedEvent e) {
        if(e.getKeyCode() == C78E::Input::Key::Insert) {

            static const WindowCreateInfo propBase{
                "C78ESandboxWindow",
                Resolution::resolution(Resolution::nHD),
                WindowMode::Windowed,
                WindowRefreshMode::Unlimited,

            };

            WindowCreateInfo props = propBase;
			props.title += " " + std::to_string(C78E::UUID());
            C78E::Application::get().getWindowSystem().createWindow(props);
            e.handled = true;
            return;
		}
        if (e.getKeyCode() == C78E::Input::Key::Home) {

            const auto& monitors = Application::get().getWindowSystem().getMonitors();
            Ref<Monitor> monitor = monitors[0];
            if (monitors.size() > 1) {
                monitor = monitors[1];
            }

            static const WindowCreateInfo propBase{
                "C78ESandboxWindow",
                Resolution::resolution(Resolution::nHD),
                WindowMode::BorderlessWindow,
                WindowRefreshMode::Unlimited,
                monitor
            };

            WindowCreateInfo props = propBase;
            props.title += " " + std::to_string(C78E::UUID());
            C78E::Application::get().getWindowSystem().createWindow(props);
            e.handled = true;
            return;
        }
        if (e.getKeyCode() == C78E::Input::Key::F1) {
            m_Window.setMouseMode(WindowMouseCursorMode::Hidden);
            return;
        }
        if (e.getKeyCode() == C78E::Input::Key::F2) {
            m_Window.setMouseMode(WindowMouseCursorMode::Normal);
            return;
        }
    }

    bool onMouseButtonPressed(MouseButtonPressedEvent e) {
        return false;
    }

    void onWindowResize(WindowResizeEvent e) {
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
