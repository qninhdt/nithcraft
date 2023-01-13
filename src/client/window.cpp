#include "client/window.hpp"
#include "client/nith_client.hpp"

namespace nith
{
    Window::Window(const u32 &width, const u32 &height, const string &title)
        : m_width(width),
          m_height(height),
          m_title(title),
          m_isClosed(false)
    {
    }

    bool Window::open()
    {
        NithClient::GetClient().initGLFW();

        m_nativeWindow = glfwCreateWindow((i32)m_width, (i32)m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_nativeWindow)
        {
            // error handling
            return false;
        }

        glfwMakeContextCurrent(m_nativeWindow);

        NithClient::GetClient().initGL();

        glfwSetWindowUserPointer(m_nativeWindow, this);

        i32 posX, posY;
        glfwGetWindowPos(m_nativeWindow, &posX, &posY);
        m_posX = posX;
        m_posY = posY;

        glClearColor(0, 0, 0, 0);

        glfwSetWindowSizeCallback(m_nativeWindow, GlfwResizeCallback);
        glfwSetWindowCloseCallback(m_nativeWindow, GlfwCloseCallback);

        return true;
    }

    void Window::beginLoop()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::endLoop()
    {
        glfwSwapBuffers(m_nativeWindow);
        glfwPollEvents();
    }

    void Window::close()
    {
        m_isClosed = true;
    }

    void Window::closeImmediately()
    {
    }

    void Window::GlfwResizeCallback(GLFWwindow *nativeWindow, int width, int height)
    {
        Window &window = *(Window *)glfwGetWindowUserPointer(nativeWindow);
    }

    void Window::GlfwCloseCallback(GLFWwindow *nativeWindow)
    {
        Window &window = *(Window *)glfwGetWindowUserPointer(nativeWindow);
        window.m_isClosed = true;
    }

    Window::~Window()
    {
        if (!m_isClosed)
            closeImmediately();
    }
} // namespace nith
