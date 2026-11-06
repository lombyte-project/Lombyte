/* Level 05 swinging flap (class 895): turns away from its placed yaw, then swings back once its linked moby gets a command. */
#include "types.h"
#include "rnc/overlay/l05.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"

extern int allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern float
approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");

void         swinging_flap_update(struct Moby *moby) __asm__("FUN_L05_003166a0");
extern float fast_add_rotations(float, float);
extern float fast_subtract_rotations(float, float);

/* Moby class 895 */
void swinging_flap_update(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE_YAW = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_APPROACH_SAVED_YAW_OFFSET = 2
    };
    int *yaw_control_vars = (int *)moby->pvars;

    switch (moby->state) {
    case MOBY_INITIALIZE_YAW:
        if (yaw_control_vars[0] != 0)
            moby->flags |= 0x8000;
        moby->state = MOBY_WAIT_FOR_TRIGGER;
        *(float *)&yaw_control_vars[3] = moby->rot.z;
        if (yaw_control_vars[0] != 0)
            moby->rot.z = fast_subtract_rotations(moby->rot.z, 1.5707964f);
        else
            moby->rot.z = fast_add_rotations(moby->rot.z, 1.5707964f);
        /* fall through */
    case MOBY_WAIT_FOR_TRIGGER:
        if (((unsigned char *)l05_moby_array)[yaw_control_vars[2] * 256 + 0xBC] != 0) {
            allocate_voice_for_target_entry(0, 0, moby);
            moby->state = MOBY_APPROACH_SAVED_YAW_OFFSET;
        }
        break;
    case MOBY_APPROACH_SAVED_YAW_OFFSET: {
        float target;
        if (yaw_control_vars[0] != 0)
            target = fast_subtract_rotations(*(float *)&yaw_control_vars[3], 0.6981317f);
        else
            target = fast_add_rotations(*(float *)&yaw_control_vars[3], 0.6981317f);
        approach_angle(&moby->rot.z,
                       target,
                       (float *)&yaw_control_vars[1],
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        break;
    }
    }
}
