#include "gl.h"

#include <stdio.h>
#include <glad/gl.h>

static void validateShader(int id)
{
    int success;
    char info[512];

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, NULL, info);
        printf("shader compilation failure:\n%s\n", info);
    }
}

static void validateProgram(int id)
{
    int success;
    char info[512];

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, info);
        printf("program link failure:\n%s\n", info);
    }
}

unsigned int Utl_GlCompileShader(const char* source0, unsigned int type, bool validate)
{
    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, &source0, NULL);
    glCompileShader(shader);

    if (validate) 
    {
        validateShader(shader);
        printf("shader validation complete.\n");
    }

    return shader;
}

unsigned int Utl_GlMakeProgram(int vs, int fs, bool validate)
{
    int program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    // glValidateProgram(program);

    if (validate) 
    {
        validateProgram(program);
        printf("program validation complete.\n");
    }

    return program;
}

void Utl_GlCheckpoint()
{
    int err;
    if ((err = glGetError()) != GL_NO_ERROR)
    {
        printf("checkpoint err 0x%X\n", err);
    }
    else
    {
        printf("checkpoint clear.\n");
    }
}