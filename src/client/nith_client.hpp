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

        bool initGLFW() const;

        bool initGL() const;

        bool openMainWindow();

        ShaderManager &getShaderManager()
        {
            return m_shaderManager;
        }

        string getFolderPath() const
        {
            return m_resourceFolderPath;
        }

        NITH_INLINE Window &getMainWindow()
        {
            return m_mainWindow;
        }

        static NITH_INLINE NithClient &GetClient()
        {
            return *s_client;
        }

    private:
        static NithClient *s_client;

        string m_resourceFolderPath;

        Window m_mainWindow;

        ShaderManager m_shaderManager;
    };

} // namespace nith