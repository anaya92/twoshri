// gl utilities

#include <stdbool.h>

extern unsigned int Utl_GlCompileShader(const char* source0, unsigned int type, bool validate);
extern unsigned int Utl_GlMakeProgram(int vs, int fs, bool validate);

extern void Utl_GlCheckpoint();