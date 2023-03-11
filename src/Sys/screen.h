// TWOSHRI SCREEN STUFF

#ifndef SYS_SCREEN_H_
#define SYS_SCREEN_H_

#include <stdbool.h>

// init/gone
extern void Sys_InitScreen(bool startHidden);
extern void Sys_CloseScreen();

extern void Sys_MakeFullscreen(bool enabled);
extern void Sys_ScreenHidden(bool hidden);

extern int Sys_GetScreenFramebufferWidth();
extern int Sys_GetScreenFramebufferHeight();

// controls
extern void Sys_UpdateScreen();

// screen handles
extern void* Sys_GetWndHandle();

#endif