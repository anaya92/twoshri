// TWOSHRI RENDERER FILE
// (C) ANAYAE 2022

#pragma once

#include <stdbool.h>

#include "../Dll/dlcall.h"

// INIT/CLEANUP (DLL LOADING/UNLOAD)
extern bool R_LoadLibrary(const char* rlib); // GOES FIRST! library name, no extension, ex. "R.Gl46"
extern void R_UnloadLibrary();

// metadata functions, doesnt require renderer creation
typedef struct
{
    int version;
    const char* dataString;
    
} R_Metadata; // .contextSettings = "GL 4 6 " OR "VK 1 1"
extern R_Metadata R_GetMetadata();

// renderer creation functions
extern bool R_Create();
extern void R_Destroy();

// basics
void TsImport(R_Clear)();
void TsImport(R_SetFramebufferSize)(int w, int h);

// universal types/utilities, etc.
// -------------------------------

// texture2d
typedef struct
{
    unsigned char* data;
    int w, h;
    int channelcount;
} R_ImageSpec;
