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
#include "rnc/overlay/shared/world/cuboid.h"

extern float
           approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern int D_L06_0015F5C4;
void       hangar_door_update(char *m) __asm__("FUN_L06_002b4770");

#include "sda.h"

typedef struct HangarDoorVars {
    u8  pad_0[0x4];
    f32 base_yaw;
    f32 open_angle_speed;
    f32 pivot_distance;
    s32 ship_swing_started;
    f32 target_open_angle;
    f32 open_angle;
} HangarDoorVars;

/* Moby class 55 */
void hangar_door_update(char *door_moby) {
    enum {
        HANGAR_DOOR_INITIALIZE = 0,
        HANGAR_DOOR_POSITIVE_ANGLE_OFFSET = 1,
        HANGAR_DOOR_NEGATIVE_ANGLE_OFFSET = 2
    };
    HangarDoorVars *door_data = (HangarDoorVars *)MOBY(door_moby)->pvars;
    float           z;
    float           position_angle_radians;
    float           pivot_offset_y;
    LevelCuboid    *tb;
    if (D_L06_0015F5C4 == 6) {
        if (door_data->ship_swing_started == 0) {
            int x;
            door_data->ship_swing_started = 0;
            x = *(int *)(ship_substate);
            if (x == 8) {
                door_data->ship_swing_started = 1;
                door_data->open_angle = 0.3490658402442932f;
                *(int *)&door_data->target_open_angle = 0;
            } else if (x == 3) {
                door_data->ship_swing_started = 1;
                door_data->target_open_angle = 0.3490658402442932f;
                *(int *)&door_data->open_angle = 0;
            }
        }
    } else {
        door_data->ship_swing_started = 0;
    }
    if (door_data->open_angle != door_data->target_open_angle) {
        approach_angle(&door_data->open_angle,
                       &door_data->open_angle_speed,
                       door_data->target_open_angle,
                       frame_time_sq * 1.5707964f,
                       frame_time_sq * 1.5707964f,
                       frame_time * 1.5707964f);
    }
    switch (((unsigned char *)door_moby)[0x20]) {
    case HANGAR_DOOR_INITIALIZE:
        door_data->pivot_distance =
            vector_distance_xy(door_moby + 0x10, &l06_cuboids[*(int *)door_data].pos);
        door_data->base_yaw = MOBY(door_moby)->rot.z;
        if (MOBY(door_moby)->rot.y != 0.0f) {
            door_moby[0x20] = 1;
        } else {
            door_moby[0x20] = 2;
        }
        break;
    case HANGAR_DOOR_POSITIVE_ANGLE_OFFSET:
        z = MOBY(door_moby)->pos.z;
        position_angle_radians = fast_add_rotations(door_data->open_angle, -1.5707950592041016f);
        MOBY(door_moby)->pos.x = fast_cos(position_angle_radians) * door_data->pivot_distance;
        pivot_offset_y = fast_sin(position_angle_radians) * door_data->pivot_distance;
        tb = l06_cuboids;
        MOBY(door_moby)->pos.y = pivot_offset_y;
        *(int *)(door_moby + 0x18) = 0;
        tb += *(int *)door_data;
        fast_vec_add(door_moby + 0x10, door_moby + 0x10, &tb->pos);
        MOBY(door_moby)->pos.z = z;
        MOBY(door_moby)->rot.z = fast_add_rotations(door_data->base_yaw, door_data->open_angle);
        break;
    case HANGAR_DOOR_NEGATIVE_ANGLE_OFFSET:
        z = MOBY(door_moby)->pos.z;
        position_angle_radians =
            fast_subtract_rotations(-1.5707950592041016f, door_data->open_angle);
        MOBY(door_moby)->pos.x = fast_cos(position_angle_radians) * door_data->pivot_distance;
        pivot_offset_y = fast_sin(position_angle_radians) * door_data->pivot_distance;
        tb = l06_cuboids;
        MOBY(door_moby)->pos.y = pivot_offset_y;
        *(int *)(door_moby + 0x18) = 0;
        tb += *(int *)door_data;
        fast_vec_add(door_moby + 0x10, door_moby + 0x10, &tb->pos);
        MOBY(door_moby)->pos.z = z;
        MOBY(door_moby)->rot.z =
            fast_subtract_rotations(door_data->base_yaw, door_data->open_angle);
        break;
    }
}
