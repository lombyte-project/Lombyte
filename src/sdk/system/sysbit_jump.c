#include "types.h"

typedef struct SysbitState {
    s64 bits;
    s32 field8;
    u32 bit_pointer;
    u32 cnt;
    u32 pad14;
    s64 total;
    u32 base;
    u32 limit;
    u32 wrap_base;
} SysbitState;

extern void _sysbitFlush(SysbitState *ctx, s32 amount);

void _sysbitJump(SysbitState *ctx, s32 bits) {
    s64 total;
    u32 offset;
    u32 pointer;

    ctx->bits = 0;
    ctx->cnt = 0;
    total = (ctx->total = (bits * 8) + ctx->total);
    offset = (u32)(total >> 3);
    pointer = ctx->field8 + offset;
    offset = pointer;
    ctx->bit_pointer = pointer;
    if (pointer >= ctx->limit) {
        ctx->bit_pointer = offset - ctx->wrap_base;
    }
    _sysbitFlush(ctx, 0);
}
