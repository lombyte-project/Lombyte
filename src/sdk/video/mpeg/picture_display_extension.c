/* _pictureDisplayExtension p5: MPEG2 picture display extension offsets. */

#include "types.h"

typedef struct PdeContext {
    u8 pad_0[0x124];
    s32 unk124;
    s32 unk128;
    u8 pad_12C[0x8];
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    s32 unk140;
    u8 pad_144[0x30];
    s32 unk174;
    s32 unk178;
    u8 pad_17C[0x8];
    s32 unk184;
    u8 pad_188[0x4];
    s32 arr1[3];
    s32 arr2[3];
} PdeContext;

extern s32 _nextBit();

void _pictureDisplayExtension(PdeContext *ctx) {
    s32 num;
    s32 i;

    if (ctx->unk13C != 0) {
        if (ctx->unk184 == 0)
            num = 1;
        else
            num = ctx->unk178 != 0 ? 3 : 2;
    } else {
        if (ctx->unk174 != 3)
            num = 1;
        else
            num = ctx->unk184 != 0 ? 3 : 2;
    }

    for (i = 0; i < num; i++) {
        ctx->arr1[i] = _nextBit(ctx, 0x10);
        _nextBit(ctx, 1);
        ctx->arr2[i] = _nextBit(ctx, 0x10);
        _nextBit(ctx, 1);
    }
}
