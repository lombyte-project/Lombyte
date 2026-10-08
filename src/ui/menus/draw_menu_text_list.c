#include "types.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/globals.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

/* Draws a vertical text menu: picks the font size from the flags, sizes
   the rows, then prints each item (and its optional subtitle) with the
   focused row highlighted, scrolling the box to keep it visible.
   Returns 1 once after flag 0x8000 is consumed, else 2. */

extern u8 D_0013D408[];
extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern u8 D_001DF790[];

extern void vu1_add_g_sregister(s32, long) __asm__("FUN_00233980");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void EnableGlobalStateFlag(void) __asm__("func_001F61E8");
extern void DisableGlobalStateFlag(void) __asm__("func_001F61F8");
extern void font_print_window(struct TextRegion *, long, char *, s32, s32,
                              u8 *) __asm__("FUN_001f7090");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void draw_menu_selection_marker(s32, s32, s32) __asm__("func_0021F8E8");
extern void *memset(void *, int, unsigned int);

s32 draw_menu_text_list(struct MenuScreen *menu) __asm__("FUN_0021d4a8");

s32 draw_menu_text_list(struct MenuScreen *menu) {
    s32 font_size;
    s32 font_kind;
    s32 focused;
    u8 *font;
    s32 item_count;
    struct MenuTextItem *item;
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
    focused = menu_system.current->focus == menu;
    if (menu->data.list.flags & 4) {
        font_size = 14;
        font_kind = 3;
        font = D_001DF790;
    }
    if (menu->data.list.flags & 8) {
        font_size = 10;
        font_kind = 2;
        font = D_001DF3F0;
    }
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0x2004B);
    setup_gif_paging(0);

    item_count = 0;
    item = menu->data.list.items;
    while (item->text != 0) {
        item++;
        item_count++;
    }
    if (menu->data.list.flags & 0x10) {
        row_height = font_size + 3;
    } else {
        row_height = menu->height / (item_count + 1);
    }
    y = row_height - font_size / 2;
    {
        struct TextRegion box = {
            4, menu->height - 4, 0, menu->width - 2, 0, y - menu->data.list.scroll, 0,
            0, font_size + 2};

        glyph_texture = get_effect_texture(font_kind);
        for (i = 0; menu->data.list.items[i].text != 0; i++) {
            selected = 0;
            if (focused && menu->data.list.selected == i) {
                selected = 1;
            }
            enabled = menu->data.list.items[i].action != 0;
            if (menu->data.list.flags & 2) {
                color = 0x80FFA888;
            } else if (selected) {
                color = enabled ? 0x8020FFFF : 0x80006060;
            } else {
                color = enabled ? 0x80FFA888 : 0x80303030;
            }
            if (!(menu->data.list.flags & 0x10000) && selected && box.anchor_y < 4) {
                menu->data.list.scroll -= 4;
            }
            text = get_help_message_text(menu->data.list.items[i].text);
            box.anchor_x = (menu->data.list.flags & 0xA00) ? 0x20 : 4;
            if (menu->data.list.flags & 0x400) {
                box.flags = 1;
                box.anchor_x = menu->width >> 1;
            }
            if (selected) {
                DisableGlobalStateFlag();
            }
            font_print_window(&box, color, text, -1, glyph_texture, font);
            if (selected) {
                EnableGlobalStateFlag();
            }
            if (menu->data.list.flags & 0x200) {
                draw_menu_selection_marker(0xF, box.anchor_y + 9, D_0013D408[i] != 0);
            }
            if (menu->data.list.flags & 0x800) {
                draw_menu_selection_marker(0xF, box.anchor_y + 9,
                                           game_language == menu->data.list.items[i].param.value);
            }
            box.anchor_y += box.rendered_height;
            if (menu->data.list.items[i].subtext != 0) {
                text = get_help_message_text(menu->data.list.items[i].subtext);
                box.anchor_x = 0x14;
                font_print_window(&box, color, text, -1, glyph_texture, font);
                box.anchor_y += row_height;
            }
            box.anchor_y += 8;
            if (!(menu->data.list.flags & 0x10000) && selected) {
                row_bottom = box.anchor_y + box.rendered_height;
                if (box.bottom < row_bottom) {
                    if (menu->data.list.flags & 0x8000) {
                        menu->data.list.scroll += row_bottom - box.bottom;
                    } else {
                        menu->data.list.scroll += 4;
                    }
                }
            }
        }
    }
    do_gif_paging();
    if (menu->data.list.flags & 0x8000) {
        menu->data.list.flags ^= 0x8000;
        return 1;
    }
    return 2;
}

extern __typeof__(draw_menu_text_list) func_0021D4A8 __attribute__((alias("FUN_0021d4a8")));
