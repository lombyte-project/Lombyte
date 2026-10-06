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
f32 compute_interpolated_record_value(struct M2c_arg0 *anim) __asm__("FUN_0020c9e0");

f32 compute_interpolated_record_value(struct M2c_arg0 *anim) {
    struct M2c_temp_16_48 *next_record;
    struct M2c_var_4_12 *cur_record;
    f32 temp_f20_55;
    u8 u50, u51;

    if (anim->unk52 != 0xFF) {
        cur_record = anim->unk68;
    } else {
        cur_record = anim->unk6C;
    }
    if (anim->unk54 == 0.0f) {
        return func_001FA6C0(cur_record->unk4) * 0.0625f;
    }
    if (anim->unk52 != anim->unk53) {
        return (func_001FA6C0(cur_record->unk4) * 0.0625f) + anim->unk54;
    }
    u50 = anim->unk50;
    u51 = anim->unk51;
    if ((u8)u51 >= (u8)u50) {
        next_record = anim->unk6C;
        temp_f20_55 = func_001FA6C0(cur_record->unk4) * (1.0f - anim->unk54);
        return (temp_f20_55 + (func_001FA6C0(next_record->unk4) * anim->unk54)) * 0.0625f;
    }
    return (func_001FA6C0(cur_record->unk4) * 0.0625f) + anim->unk54;
}

extern f32 func_0020C9E0(struct M2c_arg0 *) __attribute__((alias("FUN_0020c9e0")));
