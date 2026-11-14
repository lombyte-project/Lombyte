#include "types.h"
#include "sda.h"
#include "rnc/sdk/rpc/rpc.h"

extern s32 D_0012FCAC NOT_SDA;
extern u8             D_00158040[];
extern u32            D_00158080[];
extern u8             D_001580C0[];

void *sceSifAllocIopHeap(s32 size_bytes) {
    if (D_0012FCAC < 0)
        return 0;
    *(s32 *)D_001580C0 = size_bytes;
    if (sceSifCallRpc(D_00158040, 1, 0, D_001580C0, 4, D_00158080, 4, 0, 0) < 0)
        return 0;
    return *(void **)D_00158080;
}
