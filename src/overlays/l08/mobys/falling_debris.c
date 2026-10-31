/* Level 08 moby classes 0x1b9, 0x1ba, 0x1bb, 0x1bd, 0x1c0, 0x1c1, 0x1c2, 0x1c3, 0x1d0, 0x1d1: falling debris. */
#include "types.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"

extern float D_0015ED70;
extern int   FUN_L00_00257b90(int, int);

/* Update function for moby classes 441-443 on level 08 (falling debris): picks its spin, falls with a smoke trail, bursts into particles near the ground, then falls on until out of bounds. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E2A38), where it is exact; names translated to the US level program. */
typedef struct {
    float vel[4]; /* 0x00 */
    float spin_x; /* 0x10 */
    float spin_y; /* 0x14 */
    int   life;   /* 0x18 */
} Data;

typedef struct {
    char          pad0[0x10];
    float         pos[4]; /* 0x10 */
    unsigned char state;  /* 0x20 */
    char          pad21[0x1F];
    float         rot[4]; /* 0x40 */
    char          pad50[0x28];
    Data         *data; /* 0x78 */
} Moby_2E2A38;

typedef struct {
    int v[2];
} Pair_2E2A38;

extern Pair_2E2A38 D_L08_00161C30 NOT_SDA;
extern float                      D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float D_0015ED70           MACRO_ADDR;
extern float                      FUN_001f96b0_c(float) __asm__("FUN_001f96b0");

#include "rnc/overlay/l08/mobys/falling_debris_data/update_falling_debris_2.h"

extern int l08_falling_debris_burst_initial_particle_word __asm__("D_L08_00161C04")
    __attribute__((sda));
extern int l08_falling_debris_burst_second_particle_word __asm__("D_L08_00161C08")
    __attribute__((sda));

#include "rnc/overlay/l08/mobys/falling_debris_data/falling_debris_update.h"

