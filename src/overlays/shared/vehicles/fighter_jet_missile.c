/* Fighter jet missile (class 0x3F1): the homing missile the fighter jets fire, and the function that spawns it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/runtime/callbacks/noop_main_callback.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"

typedef struct {
    float x, y, z;
} Vec3_089d0;

typedef struct {
    float        vel[4];
    float        size;
    int          unk14;
    int          timer;
    struct Moby *owner;
    float        target_pos[3];
    struct Moby *target;
    float       *aim;
    float       *src;
} MissileVars;

typedef struct {
    u8  pad0[0x1A];
    s16 oclass;
    u8  pad1C[0x14];
} MissileHit;

extern char *target_search_moby_list_head __asm__("D_L11_0015FFE4");

extern char *D_L11_0015FFD8_089d0 __asm__("D_L11_0015FFD8");
extern char  D_L11_00174880[];
extern void
initialize_hit_descriptor(float, void *, struct Moby *, int) __asm__("FUN_L00_00259858");
extern char *FUN_L00_0026dd20_c(void *, void *, int, int, float) __asm__("FUN_L00_0026dd20");
char *select_homing_missile_target(float *pos, float *aim, float r1, float r2, float maxd) __asm__("FUN_L11_00308670");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00309A40), where it is exact; names translated to the US level program. */

char *select_homing_missile_target(float *targeting_origin,
                                   float *aim,
                                   float  max_yaw_error,
                                   float  max_pitch_error,
                                   float  maxd) __asm__("FUN_L11_00308670");

char *select_homing_missile_target(

    float *targeting_origin, float *aim, float max_yaw_error, float max_pitch_error, float maxd) {
    char *best = 0;
    char *candidate_position;
    char *candidate_moby = target_search_moby_list_head;
    float bestd = 10.0f;
    while (candidate_moby != 0) {
        if ((MOBY(candidate_moby)->flags & 0x1000) && candidate_moby != 0 &&
            ((char *)MOBY(candidate_moby)->pclass) != 0 &&
            *(short *)(((char *)MOBY(candidate_moby)->pclass) + 0x46) == 5) {
            float distance_xy;
            candidate_position = candidate_moby + 0x10;
            distance_xy = vector_distance_xy(targeting_origin, candidate_position);
            if (distance_xy < maxd) {
                float adjusted_target_position[4];
                float ra, rb, y, score;
                qcopy(adjusted_target_position, candidate_position);
                adjusted_target_position[2] = adjusted_target_position[2] + 0.4f;
                ra = fast_difference_between_rotations(
                    aim[2],
                    fast_arc_tan(adjusted_target_position[0] - targeting_origin[0],
                                 adjusted_target_position[1] - targeting_origin[1]));
                ra = ra * ra;
                if (ra < max_yaw_error * max_yaw_error) {
                    y = fast_difference_between_rotations(
                        aim[1],
                        fast_arc_tan(distance_xy,
                                     adjusted_target_position[2] - targeting_origin[2]));
                    rb = y * y;
                    if (rb < max_pitch_error * max_pitch_error) {
                        score = ra * rb * 30.0f;
                        score = score + square_root(fast_vec_dist(targeting_origin,
                                                                  adjusted_target_position));
                        if (score < bestd) {
                            bestd = score;
                            best = candidate_moby;
                        }
                    }
                }
            }
        }
        candidate_moby = ((char *)MOBY(candidate_moby)->next);
    }
    return best;
}

/* spawns a moby of class 0x3F1 aimed along a direction */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00309C18), where it is exact; names translated to the US level program. */

unsigned char *spawn_fighter_jet_missile(char  *owner,
                                         float *launch_direction,
                                         float *spawn_position,
                                         float  travel_distance,
                                         float  step_length) __asm__("FUN_L11_00308848");

