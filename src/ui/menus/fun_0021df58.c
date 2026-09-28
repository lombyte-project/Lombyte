#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x44];
    s32 unk44;
    s32 unk48;
};

extern s32 FUN_00225530();
s32 FUN_0021df58(struct M2c_arg0 *arg0) {
    s32 temp_2_9;

    temp_2_9 = FUN_00225530(arg0->unk44);
    arg0->unk44 = temp_2_9;
    arg0->unk48 = FUN_00225530(temp_2_9);
    return 0;
}
