#include "Instance.h"
#include "vulkanBuffer.h"

#include <stdexcept>
#include <array>
#include <cassert>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <iostream>

namespace C78e {

    VKInstance::VKInstance(int width, int height, string title)
        : vulkWindow(width, height, title),
        vulkDevice(vulkWindow),
        vulkMainRenderer{ vulkDevice, vulkWindow, SwapChain::MAX_FRAMES_IN_FLIGHT } {

        currTime = chrono::high_resolution_clock::now();
    }

    VKInstance::~VKInstance() {  }

    void VKInstance::run(Camera& camera, VulkanObject::Map& objects) {

        glfwPollEvents();
        auto newTime = chrono::high_resolution_clock::now();
        float frameTime = chrono::duration<float, chrono::seconds::period>(newTime - currTime).count();
        lastFrameTime = frameTime;
        currTime = newTime;

        // ANIMATION
        for (auto& entry : objects) {
            auto& obj = entry.second;
            if (obj.pointLight == nullptr) continue;
            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                frameTime * glm::two_pi<float>() * 0.08f,
                { .0f, -1.f, .0f }
            );
            obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.f));
        }


        vulkMainRenderer.render(camera, objects);

       
    }


    void VKInstance::loadObjects(VulkanObject::Map& objects) {

        shared_ptr<Model> model = Model::createModelFromFile(vulkDevice, "../res/models/smooth_vase.obj");

        for (uint32_t i = 0; i < 100; i++){
            auto smoothVase = VulkanObject::createObject();
            smoothVase.model = model;
            smoothVase.transform.translation = { .5f + 0.001f * i, .5f, 0.5f + 0.001f * i };
            smoothVase.transform.scale = glm::vec3{ 2.f, 1.f, 2.f };

            objects.emplace(smoothVase.getID(), move(smoothVase));
        }
        

        auto flatVase = VulkanObject::createObject();
        flatVase.model = Model::createModelFromFile(vulkDevice, "../res/models/flat_vase.obj");
        flatVase.transform.translation = { -.5f, .5f, 0.5f };
        flatVase.transform.scale = glm::vec3{ 2.f, 1.f, 2.f };

        objects.emplace(flatVase.getID(), move(flatVase));

        auto floor = VulkanObject::createObject();
        floor.model = Model::createModelFromFile(vulkDevice, "../res/models/quad.obj");
        floor.transform.translation = { .0f, 0.5f, .0f };
        floor.transform.scale = glm::vec3{ 2.f, 2.f, 2.f };

        floor.textureID = 0;

        objects.emplace(floor.getID(), move(floor));

        auto cube = VulkanObject::createObject();
        cube.model = Model::createModelFromFile(vulkDevice, "../res/models/cube_perFace.obj");
        cube.transform.translation = { -1.5f, -0.001f, 1.5f };
        cube.transform.scale = glm::vec3{ .5f, .5f, .5f };
        
        cube.textureID = 2;

        objects.emplace(cube.getID(), move(cube));

        vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}
        };

        for (int i = 0; i < lightColors.size(); i++) {
            auto pointLight = VulkanObject::makePointLight(1.3f, 0.07f, lightColors[i]);

            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                { .0f, -1.f, .0f }
            );
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            objects.emplace(pointLight.getID(), move(pointLight));
        }

        return;
    }
}