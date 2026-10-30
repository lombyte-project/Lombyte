/* Mobys: selects a moby's target goal. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"

#include "qcopy.h"
#include "qzero.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

extern char *D_L00_0015FFD8_25fcb8 __asm__("D_L02_0015FFD8");

extern short       D_L02_001B0E30[] NOT_SDA;
extern char       *D_L02_001D1B80[] NOT_SDA;
extern struct Hero hero_lui __asm__("D_0013F350") NOT_SDA;
int                select_moby_target_goal(char *comparison_source,
                                           char *output_data,
                                           float best) __asm__("FUN_L02_00261c80");

int select_moby_target_goal(char *comparison_source, char *output_data, float best) {
    struct Hero *hero = &hero_lui;
    char        *selected_moby;
    char        *candidate_moby;
    int          selection_status;
    int          i;
    int          j;
    float        candidate_metric;

    selected_moby = ((char *)hero->moby);
    selection_status = 0;
    if (hero->state.control_mode == 0x18 || hero->state.current == 0x72) {
        selected_moby = 0;
        selection_status = 2;
    }
    for (i = 1; i <= D_L02_001B0E30[0]; i++) {
        candidate_moby = D_L00_0015FFD8_25fcb8 + (D_L02_001B0E30[i] << 8);
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
        candidate_metric = vector_distance_xy(comparison_source + 0x10, candidate_moby + 0x10);
        if (candidate_metric < best) {
            selected_moby = candidate_moby;
            best = candidate_metric;
            selection_status = 1;
        }
    }
    for (j = 0; j < 20; j++) {
        candidate_moby = D_L02_001D1B80[j];
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
        candidate_metric = vector_distance_xy(comparison_source + 0x10, candidate_moby + 0x10);
        if (candidate_metric < best) {
            selected_moby = candidate_moby;
            best = candidate_metric;
            selection_status = 1;
        }
    }
    if (selected_moby != 0) {
        *(char **)(output_data + 0x40) = selected_moby;
        qcopy(output_data, selected_moby + 0x10);
        qcopy(output_data + 0x10, selected_moby + 0x40);
        hero = &hero_lui;
        if (selected_moby == ((char *)hero->moby)) {
            qcopy(output_data + 0x20, &hero->motion.near_ground_point);
            qcopy(output_data + 0x30, &hero->motion.body_center);
        } else {
            qcopy(output_data + 0x30, output_data);
            *(float *)(output_data + 0x38) += 0.6f;
            qcopy(output_data + 0x20, output_data + 0x30);
        }
    } else {
        *(char **)(output_data + 0x40) = 0;
        qzero(output_data);
        qzero(output_data + 0x10);
        qzero(output_data + 0x30);
        qzero(output_data + 0x20);
    }
    *(int *)(output_data + 0x44) = selection_status;
    return selection_status;
}
