/* Ported from rac1-decomp (src/game/pause.c, func_00222B98). */
#include "types.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/rendering/vu1_registers.h"

extern void  setup_gif_paging(int) __asm__("func_001F4280");
extern void  do_gif_paging(void) __asm__("func_001F4398");
extern void *get_help_message_text(int) __asm__("func_001FDD10");
extern char  fallback_prompt_text[] __asm__("D_001602E8");
extern void
font_print_window_small(struct TextRegion *, long, char *, int) __asm__("func_001F75F0");
/* Draws the two-line prompt box (text 0x4FB3 for menu_system.unkD4 in 0..2,
   0x4FB5 for 3, else D_001602E8) sized from arg0's +0x20/+0x24. The box
   is an aggregate initializer: this compiler clears it with a memset
   libcall, fills a temporary and copies that into the local with
   ldl/ldr/sdl/sdr pairs, exactly retail's sequence. */
int draw_prompt_box(char *prompt_dimensions) __asm__("FUN_00221b50");

int draw_prompt_box(char *prompt_dimensions) {
    char *text;
    int   prompt_text_selector;

    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0x2004B);
    setup_gif_paging(0);
    text = fallback_prompt_text;
    prompt_text_selector = menu_system.unkD4;
    switch (prompt_text_selector) {
    case 0:
    case 1:
    case 2:
        text = get_help_message_text(0x4FB3);
        break;
    case 3:
        text = get_help_message_text(0x4FB5);
        break;
    }
    {
        struct TextRegion c = {1,
                               *(int *)(prompt_dimensions + 0x24) + 1,
                               1,
                               *(int *)(prompt_dimensions + 0x20) + 1,
                               *(int *)(prompt_dimensions + 0x20) >> 1,
                               5,
                               0,
                               0,
                               0x10,
                               5};

        font_print_window_small(&c, 0x80000000L, text, -1);
        c.anchor_y = (*(int *)(prompt_dimensions + 0x24) - c.rendered_height) >> 1;
        c.flags ^= 4;
        font_print_window_small(&c, 0x80000000L, text, -1);
        c.top--;
        c.bottom--;
        c.left--;
        c.right--;
        c.anchor_x--;
        c.anchor_y--;
        font_print_window_small(&c, 0x80FFA888L, text, -1);
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(draw_prompt_box) func_00221B50 __attribute__((alias("FUN_00221b50")));
