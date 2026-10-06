#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x50];
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    f32 unk54;
    u8 pad_58[0x10];
    struct M2c_var_4_12 *unk68;
    struct M2c_temp_16_48 *unk6C;
};

struct M2c_temp_16_48 {
    u8 pad_0[0x4];
    s16 unk4;
};

struct M2c_var_4_12 {
    u8 pad_0[0x4];
    s16 unk4;
};

extern float func_001FA6C0();
f32 compute_interpolated_record_value(struct M2c_arg0 *arg0) __asm__("FUN_0020c9e0");

f32 compute_interpolated_record_value(struct M2c_arg0 *arg0) {
    struct M2c_temp_16_48 *temp_16_48;
    struct M2c_var_4_12 *var_4_12;
    f32 temp_f20_55;
    u8 u50, u51;

    if (arg0->unk52 != 0xFF) {
        var_4_12 = arg0->unk68;
    } else {
        var_4_12 = arg0->unk6C;
    }
    if (arg0->unk54 == 0.0f) {
        return func_001FA6C0(var_4_12->unk4) * 0.0625f;
    }
    if (arg0->unk52 != arg0->unk53) {
        return (func_001FA6C0(var_4_12->unk4) * 0.0625f) + arg0->unk54;
    }
    u50 = arg0->unk50;
    u51 = arg0->unk51;
    if ((u8)u51 >= (u8)u50) {
        temp_16_48 = arg0->unk6C;
        temp_f20_55 = func_001FA6C0(var_4_12->unk4) * (1.0f - arg0->unk54);
        return (temp_f20_55 + (func_001FA6C0(temp_16_48->unk4) * arg0->unk54)) * 0.0625f;
    }
    return (func_001FA6C0(var_4_12->unk4) * 0.0625f) + arg0->unk54;
}

extern f32 func_0020C9E0(struct M2c_arg0 *) __attribute__((alias("FUN_0020c9e0")));
