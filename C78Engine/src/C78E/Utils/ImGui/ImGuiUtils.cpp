#include "C78EPCH.h"
#include "ImGuiUtils.h"

namespace C78E::ImGuiImplementation {
    
    void init(Window& window) {
        //ImGui_ImplGlfw_InitForVulkan(window.getNativeWindowAs<GLFWwindow>(), false);
    }

    void newFrame() {
        //ImGui_ImplGlfw_NewFrame();
    }

    void renderDrawData(ImDrawData* draw_data) {
        //ImGui_ImplVulkan_RenderDrawData(draw_data, );
    }

}
