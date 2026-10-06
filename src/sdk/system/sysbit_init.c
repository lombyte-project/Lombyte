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

void _sysbitInit(SysbitState *ctx, s32 arg1, s32 base_addr, s32 size) {
    ctx->field8 = arg1;
    ctx->bit_pointer = arg1;
    ctx->bits = 0;
    ctx->cnt = 0;
    ctx->total = 0;
    ctx->base = base_addr;
    ctx->limit = base_addr + size;
    ctx->wrap_base = size;
    _sysbitFlush(ctx, 0);
}
