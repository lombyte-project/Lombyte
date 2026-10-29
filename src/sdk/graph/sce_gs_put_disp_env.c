#include "types.h"
#include "rnc/sdk/libgraph.h"
extern s32 GetCoreDataTable();
void sceGsPutDispEnv(struct sceGsDispEnv *env) {
    s32 *core;

    core = (s32 *)GetCoreDataTable();
    if (*(s16 *)((u8 *)core + 6) == 1) {
        *(volatile u64 *)0x12000000 = env->pmode;
        *(volatile u64 *)0x12000070 = env->dispfb;
        *(volatile u64 *)0x12000080 = env->display;
        *(volatile u64 *)0x120000C0 = env->bgcolor;
    } else {
        *(volatile u64 *)0x12000000 = env->pmode;
        *(volatile u64 *)0x12000020 = env->smode2;
        *(volatile u64 *)0x12000090 = env->dispfb;
        *(volatile u64 *)0x120000A0 = env->display;
        *(volatile u64 *)0x120000E0 = env->bgcolor;
    }
}
