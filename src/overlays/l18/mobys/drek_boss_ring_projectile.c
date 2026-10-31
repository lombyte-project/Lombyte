/* Level 18 moby class drek_boss_ring_projectile (0x270): a ballistic projectile Drek's boss robot fires that expands into a ring on landing, and its spawner. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"

#include "rnc/overlay/l18/mobys/drek_boss_ring_projectile_globals.h"

extern void  add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void  FUN_L00_00257d78(float *, float, float);
extern float FUN_L04_001f1978(float);
extern int   D_L18_0015F5C4;

extern int scale_game_frames(int);
f32        probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");

extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int FUN_001fa6e0(int, int, float);
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DC850), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float);
extern float fast_sin(float);
extern float probe_ground_height(void *, int, float);
extern int trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");
extern short D_L18_00161BFC __attribute__((sda));
extern int   ballistic_ring_glow_color_end_gp __asm__("D_L18_00161C00") __attribute__((sda));
extern float ballistic_ring_trail_spark_gravity_gp __asm__("D_L18_00161C04") __attribute__((sda));
extern float ballistic_ring_trail_movement_step_scale_gp __asm__("D_L18_00161C08")
    __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_ring_projectile_data/update_drek_boss_ring_projectile_4.h"

extern int D_L18_00161C10 __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_ring_projectile_data/update_drek_boss_ring_projectile_3.h"

extern int D_L18_00161C18 __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_ring_projectile_data/update_drek_boss_ring_projectile_2.h"

extern int ballistic_ring_trail_initial_color_start_gp __asm__("D_L18_00161C30")
    __attribute__((sda));
extern int ballistic_ring_trail_second_color_start_gp __asm__("D_L18_00161C38")
    __attribute__((sda));
extern int ballistic_ring_trail_second_color_end_gp __asm__("D_L18_00161C3C") __attribute__((sda));
extern int ballistic_ring_trail_sparks_per_update_gp __asm__("D_L18_00161C40") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_ring_projectile_data/drek_boss_ring_projectile_update.h"

extern void jet_animate(unsigned char *m, float s) __asm__("FUN_L00_00259fe8");
extern void draw_expanding_projectile_ring() __asm__("FUN_L18_002dba20");
void        process_expanding_ring_collisions(struct Moby *moby) __asm__("FUN_L18_002dbe80");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

void drek_boss_ring_projectile_update(struct Moby *moby) __asm__("FUN_L18_002db460");

/* Moby class 624 */
void drek_boss_ring_projectile_update(struct Moby *moby) {
    enum {
        STATE_BALLISTIC_FLIGHT = 0,
        STATE_GROUND_IMPACT_DELAY = 1,
        STATE_EXPANDING_RING = 2,
        STATE_REMOVE = 3
    };
    float  first_spark_vector[4];
    float  second_spark_vector[4];
    float  spark_vector_offset[4];
    float  spark_spawn_position[4];
    float *moby_vars = (float *)moby->pvars;
    int    i;
    int    i0, i1, i2, i3, i4;

    if (hero.state.current == 0x72 || D_L18_0015F5C4 == 2) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case STATE_BALLISTIC_FLIGHT: {
        float *cc;
        i = 0;
        for (; i < ballistic_ring_trail_sparks_per_update_gp; i++) {
            cc = spark_vector_offset;
            fast_vec_scale(
                first_spark_vector, moby_vars, ballistic_ring_trail_movement_step_scale_gp);
            FUN_L00_00257d78(cc, 0.0f, ballistic_ring_trail_vector_offset_rate_gp * frame_time);
            fast_vec_add(first_spark_vector, cc, first_spark_vector);
            FUN_L00_00257d78(spark_spawn_position, 0.0f, 0.5f);
            fast_vec_add(spark_spawn_position, spark_spawn_position, &moby->pos);
            clear_u64_value(second_spark_vector);
            first_spark_vector[3] =
                random_float_between(ballistic_ring_trail_first_spark_w_range_start_gp,
                                     ballistic_ring_trail_first_spark_w_range_end);
            second_spark_vector[3] =
                random_float_between(ballistic_ring_trail_second_spark_w_range_start_gp,
                                     ballistic_ring_trail_second_spark_w_range_end_gp);
            i0 = FUN_001fa6e0(ballistic_ring_trail_initial_color_start_gp,
                              ballistic_ring_trail_initial_color_end,
                              random_float_between(0.0f, 1.0f));
            i1 = FUN_001fa6e0(ballistic_ring_trail_second_color_start_gp,
                              ballistic_ring_trail_second_color_end_gp,
                              random_float_between(0.0f, 1.0f));
            i2 = truncate_float_to_s32(
                FUN_L04_001f1978((float)D_L18_00161C10 * random_float_between(0.0f, 1.0f) + 1.0f));
            i3 = truncate_float_to_s32(FUN_L04_001f1978(
                (float)ballistic_ring_trail_spark_gravity_multiplier_base_gp *
                (random_float_between(-ballistic_ring_trail_spark_relative_variation_limit_gp,
                                      ballistic_ring_trail_spark_relative_variation_limit_gp) +
                 1.0f)));
            i4 = truncate_float_to_s32(FUN_L04_001f1978(
                (float)D_L18_00161C18 *
                (random_float_between(-ballistic_ring_trail_spark_relative_variation_limit_gp,
                                      ballistic_ring_trail_spark_relative_variation_limit_gp) +
                 1.0f)));
            second_spark_vector[2] =
                second_spark_vector[2] -
                ballistic_ring_trail_spark_gravity_gp * frame_time_sq * (float)i3;
            spawn_spark(spark_spawn_position,
                        first_spark_vector,
                        second_spark_vector,
                        i0,
                        i1,
                        i2,
                        i3,
                        i4,
                        ballistic_ring_trail_spark_resource_selector_gp);
        }
        moby_vars[12] = fast_add_rotations(moby_vars[12], frame_time * 18.849556f);
        {
            int   col = FUN_001fa6e0(*(int *)&D_L18_00161BFC,
                                   ballistic_ring_glow_color_end_gp,
                                   (fast_sin(moby_vars[12]) + 1.0f) * 0.5f);
            float k = frame_time_sq * 10.0f;
            moby->glow_color = col;
            moby_vars[2] = moby_vars[2] - k;
        }
        fast_vec_add(&moby->pos, &moby->pos, moby_vars);
        {
            float ground_height = probe_ground_height(&moby->pos, 0, 0.5f);
            if (moby->pos.z - 0.333f < ground_height) {
                moby->state = 1;
                moby->pos.z = ground_height + 0.333f;
                *(int *)(moby_vars + 11) = scale_game_frames(1);
            }
        }
        moby->rot.y = fast_add_rotations(moby->rot.y, frame_time * 8.726646f);
        if (moby->pos.z < 10.0f) {
            mark_moby_for_removal(moby);
        } else {
            jet_animate((unsigned char *)moby, 1.0f);
        }
        break;
    }
    case STATE_GROUND_IMPACT_DELAY:
        moby_vars[12] = fast_add_rotations(moby_vars[12], frame_time * 18.849556f);
        moby->glow_color = FUN_001fa6e0(*(int *)&D_L18_00161BFC,
                                        ballistic_ring_glow_color_end_gp,
                                        (fast_sin(moby_vars[12]) + 1.0f) * 0.5f);
        if (fast_dec_timer((int *)(moby_vars + 11))) {
            trooper_play_sound(0xF, 0, (int)moby, 0x58E);
            moby->state = 2;
            moby_vars[9] = 0.5f;
            moby->collision = 0;
            moby->visible = 0;
            moby->flags |= 1;
        }
        break;
    case STATE_EXPANDING_RING:
        moby_vars[9] = moby_vars[9] + moby_vars[8];
        process_expanding_ring_collisions(moby);
        add_draw_callback(draw_expanding_projectile_ring, moby);
        if (moby_vars[9] > moby_vars[10]) {
            moby->state = 3;
        }
        break;
    case STATE_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}

