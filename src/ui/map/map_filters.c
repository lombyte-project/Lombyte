/* Map reveal filters: the callbacks in the level-indexed table
   D_L00_00183F90[19][8] (map_filter_lNN_S is level NN, slot S). The map
   update calls the ones its filter flags select and reveals a pixel only
   when they return nonzero. */
#include "types.h"
#include "eetypes.h"
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/math/vector.h"

extern s32 point_is_clockwise(s32, s32, s32, s32, s32, s32) __asm__("func_00208818");

void map_filter_l01_0(s32 x, s32 y) __asm__("func_00206978");

void map_filter_l01_0(s32 x, s32 y) {
    point_is_clockwise(x, y, 0xd3, 0xdb, 0x129, 0xf9);
}

/* Ported from rac1-decomp (src/game/menu.c, func_002071D0). */
int map_filter_l01_1(void) __asm__("func_002069A0");

int map_filter_l01_1(void) {
    return menu_item_enabled_0 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002071E0). */
extern unsigned char menu_item_enabled_1 __asm__("D_0013D395") NOT_SDA;
int                  map_filter_l01_2(void) __asm__("func_002069B0");

int map_filter_l01_2(void) {
    return menu_item_enabled_1 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002071F0). */
int map_filter_l02_0(void) __asm__("func_002069C0");

int map_filter_l02_0(void) {
    return menu_item_enabled_9 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207200). */
/* Menu hit test: arg3 is the third float ($f14). b (state 16) is
   computed before a, as retail evaluates it; the second if's own
   `arg1 >= 0xC8` is retail's second test of $a1. */
