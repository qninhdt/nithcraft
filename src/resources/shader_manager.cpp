#include "resources/shader_manager.hpp"
#include "util/file.hpp"

namespace nith
{
    GLuint ShaderManager::loadShaderFromSource(const string &vertexSource, const string &fragmentSource)
    {
        int vertSuccess, fragSuccess, programSuccess;
        char infoLog[512];

        // create vertex shader
        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const GLchar *vertexCSource = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &vertexCSource, NULL);

        // compile vertex shader
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
        if (!vertSuccess)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            // NITH_CLIENT_ERROR("Complie vertex shader [{0}]", m_name);
            // NITH_CLIENT_ERROR("Vertex shader error {0}", infoLog);
            std::cout << infoLog << '\n';
        }

        // create fragment shader
        const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar *fragmentCSource = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentCSource, NULL);

        // compile fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
        if (!fragSuccess)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            // NITH_CLIENT_ERROR("Complie fragment shader [{0}]", m_name);
            // NITH_CLIENT_ERROR("Fragment shader error {0}", infoLog);
            std::cout << infoLog << '\n';
        }

        // create shader program
        GLuint programId = glCreateProgram();

        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);

        glLinkProgram(programId);
        glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);

        if (!programSuccess || !vertSuccess || !fragSuccess)
        {
            glGetProgramInfoLog(programId, 512, NULL, infoLog);
            // NITH_CLIENT_ERROR("Linking shader [{0}] failed", m_name);
            // NITH_CLIENT_ERROR("Shader program error: {0}", infoLog);
            std::cout << infoLog << '\n';
            return false;
        }

        // delete shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // loadUniformLocations();

        return programId;
    }

    void ShaderManager::loadShaderFromFile(const string &vertexPath, const string &fragmentPath)
    {
        string vertexSource;
        string fragmentSource;
        string name0 = util::GetFileNameFromPath(vertexPath);
        string name1 = util::GetFileNameFromPath(fragmentPath);

        assert(name0 == name1);

        auto data = new internal::ShaderData();
        data->name = name0;
        data->vertexPath = vertexPath;
        data->fragmentPath = fragmentPath;

        if (!util::ReadFile(vertexPath, vertexSource))
        {
            std::cout << "Vertex not found\n";
        }

        if (!util::ReadFile(fragmentPath, fragmentSource))
        {
            std::cout << "Fragment not found\n";
        }
        data->programId = loadShaderFromSource(vertexSource, fragmentSource);

        m_shaders[data->name] = data;
    }

    void ShaderManager::reloadShader(const string &name)
    {
        auto &shaderData = *m_shaders[name];

        string vertexSource;
        string fragmentSource;

        if (!util::ReadFile(shaderData.vertexPath, vertexSource))
        {
            std::cout << "Vertex not found\n";
        }

        if (!util::ReadFile(shaderData.fragmentPath, fragmentSource))
        {
            std::cout << "Fragment not found\n";
        }

        shaderData.programId = loadShaderFromSource(vertexSource, fragmentSource);
    }
} // namespace nith
