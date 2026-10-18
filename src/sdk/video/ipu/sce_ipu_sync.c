#include "types.h"
u32 sceIpuSync(s32 sync_mode) {
    enum {
        IPU_SYNC_WAIT = 0,
        IPU_SYNC_POLL = 1
    };
    u32 result;

    result = 0;
    switch (sync_mode) {
    case IPU_SYNC_WAIT:
        while (*(volatile s32 *)0x10002010 < 0) {
        }
        result = 0;
        break;
    case IPU_SYNC_POLL:
        result = *(volatile u32 *)0x10002010 >> 31;
        break;
    default:
        break;
    }
    return result;
}
