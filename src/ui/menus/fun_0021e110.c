#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
struct Descriptor {
    u8 pad0[0x10];
    s16 w;
    s16 h;
    u8 pad14[0xC];
};
#include "rnc/gameplay/state/item_state.h"
extern void PackImageDescriptor(struct Descriptor *, struct MenuScreen *);
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern void font_print_window_regular(struct Descriptor *, u64, u8 *, s32) __asm__("func_001F7580");
extern u8 *get_help_message_text(s32) __asm__("func_001FDD10");
extern void draw_moby_list(void *, s32) __asm__("func_0020D330");
s32 FUN_0021e110(struct MenuScreen *item) {
    struct Descriptor desc;
    struct MenuScreen *grid;

    grid = menu_system.current->focus;
    if (item_available[grid->data.grid.cells[grid->data.grid.selected_cell].id] == 0) {
        return 0;
    }
    if (item->data.preview.moby != 0) {
        draw_moby_list(item->data.preview.moby, 1);
        if (item->data.preview.second_moby != 0) {
            draw_moby_list(item->data.preview.second_moby, 1);
        }
        return 8;
    }
    setup_gif_paging(0);
    PackImageDescriptor(&desc, item);
    desc.w = 0x10;
    desc.h = 3;
    font_print_window_regular(&desc, ((u64)0x80FF << 16) | 0xA888, get_help_message_text(0x4F4D),
                              -1);
    do_gif_paging();
    return 2;
}

extern __typeof__(FUN_0021e110) func_0021E110 __attribute__((alias("FUN_0021e110")));
