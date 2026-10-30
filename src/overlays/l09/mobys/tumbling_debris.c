/* Level 09 moby class 0x139: tumbling debris. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "sda.h"

extern char  D_L09_00166F40_2ef430[] __asm__("D_L09_00166F40");
extern void *spawn_spark_2ef430(void *, void *, int, int, float) __asm__("FUN_L00_0026dd20");

/* Per-frame update of a tumbling debris moby: moves and spins it under gravity, bounces it off what it hits, and
   bursts it into two sparks and removes it when its timer runs out or it leaves the area. */
void update_tumbling_debris(struct Moby *moby) __asm__("FUN_L09_002ef430");

void update_tumbling_debris(struct Moby *moby) {
    float  old[4];
    float  spark_velocity[4];
    float  random_spark_vector[4];
    float *debris_vars = (float *)moby->pvars;
    float *pos = (float *)&moby->pos;
    int    i;

    qcopy(old, pos);
    fast_vec_add(pos, pos, debris_vars);
    if (moby->pos.x < 0.0f || moby->pos.y < 0.0f || moby->pos.z < 0.0f ||
        vector_distance_xy(pos, D_L09_00166F40_2ef430) > 64.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    moby->rot.z = fast_add_rotations(moby->rot.z, debris_vars[4]);
    moby->rot.y = fast_add_rotations(moby->rot.y, debris_vars[5]);
    debris_vars[2] -= frame_time_sq * 9.8f;
    if (fast_dec_timer((int *)(debris_vars + 7))) {
        for (i = 0; i < 2; i++) {
            *(OvlQuad *)random_spark_vector = 0;
            random_spark_vector[0] = random_float_between(-1.0f, 1.0f);
            random_spark_vector[1] = random_float_between(-1.0f, 1.0f);
            random_spark_vector[2] = random_float_between(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)random_spark_vector;
            fast_vec_normalize(
                spark_velocity, spark_velocity, fast_vec_length(random_spark_vector) * 0.5f);
            fast_vec_add(spark_velocity, random_spark_vector, spark_velocity);
            fast_vec_normalize(spark_velocity,
                               spark_velocity,
                               random_float_between(frame_time * 3.0f, frame_time * 6.0f));
            spawn_spark_2ef430(&moby->pos,
                               spark_velocity,
                               0x7F2F4F6F,
                               random_int_between(scale_game_frames(10), scale_game_frames(15)),
                               30000.0f);
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (collision_line(old, pos, 2, moby, 0)) {
        reflect_vector(debris_vars, debris_vars, D_L09_00174000);
        fast_vec_scale(debris_vars, debris_vars, random_float_between(0.45f, 0.75f));
        if (fast_vec_length(debris_vars) < frame_time_sq * 10.0f) {
            clear_u64_value(debris_vars);
        }
        fast_vec_normalize(D_L09_00174000, D_L09_00174000, 0.05f);
        fast_vec_add(pos, D_L09_00174000 - 0x20, D_L09_00174000);
    }
}