#include "qcopy.h"

extern void *create_moby() __asm__("FUN_0020c4f8");

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} BallisticRingProjectileMoby;

/* Allocate an object, init its fields and child record. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DCD28), where it is exact; names translated to the US level program. */

extern short D_L18_00161BFC_gp __asm__("D_L18_00161BFC") __attribute__((sda));

extern float D_L18_00161BF8 __attribute__((sda));
void         spawn_drek_boss_ring_projectile(char  *pos,
                                             float *initial_movement_step,
                                             int    collision_filter_moby,
                                             float  fa,
                                             float  fb) __asm__("FUN_L18_002db938");

void spawn_drek_boss_ring_projectile(

    char *pos, float *initial_movement_step, int collision_filter_moby, float fa, float fb) {
    BallisticRingProjectileMoby *spawned_moby = (BallisticRingProjectileMoby *)create_moby(0x270);
    if (spawned_moby != 0) {
        Child *moby_vars;
        int    initial_glow_color;
        spawned_moby->b30 = 0xFF;
        spawned_moby->b31 = 1;
        spawned_moby->b20 = 0;
        initial_glow_color = *(int *)&D_L18_00161BFC_gp;
        spawned_moby->f2C = spawned_moby->f2C * D_L18_00161BF8;
        spawned_moby->s32 = 0xFF;
        spawned_moby->h34 |= 0x10;
        spawned_moby->f90 = initial_glow_color;
        moby_vars = spawned_moby->child;
        qcopy(spawned_moby->pos, pos);
        spawned_moby->f48 = fast_arc_tan(initial_movement_step[0], initial_movement_step[1]);
        qcopy(moby_vars->v, initial_movement_step);
        moby_vars->f34 = collision_filter_moby;
        moby_vars->f28 = fa;
        moby_vars->f20 = fb;
        moby_vars->f30 = 0;
    }
}

