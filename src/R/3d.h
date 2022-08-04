// LOUDMOUNTAIN 3D RENDERING API
// (C) ANAYAE JUL. 2022

// note (7/28/2022)
// api goals: 
// - be basic and easy to understand on first glance.
// - very easy to implement on other gfx apis 
// - do not use glBegin()/glEnd() style vertex declaration (slow!)
// - use pipelines to extend its functionality dynamically. (sort of like NES mappers,
//   using custom logic to do stuff)

#pragma once

#include <stdbool.h>
#include <HandmadeMath.h>

// allocate/deallocate Renderer3d resources
extern void R3d_Init();
extern void R3d_Destroy();

// TBD: POSSIBLE EXTENSION UNDER SCRUTINY
// // PIPELINE EXTENSION:
// typedef struct
// {
    
// } R3d_PipelineBuilder;

// extern void R3d_AddPipelineExt(/* R_PipelineBuilder builder */); // takes a shader program + other config stuff (depth/blend testing maybe?)
//                                                                  // and return a pipeline id that can be used in meshes, letting the renderer know to swap
//                                                                  // states.

// set default state
extern void R3d_DefaultState();

// render begin/end
extern void R3d_Begin();
extern void R3d_End();

// MODEL transform
typedef struct
{
    hmm_vec3 position; // translates model matrix from vec3_zero
    hmm_vec3 scaling;
    hmm_vec3 rotate;
} R3d_Transform;
#define R3d_DefineTransform(_p, _s, _r) (R3d_Transform) { _p, _s, _r }

extern void R3d_SetTransformIdentity(); // sets to identity matrix
extern void R3d_SetTransform(R3d_Transform* transform); // sets program uniforms.

// primitives (optional)
extern void R3d_LoadGfxPrimitives();
extern void R3d_UnloadGfxPrimitives();

extern void R3d_GfxPrimitivesUv(bool enabled); // enables texturing on primitives

extern void R3d_DrawSphere();
extern void R3d_DrawCube();
extern void R3d_DrawPyramid();
extern void R3d_TestPass();

// fog stuff
extern void R3d_Fog(bool enabled);
extern void R3d_FogSpec(hmm_vec4 color, float near, float far);

// lighting stuff
extern void R3d_Lighting(bool enabled);
extern void R3d_SetAmbientLight(hmm_vec4 rgba);

extern int R3d_AddLight(); // returns id of new lighting object (default color, pos, brightness)
extern void R3d_RemoveLight();
extern void R3d_RemoveAllLights();

extern void R3d_LightPos(int id, hmm_vec3 position);
extern void R3d_LightColor(int id, float brightness, hmm_vec4 color);