#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/debug/printf_impl/_printf.s", _printf);
#else
#include "rnc/sdk_debug_printf_impl_types.h"
#include "types.h"


extern s32 DIntr();
extern s32 EnableInterrupts();
extern s32 __divdi3();
extern s32 __moddi3();
extern s32 __udivdi3();
extern s32 __umoddi3();
extern s32 fptodp();
extern s32 printfloat();
extern s32 D_0012FC00();
extern void jtbl_001527B0();
void _printf(s8 *arg0, s32 *arg1) {
u8 sp_slot[0xA0];    s8 sp1E;
    s8 sp1F;
    f32 temp_f12_338;
    s32 temp_22_15;
    s32 temp_2_118;
    s32 temp_5_59;
    s32 var_2_174;
    s32 var_5_46;
    s32 var_7_25;
    s64 var_17_209;
    s64 var_17_280;
    s64 var_4_235;
    s64 var_4_299;
    s8 *temp_3_358;
    s8 *temp_3_74;
    s8 *var_16_104;
    s8 *var_16_160;
    s8 *var_16_218;
    s8 *var_16_289;
    s8 *var_16_385;
    s8 *var_16_7;
    s8 *var_18_111;
    s8 temp_2_32;
    s8 temp_6_50;
    u32 temp_3_171;
u8 *var_20_21;
    u64 var_17_151;
    u64 var_17_95;
    u8 temp_2_395;
    u8 var_3_18;
    u8 var_4_131;
    u8 var_4_189;
    u8 var_4_261;
    u8 var_4_321;
    u8 var_4_361;
    struct M2c_var_18_27 *var_18_27;
    s32 *var_19_6;

    var_19_6 = arg1;
    var_16_7 = arg0;
    temp_22_15 = DIntr();
    var_3_18 = (u8) *var_16_7;
    if (*var_16_7 != 0) {
        do {
            var_20_21 = 0;
            var_7_25 = 0;
            if ((s8) var_3_18 == 0x25) {
loop_2:
                var_18_27 = var_16_7 + 1;
loop_3:
                temp_2_32 = var_18_27->unk0 - 0x30;
                switch (temp_2_32) {
                case 0x0:
                    var_5_46 = var_18_27->unk1 - 0x30;
                    temp_6_50 = var_18_27->unk2;
                    if ((u32) (var_5_46 & 0xFF) < 0xAU) {
                        if ((u32) (temp_6_50 - 0x30) < 0xAU) {
                            var_16_7 = ((u8 *)var_18_27 + (2));
                            temp_5_59 = (var_5_46 * 0xA) - 0x30 + temp_6_50;
                            var_5_46 = (temp_5_59 >= 0x20) ? 0x1F : temp_5_59;
                        } else {
                            var_16_7 = ((u8 *)var_18_27 + (1));
                        }
                        var_20_21 = &sp1F - var_5_46;
                        if (var_5_46 > 0) {
                            var_18_27 = var_16_7 + 1;
                            do {
                                temp_3_74 = sp_slot + (0x1F - var_5_46);
                                var_5_46 -= 1;
                                *temp_3_74 = 0x30;
                            } while (var_5_46 > 0);
                        } else {
                            goto loop_2;
                        }
                    } else {
block_14:
                        var_18_27 += 1;
                    }
                    goto loop_3;
                case 0x3C:
                    var_7_25 = 0x6C;
                    goto block_14;
                case 0x38:
                    var_7_25 = 0x68;
                    goto block_14;
                case 0x3F:
                    if (var_7_25 == 0x6C) {
                        var_19_6 += 2;
                        var_17_95 = *(s32 *)((u8 *)var_19_6 - 0x8);
                    } else {
                        var_19_6 += 8;
                        if (var_7_25 == 0x68) {
                            var_17_95 = (u64) *(s32 *)((u8 *)var_19_6 - 0x8);
                        } else {
                            var_17_95 = (u64) (u32) *(s32 *)((u8 *)var_19_6 - 0x8);
                        }
                    }
                    var_16_104 = &sp1F;
                    sp1F = 0;
                    if (var_17_95 == 0) {
                        var_16_104 = &sp1E;
                        sp1E = 0x30;
                        var_18_111 = ((u8 *)var_18_27 + (1));
                    } else {
                        var_18_111 = ((u8 *)var_18_27 + (1));
                        do {
                            var_16_104 -= 1;
                            temp_2_118 = (var_17_95 & 7) + 0x30;
                            var_17_95 = var_17_95 >> 3;
                            *var_16_104 = temp_2_118 & 0xFF;
                        } while (var_17_95 != 0);
                    }
                    if (var_20_21 != 0) {
                        var_16_104 = (s8 *) ((var_20_21 < (u32) var_16_104) ? var_20_21 : (u32) var_16_104);
                    }
                    var_4_131 = (u8) *var_16_104;
                    if (*var_16_104 != 0) {
                        do {
                            var_16_104 += 1;
                            D_0012FC00((s8) var_4_131);
                            var_4_131 = (u8) *var_16_104;
                        } while (*var_16_104 != 0);
                    }
                    break;
                case 0x48:
                    if (var_7_25 == 0x6C) {
                        var_19_6 += 8;
                        var_17_151 = *(s32 *)((u8 *)var_19_6 - 0x8);
                    } else {
                        var_19_6 += 8;
                        if (var_7_25 == 0x68) {
                            var_17_151 = (u64) *(s32 *)((u8 *)var_19_6 - 0x8);
                        } else {
                            var_17_151 = (u64) (u32) *(s32 *)((u8 *)var_19_6 - 0x8);
                        }
                    }
                    var_16_160 = &sp1F;
                    sp1F = 0;
                    if (var_17_151 == 0) {
                        var_16_160 = &sp1E;
                        sp1E = 0x30;
                        var_18_111 = ((u8 *)var_18_27 + (1));
                    } else {
                        var_18_111 = ((u8 *)var_18_27 + (1));
                        do {
                            temp_3_171 = var_17_151 & 0xF;
                            var_2_174 = temp_3_171 + 0x30;
                            if (temp_3_171 >= 0xAU) {
                                var_2_174 = temp_3_171 + 0x57;
                            }
                            var_16_160 -= 1;
                            var_17_151 = var_17_151 >> 4;
                            *var_16_160 = var_2_174 & 0xFF;
                        } while (var_17_151 != 0);
                    }
                    if (var_20_21 != 0) {
                        var_16_160 = (s8 *) ((var_20_21 < (u32) var_16_160) ? var_20_21 : (u32) var_16_160);
                    }
                    var_4_189 = (u8) *var_16_160;
                    if (*var_16_160 != 0) {
                        do {
                            var_16_160 += 1;
                            D_0012FC00((s8) var_4_189);
                            var_4_189 = (u8) *var_16_160;
                        } while (*var_16_160 != 0);
                    }
                    break;
                case 0x34:
                    if (var_7_25 == 0x6C) {
                        var_19_6 += 8;
                        var_17_209 = *(s32 *)((u8 *)var_19_6 - 0x8);
                    } else {
                        var_19_6 += 8;
                        if (var_7_25 == 0x68) {
                            var_17_209 = (s64) *(s32 *)((u8 *)var_19_6 - 0x8);
                        } else {
                            var_17_209 = (s64) (s32) *(s32 *)((u8 *)var_19_6 - 0x8);
                        }
                    }
                    var_16_218 = &sp1F;
                    sp1F = 0;
                    if (var_17_209 == 0) {
                        var_16_218 = &sp1E;
                        sp1E = 0x30;
                        var_18_111 = ((u8 *)var_18_27 + (1));
                    } else {
                        if (var_17_209 < 0) {
                            var_17_209 = 0 - var_17_209;
                            D_0012FC00(0x2D);
                        }
                        var_18_111 = ((u8 *)var_18_27 + (1));
                        if (var_17_209 != 0) {
                            var_4_235 = var_17_209;
                            do {
                                var_16_218 -= 1;
                                *var_16_218 = (__moddi3((s32) var_4_235, 0xA) + 0x30) & 0xFF;
                                var_17_209 = __divdi3((s32) var_17_209, 0xA);
                                var_4_235 = var_17_209;
                            } while (var_17_209 != 0);
                        }
                    }
                    if (var_20_21 != 0) {
                        var_16_218 = (s8 *) ((var_20_21 < (u32) var_16_218) ? var_20_21 : (u32) var_16_218);
                    }
                    var_4_261 = (u8) *var_16_218;
                    if (*var_16_218 != 0) {
                        do {
                            var_16_218 += 1;
                            D_0012FC00((s8) var_4_261);
                            var_4_261 = (u8) *var_16_218;
                        } while (*var_16_218 != 0);
                    }
                    break;
                case 0x45:
                    if (var_7_25 == 0x6C) {
                        var_19_6 += 8;
                        var_17_280 = *(s32 *)((u8 *)var_19_6 - 0x8);
                    } else {
                        var_19_6 += 8;
                        if (var_7_25 == 0x68) {
                            var_17_280 = (s64) *(s32 *)((u8 *)var_19_6 - 0x8);
                        } else {
                            var_17_280 = (s64) (u32) *(s32 *)((u8 *)var_19_6 - 0x8);
                        }
                    }
                    var_16_289 = &sp1F;
                    sp1F = 0;
                    if (var_17_280 == 0) {
                        var_16_289 = &sp1E;
                        sp1E = 0x30;
                        var_18_111 = ((u8 *)var_18_27 + (1));
                    } else {
                        var_18_111 = ((u8 *)var_18_27 + (1));
                        var_4_299 = var_17_280;
                        do {
                            var_16_289 -= 1;
                            *var_16_289 = (__umoddi3((u32) var_4_299, 0xA) + 0x30) & 0xFF;
                            var_17_280 = __udivdi3((u32) var_17_280, 0xA);
                            var_4_299 = var_17_280;
                        } while (var_17_280 != 0);
                    }
                    if (var_20_21 != 0) {
                        var_16_289 = (s8 *) ((var_20_21 < (u32) var_16_289) ? var_20_21 : (u32) var_16_289);
                    }
                    var_4_321 = (u8) *var_16_289;
                    if (*var_16_289 != 0) {
                        do {
                            var_16_289 += 1;
                            D_0012FC00((s8) var_4_321);
                            var_4_321 = (u8) *var_16_289;
                        } while (*var_16_289 != 0);
                    }
                    break;
                case 0x35:
                case 0x36:
                    var_19_6 += 8;
                    temp_f12_338 = *(s32 *)((u8 *)var_19_6 - 0x8);
                    if (temp_f12_338 == 0.0f) {
                        var_18_111 = ((u8 *)var_18_27 + (1));
                        D_0012FC00(0x30, temp_f12_338);
                    } else {
                        var_18_111 = ((u8 *)var_18_27 + (1));
                        printfloat(fptodp(*((temp_2_32 * 4) + &jtbl_001527B0), temp_f12_338));
                    }
                    break;
                case 0x43:
                    var_19_6 += 8;
                    temp_3_358 = *(s32 *)((u8 *)var_19_6 - 0x8);
                    var_4_361 = (u8) *temp_3_358;
                    if (*temp_3_358 == 0) {
                        var_18_111 = ((u8 *)var_18_27 + (1));
                        D_0012FC00(0x28);
                        D_0012FC00(0x6E);
                        D_0012FC00(0x75);
                        D_0012FC00(0x6C);
                        D_0012FC00(0x6C);
                        D_0012FC00(0x29);
                    } else {
                        var_16_385 = temp_3_358;
                        var_18_111 = ((u8 *)var_18_27 + (1));
                        do {
                            var_16_385 += 1;
                            D_0012FC00((s8) var_4_361);
                            temp_2_395 = *var_16_385;
                            var_4_361 = temp_2_395;
                        } while (temp_2_395 != 0);
                    }
                    break;
                case 0x33:
                    var_19_6 += 8;
                    var_18_111 = ((u8 *)var_18_27 + (1));
                    D_0012FC00((s8) ((s64) ((s64) *(s32 *)((u8 *)var_19_6 - 0x8) << 0x20) >> 0x20));
                    break;
                default:
                    var_18_111 = ((u8 *)var_18_27 + (1));
                    break;
                case 0x1:
                case 0x2:
                case 0x3:
                case 0x4:
                case 0x5:
                case 0x6:
                case 0x7:
                case 0x8:
                case 0x9:
                case 0xA:
                case 0xB:
                case 0xC:
                case 0xD:
                case 0xE:
                case 0xF:
                case 0x10:
                case 0x11:
                case 0x12:
                case 0x13:
                case 0x14:
                case 0x15:
                case 0x16:
                case 0x17:
                case 0x18:
                case 0x19:
                case 0x1A:
                case 0x1B:
                case 0x1C:
                case 0x1D:
                case 0x1E:
                case 0x1F:
                case 0x20:
                case 0x21:
                case 0x22:
                case 0x23:
                case 0x24:
                case 0x25:
                case 0x26:
                case 0x27:
                case 0x28:
                case 0x29:
                case 0x2A:
                case 0x2B:
                case 0x2C:
                case 0x2D:
                case 0x2E:
                case 0x2F:
                case 0x30:
                case 0x31:
                case 0x32:
                case 0x37:
                case 0x39:
                case 0x3A:
                case 0x3B:
                case 0x3D:
                case 0x3E:
                case 0x40:
                case 0x41:
                case 0x42:
                case 0x44:
                case 0x46:
                case 0x47:
                    var_18_111 = ((u8 *)var_18_27 + (1));
                    break;
                }
            } else {
                var_18_111 = var_16_7 + 1;
                D_0012FC00((s8) var_3_18);
            }
            var_16_7 = var_18_111;
            var_3_18 = (u8) *var_16_7;
        } while (*var_16_7 != 0);
    }
    if (temp_22_15 != 0) {
        EnableInterrupts();
    }
}
#endif /* NON_MATCHING */
