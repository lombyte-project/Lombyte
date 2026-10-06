#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
};

extern s32 func_0023D2C8();
s32 vo_buf_get_tag(struct M2c_arg0 *arg0) __asm__("FUN_0023d2d8");

s32 vo_buf_get_tag(struct M2c_arg0 *arg0) {
    s32 temp_3_16;
    s32 unk8;
    s32 unkC;

    if (func_0023D2C8() != 0) {
        return 0;
    }
    unk8 = *((volatile s32 *)(&arg0->unk8));
    unkC = *((volatile s32 *)(&arg0->unkC));
    temp_3_16 = arg0->unk10;
    return arg0->unk4 + (((s32)((unk8 - unkC) + temp_3_16) % temp_3_16) * 0x138C0);
}
