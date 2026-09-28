#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00230f60/FUN_00230f60.s", FUN_00230f60);
#else
#include "rnc/fun_00230f60_types.h"
#include "types.h"




















extern struct M2c_D_00137B80 D_00137B80;
extern s32 D_0013DD43;
extern struct M2c_D_0013E030 D_0013E030;
extern u8 D_001413D0[];
extern u8 D_0015ED5C[];
extern s32 D_0015ED84;
extern s32 D_0015ED88;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015EE8C;
extern f32 D_0015F43C;
extern s32 D_0015F440;
extern s32 D_0015F460;
extern s32 D_0015F618;
extern s32 D_0015FF00;
extern s32 D_0015FF08;
extern struct M2c_D_0015FF18 *D_0015FF18;
extern s32 D_0015FF1C;
extern s32 D_0015FF20;
extern s32 D_0015FF28;
extern s32 D_0015FF30;
extern s32 D_001600AC;
extern s32 D_001600B0;
extern s32 D_001600B4;
extern s32 D_001600B8;
extern s32 D_001600BC;
extern s32 D_001604E0;
extern s32 D_001604F0;
extern s32 D_00160580;
extern s32 D_00160588;
extern s32 D_00160F0C;
extern u8 D_00186310[];
extern u8 D_00186350[];
extern struct M2c_D_0018CB20 D_0018CB20;
extern struct M2c_D_001940C0 D_001940C0;
extern u8 D_00194180[];
extern u8 D_0019BDC0[];
extern u8 D_0019C1C0[];
extern u8 D_0019C3C0[];
extern struct M2c_D_0019E6C0 D_0019E6C0;
extern u8 D_001B3AC0[];
extern u8 D_001B5980[];
extern u8 D_001B6180[];
extern u8 D_001B6880[];
extern u8 D_001CAAC0[];
extern u8 D_001CD780[];
extern u8 D_001D9740[];
extern void FUN_0022dd78();
extern void FillTransferWords();
extern void FlushCache();
extern void func_0012DC80();
extern void func_0012E088();
extern void func_0012E1A8();
extern void func_001F2C60();
extern void func_001F2D98();
extern void func_001F37E8();
extern void func_001F4A58();
extern s32 func_001F96F8();
extern s32 func_001F97A0();
extern void func_001F9810();
extern void func_001F98D0();
extern void func_002015D8();
extern void func_00202270();
extern void func_002026C8();
extern void func_00202800();
extern void func_002028E0();
extern void func_00203120();
extern void func_00203640();
extern void func_002049F0();
extern s32 func_0020B618();
extern void func_00216728();
extern void func_002335D0();
extern s32 rand();
extern void sceCdSync();
extern void sceGsSyncV();
/* retail small-data globals, declared to GAS before the body */
__asm__(".extern D_0015F440, 4");
__asm__(".extern D_0015F460, 4");
__asm__(".extern D_0015FF00, 4");
__asm__(".extern D_0015FF08, 4");
__asm__(".extern D_001600B8, 4");
__asm__(".extern D_001600BC, 4");
__asm__(".extern D_001604E0, 4");
__asm__(".extern D_001604F0, 4");
__asm__(".extern D_00160588, 8");

