// LOUDMOUNTAIN MODEL LOADING API:
// USES COLLADA (.DAE) FILES

#pragma once

typedef struct R_Model;

extern R_Model* R_LoadModel();
extern void R_UnloadModel();