#pragma once

#include <C78E.h>
#include <C78Egui.h>
#include <C78Elibs.h>

namespace C78Editor {

    static constexpr float c_vMouseSense = 0.1f;
    static constexpr float c_hMouseSense = 0.1f;
    static constexpr float c_vMoveSpeed = 2.0f;
    static constexpr float c_hMoveSpeed = 2.0f;

    static constexpr float c_BoostSpeed = 4.0f;
    static constexpr float c_SlowSpeed = 0.25f;

    void update(C78E::Entity camera, C78E::Timestep dt) {
        auto& cameraTransform = camera.getComponent<C78E::TransformComponent>();

        auto& pos = cameraTransform.Translation;
        auto& rot = cameraTransform.Rotation;

        const glm::vec3 dirRight = -glm::normalize(cameraTransform.getTransform()[0]);
        const glm::vec3 dirUp = glm::normalize(cameraTransform.getTransform()[1]);
        const glm::vec3 dirForward = glm::normalize(cameraTransform.getTransform()[2]);

        glm::vec3 posChg = { 0.f, 0.f, 0.f };
        glm::vec3 rotChg = { 0.f, 0.f, 0.f };


        const glm::vec3 dirRight = glm::normalize(glm::vec3(cameraTransform.getTransform()[0]) * glm::vec3{ 1.f, 0.f, 1.f });
        const glm::vec3 dirUp = { 0.f, 1.f, 0.f };
        const glm::vec3 dirForward = glm::normalize(glm::vec3(cameraTransform.getTransform()[2]) * glm::vec3{ 1.f, 0.f, 1.f });
        const glm::vec3 pos = cameraTransform.getTransform()[3];

        float vMouseSense = c_vMouseSense;
        float hMouseSense = c_hMouseSense;
        float vMoveSpeed = c_vMoveSpeed;
        float hMoveSpeed = c_hMoveSpeed;

        if (C78E::Input::isKeyPressed(C78E::Key::LeftShift)) {
            vMouseSense *= c_BoostSpeed;
            hMouseSense *= c_BoostSpeed;
            vMoveSpeed *= c_BoostSpeed;
            hMoveSpeed *= c_BoostSpeed;
        }
        if (C78E::Input::isKeyPressed(C78E::Key::LeftAlt)) {
            vMouseSense *= c_SlowSpeed;
            hMouseSense *= c_SlowSpeed;
            vMoveSpeed *= c_SlowSpeed;
            hMoveSpeed *= c_SlowSpeed;
        }

        //TODO : C78E Input -> no hard code
        rotChg = glm::vec3(0.f, C78E::Input::getMouseX() * dt.getSeconds() * hMouseSense, 0.f)
            + glm::vec3(C78E::Input::getMouseY() * dt.getSeconds() * vMouseSense, 0.f, 0.f);
        C78E::Input::setMousePosition(0, 0);

        // Limit to look straight up or down, no upside down view
        if ((rot + rotChg).x < -glm::half_pi<float>()) rotChg.x = -glm::half_pi<float>() - rot.x;
        if ((rot + rotChg).x > +glm::half_pi<float>()) rotChg.x = +glm::half_pi<float>() - rot.x;

        if (C78E::Input::isKeyPressed(C78E::Key::W)) { posChg += dirForward * (float)dt.getSeconds() * hMoveSpeed; }
        if (C78E::Input::isKeyPressed(C78E::Key::S)) { posChg -= dirForward * (float)dt.getSeconds() * hMoveSpeed; }
        if (C78E::Input::isKeyPressed(C78E::Key::A)) { posChg += dirRight * (float)dt.getSeconds() * hMoveSpeed; }
        if (C78E::Input::isKeyPressed(C78E::Key::D)) { posChg -= dirRight * (float)dt.getSeconds() * hMoveSpeed; }
        if (C78E::Input::isKeyPressed(C78E::Key::Space)) { posChg += dirUp * (float)dt.getSeconds() * vMoveSpeed; }
        if (C78E::Input::isKeyPressed(C78E::Key::LeftControl)) { posChg -= dirUp * (float)dt.getSeconds() * vMoveSpeed; }


        pos += posChg;
        rot += rotChg;
    }

}