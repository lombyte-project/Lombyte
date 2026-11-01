#include "rnc/rendering/vu1_registers.h"
#include "rnc/rendering/texture/pack_image_descriptor.h"
#include "rnc/ui/menus/menu_screen.h"

/* Ported from rac1-decomp (src/game/pause.c, func_00223810). */
extern void  setup_gif_paging(int) __asm__("func_001F4280");
extern void  do_gif_paging(void) __asm__("func_001F4398");
extern void *get_help_message_text(int) __asm__("func_001FDD10");
extern void  font_print_window_regular(void *, long, void *, int) __asm__("func_001F7580");
/* The line count's low nibble goes through `& 0xF` (retail's lbu + dsrl)
   and the start subtracts its own variable, which keeps retail's order. */
int draw_menu_text_box(struct MenuScreen *menu) __asm__("FUN_00222768");

int draw_menu_text_box(struct MenuScreen *menu) {
    enum {
        MENU_TEXT_DRAW_NONE = 0
    };
    short box[12];
    int   ypos;
    int   text_start_y_offset;
    int   i;

    vu1_add_g_sregister(0x47, 0x30000);
    vu1_add_g_sregister(0x42, 0x8000000044L);
    setup_gif_paging(0);
    pack_image_descriptor(box, menu);
    box[9] = 9;
    box[3] = menu->x + menu->width;
    box[0] = menu->y + 4;
    box[1] = menu->y + menu->height - 4;
    box[2] = menu->x;
    box[4] = menu->x + (menu->width >> 1);
    box[10] = 0;
    box[11] = 0;
    switch (menu->data.timed.phase) {
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 14:
    case 16:
    case 17:
    case 18:
        text_start_y_offset = (menu->data.timed.scroll >> 4) - 4;
        ypos = menu->y - text_start_y_offset;
        for (i = 0; ((int *)menu->data.timed.text)[i] != -1; i++) {
            box[5] = ypos;
            box[11] = (menu->data.timed.scroll >> 4) & 0xF;
            font_print_window_regular(
                box, 0x80FFA888L, get_help_message_text(((int *)menu->data.timed.text)[i]), -1);
            ypos += box[7];
            ypos += 10;
        }
        if (ypos + 0x18 < menu->y + menu->height) {
            menu->data.timed.list_done = 1;
        }
        break;
    case 4:
    case 11:
    case 15:
    case 19:
        box[5] = menu->y + 0x20;
        font_print_window_regular(
            box, 0x80FFA888L, get_help_message_text(menu->data.timed.text), -1);
        break;
    case MENU_TEXT_DRAW_NONE:
        break;
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(draw_menu_text_box) func_00222768 __attribute__((alias("FUN_00222768")));
