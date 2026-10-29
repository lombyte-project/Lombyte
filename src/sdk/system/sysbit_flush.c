#include "types.h"

typedef struct SysbitStream {
    s64 bits;
    s32 field8;
    u8 *ptr;
    u32 cnt;
    u32 pad14;
    s64 total;
    u8 *base;
    u8 *end;
} SysbitStream;

s64 _sysbitFlush(SysbitStream *ctx, s32 amount) {
    s64 new_var;
    SysbitStream *new_var2;
    s64 bits = ctx->bits;
    s32 cnt = ctx->cnt - amount;

    new_var = bits << amount;
    ctx->bits = new_var;
    ctx->cnt = cnt;
    if ((u32)cnt < 0x39U) {
        do {
            ctx->bits |= (s64)(*(ctx->ptr++)) << (0x38 - ctx->cnt);
            new_var2 = ctx;
            if ((u32)new_var2->ptr >= (u32)new_var2->end) {
                if (ctx->cnt) {
                    ctx->ptr = ctx->base;
                } else {
                    ctx->ptr = ctx->base;
                }
            }
            ctx->cnt += 8;
        } while (ctx->cnt < 0x39U);
    }
    return ctx->total = ctx->total + amount;
}
