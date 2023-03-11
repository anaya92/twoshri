#include "Sys/screen.h"
#include "Sys/extras.h"
#include "Sys/events.h"

#include "R/renderer.h"
#include "R/3d.h"

#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "bandaid.h"

int main(int argc, char* argv[])
{
    // load apis
    if (!R_LoadLibrary("Ts.Gl46"))
    {
        Sys_ErrorMsgbox("failed to load renderer dll (are you missing a file?)");
        return -4000;
    }

    // open window
    Sys_InitScreen(true);

    // create renderer
    if (!R_Create())
    {
        Sys_CloseScreen();
        printf("renderer initialization failed! (fatal)\n");
        return false;
    }

    Sys_RequestGamepad(SYS_PLAYER0);
    Sys_Gamepad* input0 = Sys_ForceGrabGamepad(SYS_PLAYER0);
    printf("[main] JoyDevice%i Gamepad%i \"%s\" grabbed.\n", input0->header.deviceIndex, input0->header.pslot, input0->header.name);

    Sys_RequestGamepad(SYS_PLAYER1);
    Sys_Gamepad* input1 = Sys_ForceGrabGamepad(SYS_PLAYER1);
    printf("[main] JoyDevice%i Gamepad%i \"%s\" grabbed.\n", input1->header.deviceIndex, input1->header.pslot, input1->header.name);

    printf("Framebuffer size: %i, %i\n", Sys_GetScreenFramebufferWidth(), Sys_GetScreenFramebufferHeight());
    R_SetFramebufferSize(854, 480);

    // init renderers
    R3d_Create();

    R_ImageSpec skyboxData[6] = {
        Ba_LoadImage("./forestSkybox/px.png", false), // right
        Ba_LoadImage("./forestSkybox/nx.png", false), // left
        Ba_LoadImage("./forestSkybox/py.png", false), // top
        Ba_LoadImage("./forestSkybox/ny.png", false), // bottom
        Ba_LoadImage("./forestSkybox/pz.png", false), // back
        Ba_LoadImage("./forestSkybox/nz.png", false), // front
    };

    R3d_LoadSkybox(skyboxData);
    R3d_DrawSkybox(true);

    for (int i = 0; i < 6; i++)
    {
        stbi_image_free(skyboxData[i].data);
    }

    // Sys_ErrorMsgbox("womp womp womp wahhh");

    // Sys_AddScreenMenubar();

    // R3d_Lighting(true);
    // int light = R3d_AddLight();
    // R3d_LightPos(light, HMM_Vec3(0.0f, 0.0f, -1.0f));
    // R3d_LightColor(light, 1.0f, HMM_Vec4(1.0f, 0.2f, 0.9f, 1.0f));

    // R3d_Fog(true);
    // R3d_FogSpec(HMM_Vec4(0.4, 0.4f, 0.4f, 1.0f), 1.0f, 1000.0f);

    R3d_Camera cam = R3d_DefineCamera(
        HMM_Vec3(0.0f, 0.0f, -5.0f),
        90.0f
    );

    R3d_Transform trans = R3d_DefineTransform(
        HMM_Vec3(0.0f, 0.0f, 3.0f),
        HMM_Vec3(2.0f, 1.0f, 1.0f),
        HMM_Vec3(0.0f, 0.0f, 0.0f)
    );    

    cam.transform.rotate.Y = 110.0f;

    Sys_ScreenHidden(false);

    // delta time data
    uint64_t now, last;
    float delta;

    last = Sys_GetCounter();
    while (!Sys_ExitRequested())
    {
        now = Sys_GetCounter();
        delta = (float)(now - last) / (float) Sys_GetCounterRate();
        last = now;

        Sys_ProcessEvents();

        // trans.position.X -= 0.1f;
        trans.rotate.Y += 100.0f * delta;

        if (input0->active)
        {
            if (input0->centerL.down) Sys_RequestExit();
            if (input0->centerR.down) 
            {
                Sys_MakeFullscreen(true);
                R_SetFramebufferSize(854, 480);
            }

            if (input0->triggerL.pressed > 0.1f) cam.fov = HMM_Lerp(110.0f, input0->triggerL.pressed, 45.0f);
            else cam.fov = 110.0f;
            

            if (fabsf(input0->axisR.x) > 0.1f) cam.transform.rotate.Y += 50.0f * delta * input0->axisR.x;
            if (fabsf(input0->axisR.y) > 0.1f) cam.transform.rotate.X += -50.0f * delta * input0->axisR.y;
        }
        // printf("cam rotation %f\n", cam.transform.rotate.Y);

        R3d_Begin();
            R_Clear();

            // R3d_SetCameraIdentity();
            // R3d_SetCameraDefault();
            R3d_SetCamera(&cam);

            // R3d_SetTransformIdentity();
            R3d_SetTransform(&trans);
            R3d_TestPass();
        R3d_End();

        Sys_UpdateScreen();
    }
    Sys_ScreenHidden(true);

    R3d_Destroy();
    R_Destroy();
    R_UnloadLibrary();

    Sys_FreeEventResources();
    Sys_CloseScreen();
}