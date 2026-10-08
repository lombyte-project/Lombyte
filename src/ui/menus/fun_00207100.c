#include "types.h"
#include "rnc/gameplay/hero.h"
extern u32 D_001A03BC[];
extern s32 func_00208818();
s32 FUN_00207100(s32 px, s32 py, f32 unused1, f32 unused2, f32 arg3) {
    if (py < 0xBB) {
        struct Hero *s = &hero;
        s32 a = s->state.control_mode == 17 || s->state.control_mode == 18 || s->base_condition == 1;

        if (!a && D_001A03BC[0] != 0) {
            return 1;
        }
        return 0;
    }
    if (arg3 >= 51.5f && arg3 <= 54.0f &&
        func_00208818(px, py, 0x10A, 0xE5, 0x124, 0xF9) != 0) {
        return 1;
    }
    return 0;
}
