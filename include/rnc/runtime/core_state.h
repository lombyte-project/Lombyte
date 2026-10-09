#ifndef LOMBYTE_RNC_RUNTIME_CORE_STATE_H
#define LOMBYTE_RNC_RUNTIME_CORE_STATE_H

#include "types.h"
#include "sda.h"

/* Boot-time runtime words; all of them stay out of small data. */
extern s32 GlobalStateResource __asm__("D_0012F76C") NOT_SDA;
extern s32 CoreGlobalWord __asm__("D_0012FBF0") NOT_SDA;
extern s32 RpcCommandState __asm__("D_0012FC08") NOT_SDA;

#endif /* LOMBYTE_RNC_RUNTIME_CORE_STATE_H */
