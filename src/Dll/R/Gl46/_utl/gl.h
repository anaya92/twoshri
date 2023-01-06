// gl utilities

#include <stdbool.h>

extern void Utl_GlCheckpoint();

// SHADER STUFF
extern unsigned int Utl_GlCompileShader(const char* source0, unsigned int type, bool validate);
extern unsigned int Utl_GlMakeProgram(int vs, int fs, bool validate);

// EXTREMELY IMPORTANT! (BUFFER STUFF)
typedef enum
{
    UTL_GLBUFFER_HAS_COLOR = (1 << 0),
    UTL_GLBUFFER_HAS_NORMALS = (1 << 1),
    UTL_GLBUFFER_HAS_UV = (1 << 2),
} Utl_GlBufferFlags;

typedef struct
{
    int vao, vbo;
    int ibo; // also shows whether buffer is indexed
} Utl_GlBuffer;

extern Utl_GlBuffer Utl_GlCreateBuffer(void* vertexData, int vcount, void* indexData, int icount, Utl_GlBufferFlags flags); // NOT INTERLACED!
extern void Utl_GlFreeBuffer(Utl_GlBuffer buffer);