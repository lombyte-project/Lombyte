/* Level 06 moby class 0x38f: toxic gas vent. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/world/collision/collision_line.h"
#include "qcopy.h"

/* raise each listed moby's position z by a fixed amount and notify it */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC4C0), where it is exact; names translated to the US level program. */
extern float random_float_between(float, float) __asm__("FUN_002132a8");

/* Registers the moby's draw callbacks, choosing by its four slot ids. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FE4B8), where it is exact; names translated to the US level program. */
extern int   is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
void         emit_toxic_gas_vent_sparks(char *vent_moby) __asm__("FUN_L06_002f3d70");
extern void  emit_toxic_gas_vent_sparks(char *) __asm__("FUN_L06_002f3d70");
extern void  check_oscillating_toxic_gas_vent_collisions(char *) __asm__("FUN_L06_002f4130");
extern void  check_timed_toxic_gas_vent_collisions(char *) __asm__("FUN_L06_002f4390");
extern void  release_voice_slot(s32) __asm__("FUN_0022d798");
extern char  D_0013E550[];
extern int   allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern short D_L06_00161D24 __attribute__((sda));
extern short D_L06_00161D28 __attribute__((sda));
void         toxic_gas_vent_update(char *m) __asm__("FUN_L06_002f3ad8");

/* Sparks thrown off the swinging hazard while the camera is near it. */
extern f32  random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void random_vector(void *, f32, f32) __asm__("FUN_L00_00257d78");
extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float spark_speed_gp __asm__("D_L06_00161CEC") __attribute__((sda));

int spark_color0 __asm__("D_L06_00161CFC") = 0x6070ff90;

int spark_color1 __asm__("D_L06_00161D00") = 0x10b0ffc0;

extern int spark_life_min __asm__("D_L06_00161D04") __attribute__((sda));
extern int spark_life_max __asm__("D_L06_00161D08") __attribute__((sda));
extern int spark_fade_min __asm__("D_L06_00161D0C") __attribute__((sda));
extern int spark_fade_max __asm__("D_L06_00161D10") __attribute__((sda));

int spark_frames __asm__("D_L06_00161D14") = 0x1e;

extern float spark_drift_scale __asm__("D_L06_00161D1C") __attribute__((sda));
extern float spark_spread __asm__("D_L06_00161D20") __attribute__((sda));
extern float spark_dir __asm__("D_L06_00161CF4") __attribute__((sda));
extern float spark_vel __asm__("D_L06_00161CF8") __attribute__((sda));
extern float spark_drift __asm__("D_L06_00161D18") __attribute__((sda));

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F5560), where it is exact; names translated to the US level program. */
typedef struct {
    int            pad0;
    int            pad4;
    float          f8;
    float          fC;
    void          *m;
    int            flags;
    char           b18;
    char           b19;
    unsigned short h1A;
    float          f1C;
    int            i20;
} Sx;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vx;

extern f32   spark_speed_gp __asm__("D_L06_00161CEC") __attribute__((sda));
extern short toxic_gas_vent_max_collision_range_gp __asm__("D_L06_00161CF0") __attribute__((sda));
extern void  release_voice_slot(int) __asm__("FUN_0022d798");
extern float scale_time(float) __asm__("FUN_001f96b0");

#include "sda.h"

