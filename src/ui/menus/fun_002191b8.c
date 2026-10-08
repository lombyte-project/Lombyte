#include "types.h"
#include "rnc/globals.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 D_001D5D90[];
extern s32 delete_moby(s32) __asm__("FUN_00225530");
void FUN_002191b8(void) {
    s32 i;
    s32 j;
    struct MenuScreen *screen;

    if (menu_system.update_count < 10) {
        return;
    }
    if (menu_system.current != 0) {
        for (i = 0; i < 14; i++) {
            screen = menu_system.current->screens[i];
            if (screen != 0 && screen->leave != 0) {
                screen->leave(screen, 0);
            }
        }
        menu_system.current = 0;
    }
    gs_texture_allocation_start = menu_system.saved_texture_start;
    for (j = 0; j < 14; j++) {
        D_001D5D90[j] = delete_moby(D_001D5D90[j]);
    }
    menu_system.state = 20;
    menu_system.timer = 2;
}

extern __typeof__(FUN_002191b8) func_002191B8 __attribute__((alias("FUN_002191b8")));
