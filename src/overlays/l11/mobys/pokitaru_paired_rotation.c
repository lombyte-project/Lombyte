/* Level 11 moby class pokitaru_paired_rotation. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002CC9D0), where it is exact; names translated to the US level program. */

extern float
approach_angle(float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");

void update_pokitaru_paired_rotation(struct Moby *moby) __asm__("FUN_L11_002cb810");

void update_pokitaru_paired_rotation(struct Moby *moby) {
    enum {
        ROTATION_INITIALIZE = 0,
        ROTATION_WAIT_FOR_TRIGGER = 1,
        ROTATION_APPROACH_OFFSET = 2
    };
    char *data = (char *)moby->pvars;
    int   state = (unsigned char)MOBY(moby)->state;
    switch (state) {
    case ROTATION_INITIALIZE:
        MOBY(moby)->state = 1;
        *(float *)(data + 4) = moby->rot.z;
        if ((moby->flags & 0x8000) == 0) {
            char *partner_moby = create_moby(0x41);
            MOBY(partner_moby)->draw_dist = moby->draw_dist;
            partner_moby[0x30] = moby->update_dist;
            MOBY(partner_moby)->flags = moby->flags | 0x8000;
            qcopy(partner_moby + 0x10, &moby->pos);
            qcopy(partner_moby + 0x40, &moby->rot);
            *(char **)(data + 8) = partner_moby;
            *(float *)(((char *)MOBY(partner_moby)->pvars) + 4) = moby->rot.z;
        }
        break;
    case ROTATION_WAIT_FOR_TRIGGER:
        break;
    case ROTATION_APPROACH_OFFSET: {
        float target_angle;
        if ((moby->flags & 0x8000) == 0)
            target_angle = fast_add_rotations(*(float *)(data + 4), 0.20769417f);
        else
            target_angle = fast_subtract_rotations(*(float *)(data + 4), 0.20769417f);
        if (approach_angle(&moby->rot.z,
                           target_angle,
                           (float *)data,
                           frame_time_sq * 0.34906584f,
                           frame_time_sq * 0.34906584f,
                           frame_time * 0.7853982f) == 0.0f)
            MOBY(moby)->state = 3;
        break;
    }
    }
}
