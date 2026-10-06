#include "types.h"
struct M2c_arg1 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

extern u8 D_00153A38[];
extern s32 _Error();
s32 reserve_aligned_buffer_space(s32 arg0, struct M2c_arg1 *arg1, s32 arg2,
                                 u32 arg3) __asm__("FUN_0012bc20");

s32 reserve_aligned_buffer_space(s32 arg0, struct M2c_arg1 *arg1, s32 arg2, u32 arg3) {
    s32 temp_2_17;
    u32 temp_4_18;

    temp_2_17 = ((u32)((arg1->unk8 + arg3) - 1) / arg3) * arg3;
    temp_4_18 = temp_2_17 + arg2;
    if ((u32)(arg1->unk0 + arg1->unk4) >= temp_4_18) {
        arg1->unk8 = temp_4_18;
        return temp_2_17;
    }
    _Error(arg0, D_00153A38);
    return 0;
}