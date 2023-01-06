#include "screen.h"

#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

static SDL_Window* window = NULL;

void Sys_InitScreen(bool startHidden)
{
    //clib stuff
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Sdl2: init failure! %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    Uint32 flags = SDL_WINDOW_OPENGL;
    if (startHidden) flags |= SDL_WINDOW_HIDDEN;

    window = SDL_CreateWindow
    (
        "TWOSHRI (PROTOTYPE BUILD)",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        854,
        480,
        flags
    );

    if (!window)
    {
        printf("Os: window creation failure!\n");
    }
    SDL_GL_SetSwapInterval(-1);
}

void Sys_CloseScreen()
{
    SDL_MinimizeWindow(window);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Sys_MakeFullscreen(bool enabled)
{
    SDL_SetWindowFullscreen(window, enabled);
}

void Sys_ScreenHidden(bool hidden)
{
    if (hidden) SDL_HideWindow(window);
    else SDL_ShowWindow(window);
}

int Sys_GetScreenFramebufferWidth()
{
    int w;
    SDL_GL_GetDrawableSize(window, &w, NULL);
    return w;
}

int Sys_GetScreenFramebufferHeight()
{
    int h;
    SDL_GL_GetDrawableSize(window, &h, NULL);
    return h;
}

void Sys_UpdateScreen()
{
    SDL_GL_SwapWindow(window);
}

void* Sys_GetWndHandle()
{
    return window;
}
