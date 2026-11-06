/* Level 05 moby class that slides and lowers when triggered. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/world/cuboid.h"

extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern short D_L05_0015FFD8;

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

/* Slides the moby out along its pose-table entry's direction: 0 places it,
   1 waits for the trigger byte 0xBC of its D_L05_0015FFD8 record, 2 eases
   it toward the end point, 3 lowers it to 5.9 below the entry's height. */
void triggered_slide_update(char *moby) __asm__("FUN_L05_0030e508");

typedef struct TriggeredSlideVars {
    u8  pad_0[0x8];
    f32 lower_speed; /* z of the slide velocity (pvars 0x0), reused while lowering */
    u8  pad_C[0x4];
    s32 cuboid_10;
    s32 trigger_moby; /* moby index whose command byte triggers it */
    f32 slide_distance;
    s32 reversed; /* nonzero: offset and slide along -axes[1] */
} TriggeredSlideVars;

/* Moby class 843 */
void triggered_slide_update(char *moby) {
    enum {
        MOBY_PLACE_AT_POSE_OFFSET = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_SLIDE_TO_END_POINT = 2,
        MOBY_LOWER_BELOW_POSE_HEIGHT = 3
    };
    enum {
        MOBY_LOWERING_COMPLETE = 4
    };
    TriggeredSlideVars *slide_and_lowering_vars = (TriggeredSlideVars *)MOBY(moby)->pvars;
    float               slide_motion_scratch[4];
    float               slide_step_distance;
    float               speed;
    LevelCuboid        *pose_entry;
    float               remaining_slide_distance;
    float               reach;

    switch (MOBY(moby)->state) {
    case MOBY_PLACE_AT_POSE_OFFSET: {
        LevelCuboid *initial_pose_entry;

        initial_pose_entry = &l05_cuboids[slide_and_lowering_vars->cuboid_10];
        qcopy(moby + 0x10, &initial_pose_entry->pos);
        MOBY(moby)->rot.z = initial_pose_entry->rot.z;
        fast_vec_normalize(slide_motion_scratch,
                           &initial_pose_entry->axes[1],
                           slide_and_lowering_vars->slide_distance);
        if (slide_and_lowering_vars->reversed != 0) {
            fast_vec_scale(slide_motion_scratch, slide_motion_scratch, -1.0f);
            MOBY(moby)->flags |= 0x8000;
        }
        fast_vec_add(moby + 0x10, moby + 0x10, slide_motion_scratch);
        MOBY(moby)->state = MOBY_WAIT_FOR_TRIGGER;
        break;
    }
    case MOBY_WAIT_FOR_TRIGGER:
        if ((*(unsigned char **)&D_L05_0015FFD8)[slide_and_lowering_vars->trigger_moby * 256 +
                                                 0xBC] != 0) {
            allocate_voice_for_target_entry(0, 0, moby);
            MOBY(moby)->state = MOBY_SLIDE_TO_END_POINT;
            clear_u64_value(slide_and_lowering_vars);
        }
        break;
    case MOBY_SLIDE_TO_END_POINT:
        pose_entry = &l05_cuboids[slide_and_lowering_vars->cuboid_10];
        reach = slide_and_lowering_vars->slide_distance * 3.0f;
        slide_step_distance = 0.0f;
        fast_vec_normalize(slide_motion_scratch, &pose_entry->axes[1], reach);
        if (slide_and_lowering_vars->reversed != 0) {
            fast_vec_scale(slide_motion_scratch, slide_motion_scratch, -1.0f);
        }
        fast_vec_add(slide_motion_scratch, &pose_entry->pos, slide_motion_scratch);
        fast_vec_sub(slide_motion_scratch, slide_motion_scratch, moby + 0x10);
        remaining_slide_distance = fast_vec_length(slide_motion_scratch);
        speed = fast_vec_length(slide_and_lowering_vars);
        advance_accelerated_scalar(remaining_slide_distance,
                                   frame_time_sq * 20.0f,
                                   frame_time_sq * 30.0f,
                                   frame_time * 10.0f,
                                   &slide_step_distance,
                                   &speed);
        fast_vec_normalize(slide_and_lowering_vars, slide_motion_scratch, speed);
        fast_vec_add(moby + 0x10, moby + 0x10, slide_and_lowering_vars);
        if (fast_vec_length(slide_motion_scratch) < 0.01f) {
            MOBY(moby)->state = MOBY_LOWER_BELOW_POSE_HEIGHT;
        }
        break;
    case MOBY_LOWER_BELOW_POSE_HEIGHT:
        pose_entry = l05_cuboids;
        pose_entry += slide_and_lowering_vars->cuboid_10;
        advance_accelerated_scalar(pose_entry->pos.z - 5.9f,
                                   frame_time_sq * 20.0f,
                                   frame_time_sq * 40.0f,
                                   frame_time * 10.0f,
                                   (float *)&MOBY(moby)->pos.z,
                                   &slide_and_lowering_vars->lower_speed);
        if (MOBY(moby)->pos.z < pose_entry->pos.z - 5.9f) {
            MOBY(moby)->state = MOBY_LOWERING_COMPLETE;
        }
        break;
    }
}
