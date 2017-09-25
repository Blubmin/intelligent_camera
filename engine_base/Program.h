#pragma once

#include <map>
#include <string>

#include <GL\glew.h>

class Program
{
public:
    Program();
    Program(const std::string& vertexShader, const std::string& fragShader);
    Program(GLuint vert_size, const GLchar* vert_buffer, GLuint frag_size, const GLchar* frag_buffer);
    ~Program();

    GLint getAttributeHandle(std::string name);
    GLint uniform(std::string name);

    GLuint prog;
    GLuint vert;
    GLuint frag;
    std::map<std::string, GLuint> attributes;
    std::map<std::string, GLuint> uniforms;

private:
    GLuint compileShader(const GLchar *contents, const GLint* length, GLenum shaderType, GLint* rc);
    void setupHandles();
};