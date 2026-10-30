/* Mobys: selects a moby record by XY distance. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"

#include "qcopy.h"
#include "qzero.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

extern short       D_L04_001B09B0[] NOT_SDA;
extern char       *D_L04_001D1700[] NOT_SDA;
extern struct Hero hero_lui __asm__("D_0013F350") NOT_SDA;
int select_moby_record_by_xy_distance(char *a, char *o, float best) __asm__("FUN_L04_00252e18");

int select_moby_record_by_xy_distance(char *reference_object, char *output_record, float best) {
    struct Hero *hero = &hero_lui;
    char        *selected_moby;
    char        *candidate_moby;
    int          selection_status;
    int          i;
    int          j;
    float        candidate_comparison_value;

    selected_moby = ((char *)hero->moby);
    selection_status = 0;
    if (hero->state.control_mode == 0x18 || hero->state.current == 0x72) {
        selected_moby = 0;
        selection_status = 2;
    }
    for (i = 1; i <= D_L04_001B09B0[0]; i++) {
        candidate_moby = moby_array + (D_L04_001B09B0[i] << 8);
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
        candidate_comparison_value =
            vector_distance_xy(reference_object + 0x10, candidate_moby + 0x10);
        if (candidate_comparison_value < best) {
            selected_moby = candidate_moby;
            best = candidate_comparison_value;
            selection_status = 1;
        }
    }
    for (j = 0; j < 20; j++) {
        candidate_moby = D_L04_001D1700[j];
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
        candidate_comparison_value =
            vector_distance_xy(reference_object + 0x10, candidate_moby + 0x10);
        if (candidate_comparison_value < best) {
            selected_moby = candidate_moby;
            best = candidate_comparison_value;
            selection_status = 1;
        }
    }
    if (selected_moby != 0) {
        *(char **)(output_record + 0x40) = selected_moby;
        qcopy(output_record, selected_moby + 0x10);
        qcopy(output_record + 0x10, selected_moby + 0x40);
        hero = &hero_lui;
        if (selected_moby == ((char *)hero->moby)) {
            qcopy(output_record + 0x20, &hero->motion.near_ground_point);
            qcopy(output_record + 0x30, &hero->motion.body_center);
        } else {
            qcopy(output_record + 0x30, output_record);
            *(float *)(output_record + 0x38) += 0.6f;
            qcopy(output_record + 0x20, output_record + 0x30);
        }
    } else {
        *(char **)(output_record + 0x40) = 0;
        qzero(output_record);
        qzero(output_record + 0x10);
        qzero(output_record + 0x30);
        qzero(output_record + 0x20);
    }
    *(int *)(output_record + 0x44) = selection_status;
    return selection_status;
}
