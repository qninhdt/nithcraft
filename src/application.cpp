#include "application.hpp"

namespace nith
{
    Application *Application::s_application = nullptr;

    Application::Application():
        m_resourceFolderPath("D:/github/nithcraft/resources/"),
        m_mainWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)
    {
        s_application = this;
    }

    bool Application::initGLFW() const
    {
        if (!glfwInit())
            return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        return true;
    }

    bool Application::initGL() const
    {
        return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }


} // namespace nith
