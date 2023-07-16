#pragma once

#include <glad/gl.h>

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
    explicit GLProgram(const GLShader& a);
    GLProgram(const GLShader& a, const GLShader& b);
    GLProgram(const GLShader& a, const GLShader& b, const GLShader& c);
    GLProgram(const GLShader& a, const GLShader& b, const GLShader& c, const GLShader& d, const GLShader& e);
    ~GLProgram();

    void useProgram() const;
    [[nodiscard]] GLuint getHandle() const { return handle_; }

private:
    GLuint handle_;
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

