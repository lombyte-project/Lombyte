/* Level 03 moby class path_mover: a moby that moves along a rail path. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/quad.h"

#include "rnc/overlay/l03/mobys/path_mover_globals.h"

extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

/* Moves the moby along its path: advances the distance, places it on the
 * current segment, and dies at the end of the path. */
void update_path_mover(struct Moby *path_moby) __asm__("FUN_L03_002db020");

void update_path_mover(struct Moby *path_moby) {
    float old[4];
    float movement_scratch[4];
    float segment_offset[4];
    char *path_moby_vars;
    char *path;
    int   i;
    float segment_fraction;

    *(OvlQuad *)old = *(OvlQuad *)&path_moby->pos;
    path_moby_vars = (char *)path_moby->pvars;
    *(float *)(path_moby_vars + 0x64) += path_mover_distance_step;
    movement_scratch[0] = fast_cos(path_moby->rot.z) * *(float *)(path_moby_vars + 0x64);
    movement_scratch[1] = fast_sin(path_moby->rot.z) * *(float *)(path_moby_vars + 0x64);
    *(int *)&movement_scratch[2] = 0;
    fast_vec_add(movement_scratch, movement_scratch, *(char **)(path_moby_vars + 0x60) + 0x10);
    i = truncate_float_to_s32(*(float *)(path_moby_vars + 0x64) /
                              *(float *)(*(char **)(path_moby_vars + 0x60) + 0x1C));
    segment_fraction = convert_integer_to_float(i);
    path = *(char **)(path_moby_vars + 0x60);
    segment_fraction =
        (*(float *)(path_moby_vars + 0x64) - segment_fraction * *(float *)(path + 0x1C)) /
        *(float *)(path + 0x1C);
    if (i >= *(int *)path - 2) {
        mark_moby_for_removal(path_moby);
        return;
    }
    fast_vec_sub(segment_offset, ((float(*)[4])path)[i + 2], ((float(*)[4])path)[i + 1]);
    fast_vec_scale(segment_offset, segment_offset, segment_fraction);
    fast_vec_add(&path_moby->pos, segment_offset, (*(float(**)[4])(path_moby_vars + 0x60))[i + 1]);
    fast_vec_sub(movement_scratch, &path_moby->pos, old);
    update_entity_matrix(path_moby_vars + 0x20, movement_scratch, &path_moby->rot, &path_moby->rot);
}

float path_mover_distance_step = 0;
