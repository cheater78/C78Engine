#pragma once
#include <C78e.h>

class TestLayer : public C78e::Layer {
public:
    TestLayer(): Layer("TestLayer") {

    }

    void onUpdate() override {

    }

    void onEvent(C78e::Event& e) override {

    }

    virtual void onImGuiRender() override {
        if (!m_active) return;

    }
private:
    bool m_active = true;
};