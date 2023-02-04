#pragma once

#include "window.hpp"

namespace nith
{
    class DebugLayer
    {
    public:
        DebugLayer(Window& window);
        ~DebugLayer();

        void render();
    private:
        Window* m_window;
        ImGuiIO* m_io;
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };
}