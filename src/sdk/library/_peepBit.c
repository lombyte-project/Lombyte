typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef struct PeepBitContext
{
  u8 pad_0[0x818];
  s32 unk818;
  u8 pad_81C[0x1C];
  s32 unk838;
  s32 unk83C;
  u8 pad_840[0x18];
  s32 unk858;
} PeepBitContext;
extern s32 D_00132E70[];
extern s32 _dispatchMpegCbNodata();
extern s64 _waitIpuIdle64();
s32 _peepBit(PeepBitContext *ctx, s32 count)
{
  s32 counter;
  s32 *p;

  if (ctx->unk818 != 0 || ctx->unk83C < count)
  {
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
    *(volatile u32 *)0x10002000 = 0x40000000;
    p = D_00132E70;
    ctx->unk818 = p[4];
    ctx->unk838 = (s32)_waitIpuIdle64(ctx);
    ctx->unk83C = 0x20;
  }
  return (u32)ctx->unk838 >> (0x20 - count);
}
