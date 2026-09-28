#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/draw_debug_profiler/FUN_001f39d0.s", FUN_001f39d0);
#else
#include "rnc/rendering_draw_debug_profiler_types.h"
#include "types.h"




extern u8 D_00100AE0[];
extern u16 D_0010FA90;
extern u8 D_0010FAA0[];
extern s32 D_0015ED80;
extern u8 D_0015EE40;
extern s32 D_0015F34C;
extern s32 D_0015F350;
extern u8 D_0015F370[];
extern u8 D_0015F380[];
extern u8 D_0015F390[];
extern u8 D_0015F3A0[];
extern u8 D_0015F3B0[];
extern u8 D_0015F3C0[];
extern u8 D_0015F3D0[];
extern u8 D_0015F3E0[];
extern u8 D_0015F3F0[];
extern u8 D_0015F3F8[];
extern u8 D_0015F400[];
extern u8 D_0015F410[];
extern u8 D_0015F418[];
extern u8 D_0015F428[];
extern s32 D_0015F434;
extern f32 D_0015F43C;
extern s32 D_0015F440[];
extern s32 D_0015F464;
extern s32 D_0015F468;
extern s32 D_0015F46C;
extern s32 D_0015F470;
extern s32 D_0015F604;
extern f32 D_0015F614;
extern s32 D_0015F620;
extern s32 D_0015F648[];
extern struct M2c_D_0016045C *D_0016045C;
extern u8 D_001610C0;
extern u8 D_001610C1;
extern u8 D_001610C2;
extern u8 D_001610C3;
extern s32 D_001872D4[];
extern struct M2c_D_0018A2B0 D_0018A2B0;
extern s32 D_0018C34C;
extern u8 D_001D8EB0[];
extern u8 D_001E1300[];
extern u8 D_001E3200[];
extern u8 D_001E78A0[];
extern u8 D_001E78B8[];
extern void AppendDmaTag(u32);
extern void FlushCache();
extern void func_001E9AB8();
extern void func_001EDC50();
extern void func_001EE338();
extern void func_001F21B0();
extern void func_001F21B8();
extern void func_001F2260();
extern void func_001F2588();
extern void func_001F2C10(void);
extern void func_001F3868();
extern void func_001F4280();
extern void func_001F4398();
extern void func_001F4650();
extern void func_001F46C8();
extern void func_001F4740();
extern void func_001F4808();
extern void func_001F4880();
extern void func_001F4BE0();
extern void func_001F4D98();
extern void func_001F4FB8();
extern void func_001F5138();
extern void func_001F5210();
extern void func_001F79A8();
extern void func_001F92B0();
extern s32 func_001FA6C0();
extern s32 func_001FA6D0(f32);
extern void func_001FB368();
extern void func_001FB680();
extern void func_001FE980();
extern void func_001FF780(void);
extern void func_0020CC60(void);
extern void func_0020CEF8();
extern void func_0020D460(void);
extern void func_00217C18();
extern void func_00228A30();
extern void func_00228B38();
extern void func_0022A5E0();
extern void func_00233308();
extern void func_002333A8();
extern void func_002334D8();
extern void func_002337B0();
extern void func_00233830();
extern void func_00233980(s32, s64);
extern void func_00233BC8();
extern void func_00233C28();
extern void func_00234F98();
extern void func_00235780();
extern void func_00235840(void);
extern void func_00235898(void);
extern void func_002358C8();
extern void func_00235990();
extern void func_00237370();
extern void func_00237A70();
void draw_debug_profiler(void) __asm__("FUN_001f39d0");