/* Ported from rac1-decomp nonmatching/l18_veldin2/func_L18_002DCE10.c (func_L18_002DCE10) */
typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_dba20;

typedef struct {
    float u, v;
} UV_dba20;

typedef struct {
    V_dba20      corner[4];
    unsigned int color[4];
    UV_dba20     uv[4];
    long         unk70, tex, unk80, unk88;
} Q_dba20;

typedef struct {
    char  pad0[0x20];
    float f20, f24, f28;
} D_dba20;

typedef struct {
    char     pad0[0x10];
    V_dba20  pos;
    char     pad20[0x58];
    D_dba20 *data;
} M_dba20;

extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern int   tween_color_dba20(int, int, float) __asm__("FUN_001fa6e0");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  draw_quad_dba20(void *, void *, int) __asm__("FUN_001f7d30");

/* One (u, v) pair for each of the 4 quad corners. */
float expanding_projectile_ring_quad_uvs[8] __asm__("D_L18_001D4310") = {
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};

#include "rnc/overlay/l18/mobys/drek_boss_ring_projectile_data/draw_expanding_projectile_ring_2.h"

extern int expanding_projectile_ring_color_start_gp __asm__("D_L18_00161C5C") __attribute__((sda));
extern int expanding_projectile_ring_color_end_gp __asm__("D_L18_00161C60") __attribute__((sda));
extern float expanding_projectile_ring_segment_angle_degrees_gp __asm__("D_L18_00161C64")
    __attribute__((sda));
extern float expanding_projectile_ring_draw_height_offset_gp __asm__("D_L18_00161C68")
    __attribute__((sda));
extern float D_L18_00161C6C __attribute__((sda));
extern float expanding_projectile_ring_layer_radius_offset_gp __asm__("D_L18_00161C70")
    __attribute__((sda));
extern float D_L18_00161C74 __attribute__((sda));
extern float D_L18_00161C78 __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_ring_projectile_data/draw_expanding_projectile_ring.h"

