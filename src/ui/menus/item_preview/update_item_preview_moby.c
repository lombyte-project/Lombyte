/* Ported from rac1-decomp (src/game/pause.c, func_0021EFA0). */
#include "sda.h"
#include "qcopy.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
extern char *delete_moby(char *) __asm__("FUN_00225530");
extern char D_001863D0[];
extern char D_00186F40[];
extern void *create_menu_preview_moby(int) __asm__("func_00225490");
extern void rotate_item_preview_moby(struct MenuScreen *) __asm__("FUN_0021e1f8");
/* Keeps the item preview moby in step with the highlighted entry: drop it
   when the entry's class (D_001863D0 record +0x3A) is -1, spawn it in
   front of the camera focus when there is none yet, or respawn it with
   the old one's position and orientation when the class changed. */
int update_item_preview_moby(struct MenuScreen *menu) __asm__("FUN_0021df98");

int update_item_preview_moby(struct MenuScreen *menu) {
    struct MenuScreen *grid = menu_system.current->focus;
    int item = grid->data.grid.cells[grid->data.grid.selected_cell].id;
    short cur;
    char *rec;
    short want;

    cur = menu->data.preview.moby != 0 ? *(short *)(menu->data.preview.moby + 0xA6) : -1;
    rec = D_001863D0 + item * 0x4C;
    want = *(short *)(rec + 0x3A);
    if (want != -1 && cur == -1) {
        char *o = create_menu_preview_moby(want);

        if (o != 0) {
            char *t = D_00186F40;

            menu->data.preview.moby = o;
            *(short *)(o + 0x34) = 0;
            *(float *)(o + 0x10) = *(float *)(t + 0x140) + 6.0f;
            *(float *)(o + 0x14) = *(float *)(t + 0x144);
            *(float *)(o + 0x18) = *(float *)(t + 0x148) - 0.3f;
            *(float *)(o + 0x48) = 3.1415927f;
            *(void **)(o + 0x74) = (void *)rotate_item_preview_moby;
            **(void ***)(o + 0x78) = menu;
        }
    } else if (want == -1) {
        menu->data.preview.moby = delete_moby(menu->data.preview.moby);
    } else if (cur != want) {
        char *n = create_menu_preview_moby(want);

        if (n != 0) {
            char *old;

            *(short *)(n + 0x34) = 0;
            old = menu->data.preview.moby;
            qcopy(n + 0x10, old + 0x10);
            qcopy(n + 0x40, old + 0x40);
            *(int *)(n + 0x74) = *(int *)(old + 0x74);
            **(void ***)(n + 0x78) = menu;
        }
        delete_moby(menu->data.preview.moby);
        menu->data.preview.moby = n;
    }
    return 0;
}

extern __typeof__(update_item_preview_moby) func_0021DF98 __attribute__((alias("FUN_0021df98")));
