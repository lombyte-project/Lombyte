/* Level 15 conveyor belt moby class. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern void update_entity_matrix(char *a, void *b, void *c, void *d) __asm__("FUN_L00_00260738");

void conveyor_belt_update(struct Moby *moby) __asm__("FUN_L15_002e73c0");

void conveyor_belt_update(struct Moby *moby) {
    enum {
        CONVEYOR_BELT_INITIALIZE = 0,
        CONVEYOR_BELT_UPDATE_FRAME_OFFSET = 3
    };
    char *data = (char *)moby->pvars;
    if (data != 0) {
        unsigned char state = moby->state;
        switch (state) {
        case CONVEYOR_BELT_INITIALIZE: {
            char *frame_offset = data + 0x40;
            clear_u64_value(frame_offset);
            *(int *)(data + 0x9C) |= 4;
            moby->state = 3;
            break;
        }
        case CONVEYOR_BELT_UPDATE_FRAME_OFFSET:
            moby->anim_speed = 1.3888889f;
            *(float *)(data + 0x40) =
                fast_cos(fast_add_rotations(moby->rot.z, 1.5707964f)) * (frame_time * 2.5f);
            *(float *)(data + 0x44) =
                fast_sin(fast_add_rotations(moby->rot.z, 1.5707964f)) * (frame_time * 2.5f);
            *(int *)(data + 0x48) = 0;
            break;
        }
        update_entity_matrix(data + 0x60, data + 0x40, &moby->rot, &moby->rot);
    }
}

extern float fast_add_rotations(float, float);
