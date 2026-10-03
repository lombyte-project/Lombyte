#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/save_data/draw_checking_memory_card_data_menu/FUN_00220348.s", FUN_00220348);
#else
#include "types.h"

struct FontWindow {
    s16 v[12];
};

struct MemoryCardMenuEntry {
    u8 pad0[4];
    s16 type;
    s16 id;
    u8 pad8[2];
};

struct MemoryCardMenuLevel {
    u8 pad0[0x48];
    struct MemoryCardMenuEntry *entries;
};

struct MemoryCardMenuPlanet {
    u8 pad0[0x40];
    struct MemoryCardMenuLevel *level;
};

struct MemoryCardDataMenu {
    u8 pad0[0x20];
    s32 width;
    s32 height;
    u8 pad28[0xC];
    s32 flags;
    s32 texture_width;
    s32 texture_height;
    u8 pad40[4];
    s32 state;
    s32 first_texture;
    s32 second_texture;
    s32 entry_indices[2];
};

struct MemoryCardState {
    u8 pad0[8];
    s32 phase;
    u8 padC[0xC8];
    s32 unkD4;
    u8 padD8[4];
    s32 unkDC;
};

struct MemoryCardMenuGame {
    u8 pad0[0x128];
    s32 has_text;
    s32 text_id;
};

struct ScreenDimensions {
    u8 pad0[0x160];
    s16 width;
    s16 height;
};

extern struct MemoryCardState D_0013D290;
extern struct MemoryCardMenuGame D_001D5BF0;
extern struct MemoryCardMenuPlanet *D_001D5BF4;
extern u8 D_0013D4C0[];
extern u8 D_0013D388[];
extern struct ScreenDimensions D_00151780;
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void memset(void *, s32, u32);
extern void font_print_window_small(struct FontWindow *, u64, char *, s32) __asm__("func_001F75F0");
extern s64 func_00204CF0(s32);
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("func_001F5450");

s32 draw_checking_memory_card_data_menu(struct MemoryCardDataMenu *menu) __asm__("FUN_00220348");

s32 draw_checking_memory_card_data_menu(struct MemoryCardDataMenu *menu) {
    struct FontWindow text_window;
    s16 window_fields[12];
    struct MemoryCardMenuEntry *entry;
    char *text;
    s64 texture;

    if (menu->state < 2) {
        if (!(menu->flags & 0x100)) {
            return 1;
        }
        if (D_0013D290.phase != 2) {
            return 2;
        }
        if (D_0013D290.unkD4 < 3 && D_0013D290.unkDC < 0) {
            return 2;
        }
        setup_gif_paging(0);
        text = get_help_message_text(D_001D5BF0.has_text ? D_001D5BF0.text_id : 0x4FB9);
        memset(window_fields, 0, sizeof(window_fields));
        window_fields[1] = menu->height + 1;
        window_fields[0] = 1;
        window_fields[2] = 1;
        window_fields[3] = menu->width + 1;
        window_fields[4] = menu->width >> 1;
        window_fields[5] = 5;
        window_fields[8] = 16;
        window_fields[9] = 5;
        text_window = *(struct FontWindow *)window_fields;
        font_print_window_small(&text_window, 0x80000000, text, -1);
        text_window.v[5] = (menu->height - text_window.v[7]) >> 1;
        text_window.v[9] ^= 4;
        font_print_window_small(&text_window, 0x80000000, text, -1);
        text_window.v[0]--;
        text_window.v[1]--;
        text_window.v[2]--;
        text_window.v[3]--;
        text_window.v[4]--;
        text_window.v[5]--;
        font_print_window_small(&text_window, 0x80FFA888, text, -1);
        do_gif_paging();
        return 2;
    }
    if (menu->flags & 4) {
        entry = &D_001D5BF4->level->entries[*(s32 *)((u8 *)menu->entry_indices + (((menu->state < 4) ^ 1) << 2))];
        if (entry->type == 0 && D_0013D4C0[entry->id] == 0) {
            return 1;
        }
        if (entry->type == 1 && D_0013D388[entry->id] == 0) {
            return 1;
        }
    }
    setup_gif_paging(0);
    texture = func_00204CF0(menu->state < 4 ? menu->first_texture : menu->second_texture);
    draw_textured_quad(0, 0, D_00151780.width, D_00151780.height, 0, 0, menu->texture_width, menu->texture_height, 0x80808080, texture);
    do_gif_paging();
    return 0x10;
}
extern __typeof__(draw_checking_memory_card_data_menu) func_00220348 __attribute__((alias("FUN_00220348")));
#endif /* NON_MATCHING */
