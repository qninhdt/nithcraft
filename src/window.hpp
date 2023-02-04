#pragma once

#include <eventpp/callbacklist.h>
#include "keycode.hpp"

namespace nith
{
    class Window
    {
    public:
        Window(const u32 &width, const u32 &height, const string &title);

        void beginLoop();
        void endLoop();

        bool open();
        void close();
        void closeImmediately();

        GLFWwindow *getNativeWindow() const;

        u32 getWidth() const;
        u32 getHeight() const;
        uvec2 getPos() const;
        f32 getAspect() const;
        bool isClosed() const;

        bool isKeyPressed(const Keycode& key) const;

        vec2 getDeltaMousePos() const;

        ~Window();

    private:
        static void GlfwResizeCallback(GLFWwindow *nativeWindow, int width, int height);
        static void GlfwCloseCallback(GLFWwindow *nativeWindow);
        static void GlfwMouseMoveCallback(GLFWwindow* nativeWindow, double x, double y);

        void updateMouse();

        GLFWwindow *m_nativeWindow;
        u32 m_width;
        u32 m_height;
        uvec2 m_pos;
        vec2 m_mousePos;
        vec2 m_lastMousePos;
        string m_title;
        bool m_isClosed;


        eventpp::CallbackList<void(const Keycode&, const bool&)> m_keyPressedListeners;
        eventpp::CallbackList<void(const Keycode&)> m_keyReleasedListeners;
    };
} // namespace nith
