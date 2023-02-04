#include "debug_layer.hpp"

namespace nith
{
    DebugLayer::DebugLayer(Window& window):
        m_window(&window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_io = &ImGui::GetIO();

        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        ImGui_ImplGlfw_InitForOpenGL(m_window->getNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }
    
    void DebugLayer::render()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug");                        
        ImGui::Text("Info: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window->getNativeWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    DebugLayer::~DebugLayer()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}