void toxic_gas_vent_update(char *vent_moby) {
    enum {
        VENT_INITIALIZE = 0,
        VENT_OSCILLATE = 1,
        VENT_TIMED_ACTIVE = 2,
        VENT_WAIT_FOR_ACTIVATION = 3
    };
    char *vent_data = ((char *)MOBY(vent_moby)->pvars);
    float heading_offset_radians;
    if (*(int *)(vent_data + 0x90) >= 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(vent_data + 0x90)) != 0) {
            float x = *(float *)&D_L06_00161D24;
            float y = *(float *)&D_L06_00161D28;
            *(float *)(vent_data + 0x50) = x;
            *(float *)(vent_data + 0x54) = y;
        } else {
            *(int *)(vent_data + 0x50) = 0;
            *(int *)(vent_data + 0x54) = 0;
        }
    }
    switch (((unsigned char *)vent_moby)[0x20]) {
    case VENT_INITIALIZE:
        if (*(int *)(vent_data + 0x60) != 0) {
            vent_moby[0x20] = 1;
            *(float *)(vent_data + 0x80) =
                360.0f / *(float *)(vent_data + 0x68) * DEG_TO_RAD * frame_time;
            *(float *)(vent_data + 0x7C) = *(float *)(vent_data + 0x6C) * DEG_TO_RAD;
            *(float *)(vent_data + 0x84) = MOBY(vent_moby)->rot.z;
        } else {
            vent_moby[0x20] = 3;
            *(int *)(vent_data + 0x88) = scale_game_frames(truncate_float_to_s32(
                *(float *)(vent_data + 0x74) + *(float *)(vent_data + 0x78) * 60.0f));
        }
        *(int *)(vent_data + 0x94) = -1;
        break;
    case VENT_OSCILLATE:
        if (voice_is_playing(vent_moby, *(int *)(vent_data + 0x94)) == 0) {
            *(int *)(vent_data + 0x94) = allocate_voice_for_target_entry(0, 4, (int)vent_moby);
        }
        emit_toxic_gas_vent_sparks(vent_moby);
        check_oscillating_toxic_gas_vent_collisions(vent_moby);
        heading_offset_radians =
            *(float *)(vent_data + 0x64) * DEG_TO_RAD * fast_sin(*(float *)(vent_data + 0x7C));
        *(float *)(vent_data + 0x7C) =
            fast_add_rotations(*(float *)(vent_data + 0x7C), *(float *)(vent_data + 0x80));
        MOBY(vent_moby)->rot.z =
            fast_add_rotations(*(float *)(vent_data + 0x84), heading_offset_radians);
        break;
    case VENT_TIMED_ACTIVE:
        if (voice_is_playing(vent_moby, *(int *)(vent_data + 0x94)) == 0) {
            *(int *)(vent_data + 0x94) = allocate_voice_for_target_entry(0, 4, (int)vent_moby);
        }
        emit_toxic_gas_vent_sparks(vent_moby);
        check_timed_toxic_gas_vent_collisions(vent_moby);
        if (fast_dec_timer(vent_data + 0x88) != 0) {
            vent_moby[0x20] = 3;
            *(int *)(vent_data + 0x88) =
                scale_game_frames(truncate_float_to_s32(*(float *)(vent_data + 0x74) * 60.0f));
            if (*(int *)(vent_data + 0x94) != -1) {
                char *voice_slot = ((char *)&D_0013E550) + *(int *)(vent_data + 0x94) * 0x70;
                if (*(char **)(voice_slot + 0x88) == vent_moby &&
                    ((unsigned char *)voice_slot)[0x74] != 0) {
                    release_voice_slot(*(int *)(vent_data + 0x94));
                }
            }
            *(int *)(vent_data + 0x94) = -1;
        }
        break;
    case VENT_WAIT_FOR_ACTIVATION:
        if (fast_dec_timer(vent_data + 0x88) != 0) {
            vent_moby[0x20] = 2;
            *(int *)(vent_data + 0x88) =
                scale_game_frames(truncate_float_to_s32(*(float *)(vent_data + 0x70) * 60.0f));
        }
        break;
    }
}

