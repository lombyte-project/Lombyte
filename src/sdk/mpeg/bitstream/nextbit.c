#include "types.h"
#include "rnc/sdk/ipu_command_table.h"
#include "rnc/sdk/ipu.h"
#include "rnc/sdk/mpeg/api.h"

typedef struct NextBitContext {
    u8  pad_0[0x818];
    s32 ipu_command_class;
    u8  pad_81C[0x1C];
    s32 cached_bit_window;
    s32 available_bit_count;
    u8  pad_840[0x18];
    s32 unk858;
} NextBitContext;

s32 _nextBit(NextBitContext *ctx, s32 bit_count) {
    s32 consumed_bits;
    s32 counter;
    u32 word;
    counter = 0;
    if ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (counter++ >= 0x1389) {
                DispatchMpegCallbackNoData(ctx->unk858);
                counter = 0;
            }
        } while ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000);
    }
    if (ctx->ipu_command_class != 0 || ctx->available_bit_count < bit_count) {
        *(volatile u32 *)0x10002000 = 0x40000000;
        ctx->ipu_command_class = IpuCommandTable[4];
        ctx->cached_bit_window = (s32)(s64)_waitIpuIdle64(ctx);
    }
    consumed_bits = ((u32)ctx->cached_bit_window) >> (0x20 - bit_count);
    ctx->available_bit_count = 0x20;
    word = bit_count | 0x40000000;
    *(volatile u32 *)0x10002000 = word;
    ctx->ipu_command_class = IpuCommandTable[word >> 28];
    ctx->cached_bit_window = (s32)(s64)_waitIpuIdle64(ctx);
    return consumed_bits;
}
