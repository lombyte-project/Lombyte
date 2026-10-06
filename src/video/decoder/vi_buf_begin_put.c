#include "types.h"
struct M2c_arg0 {
    s32 unk0;
    u8 pad_4[0x4];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad_1C[0x24];
    s32 unk40;
};

extern s32 FUN_00118990();
extern s32 FUN_001189b0();
void vi_buf_begin_put(struct M2c_arg0 *arg0, s32 *arg1, s32 *arg2, s32 *arg3,
                      s32 *arg4) __asm__("FUN_0023be20");

void vi_buf_begin_put(struct M2c_arg0 *arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 temp_2_37;
    s32 temp_3_22;
    s32 temp_4_17;
    s32 temp_5_19;
    s32 temp_5_32;
    s32 temp_6_20;
    s32 temp_hi_29;

    FUN_001189b0(arg0->unk40);
    temp_4_17 = arg0->unk10;
    temp_5_19 = arg0->unk14;
    temp_6_20 = temp_4_17 + 2;
    temp_3_22 = arg0->unk18;
    temp_5_32 = ((arg0->unk8 - temp_6_20) << 0xB) - temp_5_19;
    temp_hi_29 = (s32)(((arg0->unkC + temp_4_17) << 0xB) + temp_5_19) % temp_3_22;
    if ((temp_3_22 - temp_hi_29) >= temp_5_32) {
        *arg1 = arg0->unk0 + temp_hi_29;
        *arg2 = temp_5_32;
        *arg3 = 0;
        *arg4 = 0;
    } else {
        *arg1 = arg0->unk0 + temp_hi_29;
        *arg2 = arg0->unk18 - temp_hi_29;
        *arg3 = arg0->unk0;
        *arg4 = temp_5_32 - (arg0->unk18 - temp_hi_29);
    }
    SignalSema(arg0->unk40, temp_5_32, temp_6_20, temp_hi_29);
}
