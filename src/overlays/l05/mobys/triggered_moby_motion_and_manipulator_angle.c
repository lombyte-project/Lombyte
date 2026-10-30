/* Level 05 moby class that moves and turns a manipulator when triggered. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/attach_manipulator.h"

extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern short D_L05_0015FFD8 __asm__("D_L05_0015FFD8");
extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float FUN_L00_0025be00(float *, float *, float, float, float, float);

extern void  FUN_L00_001fff28(void *, int, float);
extern char *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;

void update_triggered_moby_motion_and_manipulator_angle(char *moby) __asm__("FUN_L05_00316258");

void update_triggered_moby_motion_and_manipulator_angle(char *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_WAIT_TO_RETURN_TO_SAVED_POSITION = 2,
        MOBY_MOVE_TO_OFFSET_POSITION = 3,
        MOBY_RETURN_TO_SAVED_POSITION = 4,
        MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE = 5,
        MOBY_SETTLE_AT_ZERO_MANIPULATOR_ANGLE = 6
    };
    char *data = ((char *)MOBY(moby)->pvars);
    float target[4];
    float delta[4];
    float stopped;

    switch (MOBY(moby)->state) {
    case MOBY_INITIALIZE:
        if (*(int *)(data + 0x50) != 0) {
            MOBY(moby)->rot.x = 3.1415927f;
            MOBY(moby)->pos.z += 4.0f;
        }
        qcopy(data, moby + 0x10);
        attach_manipulator(moby, 0, data + 0x10);
        if (*(int *)(data + 0x54) != 0) {
            *(float *)(data + 0x60) = 0.0f;
            MOBY(moby)->state = 2;
            if (*(int *)(data + 0x50) != 0)
                fast_vec_scale(target, moby + 0xD0, 2.0f);
            else
                fast_vec_scale(target, moby + 0xD0, -2.0f);
            fast_vec_add(moby + 0x10, moby + 0x10, target);
        } else {
            MOBY(moby)->state = 1;
            *(float *)(data + 0x60) = 0.5934119f;
            FUN_L00_001fff28(data + 0x20, 0, 0.5934119f);
        }
        *(long long *)(moby + 0x38) = *(long long *)(D_001413D0_lui + 0x38);
        break;
    case MOBY_WAIT_FOR_TRIGGER:
    case MOBY_WAIT_TO_RETURN_TO_SAVED_POSITION: {
        u8 trigger = ((u8 *)*(int *)&D_L05_0015FFD8)[*(int *)(data + 0x58) * 256 + 0xBC];
        if (trigger == 0)
            return;
        if (MOBY(moby)->state == 2 && trigger != 2) {
            MOBY(moby)->state = 4;
            break;
        }
        if (MOBY(moby)->state != 1)
            return;
        allocate_voice_for_target_entry(0, 0, moby);
        MOBY(moby)->state = 6;
        return;
    }
    case MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE:
    case MOBY_SETTLE_AT_ZERO_MANIPULATOR_ANGLE: {
        float end;
        float approach;
        if (MOBY(moby)->state == 5) {
            end = 0.5934119f;
            approach = 1.1868238f;
        } else {
            end = 0.0f;
            approach = -0.5934119f;
        }
        FUN_L00_0025be00((float *)(data + 0x60),
                         (float *)(data + 0x64),
                         approach,
                         frame_time_sq * 4.1887903f,
                         frame_time_sq * 4.1887903f,
                         frame_time * 12.566371f);
        if (*(float *)(data + 0x60) > 0.5934119f || *(float *)(data + 0x60) < 0.0f) {
            if (*(float *)(data + 0x60) > 0.5934119f)
                *(float *)(data + 0x60) = 0.5934119f;
            else if (*(float *)(data + 0x60) < 0.0f)
                *(float *)(data + 0x60) = 0.0f;
            if (MOBY(moby)->command == 0) {
                MOBY(moby)->command = 1;
                *(float *)(data + 0x64) *= -0.33f;
            } else if (*(float *)(data + 0x60) == end) {
                if (MOBY(moby)->state == 5)
                    MOBY(moby)->state = 7;
                else
                    MOBY(moby)->state = 3;
            }
        }
        FUN_L00_001fff28(data + 0x20, 0, *(float *)(data + 0x60));
    } break;
    case MOBY_MOVE_TO_OFFSET_POSITION:
    case MOBY_RETURN_TO_SAVED_POSITION:
        if (MOBY(moby)->state == 3) {
            fast_vec_scale(target, moby + 0xD0, -1.9f);
            fast_vec_add(target, data, target);
        } else {
            qcopy(target, data);
        }
        fast_vec_sub(delta, target, moby + 0x10);
        stopped = 0.0f;
        advance_accelerated_scalar(fast_vec_length(delta),
                                   frame_time_sq * 6.0f,
                                   frame_time_sq * 12.0f,
                                   frame_time * 10.0f,
                                   &stopped,
                                   (float *)(data + 0x5C));
        fast_vec_sub(delta, target, moby + 0x10);
        fast_vec_normalize(delta, delta, *(float *)(data + 0x5C));
        fast_vec_add(moby + 0x10, moby + 0x10, delta);
        if (!(fast_vec_dist(moby + 0x10, target) < 0.0001f))
            return;
        if (MOBY(moby)->state == 3) {
            allocate_voice_for_target_entry(1, 0, moby);
            MOBY(moby)->state = 7;
            return;
        }
        MOBY(moby)->state = 5;
        break;
    }
}
