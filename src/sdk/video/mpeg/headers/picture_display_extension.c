/* _pictureDisplayExtension p5: MPEG2 picture display extension offsets. */

#include "types.h"

typedef struct PdeContext {
    u8  pad_0[0x124];
    s32 horizontal_size;
    s32 vertical_size;
    u8  pad_12C[0x8];
    s32 bit_rate;
    s32 vbv_buffer_size;
    s32 progressive_sequence;
    s32 chroma_format;
    u8  pad_144[0x30];
    s32 picture_structure;
    s32 top_field_first;
    u8  pad_17C[0x8];
    s32 repeat_first_field;
    u8  pad_188[0x4];
    s32 arr1[3];
    s32 arr2[3];
} PdeContext;

extern s32 _nextBit();

void _pictureDisplayExtension(PdeContext *ctx) {
    s32 num;
    s32 i;

    if (ctx->progressive_sequence != 0) {
        if (ctx->repeat_first_field == 0)
            num = 1;
        else
            num = ctx->top_field_first != 0 ? 3 : 2;
    } else {
        if (ctx->picture_structure != 3)
            num = 1;
        else
            num = ctx->repeat_first_field != 0 ? 3 : 2;
    }

    for (i = 0; i < num; i++) {
        ctx->arr1[i] = _nextBit(ctx, 0x10);
        _nextBit(ctx, 1);
        ctx->arr2[i] = _nextBit(ctx, 0x10);
        _nextBit(ctx, 1);
    }
}
