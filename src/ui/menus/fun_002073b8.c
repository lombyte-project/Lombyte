#include "types.h"
#include "rnc/gameplay/hero.h"
extern s32 func_00208818();
s32 FUN_002073b8(s32 px, s32 py, f32 fparg0, f32 fparg1, f32 fparg2) {
    s32 var_4_15;

    var_4_15 = 0;
    if (((u32)(hero.state.control_mode - 0x11) < 2U) || (hero.base_condition == 1)) {
        var_4_15 = 1;
    }
    if (var_4_15 != 0) {
        return 0;
    }
    if (fparg2 < 71.5f) {
        return 0;
    }
    if (func_00208818(px, py, 0x131, 0xE2, 0xC6, 0x93) != 0) {
        return 0;
    }
    return func_00208818(px, py, 0x190, 0x89, 0xD1, 0xFB) == 0;
}