void draw_expanding_projectile_ring(M_dba20 *moby) {
    Q_dba20  quad_packets[4];
    V_dba20  draw_matrix[4];
    V_dba20  segment_rotation_basis[3];
    V_dba20  rv;
    D_dba20 *moby_vars = moby->data;
    int      j, k, ring_segment_index;
    int      col;

    set_identity_matrix(draw_matrix);
    qcopy(&draw_matrix[3], &moby->pos);
    draw_matrix[3].f[2] += expanding_projectile_ring_draw_height_offset_gp;
    draw_matrix[3].f[3] = 1.0f;
    for (j = 0; j < 4; j++) {
        float fj;
        float layer_phase, color_fade_weight, rad;
        quad_packets[j].tex = get_effect_texture(0xE);
        quad_packets[j].unk70 = 0;
        quad_packets[j].unk80 = 0xFF9000000260L;
        quad_packets[j].unk88 =
            (long)expanding_projectile_ring_blend_minuend_selector_gp |
            ((long)expanding_projectile_ring_blend_subtrahend_selector_gp << 2) |
            ((long)expanding_projectile_ring_blend_alpha_factor_selector_gp << 4) |
            ((long)expanding_projectile_ring_blend_addend_selector_gp << 6) | 0x8000000000L;
        layer_phase = moby_vars->f24 * D_L18_00161C74 + (float)j * 0.25f;
        rad = moby_vars->f24 + expanding_projectile_ring_layer_radius_offset_gp * (float)j;
        layer_phase = layer_phase - (float)truncate_float_to_s32(layer_phase);
        col = tween_color_dba20(expanding_projectile_ring_color_start_gp,
                                expanding_projectile_ring_color_end_gp,
                                layer_phase);
        color_fade_weight = layer_phase * D_L18_00161C78;
        if (color_fade_weight > 1.0f)
            color_fade_weight = 1.0f;
        col = tween_color_dba20(col & 0xFFFFFF, col, color_fade_weight);
        color_fade_weight =
            (moby_vars->f28 - moby_vars->f24) /
            (moby_vars->f20 * scale_time(expanding_projectile_ring_fade_duration_input_gp));
        if (color_fade_weight > 1.0f) {
            color_fade_weight = 1.0f;
        } else if (color_fade_weight < 0.0f) {
            color_fade_weight = 0.0f;
        }
        col = tween_color_dba20(col & 0xFFFFFF, col, color_fade_weight);
        fj = (float)j;
        for (k = 0; k < 4; k++) {
            float corner_angle;
            quad_packets[j].uv[k].u = expanding_projectile_ring_quad_uvs[2 * k];
            quad_packets[j].uv[k].v = expanding_projectile_ring_quad_uvs[2 * k + 1];
            if (k & 1) {
                quad_packets[j].corner[k].f[2] = layer_phase * D_L18_00161C6C;
            } else {
                quad_packets[j].corner[k].f[2] = -layer_phase * D_L18_00161C6C;
            }
            corner_angle = fast_add_rotations(
                (float)(k >> 1) *
                        (expanding_projectile_ring_segment_angle_degrees_gp * 0.017453292f) -
                    3.14159f,
                expanding_projectile_ring_segment_angle_degrees_gp * 0.25f * fj * 0.017453292f);
            quad_packets[j].corner[k].f[0] = fast_cos(corner_angle) * rad;
            quad_packets[j].corner[k].f[1] = fast_sin(corner_angle) * rad;
            quad_packets[j].corner[k].f[3] = 1.0f;
            quad_packets[j].color[k] = col;
        }
    }
    clear_u64_value(&rv);
    rv.f[2] = expanding_projectile_ring_segment_angle_degrees_gp * 0.017453292f;
    euler_rotation_basis(segment_rotation_basis, &rv);
    for (ring_segment_index = 0;
         (float)ring_segment_index < 360.0f / expanding_projectile_ring_segment_angle_degrees_gp;
         ring_segment_index++) {
        draw_quad_dba20(&quad_packets[0], draw_matrix, 0);
        draw_quad_dba20(&quad_packets[1], draw_matrix, 0);
        draw_quad_dba20(&quad_packets[2], draw_matrix, 0);
        draw_quad_dba20(&quad_packets[3], draw_matrix, 0);
        multiply_matrix3x3(draw_matrix, draw_matrix, segment_rotation_basis);
    }
}

extern void
make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD270), where it is exact; names translated to the US level program. */

extern void FUN_L18_002dc0c8_u(char *moby, void *a1, void *a2) __asm__("FUN_L18_002dc0c8");

void process_expanding_ring_collisions(struct Moby *moby) {
    char  hit[0x30];
    float hit_direction_vector[4];
    float segment_start[4];
    float segment_end[4];
    char *moby_vars = (char *)moby->pvars;
    float segment_start_angle, segment_end_angle, step;
    int   i;

    fast_vec_sub(hit_direction_vector, &hero.motion.pos, &moby->pos);
    hit_direction_vector[2] = 0.0f;
    fast_vec_normalize(hit_direction_vector, hit_direction_vector, frame_time * 4.0f);
    hit_direction_vector[2] = frame_time * 4.0f;
    make_hit_descriptor(hit, (int)moby, 0x10001, hit_direction_vector, 3.0f);
    step = 1.0f / convert_integer_to_float(0x40) * 6.28318f;
    segment_start_angle = -3.14159f;
    segment_end_angle = fast_add_rotations(segment_start_angle, step);
    for (i = 0x3F; i >= 0; i--) {
        segment_start[0] = fast_cos(segment_start_angle) * *(float *)(moby_vars + 0x24);
        segment_start[1] = fast_sin(segment_start_angle) * *(float *)(moby_vars + 0x24);
        segment_start[2] = 0.0f;
        segment_end[0] = fast_cos(segment_end_angle) * *(float *)(moby_vars + 0x24);
        segment_end[1] = fast_sin(segment_end_angle) * *(float *)(moby_vars + 0x24);
        segment_end[2] = 0.0f;
        fast_vec_add(segment_start, segment_start, &moby->pos);
        segment_start[2] += 0.5f;
        fast_vec_add(segment_end, segment_end, &moby->pos);
        segment_end[2] += 0.5f;
        collision_line(segment_start, segment_end, 1, *(void **)(moby_vars + 0x34), hit);
        if (*(float *)(moby_vars + 0x24) < 3.0f) {
            FUN_L18_002dc0c8_u(moby, segment_start, segment_end);
        }
        segment_start_angle = fast_add_rotations(segment_start_angle, step);
        segment_end_angle = fast_add_rotations(segment_end_angle, step);
    }
}

