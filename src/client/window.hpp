#pragma once

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

        u32 getPosX() const;

        u32 getPosY() const;

        bool isClosed() const;

        ~Window();

    private:
        static void GlfwResizeCallback(GLFWwindow *nativeWindow, int width, int height);
        static void GlfwCloseCallback(GLFWwindow *nativeWindow);

        GLFWwindow *m_nativeWindow;
        u32 m_width;
        u32 m_height;
        u32 m_posX;
        u32 m_posY;
        string m_title;
        bool m_isClosed;
    };

    NITH_INLINE u32 Window::getWidth() const
    {
        return m_width;
    }

    NITH_INLINE u32 Window::getHeight() const
    {
        return m_height;
    }

    NITH_INLINE u32 Window::getPosX() const
    {
        return m_posX;
    }

    NITH_INLINE u32 Window::getPosY() const
    {
        return m_posY;
    }

    NITH_INLINE bool Window::isClosed() const
    {
        return m_isClosed;
    }
} // namespace nith
