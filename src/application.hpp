#pragma once

#include "resources/shader_manager.hpp"
#include "resources/block_texture_manager.hpp"
#include "window.hpp"
#include "camera.hpp"

namespace nith
{
    class Application
    {
    public:
        static constexpr u32 WINDOW_WIDTH = 1600;
        static constexpr u32 WINDOW_HEIGHT = 1200;
        static constexpr char WINDOW_TITLE[] = "Nithcraft";

        Application();

        bool initGLFW() const;

        bool initGL() const;

        bool openMainWindow();

        void loadResources();

        ShaderManager &getShaderManager()
        {
            return m_shaderManager;
        }

        BlockTextureManager& getBlockTextureManager()
        {
            return m_blockTextureManager;
        }

        string getFolderPath() const
        {
            return m_resourceFolderPath;
        }

        Window &getMainWindow()
        {
            return m_mainWindow;
        }

        static Application &Get()
        {
            return *s_application;
        }

    private:
        static Application *s_application;

        string m_resourceFolderPath;
        Window m_mainWindow;
        ShaderManager m_shaderManager;
        BlockTextureManager m_blockTextureManager;

    };

} // namespace nith