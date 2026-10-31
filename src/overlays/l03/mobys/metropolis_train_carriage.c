/* Level 03 moby class metropolis_train_carriage. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "qcopy.h"

extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002965A0), where it is exact; names translated to the US level program. */

void metropolis_train_carriage_update(struct Moby *moby) __asm__("FUN_L03_002953f8");

/* Moby class 1210 */
void metropolis_train_carriage_update(struct Moby *moby) {
    float position_delta[4];
    float joint_zero_world_position[4];
    char *carriage_pvars;
    char *current_position;
    char *current_rotation;
    char *previous_rotation;
    char *previous_position;
    carriage_pvars = (char *)moby->pvars;
    if ((moby->flags & 2) == 0) {
        current_position = &moby->pos;
        previous_position = carriage_pvars + 0xA0;
        previous_rotation = carriage_pvars + 0xB0;
        current_rotation = &moby->rot;
        if (moby->state == 0) {
            *(int *)(carriage_pvars + 0x60) = 0;
            *(short *)(carriage_pvars + 0x64) = 0;
            carriage_pvars[0x68] = 4;
            *(short *)(carriage_pvars + 0x7E) = 0xD;
            moby_point_position(moby, 0, joint_zero_world_position);
            *(float *)(carriage_pvars + 0xC4) =
                fast_vec_dist(current_position, joint_zero_world_position);
            qcopy(previous_position, current_position);
            qcopy(previous_rotation, current_rotation);
            *(int *)(carriage_pvars + 0x5C) = 3;
            moby->update_dist = moby->draw_dist = 0xFF;
        }
        fast_vec_sub(position_delta, current_position, previous_position);
        update_entity_matrix(
            carriage_pvars + 0x20, position_delta, previous_rotation, current_rotation);
        qcopy(previous_position, current_position);
        qcopy(previous_rotation, current_rotation);
    }
}
