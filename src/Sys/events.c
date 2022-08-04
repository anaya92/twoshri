#include "events.h"

#include <SDL2/SDL.h>

bool exitRequest = false;

void Sys_ProcessEvents()
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
            case SDL_QUIT:
                exitRequest = true;
                break;
        }
    }
}

bool Sys_ExitRequested()
{
    return exitRequest;
}