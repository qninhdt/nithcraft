#include "window.hpp"
#include "application.hpp"

namespace nith
{
    Window::Window(const u32 &width, const u32 &height, const string &title):
        m_width(width),
        m_height(height),
        m_mousePos(0, 0),
        m_title(title),
        m_isClosed(false),
        m_enableCursor(true)
    {
        updateMouse();
        m_lastMousePos = m_mousePos;
    }

    bool Window::open()
    {
        Application::Get().initGLFW();

        m_nativeWindow = glfwCreateWindow((i32)m_width, (i32)m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_nativeWindow)
        {
            // error handling
            return false;
        }

        glfwMakeContextCurrent(m_nativeWindow);

        Application::Get().initGL();

        glfwSetWindowUserPointer(m_nativeWindow, this);

        glClearColor(1, 1, 1, 1);

        glfwSetWindowSizeCallback(m_nativeWindow, GlfwResizeCallback);
        glfwSetWindowCloseCallback(m_nativeWindow, GlfwCloseCallback);
        glfwSetCursorPosCallback(m_nativeWindow, GlfwMouseMoveCallback);
        glfwSetKeyCallback(m_nativeWindow, GlfwKeyPressedCallback);

        return true;
    }

    void Window::beginLoop()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::endLoop()
    {
        glfwSwapBuffers(m_nativeWindow);
        glfwPollEvents();

        updateMouse();
    }

    void Window::close()
    {
        m_isClosed = true;
    }

    void Window::closeImmediately()
    {
    }

    GLFWwindow* Window::getNativeWindow() const
    {
        return m_nativeWindow;
    }

    void Window::GlfwResizeCallback(GLFWwindow *nativeWindow, int width, int height)
    {
        Window &window = *(Window *)glfwGetWindowUserPointer(nativeWindow);

        window.m_height = height;
        window.m_width = width;

        glViewport(0, 0, width, height);
    }

    void Window::GlfwCloseCallback(GLFWwindow *nativeWindow)
    {
        Window& window = *(Window*)glfwGetWindowUserPointer(nativeWindow);
        window.m_isClosed = true;
    }

    void Window::GlfwMouseMoveCallback(GLFWwindow* nativeWindow, double x, double y)
    {
        Window& window = *(Window*)glfwGetWindowUserPointer(nativeWindow);

    }

    void Window::GlfwKeyPressedCallback(GLFWwindow* nativeWindow, int key, int scancode, int action, int mods)
    {
        Window& window = *(Window*)glfwGetWindowUserPointer(nativeWindow);

        if (action == GLFW_PRESS)
            window.m_keyPressedListeners((Keycode)key, false);
        else if (action == GLFW_REPEAT)
            window.m_keyPressedListeners((Keycode)key, true);
        else
            window.m_keyReleasedListeners((Keycode)key);
    }

    void Window::updateMouse()
    {
        //i32 posX, posY;
        //glfwGetWindowPos(m_nativeWindow, &posX, &posY);
        //m_pos = { posX, posY };

        double x, y;
        glfwGetCursorPos(m_nativeWindow, &x, &y);
        m_lastMousePos = m_mousePos;
        m_mousePos = { f32(x), f32(y) };

        if (m_lastMousePos.x == 0 && m_lastMousePos.y == 0)
            m_lastMousePos = m_mousePos;
    }

    u32 Window::getWidth() const
    {
        return m_width;
    }

    u32 Window::getHeight() const
    {
        return m_height;
    }

    uvec2 Window::getPos() const
    {
        return m_pos;
    }

    f32 Window::getAspect() const
    {
        return 1.0f * m_width / m_height;
    }

    bool Window::isClosed() const
    {
        return m_isClosed;
    }

    bool Window::isKeyPressed(const Keycode& key) const
    {
        return glfwGetKey(m_nativeWindow, (i32) key) == GLFW_PRESS;
    }

    void Window::toggleCursor()
    {
        m_enableCursor = !m_enableCursor;

        if (m_enableCursor)
            glfwSetInputMode(m_nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(m_nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    bool Window::isEnableCursor() const
    {
        return m_enableCursor;
    }

    vec2 Window::getDeltaMousePos() const
    {
        return m_mousePos - m_lastMousePos;
    }

    void Window::onKeyPressed(std::function<void(const Keycode&, const bool&)> callback)
    {
        m_keyPressedListeners.append(callback);
    }

    Window::~Window()
    {
        if (!m_isClosed)
            closeImmediately();
    }
} // namespace nith