unsigned char *spawn_fighter_jet_missile(char  *launching_moby,
                                         float *launch_direction,
                                         float *spawn_position,
                                         float  travel_distance,

                                         float step_length) {
    unsigned char *spawned_moby = create_moby(0x3F1);
    if (spawned_moby) {
        char *moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        float direction_length_xy;
        *(char **)(moby_vars + 0x1C) = launching_moby;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = truncate_float_to_s32(travel_distance);
        *(int *)(spawned_moby + 0x40) = 0;
        spawned_moby[0x31] = 1;
        direction_length_xy = vector_length_xy(launch_direction);
        MOBY(spawned_moby)->rot.y = -fast_arc_tan(direction_length_xy, launch_direction[2]);
        MOBY(spawned_moby)->rot.z = fast_arc_tan(launch_direction[0], launch_direction[1]);
        *(float *)(moby_vars + 0x10) = 1.0f;
        if (step_length > 0.0f) {
            fast_vec_normalize(moby_vars, launch_direction, step_length);
        } else {
            qcopy(moby_vars, launch_direction);
            step_length = fast_vec_length(launch_direction);
        }
        qcopy(spawned_moby + 0x10, spawn_position);
        *(int *)(moby_vars + 0x18) = *(int *)(moby_vars + 0x14) =
            truncate_float_to_s32(travel_distance / step_length);
        MOBY(spawned_moby)->flags = 0x200;
        MOBY(spawned_moby)->scale = *(float *)(((char *)MOBY(spawned_moby)->pclass) + 0x24) *
                                    (*(float *)(launching_moby + 0x2C) /
                                     *(float *)(*(char **)(launching_moby + 0x24) + 0x24));
        spawned_moby[0x23] = truncate_float_to_s32(*(float *)(moby_vars + 0x10) * 24.0f);
        set_moby_color(spawned_moby, 0xFF, 0xFF, 0xFF);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

static inline void copy_xyz_089d0(float *dst, float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

/* Homing missile: eases its scale, re-picks a target every sixth frame, turns toward it within a cone and flies on;
   on impact it bursts into five sparks, otherwise it shrinks away once its timer runs out. */

void fighter_jet_missile_update(struct Moby *moby) __asm__("FUN_L11_003089d0");

/* Moby class 1009 */
void fighter_jet_missile_update(struct Moby *moby) {
    MissileVars *missile_vars = (MissileVars *)moby->pvars;
    float        previous_missile_position[4];
    float        look[4];
    MissileHit   hit;
    float        spark_velocity[4];
    float        spark_scatter_vector[4];
    struct Moby *target;
    float        speed, yaw, yaw_off, pitch, pitch_off, max_yaw, max_pitch;
    int          i;

    moby->scale += (moby->pclass->scale - moby->scale) * 0.1f;
    qcopy(previous_missile_position, &moby->pos);
    if (level_frame % 6 == (((char *)moby - D_L11_0015FFD8_089d0) >> 8) % 6)
        missile_vars->target = 0;
    moby->alpha = truncate_float_to_s32(missile_vars->size * 24.0f);
    copy_xyz_089d0(look, missile_vars->target_pos);
    ((int *)look)[3] = 0;
    target = missile_vars->target;
    if (target == 0 || target->state == 0xFE || target->state == 0xFD ||
        fast_vec_dist(&target->pos, look) > 3.0f)
        missile_vars->target = (struct Moby *)select_homing_missile_target(
            (float *)&moby->pos, (float *)&moby->rot, 0.2617994f, 0.2617994f, 60.0f);
    if (missile_vars->target != 0) {
        copy_xyz_089d0(missile_vars->target_pos, (float *)&missile_vars->target->pos);
        speed = fast_vec_length(missile_vars);
        yaw_off = fast_subtract_rotations(fast_arc_tan(missile_vars->target->pos.x - moby->pos.x,
                                                       missile_vars->target->pos.y - moby->pos.y),
                                          moby->rot.z);
        yaw = absolute_float(yaw_off);
        pitch_off = fast_subtract_rotations(
            -fast_arc_tan(vector_distance_xy(&moby->pos, &missile_vars->target->pos),
                          missile_vars->target->pos.z - moby->pos.z),
            moby->rot.y);
        pitch = absolute_float(pitch_off);
        if (yaw < 0.2617994f && pitch < 0.2617994f) {
            max_yaw = frame_time * 0.87266463f;
            if (max_yaw < yaw) {
                yaw_off /= yaw;
                yaw_off *= max_yaw;
            }
            moby->rot.z = fast_add_rotations(moby->rot.z, yaw_off);
            max_pitch = frame_time * 0.87266463f;
            if (max_pitch < pitch) {
                pitch_off /= pitch;
                pitch_off *= max_pitch;
            }
            moby->rot.y = fast_add_rotations(moby->rot.y, pitch_off);
            build_spherical_offset(missile_vars, speed, moby->rot.z, -moby->rot.y);
            noop_main_callback(&moby->pos, &missile_vars->target->pos);
        } else {
            missile_vars->target = 0;
        }
    }
    fast_vec_add(&moby->pos, &moby->pos, missile_vars);
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    initialize_hit_descriptor(1.0f, &hit, moby, 0x10000);
    hit.oclass = moby->oclass;
    if (collision_line(previous_missile_position, &moby->pos, 0, missile_vars->owner, &hit)) {
        for (i = 0; i < 5; i++) {
            *(OvlQuad *)spark_scatter_vector = 0;
            spark_scatter_vector[0] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector[1] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector[2] = random_float_between(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)spark_scatter_vector;
            reflect_vector(spark_scatter_vector, missile_vars, D_L11_00174880);
            fast_vec_normalize(
                spark_velocity, spark_velocity, fast_vec_length(spark_scatter_vector) * 0.5f);
            fast_vec_add(spark_velocity, spark_scatter_vector, spark_velocity);
            fast_vec_normalize(spark_velocity,
                               spark_velocity,
                               random_float_between(frame_time * 5.0f, frame_time * 9.0f));
            FUN_L00_0026dd20_c(&moby->pos,
                               spark_velocity,
                               0x7F2F4F6F,
                               random_int_between(scale_game_frames(10), scale_game_frames(15)),
                               90000.0f);
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (fast_dec_timer(&missile_vars->timer)) {
        missile_vars->size *= 0.74f;
        if (missile_vars->size < 0.02f)
            mark_moby_for_removal(moby);
    }
}
