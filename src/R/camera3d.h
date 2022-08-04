#pragma once

#include "transform.h"

typedef struct
{
    R_Transform transform;
    float fov;
} R_Camera3d;

void UpdateCamera();