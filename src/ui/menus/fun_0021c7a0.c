#include "types.h"

struct PadState {
    u8 pad_0[0x1C4];
    s32 unk1C4;
};
struct Globals_00141848 {
    u8 pad_0[0xA8];
    u16 unkA8;
    u16 unkAA;
    s32 unkAC;
};
struct MenuListInfo {
    u8 pad_0[0x3C];
    s32 unk3C;
    u8 pad_40[0x8];
    s32 unk48;
};
struct ModeRef {
    u8 pad_0[0x40];
    struct MenuListInfo *unk40;
};
struct MenuScreen {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x18];
    s32 unk30[8];
    s32 unk50;
};

extern struct PadState D_0013C940;
extern u8 D_0013D4C0[];
extern struct Globals_00141848 D_00141848;
extern s32 D_0015ED84;
extern s32 D_0015EEA4;
extern struct ModeRef *D_001D5BF4[];
extern s32 scale_game_frames() __asm__("FUN_001f96f8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 FUN_0021c7a0(struct MenuScreen *menu) {
    s16 id;
    s32 *slots;
    s32 *scan;
    s32 cur;
    s32 tmp;
    int index;
    int start_index;
    int i;
    struct MenuListInfo *table;

    table = D_001D5BF4[0]->unk40;
    index = table->unk3C;
    id = *((s16 *)(((u8 *)(table->unk48 + (index * 0xA))) + 0x6));
    start_index = menu->unk50;
    if (D_0013C940.unk1C4 & 8) {
        menu->unk50 = (start_index + 1) % 8;
    }
    if (D_0013C940.unk1C4 & 4) {
        tmp = menu->unk50;
        menu->unk50 = (tmp + 7) % 8;
    }
    cur = menu->unk50;
    if (cur != start_index) {
        allocate_voice_for_target_entry(1, 0x11, menu->unk14);
    }
    if (id != 0) {
        if (D_0013D4C0[id] != 0 && (D_0013C940.unk1C4 & 0x40)) {
            if ((u16)D_00141848.unkA8 <= 0xFFFEU) {
                D_00141848.unkA8 = (u16)(D_00141848.unkA8 + 1);
            }
            tmp = scale_game_frames(D_0015EEA4) / 600;
            if ((s32)D_00141848.unkAA < tmp) {
                D_00141848.unkAA = (u16)(scale_game_frames(D_0015EEA4) / 600);
            }
            slots = menu->unk30;
            D_00141848.unkAC = (s32)((D_00141848.unkAC | (1 << D_0015ED84)) | 0x80000000);
            i = 0;
            if (menu->unk30[0] != id) {
                scan = slots;
            loop_17:
                i += 1;
                scan += 1;
                if (i < 8) {
                    if ((*scan) == id) {
                        goto block_19;
                    }
                    goto loop_17;
                }
            } else {
            block_19:
                if (i < 8) {
                    menu->unk30[i] = 0;
                }
            }
            menu->unk30[menu->unk50] = (s32)id;
            menu->unk50 = (menu->unk50 + 1) % 8;
        }
    }
    return 0;
}

extern __typeof__(FUN_0021c7a0) func_0021C7A0 __attribute__((alias("FUN_0021c7a0")));
