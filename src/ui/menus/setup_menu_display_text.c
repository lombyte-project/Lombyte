/* Ported from rac1-decomp (src/game/pause.c, func_0021FF80). */

#include "rnc/ui/map/map_state.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
extern void setup_gif_paging(int) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void draw_moby_list(int, int) __asm__("func_0020D330");
extern void *func_001FE540_id(int) __asm__("func_001FDD10");
extern const float D_001602A4_f __asm__("D_001602A4") __attribute__((sda));
extern char D_001602A8[];
extern int D_001E0888[];
#include "rnc/rendering/screen.h"
extern int sprintf(char *, const char *, ...);
extern void func_001F6CF8_c(int, int, long, char *, int) __asm__("func_001F6940");
int setup_menu_display_text(char *menu) __asm__("FUN_0021ef78");

int setup_menu_display_text(char *menu) {
    char formatted_menu_text[0x100];
    char *preview_camera_state = preview_camera;
    char *menu_moby_list = *(char **)(menu + 0x44);
    int count;

    *(float *)(menu_moby_list + 0x10) = *(float *)(preview_camera_state + 0x140) + 8.0f;
    *(float *)(menu_moby_list + 0x14) = *(float *)(preview_camera_state + 0x144) + D_001602A4_f;
    *(float *)(menu_moby_list + 0x18) = *(float *)(preview_camera_state + 0x148) - 0.1f;
    if (*(int *)(menu + 0x44) != 0) {
        draw_moby_list(*(int *)(menu + 0x44), 1);
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
    sprintf(formatted_menu_text, D_001602A8, func_001FE540_id(0x4F4F), count, func_001FE540_id(0x4F53),
            D_001E0888[level_map_selection.level]);
    func_001F6CF8_c(*(int *)(menu + 0x20) - 0x10, (screen_extent.height >> 1) - 8, 0x80000000L, formatted_menu_text, -1);
    func_001F6CF8_c(*(int *)(menu + 0x20) - 0x11, (screen_extent.height >> 1) - 9, 0x80FFA888L, formatted_menu_text, -1);
    do_gif_paging();
    return 8;
}

extern __typeof__(setup_menu_display_text) func_0021EF78 __attribute__((alias("FUN_0021ef78")));
