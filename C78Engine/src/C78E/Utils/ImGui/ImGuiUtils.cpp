#include "C78EPCH.h"
#include "ImGuiUtils.h"

namespace C78E::ImGuiImplementation {
    
    void init() {
        switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:
			C78E_ASSERT(false);
			break;
		case RendererAPI::API::OpenGL:
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Application::get().getWindow().getNativeWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 450");
			break;
		case RendererAPI::API::Vulkan:
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Application::get().getWindow().getNativeWindow(), true);
			C78E_ASSERT(false);
			//ImGui_ImplVulkan_Init();
			break;
		default:
			break;
		}
    }

    void newFrame() {
        switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:
			C78E_ASSERT(false);
			break;
		case RendererAPI::API::OpenGL:
			ImGui_ImplOpenGL3_NewFrame();
		    ImGui_ImplGlfw_NewFrame();
			break;
		case RendererAPI::API::Vulkan:
			ImGui_ImplVulkan_NewFrame();
		    ImGui_ImplGlfw_NewFrame();
			break;
		default:
			break;
		}
    }

    void renderDrawData(ImDrawData* draw_data) {
        switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:
			C78E_ASSERT(false);
			break;
		case RendererAPI::API::OpenGL:
			ImGui_ImplOpenGL3_RenderDrawData(draw_data);
			break;
		case RendererAPI::API::Vulkan:
			C78E_ASSERT(false);
            //ImGui_ImplVulkan_RenderDrawData(draw_data, );
			break;
		default:
			break;
		}
    }

}