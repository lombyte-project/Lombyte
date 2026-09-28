#include "types.h"
struct SifClientData { u8 pad0[0x24]; void *serve; };
struct StashEntry { s32 addr; s32 count; s32 tag; s32 pad; };
struct Stash { s32 base; s32 size; struct SifClientData cd; s32 cur; s32 n; };
extern struct Stash D_001DD1A0;
extern struct StashEntry D_001DD1D8[64];
extern s32 sceSifBindRpc(struct SifClientData *, u32, s32);
extern s32 SceSifCheckStatRpc(struct SifClientData *);
extern s32 sceSifCallRpc(void *, u32, u32, void *, s32, void *, s32, void (*)(void *), void *);
void FUN_00232d00(void) {
    s32 buf[4];
    s32 i;

    for (;;) {
        if (sceSifBindRpc(&D_001DD1A0.cd, 0x11, 1) < 0) {
            for (;;) {
            }
        }
        while (SceSifCheckStatRpc(&D_001DD1A0.cd) != 0) {
        }
        if (D_001DD1A0.cd.serve != 0) {
            break;
        }
        for (i = 0xFFFF; i != -1; i--) {
        }
    }
    sceSifCallRpc(&D_001DD1A0.cd, 2, 0, 0, 0, buf, 16, 0, 0);
    D_001DD1A0.base = buf[0];
    D_001DD1A0.size = buf[1];
    D_001DD1A0.cur = buf[0];
    D_001DD1A0.n = 0;
    for (i = 0; i < 64; i++) {
        D_001DD1D8[i].addr = 0;
        D_001DD1D8[i].count = 0;
    }
}

extern __typeof__(FUN_00232d00) func_00232D00 __attribute__((alias("FUN_00232d00")));
