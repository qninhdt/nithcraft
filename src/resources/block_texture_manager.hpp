#pragma once

#include "graphic/texture_array.hpp"

namespace nith
{
    class BlockTextureManager
    {
    public:
        void load();
        void use();

    private:
        TextureArray *m_textureArray;
    };
}