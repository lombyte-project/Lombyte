#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/texture/load_pif_as_psmt8_h/FUN_001e9168.s", FUN_001e9168);
#else
#include "rnc/rendering_texture_load_pif_as_psmt8_h_types.h"
#include "types.h"




extern s32 func_00118A80();
extern s32 func_00120558();
extern s32 func_00122330();
extern s32 func_00122658();
extern s32 func_001F97A0();
extern s32 func_001F97E8();
void load_pif_as_psmt8_h(struct M2c_arg0 *arg0, struct M2c_arg1 *arg1, u32 arg2, u32 arg3) __asm__("FUN_001e9168");

void load_pif_as_psmt8_h(struct M2c_arg0 *arg0, struct M2c_arg1 *arg1, u32 arg2, u32 arg3) {
u8 sp_slot[0x120];    s32 *sp0;
    s32 *sp4;
    s32 sp14;
    s32 sp18;
    s32 sp2C;
    s32 sp3C;
    s32 sp4C;
    s32 sp50;
    s32 sp60;
    s32 temp_19_30;
    s32 temp_3_63;
    s32 var_2_24;

    func_001F97E8(sp_slot, 0, 0x54);
    sp0 = ((u8 *)arg0 + (0x20));
    var_2_24 = 0x200;
    if (arg0->unk14 == 0) {
        var_2_24 = 0x400;
    }
    sp14 = var_2_24;
    temp_19_30 = arg3 >> 8;
    sp4C = func_001F97A0(arg0->unk8);
    sp50 = func_001F97A0(arg0->unkC);
    sp4 = ((u8 *)arg0 + ((sp14 + 0x20)));
    sp18 = arg0->unk8 * arg0->unkC;
    func_00122330(&sp60, (s16) temp_19_30, 1, (s16) arg0->unk14, 0, 0, 0x10, 0x10);
    func_00118A80(0);
    func_00122658(&sp60, sp0);
    func_00120558(0, 0);
    temp_3_63 = (s32) arg0->unk8 >> 6;
    sp3C = temp_3_63;
    if (temp_3_63 <= 0) {
        sp3C = 1;
    }
    sp2C = arg2 >> 8;
    func_00122330(&sp60, (s16) sp2C, (s16) sp3C, 0x1B, 0, 0, (s16) arg0->unk8, (s16) arg0->unkC);
    func_00118A80(0);
    func_00122658(&sp60, sp4);
    func_00120558(0, 0);
    arg1->unk0 = (s64) (sp2C | ((s64) sp3C << 0xE) | (((s64) sp4C << 0x1A) | 0x01B00000) | ((s64) sp50 << 0x1E) | (((s64) temp_19_30 << 0x25) | (0x8000 << 0x13)) | ((s64) arg0->unk14 << 0x33) | (-1 << 0x3F));
    arg1->unk8 = 1;
    arg1->unk10 = 0;
}
#endif /* NON_MATCHING */
