#include <renderer.h>

#include <stdio.h>
#include <assert.h>

#include <SDL2/SDL.h> // SDL WILL be exposed in these dlls

#include <glad/gl.h>

#include "../../dlcall.h"

SDL_GLContext glctx = NULL;

int VALIDATION = 69;

bool TsExport(INITBACKEND)(void* wndHandle)
{
    glctx = SDL_GL_CreateContext((SDL_Window*) wndHandle);
    if (!glctx)
    {
        printf("[R/renderer] gl context creation failed!\n");
        return false;
    }

    SDL_GL_MakeCurrent((SDL_Window*) wndHandle, glctx);

    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    if (version == 0)
    {
        printf("[R/renderer] opengl failed to load!\n");
        return false;
    }

    printf("[R/renderer] started: opengl %s\n\tglad report: %d.%d\n", glGetString(GL_VERSION), GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    
    int w, h;
    SDL_GL_GetDrawableSize(wndHandle, &w, &h);
    glViewport(0, 0, w, h); // TODO: Viewport change handling

    return true;
}

void TsExport(CLEANUPBACKEND)()
{
    SDL_GL_DeleteContext(glctx);
}
