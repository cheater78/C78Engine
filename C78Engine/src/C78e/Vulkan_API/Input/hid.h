#pragma once

/*

 TEMPORARY CLASS

*/
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace C78e { class Scene; }
using namespace C78e;

class KeyboardMovementController {
public:
    struct KeyMappings {
        int moveLeft = GLFW_KEY_A;
        int moveRight = GLFW_KEY_D;
        int moveForward = GLFW_KEY_W;
        int moveBackward = GLFW_KEY_S;
        int moveUp = GLFW_KEY_SPACE;
        int moveDown = GLFW_KEY_LEFT_SHIFT;
        int pause = GLFW_KEY_ESCAPE;
    };

    void handleInputs(GLFWwindow* window, float dt, Scene& scene);


private:

    void moveInYXZ(GLFWwindow* window, float dt, Scene& scene);

    KeyMappings keys{};
    float moveSpeed{ 3.f };
    float mouseSense{ 0.8f };

    bool winFocused{ true };

};