/* Level 18 class 0x3d7: the electric ball Drek's boss robot fires, travelling to a target with bolts and exploding. */
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

extern void add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");

#include "qcopy.h"

extern void FUN_L00_00257d78(float *, float, float);

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
f32        probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");

extern float fast_add_rotations(float, float);
extern float probe_ground_height(void *, int, float);
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB260), where it is exact; names translated to the US level program. */

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern void  draw_electric_water_bolt_effect(char *) __asm__("FUN_L15_002d8710");
extern void  FUN_L18_002ea1f8_u(struct Moby *) __asm__("FUN_L18_002ea1f8");
void         spawn_explosion_burst(void *arg) __asm__("FUN_L18_002ea800");
void initialize_electric_bolt_effect_if_requested(struct Moby *moby) __asm__("FUN_L18_002eacd8");

void initialize_electric_bolt_effect_if_requested(struct Moby *m) __asm__("FUN_L18_002eacd8");
extern void update_electric_bolt_geometry_and_particles(void *) __asm__("FUN_L18_002eaea0");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
void        FUN_L18_002ea598_c(char *moby) __asm__("FUN_L18_002ea598");
extern void
spark_s(int, void *, float, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/update_moby_target_travel_and_electric_effects.h"

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_globals.h"

extern float electric_arc_endpoint_radius_scale_gp __asm__("D_L18_00161ED8") __attribute__((sda));
extern float target_travel_speed_gp __asm__("D_L18_00161EE0") __attribute__((sda));
void update_moby_target_travel_and_electric_effects(struct Moby *moby) __asm__("FUN_L18_002e9e70");

void update_moby_target_travel_and_electric_effects(struct Moby *moby) {
    enum {
        STATE_TIMED_REMOVAL_WAIT = 0,
        STATE_TRAVEL_TO_TARGET = 1
    };
    float *moby_vars = (float *)moby->pvars;
    float  updated_rotation_angle = fast_add_rotations(
        moby_vars[0xB], target_effect_first_rotation_rate_degrees_gp * DEG_TO_RAD * frame_time);
    float second_rotation_angle_step =
        target_effect_second_rotation_rate_degrees_gp * DEG_TO_RAD * frame_time;
    moby_vars[0xB] = updated_rotation_angle;
    moby_vars[0xC] = fast_add_rotations(moby_vars[0xC], second_rotation_angle_step);
    switch (MOBY(moby)->state) {
    case STATE_TIMED_REMOVAL_WAIT:
        if (fast_dec_timer((int *)(moby_vars + 10))) {
            mark_moby_for_removal_c2(moby);
            return;
        }
        break;
    case STATE_TRAVEL_TO_TARGET: {
        float  target_displacement[4];
        float  ground_position[4];
        float *position = &moby->pos.x;
        float *arc_endpoint_position = moby_vars + 4;
        float  movement_step_length = target_travel_speed_gp * frame_time;
        float  target_distance;
        fast_vec_sub(target_displacement, moby_vars, position);
        target_distance = fast_vec_length(target_displacement);
        FUN_L18_002ea598_c(moby);
        FUN_L00_00257d78(arc_endpoint_position,
                         electric_arc_endpoint_radius_scale_gp * moby_vars[9],
                         electric_arc_endpoint_radius_scale_gp * moby_vars[9]);
        fast_vec_add(arc_endpoint_position, arc_endpoint_position, position);
        initialize_electric_bolt_effect_if_requested(moby);
        update_electric_bolt_geometry_and_particles(moby);
        add_draw_callback((void (*)(void))draw_electric_water_bolt_effect, moby);
        if (target_distance < movement_step_length) {
            spawn_explosion_burst(moby);
            mark_moby_for_removal_c2(moby);
            return;
        }
        fast_vec_normalize(target_displacement, target_displacement, movement_step_length);
        fast_vec_add(position, position, target_displacement);
        qcopy(ground_position, position);
        ground_position[2] = probe_ground_height(ground_position, 0, 0.5f);
        spark_s(*(int *)(moby_vars + 8), position, 2.0f, 0x10001, 5.0f, 1.0f, 0, 1, 0);
        spark_s(*(int *)(moby_vars + 8), ground_position, 2.0f, 0x10001, 5.0f, 1.0f, 0, 1, 0);
        if (fast_dec_timer((int *)(moby_vars + 13))) {
            approach_value(moby_vars + 9, 0.0f, 0.2f);
            if (moby_vars[9] == 0.0f) {
                mark_moby_for_removal_c2(moby);
                return;
            }
        }
        break;
    }
    }
    add_draw_callback(FUN_L18_002ea1f8_u, moby);
}

extern void *create_moby() __asm__("FUN_0020c4f8");
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB4E0), where it is exact; names translated to the US level program. */

char *
spawn_drek_boss_electric_ball(void *owner, void *vector, int value) __asm__("FUN_L18_002ea0f0");

char *spawn_drek_boss_electric_ball(void *owner, void *vector, int value) {
    char *moby = create_moby(0x3D7);
    if (moby != 0) {
        char *data;
        ((unsigned char *)moby)[0x30] = 0xFF;
        qcopy(moby + 0x10, vector);
        data = ((char *)MOBY(moby)->pvars);
        *(void **)(data + 0x20) = owner;
        *(int *)(data + 0x28) = 5;
        *(int *)(data + 0x34) = value;
        *(int *)(data + 0x24) = 0;
    }
    return moby;
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB558), where it is exact; names translated to the US level program. */

void refresh_moby_position_and_activate(struct Moby *moby,
                                        void        *moby_position,
                                        void        *position,
                                        int          active,
                                        float        speed) __asm__("FUN_L18_002ea168");

void refresh_moby_position_and_activate(

    struct Moby *moby, void *moby_position, void *position, int active, float speed) {
    char *data = (char *)moby->pvars;
    qcopy(&moby->pos, moby_position);
    *(int *)(data + 0x28) = scale_game_frames(5);
    *(float *)(data + 0x24) = speed;
    if (active && moby->state == 0) {
        moby->state = 1;
        *(int *)(data + 0x28) = 0;
        qcopy(data, position);
    }
}

