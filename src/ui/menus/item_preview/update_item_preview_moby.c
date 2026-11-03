/* Ported from rac1-decomp (src/game/pause.c, func_0021EFA0). */
#include "sda.h"
#include "qcopy.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/data_labels.h"
#include "rnc/mobys/moby.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/mobys/delete_moby.h"
#include "rnc/ui/menus/item_preview.h"

extern struct Moby *create_menu_preview_moby(int) __asm__("func_00225490");
/* Keeps the item preview moby in step with the highlighted entry: drop it
   when the entry's class (menu_item_info preview_class) is -1, spawn it in
   front of the camera focus when there is none yet, or respawn it with
   the old one's position and orientation when the class changed. */
int update_item_preview_moby(struct MenuScreen *menu) __asm__("FUN_0021df98");

int update_item_preview_moby(struct MenuScreen *menu) {
    struct MenuScreen *grid = menu_system.current->focus;
    int                selected_item_id = grid->data.grid.cells[grid->data.grid.selected_cell].id;
    short              current_preview_class;
    MenuItemInfo      *selected_item_info;
    short              want;

    current_preview_class = menu->data.preview.moby != 0 ? menu->data.preview.moby->oclass : -1;
    selected_item_info = &menu_item_info[selected_item_id];
    want = selected_item_info->preview_class;
    if (want != -1 && current_preview_class == -1) {
        struct Moby *spawned_moby = create_menu_preview_moby(want);

        if (spawned_moby != 0) {
            struct GameCamera *camera = &game_camera;

            menu->data.preview.moby = spawned_moby;
            spawned_moby->flags = 0;
            spawned_moby->pos.x = camera->position.f[0] + 6.0f;
            spawned_moby->pos.y = camera->position.f[1];
            spawned_moby->pos.z = camera->position.f[2] - 0.3f;
            spawned_moby->rot.z = 3.1415927f;
            spawned_moby->update = (void (*)(struct Moby *))rotate_item_preview_moby;
            *(struct MenuScreen **)spawned_moby->pvars = menu;
        }
    } else if (want == -1) {
        menu->data.preview.moby = delete_moby(menu->data.preview.moby);
    } else if (current_preview_class != want) {
        struct Moby *replacement_moby = create_menu_preview_moby(want);

        if (replacement_moby != 0) {
            struct Moby *old_preview_moby;

            replacement_moby->flags = 0;
            old_preview_moby = menu->data.preview.moby;
            qcopy(&replacement_moby->pos, &old_preview_moby->pos);
            qcopy(&replacement_moby->rot, &old_preview_moby->rot);
            replacement_moby->update = old_preview_moby->update;
            *(struct MenuScreen **)replacement_moby->pvars = menu;
        }
        delete_moby(menu->data.preview.moby);
        menu->data.preview.moby = replacement_moby;
    }
    return 0;
}

extern __typeof__(update_item_preview_moby) func_0021DF98 __attribute__((alias("FUN_0021df98")));
