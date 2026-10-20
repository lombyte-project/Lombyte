#include "types.h"

struct OptItem {
    s32 text;
    u8 *value;
    s32 names[4];
};
struct OptMenu {
    u8 pad0[0x20];
    s32 x;
    s32 height;
    u8 pad28[0xC];
    struct OptItem *items;
    s32 selected;
};

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern s32 get_help_message_text(s32) __asm__("func_001FDD10");
extern void font_print_large(s32, s32, u64, s32, s32) __asm__("func_001F6530");
extern void font_print_right(s32, s32, u64, s32, s32) __asm__("func_001F6940");

s32 draw_menu_choice_list(struct OptMenu *m) __asm__("FUN_00221460");

s32 draw_menu_choice_list(struct OptMenu *menu) {
    struct OptItem *item_cursor;
    struct OptItem *it;
    s32 item_count;
    s32 i;
    s32 step;
    s32 y;
    s32 color;

    vu1_add_g_sregister(0x47, 0x2004B);
    setup_gif_paging(0);
    item_count = 0;
    for (item_cursor = menu->items; item_cursor->text != 0; item_cursor++) {
        item_count++;
    }
    step = menu->height / (item_count + 1);
    item_cursor = menu->items;
    y = step - 8;
    for (i = 0; menu->items[i].text != 0; i++) {
        it = &menu->items[i];
        if (i == menu->selected) {
            color = 0x8020FFFF;
        } else {
            color = 0x80FFA888;
        }
        font_print_large(0xC, y, color, get_help_message_text(it->text), -1);
        font_print_right(menu->x - 0xC, y, 0x80FFA888, get_help_message_text(it->names[*it->value]),
                         -1);
        y += step;
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(draw_menu_choice_list) func_00221460 __attribute__((alias("FUN_00221460")));
