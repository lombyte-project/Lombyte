#include "types.h"
struct LightLinkEntryB {
    u8 pad_0[0x1B];
    u8 unk1B;
    u8 pad_1C[0x2];
    u16 unk1E;
};

struct LightLinkEntryWide {
    u8 pad_0[0x35];
    u8 unk35;
    u16 unk36;
};

struct LightLinkEntryA {
    u8 pad_0[0x1B];
    u8 unk1B;
    u8 pad_1C[0x2];
    u16 unk1E;
};

struct LightRecord {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s32 unkC;
};

extern s32 D_001603D4[];
extern s32 D_00160E8C[];
extern s32 D_00160F50[];
extern u8 D_0019C3C0[];
extern u8 D_001E7B50[];
extern u8 D_001E7B70[];
extern u8 D_001E7B98[];
extern s32 DebugPrint();
void detach_point_light(s32 light_index) __asm__("FUN_00201f88");

void detach_point_light(s32 light_index) {
    s16 *var_17_29;
    s16 *var_18_145;
    s16 *var_18_87;
    s32 temp_20_148;
    s32 temp_20_31;
    s32 temp_20_90;
    s32 temp_21_24;
    s32 temp_22_33;
    s32 temp_23_22;
    s32 temp_30_16;
    s32 temp_3_100;
    s32 temp_3_158;
    s32 temp_3_42;
    u32 var_16_157;
    u32 var_16_41;
    u32 var_16_99;
    struct LightLinkEntryB *temp_17_156;
    struct LightLinkEntryWide *temp_17_98;
    struct LightLinkEntryA *temp_18_40;
    struct LightRecord *light_entry;

    temp_30_16 = light_index & 0xFFFF;
    light_entry = (light_index * 0x30) + D_0019C3C0;
    temp_22_33 = (light_index * 0x10) & 0xFFFF;
    temp_21_24 = (light_index << 8) & 0xFFFF;
    var_17_29 = light_entry->unkC + (light_entry->unk0 * 2);
    temp_20_31 = var_17_29 + light_entry->unk2;
    temp_23_22 = (light_index << 0xC) & 0xFFFF;
    if (var_17_29 != temp_20_31) {
        do {
            temp_18_40 = *(s32 *)0x160F50 + (*var_17_29 << 5);
            var_16_41 = temp_18_40->unk1E;
            temp_3_42 = var_16_41 & 0xF;
            if (temp_3_42 == temp_30_16) {
                var_16_41 = (var_16_41 >> 4) | 0xF000;
            } else if ((var_16_41 & 0xF0) == temp_22_33) {
                var_16_41 = temp_3_42 | ((var_16_41 >> 4) & 0xFF0) | 0xF000;
            } else if ((var_16_41 & 0xF00) == temp_21_24) {
                var_16_41 = (var_16_41 & 0xFF) | ((var_16_41 >> 4) & 0xF00) | 0xF000;
            } else if ((var_16_41 & 0xF000) == temp_23_22) {
                var_16_41 |= 0xF000;
            } else {
                DebugPrint(D_001E7B50);
            }
            var_17_29 += 1;
            if (var_16_41 == 0xFFFF) {
                temp_18_40->unk1B = 1;
            }
            temp_18_40->unk1E = var_16_41;
        } while (var_17_29 != temp_20_31);
    }
    var_18_87 = light_entry->unkC + (light_entry->unk8 * 2);
    light_entry->unk0 = 0;
    temp_20_90 = var_18_87 + light_entry->unkA;
    light_entry->unk2 = 0;
    if (var_18_87 != temp_20_90) {
        do {
            temp_17_98 = *(s32 *)0x160E8C + (*var_18_87 << 6);
            var_16_99 = temp_17_98->unk36;
            temp_3_100 = var_16_99 & 0xF;
            if (temp_3_100 == temp_30_16) {
                var_16_99 = (var_16_99 >> 4) | 0xF000;
            } else if ((var_16_99 & 0xF0) == temp_22_33) {
                var_16_99 = temp_3_100 | ((var_16_99 >> 4) & 0xFF0) | 0xF000;
            } else if ((var_16_99 & 0xF00) == temp_21_24) {
                var_16_99 = (var_16_99 & 0xFF) | ((var_16_99 >> 4) & 0xF00) | 0xF000;
            } else if ((var_16_99 & 0xF000) == temp_23_22) {
                var_16_99 |= 0xF000;
            } else {
                DebugPrint(D_001E7B70);
            }
            var_18_87 += 1;
            if (var_16_99 == 0xFFFF) {
                temp_17_98->unk35 = 1;
            }
            temp_17_98->unk36 = var_16_99;
        } while (var_18_87 != temp_20_90);
    }
    var_18_145 = light_entry->unkC + (light_entry->unk4 * 2);
    light_entry->unk8 = 0;
    temp_20_148 = var_18_145 + light_entry->unk6;
    light_entry->unkA = 0;
    if (var_18_145 != temp_20_148) {
        do {
            temp_17_156 = *(s32 *)0x1603D4 + (*var_18_145 << 5);
            var_16_157 = temp_17_156->unk1E;
            temp_3_158 = var_16_157 & 0xF;
            if (temp_3_158 == temp_30_16) {
                var_16_157 = (var_16_157 >> 4) | 0xF000;
            } else if ((var_16_157 & 0xF0) == temp_22_33) {
                var_16_157 = temp_3_158 | ((var_16_157 >> 4) & 0xFF0) | 0xF000;
            } else if ((var_16_157 & 0xF00) == temp_21_24) {
                var_16_157 = (var_16_157 & 0xFF) | ((var_16_157 >> 4) & 0xF00) | 0xF000;
            } else if ((var_16_157 & 0xF000) == temp_23_22) {
                var_16_157 |= 0xF000;
            } else {
                DebugPrint(D_001E7B98);
            }
            var_18_145 += 1;
            if (var_16_157 == 0xFFFF) {
                temp_17_156->unk1B = 1;
            }
            temp_17_156->unk1E = var_16_157;
        } while (var_18_145 != temp_20_148);
    }
    light_entry->unk4 = 0;
    light_entry->unk6 = 0;
}