void draw_debug_profiler(void) {
    f32 temp_f1_469;
    f32 var_f0_471;
    s32 temp_2_273;
    s32 temp_2_321;
    s32 temp_2_422;
    s32 temp_3_329;

    if (D_0016045C == NULL) {
        goto block_5;
    }
    if (D_0016045C->unk4 != 0) {
        goto block_5;
    }
    if ((D_0015F434 ^ 1) & 1) {
        goto block_5;
    }
    if (D_0018A2B0.unk8 == 0) {
        goto block_5;
    }
    if ((*(s32 *)0x18C34C) == 0) {
        goto block_6;
    }
block_5:
    func_001FB368();
block_6:
    func_001F2260();
    func_0020CC60();
    func_001F2C10();
    func_001F3868();
    D_0015F620 = -1;
    func_001F21B0(D_0015F380, 0xF);
    func_001F21B8(D_0015F380, 0xF);
    if (D_0016045C == NULL) {
        goto block_11;
    }
    if (!(D_0015F434 & 1)) {
        goto block_11;
    }
    if (D_0018A2B0.unk8 == 0) {
        goto block_10;
    }
    func_001E9AB8();
block_10:
    func_001F21B8(D_0015F390, 0xE);
    func_001F21B0(D_0015F390, 0xE);
block_11:
    if (!(*(s32 *)0x15F434 & 2)) {
        goto block_13;
    }
    func_002333A8();
block_13:
    AppendDmaTag(0x02010000);
    if (!(*(s32 *)0x15F434 & 4)) {
        goto block_17;
    }
    if (D_0015ED80 == 0) {
        goto block_16;
    }
    func_00235990();
    goto block_17;
block_16:
    func_002358C8();
block_17:
    AppendDmaTag(0x02020000);
    if (D_0018A2B0.unk34 == 0) {
        goto block_20;
    }
    if (D_0015F46C == 0) {
        goto block_21;
    }
    func_00233C28();
    func_001F4280(1);
    func_001F46C8();
    func_001F4398();
    func_00233BC8();
block_20:
block_21:
    func_001F21B0(D_0015F3A0, 6);
    func_001F21B8(D_0015F3A0, 6);
    if (!(*(s32 *)0x15F434 & 8)) {
        goto block_23;
    }
    func_00228B38();
block_23:
    AppendDmaTag(0x02040000);
    if (D_0018A2B0.unk44 == 0) {
        goto block_26;
    }
    if (*(s32 *)D_0015F370 == 0) {
        goto block_27;
    }
    func_001F5138(D_0015F370);
block_26:
block_27:
    if (!(*(s32 *)0x15F434 & 0x20)) {
        goto block_29;
    }
    func_001F4280(1);
    func_001F79A8();
    func_001F4398();
block_29:
    if (D_0018A2B0.unk34 == 0) {
        goto block_32;
    }
    if (D_0015F470 == 0) {
        goto block_33;
    }
    func_00233C28();
    func_001F4280(1);
    func_001F4740();
    func_001F4398();
    func_00233BC8();
block_32:
block_33:
    if (!(*(s32 *)0x15F434 & 0x10)) {
        goto block_35;
    }
    func_0020D460();
block_35:
    AppendDmaTag(0x02080000);
    func_001F4280(0);
    if (!(*(s32 *)0x15F434 & 0x20)) {
        goto block_39;
    }
    if (D_0018A2B0.unk30 == 0) {
        goto block_39;
    }
    if (D_0015F620 == 6) {
        goto block_39;
    }
    func_00233830(D_0010FAA0, *(s32 *)0x10FA90);
    D_0015F620 = 6;
block_39:
    func_001F21B8(D_0015F3B0, 4);
    func_001F21B0(D_0015F3B0, 4);
    if (!(*(s32 *)0x15F434 & 0x20)) {
        goto block_53;
    }
    func_00233C28();
    if (D_0018A2B0.unk34 == 0) {
        goto block_44;
    }
    if (D_0015F464 == 0) {
        goto block_43;
    }
    func_001F4650();
block_43:
    func_00233980(0x42, (0x8000 << 0x18) | 0x48);
    func_001EDC50();
    func_00233C28();
    func_001F4880();
block_44:
    func_001F21B0(D_0015F3C0, 6);
    func_001F21B8(D_0015F3C0, 6);
    if (D_0018A2B0.unk38 == 0) {
        goto block_46;
    }
    func_00233980(8, 5);
    func_00233C28();
    FlushCache(0);
    func_00217C18();
    D_0015F620 = 8;
block_46:
    func_001F21B0(D_0015F3D0, 8);
    func_001F21B8(D_0015F3D0, 8);
    if (D_0018A2B0.unk3C == 0) {
        goto block_52;
    }
    temp_2_273 = D_0015F604;
    if (D_0015F468 == 0) {
        goto block_49;
    }
    func_00233C28();
    func_001F4808();
block_49:
    if (temp_2_273 != 0) {
        goto block_51;
    }
    func_001F92B0();
block_51:
    func_00233980(0x42, (0x8000 << 0x18) | 0x44);
    func_001EE338();
block_52:
    func_001F21B0(D_0015F3E0, 6);
    func_001F21B8(D_0015F3E0, 6);
block_53:
    if (D_0018A2B0.unk48 == 0) {
        goto block_55;
    }
    func_001FB680();
block_55:
    func_001F21B8(D_0015F3F0, 0xF);
    func_00233C28();
    temp_2_321 = *(s32 *)0x15F434;
    if (!(*(s32 *)0x15F434 & 0x10000)) {
        goto block_57;
    }
    func_00237A70();
block_57:
    temp_3_329 = D_0015F604;
    if (!(temp_2_321 & 0x80)) {
        goto block_60;
    }
    if (D_0018A2B0.unk40 == 0) {
        goto block_61;
    }
    func_001FF780();
    func_001FE980();
    func_001F4D98();
block_60:
block_61:
    if (temp_3_329 != 2) {
        goto block_64;
    }
    if (*(s32 *)0x15EE40 == 0) {
        goto block_64;
    }
    func_001F4BE0();
block_64:
    func_001F21B8(D_0015F3F8, 0xE);
    func_001F21B0(D_0015F3F8, 0xE);
    func_001F4398();
    if (!(*(s32 *)0x15F434 & 0x40)) {
        goto block_83;
    }
    if (D_0018A2B0.unk44 == 0) {
        goto block_82;
    }
    func_00233980(0x42, (0x8000 << 0x18) | 0x44);
    if (D_001872D4[0] == 0) {
        goto block_68;
    }
    func_001F5210(D_001610C0, *(s32 *)0x1610C1, *(s32 *)0x1610C2, D_001610C3);
block_68:
    if (D_0015F43C > 0.0f) {
        goto block_70;
    }
    goto block_74;
block_70:
    if (!(D_0015F43C > 1.0f)) {
        goto block_73;
    }
    D_0015F43C = 1.0f;
block_73:
    func_001F5210(0U, 0U, 0U, func_001FA6D0(D_0015F43C * 128.0f));
block_74:
    temp_2_422 = D_0015F34C;
    if (!(D_0015F440[0] > 0.0f)) {
        goto block_78;
    }
    if (!(D_0015F440[0] > 1.0f)) {
        goto block_77;
    }
    D_0015F440[0] = 1.0f;
block_77:
    func_001F5210(0xFFU, 0xFFU, 0xFFU, func_001FA6D0(D_0015F440[0] * 128.0f));
block_78:
    if (temp_2_422 == 0) {
        goto block_81;
    }
    if (D_0015F350 == 0) {
        goto block_81;
    }
    func_001F4FB8();
block_81:
block_82:
    func_001F21B8(D_0015F400, 0xA);
block_83:
    func_002334D8(D_00100AE0);
    FlushCache(0);
    temp_f1_469 = func_001FA6C0(*(s32 *)0x10000800);
    __asm__ volatile ("" : "+f" (temp_f1_469));
    var_f0_471 = 9600.0f;
    if (D_0015ED80 == 0) {
        goto block_85;
    }
    var_f0_471 = 11520.0f;
block_85:
    D_0015F614 = temp_f1_469 / var_f0_471;
    func_002337B0(2);
    func_001F21B0(D_0015F410, 0x11);
    if (!(*(s32 *)0x15F434 & 2)) {
        goto block_89;
    }
    if (D_0018A2B0.unk10 == 0) {
        goto block_88;
    }
    func_00234F98(D_001E1300);
    func_00233308();
block_88:
    func_001F21B0(D_001E78A0, 2);
block_89:
    func_002337B0(4);
    func_001F21B0(D_0015F410, 0x11);
    if (!(*(s32 *)0x15F434 & 4)) {
        goto block_97;
    }
    if (D_0018A2B0.unk18 == 0) {
        goto block_96;
    }
    if (D_0015ED80 != 0) {
        goto block_93;
    }
    goto block_95;
block_93:
    func_00235898();
    func_00235840();
    goto block_96;
block_95:
    func_00237370(D_001E3200);
    func_00235780();
block_96:
    func_001F21B0(D_0015F418, 5);
block_97:
    func_002337B0(8);
    func_001F21B0(D_0015F410, 0x11);
    if (!(*(s32 *)0x15F434 & 8)) {
        goto block_101;
    }
    if (D_0018A2B0.unk20 == 0) {
        goto block_100;
    }
    func_0022A5E0(D_001D8EB0);
    func_00228A30();
block_100:
    func_001F21B0(D_001E78B8, 7);
block_101:
    func_002337B0(0x10);
    func_001F21B0(D_0015F410, 0x11);
    if (!(*(s32 *)0x15F434 & 0x10)) {
        goto block_105;
    }
    if (D_0018A2B0.unk28 == 0) {
        goto block_104;
    }
    func_0020CEF8();
block_104:
    func_001F21B0(D_0015F428, 3);
block_105:
    func_001F2588();
    D_0015F648[0] = 0;
    return;
}
#endif /* NON_MATCHING */
