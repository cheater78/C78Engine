#include <C78E/Core/Window/Window.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <C78E/Utils/ImGui/TextureHandle.h>

#pragma warning(push, 0)
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_vulkan.h>
#pragma warning(pop)

namespace C78E::ImGuiImplementation {

    //TODO:: Platform Abstraction

    void init(Window& window);

    void newFrame();

    void renderDrawData(ImDrawData* draw_data);

}
