#include "resources/block_texture_manager.hpp"
#include "application.hpp"
#include "stb_image.h"

namespace nith
{
    void BlockTextureManager::load()
    {
        string folderPath = Application::Get().getFolderPath() + "blocks/";
        vector<string> blockTextures = {
            "grass_top",
            "grass_bottom",
            "grass_side",
            "dirt",
        };

        m_textureArray = new TextureArray(16, 16, blockTextures.size(), 4, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

        i32 h, w, n;
        stbi_set_flip_vertically_on_load(true);

        for (u32 layer = 0; layer < blockTextures.size(); ++layer)
        {
            string filePath = folderPath + blockTextures[layer] + ".png";
            u8* data = stbi_load(filePath.c_str(), &h, &w, &n, STBI_rgb_alpha);
            m_textureArray->setSubImage(layer, data);
            stbi_image_free(data);
        }
    }

    void BlockTextureManager::use()
    {
        m_textureArray->bind(0);
    }
}