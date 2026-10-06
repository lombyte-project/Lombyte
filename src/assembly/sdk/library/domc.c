#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _doMC; includes target internal entry symbols. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc/_doMC.s", _doMC);
#else
#include "types.h"
#include "rnc/sdk/library/domc.h"

extern u8 D_00153770[];
extern s32 _Error();

void _doMC(struct MpegDecoder *mpeg, s32 arg1) {
    s32 sp0;
    u8 *sp4;
    u8 *sp8;
    u8 *var30;
    u8 *base2;
    u8 *p;
    s32 idx;
    s32 jdx;
    s32 i;
    u8 *slot;
    u8 *count_slot;

    sp0 = arg1;
    slot = (u8 *)mpeg + arg1 * 0x140;
    if (*(s32 *)(slot + 0x6C8) == 0) {
        goto block_5;
    }
    sp4 = (u8 *)mpeg + 0x6BC;
    i = 0;
    count_slot = sp4 + arg1 * 0x140;
    if (*(s32 *)count_slot <= 0) {
        goto block_6;
    }
    sp8 = (u8 *)mpeg + 0x6C0;
    for (;; i++) {
        var30 = (u8 *)mpeg + 0x6B8;
        base2 = (u8 *)mpeg + 0x5C8;
        idx = i * 4 + sp0 * 0x140;
        jdx = i * 0x1C;
        p = (u8 *)arg0 + (sp0 * 0x140 + 0x590);
        ((void (*)(u8 *)) * (s32 *)((u8 *)arg0 + idx + 0x5B8))(p + (jdx + 0x48));
        ((void (*)(u8 *)) * (s32 *)(base2 + idx))(p + (jdx + 0xB8));
        if (i + 1 >= *(s32 *)(sp4 + sp0 * 0x140)) {
            break;
        }
    }
    goto block_7;
block_5:
    sp8 = (u8 *)mpeg + 0x6C0;
    var30 = (u8 *)mpeg + 0x6B8;
    goto block_7;
block_6:
    sp8 = (u8 *)mpeg + 0x6C0;
    var30 = (u8 *)mpeg + 0x6B8;
block_7:
    if (*(s32 *)((u8 *)sp8 + sp0 * 0x140) == 0) {
        goto block_10;
    }
    if (*(s32 *)((u8 *)mpeg + sp0 * 0x140 + 0x6CC) == 0) {
        goto block_10;
    }
    _Error(mpeg, D_00153770);
block_10:
    if (*(s32 *)((u8 *)sp8 + sp0 * 0x140) == 0) {
        goto block_13;
    }
    func_001271D8(*(s32 *)(var30 + sp0 * 0x140), *(s32 *)((u8 *)mpeg + sp0 * 0x140 + 0x594));
    return;
block_13:
    if (*(s32 *)((u8 *)mpeg + sp0 * 0x140 + 0x6CC) == 0) {
        goto block_16;
    }
    func_001271D8(*(s32 *)(var30 + sp0 * 0x140), mpeg->unk81C);
    return;
block_16:
    func_00127178(*(s32 *)(var30 + sp0 * 0x140), mpeg->unk81C,
                  *(s32 *)((u8 *)mpeg + sp0 * 0x140 + 0x594));
    return;
}
#endif /* NON_MATCHING */
