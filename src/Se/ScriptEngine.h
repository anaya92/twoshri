#ifndef SE_SCRIPTENGINE_H_
#define SE_SCRIPTENGINE_H_

#include "../Dll/dlcall.h"

typedef struct* Se_ScriptCore;

void Se_LoadLibrary();
void Se_UnloadLibrary();

Se_ScriptCore TsImport(Se_CreateCore)();
Se_ScriptCore TsImport(Se_DestroyCore)(Se_ScriptCore core);

void TsImport(Se_LoadModule)(const char* module);

#endif