#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB5E8), where it is exact; names translated to the US level program. */

typedef struct {
    float vtx[4][4];
    int   color[4];
    float uv[4][2];
    long  a;
    long  b;
    long  c;
    long  d;
} Pkt;

extern LevelCamera l18_camera_lui __asm__("D_L18_00167680") NOT_SDA;
extern float       D_L18_001D9B50[][2];
extern float       D_L18_001D9B70[][4][4];
extern s32         fast_tween_color_c(s32, s32, f32) __asm__("func_001FA6E0");
extern s64         get_effect_texture_c(s32) __asm__("func_001F44B8");
extern int         D_L18_00161EE4 __attribute__((sda));
extern int         D_L18_00161EE8 __attribute__((sda));
extern int         D_L18_00161EEC __attribute__((sda));
extern int         D_L18_00161EF0 __attribute__((sda));
extern int         D_L18_00161EF8 __attribute__((sda));
extern int         D_L18_00161F08 __attribute__((sda));
extern void        fast_draw_quad_real(void *, void *, int) __asm__("FUN_001f7d30");

void homing_spark_draw(char *moby) __asm__("FUN_L18_002ea1f8");

void homing_spark_draw(char *moby) {
    Pkt   pkt[4];
    float rot[4];
    float m[4][4][4];
    char *data = ((char *)MOBY(moby)->pvars);

    clear_u64_value(rot);
    rot[2] = fast_arc_tan(l18_camera_lui.pos.f[0] - MOBY(moby)->pos.x,
                          l18_camera_lui.pos.f[1] - MOBY(moby)->pos.y);
    rot[1] = -fast_arc_tan(vector_distance_xy(moby + 0x10, &l18_camera_lui.pos.f[0]),
                           l18_camera_lui.pos.f[2] - MOBY(moby)->pos.z);
    euler_rotation_matrix(m[0][0], rot);
    euler_rotation_matrix(m[1][0], rot);
    fast_vec_scale(m[1][1], m[1][1], *(float *)(data + 0x24));
    fast_vec_scale(m[1][2], m[1][2], *(float *)(data + 0x24));
    qcopy(m[0][3], moby + 0x10);
    qcopy(m[1][3], moby + 0x10);
    rot[0] = *(float *)(data + 0x2C);
    euler_rotation_matrix(m[2][0], rot);
    fast_vec_scale(m[2][1], m[2][1], *(float *)(data + 0x24));
    fast_vec_scale(m[2][2], m[2][2], *(float *)(data + 0x24));
    qcopy(m[2][3], moby + 0x10);
    rot[0] = *(float *)(data + 0x30);
    euler_rotation_matrix(m[3][0], rot);
    fast_vec_scale(m[3][1], m[3][1], *(float *)(data + 0x24));
    fast_vec_scale(m[3][2], m[3][2], *(float *)(data + 0x24));
    qcopy(m[3][3], moby + 0x10);
    {
        int  i;
        int  j;
        Pkt *p;
        for (i = 0; i < 4; i++) {
            {
                long r =
                    ((long (*)(int))get_effect_texture_c)(((int *)&(*(short *)&D_L18_00161F08))[i]);
                p = &pkt[i];
                p->b = r;
            }
            p->c = 0xFF9000000260;
            p->d = (D_L18_00161EE4) | (long)(D_L18_00161EE8) << 2 | (long)(D_L18_00161EEC) << 4 |
                   (long)(D_L18_00161EF0) << 6 | 0x8000L << 24;
            p->a = 0;
            for (j = 0; j < 4; j++) {
                pkt[i].uv[j][0] = D_L18_001D9B50[j][0];
                pkt[i].uv[j][1] = D_L18_001D9B50[j][1];
                if (i == 0) {
                    pkt[0].color[j] = fast_tween_color_c(
                        D_L18_00161EF8 & 0xFFFFFF, D_L18_00161EF8, *(float *)(data + 0x24));
                } else {
                    pkt[i].color[j] = ((int *)&(*(short *)&D_L18_00161EF8))[i];
                }
                qcopy(pkt[i].vtx[j], D_L18_001D9B70[i][j]);
            }
        }
    }
    fast_draw_quad_real(&pkt[0], m[0], 0);
    fast_draw_quad_real(&pkt[1], m[1], 0);
    fast_draw_quad_real(&pkt[2], m[2], 0);
    fast_draw_quad_real(&pkt[3], m[3], 0);
}

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float FUN_L04_001f1978(float);

extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int FUN_001fa6e0(int, int, float);
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB988), where it is exact; names translated to the US level program. */

extern short target_direction_spark_initial_color_range_start_gp __asm__("D_L18_00161F1C")
    __attribute__((sda));
extern short target_direction_spark_initial_color_range_end_gp __asm__("D_L18_00161F20")
    __attribute__((sda));
extern short target_direction_spark_second_color_range_start_gp __asm__("D_L18_00161F24")
    __attribute__((sda));
extern short target_direction_spark_second_color_range_end_gp __asm__("D_L18_00161F28")
    __attribute__((sda));
extern short target_direction_spark_first_vector_w_range_start_gp __asm__("D_L18_00161F2C")
    __attribute__((sda));
extern short target_direction_spark_first_vector_w_range_end_gp __asm__("D_L18_00161F30")
    __attribute__((sda));
extern short target_direction_spark_second_vector_w_range_start_gp __asm__("D_L18_00161F34")
    __attribute__((sda));
extern short target_direction_spark_second_vector_w_range_end_gp __asm__("D_L18_00161F38")
    __attribute__((sda));
extern short target_direction_spark_frame_count_range_start_gp __asm__("D_L18_00161F3C")
    __attribute__((sda));
