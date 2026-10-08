#include "types.h"
#include "rnc/gameplay/hero.h"

extern s32 D_001A03A8[];
extern s32 func_00208818();

s32 FUN_00206e18(s32 px, s32 py, f32 unused1, f32 unused2, f32 arg3) {
    struct Hero *s = &hero;
    s32 a = s->state.control_mode == 17 || s->state.control_mode == 18 || s->base_condition == 1;

    if (func_00208818(px, py, 0x93, 0x168, 0x182, 0x168) != 0 && py >= 0x135 &&
        D_001A03A8[0] != 0 && arg3 >= 47.7f) {
        return 1;
    }
    if (!a) {
        return 0;
    }
    if (func_00208818(px, py, 0xC5, 0x9A, 0x13C, 0xE1) != 0 &&
        func_00208818(px, py, 0xD6, 0xC3, 0x157, 0xC5) != 0 &&
        func_00208818(px, py, 0x107, 0xDA, 0x171, 0xA0) != 0) {
        return 1;
    }
    return 0;
}
