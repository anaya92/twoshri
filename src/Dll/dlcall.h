// TWOSHRI DYNAMIC LIBRARY CALLING CONVENTION/UTILITY LIB (dlcall)

#pragma once

#ifdef _MSC_VER
#define TsExport(symbol) __declspec(dllexport) export_##symbol
#elif __GNUC__
#define TsExport(symbol) __attribute__((visibility("default"))) export_##symbol
#endif

#define TsImport(symbol) (*symbol)