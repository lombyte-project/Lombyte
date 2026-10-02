#include "types.h"

/* Draws a vertical text menu: picks the font size from the flags, sizes
   the rows, then prints each item (and its optional subtitle) with the
   focused row highlighted, scrolling the box to keep it visible.
   Returns 1 once after flag 0x8000 is consumed, else 2. */

typedef struct {
    short s[12];
} TextBox;

typedef struct {
    s16 text;
    s16 enabled;
    s32 id;
    s16 subtext;
    s16 padA;
} MenuItem;

typedef struct {
    u8 pad0[0x20];
    s32 width;
    s32 height;
    u8 pad28[8];
    s32 flags;
    MenuItem *items;
    u8 pad38[8];
    s32 selected;
    s32 scroll;
} Menu;

typedef struct {
    u8 pad0[0x40];
    Menu *focus;
} MenuState;

extern MenuState *D_001D5BF4[];
extern u8 D_0013D408[];
extern s32 D_0015ED88;
extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern u8 D_001DF790[];

extern void vu1_add_g_sregister(s32, long) __asm__("FUN_00233980");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void EnableGlobalStateFlag(void) __asm__("func_001F61E8");
extern void DisableGlobalStateFlag(void) __asm__("func_001F61F8");
extern void font_print_window(TextBox *, long, char *, s32, s32, u8 *) __asm__("FUN_001f7090");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void draw_menu_selection_marker(s32, s32, s32) __asm__("func_0021F8E8");
extern void *memset(void *, int, unsigned int);

s32 draw_menu_text_list(Menu *menu) __asm__("FUN_0021d4a8");

s32 draw_menu_text_list(Menu *menu) {
    s32 font_size;
    s32 font_kind;
    s32 focused;
    u8 *font;
    s32 item_count;
    MenuItem *item;
    s32 row_height;
    s32 glyph_texture;
    s32 i;
    s32 selected;
    s32 enabled;
    s32 color;
    char *text;
    s32 row_bottom;
    s32 y;

    font_size = 12;
    font_kind = 1;
    font = D_001DF050;
    focused = D_001D5BF4[0]->focus == menu;
    if (menu->flags & 4) {
        font_size = 14;
        font_kind = 3;
        font = D_001DF790;
    }
    if (menu->flags & 8) {
        font_size = 10;
        font_kind = 2;
        font = D_001DF3F0;
    }
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0x2004B);
    setup_gif_paging(0);

    item_count = 0;
    item = menu->items;
    while (item->text != 0) {
        item++;
        item_count++;
    }
    if (menu->flags & 0x10) {
        row_height = font_size + 3;
    } else {
        row_height = menu->height / (item_count + 1);
    }
    y = row_height - font_size / 2;
    {
        TextBox box = { { 4, menu->height - 4, 0, menu->width - 2, 0,
                          y - menu->scroll, 0, 0, font_size + 2 } };

        glyph_texture = get_effect_texture(font_kind);
        for (i = 0; menu->items[i].text != 0; i++) {
            selected = 0;
            if (focused && menu->selected == i) {
                selected = 1;
            }
            enabled = menu->items[i].enabled != 0;
            if (menu->flags & 2) {
                color = 0x80FFA888;
            } else if (selected) {
                color = enabled ? 0x8020FFFF : 0x80006060;
            } else {
                color = enabled ? 0x80FFA888 : 0x80303030;
            }
            if (!(menu->flags & 0x10000) && selected && box.s[5] < 4) {
                menu->scroll -= 4;
            }
            text = get_help_message_text(menu->items[i].text);
            box.s[4] = (menu->flags & 0xA00) ? 0x20 : 4;
            if (menu->flags & 0x400) {
                box.s[9] = 1;
                box.s[4] = menu->width >> 1;
            }
            if (selected) {
                DisableGlobalStateFlag();
            }
            font_print_window(&box, color, text, -1, glyph_texture, font);
            if (selected) {
                EnableGlobalStateFlag();
            }
            if (menu->flags & 0x200) {
                draw_menu_selection_marker(0xF, box.s[5] + 9, D_0013D408[i] != 0);
            }
            if (menu->flags & 0x800) {
                draw_menu_selection_marker(0xF, box.s[5] + 9, D_0015ED88 == menu->items[i].id);
            }
            box.s[5] += box.s[7];
            if (menu->items[i].subtext != 0) {
                text = get_help_message_text(menu->items[i].subtext);
                box.s[4] = 0x14;
                font_print_window(&box, color, text, -1, glyph_texture, font);
                box.s[5] += row_height;
            }
            box.s[5] += 8;
            if (!(menu->flags & 0x10000) && selected) {
                row_bottom = box.s[5] + box.s[7];
                if (box.s[1] < row_bottom) {
                    if (menu->flags & 0x8000) {
                        menu->scroll += row_bottom - box.s[1];
                    } else {
                        menu->scroll += 4;
                    }
                }
            }
        }
    }
    do_gif_paging();
    if (menu->flags & 0x8000) {
        menu->flags ^= 0x8000;
        return 1;
    }
    return 2;
}

extern __typeof__(draw_menu_text_list) func_0021D4A8 __attribute__((alias("FUN_0021d4a8")));