extern short target_direction_spark_frame_count_range_end_gp __asm__("D_L18_00161F40")
    __attribute__((sda));
extern short target_direction_spark_second_frame_count_range_start_gp __asm__("D_L18_00161F44")
    __attribute__((sda));
extern short target_direction_spark_second_frame_count_range_end_gp __asm__("D_L18_00161F48")
    __attribute__((sda));
extern short target_direction_spark_third_frame_count_range_start_gp __asm__("D_L18_00161F4C")
    __attribute__((sda));
extern short target_direction_spark_third_frame_count_range_end_gp __asm__("D_L18_00161F50")
    __attribute__((sda));
extern short target_direction_spark_speed_range_start_gp __asm__("D_L18_00161F54")
    __attribute__((sda));
extern short target_direction_spark_speed_range_end_gp __asm__("D_L18_00161F58")
    __attribute__((sda));
extern short target_direction_spark_second_vector_scale_gp __asm__("D_L18_00161F5C")
    __attribute__((sda));
extern short target_direction_spark_downward_z_rate_range_start_gp __asm__("D_L18_00161F60")
    __attribute__((sda));
extern short target_direction_spark_downward_z_rate_range_end_gp __asm__("D_L18_00161F64")
    __attribute__((sda));
extern short target_direction_spark_lookup_index_and_flags_gp __asm__("D_L18_00161F68")
    __attribute__((sda));
void spawn_sparks_along_target_direction(struct Moby *moby) __asm__("FUN_L18_002ea598");

void spawn_sparks_along_target_direction(struct Moby *moby) {
    float spark_spawn_position[4];
    float first_spark_vector[4];
    float second_spark_vector[4];
    float scaled_target_direction[4];
    int   i;
    int   initial_spark_color, second_spark_color, spark_frame_count, second_spark_frame_count,
        third_spark_frame_count;
    float speed_min, speed_max, sink_min, sink_max;
    fast_vec_sub(scaled_target_direction, (void *)moby->pvars, &moby->pos);
    fast_vec_normalize(scaled_target_direction, scaled_target_direction, frame_time * 60.0f);
    speed_min = (*(float *)&(target_direction_spark_speed_range_start_gp)) * frame_time;
    speed_max = (*(float *)&(target_direction_spark_speed_range_end_gp)) * frame_time;
    sink_min = (*(float *)&(target_direction_spark_downward_z_rate_range_start_gp)) * frame_time;
    sink_max = (*(float *)&(target_direction_spark_downward_z_rate_range_end_gp)) * frame_time;
    for (i = 0; i < 3; i++) {
        FUN_L00_00257d78(first_spark_vector, speed_min, speed_max);
        fast_vec_scale(second_spark_vector,
                       first_spark_vector,
                       (*(float *)&(target_direction_spark_second_vector_scale_gp)));
        second_spark_vector[2] -= random_float_between(sink_min, sink_max);
        spark_frame_count = truncate_float_to_s32(FUN_L04_001f1978(
            random_float_between((*(float *)&(target_direction_spark_frame_count_range_start_gp)),
                                 (*(float *)&(target_direction_spark_frame_count_range_end_gp)))));
        second_spark_frame_count = truncate_float_to_s32(FUN_L04_001f1978(random_float_between(
            (*(float *)&(target_direction_spark_second_frame_count_range_start_gp)),
            (*(float *)&(target_direction_spark_second_frame_count_range_end_gp)))));
        third_spark_frame_count = truncate_float_to_s32(FUN_L04_001f1978(random_float_between(
            (*(float *)&(target_direction_spark_third_frame_count_range_start_gp)),
            (*(float *)&(target_direction_spark_third_frame_count_range_end_gp)))));
        first_spark_vector[3] = random_float_between(
            (*(float *)&(target_direction_spark_first_vector_w_range_start_gp)),
            (*(float *)&(target_direction_spark_first_vector_w_range_end_gp)));
        second_spark_vector[3] = random_float_between(
            (*(float *)&(target_direction_spark_second_vector_w_range_start_gp)),
            (*(float *)&(target_direction_spark_second_vector_w_range_end_gp)));
        fast_vec_scale(
            spark_spawn_position, scaled_target_direction, random_float_between(0.0f, 1.0f));
        fast_vec_add(spark_spawn_position, spark_spawn_position, &moby->pos);
        initial_spark_color =
            FUN_001fa6e0((*(int *)&(target_direction_spark_initial_color_range_start_gp)),
                         (*(int *)&(target_direction_spark_initial_color_range_end_gp)),
                         random_float_between(0.0f, 1.0f));
        second_spark_color =
            FUN_001fa6e0((*(int *)&(target_direction_spark_second_color_range_start_gp)),
                         (*(int *)&(target_direction_spark_second_color_range_end_gp)),
                         random_float_between(0.0f, 1.0f));
        spawn_spark(spark_spawn_position,
                    first_spark_vector,
                    second_spark_vector,
                    initial_spark_color,
                    second_spark_color,
                    spark_frame_count,
                    second_spark_frame_count,
                    third_spark_frame_count,
                    (*(int *)&(target_direction_spark_lookup_index_and_flags_gp)));
    }
}

extern int scale_game_frames(int);

extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int   random_integer_below(int) __asm__("FUN_00213260");
extern float fast_sin(float);
extern int trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");

extern s32  random_integer_below(s32) __asm__("FUN_00213260");
extern void spawn_particle(void *, void *, int, int, float, int, int, float, int, int) __asm__("FUN_L00_0026a9f0");
extern int D_L18_0015F580;

extern void  spawn_timed_projected_sprite_effect(void *, int, int, int) __asm__("FUN_L06_0030bef8");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern char *spawn_glow_particle(void *,
                                 void *,
                                 void *,
                                 float,
                                 int,
                                 unsigned char,
                                 unsigned char,
                                 unsigned char,
                                 unsigned char) __asm__("FUN_L00_002ac910");
