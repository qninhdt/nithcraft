#pragma once

#include <glad/glad.h>

#include "core/types.hpp"
#include "core/const_string.hpp"

namespace nith
{
    namespace internal
    {
        struct ShaderData
        {
            string vertexPath;
            string fragmentPath;
            string name;
            GLuint programId;
        };

        template <const_string... Uniforms>
        class ShaderBase
        {
        public:
        };

        template <const_string FirstUniform, const_string... Uniforms>
        class ShaderBase<FirstUniform, Uniforms...>
        {
        public:
            void loadUniforms(const GLuint &programId)
            {
                m_uniformLocation = glGetUniformLocation(programId, FirstUniform.value);
                if constexpr (sizeof...(Uniforms) > 0)
                    m_others.loadUniforms(programId);
            }

            template <const_string Name>
            GLint getUniformLocation() const
            {
                if constexpr (FirstUniform == Name)
                    return m_uniformLocation;
                else
                {
                    static_assert(sizeof...(Uniforms) > 0);
                    return m_others.template getUniformLocation<Name>();
                }
            }

        private:
            ShaderBase<Uniforms...> m_others;
            GLint m_uniformLocation;
        };
    }

    template <const_string... Uniforms>
    class Shader : public internal::ShaderBase<Uniforms...>
    {
    public:
        Shader(internal::ShaderData *data) : m_data(data){};

        template <const_string Name, typename T>
        void setUniform(const T &value)
        {
            const GLint location = this->template getUniformLocation<Name>();
            if constexpr (std::is_same_v<T, f32>)
                glUniform1f(location, value);
            else if constexpr (std::is_same_v<T, vec2>)
                glUniform2f(location, value.x, value.y);
            else if constexpr (std::is_same_v<T, vec3>)
                glUniform3f(location, value.x, value.y, value.z);
            else if constexpr (std::is_same_v<T, vec4>)
                glUniform4f(location, value.x, value.y, value.z, value.w);
            else if constexpr (std::is_same_v<T, mat4>)
                glUniformMatrix4fv(location, 1, false, &value[0][0]);
            else
                NITH_STATIC_ERROR("ngu, sai type r!");
        }

        void use() const
        {
            assert(m_data->programId != 0 && "Shader program is not loaded");
            glUseProgram(m_data->programId);
        }

    private:
        internal::ShaderData *m_data;
    };
} // namespace nith
