#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x50004];
    s32 unk50004;
};

s32 GetStateTableWord(struct M2c_arg0 *arg0, s32 arg1) {
    s32 temp_2_5;
    s32 result;

    temp_2_5 = arg0->unk50004;
    result = temp_2_5;
    if (arg1 < temp_2_5) {
        temp_2_5 = arg1;
    }
    result -= temp_2_5;
    arg0->unk50004 = result;
    return temp_2_5;
}
