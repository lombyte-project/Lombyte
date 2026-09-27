#include "types.h"
extern s32 D_0015F600[4] __attribute__((section(".sdata")));
extern s32 D_0015F618[4] __attribute__((section(".sdata")));
extern s32 D_0015F5B0 __attribute__((section(".sdata")));

void InitializeGlobalStateEntry(s32 value) {
    D_0015F600[0] = value;
    D_0015F618[0] = 1;
    D_0015F5B0 = 1;
}
