#include "graphic/mesh.hpp"

namespace nith
{
    GLuint Mesh::s_globalIBO = 0;

    Mesh::Mesh(const bool &useGlobalIBO)
        : m_drawCount(0),
          m_maxBufferSize(0),
          m_maxIndicesCount(0),
          m_useGlobalIBO(useGlobalIBO)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        if (!useGlobalIBO)
        {
            glGenBuffers(1, &m_ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        }
        else
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_globalIBO);
        }
    }

    void Mesh::setVerticesData(void *data, const u32 &size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        if (m_maxBufferSize < size)
        {
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
            m_maxBufferSize = size;
        }
        else
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        }
    }

    void Mesh::setDrawCount(const u32 &drawCount)
    {
        m_drawCount = drawCount;
    }

    void Mesh::drawTriangles() const
    {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, (void *)0);
    }

    void Mesh::drawLines() const
    {
        glBindVertexArray(m_vao);
        glDrawElements(GL_LINES, m_drawCount, GL_UNSIGNED_INT, (void *)0);
    }

    void Mesh::setAttributes(std::initializer_list<MeshAttribute> attributes)
    {
        glBindVertexArray(m_vao);

        u32 index = 0;
        u32 offset = 0;
        u32 stride = 0;

        for (auto &attr : attributes)
            stride += GetTypeSize(attr.type);

        for (auto &attr : attributes)
        {
            u32 elementCount = GetElementCount(attr.type);
            u32 size = GetTypeSize(attr.type);
            GLuint glType = ToGLType(attr.type);

            glEnableVertexAttribArray(index);
            switch (attr.type)
            {
            case Type::INT:
            case Type::UINT:
            {
                glVertexAttribIPointer(index, elementCount, glType, stride, (void *)offset);
                break;
            }
            case Type::VEC4:
            case Type::VEC3:
            case Type::VEC2:
            case Type::FLOAT:
            {
                glVertexAttribPointer(index, elementCount, glType, GL_FALSE, stride, (void *)offset);
                break;
            }
            default:
                break;
            }
            index += 1;
            offset += size;
        }
    }

    void Mesh::setIndices(u32 *indices, const u32 &count)
    {
        glBindVertexArray(m_vao);

        if (m_useGlobalIBO)
            return;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        if (m_maxIndicesCount < count)
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW);
            m_maxIndicesCount = count;
        }
        else
        {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(u32), indices);
        }
    }

    GLuint Mesh::ToGLType(const Type &type)
    {
        switch (type)
        {
        case Type::VEC2:
        case Type::VEC3:
        case Type::VEC4:
        case Type::FLOAT:
            return GL_FLOAT;
        case Type::INT:
            return GL_INT;
        case Type::UINT:
            return GL_UNSIGNED_INT;
        default:
            return 0;
        }
    }

    u32 Mesh::GetElementCount(const Type &type)
    {
        switch (type)
        {
        case Type::VEC2:
            return 2;
        case Type::VEC3:
            return 3;
        case Type::VEC4:
            return 4;
        case Type::INT:
        case Type::UINT:
        case Type::FLOAT:
            return 1;
        default:
            return 0;
        }
    }

    u32 Mesh::GetTypeSize(const Type &type)
    {
        switch (type)
        {
        case Type::VEC2:
            return 8;
        case Type::VEC3:
            return 12;
        case Type::VEC4:
            return 16;
        case Type::INT:
        case Type::UINT:
        case Type::FLOAT:
            return 4;
        default:
            return 0;
        }
    }

    Mesh::~Mesh()
    {
        glDeleteBuffers(1, &m_vbo);

        if (!m_useGlobalIBO)
            glDeleteBuffers(1, &m_ibo);

        glDeleteVertexArrays(1, &m_vao);
    }

    void Mesh::GenerateGlobalIBO()
    {
        glGenBuffers(1, &s_globalIBO);

        constexpr u32 MAX_TRIANGLES = 32 * 32 * 32 * 3;

        std::vector<u32> indices;
        for (u32 i = 0; i < MAX_TRIANGLES; ++i)
        {
            u32 j = i * 6;
            indices.insert(indices.end(), {
                j + 0, j + 1, j + 2,
                j + 0, j + 2, j + 3,
            });
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_globalIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} // namespace nith