extern int   explosion_burst_spark_count_gp __asm__("D_L18_00161F6C") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/spawn_explosion_burst_5.h"

extern int explosion_burst_spark_frame_count_gp __asm__("D_L18_00161F80") __attribute__((sda));
extern int explosion_burst_spark_second_frame_count_gp __asm__("D_L18_00161F84")
    __attribute__((sda));
extern int explosion_burst_spark_third_frame_count_gp __asm__("D_L18_00161F88")
    __attribute__((sda));
extern int explosion_burst_particle_count_gp __asm__("D_L18_00161F98") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/spawn_explosion_burst_4.h"

extern int explosion_burst_camera_shake_duration_frames_gp __asm__("D_L18_00161FE8")
    __attribute__((sda));
extern float explosion_burst_spark_speed_min_gp __asm__("D_L18_00161F70") __attribute__((sda));
extern float explosion_burst_spark_speed_max_gp __asm__("D_L18_00161F74") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/spawn_explosion_burst_3.h"

extern float explosion_burst_spark_second_vector_scale_gp __asm__("D_L18_00161F94")
    __attribute__((sda));
extern float explosion_burst_particle_speed_min_gp __asm__("D_L18_00161FA0") __attribute__((sda));
extern float explosion_burst_particle_speed_max_gp __asm__("D_L18_00161FA4") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/spawn_explosion_burst_2.h"

extern float explosion_burst_particle_xy_offset_limit_gp __asm__("D_L18_00161FBC")
    __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/spawn_explosion_burst.h"

extern float explosion_burst_camera_shake_amplitude_gp __asm__("D_L18_00161FE4")
    __attribute__((sda));
extern int           explosion_burst_particle_first_color_choices[] __asm__("D_L18_001D9C70");
extern int           explosion_burst_particle_second_color_choices[] __asm__("D_L18_001D9C88");
extern unsigned char l18_explosion_burst_first_glow_color_channel __asm__("D_L18_00161FC8")
    __attribute__((sda));
extern unsigned char l18_explosion_burst_second_glow_color_channel __asm__("D_L18_00161FD4")
    __attribute__((sda));
extern unsigned char l18_explosion_burst_third_glow_color_channel __asm__("D_L18_00161FE0")
    __attribute__((sda));
extern unsigned char D_L18_00161FC9[3] MACRO_ADDR;
extern unsigned char D_L18_00161FD5[3] MACRO_ADDR;
extern unsigned char D_L18_00161FE1[3] MACRO_ADDR;

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EBBF0). */
void spawn_explosion_burst(void *source_moby) {
    float        velocity[4] __attribute__((aligned(16)));
    float        offset[4] __attribute__((aligned(16)));
    struct Moby *moby = source_moby;
    int          i = 0;
    int          j;
    LevelCamera *camera;
    *(u128 *)velocity = *(u128 *)&moby->pos;
    spawn_timed_projected_sprite_effect(velocity, scale_game_frames(30), 0x80806060, 0x801010);
    trooper_play_sound(0x10, 0, (int)source_moby, 0x58E);
    if (i < explosion_burst_spark_count_gp) {
        int   a, b, third_spark_frame_count;
        float angle, elevation, scale, speed;
        do {
            ++i;
            elevation = random_float_between(-0.34906584f, 0.34906584f);
            angle = random_angle_radians();
            speed = random_float_between(explosion_burst_spark_speed_min_gp,
                                         explosion_burst_spark_speed_max_gp);
            build_spherical_offset(velocity, speed * frame_time, angle, elevation);
            fast_vec_scale(offset, velocity, explosion_burst_spark_second_vector_scale_gp);
            scale = random_float_between(0.8f, 1.2f);
            offset[3] = velocity[3] = explosion_burst_spark_vector_w_gp;
            offset[2] += explosion_burst_spark_second_vector_z_rate_gp * scale * frame_time;
            scale = random_float_between(0.8f, 1.2f);
            a = truncate_float_to_s32(
                (float)scale_game_frames(explosion_burst_spark_frame_count_gp) * scale);
            b = truncate_float_to_s32(
                (float)scale_game_frames(explosion_burst_spark_second_frame_count_gp) * scale);
            third_spark_frame_count = truncate_float_to_s32(
                (float)scale_game_frames(explosion_burst_spark_third_frame_count_gp) * scale);
            spawn_spark(&moby->pos,
                        velocity,
                        offset,
                        explosion_burst_spark_initial_color_gp,
                        explosion_burst_spark_second_color_gp,
                        a,
                        b,
                        third_spark_frame_count,
                        *(int *)&target_direction_spark_lookup_index_and_flags_gp);
        } while (i < explosion_burst_spark_count_gp);
    }
    j = 0;
    if (explosion_burst_particle_count_gp > 0) {
        int   a, b, color1, color2;
        float offset_x, offset_y;
        float angle, elevation, scale, speed;
        do {
            ++j;
            speed = random_float_between(explosion_burst_particle_speed_min_gp,
                                         explosion_burst_particle_speed_max_gp) *
                    frame_time;
            elevation = random_float_between(1.0471976f, 1.3962634f);
            angle = random_angle_radians();
            build_spherical_offset(velocity, speed, angle, elevation);
            angle = random_angle_radians();
            offset_x = fast_cos(angle);
            offset[0] = random_float_between(0.0f, explosion_burst_particle_xy_offset_limit_gp);
            offset_x *= offset[0];
            offset[0] = offset_x;
            offset_y = fast_sin(angle);
            offset[1] = random_float_between(0.0f, explosion_burst_particle_xy_offset_limit_gp);
            offset_y *= offset[1];
            offset[1] = offset_y;
            offset[2] = 0.0f;
            fast_vec_add(offset, offset, &moby->pos);
            color1 = explosion_burst_particle_first_color_choices[random_integer_below(6)];
            color2 = explosion_burst_particle_second_color_choices[random_integer_below(6)];
            scale = explosion_burst_particle_scale_multiplier_gp * 400000.0f;
            a = scale_game_frames(
                random_int_between(explosion_burst_particle_frame_count_range_start_gp,
                                   explosion_burst_particle_frame_count_range_end_gp));
            b = scale_game_frames(
                random_int_between(explosion_burst_particle_second_frame_count_range_start_gp,
                                   explosion_burst_particle_second_frame_count_range_end_gp));
            spawn_particle(offset, velocity, color1, color2, scale, a, b, speed, 0, 0);
        } while (j < explosion_burst_particle_count_gp);
    }
    if (explosion_burst_first_glow_scale_multiplier_gp > 0.0f) {
        void *pos = &moby->pos;
        int   duration = scale_game_frames(explosion_burst_first_glow_duration_frames_gp);
        spawn_glow_particle(source_moby,
                            pos,
                            &D_L18_0015F580,
                            explosion_burst_first_glow_scale_multiplier_gp,
                            duration,
                            l18_explosion_burst_first_glow_color_channel,
                            D_L18_00161FC9[0],
                            D_L18_00161FC9[1],
                            D_L18_00161FC9[2]);
    }
    if (explosion_burst_second_glow_scale_multiplier_gp > 0.0f) {
        void *pos = &moby->pos;
        int   duration = scale_game_frames(explosion_burst_second_glow_duration_frames_gp);
        spawn_glow_particle(source_moby,
                            pos,
                            &D_L18_0015F580,
                            explosion_burst_second_glow_scale_multiplier_gp,
                            duration,
                            l18_explosion_burst_second_glow_color_channel,
                            D_L18_00161FD5[0],
                            D_L18_00161FD5[1],
                            D_L18_00161FD5[2]);
    }
    if (explosion_burst_third_glow_scale_multiplier_gp > 0.0f) {
        void *pos = &moby->pos;
        int   duration = scale_game_frames(explosion_burst_third_glow_duration_frames_gp);
        spawn_glow_particle(source_moby,
                            pos,
                            &D_L18_0015F580,
                            explosion_burst_third_glow_scale_multiplier_gp,
                            duration,
                            l18_explosion_burst_third_glow_color_channel,
                            D_L18_00161FE1[0],
                            D_L18_00161FE1[1],
                            D_L18_00161FE1[2]);
    }
    if (explosion_burst_camera_shake_amplitude_gp > 0.0f) {
        camera = &l18_camera_lui;
        camera->shake_amount = explosion_burst_camera_shake_amplitude_gp;
        camera->shake_frames = scale_game_frames(explosion_burst_camera_shake_duration_frames_gp);
    }
}

