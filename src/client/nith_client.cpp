#include "client/nith_client.hpp"

namespace nith
{
    NithClient *NithClient::s_client = nullptr;

    NithClient::NithClient()
        : m_resourceFolderPath("D:/github/nithcraft/resources/"),
          m_mainWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)
    {
        s_client = this;
    }

    bool NithClient::initGLFW() const
    {
        if (!glfwInit())
            return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        return true;
    }

    bool NithClient::initGL() const
    {
        return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

} // namespace nith
