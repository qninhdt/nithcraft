#pragma once

#include "client/resources/shader_manager.hpp"
#include "client/window.hpp"

namespace nith
{
    class NithClient
    {
    public:
        static constexpr u32 WINDOW_WIDTH = 800;
        static constexpr u32 WINDOW_HEIGHT = 600;
        static constexpr char WINDOW_TITLE[] = "Nithcraft";

        NithClient();

        Window &getMainWindow();

        bool initGLFW() const;

        bool initGL() const;

        bool openMainWindow();

        static NithClient &GetClient();

        ShaderManager &getShaderManager()
        {
            return m_shaderManager;
        }

        string getFolderPath() const
        {
            return m_resourceFolderPath;
        }

    private:
        static NithClient *s_client;

        string m_resourceFolderPath;

        Window m_mainWindow;

        ShaderManager m_shaderManager;
    };

    NITH_INLINE NithClient &NithClient::GetClient()
    {
        return *s_client;
    }

    NITH_INLINE Window &NithClient::getMainWindow()
    {
        return m_mainWindow;
    }

} // namespace nith