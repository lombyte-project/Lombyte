/* Level 06 moby class 0x37: hangar door. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002B5990), where it is exact; names translated to the US level program. */
#include "rnc/overlay/cuboid.h"

extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern int   D_L06_0015F5C4;
void         hangar_door_update(char *m) __asm__("FUN_L06_002b4770");

#include "sda.h"

void hangar_door_update(char *door_moby) {
    enum {
        HANGAR_DOOR_INITIALIZE = 0,
        HANGAR_DOOR_POSITIVE_ANGLE_OFFSET = 1,
        HANGAR_DOOR_NEGATIVE_ANGLE_OFFSET = 2
    };
    char        *door_data = ((char *)MOBY(door_moby)->pvars);
    float        z;
    float        position_angle_radians;
    float        pivot_offset_y;
    LevelCuboid *tb;
    if (D_L06_0015F5C4 == 6) {
        if (*(int *)(door_data + 0x10) == 0) {
            int x;
            *(int *)(door_data + 0x10) = 0;
            x = *(int *)(ship_substate);
            if (x == 8) {
                *(int *)(door_data + 0x10) = 1;
                *(float *)(door_data + 0x18) = 0.3490658402442932f;
                *(int *)(door_data + 0x14) = 0;
            } else if (x == 3) {
                *(int *)(door_data + 0x10) = 1;
                *(float *)(door_data + 0x14) = 0.3490658402442932f;
                *(int *)(door_data + 0x18) = 0;
            }
        }
    } else {
        *(int *)(door_data + 0x10) = 0;
    }
    if (*(float *)(door_data + 0x18) != *(float *)(door_data + 0x14)) {
        FUN_L00_0025be00((float *)(door_data + 0x18),
                         (float *)(door_data + 8),
                         *(float *)(door_data + 0x14),
                         frame_time_sq * 1.5707964f,
                         frame_time_sq * 1.5707964f,
                         frame_time * 1.5707964f);
    }
    switch (((unsigned char *)door_moby)[0x20]) {
    case HANGAR_DOOR_INITIALIZE:
        *(float *)(door_data + 0xC) =
            vector_distance_xy(door_moby + 0x10, &l06_cuboids[*(int *)door_data].pos);
        *(float *)(door_data + 4) = MOBY(door_moby)->rot.z;
        if (MOBY(door_moby)->rot.y != 0.0f) {
            door_moby[0x20] = 1;
        } else {
            door_moby[0x20] = 2;
        }
        break;
    case HANGAR_DOOR_POSITIVE_ANGLE_OFFSET:
        z = MOBY(door_moby)->pos.z;
        position_angle_radians =
            fast_add_rotations(*(float *)(door_data + 0x18), -1.5707950592041016f);
        MOBY(door_moby)->pos.x = fast_cos(position_angle_radians) * *(float *)(door_data + 0xC);
        pivot_offset_y = fast_sin(position_angle_radians) * *(float *)(door_data + 0xC);
        tb = l06_cuboids;
        MOBY(door_moby)->pos.y = pivot_offset_y;
        *(int *)(door_moby + 0x18) = 0;
        tb += *(int *)door_data;
        fast_vec_add(door_moby + 0x10, door_moby + 0x10, &tb->pos);
        MOBY(door_moby)->pos.z = z;
        MOBY(door_moby)->rot.z =
            fast_add_rotations(*(float *)(door_data + 4), *(float *)(door_data + 0x18));
        break;
    case HANGAR_DOOR_NEGATIVE_ANGLE_OFFSET:
        z = MOBY(door_moby)->pos.z;
        position_angle_radians =
            fast_subtract_rotations(-1.5707950592041016f, *(float *)(door_data + 0x18));
        MOBY(door_moby)->pos.x = fast_cos(position_angle_radians) * *(float *)(door_data + 0xC);
        pivot_offset_y = fast_sin(position_angle_radians) * *(float *)(door_data + 0xC);
        tb = l06_cuboids;
        MOBY(door_moby)->pos.y = pivot_offset_y;
        *(int *)(door_moby + 0x18) = 0;
        tb += *(int *)door_data;
        fast_vec_add(door_moby + 0x10, door_moby + 0x10, &tb->pos);
        MOBY(door_moby)->pos.z = z;
        MOBY(door_moby)->rot.z =
            fast_subtract_rotations(*(float *)(door_data + 4), *(float *)(door_data + 0x18));
        break;
    }
}
