#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x20];
    u16 unk20;
    u8 pad_22[2];
    u16 unk24;
};

struct MenuPacket {
    u8 pad_0[2];
    s16 unk2;
    u8 pad_4[2];
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 pad_C[2];
    u16 unkE;
    s16 unk10;
    u8 pad_12[6];
};

extern void func_001153FC();
extern void func_001F4280();
extern void func_001F4398();
extern void func_001F7580(void *, u64, void *, s32);
extern void *func_001FDD10(s32);
extern s32 func_001FF960();
extern void FUN_001ffc30();

s32 obtain_all_gold_weapons_menu(struct M2c_arg0 *arg0) __asm__("FUN_00222948");

s32 obtain_all_gold_weapons_menu(struct M2c_arg0 *arg0) {
    struct MenuPacket packet;
    struct MenuPacket tmp;
    void *tex;
    s16 width;
    s32 first_y = 4;
    s16 pos_x = 0x18;

    func_001153FC(&tmp, 0, 0x18);
    tmp.unk2 = arg0->unk24;
    tmp.unk6 = arg0->unk20;
    tmp.unk10 = 0x10;
    packet = tmp;
    func_001F4280(0);
    FUN_001ffc30(func_001FF960(0xE99A, 6), 4, 0xC, 0x10, 0x10, 0x80);
    packet.unkA = first_y;
    packet.unk8 = pos_x;
    tex = func_001FDD10(0x5182);
    func_001F7580(&packet, ((u64)0x80FF << 16) | 0xA888, tex, -1);
    width = packet.unkE;
    first_y = width + 0x10;
    packet.unkA = first_y;
    tex = func_001FF960(0xE99A, 6);
    FUN_001ffc30(tex, 4, width + 0x18, 0x10, 0x10, 0x80);
    func_001F7580(&packet, ((u64)0x80FF << 16) | 0xA888, func_001FDD10(0x5183), -1);
    func_001F4398();
    return 2;
}

extern __typeof__(obtain_all_gold_weapons_menu) func_00222948 __attribute__((alias("FUN_00222948")));