extern int random_integer_below(int) __asm__("FUN_00213260");
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD4B8), where it is exact; names translated to the US level program. */

extern s32  random_integer_below(s32) __asm__("FUN_00213260");
extern void spawn_moving_hero_relative_alpha_fade_particle(
    void *, void *, int, int, int, int, float) __asm__("FUN_L00_002730e0");

extern float ring_segment_particle_cull_depth_limit_gp __asm__("D_L18_00161BF4")
    __attribute__((sda));
extern int   ring_segment_particle_color_start_gp __asm__("D_L18_00161C80") __attribute__((sda));
extern int   ring_segment_particle_color_end_gp __asm__("D_L18_00161C84") __attribute__((sda));
extern float ring_segment_particle_scale_gp __asm__("D_L18_00161C88") __attribute__((sda));
extern int   ring_segment_radial_particle_lifetime_frames_gp __asm__("D_L18_00161C8C")
    __attribute__((sda));
extern int ring_segment_horizontal_particle_lifetime_frames_gp __asm__("D_L18_00161C90")
    __attribute__((sda));
extern float ring_segment_particle_spawn_probability_gp __asm__("D_L18_00161C98")
    __attribute__((sda));
void spawn_expanding_ring_segment_particles(struct Moby *moby,
                                            void        *segment_start,
                                            void        *segment_end) __asm__("FUN_L18_002dc0c8");

void spawn_expanding_ring_segment_particles(struct Moby *moby,
                                            void        *segment_start,

                                            void *segment_end) {
    float segment_bounding_sphere[4];
    float particle_spawn_position[4];
    float particle_velocity[4];
    char *moby_vars = (char *)moby->pvars;
    int   col;

    lerp_vector(segment_bounding_sphere, segment_start, segment_end, 0.5f);
    segment_bounding_sphere[3] = fast_vec_dist(segment_start, segment_end) * 0.5f;
    if (fast_bsphere_check((char *)segment_bounding_sphere,
                           ring_segment_particle_cull_depth_limit_gp) == -1) {
        return;
    }
    if (random_float_between(0.0f, 1.0f) < ring_segment_particle_spawn_probability_gp) {
        lerp_vector(
            particle_spawn_position, segment_start, segment_end, random_float_between(0.0f, 1.0f));
        fast_vec_sub(particle_velocity, particle_spawn_position, &moby->pos);
        fast_vec_normalize(particle_velocity, particle_velocity, *(float *)(moby_vars + 0x20));
        col = FUN_001fa6e0(ring_segment_particle_color_start_gp,
                           ring_segment_particle_color_end_gp,
                           random_float_between(0.0f, 1.0f));
        spawn_moving_hero_relative_alpha_fade_particle(
            particle_spawn_position,
            particle_velocity,
            col,
            scale_game_frames(ring_segment_radial_particle_lifetime_frames_gp) & 0xFF,
            random_integer_below(0xFF) & 0xFF,
            0,
            ring_segment_particle_scale_gp);
        particle_velocity[2] = 0.0f;
        spawn_moving_hero_relative_alpha_fade_particle(
            particle_spawn_position,
            particle_velocity,
            col,
            scale_game_frames(ring_segment_horizontal_particle_lifetime_frames_gp) & 0xFF,
            random_integer_below(0xFF) & 0xFF,
            0,
            ring_segment_particle_scale_gp);
    }
}

short D_L18_00161BFC_gp __asm__("D_L18_00161BFC") = -16192;
