#ifndef SYS_EXTRAS_H_
#define SYS_EXTRAS_H_

#include <stdint.h>

extern uint64_t Sys_GetTime();
extern uint64_t Sys_GetCounter();
extern uint64_t Sys_GetCounterRate();

extern void Sys_ErrorMsgbox(const char* msg);
extern void Sys_AddScreenMenubar();
extern void Sys_AddScreenMenubarItem(const char* itemName);

#endif