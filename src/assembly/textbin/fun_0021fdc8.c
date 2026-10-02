#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021fdc8/FUN_0021fdc8.s", FUN_0021fdc8);
#else
#include "types.h"
struct M2c_D_0013D290 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0xC8];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
};

struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_D_001D5BF4_unk40 * unk40;
};

struct M2c_D_001D5BF4_unk40 {
    u8 pad_0[0x3C];
    s32 unk3C;
    s32 unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x30];
    s32 unk30;
    s32 unk34;
    u8 pad_38[0x8];
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct M2c_temp_2_207 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_215 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_295 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_303 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_355 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_363 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_4_139 {
    u8 pad_0[0x34];
    s32 unk34;
    u8 pad_38[0x8];
    s32 unk40;
};

struct M2c_temp_4_80 {
    u8 pad_0[0x3C];
    s32 unk3C;
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
};

extern struct M2c_D_0013D290 D_0013D290;
extern u8 D_0013D408[];
extern s32 D_001516D8;
extern s32 D_0015ED88;
extern s32 D_001A0314[];
extern struct M2c_D_001D5BF4 *D_001D5BF4[];
extern s32 func_001F96F8();
extern s32 func_0020B618();
extern s32 func_00216788();
extern s32 func_00225D88();
extern s32 func_00225DD8();
extern s32 func_00225E20();
s32 FUN_0021fdc8(struct M2c_arg0 *arg0) {
u8 sp_slot[0x20];    s32 *temp_16_236;
    s32 temp_16_140;
    s32 temp_16_48;
    s32 temp_16_50;
    s32 temp_19_183;
    s32 temp_19_271;
    s32 temp_19_331;
    s32 temp_2_153;
    s32 temp_2_198;
    s32 temp_2_286;
    s32 temp_2_346;
    s32 temp_3_165;
    s32 temp_4_176;
    s32 temp_4_239;
    s32 temp_4_258;
    s32 temp_4_321;
    s32 temp_5_11;
    s32 var_16_16;
    s32 var_18_191;
    s32 var_18_279;
    s32 var_18_339;
    s32 var_2_209;
    s32 var_2_221;
    s32 var_2_297;
    s32 var_2_357;
    struct M2c_temp_2_207 *temp_2_207;
    struct M2c_temp_2_215 *temp_2_215;
    struct M2c_temp_2_295 *temp_2_295;
    struct M2c_temp_2_303 *temp_2_303;
    struct M2c_temp_2_355 *temp_2_355;
    struct M2c_temp_2_363 *temp_2_363;
    struct M2c_temp_4_139 *temp_4_139;
    struct M2c_temp_4_80 *temp_4_80;

    temp_5_11 = arg0->unk34;
    arg0->unk5C = (s32) (arg0->unk5C + 1);
    if (temp_5_11 & 1) {
        var_16_16 = arg0->unk58;
        if (var_16_16 == -1) {

        } else {
            goto block_28;
        }
    } else {
        if (temp_5_11 & 2) {
            var_16_16 = D_001A0314[0];
        } else if (temp_5_11 & 4) {
            var_16_16 = D_001D5BF4[0]->unk40->unk3C;
        } else if (temp_5_11 & 0x100) {
            temp_16_48 = D_001D5BF4[0]->unk40->unk40;
            temp_16_50 = (temp_16_48 <= -1) ? 0 : temp_16_48;
            var_16_16 = (temp_16_50 >= 5) ? 4 : temp_16_50;
            if (D_0013D290.unkD4 < 3) {
                if (D_0013D290.unkDC < 0) {
                    if (arg0->unk44 == -1) {
                        arg0->unk44 = 0;
                    }
                    var_16_16 = *(s32 *)((u8 *)((var_16_16 * 0x1C) + &D_0013D290) + 0x20);
                    if (var_16_16 == -1) {
                        arg0->unk44 = var_16_16;
                    }
                    if (D_0013D290.unk8 != 2) {
                        arg0->unk44 = -1;
                    }
                } else {
                    goto block_21;
                }
            } else {
                goto block_20;
            }
        } else if (temp_5_11 & 8) {
            temp_4_80 = D_001D5BF4[0]->unk40;
            var_16_16 = (s32) *(s16 *)((u8 *)((temp_4_80->unk3C * 0xA) + temp_4_80->unk48) + 0x8);
            if (temp_4_80->unk40 == 0) {
block_20:
block_21:
                arg0->unk44 = -1;
            }
        } else if (temp_5_11 & 0x400) {
            var_16_16 = (s32) ((s32) arg0->unk5C / func_001F96F8(0x12C, temp_5_11)) % 19;
        } else if (temp_5_11 & 0x1000) {
            /* m2c-unknown:  unknown instruction: ldl $v1, 0x7($a4)  */
            /* m2c-unknown:  unknown instruction: ldr $v1, ($a4)  */
            /* m2c-unknown:  unknown instruction: ldl $a2, 0xf($a4)  */
            /* m2c-unknown:  unknown instruction: ldr $a2, 0x8($a4)  */
            /* m2c-unknown:  unknown instruction: ldl $a3, 0x17($a4)  */
            /* m2c-unknown:  unknown instruction: ldr $a3, 0x10($a4)  */
            /* m2c-unknown:  unknown instruction: sdl $v1, 0x7($sp_slot)  */
            /* m2c-unknown:  unknown instruction: sdr $v1, ($sp_slot)  */
            /* m2c-unknown:  unknown instruction: sdl $a2, 0xf($sp_slot)  */
            /* m2c-unknown:  unknown instruction: sdr $a2, 0x8($sp_slot)  */
            /* m2c-unknown:  unknown instruction: sdl $a3, 0x17($sp_slot)  */
            /* m2c-unknown:  unknown instruction: sdr $a3, 0x10($sp_slot)  */
            var_16_16 = *(s32 *)arg0->unk40 + *(sp_slot + (D_0015ED88 * 4));
        } else {
            temp_4_139 = D_001D5BF4[0]->unk40;
            temp_16_140 = temp_4_139->unk40;
            var_16_16 = (temp_16_140 <= -1) ? 0 : temp_16_140;
            if (temp_5_11 & 0x4000) {
                var_16_16 = ((*(s32 *)((u8 *)(temp_4_139->unk34 + (var_16_16 * 0xC)) + 0x2) ^ 2) == 0) ? 9 : var_16_16;
            }
        }
block_28:
        temp_2_153 = arg0->unk34;
        if (temp_2_153 & 0x2000) {
            var_16_16 = (*(s32 *)(var_16_16 + D_0013D408) == 0) ? 0x1E : var_16_16;
        }
        temp_3_165 = arg0->unk44;
        switch (temp_3_165) {
        case 0:
            temp_4_176 = arg0->unk48;
            if (temp_4_176 != 0) {
                if (*(s16 *)0x1516D8 == 0) {
                    temp_19_183 = var_16_16 * 8;
                    if (*(s32 *)((u8 *)(temp_19_183 + arg0->unk30) + 0x4) != 0) {
                        var_18_191 = temp_4_176;
                        if (arg0->unk34 & 0x20) {
                            temp_2_198 = func_00225D88(temp_4_176, temp_2_153) - (*(s32 *)((u8 *)(temp_19_183 + arg0->unk30) + 0x4) << 0xB);
                            arg0->unk60 = temp_2_198;
                            var_18_191 += temp_2_198;
                        }
                        if (arg0->unk34 & 0x10) {
                            temp_2_207 = temp_19_183 + arg0->unk30;
                            var_2_209 = func_00216788(var_18_191, temp_2_207->unk0, temp_2_207->unk4);
                        } else {
                            temp_2_215 = temp_19_183 + arg0->unk30;
                            var_2_209 = func_00216788(var_18_191, temp_2_215->unk0, temp_2_215->unk4);
                        }
                        var_2_221 = -1;
                        if (var_2_209 == 0) {
block_80:
                            arg0->unk44 = var_2_221;
                        } else {
block_77:
                            func_00225DD8(arg0->unk48);
                            arg0->unk50 = var_16_16;
block_78:
                            arg0->unk44 = (s32) (arg0->unk44 + 1);
                        }
                    }
                }
            } else {
            default:
            }
            break;
        case 1:
        case 3:
        case 5:
            if (*(s16 *)0x1516D8 == 0) {
                if (temp_2_153 & 0x20) {
                    temp_16_236 = ((u8 *)arg0) + 0x48 + ((arg0->unk44 == 3) * 4);
                    func_00225E20(*temp_16_236, temp_2_153);
                    temp_4_239 = *temp_16_236;
                    func_0020B618(temp_4_239 + arg0->unk60, temp_4_239);
                    arg0->unk60 = 0;
                }
                var_2_221 = arg0->unk44 + 1;
                goto block_80;
            }
            break;
        case 6:
            arg0->unk44 = 2;
            /* fallthrough */
        case 2:
            if (var_16_16 != arg0->unk50) {
                if (var_16_16 != arg0->unk54) {
                    temp_4_258 = arg0->unk4C;
                    if (temp_4_258 == 0) {
                        arg0->unk44 = 0;
                    } else if (*(s16 *)0x1516D8 == 0) {
                        temp_19_271 = var_16_16 * 8;
                        if (*(s32 *)((u8 *)(temp_19_271 + arg0->unk30) + 0x4) != 0) {
                            var_18_279 = temp_4_258;
                            if (arg0->unk34 & 0x20) {
                                temp_2_286 = func_00225D88(temp_4_258, temp_2_153) - (*(s32 *)((u8 *)(temp_19_271 + arg0->unk30) + 0x4) << 0xB);
                                arg0->unk60 = temp_2_286;
                                var_18_279 += temp_2_286;
                            }
                            if (arg0->unk34 & 0x10) {
                                temp_2_295 = temp_19_271 + arg0->unk30;
                                var_2_297 = func_00216788(var_18_279, temp_2_295->unk0, temp_2_295->unk4);
    __asm__ volatile ("" : "+r" (var_2_297));
                            } else {
                                temp_2_303 = temp_19_271 + arg0->unk30;
                                var_2_297 = func_00216788(var_18_279, temp_2_303->unk0, temp_2_303->unk4);
                            }
                            var_2_221 = -1;
                            if (var_2_297 != 0) {
                                func_00225DD8(arg0->unk4C);
                                arg0->unk54 = var_16_16;
                                goto block_78;
                            }
                            goto block_80;
                        }
                    }
                } else {
                    var_2_221 = 4;
                    goto block_80;
                }
            }
            break;
        case 4:
            if (var_16_16 != arg0->unk54) {
                if (var_16_16 != arg0->unk50) {
                    temp_4_321 = arg0->unk48;
                    if (temp_4_321 != 0) {
                        if (*(s16 *)0x1516D8 == 0) {
                            temp_19_331 = var_16_16 * 8;
                            if (*(s32 *)((u8 *)(temp_19_331 + arg0->unk30) + 0x4) != 0) {
                                var_18_339 = temp_4_321;
                                if (arg0->unk34 & 0x20) {
                                    temp_2_346 = func_00225D88(temp_4_321, temp_2_153) - (*(s32 *)((u8 *)(temp_19_331 + arg0->unk30) + 0x4) << 0xB);
                                    arg0->unk60 = temp_2_346;
                                    var_18_339 += temp_2_346;
                                }
                                if (arg0->unk34 & 0x10) {
                                    temp_2_355 = temp_19_331 + arg0->unk30;
                                    var_2_357 = func_00216788(var_18_339, temp_2_355->unk0, temp_2_355->unk4);
                                } else {
                                    temp_2_363 = temp_19_331 + arg0->unk30;
                                    var_2_357 = func_00216788(var_18_339, temp_2_363->unk0, temp_2_363->unk4);
                                }
                                var_2_221 = -1;
                                if (var_2_357 != 0) {
                                    goto block_77;
                                }
                                goto block_80;
                            }
                        }
                    } else {
                        var_2_221 = -1;
                        goto block_80;
                    }
                } else {
                    var_2_221 = 2;
                    goto block_80;
                }
            }
            break;
        }
    }
    return 0;
}

extern s32 func_0021FDC8(struct M2c_arg0 *arg0) __attribute__((alias("FUN_0021fdc8")));
#endif /* NON_MATCHING */
