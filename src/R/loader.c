#include "renderer.h"

#include <stdio.h>
#include <dlfcn.h>

#include "3d.h"
#include "2d.h"

#include "../Sys/screen.h"

void* dlib = NULL;

bool (*INITBACKEND)(void*) = NULL;
void (*CLEANUPBACKEND)() = NULL;

bool R_LoadLibrary(const char* rlib)
{
    char resolvedFilepath[16];

    #ifdef _WIN32
    sprintf(resolvedFilepath, "%s.%s", rlib, "dll");
    #elif __APPLE__
    sprintf(resolvedFilepath, "%s.%s", rlib, "so");
    #elif __LINUX__
    sprintf(resolvedFilepath, "%s.%s", rlib, "so");
    #endif

    void* dlib = dlopen(resolvedFilepath, RTLD_LAZY);
    if (!dlib)
    {
        printf("FATAL: failed to load renderer %s!\n", rlib);
        return false;
    }

    // link all functions
    #define getSymbol(SYMBOL) SYMBOL = dlsym(dlib, "export_"#SYMBOL)

    // verification symbol
    int validation = *((int*)dlsym(dlib, "VALIDATION"));

    // R
    getSymbol(R_Clear);
    getSymbol(R_SetFramebufferSize);

    // R/3d
    getSymbol(R3d_Create);
    getSymbol(R3d_Destroy);
    
    getSymbol(R3d_Begin);
    getSymbol(R3d_End);

    getSymbol(R3d_SetTransformIdentity);
    getSymbol(R3d_SetTransform);

    getSymbol(R3d_SetCameraIdentity);
    getSymbol(R3d_SetCameraDefault);
    getSymbol(R3d_SetCamera);

    getSymbol(R3d_LoadSkybox);
    getSymbol(R3d_DrawSkybox);

    getSymbol(R3d_TestPass);


    getSymbol(INITBACKEND);
    if (INITBACKEND == NULL)
    {
        printf("backend init function not found!!\n");
        dlclose(dlib);
        return false;
    }

    return true;
}

void R_UnloadLibrary()
{
    dlclose(dlib);
}

bool R_Create()
{
    if (!INITBACKEND)
    {
        printf("no renderer library loaded before renderer create attempt.\n");
        return false;
    }

    // Backend Startup/stop (not exposed)// Backend Startup/stop (not exposed)
    if (!INITBACKEND(Sys_GetWndHandle()))
    {
        printf("[R/loader] backend initialization failed!");
        dlclose(dlib);
        return false;
    }

    getSymbol(CLEANUPBACKEND);

    return true;
}

void R_Destroy()
{
    //dispose
    CLEANUPBACKEND();
}