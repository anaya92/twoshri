#include "renderer.h"

#include "../Sys/screen.h"

#include <stdio.h>

#include <SDL2/SDL.h>

#include <glad/gl.h>

void R_Init()
{
    SDL_GL_MakeCurrent(Sys_GetWndHandle(), Sys_GetGfxDevice());

    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    if (version == 0)
    {
        printf("opengl failed to load!\n");
    }

    printf("renderer started: opengl %s\n", glGetString(GL_VERSION));
}

void R_Destroy()
{
    // gl context destroys with window
}

void R_Clear()
{
    glClearColor(0.4f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}