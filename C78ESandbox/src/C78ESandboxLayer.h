#pragma once
#include <C78E.h>

using namespace C78E; // not great, but im lazy

class C78ESandboxLayer : public C78E::Layer {
public:
    C78ESandboxLayer(C78E::Window& window)
        : Layer(window, "C78ESandboxLayer") {
    }
	virtual ~C78ESandboxLayer() = default;

    void onAttach() override {
        C78E_INFO("C78ESandboxLayer attached!");
    }

    void onDetach() override {
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
    }

    bool onMouseButtonPressed(C78E::MouseButtonPressedEvent e) {
        return false;
    }

    void onWindowResize(C78E::WindowResizeEvent e) {
    }

    virtual void onDebugRender() override {
    }

private:

};
