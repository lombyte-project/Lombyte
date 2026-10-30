/* Shared moby class 0x3FD: rotates a group of mobys around it. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/props/group_rotator_data/update_moby_rotation_group_2.h"
#include "rnc/overlay/shared/props/group_rotator_data/update_moby_rotation_group.h"

typedef struct Moby Moby;

typedef struct {
    float pos[4];   /* 0x00 */
    float unk10;    /* 0x10 */
    float unk14;    /* 0x14 */
    int   unk18;    /* 0x18 */
    int   idx;      /* 0x1C */
    float unk20;    /* 0x20 */
    Moby *slots[7]; /* 0x24 */
} Vars;

extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern float fast_add_rotations(float, float);
extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");

extern float            D_0015ED6C;
extern float D_0015ED70 MACRO_ADDR;
extern char             D_0013F3D0[];
extern f32              D_0015ED6C;
extern Moby            *D_L06_0015FFD8;
extern float            D_0015ED70;
extern float            rotation_group_position_offset_distance_gp __asm__("D_L06_00161D48")
    __attribute__((sda));

float D_L17_00161DE0 = 3.109f;
float D_L18_001620FC = 3.109f;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F6330), where it is exact; names translated to the US level program. */
void update_moby_rotation_group(Moby *moby) __asm__("FUN_L06_002f4f00");

