#pragma once
#include <C78E.h>

class Applayer : public C78E::Layer {
public:
    Applayer(C78E::Window& window)
        : Layer("Applayer"), m_Window(window) {
        
    }

    void onAttach() { }

    void onDetach() { }

    void onUpdate(C78E::Timestep delta) override { }

    bool onEvent(C78E::Event& e) override {
        C78E::EventDispatcher dispatcher(e);
        if(dispatcher.dispatch<C78E::KeyPressedEvent>(BIND_CALLBACK_FN(Applayer::onKeyPressed))) return true;
        if(dispatcher.dispatch<C78E::WindowResizeEvent>(BIND_CALLBACK_FN(Applayer::onWindowResize))) return true;

        return false;
    }

    bool onKeyPressed(C78E::KeyPressedEvent e) {
        return false;
    }

    bool onWindowResize(C78E::WindowResizeEvent e) {
        C78E::RenderCommand::setViewport(0, 0, e.getWidth(), e.getHeight());
        return true;
    }

    void onImGuiRender() override { }

private:
    C78E::Window& m_Window;
};