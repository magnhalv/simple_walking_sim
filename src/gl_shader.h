#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "types.h"

class GLShader
{
public:
    explicit GLShader(const char* file_name);
    GLShader(GLenum type, const char* text, const char* debug_file_name = "");
    ~GLShader();
    [[nodiscard]] GLenum getType() const { return type_; }
    [[nodiscard]] GLuint getHandle() const { return handle_; }

private:
    GLenum type_;
    GLuint handle_;
};

class GLProgram
{
public:
    void initialize(const GLShader& a);
    void initialize(const GLShader& a, const GLShader& b);
    void initialize(const GLShader& a, const GLShader& b, const GLShader& c);
    void initialize(const GLShader& a, const GLShader& b, const GLShader& c, const GLShader& d, const GLShader& e);
    ~GLProgram();

    void useProgram() const;
    [[nodiscard]] GLuint getHandle() const { return handle_; }

    void set_uniform(const char *name, const glm::vec4 &vec) const;

private:
    GLuint handle_ = 0;
};

GLenum GLShaderTypeFromFileName(const char* file_name);

class GLBuffer
{
public:
    GLBuffer(GLsizeiptr size, const void* data, GLbitfield flags);
    ~GLBuffer();

    [[nodiscard]] GLuint getHandle() const { return handle_; }

private:
    GLuint handle_;
};

