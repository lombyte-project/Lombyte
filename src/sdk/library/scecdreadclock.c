#include "types.h"
struct unaligned64 {
    u64 v;
} __attribute__((packed));
extern s32 D_001312D0[];
extern u32 D_001312EC[];
extern u8 D_001324C0[];
extern u8 D_00132D08[];
extern u8 D_00152FF0[];
extern u8 D_00153010[];
extern s32 SignalSema();
extern s32 cd_check_scmd() __asm__("func_00120D40");
extern s32 scePrintf();
extern s32 sceSifCallRpc();
extern void *memcpy();

s32 sceCdReadClock(u64 *clock) {
    u32 result;

    if (cd_check_scmd(0xF) == 0) {
        return 0;
    }
    if (D_001312D0[0] > 0) {
        scePrintf(D_00152FF0);
    }
    if (sceSifCallRpc(D_00132D08, 1, 0, 0, 0, D_001324C0, 0x10, 0, 0) < 0) {
        SignalSema(D_001312EC[0]);
        return 0;
    }
    *(struct unaligned64 *)clock = *(struct unaligned64 *)((u32)(D_001324C0 + 4) | 0x20000000);
    if (D_001312D0[0] > 0) {
        scePrintf(D_00153010);
    }
    result = *(u32 *)((u32)D_001324C0 | 0x20000000);
    SignalSema(D_001312EC[0]);
    return result;
}
