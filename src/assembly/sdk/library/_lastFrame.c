#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/_lastFrame/_lastFrame.s", _lastFrame);
#else
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
struct MpegDecoder {
    u8 pad_0[0x118];
    s32 unk118;
    u8 pad_11C[0x4];
    s32 unk120;
    u8 pad_124[0x50];
    s32 unk174;
    u8 pad_178[0x44];
    s32 unk1BC;
    u8 pad_1C0[0xC];
    s32 unk1CC;
    s32 unk1D0;
    s32 unk1D4;
    s32 unk1D8;
    s32 unk1DC;
};
extern u8 D_00153AB8[];
extern s32 _Error(struct MpegDecoder *, u8 *, s32);
extern s32 _dispRefImage(struct MpegDecoder *, s32, s32);
extern s32 _dispRefImageField(struct MpegDecoder *, s32, s32, s32);

void _lastFrame(struct MpegDecoder *arg0) {
    s32 count;
    count = arg0->unk118;
    if (arg0->unk120 != 0) {
        _Error(arg0, D_00153AB8, count);
    } else {
        if (arg0->unk174 == 3) {
            _dispRefImage(arg0, arg0->unk1BC, count - 1);
        } else {
            _dispRefImageField(arg0, arg0->unk1CC, arg0->unk1DC, count - 1);
        }
    }
    arg0->unk120 = 0;
}
#endif /* NON_MATCHING */
