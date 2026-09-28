#include "types.h"
#include "rnc/sdk_library_motion_vector_types.h"

extern s32 InitializeMemoryCardDirectory();
extern s32 _ipuVdec();
extern s32 _nextBit();
extern s32 func_001280A8();
void _motionVector(s32 arg0, struct M2c_arg1 *arg1, struct M2c_arg2 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    register s32 saved_arg3 __asm__("s1") = arg3;
    register s32 var_7_31 __asm__("a3");
    s32 temp_16_25;
    s32 temp_16_50;
    s32 var_7_56;

    temp_16_25 = _ipuVdec(arg0, 2);
    if ((saved_arg3 != 0) && (temp_16_25 != 0)) {
        var_7_31 = _nextBit(arg0, saved_arg3);
    } else {
        var_7_31 = 0;
    }
    func_001280A8(arg1, saved_arg3, temp_16_25, var_7_31, arg7);
    if (arg5 != 0) {
        arg2->unk0 = InitializeMemoryCardDirectory(arg0);
    }
    temp_16_50 = _ipuVdec(arg0, 2);
    if ((arg4 != 0) && (temp_16_50 != 0)) {
        var_7_56 = _nextBit(arg0, arg4);
    } else {
        var_7_56 = 0;
    }
    if (arg6 != 0) {
        arg1->unk4 = (s32) ((s32) arg1->unk4 >> 1);
    }
    func_001280A8(((u8 *)arg1 + (4)), arg4, temp_16_50, var_7_56, arg7);
    if (arg6 != 0) {
        arg1->unk4 = (s32) (arg1->unk4 * 2);
    }
    if (arg5 != 0) {
        arg2->unk4 = InitializeMemoryCardDirectory(arg0);
    }
}
