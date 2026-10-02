#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/audio/sound_options_menu/FUN_0021cb30.s", FUN_0021cb30);
#else
#include "types.h"
#include "sda.h"

struct M2c_D_0013C940 {
    u8 pad_0[0x1C0];
    s32 unk1C0;
    s32 unk1C4;
};

struct M2c_D_0013E550 {
    u8 pad_0[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    struct M2c_D_001D5BF0_unk4 *unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
};

struct M2c_D_001D5BF0_unk4 {
    u8 pad_0[0x38];
    s32 unk38;
};

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x18];
    s32 unk30;
    u8 pad_34[0xC];
    s32 unk40;
};

extern struct M2c_D_0013C940 D_0013C940;
extern struct M2c_D_0013E550 D_0013E550;
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 D_001D5D14 NOT_SDA;
extern s32 *D_001D5BF4 NOT_SDA;
extern s32 D_0015EDEC MACRO_ADDR;
extern s32 D_0015EDF0 MACRO_ADDR;
extern s32 D_0015EDE8 MACRO_ADDR;
extern s32 D_001A0314 NOT_SDA;
extern s32 *D_001601E0 __attribute__((sda));
extern s32 func_0022DA68(s32 arg0, s32 arg1, s32 arg2);
void snd_set_playback_mode(s32 arg0) __asm__("FUN_0012e240");

s32 sound_options_menu(struct M2c_arg0 *arg0) __asm__("FUN_0021cb30");

s32 sound_options_menu(struct M2c_arg0 *arg0) {
    s32 var_40;
    s32 *music;
    s32 var_EDE8;
    s32 var_EDEC;
    s32 var_EDF0;

    if (D_0013C940.unk1C4 & 0xD00) {
        if (D_001D5D14 == 0) {
            return 1;
        }
    }
    if (D_0013C940.unk1C4 & 0x10) {
        s32 temp = D_001D5BF0.unk4->unk38;

        if (temp != 0) {
            D_001D5BF0.unk8 = temp;
        } else if (D_001D5BF0.unk124 == 0) {
            return -1;
        }
    }
    var_40 = arg0->unk40;
    if (D_0013C940.unk1C4 & 0x1000) {
        arg0->unk40 = (var_40 + 2) % 3;
    }
    if (D_0013C940.unk1C4 & 0x4000) {
        arg0->unk40 = (arg0->unk40 + 1) % 3;
    }
    if ((arg0->unk40 != var_40) || (D_001D5BF4[0x20] != 0)) {
        func_0022DA68(1, 0x11, arg0->unk14);
        if (arg0->unk30 & 0x20) {
            D_001A0314 = D_001601E0[arg0->unk40];
        }
    }
    var_EDEC = D_0015EDEC;
    var_EDF0 = D_0015EDF0;
    music = &D_0015EDF0;
    if (D_0013C940.unk1C0 & 0x2000) {
        if (arg0->unk40 == 0) {
            D_0015EDF0 = (0x400 < D_0015EDF0 + 3) ? 0x400 : D_0015EDF0 + 3;
        }
        if (arg0->unk40 == 1) {
            D_0015EDEC = (0x400 < D_0015EDEC + 3) ? 0x400 : D_0015EDEC + 3;
        }
    }
    if (D_0013C940.unk1C0 & 0x8000) {
        if (arg0->unk40 == 0) {
            D_0015EDF0 = (D_0015EDF0 - 3 <= 0) ? 0 : D_0015EDF0 - 3;
        }
        if (arg0->unk40 == 1) {
            D_0015EDEC = (D_0015EDEC - 3 <= 0) ? 0 : D_0015EDEC - 3;
        }
    }
    if ((var_EDEC != D_0015EDEC) || (var_EDF0 != *music)) {
        D_0013E550.unk50 = D_0015EDEC;
        D_0013E550.unk4C = D_0015EDEC;
        D_0013E550.unk48 = *music * 8 / 10;
        D_0013E550.unk54 = D_0013E550.unk58 = *music * 7 / 10;
        D_0013E550.unk5C = *music;
    }
    if (D_0013C940.unk1C4 & 0x40) {
        if (arg0->unk40 == 2) {
            D_0015EDE8 = !D_0015EDE8;
        }
        snd_set_playback_mode(!D_0015EDE8);
        func_0022DA68(0, 0x11, arg0->unk14);
    }
    return 0;
}
#endif /* NON_MATCHING */