void update_moby_rotation_group(Moby *moby) {
    enum {
        SPAWN_CHILD_MOBYS = 0,
        INITIALIZE_BASE_TRANSFORM = 1,
        WAIT_FOR_TRIGGER_AND_CHILDREN = 2,
        UPDATE_POSITION_FROM_BASE = 3,
        ROTATE_AND_HIDE_AFTER_TRIGGER_AND_CHILDREN = 4,
        ROTATE_AND_HIDE = 5,
        RESTORE_ROTATION_THEN_WAIT_FOR_TRIGGER_AND_CHILDREN = 8,
        RESTORE_ROTATION_THEN_UPDATE_POSITION_FROM_BASE = 9
    };
    Vars *moby_vars = (Vars *)(*(void **)&moby->pvars);
    float position_offset[4];
    int   i;
    int   j;
    int   k;
    int   flag;

    switch (moby->state) {
    case SPAWN_CHILD_MOBYS:
        moby_vars->unk10 = (*(float(*)[4]) & moby->rot)[0];
        qcopy(moby_vars->pos, (*(float(*)[4]) & moby->pos));
        moby->state = 2;
        for (i = 0; i < 7; i++) {
            moby_vars->slots[i] = create_moby(0x3FD);
            (*(char *)&moby_vars->slots[i]->update_dist) = 0x40;
            moby_vars->slots[i]->draw_dist = 0x40;
            (*(char *)&moby_vars->slots[i]->visible) = 1;
            (*(long *)&moby_vars->slots[i]->spawn_frame) = (*(long *)&moby->spawn_frame);
            moby_vars->slots[i]->flags = moby->flags;
            qcopy_nc((*(float(*)[4]) & moby_vars->slots[i]->pos), (*(float(*)[4]) & moby->pos));
            qcopy_nc((*(float(*)[4]) & moby_vars->slots[i]->rot), (*(float(*)[4]) & moby->rot));
            (*(float(*)[4]) & moby_vars->slots[i]->rot)[0] =
                fast_add_rotations((*(float(*)[4]) & moby_vars->slots[i]->rot)[0],
                                   (float)((i + 1) * 45) * 0.017453292f);
            moby_vars->slots[i]->state = 1;
            euler_rotation_basis((*(float(*)[4]) & moby_vars->slots[i]->rot_row0),
                                 (*(float(*)[4]) & moby_vars->slots[i]->rot));
        }
        break;
    case INITIALIZE_BASE_TRANSFORM:
        moby_vars->unk10 = (*(float(*)[4]) & moby->rot)[0];
        qcopy(moby_vars->pos, (*(float(*)[4]) & moby->pos));
        moby->state = 3;
        fast_vec_normalize(position_offset,
                           (*(float(*)[4]) & moby->rot_row2),
                           rotation_group_position_offset_distance_gp);
        fast_vec_add((*(float(*)[4]) & moby->pos), position_offset, moby_vars);
        break;
    case WAIT_FOR_TRIGGER_AND_CHILDREN:
        if (moby->command == 0) {
            flag = 0;
            if (moby_vars->idx >= 0) {
                if (D_L06_0015FFD8[moby_vars->idx].oclass == 0x5A8) {
                    if (vector_distance_xy(D_0013F3D0,
                                           (*(float(*)[4]) & D_L06_0015FFD8[moby_vars->idx].pos)) <
                        20.0f) {
                        flag = 1;
                    }
                } else if (D_L06_0015FFD8[moby_vars->idx].oclass == 0x24A) {
                    flag = D_L06_0015FFD8[moby_vars->idx].command != 0;
                } else {
                    flag = D_L06_0015FFD8[moby_vars->idx].state >= 3;
                }
            } else {
                if (vector_distance_xy((*(float(*)[4]) & moby->pos), D_0013F3D0) < 6.0f &&
                    rotation_group_fallback_proximity_trigger_inhibit_gp == 0.0f) {
                    flag = 1;
                }
            }
            if (flag) {
                for (j = 0; j < 7; j++) {
                    if (moby_vars->slots[j]->state != 3) {
                        flag = 0;
                    }
                }
                if (flag) {
                    allocate_voice_for_target_entry(0, 0, moby);
                    moby->state = 4;
                    moby_vars->unk18 = 0;
                    moby_vars->unk14 = 0.0f;
                    for (k = 0; k < 7; k++) {
                        moby_vars->slots[k]->state = 5;
                    }
                }
            }
        }
    case UPDATE_POSITION_FROM_BASE:
        fast_vec_normalize(position_offset,
                           (*(float(*)[4]) & moby->rot_row2),
                           rotation_group_position_offset_distance_gp);
        fast_vec_add((*(float(*)[4]) & moby->pos), position_offset, moby_vars);
        break;
    case ROTATE_AND_HIDE_AFTER_TRIGGER_AND_CHILDREN:
    case ROTATE_AND_HIDE:
        approach_angle(&moby_vars->unk14,
                       &moby_vars->unk20,
                       1.0471976f,
                       rotation_group_angular_velocity_adjustment_degrees_gp * 0.017453292f *
                           D_0015ED70,
                       rotation_group_angular_deceleration_degrees_gp * 0.017453292f * D_0015ED70,
                       rotation_group_max_angular_speed_degrees_gp * 0.017453292f * D_0015ED6C);
        (*(float(*)[4]) & moby->rot)[0] =
            fast_subtract_rotations(moby_vars->unk10, moby_vars->unk14);
        if (moby_vars->unk14 >= 1.0471976f) {
            moby->flags |= 1;
            (*(char *)&moby->visible) = 0;
            if (moby->state == 4) {
                moby->state = 6;
            } else {
                moby->state = 7;
            }
        }
        break;
    case RESTORE_ROTATION_THEN_WAIT_FOR_TRIGGER_AND_CHILDREN:
    case RESTORE_ROTATION_THEN_UPDATE_POSITION_FROM_BASE:
        approach_angle(&moby_vars->unk14,
                       &moby_vars->unk20,
                       0.0f,
                       rotation_group_angular_velocity_adjustment_degrees_gp * 0.017453292f *
                           D_0015ED70,
                       rotation_group_angular_deceleration_degrees_gp * 0.017453292f * D_0015ED70,
                       rotation_group_max_angular_speed_degrees_gp * 0.017453292f * D_0015ED6C);
        (*(float(*)[4]) & moby->rot)[0] =
            fast_subtract_rotations(moby_vars->unk10, moby_vars->unk14);
        if (moby_vars->unk14 <= 0.0f) {
            if (moby->state == 8) {
                moby->state = 2;
            } else {
                moby->state = 3;
            }
        }
        break;
    }
}
