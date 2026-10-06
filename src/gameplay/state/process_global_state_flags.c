#include "types.h"
#include "rnc/gameplay/state/process_global_state_flags.h"

extern u32 D_0013CAE4[];
extern struct Globals_0013D408 D_0013D408;
extern u8 D_0013E05A[];
extern struct GameState D_0013F350;
extern u8 D_0014BF08[];
extern s32 D_0015EEA0;
extern s32 D_0015EEB0;
extern u8 D_001D4EC0[];
extern struct Globals_001D5BF0 D_001D5BF0;
extern s32 D_001D5BF8[];
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

    if ((D_0013CAE4[0] & 0x20) != 0) {
        if (D_0015EEB0 == 1 || D_0015EEB0 == 0x10) {
            D_001D5BF8[0] = (s32)D_001D4EC0;
        } else {
            func_00226B08(-1);
            InitializeGlobalStateEntry(0);
            *(u16 *)D_0013E05A = 1;
        }
    } else if ((D_0013CAE4[0] & 0x40) != 0) {
        old_value = D_0013D408.unk1D;
        for (i = 0; i < 4; i++) {
            backup[i] = D_0014BF08[i];
        }
        memcard_restore_game(D_001D5BF0.unkE0);
        D_0013D408.unk1D = old_value;
        for (i = 0; i < 4; i++) {
            D_0014BF08[i] = backup[i];
        }
        D_0015EEA0 = 1;
        memcard_save_data(0, -1);
        clear_scene_state_buffers();
        result = scale_game_frames(0x10);
        fade_to_black(result);
        D_0013F350.unk20B1 = 1;
        return -1;
    }
    return 0;
}

extern __typeof__(process_global_state_flags) func_00222290 __attribute__((alias("FUN_00222290")));
