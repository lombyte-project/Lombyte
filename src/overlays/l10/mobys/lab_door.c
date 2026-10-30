/* Level 10 moby class lab_door. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/quad.h"

extern int     debug_print() __asm__("FUN_001e93b0");
extern float   scale_time(float) __asm__("FUN_001f96b0");
extern OvlQuad D_L10_00167240;
/* Test whether a point lies within a box around a moby, in the moby's local frame. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DA3C0), where it is exact; names translated to the US level program. */

#include "rnc/overlay/l10/mobys/lab_door_globals.h"

extern float lab_door_local_y_half_extent_gp __asm__("D_L10_00161C0C") __attribute__((sda));
extern float lab_door_local_x_half_extent_gp __asm__("D_L10_00161C10") __attribute__((sda));
int          test_point_in_lab_door_local_xy_bounds(struct Moby *moby,
                                                    float       *world_point) __asm__("FUN_L10_002d9000");
extern int   test_point_in_lab_door_local_xy_bounds() __asm__("FUN_L10_002d9000");

int test_point_in_lab_door_local_xy_bounds(struct Moby *moby, float *world_point) {
    float tested_world_point[4] __attribute__((aligned(16)));
    float door_world_to_local_rotation[16] __attribute__((aligned(16)));
    float door_local_point_offset[4] __attribute__((aligned(16)));
    float point_offset_from_door[4] __attribute__((aligned(16)));
    *(OvlQuad *)tested_world_point = *(OvlQuad *)world_point;
    transpose_rotation_matrix(door_world_to_local_rotation, &moby->rot_row0);
    fast_vec_sub(point_offset_from_door, tested_world_point, &moby->pos);
    *(OvlQuad *)door_local_point_offset = *(OvlQuad *)point_offset_from_door;
    transform_vector(
        door_local_point_offset, door_local_point_offset, door_world_to_local_rotation);
    if (absolute_float(door_local_point_offset[0]) < lab_door_local_x_half_extent_gp) {
        return absolute_float(door_local_point_offset[1]) < lab_door_local_y_half_extent_gp;
    }
    return 0;
}
/* update for a moby that glides between two heights and sets a flag */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DA468), where it is exact; names translated to the US level program. */

#include "rnc/game/item_state.h"

char D_L10_001DD160[] =
    "Moby Class %d Instance %d has no pVars!  May be in the art file.\n\tMobyDeleted\n";
extern int get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");

void       lab_door_update(unsigned char *m) __asm__("FUN_L10_002d90a8");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");

void lab_door_update(unsigned char *door_moby) {
    enum {
        LAB_DOOR_STATE_INITIALIZE = 0,
        LAB_DOOR_STATE_DESCENDING = 1,
        LAB_DOOR_STATE_AT_ORIGINAL_HEIGHT = 2,
        LAB_DOOR_STATE_RISING = 3,
        LAB_DOOR_STATE_AT_RAISED_HEIGHT = 4
    };
    int    *door_vars = ((int *)MOBY(door_moby)->pvars);
    OvlQuad point_test_positions[2];
    int     it;
    int     flag;
    int     hit;
    if (door_vars == 0) {
        debug_print(D_L10_001DD160, MOBY(door_moby)->oclass, ((short)MOBY(door_moby)->save_id));
        mark_moby_for_removal(door_moby);
        return;
    }
    switch (door_moby[0x20]) {
    case LAB_DOOR_STATE_INITIALIZE:
        *(OvlQuad *)door_vars = *(OvlQuad *)&MOBY(door_moby)->pos;
        door_moby[0x20] = 2;
        break;
    case LAB_DOOR_STATE_DESCENDING:
        flag = 0;
        {
            float f = MOBY(door_moby)->pos.z - 4.0f / scale_time(20.0f);
            MOBY(door_moby)->pos.z = f;
            if (f <= *(float *)(door_vars + 2)) {
                MOBY(door_moby)->pos.z = *(float *)(door_vars + 2);
                door_moby[0x20] = 2;
            }
        }
        if (door_vars[4] != -1) {
            if (get_first_moby_in_group(&it, door_vars[4], 0, 0)) {
                do {
                    point_test_positions[0] = *(OvlQuad *)(it + 0x10);
                    if (test_point_in_lab_door_local_xy_bounds(door_moby,
                                                               &point_test_positions[0])) {
                        flag = 1;
                        break;
                    }
                } while (get_next_moby_in_group(&it, it, 0, 0));
            }
        }
        hit = 0;
        if (flag == 0) {
            point_test_positions[0] = D_L10_00167240;
            if (test_point_in_lab_door_local_xy_bounds(door_moby, &point_test_positions[0]) == 0) {
                point_test_positions[1] = *(OvlQuad *)&hero.motion.pos;
                if (test_point_in_lab_door_local_xy_bounds(door_moby, &point_test_positions[1]) ==
                    0)
                    goto done;
            }
        }
        hit = 1;
    done:
        if (hit)
            door_moby[0x20] = 3;
        break;
    case LAB_DOOR_STATE_AT_ORIGINAL_HEIGHT:
        break;
    case LAB_DOOR_STATE_RISING: {
        float f = 4.0f / scale_time(20.0f);
        float unclamped_rising_height = MOBY(door_moby)->pos.z + f;
        float raised_height;
        MOBY(door_moby)->pos.z = unclamped_rising_height;
        raised_height = *(float *)(door_vars + 2) + 4.0f;
        if (raised_height <= unclamped_rising_height) {
            MOBY(door_moby)->pos.z = raised_height;
            door_moby[0x20] = 4;
            if (door_vars[5] != -1)
                alternate_item_available[door_vars[5] + 0x4C] = 1;
        }
        break;
    }
    case LAB_DOOR_STATE_AT_RAISED_HEIGHT:
        break;
    }
}