extern int  spawn_spark() __asm__("FUN_00218888");
extern void func_L00_0026B890_2E2A38(
    void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");
void falling_debris_update(Moby_2E2A38 *debris_moby) __asm__("FUN_L08_002e16c0");

Pair_2E2A38 D_L08_00161C30 = {
    {0x2f000000, 0x3f000000}
};

/* Moby classes 441-443, 445, 448-451, 464, 465 */
void falling_debris_update(Moby_2E2A38 *debris_moby) {
    enum {
        FALLING_DEBRIS_INITIALIZE = 1,
        FALLING_DEBRIS_BEFORE_BURST = 2,
        FALLING_DEBRIS_AFTER_BURST = 3
    };
    Data       *debris_data = debris_moby->data;
    float       particle_primary_vector[4];
    Pair_2E2A38 tab;
    float       particle_secondary_vector[4];
    float       particle_spawn_position[4];
    int         i;

    switch (debris_moby->state) {
    case FALLING_DEBRIS_INITIALIZE:
        debris_data->spin_x =
            random_float_between(-(D_0015ED6C_gp * 1.5707964f), D_0015ED6C_gp * 1.5707964f);
        debris_data->spin_y =
            random_float_between(-(D_0015ED6C_gp * 1.5707964f), D_0015ED6C_gp * 1.5707964f);
        debris_data->life = (int)random_float_between((float)scale_game_frames(0x5A),
                                                      (float)scale_game_frames(0x96));
        debris_moby->state = 2;
    case FALLING_DEBRIS_BEFORE_BURST:
        debris_data->vel[2] -= D_0015ED70 * 10.8f * 0.5f;
        fast_vec_add(debris_moby->pos, debris_moby->pos, debris_data);
        debris_moby->rot[0] = fast_add_rotations(debris_moby->rot[0], debris_data->spin_x);
        debris_moby->rot[1] = fast_add_rotations(debris_moby->rot[1], debris_data->spin_y);
        if (debris_moby->pos[0] < 8.0f || debris_moby->pos[1] < 8.0f ||
            debris_moby->pos[0] > 500.0f || debris_moby->pos[1] > 500.0f ||
            debris_moby->pos[2] > 500.0f) {
            mark_moby_for_removal(debris_moby);
            return;
        }
        if (random_integer_below(3) == 0) {
            int *trail_parameter_entry;
            int  trail_frame_count;
            int  trail_secondary_frame_count;
            clear_u64_value(particle_primary_vector);
            tab = D_L08_00161C30;
            trail_parameter_entry = &tab.v[random_integer_below(2)];
            {
                int lo = scale_game_frames(15), hi = scale_game_frames(20);
                trail_frame_count = FUN_L00_00257b90(lo, hi);
            }
            {
                int lo = scale_game_frames(25);
                trail_secondary_frame_count = FUN_L00_00257b90(lo, scale_game_frames(30));
            }
            func_L00_0026B890_2E2A38(debris_moby->pos,
                                     particle_primary_vector,
                                     0x2F3F3F7F,
                                     *trail_parameter_entry,
                                     trail_frame_count,
                                     trail_secondary_frame_count,
                                     0,
                                     0,
                                     400000.0f,
                                     random_float_between(8.0f, 16.0f) * D_0015ED6C_gp);
        }
        if (debris_moby->pos[2] < 15.2f) {
            for (i = 0; i < falling_debris_burst_particle_count; i++) {
                float particle_planar_angle = random_angle_radians();
                int   burst_frame_count;
                int   burst_secondary_frame_count;
                int   burst_tertiary_frame_count;
                qcopy(particle_spawn_position, debris_moby->pos);
                particle_spawn_position[0] += random_float_between(-3.0f, 3.0f);
                particle_spawn_position[1] += random_float_between(-3.0f, 3.0f);
                particle_primary_vector[2] =
                    falling_debris_burst_primary_vertical_scale * D_0015ED6C_gp;
                particle_primary_vector[0] +=
                    fast_cos(particle_planar_angle) * falling_debris_burst_primary_planar_scale;
                particle_primary_vector[1] +=
                    fast_sin(particle_planar_angle) * falling_debris_burst_primary_planar_scale;
                particle_secondary_vector[2] =
                    falling_debris_burst_secondary_vertical_scale * D_0015ED6C_gp;
                particle_secondary_vector[0] +=
                    fast_cos(particle_planar_angle) * falling_debris_burst_secondary_planar_scale;
                particle_secondary_vector[1] +=
                    fast_sin(particle_planar_angle) * falling_debris_burst_secondary_planar_scale;
                particle_primary_vector[3] = falling_debris_burst_primary_vector_w;
                particle_secondary_vector[3] = falling_debris_burst_secondary_vector_w;
                burst_frame_count = truncate_float_to_s32(FUN_001f96b0_c(random_float_between(
                    (float)falling_debris_burst_frame_count_random_min,
                    (float)(falling_debris_burst_frame_count_random_min * 2))));
                burst_secondary_frame_count =
                    truncate_float_to_s32(FUN_001f96b0_c(random_float_between(
                        (float)falling_debris_burst_secondary_frame_count_random_min,
                        (float)(falling_debris_burst_secondary_frame_count_random_min * 2))));
                burst_tertiary_frame_count =
                    truncate_float_to_s32(FUN_001f96b0_c(random_float_between(
                        (float)falling_debris_burst_tertiary_frame_count_random_min,
                        (float)(falling_debris_burst_tertiary_frame_count_random_min * 2))));
                spawn_spark(particle_spawn_position,
                            particle_primary_vector,
                            particle_secondary_vector,
                            l08_falling_debris_burst_initial_particle_word,
                            l08_falling_debris_burst_second_particle_word,
                            burst_frame_count,
                            burst_secondary_frame_count,
                            burst_tertiary_frame_count,
                            -1);
            }
            debris_moby->state = 3;
        }
        break;
    case FALLING_DEBRIS_AFTER_BURST:
        debris_data->vel[2] -= D_0015ED70 * 10.8f * 0.5f;
        fast_vec_add(debris_moby->pos, debris_moby->pos, debris_data);
        debris_moby->rot[0] = fast_add_rotations(debris_moby->rot[0], debris_data->spin_x);
        debris_moby->rot[1] = fast_add_rotations(debris_moby->rot[1], debris_data->spin_y);
        if (debris_moby->pos[0] < 8.0f || debris_moby->pos[1] < 8.0f ||
            debris_moby->pos[2] < 8.0f || debris_moby->pos[0] > 500.0f ||
            debris_moby->pos[1] > 500.0f) {
            mark_moby_for_removal(debris_moby);
        }
        break;
    }
}
