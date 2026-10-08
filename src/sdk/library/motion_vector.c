#include "types.h"

struct MotionVector {
    u8 pad_0[0x4];
    s32 unk4;
};

struct MotionVectorStore {
    s32 unk0;
    s32 unk4;
};

extern s32 InitializeMemoryCardDirectory();
extern s32 _ipuVdec();
extern s32 _nextBit();
extern s32 decode_motion_vector() __asm__("func_001280A8");

void _motionVector(s32 arg0, struct MotionVector *arg1, struct MotionVectorStore *arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7) {
    s32 code;
    s32 bits;

    code = _ipuVdec(arg0, 2);
    if (arg3 != 0 && code != 0) {
        bits = _nextBit(arg0, arg3);
    } else {
        bits = 0;
    }
    decode_motion_vector(arg1, arg3, code, bits, arg7);
    if (arg5 != 0) {
        arg2->unk0 = InitializeMemoryCardDirectory(arg0);
    }
    code = _ipuVdec(arg0, 2);
    if (arg4 != 0 && code != 0) {
        bits = _nextBit(arg0, arg4);
    } else {
        bits = 0;
    }
    if (arg6 != 0) {
        arg1->unk4 >>= 1;
    }
    decode_motion_vector((u8 *)arg1 + 4, arg4, code, bits, arg7);
    if (arg6 != 0) {
        arg1->unk4 *= 2;
    }
    if (arg5 != 0) {
        arg2->unk4 = InitializeMemoryCardDirectory(arg0);
    }
}
