#include "types.h"
struct M2c_temp_3_30 {
    u8 pad_0[0x7E];
    s16 unk7E;
    u8 pad_80[0x8];
    s32 unk88;
};

extern u8 D_0013E550[];
extern s32 D_0015F5B4;
extern u8 *D_0015F634;
extern s32 func_0022D7F0();
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) {
    s32 temp_2_22;
    struct M2c_temp_3_30 *temp_3_30;

    if (arg1 >= D_0015F5B4) {
        return -1;
    }
    temp_2_22 = func_0022D7F0(D_0015F634 + (arg1 << 5), arg0, arg2, 0, 0x400);
    if (temp_2_22 >= 0) {
        temp_3_30 = (struct M2c_temp_3_30 *)(D_0013E550 + temp_2_22 * 0x70);
        temp_3_30->unk88 = arg2;
        temp_3_30->unk7E = arg1;
    }
    return temp_2_22;
}
