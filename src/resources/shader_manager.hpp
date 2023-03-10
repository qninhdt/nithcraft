#pragma once

#include "graphic/shader.hpp"

namespace nith
{
    using BasicShader = Shader<"mvp">;

    class ShaderManager
    {
    public:
        void loadShaders();

        void loadShaderFromFile(const string &vertexPath, const string &fragmentPath);

        void reloadShader(const string &name);

        template <typename T>
        T getShader(const string &name)
        {
            T shader{m_shaders[name]};
            shader.use();
            shader.loadUniforms(m_shaders[name]->programId);
            return shader;
        }

    private:
        GLuint loadShaderFromSource(const string &vertexSource, const string &fragmentSource);
        bool loadShaderSources(string &vertexSource, string &fragmentSource);

        umap<string, internal::ShaderData *> m_shaders;
    };

} // namespace nith::graphic
