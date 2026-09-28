#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/save_data/saving_data_menu2/FUN_002235b8.s", FUN_002235b8);
#else
#include "rnc/ui_menus_save_data_saving_data_menu2_types.h"
#include "types.h"












extern struct M2c_D_0013C940 D_0013C940;
extern struct M2c_D_0013D290 D_0013D290;
extern s32 D_0013E05A;
extern s32 D_0015ED84[];
extern s32 D_0015ED98[];
extern s32 D_0015EE20;
extern s32 D_0015EE24;
extern s32 D_0015EE34;
extern u8 D_0015EE98[];
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern s32 D_001D2640;
extern u8 D_001D4E38[];
extern u8 D_001D4F98[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern void InitializeGlobalStateEntry();
extern s32 func_001FBAB8();
extern s32 func_00209370();
extern s32 func_00226A70();
extern s32 func_00226B08();
extern s32 func_0022DA68();
/* retail small-data globals, declared to GAS before the body */
__asm__(".extern D_0015EE34, 4");
__asm__(".extern D_0015EEB0, 4");
__asm__(".extern D_0015EEB4, 4");

s32 saving_data_menu2(struct M2c_arg0 *arg0) __asm__("FUN_002235b8");

s32 saving_data_menu2(struct M2c_arg0 *arg0) {
    s32 *temp_2_157;
    s32 *var_2_244;
    s32 temp_18_20;
    s32 temp_3_215;
    s32 temp_5_221;
    s32 var_2_230;
    s32 var_4_199;

    if (D_001D5BF0.unk4->unk40 != arg0) {
        goto block_62;
    }
    temp_18_20 = arg0->unk40;
    if (arg0->unk4C != 0) {
        goto block_9;
    }
    if (D_001D5BF0.unkD0 != D_001D4E38) {
        goto block_5;
    }
    if (D_001D5BF0.unkD0->unk84 == 0) {
        goto block_5;
    }
    arg0->unk4C = 1;
block_5:
    if (arg0->unk4C != 0) {
        goto block_9;
    }
    if (D_001D5BF0.unkD0 != D_001D4F98) {
        goto block_9;
    }
    if (D_001D5BF0.unkD0->unk84 == 0) {
        goto block_9;
    }
    arg0->unk4C = 1;
block_9:
    if (arg0->unk4C != 1) {
        goto block_16;
    }
    if (arg0->unk30 & 0x2000) {
        goto block_12;
    }
    goto block_14;
block_12:
    func_00226B08(arg0->unk40);
    goto block_15;
block_14:
    func_00226A70(arg0->unk48, arg0->unk40);
block_15:
    D_001D5BF0.unk128 = 1;
    D_001D5BF0.unk12C = 0x4FB5;
block_16:
    arg0->unk4C = 2;
    if (D_001D5BF0.unk128 == 0) {
        goto block_22;
    }
    if (D_0013D290.unkD4 >= 3) {
        goto block_62;
    }
    if (D_0013D290.unkDC >= 0) {
        goto block_62;
    }
    D_001D5BF0.unk128 = 0;
    if (D_0013D290.unkE4 == 0) {
        goto block_21;
    }
    D_0015EEB4 |= 0x80;
    func_001FBAB8(3, D_001D5BF0.unk4);
    goto block_61;
block_21:
    *(s32 *)((u8 *)((D_0013D290.unk14 * 0x1C) + &D_0013D290) + 0x24) = (s32) D_0015ED98[0];
    *(s32 *)((u8 *)((D_0013D290.unk14 * 0x1C) + &D_0013D290) + 0x20) = (s32) D_0015ED84[0];
    *(s32 *)((u8 *)((D_0013D290.unk14 * 0x1C) + &D_0013D290) + 0x2C) = (s32) D_0015EE24;
    /* m2c-unknown:  unknown instruction: ldl $v1, 0x7($a4)  */
    /* m2c-unknown:  unknown instruction: ldr $v1, ($a4)  */
    /* m2c-unknown:  unknown instruction: sdl $v1, 0x7($v0)  */
    /* m2c-unknown:  unknown instruction: sdr $v1, ($v0)  */
    *(s32 *)((u8 *)((D_0013D290.unk14 * 0x1C) + &D_0013D290) + 0x28) = (s32) D_0015EE20;
    InitializeGlobalStateEntry(0, 0x1C, D_0015ED98[0], D_0015ED84[0], ((u8 *)0x15EE98), D_0015EE20, ((u8 *)&D_0013D290) + 0x30, &D_0013D290);
    D_0013E05A = 1;
block_22:
    if (!(D_0013C940.unk1C4 & 0xD00)) {
        goto block_25;
    }
    if (D_001D5BF0.unk124 != 0) {
        goto block_26;
    }
    return 1;
block_25:
block_26:
    if (!(D_0013C940.unk1C4 & 0x10)) {
        goto block_32;
    }
    temp_2_157 = D_001D5BF0.unk4->unk38;
    if (temp_2_157 == NULL) {
        goto block_29;
    }
    D_001D5BF0.unk8 = temp_2_157;
    goto block_32;
block_29:
    if (D_001D5BF0.unk124 != 0) {
        goto block_33;
    }
    return -1;
block_32:
block_33:
    if (D_0015EEB0 == 0x10) {
        goto block_36;
    }
    if (D_0015EEB0 == 1) {
        goto block_36;
    }
    D_001D5BF0.unk8 = (void *) D_001D5BF0.unk4->unk38;
    return 0;
block_36:
    if (D_0013D290.unkD4 >= 3) {
        goto block_62;
    }
    if (D_0013D290.unkDC >= 0) {
        goto block_62;
    }
    if (D_0013D290.unk8 != 2) {
        goto block_62;
    }
    if (!(arg0->unk30 & 1)) {
        goto block_41;
    }
    var_4_199 = D_0013C940.unk1B4;
    goto block_42;
block_41:
    var_4_199 = D_0013C940.unk1A4;
block_42:
    arg0->unk40 = (s32) D_0015EE34;
    if (!(var_4_199 & 0x1000)) {
        goto block_45;
    }
    if (D_0015EE34 == 0) {
        goto block_45;
    }
    arg0->unk40 = (s32) (D_0015EE34 - 1);
block_45:
    if (!(var_4_199 & 0x4000)) {
        goto block_48;
    }
    temp_3_215 = arg0->unk40;
    if (temp_3_215 >= 4) {
        goto block_48;
    }
    arg0->unk40 = (s32) (temp_3_215 + 1);
block_48:
    temp_5_221 = arg0->unk40;
    D_0015EE34 = temp_5_221;
    if (!(var_4_199 & 0x40)) {
        goto block_56;
    }
    var_2_230 = var_4_199 & 0x20;
    if (D_0013D290.unk8 != 2) {
        goto block_57;
    }
    if (*(s32 *)((u8 *)((temp_5_221 * 0x1C) + &D_0013D290) + 0x20) == -1) {
        goto block_55;
    }
    if (!(arg0->unk30 & 0x2000)) {
        goto block_53;
    }
    var_2_244 = D_001D4F98;
    goto block_54;
block_53:
    var_2_244 = D_001D4E38;
block_54:
    D_001D5BF0.unk8 = var_2_244;
    D_001D5BF0.unkD4 = (s32) ((arg0->unk30 & 0x2000) ? 2 : 1);
    D_001D2640 = arg0->unk40;
    goto block_59;
block_55:
    arg0->unk4C = 1;
    goto block_59;
block_56:
    var_2_230 = var_4_199 & 0x20;
block_57:
    if (var_2_230 == 0) {
        goto block_59;
    }
    D_0013D290.unkF4 = 0;
    D_0015EEB4 = D_0015EEB4 & ~2 & ~4;
    func_00209370(-3, -5);
    InitializeGlobalStateEntry(0);
    D_0013E05A = 1;
block_59:
    if (arg0->unk40 == temp_18_20) {
        goto block_61;
    }
    func_0022DA68(1, 0x11, arg0->unk14);
block_61:
block_62:
    return 0;
}

extern s32 func_002235B8(struct M2c_arg0 *arg0) __attribute__((alias("FUN_002235b8")));
#endif /* NON_MATCHING */
