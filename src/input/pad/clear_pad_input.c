#include "types.h"
#include "rnc/input_pad_clear_pad_input_types.h"

void clear_pad_input(struct M2c_arg0 *arg0) __asm__("FUN_002172c0");

void clear_pad_input(struct M2c_arg0 *arg0) {
    s32 var_5_10;
    struct M2c_var_3_8 *var_3_8;

    var_3_8 = ((u8 *)arg0 + (0x140));
    var_5_10 = 0xF;
    arg0->unk1A0 = 0;
    arg0->unk1A4 = 0;
    arg0->unk1A8 = 0;
    arg0->unk1D0 = 1;
    arg0->unk1B4 = 0;
    arg0->unk1B8 = 0;
    arg0->unk1C0 = 0;
    arg0->unk1C4 = 0;
    arg0->unk1C8 = 0;
    arg0->unk1D8 = 0;
    arg0->unk1D4 = 1;
    arg0->unk1B0 = 0;
    do {
        *(s32 *)((u8 *)var_3_8 - 0x40) = 0;
        var_5_10 -= 1;
        var_3_8->unk0 = 0;
        var_3_8 += 1;
    } while (var_5_10 >= 0);
}

extern void func_002172C0(struct M2c_arg0 *arg0) __attribute__((alias("FUN_002172c0")));