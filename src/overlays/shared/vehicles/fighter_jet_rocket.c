/* Fighter jet rocket (class 0x127, Wrench name rocket): the rocket the fighter jet fires, its spawner and its update. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/random.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"

extern char *create_moby(int);
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B8FC0.c: func_L13_002E7E90), where it is exact; names translated to the US level program. */

extern char *spawn_source_attached_fading_effect_particle(char *, int, int, int, float) __asm__("FUN_L00_0026daa0");

unsigned char *spawn_fighter_jet_rocket(struct Moby *firing_moby,
                                        char        *spawn_position,
                                        char        *target,
                                        char        *initial_rotation,
                                        int          lifetime_frames,
                                        float        scale) __asm__("FUN_L13_002e6a58");

unsigned char *spawn_fighter_jet_rocket(struct Moby *firing_moby,
                                        char        *spawn_position,
                                        char        *target,
                                        char        *initial_rotation,
                                        int          lifetime_frames,

                                        float scale) {
    unsigned char *moby = create_moby(0x127);
    if (moby != 0) {
        char *data = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(char **)(data + 0x20) = firing_moby;
        *(char **)(data + 0x24) = target;
        qcopy(moby + 0x10, spawn_position);
        qcopy(moby + 0x40, initial_rotation);
        if (target != 0) {
            char *target_flag_data;
            *(int *)(data + 0x3C) = *(short *)(target + 0xA6);
            target_flag_data = suck_get_aim_height(target);
            if (target_flag_data != 0) {
                *(unsigned short *)(target_flag_data + 0x1E) |= 0x80;
            }
        }
        *(int *)(data + 0x28) = lifetime_frames;
        *(float *)(data + 0x2C) = scale;
        *(int *)(data + 0x30) = 0;
        *(int *)(data + 0x34) = 0;
        build_spherical_offset(
            data, scale, *(float *)(initial_rotation + 8), -*(float *)(initial_rotation + 4));
        if (moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        fast_vec_add(moby + 0x10, moby + 0x10, data);
        if (*(char **)(data + 0x24) != 0) {
            qcopy(data + 0x10, *(char **)(data + 0x24) + 0x10);
        }
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
                            (firing_moby->scale / *(float *)(((char *)firing_moby->pclass) + 0x24));
        update_entity(moby);
        spawn_source_attached_fading_effect_particle(
            (char *)moby, 0x60808080, lifetime_frames, 0, 500000.0f);
    }
    return moby;
}

/* Update for the rocket (class 295): smoke trail, steering toward its target, collision and expiry. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B8FC0.c: func_L13_002E8040), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int Q_2E8040 __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V4_2E8040;

typedef struct Cls {
    char  pad0[0x24];
    float scale;
} Cls;

typedef struct Rocket {
    V4_2E8040 vel;
    V4_2E8040 tpos;
    Moby     *owner;
    Moby     *target;
    int       timer;
    float     speed;
    float     yawv;
    float     pitchv;
    float     f38;
    int       target_uid;
} Rocket;

typedef struct Tex {
    char           pad0[0x5C];
    unsigned char *p5C;
} Tex;

extern float D_0015ED6C;
extern int   collision_line() __asm__("FUN_001efa68");
extern int   FUN_L00_0024f7c8();
float FUN_L00_0025be00(float *cur, float *vel, float target, float lim, float accel, float maxv);
unsigned char        *spawn_particle(void  *pos,
                                     float *vec,
                                     int    s,
                                     int    a,
                                     int    col,
                                     int    n,
                                     float  x,
                                     float  y,
                                     float  z,
                                     float  w,
                                     float  pw) __asm__("FUN_L00_002712b8");
void                  FUN_L00_0025e450(f32   burst,
                                       f32   unused_float,
                                       f32   owner_effect,
                                       f32   owner_effect2,
                                       f32   cutoff,
                                       f32   scale,
                                       f32   flash,
                                       void *owner,
                                       s32   unused2,
                                       void *pos,
                                       s32   puff_count,
                                       s32   spark_count,
                                       s32   shard_count,
                                       s32   sound,
                                       s32   shake,
                                       s32   spray_count,
                                       s32   tint,
                                       u8    alternate);
extern unsigned char *spawn_particle(void  *pos,
                                     float *vec,
                                     int    s,
                                     int    a,
                                     int    col,
                                     int    n,
                                     float  x,
                                     float  y,
                                     float  z,
                                     float  w,
                                     float  pw) __asm__("FUN_L00_002712b8");
extern unsigned char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");
extern float
           approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern int collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void      spawn_explosion(void *,
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
extern Tex       D_L13_001B2380 __asm__("D_L13_001B2380");
extern V4_2E8040 D_L13_001741E0_2E8040 __asm__("D_L13_00174160");

void update_fighter_jet_rocket(Moby *m) __asm__("FUN_L13_002e6c08");

void update_fighter_jet_rocket(Moby *rocket) {
    Rocket        *rocket_data = *(void **)&rocket->pvars;
    V4_2E8040      old;
    V4_2E8040      hitpos;
    V4_2E8040      effect_scratch_vector;
    V4_2E8040      particle_velocity;
    V4_2E8040      trail_direction;
    unsigned char *spawned_effect;
    float          trail_segment_fraction;

    qcopy(&old, &*(V4_2E8040 *)&rocket->pos);
    rocket->scale += (((Cls *)rocket->pclass)->scale - rocket->scale) * 0.1f;
    *(Q_2E8040 *)&trail_direction = 0;
    trail_direction.x = random_float_between(-1.0f, 1.0f);
    trail_direction.y = random_float_between(-1.0f, 1.0f);
    trail_direction.z = random_float_between(-1.0f, 1.0f);
    *(Q_2E8040 *)&particle_velocity = *(Q_2E8040 *)&trail_direction;
    fast_vec_normalize(
        &particle_velocity, &particle_velocity, random_float_between(0.1f, 0.2f) * D_0015ED6C);
    fast_vec_normalize(
        &effect_scratch_vector, rocket_data, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
    fast_vec_add(&particle_velocity, &particle_velocity, &effect_scratch_vector);
    spawned_effect = spawn_particle(&*(V4_2E8040 *)&rocket->pos,
                                    (float *)&particle_velocity,
                                    scale_game_frames(6),
                                    0x7F,
                                    0xB0B0B0,
                                    3,
                                    40000.0f,
                                    1000.0f,
                                    1.0f,
                                    -0.0004f,
                                    0.0f);
    if (spawned_effect != 0) {
        spawned_effect[3] = 0x48;
    }
    for (trail_segment_fraction = 0.0f; trail_segment_fraction < 1.0f;
         trail_segment_fraction += 0.33333334f) {
        fast_vec_normalize(
            &effect_scratch_vector,
            rocket_data,
            random_float_between(trail_segment_fraction, trail_segment_fraction + 0.33333334f) *
                rocket_data->speed);
        fast_vec_add(&effect_scratch_vector, &effect_scratch_vector, &*(V4_2E8040 *)&rocket->pos);
        spawned_effect = spawn_particle(&effect_scratch_vector,
                                        (float *)&particle_velocity,
                                        scale_game_frames(0x3C),
                                        0x7F,
                                        0x606060,
                                        3,
                                        40000.0f,
                                        1000.0f,
                                        1.0f,
                                        -0.0002f,
                                        0.0f);
        if (spawned_effect != 0) {
            Tex *trail_particle_attribute_source = &D_L13_001B2380;
            spawned_effect[2] = *trail_particle_attribute_source->p5C;
            spawned_effect[3] = 0x48;
        }
        *(Q_2E8040 *)&trail_direction = 0;
        trail_direction.z = 0.02f;
        transform_vector_by_basis(
            &trail_direction, &trail_direction, &*(V4_2E8040 *)&rocket->rot_row0);
        build_look_at_matrix(&trail_direction,
                             &trail_direction,
                             &*(V4_2E8040 *)&rocket->rot_row0,
                             random_angle_radians());
        spawned_effect = spawn_burst(&effect_scratch_vector,
                                     &trail_direction,
                                     0x4F007FFF,
                                     0x1FFFFFFF,
                                     scale_game_frames(5),
                                     1,
                                     20000.0f);
        if (spawned_effect != 0) {
            spawned_effect[3] = 0x48;
        }
    }
    if (rocket_data->target != 0 && !(rocket_data->target->flags & 0x1000) &&
        (rocket_data->target->flags & 1) && *(int *)&rocket_data->target->collision == 0) {
        rocket_data->target = 0;
    }
    if (rocket_data->target != 0 && rocket_data->target->oclass == rocket_data->target_uid &&
        rocket_data->target->state != 0xFE && rocket_data->target->state != 0xFD &&
        fast_vec_dist(&rocket_data->tpos, &*(V4_2E8040 *)&rocket_data->target->pos) < 3.0f) {
        rocket->rot.z = fast_arc_tan(rocket_data->target->pos.x - rocket->pos.x,
                                     rocket_data->target->pos.y - rocket->pos.y);
        rocket->rot.y = -fast_arc_tan(vector_distance_xy(&*(V4_2E8040 *)&rocket->pos,
                                                         &*(V4_2E8040 *)&rocket_data->target->pos),
                                      rocket_data->target->pos.z - rocket->pos.z);
        qcopy(&rocket_data->tpos, &*(V4_2E8040 *)&rocket_data->target->pos);
    } else {
        approach_angle(&rocket->rot.z,
                       rocket->rot.z,
                       &rocket_data->yawv,
                       frame_time_sq * 251.32741f,
                       frame_time_sq * 251.32741f,
                       D_0015ED6C * 2513.2742f);
        approach_angle(&rocket->rot.y,
                       rocket->rot.y,
                       &rocket_data->pitchv,
                       frame_time_sq * 251.32741f,
                       frame_time_sq * 251.32741f,
                       D_0015ED6C * 2513.2742f);
        rocket_data->target = 0;
    }
    build_spherical_offset(rocket_data, rocket_data->speed, rocket->rot.z, -rocket->rot.y);
    fast_vec_add(&*(V4_2E8040 *)&rocket->pos, &*(V4_2E8040 *)&rocket->pos, rocket_data);
    if (rocket->pos.x < 2.0f || rocket->pos.x > 1021.0f || rocket->pos.y < 2.0f ||
        rocket->pos.y > 1021.0f || rocket->pos.z < 2.0f || rocket->pos.z > 1021.0f) {
        mark_moby_for_removal(rocket);
        return;
    }
    moby_point_position(rocket, 1, &hitpos);
    if (collision_line(&old, &hitpos, 0, rocket_data->owner, 0) != 0) {
        qcopy(&effect_scratch_vector, &D_L13_001741E0_2E8040);
        spawn_explosion(rocket,
                        rocket_data,
                        &effect_scratch_vector,
                        2.0f,
                        10.0f,
                        10,
                        3,
                        0x10,
                        4.0f,
                        2.0f,
                        9.0f,
                        1.0f,
                        0,
                        20.0f,
                        0,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(rocket);
        return;
    }
    if (fast_dec_timer(&rocket_data->timer) != 0) {
        mark_moby_for_removal(rocket);
    }
}

Tex D_L17_001B2980 = {0};
