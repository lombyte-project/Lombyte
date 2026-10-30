/* Level 07 moby helper: get_target_sight, a line-of-sight test to a target. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"

#include "qcopy.h"
#include "qzero.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

extern short       D_L07_001B08B0[] NOT_SDA;
extern char       *D_L07_001D1600[] NOT_SDA;
extern struct Hero hero_lui __asm__("D_0013F350") NOT_SDA;
int                get_target_sight(char *a, char *o, float best) __asm__("FUN_L07_002886d8");

int get_target_sight(char *source_moby, char *sight_out, float best) {
    struct Hero *hero_context = &hero_lui;
    char        *target_moby;
    char        *candidate_moby;
    int          target_selection_status;
    int          i;
    int          j;
    float        candidate_metric;

    target_moby = ((char *)hero_context->moby);
    target_selection_status = 0;
    if (hero_context->state.control_mode == 0x18 || hero_context->state.current == 0x72) {
        target_moby = 0;
        target_selection_status = 2;
    }
    for (i = 1; i <= D_L07_001B08B0[0]; i++) {
        candidate_moby = level_mobys + (D_L07_001B08B0[i] << 8);
        if (candidate_moby == 0)
            continue;
        if (MOBY(candidate_moby)->oclass != 0xCB && MOBY(candidate_moby)->oclass != 0x76C)
            continue;
        if (MOBY(candidate_moby)->state == 0xFE)
            continue;
        if (MOBY(candidate_moby)->state == 0xFD)
            continue;
        if (MOBY(candidate_moby)->state != 3)
            continue;
        candidate_metric = vector_distance_xy(source_moby + 0x10, candidate_moby + 0x10);
        if (candidate_metric < best) {
            target_moby = candidate_moby;
            best = candidate_metric;
            target_selection_status = 1;
        }
    }
    for (j = 0; j < 20; j++) {
        candidate_moby = D_L07_001D1600[j];
        if (candidate_moby == 0)
            continue;
        if (MOBY(candidate_moby)->oclass != 0x10E)
            continue;
        if (MOBY(candidate_moby)->state == 0xFE)
            continue;
        if (MOBY(candidate_moby)->state == 0xFD)
            continue;
        if (MOBY(candidate_moby)->command == 0)
            continue;
        candidate_metric = vector_distance_xy(source_moby + 0x10, candidate_moby + 0x10);
        if (candidate_metric < best) {
            target_moby = candidate_moby;
            best = candidate_metric;
            target_selection_status = 1;
        }
    }
    if (target_moby != 0) {
        *(char **)(sight_out + 0x40) = target_moby;
        qcopy(sight_out, target_moby + 0x10);
        qcopy(sight_out + 0x10, target_moby + 0x40);
        hero_context = &hero_lui;
        if (target_moby == ((char *)hero_context->moby)) {
            qcopy(sight_out + 0x20, &hero_context->motion.near_ground_point);
            qcopy(sight_out + 0x30, &hero_context->motion.body_center);
        } else {
            qcopy(sight_out + 0x30, sight_out);
            *(float *)(sight_out + 0x38) += 0.6f;
            qcopy(sight_out + 0x20, sight_out + 0x30);
        }
    } else {
        *(char **)(sight_out + 0x40) = 0;
        qzero(sight_out);
        qzero(sight_out + 0x10);
        qzero(sight_out + 0x30);
        qzero(sight_out + 0x20);
    }
    *(int *)(sight_out + 0x44) = target_selection_status;
    return target_selection_status;
}
