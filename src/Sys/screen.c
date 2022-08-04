#include "screen.h"

#include <stdio.h>
#include <SDL2/SDL.h>

static SDL_Window* window = NULL;
static SDL_GLContext glCtx = NULL;

void Sys_InitScreen()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow
    (
        "",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );

    if (!window)
    {
        printf("Os: window creation failure!\n");
    }
}

void Sys_CloseScreen()
{
    if (glCtx) SDL_GL_DeleteContext(glCtx);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Sys_UpdateScreen()
{
    SDL_GL_SwapWindow(window);
}

void* Sys_GetWndHandle()
{
    return window;
}

void* Sys_GetGfxDevice()
{
    if (glCtx == NULL) glCtx = SDL_GL_CreateContext(window);
    if (glCtx == NULL)
    {
        printf("gl context creation failed!\n");
    }

    return glCtx;
}