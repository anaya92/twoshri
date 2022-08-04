#include "Sys/screen.h"
#include "Sys/events.h"

#include "R/renderer.h"
#include "R/3d.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
    Sys_InitScreen();
    R_Init();
    R3d_Init();

    // R3d_Lighting(true);
    // int light = R3d_AddLight();
    // R3d_LightPos(light, HMM_Vec3(0.0f, 0.0f, -1.0f));
    // R3d_LightColor(light, 1.0f, HMM_Vec4(1.0f, 0.2f, 0.9f, 1.0f));

    // R3d_Fog(true);
    // R3d_FogSpec(HMM_Vec4(0.4, 0.4f, 0.4f, 1.0f), 1.0f, 1000.0f);

    R3d_Transform trans = R3d_DefineTransform(
        HMM_Vec3(0.0f, 0.0f, 0.0f),
        HMM_Vec3(1.0f, 1.0f, 1.0f),
        HMM_Vec3(1.0f, 1.0f, 1.0f)
    );
    
    // R3d_SetTransform(&trans);


    while (!Sys_ExitRequested())
    {
        Sys_ProcessEvents();

        R3d_Begin();
            R_Clear();
            R3d_SetTransformIdentity();
            //R3d_SetTransform(&trans);
            R3d_TestPass();
        R3d_End();

        Sys_UpdateScreen();
    }

    // R3d_RemoveAllLights();

    R3d_Destroy();
    R_Destroy();
    Sys_CloseScreen();
}