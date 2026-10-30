/* Level 05 moby class that slides and lowers when triggered. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/cuboid.h"

extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern short D_L05_0015FFD8;

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

/* Slides the moby out along its pose-table entry's direction: 0 places it,
   1 waits for the trigger byte 0xBC of its D_L05_0015FFD8 record, 2 eases
   it toward the end point, 3 lowers it to 5.9 below the entry's height. */
void update_triggered_moby_slide_and_lowering(char *moby) __asm__("FUN_L05_0030e508");

void update_triggered_moby_slide_and_lowering(char *moby) {
    enum {
        MOBY_PLACE_AT_POSE_OFFSET = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_SLIDE_TO_END_POINT = 2,
        MOBY_LOWER_BELOW_POSE_HEIGHT = 3
    };
    char        *data = ((char *)MOBY(moby)->pvars);
    float        slide_motion_scratch[4];
    float        slide_step_distance;
    float        speed;
    LevelCuboid *pose_entry;
    float        remaining_slide_distance;
    float        reach;

    switch (MOBY(moby)->state) {
    case MOBY_PLACE_AT_POSE_OFFSET: {
        LevelCuboid *initial_pose_entry;

        initial_pose_entry = &l05_cuboids[*(int *)(data + 0x10)];
        qcopy(moby + 0x10, &initial_pose_entry->pos);
        MOBY(moby)->rot.z = initial_pose_entry->rot.z;
        fast_vec_normalize(
            slide_motion_scratch, &initial_pose_entry->axes[1], *(float *)(data + 0x18));
        if (*(int *)(data + 0x1C) != 0) {
            fast_vec_scale(slide_motion_scratch, slide_motion_scratch, -1.0f);
            MOBY(moby)->flags |= 0x8000;
        }
        fast_vec_add(moby + 0x10, moby + 0x10, slide_motion_scratch);
        MOBY(moby)->state = 1;
        break;
    }
    case MOBY_WAIT_FOR_TRIGGER:
        if ((*(unsigned char **)&D_L05_0015FFD8)[*(int *)(data + 0x14) * 256 + 0xBC] != 0) {
            allocate_voice_for_target_entry(0, 0, moby);
            MOBY(moby)->state = 2;
            clear_u64_value(data);
        }
        break;
    case MOBY_SLIDE_TO_END_POINT:
        pose_entry = &l05_cuboids[*(int *)(data + 0x10)];
        reach = *(float *)(data + 0x18) * 3.0f;
        slide_step_distance = 0.0f;
        fast_vec_normalize(slide_motion_scratch, &pose_entry->axes[1], reach);
        if (*(int *)(data + 0x1C) != 0) {
            fast_vec_scale(slide_motion_scratch, slide_motion_scratch, -1.0f);
        }
        fast_vec_add(slide_motion_scratch, &pose_entry->pos, slide_motion_scratch);
        fast_vec_sub(slide_motion_scratch, slide_motion_scratch, moby + 0x10);
        remaining_slide_distance = fast_vec_length(slide_motion_scratch);
        speed = fast_vec_length(data);
        advance_accelerated_scalar(remaining_slide_distance,
                                   frame_time_sq * 20.0f,
                                   frame_time_sq * 30.0f,
                                   frame_time * 10.0f,
                                   &slide_step_distance,
                                   &speed);
        fast_vec_normalize(data, slide_motion_scratch, speed);
        fast_vec_add(moby + 0x10, moby + 0x10, data);
        if (fast_vec_length(slide_motion_scratch) < 0.01f) {
            MOBY(moby)->state = 3;
        }
        break;
    case MOBY_LOWER_BELOW_POSE_HEIGHT:
        pose_entry = l05_cuboids;
        pose_entry += *(int *)(data + 0x10);
        advance_accelerated_scalar(pose_entry->pos.z - 5.9f,
                                   frame_time_sq * 20.0f,
                                   frame_time_sq * 40.0f,
                                   frame_time * 10.0f,
                                   (float *)&MOBY(moby)->pos.z,
                                   (float *)(data + 8));
        if (MOBY(moby)->pos.z < pose_entry->pos.z - 5.9f) {
            MOBY(moby)->state = 4;
        }
        break;
    }
}