int map_filter_l01_3(void *unused_input,
                     int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_002069D0");

int map_filter_l01_3(void *unused_input,
                     int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    struct Hero *hero_ptr = &hero;
    int          hero_state_blocks_high_range_reveal = hero_ptr->state.control_mode == 16;
    int          hero_state_blocks_low_range_reveal = hero_ptr->state.control_mode == 17 ||
                                             hero_ptr->state.control_mode == 18 ||
                                             hero_ptr->base_condition == 1;

    if (reveal_test_selector < 0xC8 && reveal_test_value >= 39.5f && reveal_test_value <= 42.5f &&
        !hero_state_blocks_low_range_reveal) {
        return 1;
    }
    if (reveal_test_selector >= 0xC8 && reveal_test_value >= 87.0f &&
        !hero_state_blocks_high_range_reveal) {
        return 1;
    }
    return 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002072C0). */
extern unsigned char D_0013D396 NOT_SDA;
/* `if (x) return 1;` in both arms with one shared `return 0;` keeps the
   flag test a beqz with the li in its slot; a result variable or a
   return 0 per arm becomes sltu. */
int map_filter_l01_4(int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_00206A90");

int map_filter_l01_4(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    int is16 = D_001413DC[0] == 0x10;

    if (reveal_test_selector < 0x100) {
        if (D_0013D396 != 0) {
            return 1;
        }
    } else if (reveal_test_value >= 58.0f && reveal_test_value <= 86.0f && !is16) {
        return 1;
    }
    return 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207340). */
/* The struct's address in a local keeps one base register for both
   field reads. */
int map_filter_l01_5(int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_00206B10");

int map_filter_l01_5(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector < 0x100) {
        struct Hero *hero_ptr = &hero;
        return hero_ptr->state.control_mode == 17 || hero_ptr->state.control_mode == 18 ||
               hero_ptr->base_condition == 1;
    }
    return (reveal_test_value >= 95.0f) ? 1 : 0;
}

extern u32 menu_selection_count __asm__("D_0015FD64") __attribute__((sda));

s32 map_filter_l03_0(void) __asm__("func_00206B78");

s32 map_filter_l03_0(void) {
    return menu_selection_count < 1;
}

/* Ported from rac1-decomp (src/game/menu.c, map_filter_l07_7). */
int map_filter_l04_0(void) __asm__("func_00206B88");

int map_filter_l04_0(void) {
    return menu_item_enabled_16 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002073C8). */
int map_filter_l04_1(void) __asm__("func_00206B98");

int map_filter_l04_1(void) {
    return menu_item_enabled_17 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002073D8). */
int map_filter_l04_2(void) __asm__("func_00206BA8");

int map_filter_l04_2(void) {
    return menu_item_enabled_18 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002073E8). */
int map_filter_l04_3(void) __asm__("func_00206BB8");

int map_filter_l04_3(void) {
    return menu_item_enabled_19 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002073F8). */
extern unsigned char menu_item_enabled_25 __asm__("D_0013D3AD") NOT_SDA;
int                  map_filter_l06_0(void) __asm__("func_00206BC8");

int map_filter_l06_0(void) {
    return menu_item_enabled_25 != 0;
}

/* One word: passes_projected_region_callback_0 reads it as a flag. The words around it
   (D_001A03AC, D_001A03B4 ...) are each a flag of another region callback. */
extern s32 region_enabled __asm__("D_001A03B0") NOT_SDA;

union RegionVector {
    u128 region_center;
    f32  f[4];
};

extern f32 vector_distance(void *a, void *b) __asm__("func_001F9B80");

s32 map_filter_l07_0(s32 projected_x, s32 projected_y, f32 x, f32 y, f32 z) __asm__("func_00206BD8");

s32 map_filter_l07_0(s32 projected_x, s32 projected_y, f32 x, f32 y, f32 z) {
    s32 special_state;

    if (point_is_clockwise(projected_x, projected_y, 0x189, 0x16F, 0x87, 0xED)) {
        if (point_is_clockwise(projected_x, projected_y, 0x75, 0x146, 0x18B, 0x1CC) &&
            point_is_clockwise(projected_x, projected_y, 0x141, 0x130, 0xB0, 0x130)) {
            if (43.9f <= z) {
                return 1;
            }
            return 0;
        }
    } else if (point_is_clockwise(projected_x, projected_y, 0x191, 0xCD, 0xD2, 0x13B)) {
        union RegionVector position;
        union RegionVector region_center;

        position.region_center = 0;
        region_center.region_center = 0;
        position.f[0] = x;
        position.f[1] = y;
        region_center.f[0] = 337.5f;
        region_center.f[1] = 250.0f;
        if (vector_distance(&position, &region_center) <= 7.0f) {
            return 1;
        }
        if (point_is_clockwise(projected_x, projected_y, 0x142, 0x12A, 0x173, 0xFC) &&
            region_enabled) {
            return 1;
        }
        return 0;
    } else {
        special_state = 0;
        if ((u32)(hero.state.control_mode - 0x11) < 2 || hero.base_condition == 1) {
            special_state = 1;
        }
        if (special_state &&
            point_is_clockwise(projected_x, projected_y, 0x12B, 0xB8, 0x13A, 0xF2) &&
            point_is_clockwise(projected_x, projected_y, 0x130, 0xED, 0x165, 0xCB) &&
            point_is_clockwise(projected_x, projected_y, 0x161, 0xDC, 0x13B, 0xB0) &&
            point_is_clockwise(projected_x, projected_y, 0x156, 0xA8, 0x119, 0xD1)) {
            return 1;
        }
    }
    return 0;
}

s32 region_enabled NOT_SDA = 0;

s32 map_filter_l07_1(s32 px, s32 py, f32 unused1, f32 unused2, f32 reveal_test_value) __asm__("func_00206E18");

s32 map_filter_l07_1(s32 px, s32 py, f32 unused1, f32 unused2, f32 reveal_test_value) {
    struct Hero *hero_ptr = &hero;
    s32          hero_state_allows_polygon_reveal = hero_ptr->state.control_mode == 17 ||
                                           hero_ptr->state.control_mode == 18 ||
                                           hero_ptr->base_condition == 1;

    if (point_is_clockwise(px, py, 0x93, 0x168, 0x182, 0x168) != 0 && py >= 0x135 &&
        D_001A03A8[0] != 0 && reveal_test_value >= 47.7f) {
        return 1;
    }
    if (!hero_state_allows_polygon_reveal) {
        return 0;
    }
    if (point_is_clockwise(px, py, 0xC5, 0x9A, 0x13C, 0xE1) != 0 &&
        point_is_clockwise(px, py, 0xD6, 0xC3, 0x157, 0xC5) != 0 &&
        point_is_clockwise(px, py, 0x107, 0xDA, 0x171, 0xA0) != 0) {
        return 1;
    }
    return 0;
}

s32 map_filter_l07_2(s32 px, s32 py) __asm__("func_00206F50");

s32 map_filter_l07_2(s32 px, s32 py) {
    if (py < 0xE9) {
        if (point_is_clockwise(px, py, 0x132, 0xA0, 0x15F, 0xD8) &&
            point_is_clockwise(px, py, 0x14D, 0xD8, 0x181, 0x9A) &&
            point_is_clockwise(px, py, 0x182, 0xB4, 0x137, 0x93) &&
            point_is_clockwise(px, py, 0x157, 0x8C, 0x130, 0xAA)) {
            return 1;
        }
        return 0;
    }
    {
        s32 a = point_is_clockwise(px, py, 0x8F, 0x115, 0x148, 0x14B);
        s32 b = point_is_clockwise(px, py, 0xE7, 0x108, 0x127, 0x164);
        if (a == 0 && b == 0) {
            return 0;
        }
    }
    {
        s32 a = point_is_clockwise(px, py, 0xED, 0x15F, 0x154, 0x10E);
        s32 b = point_is_clockwise(px, py, 0xA2, 0x12B, 0x16F, 0x147);
        if (a == 0 && b == 0) {
            return 0;
        }
    }
    {
        s32 a = point_is_clockwise(px, py, 0x132, 0x163, 0x141, 0xCC);
        s32 b = point_is_clockwise(px, py, 0xC2, 0x108, 0x1A0, 0x12E);
        if (!a && !b) {
            return 0;
        }
        return 1;
    }
}

extern s32 D_001A03BC NOT_SDA;
s32 map_filter_l07_3(s32 px, s32 py, f32 unused1, f32 unused2, f32 reveal_test_value) __asm__("func_00207100");

s32 map_filter_l07_3(s32 px, s32 py, f32 unused1, f32 unused2, f32 reveal_test_value) {
    if (py < 0xBB) {
        struct Hero *hero_ptr = &hero;
        s32          hero_state_blocks_low_y_reveal = hero_ptr->state.control_mode == 17 ||
                                             hero_ptr->state.control_mode == 18 ||
                                             hero_ptr->base_condition == 1;

        if (!hero_state_blocks_low_y_reveal && D_001A03BC != 0) {
            return 1;
        }
        return 0;
    }
    if (reveal_test_value >= 51.5f && reveal_test_value <= 54.0f &&
        point_is_clockwise(px, py, 0x10A, 0xE5, 0x124, 0xF9) != 0) {
        return 1;
    }
    return 0;
}

extern s32 D_001A03B4 NOT_SDA;

s32 map_filter_l07_4(s32 point_x, s32 point_y) __asm__("func_002071C0");

s32 map_filter_l07_4(s32 point_x, s32 point_y) {
    s32 point_clockwise_of_first_edge;
    s32 point_clockwise_of_second_edge;

    point_clockwise_of_first_edge = point_is_clockwise(point_x, point_y, 0x99, 0xED, 0x160, 0x117);
    point_clockwise_of_second_edge =
        point_is_clockwise(point_x, point_y, 0x10E, 0xF7, 0x13D, 0x119);
    if (D_001A03B4 == 0) {
        return 0;
    }
    if (point_clockwise_of_first_edge == 0) {
        if (point_clockwise_of_second_edge != 0) {
            if (point_clockwise_of_first_edge) {
                return 1;
            } else {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207A80). */
extern int D_001A03C0 NOT_SDA;
extern int D_001A03B8 NOT_SDA;
/* Hit test in two layouts: arg3 (the third float, $f14) must lie in
   [73.5, 80] with D_001A03C0 set, or for arg1 >= 0x9D in [51.5, 54] with
   D_001A03B8 set. Written as `<`/`>` rejections so they compile to retail's
   c.lt/bc1t; the second arm's `return 0` cross-jumps into the first's. */
int map_filter_l07_5(void *unused_input,
                     int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_00207250");

int map_filter_l07_5(void *unused_input,
                     int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector < 0x9D) {
        if (reveal_test_value < 73.5f || reveal_test_value > 80.0f || D_001A03C0 == 0) {
            return 0;
        }
        return 1;
    } else {
        if (reveal_test_value < 51.5f || reveal_test_value > 54.0f || D_001A03B8 == 0) {
            return 0;
        }
        return 1;
    }
}

extern s32 D_001A03AC[];
s32 map_filter_l07_6(s32 px, s32 py, f32 fparg0, f32 fparg1, f32 fparg2) __asm__("func_00207300");

s32 map_filter_l07_6(

    s32 px, s32 py, f32 unused_input, f32 unused_parameter, f32 reveal_test_value) {
    s32 hit;

    hit = 0;
    if ((u32)(hero.state.control_mode - 0x11) < 2U) {
        goto block_2;
    }
    if (hero.base_condition != 1) {
        goto block_3;
    }
block_2:
    hit = 1;
block_3:
    if (py < 0x105) {
        goto block_7;
    }
    if (D_001A03AC[0] == 0) {
        return 0;
    }
    if (reveal_test_value >= 47.7f) {
        return 1;
    }
    return 0;
block_7:
    if (py < 0xC1) {
        return hit;
    }
    if (point_is_clockwise(px, py, 0xD9, 0xB8, 0x156, 0xD2) != 0) {
        hit = 0;
    }
    return hit;
}

s32 map_filter_l07_7(s32 px, s32 py, f32 fparg0, f32 fparg1, f32 fparg2) __asm__("func_002073B8");

s32 map_filter_l07_7(

    s32 px, s32 py, f32 unused_input, f32 unused_parameter, f32 reveal_test_value) {
    s32 enabled;

    enabled = 0;
    if (((u32)(hero.state.control_mode - 0x11) < 2U) || (hero.base_condition == 1)) {
        enabled = 1;
    }
    if (enabled != 0) {
        return 0;
    }
    if (reveal_test_value < 71.5f) {
        return 0;
    }
    if (point_is_clockwise(px, py, 0x131, 0xE2, 0xC6, 0x93) != 0) {
        return 0;
    }
    return point_is_clockwise(px, py, 0x190, 0x89, 0xD1, 0xFB) == 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207CB0). */
int map_filter_l08_0(int unused_input, int reveal_test_selector) __asm__("func_00207480");

int map_filter_l08_0(int unused_input, int reveal_test_selector) {
    if (reveal_test_selector >= 0x101) {
        return D_001413DC[0] == 0xF;
    }
    return menu_item_enabled_41 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207CE0). */
/* The two unused float parameters put the threshold's argument in
   $f14 (floats count consecutively from $f12). The nops after the mtc1
   and the compare are ps2eeas's (tools/ps2eeas_nops.py). */
int map_filter_l08_1(int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_002074B0");

int map_filter_l08_1(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector < 0xE0) {
        return (reveal_test_value >= 47.75f) ? 1 : 0;
    }
    return (reveal_test_value < 29.0f) ? 1 : 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207D38). */
/* `(a && b) ? 1 : 0` gives retail's bc1f then bc1tl; the other arm is
   a plain `? 1 : 0`. */
int map_filter_l08_2(int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_00207508");

int map_filter_l08_2(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector < 0xE0) {
        return (reveal_test_value >= 44.0f && reveal_test_value <= 45.0f) ? 1 : 0;
    }
    return (reveal_test_value >= 37.0f) ? 1 : 0;
}

/* map_filter_l08_2's twin with other thresholds. The function is 116 bytes:
   its last three words (`move v0,0; jr ra; nop`) were split off as a
   separate FUN_002075e8, which no caller uses. */
int map_filter_l08_3(int frame, float unused0, float unused1, float value) __asm__("func_00207580");

int map_filter_l08_3(int frame, float unused0, float unused1, float reveal_test_value) {
    if (frame < 0xE0) {
        return (reveal_test_value >= 42.0f && reveal_test_value <= 43.0f) ? 1 : 0;
    }
    return (reveal_test_value >= 39.0f) ? 1 : 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207E28). */
/* Returning the compare as `? 1 : 0` gives bc1t with the `li 1` in its
   slot; folded into `&&` it becomes bc1tl. The two nops, after the mtc1
   and after the compare, are ps2eeas's (tools/ps2eeas_nops.py). */
int map_filter_l08_4(int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_002075F8");

int map_filter_l08_4(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector >= 0xE0) {
        return 0;
    }
    return (reveal_test_value <= 38.0f) ? 1 : 0;
}

extern unsigned char menu_item_enabled_36 __asm__("D_0013D3B8") NOT_SDA;

int map_filter_l08_5(void) __asm__("func_00207630");

int map_filter_l08_5(void) {
    return menu_item_enabled_36 != 0;
}

extern unsigned char menu_item_enabled_37 __asm__("D_0013D3B9") NOT_SDA;

int map_filter_l08_6(void) __asm__("func_00207640");

int map_filter_l08_6(void) {
    return menu_item_enabled_37 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207F00). */
extern unsigned char menu_item_enabled_38 __asm__("D_0013D3BA") NOT_SDA;
int                  map_filter_l08_7(void) __asm__("func_00207650");

int map_filter_l08_7(void) {
    return menu_item_enabled_38 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207F00). */
extern unsigned char menu_item_enabled_55 __asm__("D_0013D3CB") NOT_SDA;
int                  map_filter_l09_0(void) __asm__("func_00207660");

int map_filter_l09_0(void) {
    return menu_item_enabled_55 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207F20). */
extern unsigned char menu_item_enabled_56 __asm__("D_0013D3CC") NOT_SDA;
int                  map_filter_l09_1(void) __asm__("func_00207670");

int map_filter_l09_1(void) {
    return menu_item_enabled_56 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207F30). */
extern unsigned char menu_item_enabled_57 __asm__("D_0013D3CD") NOT_SDA;
int                  map_filter_l09_2(void) __asm__("func_00207680");

int map_filter_l09_2(void) {
    return menu_item_enabled_57 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207EC0). */
/* The recorded C was right; the two ps2eeas nops were the only
   residual. */
int map_filter_l10_0(int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_00207690");

int map_filter_l10_0(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector >= 0xBE)
        return menu_item_enabled_68[0] != 0;
    return (reveal_test_value >= 58.5f) ? 1 : 0;
}

extern unsigned char menu_item_enabled_64 __asm__("D_0013D3D4") NOT_SDA;

int map_filter_l10_1(void) __asm__("func_002076D0");

int map_filter_l10_1(void) {
    return menu_item_enabled_64 != 0;
}

extern unsigned char menu_item_enabled_65 __asm__("D_0013D3D5") NOT_SDA;

int map_filter_l10_2(void) __asm__("func_002076E0");

int map_filter_l10_2(void) {
    return menu_item_enabled_65 != 0;
}

extern unsigned char menu_item_enabled_66 __asm__("D_0013D3D6") NOT_SDA;

int map_filter_l10_3(void) __asm__("func_002076F0");

int map_filter_l10_3(void) {
    return menu_item_enabled_66 != 0;
}

extern unsigned char menu_item_enabled_67 __asm__("D_0013D3D7") NOT_SDA;

int map_filter_l10_4(void) __asm__("func_00207700");

int map_filter_l10_4(void) {
    return menu_item_enabled_67 != 0;
}

extern unsigned char menu_item_enabled_69 __asm__("D_0013D3D9") NOT_SDA;

int map_filter_l10_5(void) __asm__("func_00207710");

int map_filter_l10_5(void) {
    return menu_item_enabled_69 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207F50). */
extern unsigned char D_0013D3DE NOT_SDA;
int                             map_filter_l11_0(int   reveal_test_selector,
                                                 float unused1,
                                                 float unused2,
                                                 float reveal_test_value) __asm__("func_00207720");

int map_filter_l11_0(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector < 0xDD) {
        return reveal_test_value >= 232.0f && reveal_test_value <= 235.0f && D_0013D3DE != 0;
    }
    return (reveal_test_value <= 180.0f) ? 1 : 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00207FD0). */
extern unsigned char D_0013D3DF NOT_SDA;
int                             map_filter_l11_1(int   reveal_test_selector,
                                                 float unused1,
                                                 float unused2,
                                                 float reveal_test_value) __asm__("func_002077A0");

int map_filter_l11_1(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector < 0xDD) {
        return (reveal_test_value >= 242.0f) ? 1 : 0;
    }
    return reveal_test_value <= 180.0f && D_0013D3DF != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, downsample_map_coverage). */
extern unsigned char D_0013D3E0 NOT_SDA;
int                             map_filter_l11_2(int   reveal_test_selector,
                                                 float unused1,
                                                 float unused2,
                                                 float reveal_test_value) __asm__("func_00207800");

int map_filter_l11_2(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    if (reveal_test_selector < 0xDD) {
        return (reveal_test_value >= 238.0f && reveal_test_value <= 241.0f) ? 1 : 0;
    }
    return reveal_test_value <= 180.0f && D_0013D3E0 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002080B0). */
/* The struct Hero state test of func_00207340, computed up front (retail
   evaluates it before the arg0 branch), gates the arg0 < 0x15F arm; the
   other arm is func_00208160's second test with D_0013D3E2. */
int map_filter_l11_3(int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_00207880");

int map_filter_l11_3(int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    struct Hero *hero_ptr = &hero;
    int          hero_state_allows_reveal = hero_ptr->state.control_mode == 17 ||
                                   hero_ptr->state.control_mode == 18 ||
                                   hero_ptr->base_condition == 1;

    if (reveal_test_selector < 0x15F) {
        return (reveal_test_value >= 200.0f) ? hero_state_allows_reveal : 0;
    }
    return reveal_test_value >= 233.0f && reveal_test_value <= 235.0f && D_0013D3E2[0] != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00208160). */
extern unsigned char D_0013D3E3 NOT_SDA;
int map_filter_l11_4(int x, float unused1, float unused2, float y) __asm__("func_00207930");

int map_filter_l11_4(int x, float unused1, float unused2, float y) {
    if (x < 0x15F) {
        return (y >= 228.0f && y <= 230.0f) ? 1 : 0;
    }
    return y >= 233.0f && y <= 235.0f && D_0013D3E3 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002081F8). */
extern unsigned char menu_item_enabled_77 __asm__("D_0013D3E1") NOT_SDA;
int                  map_filter_l11_5(void) __asm__("func_002079C8");

int map_filter_l11_5(void) {
    return menu_item_enabled_77 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00208208). */
/* arg0 is unused; the two unused float parameters put arg3 in $f14
   (floats count consecutively from $f12, one register each; see
   func_00207CE0). With `r` defaulting to 1, reorg turns the reset into
   retail's bc1fl with `r = 0` in its delay slot. The nop after the mtc1
   is ps2eeas's (tools/ps2eeas_nops.py). */
int map_filter_l12_0(void *unused_input,
                     int   reveal_test_selector,
                     float unused1,
                     float unused2,
                     float reveal_test_value) __asm__("func_002079D8");

int map_filter_l12_0(void *unused_input,
                     int   reveal_test_selector,
                     float unused1,
                     float unused2,

                     float reveal_test_value) {
    int reveal_allowed = 1;

    if (reveal_test_selector < 0x141 && !(63.5f <= reveal_test_value)) {
        reveal_allowed = 0;
    }
    return reveal_allowed;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00208238). */
int map_filter_l12_1(void) __asm__("func_00207A08");

int map_filter_l12_1(void) {
    return 1;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00208240). */
int map_filter_l12_2(void) __asm__("func_00207A10");

int map_filter_l12_2(void) {
    return 1;
}

s32 map_filter_l15_0(s32 x, s32 y) __asm__("func_00207A18");

s32 map_filter_l15_0(s32 x, s32 y) {
    Vec4 point_position;
    Vec4 region_center;
    s32  near;

    if (D_001A03A8[0] == 0) {
        point_position.q = 0;
        region_center.q = 0;
        point_position.f[0] = x;
        point_position.f[1] = y;
        region_center.f[0] = 161.5f;
        region_center.f[1] = 249.0f;
        near = vector_distance(&point_position, &region_center) <= 35.0f;
    } else {
        near = 1;
    }
    return near;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002082E8). */
extern unsigned char menu_item_enabled_102 __asm__("D_0013D3FA") NOT_SDA;
int                  map_filter_l16_0(void) __asm__("func_00207AB8");

int map_filter_l16_0(void) {
    return menu_item_enabled_102 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_002082F8). */
extern unsigned char menu_item_enabled_103 __asm__("D_0013D3FB") NOT_SDA;
int                  map_filter_l16_1(void) __asm__("func_00207AC8");

int map_filter_l16_1(void) {
    return menu_item_enabled_103 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00208308). */
extern unsigned char menu_item_enabled_104 __asm__("D_0013D3FC") NOT_SDA;
int                  map_filter_l16_2(void) __asm__("func_00207AD8");

int map_filter_l16_2(void) {
    return menu_item_enabled_104 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00208318). */
extern unsigned char menu_item_enabled_105 __asm__("D_0013D3FD") NOT_SDA;
int                  map_filter_l16_3(void) __asm__("func_00207AE8");

int map_filter_l16_3(void) {
    return menu_item_enabled_105 != 0;
}

/* Ported from rac1-decomp (src/game/menu.c, func_00208328). */
extern unsigned char menu_item_enabled_115 __asm__("D_0013D407") NOT_SDA;
int                  map_filter_l18_0(void) __asm__("func_00207AF8");

int map_filter_l18_0(void) {
    return menu_item_enabled_115 != 0;
}

/* Four more one-word flags of the region callbacks above, after
   region_enabled in retail order. Scalars: the compiler 8-aligns arrays,
   and retail has them 4 bytes apart. */
s32 D_001A03B4 NOT_SDA = 0;
int D_001A03B8 NOT_SDA = 0;
s32 D_001A03BC NOT_SDA = 0;
int D_001A03C0 NOT_SDA = 0;
