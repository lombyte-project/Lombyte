#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceIpuInit; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/dma/sce_ipu_init/sceIpuInit.s", sceIpuInit);
#else
#include "rnc/sdk_dma_sce_ipu_init_types.h"
#include "types.h"




extern struct M2c_D_00133050 D_00133050;
extern struct M2c_D_001330A0 D_001330A0;
extern s32 SetD4ChcrVariant();
void sceIpuInit(void) {
    SetD4ChcrVariant(1);
    *(s32 *)0x10002010 = 0x40000000;
    do {

    } while (*(volatile u32 *)0x10002010 < 0);
    *(s32 *)0x10002000 = 0;
    do {

    } while (*(volatile u32 *)0x10002010 < 0);
    *(s64 *)0x10007010 = D_00133050.unk0;
    *(s64 *)0x10007010 = D_00133050.unk10;
    *(s64 *)0x10007010 = D_00133050.unk20;
    *(s64 *)0x10007010 = D_00133050.unk30;
    *(s64 *)0x10007010 = D_00133050.unk40;
    *(s64 *)0x10007010 = D_00133050.unk40;
    *(s64 *)0x10007010 = D_00133050.unk40;
    *(s64 *)0x10007010 = D_00133050.unk40;
    *(volatile u32 *)0x10002000 = 0x50000000;
    do {

    } while (*(volatile u32 *)0x10002010 < 0);
    *(volatile u32 *)0x10002000 = 0x58000000;
    do {

    } while (*(volatile u32 *)0x10002010 < 0);
    *(volatile u32 *)0x10007010 = (s64) D_001330A0.unk0;
    *(volatile u32 *)0x10007010 = (s64) D_001330A0.unk10;
    *(volatile u32 *)0x10002000 = 0x60000000;
    do {

    } while (*(volatile u32 *)0x10002010 < 0);
    *(volatile u32 *)0x10002000 = 0x90000000;
    do {

    } while (*(volatile u32 *)0x10002010 < 0);
    *(volatile u32 *)0x10002010 = 0x40000000;
    do {

    } while (*(volatile u32 *)0x10002010 < 0);
    *(volatile u32 *)0x10002000 = 0;
    do {

    } while (*(volatile u32 *)0x10002010 < 0);
}
#endif /* NON_MATCHING */
