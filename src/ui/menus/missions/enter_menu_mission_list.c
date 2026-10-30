#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"

struct Obj {
    u8  pad0[0x30];
    s32 slots[19];
    s32 handle;
};

extern void update_mission_list(struct Obj *) __asm__("FUN_0020b950");
extern s32  collect_mission_ids(void *, s32, void *, s32) __asm__("FUN_0020bc00");
s32         enter_menu_mission_list(struct Obj *obj) __asm__("FUN_0021c420");

s32 enter_menu_mission_list(struct Obj *mission_list_menu) {
    s32 *slot;

    update_mission_list(mission_list_menu);
    mission_list_menu->handle = collect_mission_ids((void *)0x70000000, 0, (void *)0x70000100, 1);
    slot = mission_list_menu->slots;
    slot += selected_level_index[0];
    if (*slot != -1) {
        D_001CF874[0] = ((s32 *)0x70000000)[*slot];
        D_001CF758[0] = ((s32 *)0x70000100)[*slot];
    }
    return 0;
}

extern __typeof__(enter_menu_mission_list) func_0021C420 __attribute__((alias("FUN_0021c420")));
