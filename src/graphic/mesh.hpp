#pragma once

namespace nith
{
    enum class Type
    {
        INT,
        UINT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4
    };

    struct MeshAttribute
    {
        Type type;

        MeshAttribute(const Type &type) : type(type) {}
    };

    class Mesh
    {
    public:
        Mesh() : Mesh(false) {}
        Mesh(const bool &useGlobalIBO);

        void setVerticesData(void *data, const u32 &size);

        template <typename T>
        void setVertices(T *vertices, const u32 &count)
        {
            setVerticesData(vertices, count * sizeof(T));
        }

        void setIndices(u32 *indices, const u32 &count);

        void setAttributes(std::initializer_list<MeshAttribute> attributes);

        void drawTriangles() const;

        void drawLines() const;

        void setDrawCount(const u32 &drawCount);

        ~Mesh();

        static void GenerateGlobalIBO();

    private:
        static GLuint ToGLType(const Type &type);
        static u32 GetElementCount(const Type &type);
        static u32 GetTypeSize(const Type &type);

        static GLuint s_globalIBO;

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ibo;
        u32 m_drawCount;
        u32 m_maxBufferSize;
        u32 m_maxIndicesCount;
        bool m_useGlobalIBO;
    };
} // namespace nith
