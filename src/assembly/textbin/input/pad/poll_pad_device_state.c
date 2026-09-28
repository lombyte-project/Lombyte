#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/input/pad/poll_pad_device_state/FUN_002170c8.s", FUN_002170c8);
#else
#include "rnc/input_pad_poll_pad_device_state_types.h"
#include "types.h"


extern s32 func_00124BD8();
extern s32 func_00124CB0();
extern s32 func_00124DA0();
extern s32 func_001250D8();
extern s32 func_002172C0();
extern s32 func_00217328();
void poll_pad_device_state(struct M2c_arg0 *arg0) __asm__("FUN_002170c8");

void poll_pad_device_state(struct M2c_arg0 *arg0) {    s32 sp0;
u8 sp_slot[0x60];    s32 temp_2_15;
    s32 temp_3_10;
    s32 temp_3_21;
    s32 var_2_51;
    s32 var_5_106;
    s32 var_5_53;
    s32 var_5_70;
    s32 var_5_92;
    s32 var_6_42;
    s32 var_6_87;
    u8 *temp_4_114;
    u8 *temp_4_57;
    u8 *temp_4_77;
    u8 *temp_4_97;
    u8 temp_3_115;
    u8 temp_3_58;
    u8 temp_3_78;
    u8 temp_3_98;
    s32 *temp_6_73;

    arg0->unk1A0 = 0;
    temp_3_10 = arg0->unk1B0;
    arg0->unk1B0 = 0;
    arg0->unk1AC = (s32) arg0->unk348;
    arg0->unk1BC = temp_3_10;
    temp_2_15 = func_00124DA0(arg0->unk194);
    arg0->unk19C = temp_2_15;
    if (temp_2_15 != 1) {
        goto block_36;
    }
    temp_3_21 = arg0->unk198;
    if (temp_3_21 == temp_2_15) {
        goto block_34;
    }
    if (temp_3_21 < 2) {
        goto block_4;
    }
    goto block_9;
block_4:
    if (temp_3_21 != 0) {
        goto block_7;
    }
    goto block_11;
block_7:
    goto block_37;
block_9:
    if (temp_3_21 == 2) {
        goto block_35;
    }
    goto block_37;
block_11:
    var_6_42 = func_00124CB0(arg0->unk194, sp_slot);
    if (sp0 != 0xFFFFFFFF) {
        goto block_13;
    }
    arg0->unk1DC = 0x79;
    goto block_14;
block_13:
    arg0->unk1DC = 0;
block_14:
    var_2_51 = 2;
    if (var_6_42 >= 5) {
        goto block_19;
    }
    var_5_53 = 0;
    if (var_6_42 <= 0) {
        goto block_18;
    }
loop_17:
    temp_4_57 = ((u8 *)arg0 + (0x180 ))+ var_5_53;
    temp_3_58 = *(sp_slot + var_5_53);
    var_5_53 += 1;
    *temp_4_57 = temp_3_58;
    if (var_5_53 < var_6_42) {
        goto loop_17;
    }
block_18:
    var_2_51 = 1;
    goto block_20;
block_19:
    var_6_42 = 0;
block_20:
    arg0->unk198 = var_2_51;
    var_5_70 = var_6_42;
    temp_6_73 = ((u8 *)arg0 + (0x180));
    if (var_5_70 >= 4) {
        goto block_23;
    }
loop_22:
    temp_4_77 = temp_6_73 + var_5_70;
    temp_3_78 = *(sp_slot + var_5_70);
    var_5_70 += 1;
    *temp_4_77 = temp_3_78;
    if (var_5_70 < 4) {
        goto loop_22;
    }
block_23:
    var_6_87 = func_001250D8(arg0->unk194, sp_slot, temp_6_73);
    if (var_6_87 < 5) {
        goto block_25;
    }
    var_6_87 = 0;
    goto block_30;
block_25:
    var_5_92 = 0;
    if (var_6_87 <= 0) {
        goto block_30;
    }
loop_28:
    temp_4_97 = ((u8 *)arg0 + (0x184 ))+ var_5_92;
    temp_3_98 = *(sp_slot + var_5_92);
    var_5_92 += 1;
    *temp_4_97 = temp_3_98;
    if (var_5_92 < var_6_87) {
        goto loop_28;
    }
block_30:
    var_5_106 = var_6_87;
    if (var_5_106 >= 4) {
        goto block_35;
    }
loop_32:
    temp_4_114 = ((u8 *)arg0 + (0x184 ))+ var_5_106;
    temp_3_115 = *(sp_slot + var_5_106);
    var_5_106 += 1;
    *temp_4_114 = temp_3_115;
    if (var_5_106 < 4) {
        goto loop_32;
    }
    goto block_35;
block_34:
    func_00217328(arg0, sp_slot, func_00124BD8(arg0->unk194, sp_slot));
    goto block_37;
block_35:
    func_002172C0(arg0);
    goto block_37;
block_36:
    func_002172C0(arg0);
    arg0->unk198 = 0;
block_37:
    arg0->unk189 = 0;
    arg0->unk188 = 0;
    return;
}
#endif /* NON_MATCHING */
