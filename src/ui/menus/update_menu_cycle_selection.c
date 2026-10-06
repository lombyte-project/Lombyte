#include "types.h"

struct MenuControllerState {
    u8 pad0[0x1A4];
    s32 repeat_buttons;
    u8 pad1A8[0x1C];
    s32 pressed;
};
struct MenuCyclePage {
    u8 pad0[0x38];
    s32 back;
};
struct MenuCycleState {
    u8 pad0[4];
    struct MenuCyclePage *page;
    s32 next;
    u8 padC[0x118];
    s32 busy;
};
struct MenuCycle {
    u8 pad0[0x14];
    void *sound_target;
    u8 pad18[0x3C];
    s32 selection;
};
extern struct MenuControllerState controller_state __asm__("D_0013C940");
extern struct MenuCycleState menu_state __asm__("D_001D5BF0");
extern s32 menu_busy[] __asm__("D_001D5D14");
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");

s32 update_menu_cycle_selection(struct MenuCycle *menu) __asm__("FUN_00221e50");

/* The twelve choices wrap in either direction; back-page changes are deferred. */
s32 update_menu_cycle_selection(struct MenuCycle *menu) {
    s32 back_page;

    if ((controller_state.pressed & 0xD00) && menu_busy[0] == 0) {
        return 1;
    }
    if (controller_state.pressed & 0x10) {
        back_page = menu_state.page->back;
        if (back_page != 0) {
            menu_state.next = back_page;
        } else if (menu_state.busy == 0) {
            return -1;
        }
    }
    if (controller_state.repeat_buttons & 0x2040) {
        menu->selection = (menu->selection + 1) % 12;
        allocate_voice_for_target_entry(1, 0x11, menu->sound_target);
    } else if (controller_state.repeat_buttons & 0x8020) {
        menu->selection = (menu->selection + 11) % 12;
        allocate_voice_for_target_entry(1, 0x11, menu->sound_target);
    }
    return 0;
}

extern __typeof__(update_menu_cycle_selection) func_00221E50 __attribute__((alias("FUN_00221e50")));
