#include "types.h"
extern s32 D_0012FCAC __attribute__((section(".data")));
extern u8 D_00158040[];
extern u8 D_00158080[];
extern u8 D_001580C0[];
extern s32 sceSifCallRpc();
void *sceSifAllocIopHeap(s32 size) {
    if (D_0012FCAC < 0)
        return 0;
    *(s32 *)D_001580C0 = size;
    if (sceSifCallRpc(D_00158040, 1, 0, D_001580C0, 4, D_00158080, 4, 0, 0) < 0)
        return 0;
    return *(void **)D_00158080;
}