void FUN_00230f60(void) {    u64 sp0;
u8 sp_slot[0xC0];    s32 *temp_16_257;
    s32 *temp_18_145;
    struct M2c_var_16_149 *var_16_149;
    s32 *temp_7_377;
    s32 temp_16_228;
    s32 temp_16_231;
    s32 temp_23_97;
    s32 temp_2_157;
    s32 temp_2_187;
    s32 temp_2_253;
    s32 temp_2_321;
    s32 temp_3_144;
    s32 temp_3_156;
    s32 temp_5_370;
    s32 var_17_175;
    s32 var_19_24;
    s32 var_2_386;
    s32 var_8_384;
    void **var_17_150;
    void **var_6_372;
    s32 *temp_16_283;
    s32 *temp_17_223;
    struct M2c_temp_20_100 *temp_20_100;
    s32 *temp_21_130;
    s32 *temp_22_141;
    struct M2c_var_16_182 *var_16_182;
    struct M2c_var_5_381 *var_5_381;

    var_19_24 = 0;
    D_0013E030.unk24 = -1;
    D_0013E030.unk20 = 4;
    D_0015F43C = 1.0f;
    *(s32 *)D_001413D0 = 0;
    D_00160F0C = 0x100000;
    D_0015F618 = 0;
    D_0015F440 = 0;
    func_002015D8(4, D_001413D0, 0x100000);
    D_0015EE78 = D_0015EE8C;
    D_0015EE74 = D_0015EE8C;
    FillTransferWords(D_00194180, 0x87654321, 0x10);
    FillTransferWords(D_001B3AC0, -1, 0x800);
    FillTransferWords(D_001B6880, -1, 0xE00);
    FillTransferWords(D_001B6180, 0, 0xE0);
    func_001F2C60();
    func_001F2D98();
    func_002335D0();
    func_00216728(D_001940C0.unk14 + 0x400000, D_00137B80.unk13B8, D_00137B80.unk13BC);
    func_001F4A58(func_001F96F8(0xC));
    sceCdSync(0);
    FlushCache(0);
    temp_23_97 = func_0020B618(D_001940C0.unk14 + 0x400000, D_001940C0.unk14);
    FlushCache(0);
    temp_20_100 = D_001940C0.unk14;
    func_00203120(((u8 *)temp_20_100) + temp_20_100->unk0, temp_20_100->unk8, ((u8 *)temp_20_100) + temp_20_100->unkC);
    temp_21_130 = ((u8 *)temp_20_100) + temp_20_100->unk4;
    D_0019E6C0.unk0 = (s64) (((s32) (D_0015EE8C + temp_20_100->unk40) >> 8) | 0x1D308000 | (((s64) ((s32) (D_0015EE8C + temp_20_100->unk44) >> 8) << 0x25) | (0xB800 << 0x13)) | (-1 << 0x3F));
    D_0019E6C0.unk10 = (s64) ((((0x8000 << 0x14) | 0x8000) << 0x13) | 0x4000);
    temp_22_141 = temp_21_130 + temp_20_100->unk30;
    D_0019E6C0.unk8 = (s64) ((0xFFA0 << 0x20) | 0xE0);
    temp_3_144 = temp_20_100->unk18;
    temp_18_145 = ((u8 *)temp_20_100) + temp_20_100->unk1C;
    D_0015FF08 = temp_3_144;
    if (temp_3_144 > 0) {
        var_16_149 = temp_18_145;
        var_17_150 = D_001B5980;
        do {
            var_19_24 += 1;
            temp_3_156 = var_16_149->unk0;
            temp_2_157 = func_001F97A0(var_16_149->unk4) << 0x1C;
            var_16_149 += 0x10;
            *var_17_150 = temp_22_141 + temp_3_156 + temp_2_157;
            var_17_150 += 4;
        } while (var_19_24 < D_0015FF08);
    }
    var_17_175 = 0;
    func_001F98D0(D_001CAAC0, temp_18_145, D_0015FF08 * 0x10);
    func_002028E0(temp_21_130 + temp_20_100->unk48);
    var_16_182 = ((u8 *)temp_20_100) + temp_20_100->unk14;
    D_0015FF00 = 0;
    if (temp_20_100->unk10 > 0) {
        do {
            temp_2_187 = var_16_182->unk0;
            var_17_175 += 1;
            func_00203640((temp_2_187 == 0) ? NULL : (temp_21_130 + temp_2_187), ((u8 *)temp_20_100) + temp_20_100->unk1C, ((u8 *)var_16_182) + 0x10, var_16_182->unk4);
            var_16_182 += 0x20;
        } while (var_17_175 < temp_20_100->unk10);
    }
    D_0015F460 = temp_21_130 + temp_20_100->unk38;
    func_00202800(((u8 *)temp_20_100) + temp_20_100->unk2C, temp_20_100->unk28);
    func_002026C8(((u8 *)temp_20_100) + temp_20_100->unk3C, temp_21_130 + temp_20_100->unk34, ((u8 *)temp_20_100) + temp_20_100->unk24, temp_20_100->unk20);
    temp_17_223 = temp_21_130 + temp_20_100->unk4C;
    temp_16_228 = D_0015ED88 - 1;
    temp_16_231 = (temp_16_228 <= -1) ? 0 : temp_16_228;
    func_00202270(temp_17_223 + *(s32 *)((u8 *)((D_0015ED84 * 4) + temp_17_223) + 0x4), sp_slot);
    D_00160580 = sp0;
    func_00202270(temp_17_223 + *(s32 *)((u8 *)((((temp_16_231 * 0x13) + D_0015ED84) * 4) + temp_17_223) + 0x50), sp_slot);
    temp_2_253 = D_0015EE74 + 0x2000;
    D_0015EE78 = temp_2_253;
    temp_16_257 = D_001940C0.unk14 + temp_23_97;
    D_001940C0.unk18 = temp_16_257;
    D_0015EE74 = temp_2_253;
    D_00160588 = sp0;
    func_001F9810(D_0019C1C0, 0x100, sp0);
    func_001F9810(D_0019C3C0, 0x180);
    func_001F9810(D_0019BDC0, 0x400);
    func_001F98D0(D_0019BDC0, D_001D9740, 0x40);
    D_0015FF18 = temp_16_257;
    FillTransferWords(temp_16_257, 0, 0x4000);
    temp_16_283 = temp_16_257 + 0x4000;
    D_0015FF1C = D_0015FF18;
    D_0015FF18->unk20 = 0xFF;
    D_0015FF28 = temp_16_283;
    D_001600AC = temp_16_283 + 0x2000;
    D_001600B4 = -1;
    D_0015FF20 = D_0015FF18 + 0x3F00;
    D_001600B0 = 0;
    D_001600B8 = 0;
    func_001F9810(D_001CD780, 0x200);
    func_001F37E8();
    D_0015FF30 = 0x1F4;
    D_001600BC = 0x1F4000;
    temp_2_321 = (rand() >> 0x10) & 3;
    D_0013E030.unk5C = 0;
    D_0013E030.unk58 = temp_2_321;
    D_0013E030.unk50 = 0;
    D_0013E030.unk54 = 0;
    D_001604F0 = D_001604E0;
    if ((D_0015ED84 == 0) || ((D_0015ED84 == 1) && (D_0013DD43 == 0))) {
        D_0013E030.unk5C = 2;
        D_0013E030.unk58 = 4;
    }
    FillTransferWords(&D_0018CB20, 0, 0x1C0);
    FillTransferWords(D_00186310, 0, 0x40);
    FillTransferWords(D_00186350, 0, 0x40);
    temp_5_370 = D_00160F0C + 0xFFFA0000;
    var_6_372 = D_001940C0.unk8 + temp_5_370;
    D_0018CB20.unk5C = var_6_372;
    D_0018CB20.unk58 = (s32) (D_001940C0.unk4 + temp_5_370);
    temp_7_377 = ((u8 *)temp_20_100) + 0x50 + (D_0013E030.unk58 * 4);
    D_00160F0C = temp_5_370;
    var_5_381 = temp_21_130 + *temp_7_377;
    var_8_384 = 0;
    if (var_5_381->unk4 != 0) {
        var_6_372 = ((u8 *)&D_0018CB20) + 0x60;
        var_2_386 = var_5_381->unk0;
loop_12:
        var_8_384 += 1;
        var_5_381 += 8;
        *var_6_372 = temp_21_130 + *temp_7_377 + (var_2_386 + 0x800);
        var_6_372 += 4;
        if (var_8_384 < 0x46) {
            if (var_5_381->unk4 != 0) {
                var_2_386 = var_5_381->unk0;
                goto loop_12;
            }
        }
    }
    func_002049F0(0, var_5_381, var_6_372, temp_7_377, var_8_384);
    *(s32 *)D_0015ED5C = 0xFFFFFFFF;
    func_0012E088(temp_21_130 + temp_20_100->unk64, FUN_0022dd78, (u64) ((s64) D_0015ED5C << 0x20) >> 0x20);
    do {
        FlushCache(0);
        sceGsSyncV(0);
        func_0012DC80();
    } while (*(s32 *)D_0015ED5C == 0xFFFFFFFF);
    func_0012E1A8();
}

extern void func_00230F60(void) __attribute__((alias("FUN_00230f60")));
#endif /* NON_MATCHING */
