#include "events.h"

#include <stdio.h>
#include <SDL2/SDL.h>

#include <HandmadeMath.h>

// event main
bool exitRequest = false;

// gamepad stuff
int gamepadsAvailable = 0; // bitfield
int gamepadsRequested = 0; // bitfield
Sys_Gamepad gamepads[4] = { 0 };
SDL_GameController* gamepadBackends[4] = { NULL };

void UpdateGamepad(Sys_EPlayerSlot slot); // forward decl.

void Sys_ProcessEvents()
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
            case SDL_QUIT: exitRequest = true; break;

            case SDL_WINDOWEVENT:
            {
                switch (ev.window.type)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        // really shitty solution
                        
                        break;
                }
                break;
            }

            // gamepad stuff
            case SDL_JOYDEVICEADDED:
            {
                if (gamepadsRequested)
                {
                    if (SDL_IsGameController(ev.jdevice.which))
                    {
                        for (int i = 0; i < SYS_PLAYER3 + 1; i++)
                        {
                            if (!(gamepadsRequested & (1 << i))) continue;

                            if (i == SDL_JoystickGetDevicePlayerIndex(ev.jdevice.which))
                            {
                                SDL_GameController* gcon = SDL_GameControllerOpen(ev.jdevice.which);
                                if (gcon != NULL)
                                {
                                    gamepadBackends[i] = gcon;
                                    
                                    // change flags
                                    gamepadsAvailable |= (1 << i);
                                    gamepadsRequested &= ~(1 << i);

                                    gamepads[i].header.pslot = i;
                                    gamepads[i].header.deviceIndex = ev.jdevice.which;
                                    gamepads[i].header.name = SDL_GameControllerName(gcon);

                                    gamepads[i].active = true;

                                    printf("[Sys/events] connected Gamepad%i \"%s\"\n", i, gamepads[i].header.name);
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            }

            case SDL_JOYDEVICEREMOVED:
            {
                Sys_EPlayerSlot slot = SDL_JoystickGetDevicePlayerIndex(ev.jdevice.which);
                if (slot >= 0 && slot < 4)
                {
                    Sys_ReleaseGamepad(slot);
                    gamepadsRequested |= (1 << slot);

                    printf("[Sys/events] disconnected Gamepad%i\n", slot);   
                } 
                break;
            }
        }
    }

    if (gamepadsAvailable)
    {
        for (int i = 0; i < SYS_PLAYER3 + 1; i++)
        {
            UpdateGamepad(i);
        }
    }
}

void Sys_RequestExit()
{
    exitRequest = true;
}

bool Sys_ExitRequested()
{
    return exitRequest;
}

void Sys_FreeEventResources()
{
    for (int i = 0; i < 4; i++)
    {
        SDL_GameControllerClose(gamepadBackends[i]);
    }
}

// gamepad stuff
void UpdateGamepad(Sys_EPlayerSlot slot)
{
    if (gamepadsAvailable & (1 << slot))
    {
        gamepads[slot].axisL.x = HMM_Clamp(-1.0f, (float)SDL_GameControllerGetAxis(gamepadBackends[slot], SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f, 1.0f);
        gamepads[slot].axisL.y = HMM_Clamp(-1.0f, (float)SDL_GameControllerGetAxis(gamepadBackends[slot], SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f, 1.0f);
        gamepads[slot].axisL.button.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_LEFTSTICK);

        gamepads[slot].axisR.x = HMM_Clamp(-1.0f, (float)SDL_GameControllerGetAxis(gamepadBackends[slot], SDL_CONTROLLER_AXIS_RIGHTX) / 32767.0f, 1.0f);
        gamepads[slot].axisR.y = HMM_Clamp(-1.0f, (float)SDL_GameControllerGetAxis(gamepadBackends[slot], SDL_CONTROLLER_AXIS_RIGHTY) / 32767.0f, 1.0f);
        gamepads[slot].axisR.button.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_RIGHTSTICK);

        gamepads[slot].triggerL.pressed = (float)SDL_GameControllerGetAxis(gamepadBackends[slot], SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 32767.0f;
        gamepads[slot].triggerR.pressed = (float)SDL_GameControllerGetAxis(gamepadBackends[slot], SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 32767.0f;

        gamepads[slot].bumperL.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        gamepads[slot].bumperR.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

        gamepads[slot].centerL.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_BACK);
        gamepads[slot].centerR.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_START);

        gamepads[slot].dpad.up.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_DPAD_UP);
        gamepads[slot].dpad.down.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        gamepads[slot].dpad.left.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        gamepads[slot].dpad.right.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

        gamepads[slot].face.up.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_Y);
        gamepads[slot].face.down.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_A);
        gamepads[slot].face.left.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_X);
        gamepads[slot].face.right.down = SDL_GameControllerGetButton(gamepadBackends[slot], SDL_CONTROLLER_BUTTON_B);
    }
}

bool Sys_RequestGamepad(Sys_EPlayerSlot slot)
{
    if (SDL_NumJoysticks() < 1)
    {
        printf("[Sys/events] Requested Gamepad%i, none connected.\n", slot);
        gamepadsRequested |= (1 << slot);
        return false;
    }

    if (gamepadsAvailable & (1 << slot))
    {
        printf("[Sys/event] warning: reloading gamepad.\n");
        SDL_GameControllerClose(gamepadBackends[slot]);

        // ensure bit is off, cuz on failure we have to ensure the gamepad slot is then inaccessible.
        gamepadsAvailable &= ~(1 << slot);
    }

    SDL_GameController* gcon = SDL_GameControllerFromPlayerIndex(slot);
    if (gcon != NULL)
    {
        gamepadBackends[slot] = gcon;
        gamepadsAvailable |= (1 << slot);

        gamepads[slot].header.pslot = slot;
        gamepads[slot].header.deviceIndex = -2;
        gamepads[slot].header.name = SDL_GameControllerName(gcon);

        gamepads[slot].active = true;

        return true;
    }
    else
    {
        printf("[Sys/event] Gamepad%i request pending...\n", slot);
        gamepadsRequested |= (1 << slot);
        return false;
    }
}

Sys_Gamepad* Sys_GrabGamepad(Sys_EPlayerSlot slot)
{
    if (gamepadsAvailable & (1 << slot))
    {
        return &gamepads[slot];
    }
    else
    {
        printf("[Sys/events] warning: attempted to grab unopened gamepad\n");
        return NULL;
    }
}

Sys_Gamepad* Sys_ForceGrabGamepad(Sys_EPlayerSlot slot)
{
    if (gamepadsAvailable & (1 << slot))
    {
        return &gamepads[slot];
    }
    else
    {
        gamepads[slot].header.deviceIndex = -1;
        gamepads[slot].header.name = "Unconnected Gamepad";
        gamepads[slot].header.pslot = slot;

        gamepads[slot].active = false;

        return &gamepads[slot];
    }
}

void Sys_ReleaseGamepad(Sys_EPlayerSlot slot)
{
    SDL_GameControllerClose(gamepadBackends[slot]);

    gamepads[slot].header.deviceIndex = -1;
    gamepads[slot].header.name = "Unconnected Gamepad";
    gamepads[slot].active = false;

    gamepadsAvailable &= ~(1 << slot);
}