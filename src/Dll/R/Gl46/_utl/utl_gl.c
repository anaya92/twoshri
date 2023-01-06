#include "gl.h"

#include <stdio.h>
#include <glad/gl.h>

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

// SHADER STUFF
static void validateShader(int id)
{
    int success;
    char info[512];

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, NULL, info);
        printf("[R/_utl/gl] shader compilation failure:\n%s\n", info);
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
        printf("[R/_utl/gl] program link failure:\n%s\n", info);
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
        printf("[R/_utl/gl] shader validation complete.\n");
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
        printf("[R/_utl/gl] program validation complete.\n");
    }

    return program;
}

// SHADER STUFF
Utl_GlBuffer Utl_GlCreateBuffer(void* vertexData, int vcount, void* indexData, int icount, Utl_GlBufferFlags flags)
{
    Utl_GlBuffer val;

    if (!vertexData) return (Utl_GlBuffer) {.vbo=0,.ibo=0,.vao=0};

    glGenVertexArrays(1, &val.vao);
    glBindVertexArray(val.vao);

    Utl_GlCheckpoint();

    glGenBuffers(1, &val.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, val.vbo);
    glBufferData(GL_ARRAY_BUFFER, vcount, vertexData, GL_STATIC_DRAW);

    Utl_GlCheckpoint();

    if (indexData)
    {
        printf("detected index data\n");
        glGenVertexArrays(1, &val.ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, val.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount, indexData, GL_STATIC_DRAW);
    }
    else
    {
        val.ibo = 0;
    }

    int vertexAttribCount = 0;
    int offset = 0;
    int stride = 3 * sizeof(float);

    if (!flags) // just vertexes, no accompanying data
    {
        glVertexAttribPointer(vertexAttribCount, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(0);
        return val;
    }

    if (flags & UTL_GLBUFFER_HAS_NORMALS)
    {
        stride += 3 * sizeof(float);
    }
    else if (flags & UTL_GLBUFFER_HAS_COLOR)
    {
        stride += 3 * sizeof(float);
    }
    else if (flags & UTL_GLBUFFER_HAS_UV)
    {
        stride += 2 * sizeof(float);
    }

    glVertexAttribPointer(vertexAttribCount, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(0);

    offset += 3 * sizeof(float);

    if (flags & UTL_GLBUFFER_HAS_NORMALS)
    {
        vertexAttribCount++;
        glVertexAttribPointer(vertexAttribCount, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(vertexAttribCount);  
        offset += 3 * sizeof(float);
    }

    if (flags & UTL_GLBUFFER_HAS_COLOR)
    {
        vertexAttribCount++;
        glVertexAttribPointer(vertexAttribCount, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(vertexAttribCount);  
        offset += 3 * sizeof(float);
    }

    if (flags & UTL_GLBUFFER_HAS_UV)
    {
        vertexAttribCount++;
        glVertexAttribPointer(vertexAttribCount, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(vertexAttribCount);  
        offset += 2 * sizeof(float);
    }
}

void Utl_GlFreeBuffer(Utl_GlBuffer buffer)
{
    glDeleteVertexArrays(1, &buffer.vao);
    glDeleteBuffers(1, &buffer.vbo);
    glDeleteBuffers(1, &buffer.ibo);
}