#pragma once

extern void Sys_InitScreen();
extern void Sys_CloseScreen();

// controls
extern void Sys_UpdateScreen();

// screen handles
extern void* Sys_GetWndHandle();
extern void* Sys_GetGfxDevice();