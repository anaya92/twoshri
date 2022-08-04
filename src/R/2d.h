// LOUD MOUNTAIN 2D RENDERER
// (C) ANAYAE JUL. 28 2022

// resource alloc/dealloc
extern void R2d_Init();
extern void R2d_Destroy();

// rendering begin/end
extern void R2d_Begin();
extern void R2d_End();

// Font functions
typedef struct LMFNT_S R2d_Font;

extern R2d_Font* R2d_LoadFont(const char* where);
extern void R2d_UnloadFont(R2d_Font* font);
extern void R2d_Print(R2d_Font* font, const char* text);

// ex R2d_LoadFont("data:font/SYSTEM.fnt")