#ifndef LOMBYTE_RNC_SDK_SCE_GS_GPARAM_H
#define LOMBYTE_RNC_SDK_SCE_GS_GPARAM_H

#include "types.h"
#include "sda.h"

/* libgraph global parameters; the table behind GetCoreDataTable(). */
typedef struct sceGsGParam {
    s16 sceGsInterMode;
    s16 sceGsOutMode;
    s16 sceGsFFMode;
    s16 sceGsVersion;
    volatile s32 (*sceGsVSCfunc)(s32);
    s32 sceGsVSCid;
} sceGsGParam;

extern sceGsGParam CoreDataTable __asm__("D_00132D40") NOT_SDA;

#endif /* LOMBYTE_RNC_SDK_SCE_GS_GPARAM_H */
