#include "types.h"
extern s32 D_001601B0;
extern s32 find_valid_animation_frame_index() __asm__("func_001FF960");
extern s32 draw_hud_sprite() __asm__("func_001FFC30");
extern s32 func_00200E08(s32, s32, s32, s32, u64, s32);
void draw_menu_selection_marker(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_0021f8e8");

void draw_menu_selection_marker(s32 arg0, s32 arg1, s32 arg2) {
    func_00200E08(arg0 - 5, arg1 - 5, arg0 + 5, arg1 + 5, (u64) 0x80FFA888, 0);
    func_00200E08(arg0 - 4, arg1 - 4, arg0 + 4, arg1 + 4, (u64) D_001601B0, 0);
    if (arg2 != 0) {
        draw_hud_sprite(find_valid_animation_frame_index(0xE99E, 1), arg0 - 0xD, arg1 - 0x12, 0x1E, 0x1E, 0x80);
    }
}

extern void func_0021F8E8(void) __attribute__((alias("FUN_0021f8e8")));
