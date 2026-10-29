#include "types.h"
#include "rnc/sdk/ipu_command_table.h"
typedef struct PeepBitContext {
    u8 pad_0[0x818];
    s32 unk818;
    u8 pad_81C[0x1C];
    s32 unk838;
    s32 unk83C;
    u8 pad_840[0x18];
    s32 unk858;
} PeepBitContext;
extern s32 _dispatchMpegCbNodata();
extern s64 _waitIpuIdle64();
s32 _peepBit(PeepBitContext *ctx, s32 count) {
    s32 counter;

    if (ctx->unk818 != 0 || ctx->unk83C < count) {
        counter = 0;
        if ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000) {
            do {
                if (counter++ >= 0x1389) {
                    _dispatchMpegCbNodata(ctx->unk858);
                    counter = 0;
                }
            } while ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000);
        }
        *(volatile u32 *)0x10002000 = 0x40000000;
        ctx->unk818 = IpuCommandTable[4];
        ctx->unk838 = (s32)_waitIpuIdle64(ctx);
        ctx->unk83C = 0x20;
    }
    return (u32)ctx->unk838 >> (0x20 - count);
}
