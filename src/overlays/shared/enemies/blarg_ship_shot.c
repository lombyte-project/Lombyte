/* Blarg ship shot (class 0x3F9): a streak projectile that leaves a trail, and the function that spawns it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"

extern char D_L11_00174880[];

extern char *FUN_L00_0026dd20(char *pos, char *vel, int c, int d, float f);
extern int   projectile_trail_color_start_gp __asm__("D_L11_00161DC0") __attribute__((sda));
int projectile_trail_color_end_gp __asm__("D_L11_00161DC4") = 0x80808020; /* RGBA, 0xAABBGGRR */
extern int projectile_trail_frame_count_gp __asm__("D_L11_00161DC8") __attribute__((sda));
extern int l11_projectile_trail_initial_particle_scalar __asm__("D_L11_00161DCC")
    __attribute__((sda));
float      projectile_trail_velocity_scale_gp __asm__("D_L11_00161DD0") = 0.5f;
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

/* spawns a streak moby aimed along a direction with the given size and colour */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030A318), where it is exact; names translated to the US level program. */

char *spawn_blarg_ship_shot(int    owner,
                            float *launch_direction,
                            float *spawn_position,
                            float  size,
                            float  step_length,
                            float  z) __asm__("FUN_L11_00308f48");

char *spawn_blarg_ship_shot(int    firing_moby,
                            float *launch_direction,
                            float *spawn_position,
                            float  size,
                            float  step_length,

                            float z) {
    char *spawned_moby = create_moby(0x3F9);
    if (spawned_moby != 0) {
        char *moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        int   size_length_ratio;
        MOBY(spawned_moby)->collision = 0;
        *(int *)(moby_vars + 0x1C) = firing_moby;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = truncate_float_to_s32(size);
        *(int *)(spawned_moby + 0x40) = 0;
        spawned_moby[0x31] = 1;
        MOBY(spawned_moby)->rot.y =
            -fast_arc_tan(vector_length_xy(launch_direction), launch_direction[2]);
        MOBY(spawned_moby)->rot.z = fast_arc_tan(launch_direction[0], launch_direction[1]);
        *(float *)(moby_vars + 0x10) = 1.0f;
        if (step_length > 0.0f) {
            fast_vec_normalize(moby_vars, launch_direction, step_length);
        } else {
            qcopy(moby_vars, launch_direction);
            step_length = fast_vec_length(launch_direction);
        }
        qcopy(spawned_moby + 0x10, spawn_position);
        size_length_ratio = truncate_float_to_s32(size / step_length);
        *(int *)(moby_vars + 0x14) = size_length_ratio;
        *(float *)(moby_vars + 0x20) = z;
        *(int *)(moby_vars + 0x18) = size_length_ratio - 13;
        MOBY(spawned_moby)->flags = 0x200;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030A468), where it is exact; names translated to the US level program. */

/* Per-frame update of a projectile moby: moves it by its velocity and leaves a trail particle; on impact it bursts into
   five sparks and removes itself, otherwise it shrinks when its timer runs out and is removed once tiny. */

void blarg_ship_shot_update(char *moby) __asm__("FUN_L11_00309098");

/* Moby class 1017 */
void blarg_ship_shot_update(char *moby) {
    float previous_moby_position[4];
    float trail_position[4];
    float trail_velocity[4];
    float hit[4];
    float pad[4][2];
    float spark_velocity[4];
    float spark_scatter_vector[4];
    char *projectile_vars;
    char *moby_position;
    int   trail_color;
    if (moby == 0)
        return;
    projectile_vars = ((char *)MOBY(moby)->pvars);
    if (projectile_vars == 0)
        return;
    moby_position = moby + 0x10;
    qcopy(previous_moby_position, moby_position);
    trail_color = fast_tween_color(projectile_trail_color_start_gp,
                                   projectile_trail_color_end_gp,
                                   random_float_between(0.0f, 1.0f));
    random_vector(trail_position, 0.0f, 0.1f);
    fast_vec_add(trail_position, trail_position, moby_position);
    fast_vec_scale(trail_velocity, projectile_vars, projectile_trail_velocity_scale_gp);
    FUN_L00_0026dd20((char *)trail_position,
                     (char *)trail_velocity,
                     trail_color,
                     scale_game_frames(projectile_trail_frame_count_gp),
                     (float)l11_projectile_trail_initial_particle_scalar);
    fast_vec_add(moby_position, moby_position, projectile_vars);
    make_hit_descriptor(hit, moby, 0x10000, *(float *)(projectile_vars + 0x20), projectile_vars);
    if (collision_line(
            previous_moby_position, moby_position, 0, *(void **)(projectile_vars + 0x1C), hit)) {
        int i;
        for (i = 0; i < 5; i++) {
            *(OvlQuad *)spark_scatter_vector = 0;
            spark_scatter_vector[0] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector[1] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector[2] = random_float_between(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)spark_scatter_vector;
            reflect_vector(spark_scatter_vector, projectile_vars, D_L11_00174880);
            fast_vec_normalize(
                spark_velocity, spark_velocity, fast_vec_length(spark_scatter_vector) * 0.5f);
            fast_vec_add(spark_velocity, spark_scatter_vector, spark_velocity);
            fast_vec_normalize(spark_velocity,
                               spark_velocity,
                               random_float_between(frame_time * 5.0f, frame_time * 9.0f));
            FUN_L00_0026dd20(moby + 0x10,
                             (char *)spark_velocity,
                             0x7F2F4F6F,
                             random_int_between(scale_game_frames(10), scale_game_frames(15)),
                             90000.0f);
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (fast_dec_timer((int *)(projectile_vars + 0x18))) {
        *(float *)(projectile_vars + 0x10) *= 0.74f;
        if (*(float *)(projectile_vars + 0x10) < 0.02f) {
            mark_moby_for_removal(moby);
        }
    }
}

int   D_L13_00161D20 = 0x80802000;
int   D_L13_00161D24 = 0x80808020;
int   D_L13_00161D28 = 0xa;
int   D_L13_00161D2C = 0x5208;
float D_L13_00161D30 = 0.5f;

int   D_L17_00161DB0 = 0x80802000;
int   D_L17_00161DB4 = 0x80808020;
int   D_L17_00161DB8 = 0xa;
int   D_L17_00161DBC = 0x5208;
float D_L17_00161DC0 = 0.5f;

int projectile_trail_color_start_gp __asm__("D_L11_00161DC0") = 0x80802000;

int projectile_trail_frame_count_gp __asm__("D_L11_00161DC8") = 0xa;
