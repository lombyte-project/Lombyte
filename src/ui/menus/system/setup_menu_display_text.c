/* Ported from rac1-decomp (src/game/pause.c, func_0021FF80). */

#include "rnc/ui/map/map_state.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/sdk/libc/stdio.h"
#include "rnc/mobys/moby.h"
#include "rnc/ui/menus/menu_screen.h"

extern void        setup_gif_paging(int) __asm__("func_001F4280");
extern void        do_gif_paging(void) __asm__("func_001F4398");
extern void        draw_moby_list(void *, int) __asm__("func_0020D330");
extern void       *get_help_message_text(int) __asm__("func_001FDD10");
extern const float setup_menu_moby_camera_y_offset __asm__("D_001602A4") __attribute__((sda));
extern char        setup_menu_text_format[] __asm__("D_001602A8");
extern int         menu_display_value_by_level[] __asm__("D_001E0888");

#include "rnc/rendering/screen.h"

extern void font_print_right(int, int, long, char *, int) __asm__("func_001F6940");
int         setup_menu_display_text(struct MenuScreen *menu) __asm__("FUN_0021ef78");

int setup_menu_display_text(struct MenuScreen *menu) {
    char                  formatted_menu_text[0x100];
    struct GameCamera *camera = &game_camera;
    struct Moby          *preview_moby = menu->data.preview.moby;
    int                   count;

    preview_moby->pos.x = camera->position.f[0] + 8.0f;
    preview_moby->pos.y = camera->position.f[1] + setup_menu_moby_camera_y_offset;
    preview_moby->pos.z = camera->position.f[2] - 0.1f;
    if (menu->data.preview.moby != 0) {
        draw_moby_list(menu->data.preview.moby, 1);
    }
    setup_gif_paging(0);
    count = 0;
    {
        int k;
        for (k = 0; k < 4; k++) {
            if (D_0014BEC0[level_map_selection.level * 4 + k] != 0) {
                count = count + 1;
            }
        }
    }
    sprintf(formatted_menu_text,
            setup_menu_text_format,
            get_help_message_text(0x4F4F),
            count,
            get_help_message_text(0x4F53),
            menu_display_value_by_level[level_map_selection.level]);
    font_print_right(
        menu->width - 0x10, (screen_extent.height >> 1) - 8, 0x80000000L, formatted_menu_text, -1);
    font_print_right(
        menu->width - 0x11, (screen_extent.height >> 1) - 9, 0x80FFA888L, formatted_menu_text, -1);
    do_gif_paging();
    return 8;
}

extern __typeof__(setup_menu_display_text) func_0021EF78 __attribute__((alias("FUN_0021ef78")));
