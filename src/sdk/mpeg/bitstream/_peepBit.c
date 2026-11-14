#include "types.h"
#include "rnc/sdk/ipu_command_table.h"
#include "rnc/sdk/ipu.h"
#include "rnc/sdk/mpeg/api.h"

typedef struct PeepBitContext {
    u8  pad_0[0x818];
    s32 ipu_command_class;
    u8  pad_81C[0x1C];
    s32 bit_window;
    s32 available_bit_count;
    u8  pad_840[0x18];
    s32 unk858;
} PeepBitContext;

s32 _peepBit(PeepBitContext *ctx, s32 requested_bit_count) {
    s32 counter;

    if (ctx->ipu_command_class != 0 || ctx->available_bit_count < requested_bit_count) {
        counter = 0;
        if ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000) {
            do {
                if (counter++ >= 0x1389) {
                    DispatchMpegCallbackNoData(ctx->unk858);
                    counter = 0;
                }
            } while ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000);
        }
        *(volatile u32 *)0x10002000 = 0x40000000;
        ctx->ipu_command_class = IpuCommandTable[4];
        ctx->bit_window = (s32)_waitIpuIdle64(ctx);
        ctx->available_bit_count = 0x20;
    }
    return (u32)ctx->bit_window >> (0x20 - requested_bit_count);
}
