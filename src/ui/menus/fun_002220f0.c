#include "types.h"
struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_temp_3_16 * unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
};

struct M2c_temp_3_16 {
    u8 pad_0[0x34];
    s32 unk34;
    u8 pad_38[0x8];
    s32 unk40;
};

extern u8 D_001D5BF4[16];
extern u8 D_001DDD40[];
extern void func_001F4280(s32);
extern s32 func_001F4398();
extern s32 func_001F6AF0(s32, s32, u64, s32, s32);
extern s32 func_001FDD10();
s32 FUN_002220f0(struct M2c_arg0 *arg0) {
    s32 temp_16_26;
    s32 temp_16_36;
    s32 temp_16_61;
    s32 temp_16_88;
    s32 temp_17_34;
    s32 temp_17_64;
    s32 temp_17_85;
    s32 temp_20_22;
    s32 temp_20_48;
    s32 temp_5_18;
    u32 temp_16_50;
    u32 temp_17_28;
    u32 temp_17_75;
    struct M2c_temp_3_16 *temp_3_16;

    temp_3_16 = (*(struct M2c_D_001D5BF4 **)(void *)D_001D5BF4)->unk40;
    temp_5_18 = temp_3_16->unk34;
    temp_20_22 = *(s32 *)((u8 *)((temp_3_16->unk40 * 0xC) + temp_5_18) + 0x4);
    func_001F4280(0);
    if (temp_20_22 == -1) {
        goto block_2;
    }
    goto block_4;
block_2:
    temp_16_26 = arg0->unk24;
    temp_17_28 = arg0->unk20;
    temp_17_34 = (s32)(temp_17_28 + ((u32)temp_17_28 >> 0x1F)) >> 1;
    temp_16_36 = ((s32) (temp_16_26 + ((u32) temp_16_26 >> 0x1F)) >> 1) - 8;
    func_001F6AF0(temp_17_34, temp_16_36, 0x80FFA888, func_001FDD10(0x5019), -1);
    goto block_5;
block_4:
    temp_20_48 = temp_20_22 * 0xC;
    temp_16_50 = arg0->unk20;
    temp_16_61 = (s32)(temp_16_50 + ((u32)temp_16_50 >> 0x1F)) >> 1;
    temp_17_64 = ((s32) arg0->unk24 / 3) - 8;
    func_001F6AF0(temp_16_61, temp_17_64, 0x80FFA888, func_001FDD10(*(s32 *)(void *)(temp_20_48 + D_001DDD40)), -1);
    temp_17_75 = arg0->unk20;
    temp_17_85 = (s32)(temp_17_75 + ((u32)temp_17_75 >> 0x1F)) >> 1;
    temp_16_88 = ((s32) (arg0->unk24 * 2) / 3) - 8;
    func_001F6AF0(temp_17_85, temp_16_88, 0x80FFA888, func_001FDD10(*(s32 *)((u8 *)(D_001DDD40 + temp_20_48) + 0x4)), -1);
block_5:
    func_001F4398();
    return 2;
}
