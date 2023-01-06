// LOUD MOUNTAIN 2D RENDERER IMPLEMENTATION
// (C) ANAYAE JUL. 28 2022

#include <2d.h>

#include <glad/gl.h>

#include "../../dlcall.h"

// primitives
typedef struct
{
    unsigned int vao;
    unsigned int vbo;
} PrimBuffer;

struct
{
    PrimBuffer quad;
} Primitives;

void LoadPrimitives()
{
    // load quad (the most important)
    Primitives.quad;
}

void UnloadPrimitives()
{
    // unload quad
}

// INIT/DEINIT
void TsExport(R2d_Init)()
{
    LoadPrimitives();
}

void TsExport(R2d_Destroy)()
{
    UnloadPrimitives();
}

// RENDER BEGIN/END
void TsExport(R2d_Begin)()
{
    glEnable(GL_DEPTH_TEST);
}

void TsExport(R2d_End)()
{
    glDisable(GL_DEPTH_TEST);
}

// basic drawing functions
