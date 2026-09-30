#include "types.h"
#include "rnc/sdk_library_motion_vector_types.h"

extern s32 InitializeMemoryCardDirectory();
extern s32 _ipuVdec();
extern s32 _nextBit();
extern s32 func_001280A8();

void _motionVector(s32 arg0, struct M2c_arg1 *arg1, struct M2c_arg2 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 code;
    s32 bits;

    code = _ipuVdec(arg0, 2);
    if (arg3 != 0 && code != 0) {
        bits = _nextBit(arg0, arg3);
    } else {
        bits = 0;
    }
    func_001280A8(arg1, arg3, code, bits, arg7);
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
    func_001280A8((u8 *)arg1 + 4, arg4, code, bits, arg7);
    if (arg6 != 0) {
        arg1->unk4 *= 2;
    }
    if (arg5 != 0) {
        arg2->unk4 = InitializeMemoryCardDirectory(arg0);
    }
}
