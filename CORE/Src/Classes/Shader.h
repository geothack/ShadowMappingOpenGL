#pragma once
#include <glad/gl.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "Window.h"

enum class ShaderType
{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1,
    GEOMETRY = 2
};

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
    std::string geometry;
};

class Shader
{
public:


    Shader() = default;

    Shader(const std::string path,Window& window);
    
    // activate the shader
    // ------------------------------------------------------------------------
    void Use();


    inline unsigned int GetId() { return m_shader_id; }

   
    // utility uniform functions
    // ------------------------------------------------------------------------
    void SetBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_shader_id, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void SetInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_shader_id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void SetFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_shader_id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void SetVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(m_shader_id, name.c_str()), 1, &value[0]);
    }
    void SetVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_shader_id, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void SetVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_shader_id, name.c_str()), 1, &value[0]);
    }
    void SetVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_shader_id, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void SetVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_shader_id, name.c_str()), 1, &value[0]);
    }
    void SetVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(m_shader_id, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void SetMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_shader_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void SetMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_shader_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void SetMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_shader_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(GLuint shader, std::string type);

    ShaderSource ParseShader(const std::string file);

    void Create(const char* vertexPath, const char* fragmentPath,Window& window,const char* geometryPath = nullptr);

    unsigned int m_shader_id;

    bool m_is_geometry = false;
    
};
