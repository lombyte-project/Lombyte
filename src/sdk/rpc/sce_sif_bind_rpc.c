#include "types.h"
struct M2c_arg0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x10];
    s32 unk24;
};

struct M2c_temp_2_15 {
    u8 pad_0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
};

extern u8 D_00156800[];
extern s32 CreateSema();
extern s32 DeleteSema();
extern s32 WaitSema();
extern s32 get_available_rpc_packet() __asm__("func_0011ACE8");
extern s32 func_0011AD90();
extern s32 sceSifSendCmd();
s32 sceSifBindRpc(struct M2c_arg0 *arg0, s32 arg1, s32 arg2) {
    s32 sema_param[6];
    s32 temp_2_31;
    struct M2c_temp_2_15 *temp_2_15;

    arg0->unk10 = 0;
    arg0->unk24 = 0;
    temp_2_15 = (struct M2c_temp_2_15 *)get_available_rpc_packet(D_00156800);
    if (temp_2_15 == NULL) {
        return -1;
    }
    arg0->unk4 = (s32)temp_2_15->unk18;
    arg0->unk0 = (s32)temp_2_15;
    temp_2_15->unk20 = arg1;
    temp_2_15->unk14 = (s32)temp_2_15;
    temp_2_15->unk1C = (s32)arg0;
    if (arg2 & 1) {
        goto block_7;
    }
    sema_param[1] = 1;
    sema_param[2] = 0;
    temp_2_31 = CreateSema(sema_param);
    arg0->unk8 = temp_2_31;
    if (temp_2_31 >= 0) {
        goto block_4;
    }
    func_0011AD90(temp_2_15);
    return -3;
block_4:
    if (sceSifSendCmd(0x80000009, temp_2_15, 0x40, 0, 0, 0) != 0) {
        goto block_6;
    }
    func_0011AD90(temp_2_15);
    DeleteSema(arg0->unk8);
    return -2;
block_6:
    WaitSema(arg0->unk8);
    DeleteSema(arg0->unk8);
    return 0;
block_7:
    arg0->unk8 = -1;
    if (sceSifSendCmd(0x80000009, temp_2_15, 0x40, 0, 0, 0) != 0) {
        return 0;
    }
    func_0011AD90(temp_2_15);
    return -2;
}
