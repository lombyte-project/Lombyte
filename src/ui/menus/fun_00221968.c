#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/globals.h"
#include "rnc/ui/menus/menu_screen.h"

extern s32 D_0013CB04[];
extern s32 D_001A0314[];
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 FUN_00221968(struct MenuScreen *arg0) {
    if (D_0013CB04[0] & 0x10) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    } else if (D_0013CB04[0] & 0x800) {
        D_001A0314[0] = current_level_index;
        return 1;
    } else if (D_0013CB04[0] & 0x40) {
        allocate_voice_for_target_entry(0, 0x11, arg0->moby);
        return 1;
    } else if (D_0013CB04[0] & 0x20) {
        menu_system.unkE4 = D_001A0314[0];
        menu_system.unkF0 = menu_system.current;
        menu_system.close_request = 3;
        menu_system.unkF4 = 0xF;
        allocate_voice_for_target_entry(0, 0x11, arg0->moby);
    }
    return 0;
}

extern __typeof__(FUN_00221968) func_00221968 __attribute__((alias("FUN_00221968")));
