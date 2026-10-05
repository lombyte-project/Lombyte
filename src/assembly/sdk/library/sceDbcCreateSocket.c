#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceDbcCreateSocket/sceDbcCreateSocket.s", sceDbcCreateSocket);
#else
#include "types.h"
struct M2c_D_0015B080 { s32 unk0; s32 unk4; s32 unk8; s32 unkC; s32 unk10; u8 pad_14[0x10]; s32 unk24; s32 unk28; s32 unk2C; };
struct M2c_arg0 { s32 unk0; s32 unk4; s32 unk8; s32 unkC; s32 unk10; };
extern u8 D_00153578[]; extern u8 D_0015B008[]; extern struct M2c_D_0015B080 D_0015B080; extern s32 func_00124A20(); extern s32 sceSifCallRpc();
s32 sceDbcCreateSocket(struct M2c_arg0 *arg0, s32 arg1, s32 arg2) {
    register struct M2c_D_0015B080 *state __asm__("s0") = &D_0015B080;
    register u8 *src __asm__("t1") = (u8 *)arg0 + 0x14;
    register u8 *dst __asm__("t2");
    register s32 i __asm__("t0") = 0;
    register s32 even __asm__("v0");
    register s32 odd __asm__("v1");
    state->unk28 = arg1;
    even = arg0->unk0;
    __asm__ volatile ("" : : "r"(even));
    dst = (u8 *)state + 0x14;
    __asm__ volatile ("" : : "r"(dst));
    state->unk2C = arg2;
    state->unk0 = even;
    odd = arg0->unk4; state->unk4 = odd;
    even = arg0->unk8; state->unk8 = even;
    odd = arg0->unkC; state->unkC = odd;
    even = arg0->unk10; state->unk10 = even;
    do { dst[i] = src[i]; i++; } while (i < 0x10);
    if (sceSifCallRpc(D_0015B008, 0x80000901, 0, &D_0015B080, 0x400, &D_0015B080, 0x400, 0, 0) < 0) {
        func_00124A20(D_00153578);
        return 0;
    }
    return state->unk24;
}

#endif /* NON_MATCHING */