void emit_toxic_gas_vent_sparks(char *vent_moby) {
    Vec4  vent_rotation_matrix[4];
    Vec4  at;
    Vec4  spark_emission_step;
    Vec4  drift;
    Vec4  offset;
    Vec4  spark_spread_with_drift;
    Vec4  sphere;
    Vec4 *pos = (Vec4 *)&MOBY(vent_moby)->pos;
    int   life;
    int   fade;
    int   frames;
    int   sampled_frame_count;

    qcopy(&sphere, pos);
    sphere.f[3] = 4.0f;
    if (fast_vec_dist(&hero.motion.pos, pos) > 10.0f && fast_bsphere_check(&sphere, 24.0f) == -1) {
        return;
    }
    if (fast_vec_dist(&hero.motion.pos, pos) > 24.0f) {
        return;
    }
    qcopy(&at, pos);
    euler_rotation_matrix(vent_rotation_matrix, vent_moby + 0x40);
    clear_u64_value(&spark_emission_step);
    spark_emission_step.f[0] = spark_speed_gp * frame_time;
    transform_vector(&spark_emission_step, &spark_emission_step, vent_rotation_matrix);
    spark_emission_step.f[3] = spark_dir;
    fast_vec_scale(&drift, &spark_emission_step, spark_drift_scale);
    drift.f[2] = spark_drift * frame_time;
    spark_spread_with_drift.f[3] = spark_vel;

    life = truncate_float_to_s32(scale_time(random_float_between(spark_life_min, spark_life_max)));
    fade = truncate_float_to_s32(scale_time(random_float_between(spark_fade_min, spark_fade_max)));
    sampled_frame_count =
        truncate_float_to_s32(scale_time(random_float_between(spark_life_min, spark_life_max)));
    if (life < sampled_frame_count)
        life = sampled_frame_count;
    sampled_frame_count =
        truncate_float_to_s32(scale_time(random_float_between(spark_fade_min, spark_life_max)));
    if (fade < sampled_frame_count)
        fade = sampled_frame_count;
    frames = truncate_float_to_s32(scale_game_frames(spark_frames));

    fast_vec_scale(&offset, &spark_emission_step, random_float_between(0.0f, 1.0f));
    fast_vec_add(&at, pos, &offset);
    random_vector(&spark_spread_with_drift, 0.0f, spark_spread * frame_time);
    fast_vec_add(&spark_spread_with_drift, &spark_spread_with_drift, &drift);
    spawn_spark(&at,
                &spark_emission_step,
                &spark_spread_with_drift,
                spark_color0,
                spark_color1,
                life,
                fade,
                frames,
                0x30);

    life = truncate_float_to_s32(scale_time(random_float_between(spark_life_min, spark_life_max)));
    fade = truncate_float_to_s32(scale_time(random_float_between(spark_fade_min, spark_fade_max)));
    fast_vec_scale(&offset, &spark_emission_step, random_float_between(0.0f, 1.0f));
    fast_vec_add(&at, pos, &offset);
    random_vector(&spark_spread_with_drift, 0.0f, spark_spread * frame_time);
    fast_vec_add(&spark_spread_with_drift, &spark_spread_with_drift, &drift);
    spawn_spark(&at,
                &spark_emission_step,
                &spark_spread_with_drift,
                spark_color0,
                spark_color1,
                life,
                fade,
                frames,
                0x30);
}

