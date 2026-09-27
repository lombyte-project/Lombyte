#include "types.h"
extern s32 D_0015F600;
extern s32 D_0015F618;
extern s32 D_0015F5B0;
void InitializeGlobalStateEntry(s32 value) {
    D_0015F600 = value;
    D_0015F618 = D_0015F5B0 = 1;
}
