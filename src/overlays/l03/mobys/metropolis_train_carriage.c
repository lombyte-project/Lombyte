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

typedef struct MetropolisTrainCarriageVars {
    u8  pad_0[0x20];
    f32 entity_matrix[4];
    u8  pad_30[0x2C];
    s32 entity_matrix_flags;
    s32 unk60;
    s16 unk64;
    u8  pad_66[0x2];
    s8  unk68;
    u8  pad_69[0x15];
    s16 unk7E;
    u8  pad_80[0x20];
    u8  previous_position;
    u8  pad_A1[0xF];
    u8  previous_rotation;
    u8  pad_B1[0x13];
    f32 joint_0_distance;
} MetropolisTrainCarriageVars;

/* Moby class 1210 */
void metropolis_train_carriage_update(struct Moby *moby) {
    enum {
        METROPOLIS_TRAIN_CARRIAGE_STATE_INITIALIZE = 0
    };
    float                        position_delta[4];
    float                        joint_zero_world_position[4];
    MetropolisTrainCarriageVars *carriage_pvars;
    char                        *current_position;
    char                        *current_rotation;
    char                        *previous_rotation;
    char                        *previous_position;
    carriage_pvars = (MetropolisTrainCarriageVars *)moby->pvars;
    if ((moby->flags & 2) == 0) {
        current_position = &moby->pos;
        previous_position = &carriage_pvars->previous_position;
        previous_rotation = &carriage_pvars->previous_rotation;
        current_rotation = &moby->rot;
        if (moby->state == METROPOLIS_TRAIN_CARRIAGE_STATE_INITIALIZE) {
            carriage_pvars->unk60 = 0;
            carriage_pvars->unk64 = 0;
            carriage_pvars->unk68 = 4;
            carriage_pvars->unk7E = 0xD;
            moby_point_position(moby, 0, joint_zero_world_position);
            carriage_pvars->joint_0_distance =
                fast_vec_dist(current_position, joint_zero_world_position);
            qcopy(previous_position, current_position);
            qcopy(previous_rotation, current_rotation);
            carriage_pvars->entity_matrix_flags = 3;
            moby->update_dist = moby->draw_dist = 0xFF;
        }
        fast_vec_sub(position_delta, current_position, previous_position);
        update_entity_matrix(
            carriage_pvars->entity_matrix, position_delta, previous_rotation, current_rotation);
        qcopy(previous_position, current_position);
        qcopy(previous_rotation, current_rotation);
    }
}
