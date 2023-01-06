#include <renderer.h>
#include <3d.h>

#include <stdio.h>
#include <glad/gl.h>
#include "_utl/gl.h"

#include "../../dlcall.h"

#define WORLD_UP (hmm_vec3) { 0.0f, 1.0f, 0.0f }

struct
{
    // defaults, no animation, etc.
    unsigned int defaultVertex;
    unsigned int defaultFragment;
    unsigned int defaultProgram;
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
layout (std140) uniform Camera\n\
{\n\
    mat4 view;\n\
    mat4 projection;\n\
};\n\
\n\
void main()\n\
{\n\
    gl_Position = projection * view * model * vec4(pos, 1.0);\n\
}";

const char* fs = "\n\
#version 410 core\n\
out vec4 FragColor;\n\
\n\
\n\
void main()\n\
{\n\
    FragColor = vec4(0.2, 0.6, 0.4, 1.0);\n\
}";

// MODEL MATRIX GLOBALS
unsigned int modelUniform;

// CAMERA GL GLOBALS
struct
{
    unsigned int id;
} CameraUbo; // UBO DATA ORDER GOES VIEW THEN PROJECTION
void CreateCameraBuffers(); // forward defs for R3d_Create();
void DestroyCameraBuffers();

// WORLD GL GLOBALS
// skybox
struct 
{
    bool enabled;
    unsigned int cubemapId, programId;
    Utl_GlBuffer buffer;

    struct
    {
        const char *vs, *fs;
    } shaderData;
} Skybox =  {
    .enabled = false,
    .cubemapId = 0,
    .programId = 0,
    .buffer = { 0 },

    .shaderData = {
        .vs = "\n\
            #version 410 core\n\
            layout (location = 0) in vec3 pos;\n\
            out vec3 uv;\n\
            layout (std140) uniform Camera\n\
            {\n\
                mat4 view;\n\
                mat4 projection;\n\
            };\n\
            \n\
            void main()\n\
            {\n\
                uv = pos;\n\
                vec4 pos = projection * mat4(mat3(view)) * vec4(pos, 1.0);\n\
                gl_Position = pos.xyww;\n\
            }",
        .fs = "\n\
            #version 410 core\n\
            out vec4 FragColor;\n\
            in vec3 uv;\n\
            uniform samplerCube skybox;\n\
            \n\
            void main()\n\
            {\n\
                FragColor = texture(skybox, uv);\n\
            }"
    }
};
void CreateSkyboxResources();
void DestroySkyboxResources();

// INIT/CLEANUP
// ------------
void TsExport(R3d_Create)()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shaders.defaultVertex = Utl_GlCompileShader(vs, GL_VERTEX_SHADER, true);
    Shaders.defaultFragment = Utl_GlCompileShader(fs, GL_FRAGMENT_SHADER, false);
    Shaders.defaultProgram = Utl_GlMakeProgram(Shaders.defaultVertex, Shaders.defaultFragment, false);

    glUseProgram(Shaders.defaultProgram);
    glUniformBlockBinding(Shaders.defaultProgram, glGetUniformBlockIndex(Shaders.defaultProgram, "Camera"), 0);

    modelUniform = glGetUniformLocation(Shaders.defaultProgram, "model");
    if (modelUniform == -1)
    {
        printf("uniform err!\n");
    }

    // TODO: SMOOTHEN CREATION OF SHADER PROGRAMS
    CreateCameraBuffers();
    CreateSkyboxResources();
}

void TsExport(R3d_Destroy)()
{
    DestroySkyboxResources();
    DestroyCameraBuffers();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

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

// RENDER START/END
// ----------------
void TsExport(R3d_Begin)()
{
    glEnable(GL_DEPTH_TEST);
    glUseProgram(Shaders.defaultProgram);
}

void TsExport(R3d_End)()
{
    glUseProgram(0);

    if (Skybox.enabled)
    {
        glDepthFunc(GL_LEQUAL);
        glUseProgram(Skybox.programId);
        glBindVertexArray(Skybox.buffer.vao);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox.cubemapId);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
        glUseProgram(0);
    }
    glDisable(GL_DEPTH_TEST);
}

// transform functions
// -------------------
void UpdateModelTransform(float* m0)
{
    modelUniform = glGetUniformLocation(Shaders.defaultProgram, "model");
    if (modelUniform == -1) printf("no UNIFORM!!!\n");
    glUniformMatrix4fv(modelUniform, 1, false, m0);
}

void TsExport(R3d_SetTransformIdentity)()
{
    hmm_mat4 matrix = HMM_Mat4d(1.0f);
    UpdateModelTransform(&matrix.Elements[0][0]);
}

