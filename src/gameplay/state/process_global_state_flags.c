#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/ui/menus/menu_system.h"

#include "rnc/globals.h"
#include "rnc/gameplay/state/item_state.h"

#include "rnc/input/pad_state.h"
extern u8 D_0013E05A[];
extern u8 D_0014BF08[];
extern s32 D_0015EEA0;
extern u8 D_001D4EC0[];
extern void InitializeGlobalStateEntry(s32);
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void memcard_restore_game(s32) __asm__("func_00209298");
extern void memcard_save_data(s32, s32) __asm__("func_0020B178");
extern void func_00226B08(s32);
extern void clear_scene_state_buffers(void) __asm__("func_00226F50");

s32 process_global_state_flags(void) __asm__("FUN_00222290");

s32 process_global_state_flags(void) {
    u8 backup[4];
    u8 old_value;
    s32 i;
    s32 result;

    if ((controller_state.pressed & 0x20) != 0) {
        if (mode_freeze_state == 1 || mode_freeze_state == 0x10) {
            menu_system.next = (struct MenuPage *)D_001D4EC0;
        } else {
            func_00226B08(-1);
            InitializeGlobalStateEntry(0);
            *(u16 *)D_0013E05A = 1;
        }
    } else if ((controller_state.pressed & 0x40) != 0) {
        old_value = item_unlocked[0x1D];
        for (i = 0; i < 4; i++) {
            backup[i] = D_0014BF08[i];
        }
        memcard_restore_game(menu_system.unkE0);
        item_unlocked[0x1D] = old_value;
        for (i = 0; i < 4; i++) {
            D_0014BF08[i] = backup[i];
        }
        D_0015EEA0 = 1;
        memcard_save_data(0, -1);
        clear_scene_state_buffers();
        result = scale_game_frames(0x10);
        fade_to_black(result);
        hero.unk20B1 = 1;
        return -1;
    }
    return 0;
}

extern __typeof__(process_global_state_flags) func_00222290 __attribute__((alias("FUN_00222290")));