void check_oscillating_toxic_gas_vent_collisions(char *m) {
    Sx    collision_params;
    Vx    segment_start;
    Vx    segment_end;
    int   i;
    char *moby_position = m + 0x10;
    int   one = 1;
    char *moby_data;
    int   segment_travel_step_count;
    float segment_phase_step, segment_radius_step, remaining_radius, segment_start_radius,
        oscillation_phase_radians, ra, rb;
    moby_data = *(char **)(m + 0x78);
    segment_travel_step_count =
        truncate_float_to_s32(*(float *)&toxic_gas_vent_max_collision_range_gp /
                              (*(float *)&spark_speed_gp * frame_time) * 0.25f);
    segment_phase_step = *(float *)(moby_data + 0x80) * (float)segment_travel_step_count;
    segment_radius_step = *(float *)&toxic_gas_vent_max_collision_range_gp * 0.25f;
    remaining_radius = *(float *)&toxic_gas_vent_max_collision_range_gp;
    oscillation_phase_radians =
        fast_subtract_rotations(*(float *)(moby_data + 0x7C), segment_phase_step * 4.0f);
    collision_params.m = m;
    collision_params.f1C = 1.0f;
    collision_params.flags = hero.items[2].item_id != 6 ? 0x10001 : 0x10000;
    collision_params.i20 = one;
    clear_u64_value(&collision_params);
    collision_params.f8 = 1.0f;
    collision_params.fC = 5627.9248f;
    collision_params.b18 = 3;
    collision_params.b19 = one;
    collision_params.h1A = *(unsigned short *)(m + 0xA6);
    for (i = 3; i >= 0; i--) {
        segment_start_radius = remaining_radius;
        ra = fast_add_rotations(*(float *)(moby_data + 0x84),
                                *(float *)(moby_data + 0x64) * DEG_TO_RAD *
                                    fast_sin(oscillation_phase_radians));
        remaining_radius -= segment_radius_step;
        oscillation_phase_radians =
            fast_add_rotations(oscillation_phase_radians, segment_phase_step);
        rb = fast_add_rotations(*(float *)(moby_data + 0x84),
                                *(float *)(moby_data + 0x64) * DEG_TO_RAD *
                                    fast_sin(oscillation_phase_radians));
        segment_start.x = fast_cos(ra) * segment_start_radius;
        segment_start.y = fast_sin(ra) * segment_start_radius;
        segment_start.z = 0.0f;
        segment_end.x = fast_cos(rb) * remaining_radius;
        segment_end.y = fast_sin(rb) * remaining_radius;
        segment_end.z = 0.0f;
        fast_vec_add(&segment_start, &segment_start, moby_position);
        fast_vec_add(&segment_end, &segment_end, moby_position);
        collision_line(&segment_start, &segment_end, 0, m, &collision_params);
    }
}

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F57C0), where it is exact; names translated to the US level program. */
void check_timed_toxic_gas_vent_collisions(char *vent_moby) {
    struct {
        int   pad0[2];
        float f8;
        float fC;
        char *owner;
        int   flags;
        char  b18;
        char  b19;
        short s1A;
        float f1C;
        int   i20;
    } q;
    float segment_start[4];
    float segment_end[4];
    char *vent_data;
    float sp;
    int   fl;

    fl = 0x10000;
    if (hero.items[2].item_id != 6)
        fl = 0x10001;
    q.flags = fl;
    vent_data = ((char *)MOBY(vent_moby)->pvars);
    q.owner = vent_moby;
    q.f1C = 1.0f;
    q.i20 = 1;
    clear_u64_value(&q);
    q.f8 = 1.0f;
    q.fC = 5627.9248046875f;
    q.b18 = 3;
    q.b19 = 1;
    q.s1A = ((unsigned short)MOBY(vent_moby)->oclass);
    sp = (*(float *)(vent_data + 0x70) * 60.0f - (float)*(int *)(vent_data + 0x88)) *
         (*(float *)&spark_speed_gp * frame_time);
    if (*(float *)&toxic_gas_vent_max_collision_range_gp < sp)
        sp = *(float *)&toxic_gas_vent_max_collision_range_gp;
    fast_vec_normalize(segment_start, vent_moby + 0xD0, 0.125f);
    fast_vec_add(segment_start, segment_start, vent_moby + 0x10);
    segment_start[2] = segment_start[2] + 0.125f;
    segment_end[0] = fast_cos(MOBY(vent_moby)->rot.z) * sp;
    segment_end[1] = fast_sin(MOBY(vent_moby)->rot.z) * sp;
    segment_end[2] = 0;
    fast_vec_add(segment_end, segment_end, segment_start);
    collision_line(segment_start, segment_end, 0, vent_moby, &q);
    fast_vec_normalize(segment_start, vent_moby + 0xD0, -0.125f);
    fast_vec_add(segment_start, segment_start, vent_moby + 0x10);
    segment_start[2] = segment_start[2] + 0.125f;
    segment_end[0] = fast_cos(MOBY(vent_moby)->rot.z) * sp;
    segment_end[1] = fast_sin(MOBY(vent_moby)->rot.z) * sp;
    segment_end[2] = 0;
    fast_vec_add(segment_end, segment_end, segment_start);
    collision_line(segment_start, segment_end, 0, vent_moby, &q);
}
