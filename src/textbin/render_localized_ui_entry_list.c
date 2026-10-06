#include "types.h"
#include "asm.h"

#include "types.h"

typedef struct {
    s16 text_id;
    s16 enabled;
    s32 action_value;
    s16 secondary_text_id;
    s16 fade_timer;
} MenuItem;
typedef struct {
    u8 pad0[0x20];
    s32 width;
    s32 height;
    u8 pad28[8];
    s32 flags;
    MenuItem *items;
    u8 pad38[8];
    s32 selected_entry;
} MenuDescriptor;
typedef struct {
    u8 pad0[0x40];
    MenuDescriptor *focus;
} MenuPage;
extern MenuPage *active_menu_page[] __asm__("D_001D5BF4");
extern u8 normal_font_metrics[] __asm__("D_001DF050");
extern u8 small_font_metrics[] __asm__("D_001DF3F0");
extern u8 large_font_metrics[] __asm__("D_001DF790");

extern s32 text_shadow_x __asm__("D_001601B8") __attribute__((sda));

extern s32 text_shadow_y __asm__("D_001601BC") __attribute__((sda));
extern void vu1_add_g_sregister(s32, long) __asm__("FUN_00233980");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");

extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void func_001F61E8(void);
extern void func_001F61F8(void);
extern s32 measure_text_width(char *, s32, u8 *) __asm__("func_001F6200");
extern void font_print(s32, s32, long, char *, s32, s32, u8 *) __asm__("func_001F62B0");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 FUN_0021b6d8(s32, s32, s32);
s32 render_localized_ui_entry_list(MenuDescriptor *menu) __asm__("FUN_0021b1c8");

s32 render_localized_ui_entry_list(MenuDescriptor *menu) {
    s32 focused;
    u8 *glyphs;
    s32 font_texture_index;
    s32 row_height;
    s32 half_width;
    s32 maximum_text_width;
    s32 selection_index;
    s32 font_height;
    s32 entry_count;
    MenuItem *entry;
    MenuItem *scan_entry;
    s32 entry_index;
    s32 selected_entry;
    s32 enabled;
    s32 color;
    char *text_id;
    char *secondary_text;
    s32 x;
    s32 y;
    s32 text_width;
    s32 flags;
    s32 menu_width;
    MenuItem *selected_item;
    s32 shadow_y;
    s32 shadow_x;
    font_height = 12;
    font_texture_index = 1;
    glyphs = normal_font_metrics;
    focused = active_menu_page[0]->focus == menu;
    if (menu->flags & 4) {
        font_height = 14;
        font_texture_index = 3;
        glyphs = large_font_metrics;
    }
    if (menu->flags & 8) {
        font_height = 10;
        font_texture_index = 2;
        glyphs = small_font_metrics;
    }
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0x2004B);
    setup_gif_paging(0);
    entry_count = 0;
    entry = menu->items;
    while (entry->text_id != 0) {
        entry++;
        entry_count++;
    }

    if (menu->flags & 0x10) {
        row_height = font_height + 3;
    } else {
        row_height = menu->height / (entry_count + 1);
    }
    half_width = menu->width >> 1;
    maximum_text_width = 0;
    y = (row_height - (font_height / 2)) - 1;
    if (menu->flags & 0x4000) {
        scan_entry = menu->items;
        if (scan_entry[0].text_id != 0) {
            entry_index = 0;
            selection_index = 0;
            do {
                text_width = measure_text_width(
                    get_help_message_text(scan_entry[entry_index].text_id), -1, glyphs);
                entry_index++;
                selection_index++;
                maximum_text_width =
                    (maximum_text_width < text_width) ? (text_width) : (maximum_text_width);
                scan_entry = menu->items;
            } while (menu->items[selection_index].text_id != 0);
        }
    }
    flags = menu->flags;
    menu_width = menu->width;
    if (flags & 0x20000) {
        if (menu_width < (maximum_text_width + 6)) {
            if (!(flags & 8)) {
                menu->flags = flags | 8;
                return 1;
            }
        }
    }
    entry_index = (selection_index = 0);
    maximum_text_width = (menu_width < maximum_text_width) ? (menu_width) : (maximum_text_width);
    if (menu->items[0].text_id != 0) {
        do {
            selected_entry = 0;
            if (focused) {
                selected_entry = menu->selected_entry == selection_index;
            }
            selected_item = (MenuItem *)((u32)(entry_index * sizeof(MenuItem)) + (u32)menu->items);
            enabled = selected_item->enabled != 0;
            if (menu->flags & 2) {
                color = 0x80FFA888;
            } else if (selected_entry) {
                if (enabled) {
                    goto fade_timer;
                }
                color = 0x80006060;
            } else if (enabled) {
            fade_timer:
                color = FUN_0021b6d8(selected_item->fade_timer, -1, -1);

            } else {
                color = 0x80303030;
            }
            text_id = get_help_message_text(menu->items[entry_index].text_id);
            if (menu->items[entry_index].enabled == 2) {
                text_id = get_help_message_text(0x4F54);
            }
            x = half_width - (measure_text_width(text_id, -1, glyphs) >> 1);
            if (menu->flags & 0x40) {
                x = 4;
            } else if (menu->flags & 0x4000) {
                x = half_width - (maximum_text_width >> 1);
            }
            func_001F61F8();
            font_print(x + text_shadow_x, y + text_shadow_y, 0x80000000L, text_id, -1,
                       get_effect_texture(font_texture_index), glyphs);
            func_001F61E8();
            if (menu->flags & 0x80) {
                func_001F61F8();
            }
            font_print(x, y, color, text_id, -1, get_effect_texture(font_texture_index), glyphs);
            y += row_height;
            if (menu->items[entry_index].secondary_text_id != 0) {
                func_001F61F8();
                shadow_x = x + text_shadow_x;
                shadow_y = y + text_shadow_y;
                secondary_text = get_help_message_text(menu->items[entry_index].secondary_text_id);
                font_print(shadow_x, shadow_y, 0x80000000L, secondary_text, -1,
                           get_effect_texture(font_texture_index), glyphs);
                if (!(menu->flags & 0x80)) {
                    func_001F61E8();
                }
                font_print(x, y, color,
                           get_help_message_text(menu->items[entry_index].secondary_text_id), -1,
                           get_effect_texture(font_texture_index), glyphs);
                y += row_height;
            }
            if (menu->flags & 0x80) {
                func_001F61E8();
            }
            entry_index++;
            selection_index++;
        } while (menu->items[entry_index].text_id != 0);
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(render_localized_ui_entry_list) func_0021B1C8
    __attribute__((alias("FUN_0021b1c8")));
