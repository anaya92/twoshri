// LOUD MOUNTAIN 2D RENDERER
// (C) ANAYAE JUL. 28 2022

#ifndef R_2D_H_
#define R_2D_H_

#include "renderer.h"

#include "../Dll/dlcall.h"

#include <HandmadeMath.h>

// resource alloc/dealloc
void TsImport(R2d_Init)();
void TsImport(R2d_Destroy)();

// rendering begin/end
void TsImport(R2d_Begin)();
void TsImport(R2d_End)();

// 2d transform stuff
typedef struct
{
    hmm_vec2 position;
    hmm_vec2 scaling;
    hmm_vec2 rotation;
} R2d_Transform;

// drawing textures
// void TsImport(R2d_DrawTexture2d)(R_Texture2d* texture, int x, int y);

// Font functions
typedef struct LMBMFNT_S R2d_BitmapFont;

R2d_BitmapFont* TsImport(R2d_LoadBitmapFont)(const char* where);
void TsImport(R2d_UnloadBitmapFont)(R2d_BitmapFont* font);
void TsImport(R2d_BmPrint)(R2d_BitmapFont* font, const char* text);

// ex. R2d_LoadFont("data:font/SYSTEM.fnt")

#endif