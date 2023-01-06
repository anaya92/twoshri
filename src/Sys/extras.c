#include "extras.h"

#include "screen.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <windows.h>

HWND windowsWndPtr;
HMENU windowsMenu;

uint64_t Sys_GetTime()
{
    return SDL_GetTicks64();
}

uint64_t Sys_GetCounter()
{
    return SDL_GetPerformanceCounter();
}

uint64_t Sys_GetCounterRate()
{
    return SDL_GetPerformanceFrequency();
}

void Sys_ErrorMsgbox(const char* msg)
{
    static const char* quotes[] = {
        "id rather die before i come in last - playboi carti",
        "i wanna be with you, koi wa itsumo - yurie kokubu",
        "what it is hoe? wassupppp? - trillville",
        "you can keep the rari, id rather have the chevy - kodak black",
        "pick up da phone bebe - travis scott",
        "boom boom boom, open the door, ATF! - dmx",
        "huh, n* what? huh, give a f* n* what? - young bleed",
        "you just a baby boy, you not no real mccoy, - tyrese",
        "slow down wayne, you know its all on me - big tymers",
        "is that a fish tank bruh in the middle of the dash?\n yeah pimp but don't put yo fingers on my glass. - mannie fresh",
        "heartbeat, heartbreak! - hirata shihoko",
        "i wish i knew you wanted me. - steve lacy"
    };

    static char text[256] = {};
    sprintf(text, "%s\n%s\0", quotes[rand() % (sizeof(quotes) / sizeof(quotes[0]))], msg);

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "error? :(", text, (SDL_Window*)Sys_GetWndHandle());
}

void Sys_AddScreenMenubar()
{
    SDL_Window* wnd = Sys_GetWndHandle();

    SDL_SysWMinfo winfo;
    SDL_VERSION(&winfo.version);

    SDL_GetWindowWMInfo(wnd, &winfo);

    windowsWndPtr = winfo.info.win.window;

    windowsMenu = CreateMenu();

    MENUITEMINFO itemInfo = {
        .cbSize = sizeof(MENUITEMINFO),
        .fMask = MIIM_STRING,
        .fType = MFT_STRING,
        .dwTypeData = "File"
    };

    InsertMenuItem(windowsMenu, 0, 0, &itemInfo);
    SetMenu(windowsWndPtr, windowsMenu);
}

void Sys_AddScreenMenubarItem(const char* itemName)
{

}