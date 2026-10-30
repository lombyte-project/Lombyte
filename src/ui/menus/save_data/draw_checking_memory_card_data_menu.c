#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/text/text_region.h"

typedef struct TextRegion FontWindow;

struct ScreenDimensions {
    u8  pad0[0x160];
    s16 width;
    s16 height;
};

#include "rnc/save/memory_card_state.h"
#include "rnc/game/item_state.h"

extern struct ScreenDimensions fs_aa_buffer __asm__("D_00151780");
extern void                    setup_gif_paging(s32) __asm__("func_001F4280");
extern void                    do_gif_paging(void) __asm__("func_001F4398");
extern char                   *get_help_message_text(s32) __asm__("func_001FDD10");
extern void                   *memset(void *, s32, u32) __asm__("func_001153FC");
extern void font_print_window_small(FontWindow *, u64, char *, s32) __asm__("func_001F75F0");
extern u64  load_texture_file(s32) __asm__("func_00204CF0");
extern void
    draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, u64, u64) __asm__("FUN_001f5450");

s32 draw_checking_memory_card_data_menu(struct MenuScreen *menu) __asm__("FUN_00220348");

/* Before the card data is ready (state < 2) this prints the "checking" message
   centred in the menu, with a shadow; afterwards it draws the loaded preview
   texture across the screen. Every return 2 shares one exit. */
s32 draw_checking_memory_card_data_menu(struct MenuScreen *menu) {
    FontWindow           text_window;
    s16                  window_fields[12];
    struct MenuGridCell *entry;
    char                *text;
    s32                  state = menu->data.stream.state;
    u64                  texture;
    s32                  first;

    if (state < 2) {
        if (!(menu->data.stream.flags & 0x100)) {
            return 1;
        }
        if (memory_card_state.card[0].type != 2) {
            goto done;
        }
        if (memory_card_state.state < 3 && memory_card_state.pending_state < 0) {
            goto done;
        }
        setup_gif_paging(0);
        text =
            get_help_message_text(menu_system.card_op_pending ? menu_system.card_op_text : 0x4FB9);
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
        goto done;
    }
    if (menu->data.stream.flags & 4) {
        first = state < 4;
        entry =
            &menu_system.current->focus->data.grid.cells[menu->data.stream.loaded_entry[first ^ 1]];
        if (entry->kind == 0 && item_available[entry->id] == 0) {
            return 1;
        }
        if (entry->kind == 1 && alternate_item_available[entry->id] == 0) {
            return 1;
        }
    }
    setup_gif_paging(0);
    if (menu->data.stream.state < 4) {
        texture = load_texture_file(menu->data.stream.buffer[0]);
    } else {
        texture = load_texture_file(menu->data.stream.buffer[1]);
    }
    draw_textured_quad(0,
                       0,
                       fs_aa_buffer.width,
                       fs_aa_buffer.height,
                       0,
                       0,
                       menu->data.stream.texture_width,
                       menu->data.stream.texture_height,
                       0x80808080,
                       texture);
    do_gif_paging();
    return 0x10;
done:
    return 2;
}

extern __typeof__(draw_checking_memory_card_data_menu) func_00220348
    __attribute__((alias("FUN_00220348")));
