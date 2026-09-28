#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};

extern s32 FUN_001fd748();
s32 FUN_00221930(struct M2c_arg0 *arg0) {
    s32 temp_4_8;
    s32 temp_6_9;

    temp_4_8 = arg0->unk18;
    temp_6_9 = arg0->unk1C;
    FUN_001fd748(temp_4_8, temp_4_8 + arg0->unk20, temp_6_9, temp_6_9 + arg0->unk24);
    return 2;
}
