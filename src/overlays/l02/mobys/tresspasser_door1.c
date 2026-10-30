/* Level 02 moby class tresspasser_door1. */
#include "types.h"
#include "rnc/math/fast_math.h"

extern float D_0015ED6C;

extern void FUN_L00_0025f730(void *a, float x);

#include "qcopy.h"

/* State machine for a moby that picks a new animation each time the previous one finishes. */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E05E0), where it is exact; names translated to the US level program. */

extern int FUN_0022da68(int, int, int);

#include "rnc/mobys/moby.h"
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E1400), where it is exact; names translated to the US level program. */

extern char D_L02_0015FFD8_c2 __asm__("D_L02_0015FFD8");

void tresspasser_door1_update(struct Moby *moby) __asm__("FUN_L02_002dffc8");

void tresspasser_door1_update(struct Moby *moby) {
    enum {
        TRESSPASSER_DOOR1_CAPTURE_START_POSITION = 0,
        TRESSPASSER_DOOR1_WAIT_FOR_LINKED_MOBY = 1,
        TRESSPASSER_DOOR1_SLIDE_TO_OFFSET_POSITION = 2
    };
    char *door_vars = (char *)moby->pvars;
    float destination_position[4];
    float movement_step[4];
    switch (moby->state) {
    case TRESSPASSER_DOOR1_CAPTURE_START_POSITION:
        qcopy(door_vars, &moby->pos);
        moby->state = 1;
        break;
    case TRESSPASSER_DOOR1_WAIT_FOR_LINKED_MOBY:
        if (*(int *)(door_vars + 0x10) >= 0) {
            unsigned char *o =
                (unsigned char *)((*(int *)(door_vars + 0x10) << 8) + *(int *)&D_L02_0015FFD8_c2);
            short c = *(short *)(o + 0xA6);
            if (c == 0x267 || c == 0x23F) {
                if (o[0x20] == 4) {
                    moby->state = 2;
                    FUN_0022da68(0, 0, (int)moby);
                }
            }
        }
        break;
    case TRESSPASSER_DOOR1_SLIDE_TO_OFFSET_POSITION:
        fast_vec_normalize(destination_position, &moby->rot_row1, 2.0f);
        fast_vec_add(destination_position, door_vars, destination_position);
        fast_vec_sub(movement_step, destination_position, &moby->pos);
        FUN_L00_0025f730(movement_step, D_0015ED6C + D_0015ED6C);
        fast_vec_add(&moby->pos, &moby->pos, movement_step);
        if (fast_vec_length(movement_step) == 0.0f) {
            FUN_0022da68(1, 0, (int)moby);
            moby->state = 3;
        }
        break;
    }
}
