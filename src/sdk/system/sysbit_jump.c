#include "types.h"
#include "rnc/sdk/system.h"

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

void _sysbitJump(SysbitState *ctx, s32 bits) {
    s64 total;
    u32 byte_position;
    u32 unwrapped_byte_pointer;

    ctx->bits = 0;
    ctx->cnt = 0;
    total = (ctx->total = (bits * 8) + ctx->total);
    byte_position = (u32)(total >> 3);
    unwrapped_byte_pointer = ctx->field8 + byte_position;
    byte_position = unwrapped_byte_pointer;
    ctx->bit_pointer = unwrapped_byte_pointer;
    if (unwrapped_byte_pointer >= ctx->limit) {
        ctx->bit_pointer = byte_position - ctx->wrap_base;
    }
    _sysbitFlush(ctx, 0);
}
