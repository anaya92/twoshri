// TWOSHRI EVENT CODE

#pragma once

#include <stdbool.h>

// gamepad stuff
typedef enum
{
    SYS_PLAYER0,
    SYS_PLAYER1,
    SYS_PLAYER2,
    SYS_PLAYER3
} Sys_EPlayerSlot;

// all scalar values are 0.0-1.0
struct _gamepadButton
{
    bool pressed;
    bool down;
};

struct _gamepadHeader
{
    Sys_EPlayerSlot pslot; // introspect: what player is this?
    int deviceIndex;
    const char* name;
};

typedef struct
{
    struct _gamepadHeader header;
    bool active;

    struct
    {
        float x, y;
        struct _gamepadButton button;
    } axisL;

    struct
    {
        float x, y;
        struct _gamepadButton button;
    } axisR;
    
    struct
    {
        float pressed; // scalar
    } triggerL;

    struct
    {
        float pressed; // scalar
    } triggerR;

    struct
    {
        struct _gamepadButton up, down, left, right;
    } face;

    struct
    {
        struct _gamepadButton up, down, left, right;
    } dpad;

    struct _gamepadButton centerL, centerR;

    struct _gamepadButton bumperL, bumperR;
} Sys_Gamepad;

extern void Sys_FreeEventResources();

extern void Sys_ProcessEvents();
extern void Sys_RequestExit();
extern bool Sys_ExitRequested();

// input stuff (gamepads impl. first cause its easier.)
extern bool Sys_RequestGamepad(Sys_EPlayerSlot slot); // this checks for availability and inits controller memory
// extern bool Sys_OpenGamepadViaIndex(int deviceIndex, Sys_EPlayerSlot slot);
extern Sys_Gamepad* Sys_GrabGamepad(Sys_EPlayerSlot slot);
extern Sys_Gamepad* Sys_ForceGrabGamepad(Sys_EPlayerSlot slot); // ALWAYS returns a valid gamepad, but may be inactive (check Sys_Gamepad.active)
extern void Sys_ReleaseGamepad(Sys_EPlayerSlot slot); // cancels pending request and release all backend data

extern struct _gamepadHeader* Sys_GetAllAvailableGamepads(); // UNIMPL