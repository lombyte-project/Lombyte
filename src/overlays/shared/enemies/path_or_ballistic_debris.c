/* Path or ballistic debris (class 0x193): a piece that follows a race path until its timer ends, then flies and falls; its spawner and trail particles. */
#include "types.h"
#include "rnc/overlay/shared/enemies/path_or_ballistic_debris.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/enemies/trooper_play_sound.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/random/random_spherical_offset.h"
#include "rnc/overlay/shared/rendering/effects/particles.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"

extern float D_0015ED60;
extern s32   coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern int   sample_path(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

extern void emit_debris_trail_particles(struct Moby *) __asm__("FUN_L14_002e03b8");
extern s32  D_001413D0_dfd10 __asm__("D_001413D0") NOT_SDA;
extern void spawn_explosion_dfd10(struct Moby *,
                                  void *,
                                  void *,
                                  f32,
                                  f32,
                                  s32,
                                  s32,
                                  s32,
                                  f32,
                                  f32,
                                  f32,
                                  f32,
                                  s32,
                                  f32,
                                  s32,
                                  s32,
                                  s32,
                                  s32) __asm__("FUN_L00_0025e450");

typedef struct {
    struct Moby *owner;
    s32          timer;
    f32          path_speed;
    s32          path;
    f32          path_t;
    f32          path_u;
    f32          offset_x;
    f32          offset_y;
    f32          spin_x;
    f32          spin_y;
    f32          spin_z;
    u8           pad2C[4];
    f32          velocity[3];
} Data;

extern f32 ballistic_debris_gravity_gp __asm__("D_L14_00161B40") __attribute__((sda));
extern f32 ballistic_debris_velocity_base_scale_gp __asm__("D_L14_00161B44") __attribute__((sda));
extern f32 ballistic_debris_planar_damping_speed_threshold_gp __asm__("D_L14_00161B48")
    __attribute__((sda));
/* A piece of debris: follows its race path until its timer runs out, then flies, spins and falls until it hits something or leaves the map. */
void path_or_ballistic_debris_update(struct Moby *moby) __asm__("FUN_L14_002dfd10");

/* Moby class 403 */
void path_or_ballistic_debris_update(struct Moby *moby) {
    Data *debris_private_vars = (Data *)moby->pvars;
    Vec4  probe;
    f32   position_offset[4];
    f32  *velocity;
    void *points;
    f32   speed;

    if (moby->state == 0) {
        points = l14_splines[debris_private_vars->path].path;
        qcopy(&probe, &moby->pos);
        sample_path(points,
                    &moby->pos,
                    &debris_private_vars->path_t,
                    &debris_private_vars->path_u,
                    0,
                    debris_private_vars->path_speed);
        fast_vec_sub(position_offset, &moby->pos, &probe);
        debris_private_vars->offset_x = position_offset[0];
        debris_private_vars->offset_y = position_offset[1];
        emit_debris_trail_particles(moby);
        if (fast_dec_timer(&debris_private_vars->timer) == 0)
            return;
        fast_vec_normalize(position_offset, position_offset, 0.5f);
        qcopy(&probe, &moby->pos);
        probe.f[2] += 0.25f;
        trooper_play_sound(1, 0, moby, debris_private_vars->owner->oclass);
        spawn_explosion_dfd10(moby,
                              position_offset,
                              &probe,
                              0.0f,
                              0.0f,
                              5,
                              2,
                              4,
                              1.0f,
                              0.5f,
                              9.0f,
                              1.0f,
                              -1,
                              15.0f,
                              0,
                              3,
                              -1,
                              0);
        coll_sphere(1.0f, &moby->pos, 0, (void *)D_001413D0_dfd10);
        mark_moby_for_removal(moby);
        return;
    }
    velocity = debris_private_vars->velocity;
    fast_vec_add(&moby->pos, &moby->pos, velocity);
    speed = absolute_float(debris_private_vars->velocity[0]);
    if (ballistic_debris_planar_damping_speed_threshold_gp * frame_scale < speed)
        debris_private_vars->velocity[0] *=
            (ballistic_debris_velocity_base_scale_gp - 1.0f) * frame_scale + 1.0f;
    speed = absolute_float(debris_private_vars->velocity[1]);
    if (ballistic_debris_planar_damping_speed_threshold_gp * frame_scale < speed)
        debris_private_vars->velocity[1] *=
            (ballistic_debris_velocity_base_scale_gp - 1.0f) * frame_scale + 1.0f;
    if (debris_private_vars->velocity[2] > 0.0f)
        debris_private_vars->velocity[2] *=
            (ballistic_debris_velocity_base_scale_gp - 1.0f) * frame_scale + 1.0f;
    debris_private_vars->velocity[2] -= ballistic_debris_gravity_gp * frame_scale_sq;
    moby->rot.x = fast_add_rotations(moby->rot.x, debris_private_vars->spin_x);
    moby->rot.y = fast_add_rotations(moby->rot.y, debris_private_vars->spin_y);
    moby->rot.z = fast_add_rotations(moby->rot.z, debris_private_vars->spin_z);
    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f ||
        1021.0f < moby->pos.y || moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    fast_vec_normalize(position_offset, &moby->rot_row2, 0.52f);
    fast_vec_add(&probe, &moby->pos, position_offset);
    if (fast_dec_timer(&debris_private_vars->timer) != 0 ||
        coll_sphere(0.5f, &probe, 0, debris_private_vars->owner) != 0) {
        trooper_play_sound(0, 0, moby, debris_private_vars->owner->oclass);
        spawn_explosion_dfd10(moby,
                              velocity,
                              &moby->pos,
                              0.0f,
                              0.0f,
                              5,
                              2,
                              4,
                              1.0f,
                              0.5f,
                              9.0f,
                              1.0f,
                              -1,
                              15.0f,
                              0,
                              3,
                              -1,
                              0);
        coll_sphere(1.0f, &moby->pos, 0, (void *)D_001413D0_dfd10);
        mark_moby_for_removal(moby);
        return;
    }
    if (random_integer_below(2) != 0)
        emit_debris_trail_particles(moby);
}

extern void create_effect_2e0170(
    void *, void *, void *, void *, void *, int, float, float, float) __asm__("FUN_L00_0025df68");

extern float ballistic_debris_launch_magnitude_range_start_gp __asm__("D_L14_00161B30")
    __attribute__((sda));
extern float ballistic_debris_launch_magnitude_range_end_gp __asm__("D_L14_00161B34")
    __attribute__((sda));
extern float ballistic_debris_z_velocity_range_start_gp __asm__("D_L14_00161B38")
    __attribute__((sda));
extern float ballistic_debris_z_velocity_range_end_gp __asm__("D_L14_00161B3C")
    __attribute__((sda));

unsigned char *
spawn_path_or_ballistic_debris(char *owner, float *scale, int mode) __asm__("FUN_L14_002e0170");

unsigned char *spawn_path_or_ballistic_debris(char *debris_origin_moby, float *scale, int mode) {
    unsigned char *spawned_moby = create_moby(0x193);
    unsigned char *pos;
    if (spawned_moby != 0) {
        char *spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        float launch_vector_scratch[8];
        pos = spawned_moby + 0x10;
        qcopy(pos, debris_origin_moby + 0x10);
        MOBY(spawned_moby)->rot.z = *(float *)(debris_origin_moby + 0x48);
        spawned_moby[0x20] = mode;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        *(char **)spawned_vars = debris_origin_moby;
        if (spawned_moby[0x20] == 0) {
            int          index;
            LevelSpline *splines;
            int         *fields = (int *)spawned_vars;
            fields[1] = scale_game_frames(90);
            splines = l14_splines;
            *(float *)(spawned_vars + 8) = *scale * frame_time;
            index = *(int *)(*(char **)(debris_origin_moby + 0x78) + 0x60);
            fields[5] = 0;
            fields[3] = index;
            fields[4] = 0;
            create_effect_2e0170(splines[index].path,
                                 pos,
                                 launch_vector_scratch,
                                 fields + 4,
                                 fields + 5,
                                 0,
                                 20.0f,
                                 5.0f,
                                 *(float *)(spawned_vars + 0x14));
        } else {
            float angle;
            *(int *)(spawned_vars + 4) = scale_game_frames(200);
            fast_vec_sub(launch_vector_scratch, pos, &hero.motion.pos);
            launch_vector_scratch[2] = 0.0f;
            fast_vec_normalize(
                launch_vector_scratch,
                launch_vector_scratch,
                random_float_between(ballistic_debris_launch_magnitude_range_start_gp * D_0015ED60,
                                     ballistic_debris_launch_magnitude_range_end_gp * D_0015ED60));
            fast_vec_cross(launch_vector_scratch + 4, &hero.gravity_dir, launch_vector_scratch);
            angle = randf_sym(0.5759586691856384f, 1.3962633609771729f);
            build_look_at_matrix(
                spawned_vars + 0x30, launch_vector_scratch, &hero.gravity_dir, angle);
            *(float *)(spawned_vars + 0x38) =
                random_float_between(ballistic_debris_z_velocity_range_start_gp * D_0015ED60,
                                     ballistic_debris_z_velocity_range_end_gp * D_0015ED60);
            *(float *)(spawned_vars + 0x20) = randf_sym(0.0f, 0.008726646f);
            *(float *)(spawned_vars + 0x24) = randf_sym(0.05235988f, 0.13962634f);
            *(float *)(spawned_vars + 0x28) = randf_sym(0.0f, 0.017453293f);
        }
        update_entity(spawned_moby);
    }
    return spawned_moby;
}
/* Ported from rac1-decomp src/overlays/shared/vendor_002B2A28.c (func_L14_002E17B8) */
/* Exhaust puffs from the moby's nozzle: two coloured sparks and three white puffs of shrinking size. */

typedef struct Moby Moby;

typedef int u128_2E17B8 __attribute__((mode(TI)));

typedef union {
    u128_2E17B8 q;
    float       f[4];
} Vec4_2E17B8;

typedef struct Vars {
    char  pad0[0x18];
    float f18; /* 0x18 */
    float f1C; /* 0x1C */
} Vars;

typedef struct ParticleFade_2E17B8 {
    char          pad0[4];
    int           i4; /* 0x04 */
    char          pad8[2];
    unsigned char bA; /* 0x0A */
    unsigned char bB; /* 0x0B */
    char          padC[4];
    float         scale; /* 0x10 */
    char          pad14[2];
    short         mode;  /* 0x16 */
    int           color; /* 0x18 */
} ParticleFade_2E17B8;

typedef struct Particle_2E17B8 {
    char                pad0[3];
    unsigned char       b3; /* 0x03 */
    char                pad4[6];
    short               frames; /* 0x0A */
    float               life;   /* 0x0C */
    char                pad10[0x10];
    ParticleFade_2E17B8 fade; /* 0x20 */
} Particle_2E17B8;

char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");
extern int   D_L14_0015F580_c __asm__("D_L14_0015F580");
int          FUN_L00_00257b90(int lo, int hi);
extern float func_002140F8_2E17B8(float, float) __asm__("random_float_between");
extern int   func_002140B0_2E17B8(int) __asm__("random_integer_below");
extern void
func_L00_0026DA50_2E17B8(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern Particle_2E17B8 *func_L00_0026DEA0_2E17B8(
    void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

void emit_debris_trail_particles(Moby *moby) {
    Vec4_2E17B8          pos;
    Vec4_2E17B8          emission_axis_offset;
    Vec4_2E17B8          particle_initial_velocity;
    Vec4_2E17B8          emission_position_offset;
    Vars                *debris_motion_state;
    Particle_2E17B8     *particle;
    Particle_2E17B8     *spawned_particle;
    ParticleFade_2E17B8 *fade;
    int                  i;
    int                  signed_particle_variation;
    int                  color_intensity;
    int                  alpha;
    float                size;

    debris_motion_state = *(Vars **)&moby->pvars;
    fast_vec_normalize(&emission_axis_offset, &*(Vec4_2E17B8 *)&moby->rot_row2, 0.2f);
    fast_vec_add(&pos, &*(Vec4_2E17B8 *)&moby->pos, &emission_axis_offset);
    random_spherical_offset(&particle_initial_velocity, 0.005f, 0.03f);
    particle = spawn_mode_controlled_rotating_alpha_fade_particle(
        &pos, &particle_initial_velocity, 0x7F, moby);
    if (particle != 0) {
        particle->life = func_002140F8_2E17B8(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        float f;
        int   color;
        particle =
            spawn_mode_controlled_rotating_alpha_fade_particle(&pos, &D_L14_0015F580_c, 0x7F, moby);
        if (particle == 0) {
            continue;
        }
        fade = &particle->fade;
        if (i == 2 && func_002140B0_2E17B8(8) == 0) {
            particle->life = 180000.0f;
        } else {
            particle->life = func_002140F8_2E17B8(80000.0f, 120000.0f);
        }
        particle->frames = scale_game_frames(2);
        f = 1.0f / convert_integer_to_float(particle->frames);
        color = 0x7F7F7F;
        fade->mode = 4;
        fade->color = color;
        fade->scale = f;
    }
    particle_initial_velocity.f[0] = debris_motion_state->f18;
    particle_initial_velocity.f[1] = debris_motion_state->f1C;
    particle_initial_velocity.f[2] = 0.0f;
    particle_initial_velocity.f[3] = 1.0f;
    if (moby->state == 0) {
        fast_vec_normalize(&emission_axis_offset, &*(Vec4_2E17B8 *)&moby->rot_row2, 0.95f);
    } else {
        fast_vec_normalize(&emission_axis_offset, &*(Vec4_2E17B8 *)&moby->rot_row2, 1.15f);
        particle_initial_velocity.f[2] = D_0015ED60 * 0.03f;
    }
    fast_vec_add(&pos, &*(Vec4_2E17B8 *)&moby->pos, &emission_axis_offset);
    emission_position_offset.f[0] = fast_cos(moby->rot.z) * 0.3f;
    emission_position_offset.f[1] = fast_sin(moby->rot.z) * 0.3f;
    emission_position_offset.f[2] = 0.0f;
    fast_vec_add(&pos, &pos, &emission_position_offset);
    if (moby->state == 1) {
        pos.f[0] += randf_sym(0.0f, 0.1f);
        pos.f[1] += randf_sym(0.0f, 0.1f);
    }
    func_L00_0026DA50_2E17B8(&pos,
                             &particle_initial_velocity,
                             0x4F007FFF,
                             0x1FFFFFFF,
                             random_int_between(scale_game_frames(10), scale_game_frames(20)),
                             1,
                             60000.0f);
    signed_particle_variation = func_002140B0_2E17B8(6);
    if (func_002140B0_2E17B8(2) != 0) {
        signed_particle_variation = -signed_particle_variation;
    }
    size = func_002140F8_2E17B8(60000.0f, 140000.0f);
    color_intensity = random_int_between(0x30, 0xFF);
    alpha = 0x60000000;
    spawned_particle = func_L00_0026DEA0_2E17B8(
        &pos,
        0.025f,
        1.0f,
        1.0f,
        signed_particle_variation,
        &particle_initial_velocity,
        size,
        color_intensity | ((color_intensity << 16) | ((color_intensity << 8) | alpha)));
    if (spawned_particle != 0) {
        ParticleFade_2E17B8 *ef;
        ef = &spawned_particle->fade;
        spawned_particle->b3 = 0x44;
        spawned_particle->frames = scale_game_frames(0x3C);
        ef->i4 = 2;
        ef->bA = 0x60;
        ef->bB = spawned_particle->frames;
    }
}

LevelSpline *l15_splines __asm__("D_L15_0015F70C") = 0;
f32          D_L15_00161AA8 = 0.003f;
f32          D_L15_00161AAC = 0.97f;
f32          D_L15_00161AB0 = 0.05f;
float        D_L15_00161A98 = 0.075f;
float        D_L15_00161A9C = 0.2f;
float        D_L15_00161AA0 = 0.2f;
float        D_L15_00161AA4 = 0.2f;