/* Starts the moby's trail when requested: resets the trail points and the counters. */
extern int   D_L18_00162008_gp __asm__("D_L18_00162008") __attribute__((sda));
extern int   D_L18_00162010_gp __asm__("D_L18_00162010") __attribute__((sda));
extern int   D_L18_00162024_gp __asm__("D_L18_00162024") __attribute__((sda));
extern int   D_L18_00162054_lui __asm__("D_L18_00162054");
extern short D_L18_001620B8_b[4] __asm__("D_L18_001620B8");
extern short electric_bolt_side_arc_reset_frames[4] __asm__("D_L18_001620C8");
extern short electric_bolt_side_arc_timer_distance_references[4] __asm__("D_L18_001620D0");
extern float D_L18_001D9CF0_q[][4] __asm__("D_L18_001D9CF0");
extern float D_L18_001D9D00_q[][4] __asm__("D_L18_001D9D00");
extern int   D_L18_001D9F80_q[] __asm__("D_L18_001D9F80");
extern float D_L18_001D9FD0_q[][4] __asm__("D_L18_001D9FD0");
extern int   D_L18_001DA110_q[] __asm__("D_L18_001DA110");
extern void  norm_q(void *, void *, float) __asm__("FUN_001f9bf8");

extern int electric_bolt_initialization_requested_gp __asm__("D_L18_00161FEC") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/initialize_electric_bolt_effect_if_requested_2.h"
#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/initialize_electric_bolt_effect_if_requested.h"

extern int D_L18_001620DC;

void initialize_electric_bolt_effect_if_requested(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float arc_endpoint_displacement[4];
    float step[4];
    int   i;
    int   j;
    if (electric_bolt_initialization_requested_gp != 0) {
        electric_bolt_initialization_requested_gp = 0;
        D_L18_001620DC = 0;
        electric_bolt_scaled_initial_frame_count =
            scale_game_frames(electric_bolt_initial_frame_count_gp);
        D_L18_00162054_lui = 0x7F2020;
        fast_vec_sub(arc_endpoint_displacement, moby_vars + 0x10, &moby->pos);
        norm_q(step, arc_endpoint_displacement, electric_bolt_initial_length_scale_gp / 20.0f);
        qcopy(D_L18_001D9D00_q, &moby->pos);
        for (i = 1; i < 20; i++) {
            fast_vec_add(D_L18_001D9D00_q[i], D_L18_001D9CF0_q[i], step);
            D_L18_001D9F80_q[i] = 0;
            clear_u64_value(D_L18_001D9FD0_q[i]);
            D_L18_001DA110_q[i] = 0;
        }
        for (j = 0; j < 4; j++)
            D_L18_001620B8_b[j] = -1;
        electric_bolt_side_arc_reset_frames[0] = 8;
        electric_bolt_side_arc_timer_distance_references[0] = 4;
        electric_bolt_side_arc_reset_frames[1] = 4;
        electric_bolt_side_arc_timer_distance_references[1] = 2;
        electric_bolt_side_arc_timer_distance_references[2] = 4;
        electric_bolt_side_arc_reset_frames[2] = 8;
        electric_bolt_side_arc_reset_frames[3] = 4;
        electric_bolt_side_arc_timer_distance_references[3] = 2;
        D_L18_00162008_gp = 0;
        D_L18_00162010_gp = 0;
        D_L18_00162024_gp = 0;
    }
}

