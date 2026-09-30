#include "types.h"

typedef struct NextBitContext {
    u8 pad_0[0x818];
    s32 unk818;
    u8 pad_81C[0x1C];
    s32 unk838;
    s32 unk83C;
    u8 pad_840[0x18];
    s32 unk858;
} NextBitContext;

extern s32 D_00132E70[];
extern s32 _dispatchMpegCbNodata();
extern s64 _waitIpuIdle64();

s32 _nextBit(NextBitContext *ctx, s32 count) {
  s32 result;
  s32 counter;
  u32 word;
  counter = 0;
  if ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000)
  {
    do
    {
      if (counter++ >= 0x1389)
      {
        _dispatchMpegCbNodata(ctx->unk858);
        counter = 0;
      }
    } while ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000);
  }
  if (ctx->unk818 != 0 || ctx->unk83C < count)
  {
    s32 *p;
    *(volatile u32 *)0x10002000 = 0x40000000;
    p = D_00132E70;
    ctx->unk818 = p[4];
    ctx->unk838 = (s32)(s64)_waitIpuIdle64(ctx);
  }
  result = ((u32)ctx->unk838) >> (0x20 - count);
  ctx->unk83C = 0x20;
  word = count | 0x40000000;
  *(volatile u32 *)0x10002000 = word;
  ctx->unk818 = D_00132E70[word >> 28];
  ctx->unk838 = (s32)(s64)_waitIpuIdle64(ctx);
  return result;
}
