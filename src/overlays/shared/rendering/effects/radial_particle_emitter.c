/* Radial particle emitter (class 0x65D): while the camera target is in range, spawns particles on a random disc around the moby. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/shared/rendering/effects/ground_particle_scatter_globals.h"
#include "rnc/overlay/shared/rendering/effects/ground_particle_scatter.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern char  D_L08_001675C0[];
extern void  spawn_span_particle(void *, void *, void *, float) __asm__("FUN_L08_00272cb8");

/* While the player is within range, scatters spawned particles around the moby on a random disc. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00309AB0), where it is exact; names translated to the US level program. */

void radial_particle_emitter_update(char *moby) __asm__("FUN_L08_003085f0");

/* Moby class 1629 */
void radial_particle_emitter_update(char *moby) {
    float range_check_delta_or_spawn_position[4];
    float spawn_center_position[4];
    float reference_to_spawn_delta[4];
    float randomized_spawn_vector[4];
    char *moby_data = ((char *)MOBY(moby)->pvars);
    int   i;
    fast_vec_sub(range_check_delta_or_spawn_position, D_L08_001675C0, moby + 0x10);
    range_check_delta_or_spawn_position[2] = 0.0f;
    if (fast_vec_dot(range_check_delta_or_spawn_position, range_check_delta_or_spawn_position) <
        *(float *)(moby_data + 4) * *(float *)(moby_data + 4)) {
        float angle_start_radians, angle_end_radians, xy_random_range;
        i = 0;
        xy_random_range = *(float *)(moby_data + 0x24) * frame_time;
        *(OvlQuad *)spawn_center_position = *(OvlQuad *)&MOBY(moby)->pos;
        angle_start_radians = (*(float *)(moby_data + 0x28) - 180.0f) / 180.0f * 3.1415927f;
        angle_end_radians = (*(float *)(moby_data + 0x2C) - 180.0f) / 180.0f * 3.1415927f;
        for (; i < *(int *)(moby_data + 0x14); i++) {
            float spawn_angle_radians, rad;
            *(OvlQuad *)range_check_delta_or_spawn_position = *(OvlQuad *)spawn_center_position;
            spawn_angle_radians = random_float_between(angle_start_radians, angle_end_radians);
            rad = random_float_between(*(float *)(moby_data + 0xC), *(float *)(moby_data + 8));
            range_check_delta_or_spawn_position[0] =
                range_check_delta_or_spawn_position[0] + fast_cos(spawn_angle_radians) * rad;
            range_check_delta_or_spawn_position[1] =
                range_check_delta_or_spawn_position[1] + fast_sin(spawn_angle_radians) * rad;
            fast_vec_sub(
                reference_to_spawn_delta, range_check_delta_or_spawn_position, D_L08_001675C0);
            if (!(vector_length_xy(reference_to_spawn_delta) <= *(float *)(moby_data + 0x10))) {
                randomized_spawn_vector[0] =
                    ground_particle_scatter_spawn_vector_xy_bias[0] +
                    random_float_between(-xy_random_range, xy_random_range);
                randomized_spawn_vector[1] =
                    ground_particle_scatter_spawn_vector_y_bias +
                    random_float_between(-xy_random_range, xy_random_range);
                randomized_spawn_vector[2] = -(random_float_between(*(float *)(moby_data + 0x18),
                                                                    *(float *)(moby_data + 0x1C)) *
                                               frame_time);
                randomized_spawn_vector[3] = 0.0f;
                spawn_span_particle(range_check_delta_or_spawn_position,
                                    *(void **)moby_data,
                                    randomized_spawn_vector,
                                    MOBY(moby)->pos.z - *(float *)(moby_data + 0x20));
            }
        }
    }
}
