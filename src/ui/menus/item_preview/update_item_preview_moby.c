/* Ported from rac1-decomp (src/game/pause.c, func_0021EFA0). */
#include "sda.h"
#include "qcopy.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/mobys/delete_moby.h"

extern void *create_menu_preview_moby(int) __asm__("func_00225490");
extern void  rotate_item_preview_moby(struct MenuScreen *) __asm__("FUN_0021e1f8");
/* Keeps the item preview moby in step with the highlighted entry: drop it
   when the entry's class (menu_item_info preview_class) is -1, spawn it in
   front of the camera focus when there is none yet, or respawn it with
   the old one's position and orientation when the class changed. */
int update_item_preview_moby(struct MenuScreen *menu) __asm__("FUN_0021df98");

int update_item_preview_moby(struct MenuScreen *menu) {
    struct MenuScreen *grid = menu_system.current->focus;
    int                item = grid->data.grid.cells[grid->data.grid.selected_cell].id;
    short              current_preview_class;
    MenuItemInfo      *rec;
    short              want;

    current_preview_class =
        menu->data.preview.moby != 0 ? *(short *)(menu->data.preview.moby + 0xA6) : -1;
    rec = &menu_item_info[item];
    want = rec->preview_class;
    if (want != -1 && current_preview_class == -1) {
        char *spawned_moby = create_menu_preview_moby(want);

        if (spawned_moby != 0) {
            char *preview_camera_base = (char *)&preview_camera;

            menu->data.preview.moby = spawned_moby;
            *(short *)(spawned_moby + 0x34) = 0;
            *(float *)(spawned_moby + 0x10) = *(float *)(preview_camera_base + 0x140) + 6.0f;
            *(float *)(spawned_moby + 0x14) = *(float *)(preview_camera_base + 0x144);
            *(float *)(spawned_moby + 0x18) = *(float *)(preview_camera_base + 0x148) - 0.3f;
            *(float *)(spawned_moby + 0x48) = 3.1415927f;
            *(void **)(spawned_moby + 0x74) = (void *)rotate_item_preview_moby;
            **(void ***)(spawned_moby + 0x78) = menu;
        }
    } else if (want == -1) {
        menu->data.preview.moby = delete_moby(menu->data.preview.moby);
    } else if (current_preview_class != want) {
        char *replacement_moby = create_menu_preview_moby(want);

        if (replacement_moby != 0) {
            char *old;

            *(short *)(replacement_moby + 0x34) = 0;
            old = menu->data.preview.moby;
            qcopy(replacement_moby + 0x10, old + 0x10);
            qcopy(replacement_moby + 0x40, old + 0x40);
            *(int *)(replacement_moby + 0x74) = *(int *)(old + 0x74);
            **(void ***)(replacement_moby + 0x78) = menu;
        }
        delete_moby(menu->data.preview.moby);
        menu->data.preview.moby = replacement_moby;
    }
    return 0;
}

extern __typeof__(update_item_preview_moby) func_0021DF98 __attribute__((alias("FUN_0021df98")));
