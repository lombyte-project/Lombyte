#include "types.h"
struct PadState {
    u8 pad_0[0x1C4];
    s32 unk1C4;
};

struct ModeEntry {
    u8 pad_0[0x38];
    s32 unk38;
    u8 pad_3C[0x4];
    void *unk40;
};

struct Globals_001D5BF0 {
    u8 pad_0[0x4];
    struct ModeEntry *unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
};

typedef struct MenuItem {
    s32 unk0;
    u8 *value;
    s32 option[4];
} MenuItem;

typedef struct Menu {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x1C];
    MenuItem *items;
    s32 cursor;
} Menu;

extern struct PadState D_0013C940;
extern struct Globals_001D5BF0 D_001D5BF0;
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 FUN_002212b8(Menu *menu) {
    MenuItem *item;
    s32 *first;
    s32 *opt;
    s32 prev;
    s32 cur;
    s32 n;

    if (D_001D5BF0.unk4->unk40 != menu) {
        return 0;
    }
    if ((D_0013C940.unk1C4 & 0xD00) && D_001D5BF0.unk124 == 0) {
        return 1;
    }
    if (D_0013C940.unk1C4 & 0x10) {
        if (D_001D5BF0.unk4->unk38 != 0) {
            D_001D5BF0.unk8 = D_001D5BF0.unk4->unk38;
        } else if (D_001D5BF0.unk124 == 0) {
            return -1;
        }
    }
    prev = menu->cursor;
    if ((D_0013C940.unk1C4 & 0x1000) && prev != 0) {
        menu->cursor = prev - 1;
    }
    if (D_0013C940.unk1C4 & 0x4000) {
        cur = menu->cursor;
        if (menu->items[cur + 1].unk0 != 0) {
            menu->cursor = cur + 1;
        }
    }
    if (prev != menu->cursor) {
        allocate_voice_for_target_entry(1, 0x11, menu->unk14);
    }
    item = &menu->items[menu->cursor];
    first = item->option;
    n = 0;
    if (first[0] != 0) {
        opt = &item->option[0];
        do {
            n++;
            opt++;
            if (*opt == 0) {
                break;
            }
        } while (n < 4);
    }
    if (D_0013C940.unk1C4 & 0x40) {
        if (item->value != NULL) {
            *item->value = (*item->value + 1) % n;
            allocate_voice_for_target_entry(0, 0x11, menu->unk14);
        }
    }
    return 0;
}