extern char  D_0013E533[];
extern float FUN_L00_00257c48(float, float);
/* Veldin 2 arc emitter: rebuilds the 20-point bolt and its mirror, regrows or jitters four side arcs, then spawns three particles. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EC290), where it is exact; names translated to the US level program. */

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_ec290;

typedef struct {
    V_ec290 p[5];
} P_ec290;

typedef struct {
    V_ec290 hdr;
    V_ec290 p[4];
} H_ec290;

extern H_ec290 l18_electric_bolt_side_arc_predecessor_view[] __asm__("D_L18_001DA150");
extern P_ec290 l18_electric_bolt_side_arc_points[] __asm__("D_L18_001DA160");
extern V_ec290 D_L18_001D9CF0[];
extern V_ec290 D_L18_001D9D00[2][20];
extern V_ec290 D_L18_001D9FD0[];
extern char    D_0013F5E0[];
extern float   D_L18_001677C0_c[] __asm__("D_L18_001677C0");
extern float   D_L18_001D9F80[];
extern float   D_L18_001DA110[];
extern float   FUN_L00_00257c48(float lo, float hi);
extern float   normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int     D_L18_0015F580; /* no foreign declaration */
extern float   primary_bolt_z_phase_step_gp __asm__("D_L18_00161FFC") __attribute__((sda));
extern float   primary_bolt_z_amplitude_start_gp __asm__("D_L18_00162000") __attribute__((sda));
extern float   primary_bolt_z_amplitude_end_gp __asm__("D_L18_00162004") __attribute__((sda));
extern s32     D_L18_00162008 __attribute__((sda));
extern float   primary_bolt_z_phase_increment_gp __asm__("D_L18_0016200C") __attribute__((sda));
extern s32     D_L18_00162010 __attribute__((sda));
extern float secondary_bolt_orbit_phase_decrement_gp __asm__("D_L18_00162014") __attribute__((sda));
extern float secondary_bolt_orbit_phase_step_gp __asm__("D_L18_00162018") __attribute__((sda));
extern float secondary_bolt_z_amplitude_gp __asm__("D_L18_0016201C") __attribute__((sda));
extern float secondary_bolt_lateral_amplitude_gp __asm__("D_L18_00162020") __attribute__((sda));
extern s32   D_L18_00162024 __attribute__((sda));
extern float secondary_bolt_z_phase_increment_gp __asm__("D_L18_00162028") __attribute__((sda));
extern float secondary_bolt_z_phase_step_gp __asm__("D_L18_0016202C") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/update_electric_bolt_geometry_and_particles_2.h"

extern s32 D_L18_00162054_gp __asm__("D_L18_00162054") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_boss_electric_ball_data/update_electric_bolt_geometry_and_particles.h"

extern float primary_bolt_random_step_blend_weight_gp __asm__("D_L18_0016208C")
    __attribute__((sda));
extern short D_L18_001620B8[] MACRO_ADDR;
extern short D_L18_001620C0[] MACRO_ADDR;
extern short electric_bolt_side_arc_reset_frames_macro[] __asm__("D_L18_001620C8") MACRO_ADDR;
extern short
    electric_bolt_side_arc_timer_distance_references_macro[] __asm__("D_L18_001620D0") MACRO_ADDR;
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void
spawn_ec290(void *, int, float, float, int, int, int, void *, float) __asm__("FUN_L00_00272060");

extern f32 frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;

