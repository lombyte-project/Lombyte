/* Level 02 moby class tresspasser_door2. */
#include "types.h"
#include "rnc/math/fast_math.h"

extern float D_0015ED6C;

extern void FUN_L00_0025f730(void *a, float x);

#include "qcopy.h"

#include "rnc/mobys/moby.h"
/* moves a moby through three states: latch position, wait for a peer to reach state 4, then slide toward it */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E1570), where it is exact; names translated to the US level program. */

extern char D_L02_0015FFD8_c3 __asm__("D_L02_0015FFD8");

void tresspasser_door2_update(struct Moby *moby) __asm__("FUN_L02_002e0138");

/* Moby class 744 */
void tresspasser_door2_update(struct Moby *moby) {
    enum {
        TRESSPASSER_DOOR2_CAPTURE_START_POSITION = 0,
        TRESSPASSER_DOOR2_WAIT_FOR_LINKED_MOBY = 1,
        TRESSPASSER_DOOR2_SLIDE_TO_OFFSET_POSITION = 2
    };
    char *tresspasser_door2_vars = (char *)moby->pvars;
    switch (moby->state) {
    case TRESSPASSER_DOOR2_CAPTURE_START_POSITION:
        qcopy(tresspasser_door2_vars, &moby->pos);
        moby->state = 1;
        break;
    case TRESSPASSER_DOOR2_WAIT_FOR_LINKED_MOBY: {
        int id = *(int *)(tresspasser_door2_vars + 0x10);
        if (id >= 0) {
            char *linked_moby = (char *)((id << 8) + *(int *)&D_L02_0015FFD8_c3);
            int   linked_moby_class = MOBY(linked_moby)->oclass;
            if (linked_moby_class == 0x267 || linked_moby_class == 0x23F) {
                if (((unsigned char *)linked_moby)[0x20] == 4)
                    moby->state = 2;
            }
        }
        break;
    }
    case TRESSPASSER_DOOR2_SLIDE_TO_OFFSET_POSITION: {
        float destination_position[4];
        float movement_step[4];
        char *pos = &moby->pos;
        fast_vec_normalize(destination_position, &moby->rot_row1, -2.0f);
        fast_vec_add(destination_position, tresspasser_door2_vars, destination_position);
        fast_vec_sub(movement_step, destination_position, pos);
        FUN_L00_0025f730(movement_step, D_0015ED6C + D_0015ED6C);
        fast_vec_add(pos, pos, movement_step);
        if (fast_vec_length(movement_step) == 0.0f)
            moby->state = 3;
        break;
    }
    }
}
