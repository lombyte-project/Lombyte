#include "types.h"
#include "asm.h"
#include "rnc/assembly_textbin_fun_0021f158_types.h"
#include "types.h"


#include "rnc/assembly_textbin_fun_0021f158_types.h"
#include "types.h"

extern s32 D_0013D428[];
extern u8 D_0013D4C0[];
extern u8 D_001602A0[];
extern u8 D_001602B8[];
extern u8 D_001602C0[];
extern u8 D_001602D0[];
extern struct M2c_D_001D5BF4 *D_001D5BF4[];
struct ImageEntry { u8 pad0[8]; u16 unk8; u8 padA[4]; u16 unkE; u8 pad10[8]; };
extern struct ImageEntry D_001DFFB0[];
extern void PackImageDescriptor(s32 *, s32);
extern void func_001F4280(s32);
extern void func_001F4398();
extern void func_001F7580(void *, u64, void *, s32);
extern u8 *func_001FDD10(s32);
extern void func_00233980(s32, s32);
extern s32 sprintf();
s32 FUN_0021f158(s32 arg0) {
u8 sp_slot[0x50];    u8 descriptor[0x20];
    s16 temp_4_20;
    s32 idx;
    s32 temp_17_38;
    s32 temp_18_40;
    struct ImageEntry *temp_16_37;
    struct M2c_temp_3_15 *temp_3_15;
    u8 *temp_4_77;

    temp_3_15 = D_001D5BF4[0]->unk40;
    temp_4_20 = *(s32 *)((u8 *)((temp_3_15->unk3C * 0xA) + temp_3_15->unk48) + 0x6);
    idx = temp_4_20;
    if (*(idx + D_0013D4C0) == 0) {
        return 0;
    }
    temp_17_38 = D_0013D428[idx];
    temp_16_37 = &D_001DFFB0[idx];
    temp_18_40 = (s32)temp_16_37->unkE;
    func_00233980(0x42, 0x44);
    func_00233980(0x47, 0xB);
    if (temp_16_37->unk8 == 0) {
        sprintf(sp_slot, func_001FDD10(0x4F52));
    } else {
        if (temp_17_38 < 0x3E8) {
            temp_4_77 = sp_slot + sprintf(sp_slot, D_001602B8, temp_17_38);
        } else {
            temp_4_77 = sp_slot + sprintf(sp_slot, D_001602C0, temp_17_38 / 1000, temp_17_38 % 1000);
        }
        if ((s32) temp_18_40 < 0x3E8) {
            sprintf(temp_4_77, D_001602A0, (s32) temp_18_40);
        } else {
            sprintf(temp_4_77, D_001602D0, (s32) temp_18_40 / 1000, (s32) temp_18_40 % 1000);
        }
    }
    func_001F4280(0);
    PackImageDescriptor((s32 *)descriptor, arg0);
    *(s16 *)(descriptor + 0x10) = 0x10;
    *(s16 *)(descriptor + 0x12) = 3;
    func_001F7580((s32 *)descriptor, (((u64)0x80FF << 0x10) | 0xA888), sp_slot, -1);
    func_001F4398();
    return 2;
}
