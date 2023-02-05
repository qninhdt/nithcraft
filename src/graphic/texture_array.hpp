#pragma once

namespace nith
{
    class TextureArray
    {
    public:
        TextureArray(const u32& width, const u32& height, const u32& layers, const u32& numLevels, const GLenum& internalFormat, const GLenum& format, const GLenum& type);

        void setSubImage(const u32& layer, void* data);

        void bind(const u32& unit) const;

        ~TextureArray();

    private:
        GLuint m_id;
        u32 m_width;
        u32 m_height;
        u32 m_layers;
        u32 m_numLevels;
        GLuint m_type;
        GLuint m_format;
    };

}