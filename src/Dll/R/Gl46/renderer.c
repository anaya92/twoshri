#include <renderer.h>
#include <glad/gl.h>

void TsExport(R_Clear)()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TsExport(R_SetFramebufferSize)(int w, int h)
{
    glViewport(0, 0, w, h);
}