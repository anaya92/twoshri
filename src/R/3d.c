#include "3d.h"

#include <stdio.h>
#include <glad/gl.h>
#include "_utl/gl.h"

struct
{
    // defaults, no animation, etc.
    unsigned int defaultVertex;
    unsigned int defaultFragment;
    unsigned int defaultProgram;    

    // shader libraries
    unsigned int animationMeshVs; // UNIMPLEMENTED
} Shaders;

float vertices[] = {
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        0.0f,  0.5f, 0.0f   // top 
};

unsigned int vbo, vao;

const char* vs = "\n\
#version 410 core\n\
layout (location = 0) in vec3 pos;\n\
\n\
uniform mat4 model;\n\
\n\
void main()\n\
{\n\
    gl_Position = model * vec4(pos, 1.0);\n\
}";

const char* fs = "\n\
#version 410 core\n\
out vec4 FragColor;\n\
\n\
\n\
void main()\n\
{\n\
    FragColor = vec4(0.2, 0.3, 0.3, 1.0);\n\
}";

unsigned int transformUniform;

void R3d_Init()
{
    Utl_GlCheckpoint();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    printf("loaded VAO,VBO\n");

    Utl_GlCheckpoint();

    Shaders.defaultVertex = Utl_GlCompileShader(vs, GL_VERTEX_SHADER, true);
    Shaders.defaultFragment = Utl_GlCompileShader(fs, GL_FRAGMENT_SHADER, true);
    printf("shaders compiled\n");

    printf("shader 1:%i\n", Shaders.defaultVertex);

    Utl_GlCheckpoint();

    Shaders.defaultProgram = Utl_GlMakeProgram(Shaders.defaultVertex, Shaders.defaultFragment, true);
    printf("shader done. id: %i\n", Shaders.defaultProgram);

    Utl_GlCheckpoint();

    glUseProgram(Shaders.defaultProgram);

    transformUniform = glGetUniformLocation(Shaders.defaultProgram, "model");
    if (transformUniform == -1)
    {
        printf("uniform err!\n");
    }

    printf("uniform@%i\n", transformUniform);

    R3d_SetTransformIdentity();

    Utl_GlCheckpoint();
}

void R3d_Destroy()
{
    glDeleteShader(Shaders.defaultVertex);
    glDeleteShader(Shaders.defaultFragment);

    glDeleteProgram(Shaders.defaultProgram);
}

// void R3d_DefaultState()
// {
//     R3d_Fog(false);
//     R3d_FogSpec(HMM_Vec4(0.7f, 0.7f, 0.7f, 0.7f), 1.0f, 1000.0f);

//     R3d_Lighting(false);
//     R3d_RemoveAllLights();
// }

void R3d_Begin()
{
    glEnable(GL_DEPTH_TEST);
    glUseProgram(Shaders.defaultProgram);

    //Utl_GlCheckpoint();
}

void R3d_End()
{
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
}

// transform functions
static void UpdateModelTransform(float* m0)
{
    glUniformMatrix4fv(glGetUniformLocation(Shaders.defaultProgram, "model"), 1, false, m0);
}

void R3d_SetTransformIdentity()
{
    hmm_mat4 matrix = {1.0f};
    UpdateModelTransform(&matrix.Elements[0][0]);
}

void R3d_SetTransform(R3d_Transform* transform)
{
    hmm_mat4 matrix = (hmm_mat4) {1};
    matrix = HMM_MultiplyMat4(HMM_Translate(transform->position), matrix);
    matrix = HMM_MultiplyMat4(HMM_Scale(transform->scaling), matrix);

    matrix = HMM_MultiplyMat4(HMM_Rotate(transform->rotate.X, HMM_Vec3(1.0f, 0.0f, 0.0f)), matrix);
    matrix = HMM_MultiplyMat4(HMM_Rotate(transform->rotate.Y, HMM_Vec3(0.0f, 1.0f, 0.0f)), matrix);
    matrix = HMM_MultiplyMat4(HMM_Rotate(transform->rotate.Z, HMM_Vec3(0.0f, 0.0f, 1.0f)), matrix);

    UpdateModelTransform(&matrix.Elements[0][0]);
}

void R3d_TestPass()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}