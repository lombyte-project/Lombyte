#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/draw_cheats_menu/FUN_00221030.s",
            FUN_00221030);
#else
#include "types.h"
#include "rnc/ui/text/text_region.h"

typedef struct TextRegion FontWindow;

struct CheatMenuEntry {
    s32 text_id;
    u8 *enabled_flag;
    s32 enabled_text_id;
    s32 disabled_text_id;
    s32 reserved10;
};

struct CheatsMenu {
    u8 pad0[0x20];
    s32 width;
    s32 height;
    u8 pad28[8];
    s32 flags;
    struct CheatMenuEntry *entries;
    s32 selected_entry;
};

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void func_001153FC(void *, s32, u32);
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void font_print_window_regular(FontWindow *, u64, char *, s32) __asm__("func_001F7580");
extern void font_print_large(s32, s32, u64, char *, s32) __asm__("func_001F6530");
extern void font_print_right(s32, s32, u64, char *, s32) __asm__("func_001F6940");

s32 draw_cheats_menu(struct CheatsMenu *menu) __asm__("FUN_00221030");

s32 draw_cheats_menu(struct CheatsMenu *menu) {
    FontWindow text_window;
    s16 window_fields[12];
    struct CheatMenuEntry *entry;
    s32 entry_count;
    s32 entry_index;
    s32 draw_index;
    s32 draw_y;
    s32 line_spacing;
    s32 color;
    s32 enabled;

    vu1_add_g_sregister(0x47, 0x2004B);
    setup_gif_paging(0);
    if ((menu->flags & 1) && menu->entries->text_id == 0) {
        func_001153FC(window_fields, 0, sizeof(window_fields));
        window_fields[1] = menu->height + 1;
        window_fields[3] = menu->width + 1;
        window_fields[4] = menu->width >> 1;
        window_fields[0] = 1;
        window_fields[2] = 1;
        window_fields[5] = menu->height / 3;
        window_fields[8] = 16;
        window_fields[9] = 1;

        text_window = *(FontWindow *)window_fields;
        font_print_window_regular(&text_window, 0x80FFA888, get_help_message_text(0x4FC0), -1);
    }
    /* Entries end at a zero text ID; each retail entry occupies 0x14 bytes. */
    entry_count = 0;
    while (menu->entries[entry_count].text_id != 0) {
        entry_count++;
    }
    /* Retail keeps row spacing in s8 and the entry offset in s7. */
    line_spacing = menu->height / (entry_count + 1);
    draw_y = line_spacing - 8;
    entry_index = 0;
    draw_index = 0;
    if (menu->entries[0].text_id != 0) {
        do {
            entry = &menu->entries[draw_index];
            color = entry_index == menu->selected_entry ? 0x8020FFFF : 0x80FFA888;
            enabled = 0;
            if (entry->enabled_flag != 0) {
                enabled = *entry->enabled_flag;
            }
            font_print_large(0xC, draw_y, color, get_help_message_text(entry->text_id), -1);
            font_print_right(
                menu->width - 0xC, draw_y, 0x80FFA888,
                get_help_message_text(enabled ? entry->enabled_text_id : entry->disabled_text_id),
                -1);
            draw_y += line_spacing;
            draw_index++;
            entry_index++;
        } while (menu->entries[entry_index].text_id != 0);
    }
    do_gif_paging();
    return 2;
}
extern __typeof__(draw_cheats_menu) func_00221030 __attribute__((alias("FUN_00221030")));
#endif /* NON_MATCHING */
