#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/save_data/"
            "draw_checking_memory_card_data_menu/FUN_00220348.s",
            FUN_00220348);
#else
#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/text/text_region.h"

typedef struct TextRegion FontWindow;

struct ScreenDimensions {
    u8 pad0[0x160];
    s16 width;
    s16 height;
};

#include "rnc/storage/memory_card/memory_card_state.h"
/* menu_system.current under its own label: retail loads it with a separate
   %hi/%lo pair instead of reusing the menu_system base (the unit loses
   exactness when it is spelled menu_system.current). */
extern struct MenuPage *active_menu_page __asm__("D_001D5BF4");
extern u8 item_available[] __asm__("D_0013D4C0");
extern u8 alternate_item_available[] __asm__("D_0013D388");
extern struct ScreenDimensions screen_dimensions __asm__("D_00151780");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void *memset(void *, s32, u32) __asm__("func_001153FC");
extern void font_print_window_small(FontWindow *, u64, char *, s32) __asm__("func_001F75F0");
extern u64 func_00204CF0(s32);
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, u64,
                               u64) __asm__("func_001F5450");

s32 draw_checking_memory_card_data_menu(struct MenuScreen *menu) __asm__("FUN_00220348");

s32 draw_checking_memory_card_data_menu(struct MenuScreen *menu) {
    FontWindow text_window;
    s16 window_fields[12];
    struct MenuGridCell *entry;
    char *text;
    s32 state = menu->data.stream.state;
    s32 entry_offset;

    if (state < 2) {
        if (!(menu->data.stream.flags & 0x100)) {
            return 1;
        }
        if (memory_card_state.card[0].type != 2) {
            return 2;
        }
        if (memory_card_state.state < 3 && memory_card_state.pending_state < 0) {
            return 2;
        }
        setup_gif_paging(0);
        text = get_help_message_text(menu_system.card_op_pending ? menu_system.card_op_text : 0x4FB9);
        memset(window_fields, 0, sizeof(window_fields));
        window_fields[1] = menu->height + 1;
        window_fields[0] = 1;
        window_fields[2] = 1;
        window_fields[3] = menu->width + 1;
        window_fields[4] = menu->width >> 1;
        window_fields[5] = 5;
        window_fields[8] = 16;
        window_fields[9] = 5;
        text_window = *(FontWindow *)window_fields;
        font_print_window_small(&text_window, 0x80000000, text, -1);
        text_window.anchor_y = (menu->height - text_window.rendered_height) >> 1;
        text_window.flags ^= 4;
        font_print_window_small(&text_window, 0x80000000, text, -1);
        text_window.top--;
        text_window.bottom--;
        text_window.left--;
        text_window.right--;
        text_window.anchor_x--;
        text_window.anchor_y--;
        font_print_window_small(&text_window, 0x80FFA888, text, -1);
        do_gif_paging();
        return 2;
    }
    if (menu->data.stream.flags & 4) {
        entry_offset = state < 4 ? 0 : sizeof(menu->data.stream.loaded_entry[0]);
        entry = &active_menu_page->focus->data.grid
                     .cells[*(s32 *)((u8 *)menu->data.stream.loaded_entry + entry_offset)];
        if (entry->kind == 0 && item_available[entry->id] == 0) {
            return 1;
        }
        if (entry->kind == 1 && alternate_item_available[entry->id] == 0) {
            return 1;
        }
    }
    setup_gif_paging(0);
    draw_textured_quad(0, 0, screen_dimensions.width, screen_dimensions.height, 0, 0,
                       menu->data.stream.texture_width, menu->data.stream.texture_height,
                       0x80808080,
                       func_00204CF0(menu->data.stream.state < 4 ? menu->data.stream.buffer[0]
                                                                 : menu->data.stream.buffer[1]));
    do_gif_paging();
    return 0x10;
}
extern __typeof__(draw_checking_memory_card_data_menu) func_00220348
    __attribute__((alias("FUN_00220348")));
#endif /* NON_MATCHING */