void update_electric_bolt_geometry_and_particles(void *moby) {
    V_ec290       scratch_vectors[10];
    V_ec290       side_arc_segment_offsets[5];
    unsigned char flag = *(unsigned char *)D_0013E533;
    char         *data = *(char **)((char *)moby + 0x78);
    float         scale;
    int           i;
    int           k;
    float         rotation_angle_or_particle_scale;
    int           r;

    *(float *)&D_L18_00162008 =
        fast_add_rotations(*(float *)&D_L18_00162008, *(float *)&primary_bolt_z_phase_increment_gp);
    *(float *)&D_L18_00162010 = fast_subtract_rotations(
        *(float *)&D_L18_00162010, *(float *)&secondary_bolt_orbit_phase_decrement_gp);
    *(float *)&D_L18_00162024 = fast_add_rotations(*(float *)&D_L18_00162024,
                                                   *(float *)&secondary_bolt_z_phase_increment_gp);
    scale = fast_vec_dist((char *)moby + 0x10, data + 0x10) * 0.05f;
    if (flag) {
        *(int *)&D_L18_00162054_gp = 0x7F2040;
    } else {
        *(int *)&D_L18_00162054_gp = 0x7F2020;
    }
    fast_vec_sub(&scratch_vectors[0], data + 0x10, (char *)moby + 0x10);
    fast_vec_normalize(&scratch_vectors[0], &scratch_vectors[0], 1.0f);
    fast_vec_cross(&scratch_vectors[1], &scratch_vectors[0], D_0013F5E0);
    build_look_at_matrix(
        &scratch_vectors[0], &scratch_vectors[0], &scratch_vectors[1], 0.17453292f);
    scale_vector_xyz_c(&scratch_vectors[3], &scratch_vectors[0], scale);
    qcopy(D_L18_001D9D00[0], (char *)moby + 0x10);
    qcopy(D_L18_001D9D00[1], (char *)moby + 0x10);
    for (i = 1; i < 20; i++) {
        float primary_bolt_z_offset;
        float arc_endpoint_distance;
        float blended_step_length;
        float azimuth_angle;
        float b;
        primary_bolt_z_offset = *(float *)&primary_bolt_z_amplitude_start_gp +
                                (*(float *)&primary_bolt_z_amplitude_end_gp -
                                 *(float *)&primary_bolt_z_amplitude_start_gp) *
                                    (convert_integer_to_float(i) * 0.05f);
        primary_bolt_z_offset =
            primary_bolt_z_offset *
            fast_sin(normalize_angle(*(float *)&D_L18_00162008 +
                                     (float)i * *(float *)&primary_bolt_z_phase_step_gp));
        primary_bolt_z_offset = primary_bolt_z_offset +
                                FUN_L00_00257c48(*(float *)&primary_bolt_z_jitter_range_start_gp,
                                                 *(float *)&primary_bolt_z_jitter_range_end_gp);
        D_L18_001D9D00[0][i].f[2] = D_L18_001D9D00[0][i].f[2] - D_L18_001D9F80[i];
        D_L18_001D9F80[i] = primary_bolt_z_offset;
        azimuth_angle = random_angle_radians();
        b = random_angle_radians();
        build_spherical_offset(&scratch_vectors[1], scale, azimuth_angle, b);
        scratch_vectors[1].f[2] = scratch_vectors[1].f[2] * 0.5f;
        lerp_vector(&scratch_vectors[2],
                    &scratch_vectors[3],
                    &scratch_vectors[1],
                    *(float *)&primary_bolt_random_step_blend_weight_gp);
        blended_step_length = fast_vec_length(&scratch_vectors[2]);
        if (blended_step_length == 0.0f) {
            qcopy(&D_L18_001D9D00[0][i], &D_L18_001D9D00[0][i - 1]);
        } else {
            scale_vector_xyz_c(
                &scratch_vectors[2], &scratch_vectors[2], scale / blended_step_length);
            fast_vec_add(&D_L18_001D9D00[0][i], &D_L18_001D9CF0[i], &scratch_vectors[2]);
            qcopy(&scratch_vectors[3], &scratch_vectors[2]);
        }
        D_L18_001D9D00[0][i].f[2] = D_L18_001D9D00[0][i].f[2] + primary_bolt_z_offset;
        fast_vec_sub(&scratch_vectors[1], &D_L18_001D9D00[0][i], &D_L18_001D9CF0[i]);
        fast_vec_sub(&scratch_vectors[5], data + 0x10, &D_L18_001D9D00[0][i]);
        arc_endpoint_distance = fast_vec_length(&scratch_vectors[5]);
        if (arc_endpoint_distance != 0.0f) {
            float endpoint_direction_blend_weight;
            if (i == 19) {
                endpoint_direction_blend_weight = 0.5f;
                scale = arc_endpoint_distance;
            } else if (i >= 16) {
                endpoint_direction_blend_weight = 0.5f;
                scale = arc_endpoint_distance / convert_integer_to_float(19 - i);
            } else {
                endpoint_direction_blend_weight = 0.1f;
            }
            scale_vector_xyz_c(
                &scratch_vectors[5], &scratch_vectors[5], scale / arc_endpoint_distance);
            lerp_vector(&scratch_vectors[3],
                        &scratch_vectors[3],
                        &scratch_vectors[5],
                        endpoint_direction_blend_weight);
        }
        if (i < 10) {
            float g;
            float secondary_bolt_z_offset;
            float secondary_bolt_lateral_angle;
            float secondary_bolt_lateral_offset;
            g = normalize_angle(*(float *)&D_L18_00162010 +
                                (float)i * *(float *)&secondary_bolt_orbit_phase_step_gp);
            secondary_bolt_z_offset =
                *(float *)&secondary_bolt_z_amplitude_gp *
                fast_sin(normalize_angle(*(float *)&D_L18_00162024 +
                                         (float)i * *(float *)&secondary_bolt_z_phase_step_gp));
            qcopy(&D_L18_001D9D00[1][i], &D_L18_001D9D00[0][i]);
            secondary_bolt_z_offset =
                secondary_bolt_z_offset * fast_sin(g) + FUN_L00_00257c48(0.0f, 0.1f);
            D_L18_001D9D00[1][i].f[2] = D_L18_001D9D00[1][i].f[2] + secondary_bolt_z_offset;
            D_L18_001DA110[i] = secondary_bolt_z_offset;
            secondary_bolt_lateral_angle = fast_add_rotations(
                fast_arc_tan(scratch_vectors[1].f[0], scratch_vectors[1].f[1]), 1.5707964f);
            secondary_bolt_lateral_offset =
                *(float *)&secondary_bolt_lateral_amplitude_gp * fast_cos(g);
            scratch_vectors[4].f[0] =
                fast_cos(secondary_bolt_lateral_angle) * secondary_bolt_lateral_offset;
            scratch_vectors[4].f[1] =
                fast_sin(secondary_bolt_lateral_angle) * secondary_bolt_lateral_offset;
            scratch_vectors[4].f[2] = 0.0f;
            qcopy(&D_L18_001D9FD0[i], &scratch_vectors[4]);
            fast_vec_add(&D_L18_001D9D00[1][i], &D_L18_001D9D00[1][i], &scratch_vectors[4]);
        } else {
            int r = 21 - i;
            qcopy(&D_L18_001D9D00[1][i], &D_L18_001D9D00[0][i]);
            D_L18_001D9D00[1][i].f[2] = D_L18_001D9D00[1][i].f[2] + D_L18_001DA110[r];
            fast_vec_add(&D_L18_001D9D00[1][i], &D_L18_001D9D00[1][i], &D_L18_001D9FD0[r]);
            qcopy(&scratch_vectors[4], &D_L18_001D9FD0[i]);
        }
    }
    fast_vec_cross(&scratch_vectors[6], &scratch_vectors[0], D_0013F5E0);
    fast_vec_normalize(&scratch_vectors[6], &scratch_vectors[6], 1.0f);
    fast_vec_normalize(&scratch_vectors[7], D_0013F5E0, 1.0f);
    for (k = 0; k < 4; k++) {
        float side_arc_scale;
        if (k < 2) {
            qcopy(&scratch_vectors[8], (char *)moby + 0x10);
            side_arc_scale = 0.4f;
        } else {
            qcopy(&scratch_vectors[8], data + 0x10);
            side_arc_scale = 0.5f;
        }
        if (fast_dec_timer_s16(D_L18_001620B8 + k)) {
            int j;
            D_L18_001620B8[k] = scale_game_frames(electric_bolt_side_arc_reset_frames_macro[k]);
            D_L18_001620C0[k] = 0;
            scale = FUN_L00_00257c48(0.52359879f, 1.04719758f);
            rotation_angle_or_particle_scale = random_float_between(-1.04719758f, 0.17453292f);
            scale_vector_xyz_c(&scratch_vectors[5], &scratch_vectors[0], side_arc_scale);
            build_look_at_matrix(&scratch_vectors[5],
                                 &scratch_vectors[5],
                                 &scratch_vectors[6],
                                 rotation_angle_or_particle_scale);
            build_look_at_matrix(
                &scratch_vectors[5], &scratch_vectors[5], &scratch_vectors[7], scale);
            qcopy(&l18_electric_bolt_side_arc_points[k].p[0], &scratch_vectors[8]);
            fast_vec_add(&l18_electric_bolt_side_arc_points[k].p[1],
                         &scratch_vectors[8],
                         &scratch_vectors[5]);
            side_arc_scale = 1.0f;
            for (j = 2; j < 5; j++) {
                fast_vec_sub(&scratch_vectors[9],
                             &l18_electric_bolt_side_arc_predecessor_view[k].p[j - 1],
                             D_L18_001677C0_c);
                rotation_angle_or_particle_scale =
                    random_float_between(0.17453292f, 0.959931076f) * side_arc_scale;
                side_arc_scale = -side_arc_scale;
                build_look_at_matrix(&scratch_vectors[5],
                                     &scratch_vectors[5],
                                     &scratch_vectors[9],
                                     rotation_angle_or_particle_scale);
                fast_vec_add((l18_electric_bolt_side_arc_points[k].p + j),
                             &l18_electric_bolt_side_arc_predecessor_view[k].p[j - 1],
                             &scratch_vectors[5]);
            }
        } else {
            int j;
            for (j = 1; j < 5; j++) {
                fast_vec_sub(&side_arc_segment_offsets[j],
                             (l18_electric_bolt_side_arc_points[k].p + j),
                             &l18_electric_bolt_side_arc_predecessor_view[k].p[j - 1]);
                side_arc_segment_offsets[j].f[0] += FUN_L00_00257c48(0.0f, 0.1f);
                side_arc_segment_offsets[j].f[1] += FUN_L00_00257c48(0.0f, 0.1f);
                side_arc_segment_offsets[j].f[2] += FUN_L00_00257c48(0.0f, 0.1f);
            }
            qcopy(&l18_electric_bolt_side_arc_points[k].p[0], &scratch_vectors[8]);
            for (j = 1; j < 5; j++) {
                fast_vec_add((l18_electric_bolt_side_arc_points[k].p + j),
                             &l18_electric_bolt_side_arc_predecessor_view[k].p[j - 1],
                             &side_arc_segment_offsets[j]);
            }
            {
                int   x = electric_bolt_side_arc_timer_distance_references_macro[k];
                int   y = D_L18_001620B8[k];
                float fr;
                if (x < y) {
                    x = y - x;
                } else {
                    x = x - y;
                }
                fr = convert_integer_to_float(x);
                fr = fr / convert_integer_to_float(scale_game_frames(15));
                D_L18_001620C0[k] = truncate_float_to_s32((1.0f - fr) * 32.0f);
            }
        }
    }
    scale = FUN_L00_00257c48(0.52359879f, 1.04719758f);
    rotation_angle_or_particle_scale = random_float_between(-0.52359879f, 1.04719758f);
    scale_vector_xyz_c(&scratch_vectors[5],
                       &scratch_vectors[0],
                       *(float *)&electric_bolt_particle_direction_scale_gp * frame_scale_macro);
    build_look_at_matrix(&scratch_vectors[5],
                         &scratch_vectors[5],
                         &scratch_vectors[6],
                         rotation_angle_or_particle_scale);
    build_look_at_matrix(&scratch_vectors[5], &scratch_vectors[5], &scratch_vectors[7], scale);
    r = random_int_between(1, 8);
    if (random_integer_below(2))
        r = -r;
    rotation_angle_or_particle_scale = random_float_between(0.2f, 0.6f);
    spawn_ec290((char *)moby + 0x10,
                scale_game_frames(15),
                rotation_angle_or_particle_scale * 0.3f,
                rotation_angle_or_particle_scale,
                *(int *)&D_L18_00162054_gp | 0x7F000000,
                0,
                r,
                &scratch_vectors[5],
                0.0f);
    r = -r;
    spawn_ec290((char *)moby + 0x10,
                scale_game_frames(15),
                rotation_angle_or_particle_scale * 0.15f,
                rotation_angle_or_particle_scale * 0.5f,
                0x307F7F7F,
                0,
                r,
                &scratch_vectors[5],
                0.0f);
    r = random_int_between(1, 8);
    if (random_integer_below(2))
        r = -r;
    rotation_angle_or_particle_scale = random_float_between(0.15f, 3.0f);
    spawn_ec290(data + 0x10,
                scale_game_frames(12),
                rotation_angle_or_particle_scale * 0.1f,
                rotation_angle_or_particle_scale,
                *(int *)&D_L18_00162054_gp | 0x7F000000,
                0,
                r,
                &D_L18_0015F580,
                0.0f);
}

int D_L18_00162008_gp __asm__("D_L18_00162008") = 0;
int D_L18_00162010_gp __asm__("D_L18_00162010") = 0;
int D_L18_00162024_gp __asm__("D_L18_00162024") = 0;

short electric_bolt_side_arc_reset_frames[4] __asm__("D_L18_001620C8") = {0};

short electric_bolt_side_arc_timer_distance_references[4] __asm__("D_L18_001620D0") = {0};

int D_L18_001620DC = 0;
