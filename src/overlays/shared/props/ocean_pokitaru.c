/* Shared moby classes: rocket, ocean_pokitaru. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/vectors/random_vector.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/commands/set_moby_color.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/runtime/callbacks/noop_main_callback.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

extern int  FUN_00116248_b7(void *, void *) __asm__("FUN_00116248");
extern void FUN_0020c828_b7(void *) __asm__("FUN_0020c828");

extern unsigned char l11_proximity_removal_flag[] __asm__("D_0013D389");
extern char          D_L11_00179B18[];
extern char          raritanium_acquired_text[] __asm__("D_L11_0021A900");

#include "rnc/overlay/shared/props/ocean_pokitaru_globals.h"

void remove_moby_when_near_hero(char *moby) __asm__("FUN_L11_002cb790");

void remove_moby_when_near_hero(char *moby) {
    if (fast_vec_dist(moby + 0x10, &hero.motion.pos) < 2.0f) {
        l11_proximity_removal_flag[0] = 1;
        FUN_00116248_b7(D_L11_00179B18, raritanium_acquired_text);
        proximity_removal_frame_count = scale_game_frames(0xB4);
        FUN_0020c828_b7(moby);
    }
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00309A40), where it is exact; names translated to the US level program. */

extern char *target_search_moby_list_head __asm__("D_L11_0015FFE4");

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

#include "qcopy.h"

/* spawns a moby of class 0x3F1 aimed along a direction */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00309C18), where it is exact; names translated to the US level program. */

unsigned char *spawn_class_3f1_moby(char  *owner,
                                    float *launch_direction,
                                    float *spawn_position,
                                    float  travel_distance,
                                    float  step_length) __asm__("FUN_L11_00308848");

