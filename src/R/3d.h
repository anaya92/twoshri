// LOUDMOUNTAIN 3D RENDERING API
// (C) ANAYAE JUL. 2022

// note (7/28/2022)
// api goals: 
// - be basic and easy to understand on first glance.
// - very easy to implement on other gfx apis 
// - do not use glBegin()/glEnd() style vertex declaration (slow!)
// - use pipelines to extend its functionality dynamically. (sort of like NES mappers,
//   using custom logic to do stuff)

#ifndef R_3D_H_
#define R_3D_H_

#include <stdbool.h>
#include <HandmadeMath.h>

#include "renderer.h"
#include "../Dll/dlcall.h"

// allocate/deallocate Renderer3d resources
void TsImport(R3d_Create)();
void TsImport(R3d_Destroy)();

// set default state
void TsImport(R3d_DefaultState)();

// render begin/end
void TsImport(R3d_Begin)();
void TsImport(R3d_End)();

// EXTENSION CAPABILITY AND VERSATILITY: PIPELINES
typedef struct
{

} R3d_Pipeline;

// MODEL transform
// ---------------
typedef struct
{
    hmm_vec3 position; // translates model matrix from vec3_zero
    hmm_vec3 scaling;
    hmm_vec3 rotate;
} R3d_Transform;
#define R3d_DefineTransform(_p, _s, _r) (R3d_Transform) { _p, _s, _r }

void TsImport(R3d_SetTransformIdentity)(); // sets to identity matrix
void TsImport(R3d_SetTransform)(R3d_Transform* transform); // sets program uniforms.

// VIEW/PROJECTION matrices
// ------------------------
typedef struct
{
    R3d_Transform transform; // transforms still apply to cameras
    hmm_vec3 up;
    float fov;
} R3d_Camera;
#define R3d_DefineCamera(p, fov) { (R3d_Transform) { .position = p, .rotate = { 0.0f }, .scaling = { 0.0f }}, HMM_Vec3(0.0f, 1.0f, 0.0f), fov }

void TsImport(R3d_SetCameraIdentity)();
void TsImport(R3d_SetCameraDefault)();
void TsImport(R3d_SetCamera)(R3d_Camera* camera);

// 3D WORLD STUFF (eg. mapwide stuff, skyboxes, fog, lighting, etc.)
// -----------------------------------------------------------------
void TsImport(R3d_LoadSkybox)(R_ImageSpec dataArray[6]); // if null, it will unload
void TsImport(R3d_DrawSkybox)(bool enabled);

// fog stuff
void TsImport(R3d_Fog)(bool enabled);
void TsImport(R3d_FogSpec)(hmm_vec4 color, float near, float far);

// lighting stuff
void TsImport(R3d_Lighting)(bool enabled);
void TsImport(R3d_SetAmbientLight)(hmm_vec4 rgba);

int TsImport(R3d_AddLight)(); // returns id of new lighting object (default color, pos, brightness)
void TsImport(R3d_RemoveLight)();
void TsImport(R3d_RemoveAllLights)();

void TsImport(R3d_LightPos)(int id, hmm_vec3 position);
void TsImport(R3d_LightColor)(int id, float brightness, hmm_vec4 color);

// TBD: POSSIBLE EXTENSION UNDER SCRUTINY
// PIPELINE EXTENSION:
// --------------------------------------
typedef enum
{
    R3D_LIGHTING = (1 << 0),
    R3D_FOG = (1 << 1)
} R3d_LinkLibraries;

typedef struct
{
    
} R3d_PipelineBuilder;

void R3d_AddPipeline(const char* id, R3d_PipelineBuilder builder); // takes a shader program + other config stuff (depth/blend testing maybe?)
                                                                   // and return a pipeline id that can be used in meshes, letting the renderer know to swap
                                                                   // states.
                                                                   // ex: R3d_AddPipeline("skybox", skyboxPipeline);

// primitives (optional)
// ---------------------
void TsImport(R3d_LoadGfxPrimitives)();
void TsImport(R3d_UnloadGfxPrimitives)();

void TsImport(R3d_GfxPrimitivesUv)(bool enabled); // enables texturing on primitives

void TsImport(R3d_DrawSphere)();
void TsImport(R3d_DrawCube)();
void TsImport(R3d_DrawPyramid)();
void TsImport(R3d_TestPass)();

#endif