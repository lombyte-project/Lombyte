typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;

typedef struct SeqExtContext {
    u8  pad_0[0x124];
    s32 horizontal_size;
    s32 vertical_size;
    u8  pad_12C[0x8];
    s32 bit_rate;
    s32 vbv_buffer_size;
    s32 progressive_sequence;
    s32 chroma_format;
    u8  pad_144[0x204];
    u8  pad_348[0x500];
    s32 mpeg2;
} SeqExtContext;

extern void _Error();
extern s32  _nextBit();
extern void _ipuSetMPEG1();
extern u8   unsupported_chroma_format_message[] __asm__("D_00153B00");
extern u8   unsupported_profile_level_message[] __asm__("D_00153B28");

void _sequenceExtension(SeqExtContext *ctx) {
    u32 bits;
    u32 bits2;
    s32 bit_rate_extension;
    s32 chroma_format;
    s32 vertical_size_extension;
    s32 horizontal_size_extension;
    s32 profile_and_level_indication;
    s32 vbv_buffer_size_extension;
    ctx->mpeg2 = 1;
    _ipuSetMPEG1(0);
    bits = _nextBit(ctx, 0x1C);
    bit_rate_extension = (bits >> 1) & 0xFFF;
    chroma_format = (bits >> 17) & 0x3;
    vertical_size_extension = (bits >> 13) & 0x3;
    horizontal_size_extension = (bits >> 15) & 0x3;
    ctx->chroma_format = chroma_format;
    if (chroma_format != 1) {
        _Error(ctx, unsupported_chroma_format_message);
    }
    ctx->progressive_sequence = (bits >> 19) & 0x1;
    profile_and_level_indication = bits >> 20;
    bits2 = _nextBit(ctx, 0x10);
    vbv_buffer_size_extension = bits2 >> 8;
    if (((profile_and_level_indication != 0x48) && (profile_and_level_indication != 0x58)) &&
        (profile_and_level_indication != 0x44)) {
        _Error(ctx, unsupported_profile_level_message);
    }
    ctx->horizontal_size = (horizontal_size_extension << 12) | (ctx->horizontal_size & 0xFFF);
    ctx->vertical_size = (vertical_size_extension << 12) | (ctx->vertical_size & 0xFFF);
    ctx->bit_rate += bit_rate_extension << 18;
    ctx->vbv_buffer_size = ctx->vbv_buffer_size + (vbv_buffer_size_extension << 10);
}
