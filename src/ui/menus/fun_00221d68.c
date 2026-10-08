#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/input/pad_state.h"

struct ModeEntry {
    u8 pad_0[0x38];
    s32 unk38;
};
struct MenuScreen {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x28];
    s32 unk40;
};
extern struct PadState D_0013C940;
__attribute__((section(".data"))) extern s32 D_001D5D14;
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 FUN_00221d68(struct MenuScreen *menu) {
    s32 prev;

    if ((D_0013C940.pressed & 0xD00) && D_001D5D14 == 0) {
        return 1;
    }
    if (D_0013C940.pressed & 0x10) {
        if (menu_system.current->unk38 != 0) {
            menu_system.next = menu_system.current->unk38;
        } else if (menu_system.unk124 == 0) {
            return -1;
        }
    }
    prev = menu->unk40;
    if (D_0013C940.unk1A4 & 0x40) {
        menu->unk40 = (prev + 1) % 30;
    } else if (D_0013C940.unk1A4 & 0x20) {
        menu->unk40 = (prev + 29) % 30;
    }
    if (menu->unk40 != prev) {
        allocate_voice_for_target_entry(1, 0x11, menu->unk14);
    }
    return 0;
}
