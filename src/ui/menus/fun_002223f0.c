
#include "types.h"
struct PadState {
    u8 pad0[0x1A0];
    s64 unk1A0;
    u8 pad1A4[0x18];
    u32 held;
    u32 pressed;
};
struct Menu {
    u8 pad0[0x34];
    s32 str;
    u8 pad38[4];
    s32 unk3C;
    s32 unk40;
    u8 pad44[0xC];
    u32 unk50;
    s32 unk54;
};
extern struct PadState D_0013C940;
#include "rnc/ui/menus/menu_system.h"
extern s32 D_001D2AF4[];
extern u8 D_001D5098[];
extern u8 D_001D5148[];
extern u8 D_001D5208[];
extern u8 D_001D5298[];
extern u8 D_001D52B0[];
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
s32 FUN_002223f0(struct Menu *menu) {
    int flag;
    s32 n;

    flag = (D_0013C940.unk1A0 & 0xF) == 0xF;
    if ((menu_system.unkDC == 0) || (flag && (((s32)D_0013C940.unk1A0) & 0x10))) {
        if ((D_0013C940.pressed & 0xD00) && (menu_system.close_locked == 0)) {
            return 1;
        }
        if (D_0013C940.pressed & 0x10) {
            if (menu_system.current->back != 0) {
                menu_system.next = menu_system.current->back;
            } else if (menu_system.close_locked == 0) {
                return -1;
            }
        }
    }
    n = menu->unk40;
    menu->unk40 = n - 1;
    if (flag) {
        menu->unk40 = n - 2;
    }
    if (menu->unk40 < 0) {
        menu->unk40 = 0;
    }
    switch (menu->unk50) {
    case 0:
        menu->unk50 = menu->unk50 + 1;
        menu->unk40 = scale_game_frames(0xB4);
        menu->unk3C = 0;
        D_001D2AF4[0] = 0x50A9;
        menu->str = (s32)D_001D5098;
        break;

    case 1:

    case 5:

    case 8:

    case 12:

    case 16:
        if (menu->unk40 == 0) {
            menu->unk54 = 0;
            menu->unk50 = *(volatile u32 *)(&menu->unk50) + 1;
        }
        break;

    case 2:

    case 6:

    case 9:

    case 13:

    case 17:
        n = menu->unk3C;
        menu->unk3C = n + 0xA;
        if (flag) {
            menu->unk3C = n + 0x14;
        }
        if (menu->unk54 != 0) {
            menu->unk40 = scale_game_frames(0xB4);
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case 4:
        if (menu->unk40 == 0) {
            menu->unk50 = 5;
            menu->unk40 = scale_game_frames(0xB4);
            menu->unk3C = 0;
            D_001D2AF4[0] = 0x50D6;
            menu->str = (s32)D_001D5148;
        }
        break;

    case 3:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0x50D4;
            menu->str = 0x50D5;
            menu->unk40 = scale_game_frames(0xF0);
            menu->unk3C = 0;
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case 7:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0x5106;
            menu->str = (s32)D_001D5208;
            menu->unk40 = scale_game_frames(0xF0);
            menu->unk3C = 0;
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case 10:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0x5136;
            menu->str = 0x5137;
            menu->unk40 = scale_game_frames(0xF0);
            menu->unk3C = 0;
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case 11:
        if (menu->unk40 == 0) {
            menu->unk40 = scale_game_frames(0xB4);
            menu->unk3C = 0;
            D_001D2AF4[0] = 0x5138;
            menu->str = (s32)D_001D5298;
            menu->unk50 = 0xC;
        }
        break;

    case 14:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0;
            menu->str = 0x5143;
            menu->unk40 = scale_game_frames(0xF0);
            menu->unk3C = 0;
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case 15:
        if (menu->unk40 == 0) {
            menu->unk40 = scale_game_frames(0xB4);
            menu->unk3C = 0;
            D_001D2AF4[0] = 0x5144;
            menu->str = (s32)D_001D52B0;
            menu->unk50 = 0x10;
        }
        break;

    case 18:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0;
            menu->str = 0x5175;
            menu->unk50 = menu->unk50 + 1;
            menu->unk40 = scale_game_frames(0x12C);
        }
        break;

    case 19:
        if (menu->unk40 == 0) {
            if (menu_system.unkDC != 0) {
                return 1;
            }
        }
        break;
    }

    return 0;
}

extern __typeof__(FUN_002223f0) func_002223F0 __attribute__((alias("FUN_002223f0")));