void TsExport(R3d_SetTransform)(R3d_Transform* transform)
{
    hmm_mat4 matrix = HMM_Mat4d(1.0f);

    // ORDER MATTERS! S * R * T
    matrix = HMM_MultiplyMat4(HMM_Scale(transform->scaling), matrix);

    transform->rotate.X = fmodf(transform->rotate.X, 360.0f);
    transform->rotate.Y = fmodf(transform->rotate.Y, 360.0f);
    transform->rotate.Z = fmodf(transform->rotate.Z, 360.0f);

    matrix = HMM_MultiplyMat4(HMM_Rotate(transform->rotate.X, HMM_Vec3(1.0f, 0.0f, 0.0f)), matrix);
    matrix = HMM_MultiplyMat4(HMM_Rotate(transform->rotate.Y, HMM_Vec3(0.0f, 1.0f, 0.0f)), matrix);
    matrix = HMM_MultiplyMat4(HMM_Rotate(transform->rotate.Z, HMM_Vec3(0.0f, 0.0f, 1.0f)), matrix);

    matrix = HMM_MultiplyMat4(HMM_Translate(transform->position), matrix);

    UpdateModelTransform(&matrix.Elements[0][0]);
}

// CAMERA FUNCTIONS
// ----------------
void CreateCameraBuffers()
{
    hmm_mat4 defaultData[2] = { HMM_Mat4d(1.0f), HMM_Mat4d(1.0f) };

    glGenBuffers(1, &CameraUbo.id);

    glBindBuffer(GL_UNIFORM_BUFFER, CameraUbo.id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(hmm_mat4) * 2, &defaultData[0], GL_STATIC_DRAW); // load identity matrices as default
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, CameraUbo.id);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void DestroyCameraBuffers()
{
    glDeleteBuffers(1, &CameraUbo.id);
}

void UpdateCameraUniform(hmm_mat4* view, hmm_mat4* proj)
{
    glBindBuffer(GL_UNIFORM_BUFFER, CameraUbo.id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(hmm_mat4), &view->Elements[0][0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(hmm_mat4), sizeof(hmm_mat4), &proj->Elements[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void TsExport(R3d_SetCameraIdentity)()
{
    hmm_mat4 defaultData[2] = { HMM_Mat4d(1.0f), HMM_Mat4d(1.0f) };
    UpdateCameraUniform(&defaultData[0], &defaultData[1]);
}

void TsExport(R3d_SetCameraDefault)() // identity CAMERA, so a position (0,0,0) 1-1000z perspective projection is created
{
    hmm_mat4 view = HMM_LookAt((hmm_vec3){0.0f, 0.0f, 0.0f},(hmm_vec3){0.0f, 0.0f, 1.0f}, WORLD_UP);
    hmm_mat4 proj = HMM_Perspective(90.0 / 2, 4/3, 1.0f, 1000.0f);

    // UBOS R IMPLEMENTED!!! SO VAMOS!!
    UpdateCameraUniform(&view, &proj);
}

void TsExport(R3d_SetCamera)(R3d_Camera* camera)
{
    // transform.rotate.x | y | z is pitch | yaw | roll
    hmm_vec3 direction = { 0.0f };
    direction.X = HMM_CosF(HMM_ToRadians(camera->transform.rotate.Y)) * HMM_CosF(HMM_ToRadians(camera->transform.rotate.X));
    direction.Y = HMM_SinF(HMM_ToRadians(camera->transform.rotate.X));
    direction.Z = HMM_SinF(HMM_ToRadians(camera->transform.rotate.Y)) * HMM_CosF(HMM_ToRadians(camera->transform.rotate.X));
    
    hmm_vec3 front = HMM_NormalizeVec3(direction);

    hmm_vec3 up = WORLD_UP; // TODO: IMPLEMENT CAMERA ROLLING.

    hmm_mat4 view = HMM_LookAt(camera->transform.position, HMM_AddVec3(camera->transform.position, front), up); // rotate some of this stuff
    hmm_mat4 proj = HMM_Perspective(camera->fov / 2, 16/9, 1.0f, 1000.0f);

    UpdateCameraUniform(&view, &proj);
}

// WORLD FUNCTIONS
// ---------------

// skybox functions
void CreateSkyboxResources()
{
    int skyboxVs = Utl_GlCompileShader(Skybox.shaderData.vs, GL_VERTEX_SHADER, true);
    int skyboxFs = Utl_GlCompileShader(Skybox.shaderData.fs, GL_FRAGMENT_SHADER, true);
    Skybox.programId = Utl_GlMakeProgram(skyboxVs, skyboxFs, true);

    glUseProgram(Skybox.programId);
    glUniformBlockBinding(Skybox.programId, glGetUniformBlockIndex(Skybox.programId, "Camera"), 0);

    glDeleteShader(skyboxVs);
    glDeleteShader(skyboxFs);

    #include "data.h"
    Skybox.buffer = Utl_GlCreateBuffer((void*)skyboxVertices, sizeof(skyboxVertices), NULL, 0, 0);
}

void DestroySkyboxResources()
{
    glDeleteTextures(1, &Skybox.cubemapId); // unloads cubemap when passed null
    glDeleteProgram(Skybox.programId);
    Utl_GlFreeBuffer(Skybox.buffer);
}

void TsExport(R3d_LoadSkybox)(R_ImageSpec dataArray[6])
{    
    glDeleteTextures(1, &Skybox.cubemapId);
    if (!dataArray) return;

    glGenTextures(1, &Skybox.cubemapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox.cubemapId);

    for(unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGB, dataArray[i].w, dataArray[i].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataArray[i].data
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
}

void TsExport(R3d_DrawSkybox)(bool enabled)
{
    Skybox.enabled = enabled;
}

// DRAWING
// -------
void TsExport(R3d_TestPass)()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}