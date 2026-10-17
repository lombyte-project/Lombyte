#include "types.h"
u32 sceIpuSync(s32 sync_mode) {
    u32 result;

    result = 0;
    switch (sync_mode) {
    case 0:
        while (*(volatile s32 *)0x10002010 < 0) {
        }
        result = 0;
        break;
    case 1:
        result = *(volatile u32 *)0x10002010 >> 31;
        break;
    default:
        break;
    }
    return result;
}