unsigned char *spawn_class_3f1_moby(char  *owner,
                                    float *launch_direction,
                                    float *spawn_position,
                                    float  travel_distance,

                                    float step_length) {
    unsigned char *spawned_moby = create_moby(0x3F1);
    if (spawned_moby) {
        char *moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        float direction_length_xy;
        *(char **)(moby_vars + 0x1C) = owner;
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
        MOBY(spawned_moby)->scale =
            *(float *)(((char *)MOBY(spawned_moby)->pclass) + 0x24) *
            (*(float *)(owner + 0x2C) / *(float *)(*(char **)(owner + 0x24) + 0x24));
        spawned_moby[0x23] = truncate_float_to_s32(*(float *)(moby_vars + 0x10) * 24.0f);
        set_moby_color(spawned_moby, 0xFF, 0xFF, 0xFF);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

#include "rnc/overlay/quad.h"
#include "rnc/mobys/moby.h"

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

extern char *D_L11_0015FFD8_089d0 __asm__("D_L11_0015FFD8");
extern char  D_L11_00174880[];
extern void
initialize_hit_descriptor(float, void *, struct Moby *, int) __asm__("FUN_L00_00259858");
extern char *FUN_L00_0026dd20_c(void *, void *, int, int, float) __asm__("FUN_L00_0026dd20");
char *select_homing_missile_target(float *pos, float *aim, float r1, float r2, float maxd) __asm__("FUN_L11_00308670");

static inline void copy_xyz_089d0(float *dst, float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

/* Homing missile: eases its scale, re-picks a target every sixth frame, turns toward it within a cone and flies on;
   on impact it bursts into five sparks, otherwise it shrinks away once its timer runs out. */
void update_homing_missile(struct Moby *moby) __asm__("FUN_L11_003089d0");

void update_homing_missile(struct Moby *moby) {
    MissileVars *vars = (MissileVars *)moby->pvars;
    float        old[4];
    float        look[4];
    MissileHit   hit;
    float        spark_velocity[4];
    float        spark_scatter_vector[4];
    struct Moby *target;
    float        speed, yaw, yaw_off, pitch, pitch_off, max_yaw, max_pitch;
    int          i;

    moby->scale += (moby->pclass->scale - moby->scale) * 0.1f;
    qcopy(old, &moby->pos);
    if (level_frame % 6 == (((char *)moby - D_L11_0015FFD8_089d0) >> 8) % 6)
        vars->target = 0;
    moby->alpha = truncate_float_to_s32(vars->size * 24.0f);
    copy_xyz_089d0(look, vars->target_pos);
    ((int *)look)[3] = 0;
    target = vars->target;
    if (target == 0 || target->state == 0xFE || target->state == 0xFD ||
        fast_vec_dist(&target->pos, look) > 3.0f)
        vars->target = (struct Moby *)select_homing_missile_target(
            (float *)&moby->pos, (float *)&moby->rot, 0.2617994f, 0.2617994f, 60.0f);
    if (vars->target != 0) {
        copy_xyz_089d0(vars->target_pos, (float *)&vars->target->pos);
        speed = fast_vec_length(vars);
        yaw_off = fast_subtract_rotations(
            fast_arc_tan(vars->target->pos.x - moby->pos.x, vars->target->pos.y - moby->pos.y),
            moby->rot.z);
        yaw = absolute_float(yaw_off);
        pitch_off = fast_subtract_rotations(
            -fast_arc_tan(vector_distance_xy(&moby->pos, &vars->target->pos),
                          vars->target->pos.z - moby->pos.z),
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
            build_spherical_offset(vars, speed, moby->rot.z, -moby->rot.y);
            noop_main_callback(&moby->pos, &vars->target->pos);
        } else {
            vars->target = 0;
        }
    }
    fast_vec_add(&moby->pos, &moby->pos, vars);
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    initialize_hit_descriptor(1.0f, &hit, moby, 0x10000);
    hit.oclass = moby->oclass;
    if (collision_line(old, &moby->pos, 0, vars->owner, &hit)) {
        for (i = 0; i < 5; i++) {
            *(OvlQuad *)spark_scatter_vector = 0;
            spark_scatter_vector[0] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector[1] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector[2] = random_float_between(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)spark_scatter_vector;
            reflect_vector(spark_scatter_vector, vars, D_L11_00174880);
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
    if (fast_dec_timer(&vars->timer)) {
        vars->size *= 0.74f;
        if (vars->size < 0.02f)
            mark_moby_for_removal(moby);
    }
}
/* spawns a streak moby aimed along a direction with the given size and colour */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030A318), where it is exact; names translated to the US level program. */

char *spawn_class_3f9_moby(int    owner,
                           float *launch_direction,
                           float *spawn_position,
                           float  size,
                           float  step_length,
                           float  z) __asm__("FUN_L11_00308f48");

char *spawn_class_3f9_moby(int    owner,
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
        *(int *)(moby_vars + 0x1C) = owner;
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

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030A468), where it is exact; names translated to the US level program. */
extern char *FUN_L00_0026dd20(char *pos, char *vel, int c, int d, float f);
extern int   projectile_trail_color_start_gp __asm__("D_L11_00161DC0") __attribute__((sda));
extern int   projectile_trail_color_end_gp __asm__("D_L11_00161DC4") __attribute__((sda));
extern int   projectile_trail_frame_count_gp __asm__("D_L11_00161DC8") __attribute__((sda));
extern int   D_L11_00161DCC __attribute__((sda));
extern float projectile_trail_velocity_scale_gp __asm__("D_L11_00161DD0") __attribute__((sda));
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
/* Per-frame update of a projectile moby: moves it by its velocity and leaves a trail particle; on impact it bursts into
   five sparks and removes itself, otherwise it shrinks when its timer runs out and is removed once tiny. */
void update_projectile_with_trail(char *moby) __asm__("FUN_L11_00309098");

void update_projectile_with_trail(char *moby) {
    float old[4];
    float trail_position[4];
    float trail_velocity[4];
    float hit[4];
    float pad[4][2];
    float spark_velocity[4];
    float spark_scatter_vector[4];
    char *data;
    char *moby_position;
    int   trail_color;
    if (moby == 0)
        return;
    data = ((char *)MOBY(moby)->pvars);
    if (data == 0)
        return;
    moby_position = moby + 0x10;
    qcopy(old, moby_position);
    trail_color = fast_tween_color(projectile_trail_color_start_gp,
                                   projectile_trail_color_end_gp,
                                   random_float_between(0.0f, 1.0f));
    random_vector(trail_position, 0.0f, 0.1f);
    fast_vec_add(trail_position, trail_position, moby_position);
    fast_vec_scale(trail_velocity, data, projectile_trail_velocity_scale_gp);
    FUN_L00_0026dd20((char *)trail_position,
                     (char *)trail_velocity,
                     trail_color,
                     scale_game_frames(projectile_trail_frame_count_gp),
                     (float)D_L11_00161DCC);
    fast_vec_add(moby_position, moby_position, data);
    make_hit_descriptor(hit, moby, 0x10000, *(float *)(data + 0x20), data);
    if (collision_line(old, moby_position, 0, *(void **)(data + 0x1C), hit)) {
        int i;
        for (i = 0; i < 5; i++) {
            *(OvlQuad *)spark_scatter_vector = 0;
            spark_scatter_vector[0] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector[1] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector[2] = random_float_between(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)spark_scatter_vector;
            reflect_vector(spark_scatter_vector, data, D_L11_00174880);
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
    if (fast_dec_timer((int *)(data + 0x18))) {
        *(float *)(data + 0x10) *= 0.74f;
        if (*(float *)(data + 0x10) < 0.02f) {
            mark_moby_for_removal(moby);
        }
    }
}

typedef struct {
    Vec4         step;       /* moved by this each frame; its length is speed */
    Vec4         target_pos; /* 0x10 */
    void        *collision;  /* 0x20 */
    struct Moby *target;     /* 0x24 */
    s32          timer;      /* 0x28 */
    f32          speed;      /* 0x2C */
    f32          turn_speed; /* 0x30 */
} Missile_94f8;

extern void
FUN_L00_0026cbb0_94f8(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern unsigned char *FUN_L00_002712b8_94f8(void *,
                                            void *,
                                            int,
                                            int,
                                            int,
                                            int,
                                            float,
                                            float,
                                            float,
                                            float,
                                            float) __asm__("FUN_L00_002712b8");
extern void           FUN_L00_0025e450_94f8(void *,
                                            void *,
                                            void *,
                                            float,
                                            float,
                                            int,
                                            int,
                                            int,
                                            float,
                                            float,
                                            float,
                                            float,
                                            int,
                                            float,
                                            int,
                                            int,
                                            int,
                                            int) __asm__("FUN_L00_0025e450");

extern char rocket_smoke_texture[] __asm__("D_L11_001B2A80");
extern char rocket_impact_effect_origin[] __asm__("D_L11_00174860");
/* Homing missile: turns toward its target while the target lives and stays within 3 units of
   where it was last frame, leaves a smoke trail, and bursts on a hit, when its timer runs out
   or outside the level box. */
void rocket_update(struct Moby *moby) __asm__("FUN_L11_003094f8");

void rocket_update(struct Moby *moby) {
    Missile_94f8 *data = (Missile_94f8 *)moby->pvars;
    Vec4f        *pos;
    Vec4          previous_pos, spray, offset, velocity, random;
    float         fraction;
    struct Moby  *target;
    char         *texture;

    qcopy(&previous_pos, &moby->pos);
    moby->scale += (moby->pclass->scale - moby->scale) * 0.1f;
    random.q = 0;
    random.f[0] = random_float_between(-1.0f, 1.0f);
    random.f[1] = random_float_between(-1.0f, 1.0f);
    random.f[2] = random_float_between(-1.0f, 1.0f);
    velocity.q = random.q;
    fast_vec_normalize(&velocity, &velocity, random_float_between(0.1f, 0.2f) * frame_time);
    fast_vec_normalize(&offset, &data->step, -random_float_between(frame_time * 0.1f, frame_time));
    fast_vec_add(&velocity, &velocity, &offset);
    FUN_L00_002712b8_94f8(&moby->pos,
                          &velocity,
                          scale_game_frames(6),
                          0x7f,
                          0xb0b0b0,
                          3,
                          40000.0f,
                          1000.0f,
                          1.0f,
                          -0.0004f,
                          0.0f);
    texture = rocket_smoke_texture;
    for (fraction = 0.0f; fraction < 1.0f; fraction += 0.33333334f) {
        unsigned char *particle;
        fast_vec_normalize(&offset,
                           &data->step,
                           random_float_between(fraction, fraction + 0.33333334f) * data->speed);
        fast_vec_add(&offset, &offset, &moby->pos);
        particle = FUN_L00_002712b8_94f8(&offset,
                                         &velocity,
                                         scale_game_frames(60),
                                         0x7f,
                                         0x606060,
                                         3,
                                         40000.0f,
                                         1000.0f,
                                         1.0f,
                                         -0.0002f,
                                         0.0f);
        if (particle != 0) {
            unsigned char *texture_pixels = *(unsigned char **)(texture + 0x5c);
            unsigned char  texture_pixel = texture_pixels[0];
            particle[3] = 0x44;
            particle[2] = texture_pixel;
        }
        random.q = 0;
        random.f[2] = 0.02f;
        transform_vector_by_basis(&random, &random, &moby->rot_row0);
        build_look_at_matrix(&random, &random, &moby->rot_row0, random_angle_radians());
        FUN_L00_0026cbb0_94f8(
            &offset, &random, 0x4f007fff, 0x1fffffff, scale_game_frames(5), 1, 20000.0f);
    }
    pos = &moby->pos;
    target = data->target;
    if (target != 0 && target->state != 0xfe && target->state != 0xfd &&
        fast_vec_dist(&data->target_pos, &target->pos) < 3.0f) {
        moby->rot.z =
            fast_arc_tan(data->target->pos.x - moby->pos.x, data->target->pos.y - moby->pos.y);
        moby->rot.y = -fast_arc_tan(vector_distance_xy(pos, &data->target->pos),
                                    data->target->pos.z - moby->pos.z);
        qcopy(&data->target_pos, &data->target->pos);
    } else {
        approach_angle(&moby->rot.z,
                       &data->turn_speed,
                       moby->rot.z,
                       frame_time_sq * 251.32741f,
                       frame_time_sq * 251.32741f,
                       frame_time * 2513.2742f);
        approach_angle(&moby->rot.y,
                       &data->turn_speed,
                       moby->rot.y,
                       frame_time_sq * 251.32741f,
                       frame_time_sq * 251.32741f,
                       frame_time * 2513.2742f);
        data->target = 0;
    }
    build_spherical_offset(&data->step, data->speed, moby->rot.z, -moby->rot.y);
    fast_vec_add(pos, pos, &data->step);
    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f ||
        1021.0f < moby->pos.y || moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    moby_point_position(moby, 1, &spray);
    if (collision_line(&previous_pos, &spray, 0, data->collision, 0)) {
        FUN_L00_0025e450_94f8(moby,
                              data,
                              rocket_impact_effect_origin,
                              2.0f,
                              10.0f,
                              10,
                              3,
                              0x10,
                              4.0f,
                              2.0f,
                              9.0f,
                              1.0f,
                              -1,
                              20.0f,
                              0,
                              1,
                              -1,
                              0);
        mark_moby_for_removal(moby);
        return;
    }
    if (fast_dec_timer(&data->timer))
        mark_moby_for_removal(moby);
}

extern void raise_kernel_trap(void) __asm__("FUN_001f97b0");

typedef struct {
    float u, v;
} UV;

typedef struct {
    float x, y, z;
} Pos;

typedef struct {
    Vec4f v[4];
    int   col[4];
    UV    uv[4];
    long  z70;
    long  z78;
    long  q80;
    long  q88;
} Quad;

typedef struct {
    int   f0;
    int   f4;
    int   f8;
    int   fC;
    int   f10;
    float f14;
    float f18;
    int   f1C;
    int   f20;
    float f24;
    float f28;
    float f2C;
} Data;

typedef struct {
    char  pad[0x228];
    float f228, f22C;
} Fog;

extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  func_L00_001FDE48_30a908(int, void *, void *, void *, int) __asm__("FUN_L00_001fde98");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L11_001677C0_30a908[] __asm__("D_L11_001677C0");
extern UV    wave_offset[] __asm__("D_L11_00161E08") MACRO_ADDR;

extern char  render_fog_context[] __asm__("D_L11_0016D440");
extern char  ocean_strip_orientation_basis[] __asm__("D_L11_001679D0");
extern Pos   l11_ocean_strip_vertex_positions[] __asm__("D_L11_001D9B08");
extern int   ocean_strip_vertex_colors[] __asm__("D_L11_001D9E08");
extern UV    l11_ocean_strip_vertex_uvs[] __asm__("D_L11_001DA310");
extern Pos   l11_ocean_wave_grid_vertex_positions[] __asm__("D_L11_001D9F08");
extern int   D_L11_001DA208[];
extern UV    l11_ocean_wave_grid_vertex_uvs[] __asm__("D_L11_001DA510");
extern float wave_speed[2] __asm__("D_L11_00161DF8") __attribute__((sda));
extern float tide_level_macro __asm__("D_L11_00161E18") MACRO_ADDR;
/* Draws the level's water sheet: a fading edge strip, a far quad, and two scrolling layers of grid cells around the camera. */
void draw_pokitaru_ocean(struct Moby *moby) __asm__("FUN_L11_0030a908");

void draw_pokitaru_ocean(struct Moby *moby) {
    Vec4f scratch_vector;
    Vec4f side_offset;
    Vec4f strip_length_direction;
    Vec4f strip_width_direction;
    Vec4f strip_corners[4];
    Vec4f first_strip_edge_delta;
    Vec4f second_strip_edge_delta;
    Quad  far_quad;
    Data *ocean_vars = (Data *)moby->pvars;
    float saved_fog_228 = ((Fog *)render_fog_context)->f228;
    float s22C = ((Fog *)render_fog_context)->f22C;
    int   i;
    int   layer;
    int   colA;
    int   colB;
    int   vertex_count;
    float color_interpolation_scalar;
    float x;
    float y;

    if (hero.state.current != 0x1D && D_L11_0016D180 == 0) {
        ((Fog *)render_fog_context)->f228 = 255.0f;
        ((Fog *)render_fog_context)->f22C = 249.9f;
    }
    vu1_add_g_sregister(0x42, 0x3200000044L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1);
    vu1_add_g_sregister(6, get_effect_texture(0x16));
    font_queue_vu_state();
    strip_length_direction.x = 1.0f;
    strip_length_direction.y = 0.0f;
    strip_length_direction.z = 0.0f;
    strip_length_direction.w = 1.0f;
    transform_vector_by_basis(
        &strip_length_direction, &strip_length_direction, ocean_strip_orientation_basis);
    strip_width_direction.x = 0.0f;
    strip_width_direction.y = 1.0f;
    strip_width_direction.z = 0.0f;
    strip_width_direction.w = 1.0f;
    transform_vector_by_basis(
        &strip_width_direction, &strip_width_direction, ocean_strip_orientation_basis);
    strip_length_direction.z = 0.0f;
    fast_vec_normalize(&strip_length_direction, &strip_length_direction, 1.0f);
    strip_width_direction.z = 0.0f;
    fast_vec_normalize(&strip_width_direction, &strip_width_direction, 1.0f);
    fast_vec_normalize(&scratch_vector, &strip_length_direction, -75.0f);
    fast_vec_normalize(&side_offset, &strip_width_direction, -87.0f);
    fast_vec_add(&strip_corners[0], &scratch_vector, &side_offset);
    fast_vec_normalize(&side_offset, &strip_width_direction, 87.0f);
    fast_vec_add(&strip_corners[2], &scratch_vector, &side_offset);
    fast_vec_normalize(&scratch_vector, &strip_length_direction, ocean_vars->f28);
    fast_vec_normalize(&side_offset, &strip_width_direction, -750.0f);
    fast_vec_add(&strip_corners[1], &scratch_vector, &side_offset);
    fast_vec_normalize(&side_offset, &strip_width_direction, 750.0f);
    fast_vec_add(&strip_corners[3], &scratch_vector, &side_offset);
    for (i = 0; i < 4; i++) {
        Vec4f *strip_corner = &strip_corners[i];
        fast_vec_add(strip_corner, strip_corner, D_L11_001677C0_30a908);
        strip_corner->z = tide_level_macro;
    }
    fast_vec_sub(&first_strip_edge_delta, &strip_corners[1], &strip_corners[0]);
    fast_vec_sub(&second_strip_edge_delta, &strip_corners[3], &strip_corners[2]);
    color_interpolation_scalar = 10.0f;
    for (i = 0; i < 12; i += 2) {
        float strip_fraction = convert_integer_to_float(i) / 10.0f;
        int   col = fast_tween_color(ocean_vars->f0,
                                   ocean_vars->f4,
                                   convert_integer_to_float(i) / color_interpolation_scalar);
        fast_vec_scale(&scratch_vector, &first_strip_edge_delta, strip_fraction);
        ocean_strip_vertex_colors[i] = col;
        l11_ocean_strip_vertex_uvs[i].u = 0.0f;
        l11_ocean_strip_vertex_uvs[i].v = 0.0f;
        fast_vec_add(&scratch_vector, &scratch_vector, &strip_corners[0]);
        l11_ocean_strip_vertex_positions[i].x = scratch_vector.x;
        l11_ocean_strip_vertex_positions[i].y = scratch_vector.y;
        l11_ocean_strip_vertex_positions[i].z = scratch_vector.z;
        fast_vec_scale(&scratch_vector, &second_strip_edge_delta, strip_fraction);
        ocean_strip_vertex_colors[i + 1] = col;
        l11_ocean_strip_vertex_uvs[i + 1].u = 0.0f;
        l11_ocean_strip_vertex_uvs[i + 1].v = 0.0f;
        fast_vec_add(&scratch_vector, &scratch_vector, &strip_corners[2]);
        l11_ocean_strip_vertex_positions[i + 1].x = scratch_vector.x;
        l11_ocean_strip_vertex_positions[i + 1].y = scratch_vector.y;
        l11_ocean_strip_vertex_positions[i + 1].z = scratch_vector.z;
    }
    func_L00_001FDE48_30a908(0xC,
                             l11_ocean_strip_vertex_positions,
                             ocean_strip_vertex_colors,
                             l11_ocean_strip_vertex_uvs,
                             1);
    far_quad.z78 = get_effect_texture(0x16);
    far_quad.q80 = 0xFF9000000260L;
    far_quad.q88 = 0x8000000044L;
    far_quad.z70 = 0;
    if (ocean_vars->f14 < l11_camera.pos.f[2] - ocean_vars->f24 && hero.state.current != 0x1D &&
        D_L11_0016D180 == 0) {
        far_quad.uv[0].u = 0.0f;
        far_quad.uv[0].v = 0.0f;
        far_quad.uv[1].u = 0.0f;
        far_quad.uv[1].v = 0.0f;
        far_quad.uv[2].u = 0.0f;
        far_quad.uv[2].v = 0.0f;
        far_quad.uv[3].u = 0.0f;
        far_quad.uv[3].v = 0.0f;
        far_quad.col[0] = ocean_vars->f4;
        far_quad.col[1] = ocean_vars->f8;
        far_quad.col[2] = far_quad.col[0];
        far_quad.col[3] = far_quad.col[1];
        *(u128 *)&far_quad.v[0] = *(u128 *)&strip_corners[1];
        *(u128 *)&far_quad.v[1] = *(u128 *)&strip_corners[1];
        far_quad.v[1].z = l11_camera.pos.f[2] - ocean_vars->f24;
        *(u128 *)&far_quad.v[2] = *(u128 *)&strip_corners[3];
        *(u128 *)&far_quad.v[3] = *(u128 *)&strip_corners[3];
        far_quad.v[3].z = l11_camera.pos.f[2] - ocean_vars->f24;
        draw_geometry_quad(&far_quad, 0, 0);
    }
    vu1_add_g_sregister(0x42, 0x8000000048L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1);
    colA = ocean_vars->fC;
    colB = ocean_vars->f10;
    color_interpolation_scalar = 1.0f - (l11_camera.pos.f[2] - tide_level_macro) / ocean_vars->f2C;
    if (color_interpolation_scalar > 1.0f) {
        color_interpolation_scalar = 1.0f;
    }
    if (color_interpolation_scalar >= 0.0f) {
        colA = fast_tween_color(0, colA, color_interpolation_scalar);
        colB = fast_tween_color(0, colB, color_interpolation_scalar);
        for (layer = 0; layer < 2; layer++) {
            int id = layer + 0x28;
            vu1_add_g_sregister(6, get_effect_texture(ocean_vars->f1C + id));
            for (y = ocean_vars->f18 * -2.0f; y < ocean_vars->f18 * 2.0f; y += ocean_vars->f18) {
                vertex_count = 0;
                for (x = ocean_vars->f18 * -2.0f; x < ocean_vars->f18 * 2.0f;
                     x += ocean_vars->f18) {
                    l11_ocean_wave_grid_vertex_uvs[vertex_count].u =
                        x / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + wave_offset[layer].u;
                    l11_ocean_wave_grid_vertex_uvs[vertex_count].v =
                        y / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + wave_offset[layer].v;
                    l11_ocean_wave_grid_vertex_positions[vertex_count].x =
                        x + l11_camera.pos.f[0] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[vertex_count].y =
                        y + l11_camera.pos.f[1] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[vertex_count].z = tide_level_macro;
                    i = vertex_count + 1;
                    l11_ocean_wave_grid_vertex_uvs[i].u =
                        x / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + wave_offset[layer].u;
                    l11_ocean_wave_grid_vertex_uvs[i].v =
                        (y + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] +
                        wave_offset[layer].v;
                    l11_ocean_wave_grid_vertex_positions[i].x = x + l11_camera.pos.f[0] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].y =
                        y + ocean_vars->f18 + l11_camera.pos.f[1] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].z = tide_level_macro;
                    i = vertex_count + 2;
                    l11_ocean_wave_grid_vertex_uvs[i].u =
                        (x + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] +
                        wave_offset[layer].u;
                    l11_ocean_wave_grid_vertex_uvs[i].v =
                        y / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + wave_offset[layer].v;
                    l11_ocean_wave_grid_vertex_positions[i].x =
                        x + ocean_vars->f18 + l11_camera.pos.f[0] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].y = y + l11_camera.pos.f[1] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].z = tide_level_macro;
                    i = vertex_count + 3;
                    l11_ocean_wave_grid_vertex_uvs[i].u =
                        (x + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] +
                        wave_offset[layer].u;
                    l11_ocean_wave_grid_vertex_uvs[i].v =
                        (y + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] +
                        wave_offset[layer].v;
                    l11_ocean_wave_grid_vertex_positions[i].x =
                        x + ocean_vars->f18 + l11_camera.pos.f[0] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].y =
                        y + ocean_vars->f18 + l11_camera.pos.f[1] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].z = tide_level_macro;
                    for (i = vertex_count; i < vertex_count + 4; i++) {
                        scratch_vector.x =
                            l11_ocean_wave_grid_vertex_positions[i].x - l11_camera.pos.f[0];
                        scratch_vector.y =
                            l11_ocean_wave_grid_vertex_positions[i].y - l11_camera.pos.f[1];
                        scratch_vector.z =
                            l11_ocean_wave_grid_vertex_positions[i].z - l11_camera.pos.f[2];
                        scratch_vector.w = 1.0f;
                        color_interpolation_scalar =
                            vector_length_xy(&scratch_vector) / (ocean_vars->f18 * 2.0f);
                        if (color_interpolation_scalar > 1.0f) {
                            color_interpolation_scalar = 1.0f;
                        }
                        D_L11_001DA208[i] =
                            fast_tween_color(colA, colB, color_interpolation_scalar);
                    }
                    vertex_count += 4;
                    if (vertex_count >= 0x40) {
                        raise_kernel_trap();
                    }
                }
                func_L00_001FDE48_30a908(vertex_count,
                                         l11_ocean_wave_grid_vertex_positions,
                                         D_L11_001DA208,
                                         l11_ocean_wave_grid_vertex_uvs,
                                         1);
            }
        }
    }
    ((Fog *)render_fog_context)->f228 = saved_fog_228;
    ((Fog *)render_fog_context)->f22C = s22C;
    vu1_add_g_sregister(0x47, 0x5360B);
}

/* Ocean update: scrolls the two wave layers with the camera and the time, keeps their offsets in -1..1 and sets the level's tide values. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030C728), where it is exact; names translated to the US level program. */
typedef float V4_30b358[4] __attribute__((aligned(16)));

typedef struct {
    char  pad00[0x14];
    float level; /* 0x14 */
    float scale; /* 0x18 */
    char  pad1C[0x4];
    float limit; /* 0x20 */
} OceanVars;

typedef struct {
    char          pad00[0x20];
    unsigned char state; /* 0x20 */
    char          pad21[0xF];
    unsigned char f30; /* 0x30 */
    char          pad31[0x47];
    OceanVars    *vars; /* 0x78 */
} OceanMoby;

extern float camera_pos_b358[] __asm__("D_L11_001677C0");
extern char  last_camera_pos[4] __asm__("D_L11_00162840")
    __attribute__((sda)); /* a 16-byte vector; declared small so it is reached through $gp */
extern float ocean_level __asm__("D_L11_00161E1C");
extern UV    wave_drift[] __asm__("D_L11_001D9AF0");
extern float frame_delta_time __asm__("D_0015ED7C");

void ocean_pokitaru_update(OceanMoby *moby) __asm__("FUN_L11_0030b358");

void ocean_pokitaru_update(OceanMoby *moby) {
    OceanVars *ocean_vars = moby->vars;
    float      camera_displacement[4];
    float      out[4];
    int        k;

    if (moby->state == 0) {
        moby->state = 1;
        moby->f30 = 0xFF;
        for (k = 0; k < 2; k++) {
            wave_offset[k].u = 0.0f;
            wave_offset[k].v = 0.0f;
        }
        qcopy(last_camera_pos, camera_pos_b358);
        ocean_level = ocean_vars->level;
        return;
    }
    fast_vec_sub(camera_displacement, camera_pos_b358, last_camera_pos);
    qcopy(last_camera_pos, camera_pos_b358);
    if (!(ocean_vars->limit < camera_pos_b358[2])) {
        return;
    }
    for (k = 0; k < 2; k++) {
        fast_vec_scale(
            out, camera_displacement, (wave_speed[k] + wave_speed[k]) / (ocean_vars->scale * 5.0f));
        if (1.0f < out[0] || out[0] < 1.0f) {
            out[0] = out[0] - convert_integer_to_float(truncate_float_to_s32(out[0]));
        }
        if (1.0f < out[1] || out[1] < 1.0f) {
            out[1] = out[1] - convert_integer_to_float(truncate_float_to_s32(out[1]));
        }
        wave_offset[k].u = wave_offset[k].u + out[0];
        wave_offset[k].v = wave_offset[k].v + out[1];
        if (1.0f < wave_offset[k].u) {
            wave_offset[k].u = wave_offset[k].u - 1.0f;
        }
        if (wave_offset[k].u < -1.0f) {
            wave_offset[k].u = wave_offset[k].u + 1.0f;
        }
        if (1.0f < wave_offset[k].v) {
            wave_offset[k].v = wave_offset[k].v - 1.0f;
        }
        if (wave_offset[k].v < -1.0f) {
            wave_offset[k].v = wave_offset[k].v + 1.0f;
        }
    }
    tide_level =
        ocean_level +
        fast_sin(convert_integer_to_float(level_frame % 360) * 0.017444445f - 3.14f) * 0.25f;
    for (k = 0; k < 2; k++) {
        float sum = wave_offset[k].u + wave_drift[k].u * frame_delta_time;
        wave_offset[k].u = sum;
        if (1.0f < sum) {
            wave_offset[k].u = sum - 1.0f;
        }
        if (wave_offset[k].u < -1.0f) {
            wave_offset[k].u = wave_offset[k].u + 1.0f;
        }
        sum = wave_offset[k].v + wave_drift[k].v * frame_delta_time;
        wave_offset[k].v = sum;
        if (1.0f < sum) {
            wave_offset[k].v = sum - 1.0f;
        }
        if (wave_offset[k].v < -1.0f) {
            wave_offset[k].v = wave_offset[k].v + 1.0f;
        }
    }
    add_draw_callback((void *)draw_pokitaru_ocean, moby);
}

f32 D_L16_001D8A28[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A2C[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A30[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A34[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A38[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A3C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D8D28[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D8D2C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D9230[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D9234[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D9238[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D923C[1] __attribute__((section(".data"))) = {0};

int                D_L13_00161D20 = 0x80802000;
int                D_L13_00161D24 = 0x80808020;
int                D_L13_00161D28 = 0xa;
int                D_L13_00161D2C = 0x5208;
float              D_L13_00161D30 = 0.5f;
int D_L16_001D9128 NOT_SDA = 0;
float              D_L16_00161C50[2] = {2.0f, 3.0f};

float D_L16_00161C70 = 0;

char D_L16_00162240[4] __attribute__((section(".bss")));

int   D_L17_00161DB0 = 0x80802000;
int   D_L17_00161DB4 = 0x80808020;
int   D_L17_00161DB8 = 0xa;
int   D_L17_00161DBC = 0x5208;
float D_L17_00161DC0 = 0.5f;

int projectile_trail_color_start_gp __asm__("D_L11_00161DC0") = 0x80802000;

int projectile_trail_frame_count_gp __asm__("D_L11_00161DC8") = 0xa;

float ocean_level __asm__("D_L11_00161E1C") = 0;
