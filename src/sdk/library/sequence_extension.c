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
typedef struct SeqExtContext {
    u8 pad_0[0x124];
    s32 unk124;
    s32 unk128;
    u8 pad_12C[0x8];
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    s32 unk140;
    u8 pad_144[0x204];
    u8 pad_348[0x500];
    s32 unk848;
} SeqExtContext;
extern void _Error();
extern s32 _nextBit();
extern void _ipuSetMPEG1();
extern u8 D_00153B00[];
extern u8 D_00153B28[];
void _sequenceExtension(SeqExtContext *ctx) {
    u32 bits;
    u32 bits2;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 f;
    s32 g;
    ctx->unk848 = 1;
    _ipuSetMPEG1(0);
    bits = _nextBit(ctx, 0x1C);
    a = (bits >> 1) & 0xFFF;
    b = (bits >> 17) & 0x3;
    c = (bits >> 13) & 0x3;
    d = (bits >> 15) & 0x3;
    ctx->unk140 = b;
    if (b != 1) {
        _Error(ctx, D_00153B00);
    }
    ctx->unk13C = (bits >> 19) & 0x1;
    f = bits >> 20;
    bits2 = _nextBit(ctx, 0x10);
    g = bits2 >> 8;
    if (((f != 0x48) && (f != 0x58)) && (f != 0x44)) {
        _Error(ctx, D_00153B28);
    }
    ctx->unk124 = (d << 12) | (ctx->unk124 & 0xFFF);
    ctx->unk128 = (c << 12) | (ctx->unk128 & 0xFFF);
    ctx->unk134 += a << 18;
    ctx->unk138 = ctx->unk138 + (g << 10);
}
