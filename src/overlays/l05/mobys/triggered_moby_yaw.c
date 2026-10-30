/* Level 05 moby class that turns when triggered. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"

extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern short D_L05_0015FFD8 __asm__("D_L05_0015FFD8");
extern float
approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");

void         update_triggered_moby_yaw(struct Moby *moby) __asm__("FUN_L05_003166a0");
extern float fast_add_rotations(float, float);
extern float fast_subtract_rotations(float, float);

void update_triggered_moby_yaw(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE_YAW = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_APPROACH_SAVED_YAW_OFFSET = 2
    };
    int *data = (int *)moby->pvars;

    switch (moby->state) {
    case MOBY_INITIALIZE_YAW:
        if (data[0] != 0)
            moby->flags |= 0x8000;
        moby->state = 1;
        *(float *)&data[3] = moby->rot.z;
        if (data[0] != 0)
            moby->rot.z = fast_subtract_rotations(moby->rot.z, 1.5707964f);
        else
            moby->rot.z = fast_add_rotations(moby->rot.z, 1.5707964f);
        /* fall through */
    case MOBY_WAIT_FOR_TRIGGER:
        if (((unsigned char *)*(int *)&D_L05_0015FFD8)[data[2] * 256 + 0xBC] != 0) {
            allocate_voice_for_target_entry(0, 0, moby);
            moby->state = 2;
        }
        break;
    case MOBY_APPROACH_SAVED_YAW_OFFSET: {
        float target;
        if (data[0] != 0)
            target = fast_subtract_rotations(*(float *)&data[3], 0.6981317f);
        else
            target = fast_add_rotations(*(float *)&data[3], 0.6981317f);
        approach_angle(&moby->rot.z,
                       target,
                       (float *)&data[1],
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        break;
    }
    }
}
