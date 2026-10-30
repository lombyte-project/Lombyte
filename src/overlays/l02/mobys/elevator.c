/* Level 02 moby class elevator. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/rendering/culling/test_point_in_transformed_unit_cylinder.h"

extern float D_0015ED6C;
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

#include "sda.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/l02/mobys/elevator_globals.h"

#include "qcopy.h"

extern int  voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");

typedef union {
    OvlQuad q;
    float   v[4];
} L16PlatformVector;

typedef struct {
    char  pad00[0x60];
    float basis[4][4];
    int   joint;
    int   stop;
    float goal;
    int   padAC;
    float speed;
    int   particle;
} L16PlatformData;

typedef struct {
    char             pad00[0x10];
    float            position[4];
    unsigned char    state;
    char             pad21[0x1F];
    float            rotation[4];
    char             pad50[0x28];
    L16PlatformData *data;
    char             pad7C[0x40];
    unsigned char    reverse;
} L16PlatformMoby;

extern char *level02_elevator_transform_table __asm__("D_L02_001600FC");
extern float level02_elevator_stop_height_offset_gp __asm__("D_L02_00161BD0") __attribute__((sda));
extern unsigned char D_0013D4DA NOT_SDA;
extern float level02_elevator_max_vertical_speed_gp __asm__("D_L02_00161BDC") __attribute__((sda));
extern float level02_elevator_vertical_acceleration_gp __asm__("D_L02_00161BE0")
    __attribute__((sda));
void draw_elevator_rotating_rings(struct Moby *moby) __asm__("FUN_L02_002dc2c8");

extern void draw_elevator_rotating_rings(struct Moby *) __asm__("FUN_L02_002dc2c8");
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Same platform routine as elevator_update, with a waiting state (1) and this level's tables. */
void elevator_update(L16PlatformMoby *moby) __asm__("FUN_L02_002dbd38");

void elevator_update(L16PlatformMoby *moby) {
    enum {
        ELEVATOR_INITIALIZE = 0,
        ELEVATOR_WAIT_FOR_RIDER_OR_TRIGGER = 1,
        ELEVATOR_WAIT_AT_STOP = 2,
        ELEVATOR_MOVE_TO_GOAL = 3
    };
    L16PlatformVector delta, old_rotation, motion;
    L16PlatformData  *elevator_vars = moby->data;
    float            *position = moby->position;
    float            *rotation = moby->rotation;

    fast_vec_scale(delta.v, position, -1.0f);
    qcopy(old_rotation.v, rotation);
    if (elevator_vars) {
        switch (moby->state) {
        case ELEVATOR_INITIALIZE:
            motion.q = 0;
            motion.v[2] = 1.0f;
            elevator_vars->particle = -1;
            transform_vector_by_basis(
                motion.v, motion.v, level02_elevator_transform_table + (elevator_vars->joint << 7));
            elevator_vars->goal = motion.v[2] + *(float *)(level02_elevator_transform_table +
                                                           elevator_vars->joint * 0x80 + 0x38);
            moby->position[2] =
                motion.v[2] +
                *(float *)(level02_elevator_transform_table + elevator_vars->joint * 0x80 + 0x38) +
                level02_elevator_stop_height_offset_gp;
            moby->reverse = 1;
            if (elevator_vars->stop) {
                moby->state = 2;
            } else {
                moby->state = 1;
            }
            break;
        case ELEVATOR_WAIT_AT_STOP:
            if (voice_is_playing(moby, elevator_vars->particle)) {
                release_voice_slot(elevator_vars->particle);
                elevator_vars->particle = -1;
            }
            if (vector_distance_xy(position, hero.motion.pos.f) < 20.0f) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                transform_vector(motion.v,
                                 motion.v,
                                 level02_elevator_transform_table + (elevator_vars->joint << 7));
                motion.v[2] = motion.v[2] +
                              *(float *)(level02_elevator_transform_table +
                                         elevator_vars->joint * 0x80 + 0x38) +
                              level02_elevator_stop_height_offset_gp;
                if (absolute_float(hero.motion.pos.f[2] - motion.v[2]) < 1.0f) {
                    moby->reverse = (moby->reverse + 1) & 1;
                    elevator_vars->goal = motion.v[2];
                    elevator_vars->speed = 0.0f;
                    moby->state = 3;
                    break;
                }
            }
            if (hero.ground_moby == (struct Moby *)moby && hero.air_frames.s == 0) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                moby->reverse = (moby->reverse + 1) & 1;
                transform_vector(motion.v,
                                 motion.v,
                                 level02_elevator_transform_table + (elevator_vars->joint << 7));
                elevator_vars->goal = motion.v[2] +
                                      *(float *)(level02_elevator_transform_table +
                                                 elevator_vars->joint * 0x80 + 0x38) +
                                      level02_elevator_stop_height_offset_gp;
                elevator_vars->speed = 0.0f;
                moby->state = 3;
            }
            break;
        case ELEVATOR_WAIT_FOR_RIDER_OR_TRIGGER:
            if ((hero.ground_moby == (struct Moby *)moby && hero.air_frames.s == 0) || D_0013D4DA) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                moby->reverse = (moby->reverse + 1) & 1;
                transform_vector(motion.v,
                                 motion.v,
                                 level02_elevator_transform_table + (elevator_vars->joint << 7));
                elevator_vars->goal = motion.v[2] +
                                      *(float *)(level02_elevator_transform_table +
                                                 elevator_vars->joint * 0x80 + 0x38) +
                                      level02_elevator_stop_height_offset_gp;
                elevator_vars->speed = 0.0f;
                moby->state = 3;
            }
            break;
        case ELEVATOR_MOVE_TO_GOAL: {
            float speed;

            if (!test_point_in_transformed_unit_cylinder(&hero.motion.body_center,
                                                         elevator_vars->joint) ||
                absolute_float(moby->position[2] - hero.motion.pos.f[2] - 2.0f) > 1.0f ||
                moby->position[2] < elevator_vars->goal) {
                speed = advance_accelerated_scalar(
                    &moby->position[2],
                    &elevator_vars->speed,
                    elevator_vars->goal,
                    level02_elevator_vertical_acceleration_gp * frame_time_sq,
                    level02_elevator_vertical_acceleration_gp * frame_time_sq,
                    level02_elevator_max_vertical_speed_gp * D_0015ED6C);
            } else {
                speed = advance_accelerated_scalar(
                    &moby->position[2],
                    &elevator_vars->speed,
                    moby->position[2],
                    level02_elevator_vertical_acceleration_gp * 4.0f * frame_time_sq,
                    level02_elevator_vertical_acceleration_gp * 4.0f * frame_time_sq,
                    level02_elevator_max_vertical_speed_gp * D_0015ED6C);
            }
            clear_u64_value(motion.v);
            motion.v[2] = speed;
            update_entity_matrix(
                (char *)elevator_vars->basis, motion.v, moby->rotation, moby->rotation);
            if (motion.v[2] != 0.0f) {
                if (!voice_is_playing(moby, elevator_vars->particle)) {
                    elevator_vars->particle = allocate_voice_for_target_entry(0, 4, (int)moby);
                }
            } else if (voice_is_playing(moby, elevator_vars->particle)) {
                release_voice_slot(elevator_vars->particle);
                elevator_vars->particle = -1;
            }
            if (moby->position[2] == elevator_vars->goal &&
                (hero.ground_moby != (struct Moby *)moby || hero.air_frames.s != 0)) {
                clear_u64_value(elevator_vars->basis[1]);
                moby->state = 2;
                break;
            }
            if (elevator_vars->speed != 0.0f) {
                add_draw_callback(draw_elevator_rotating_rings, moby);
            }
            break;
        }
        }
        fast_vec_add(delta.v, delta.v, moby->position);
        update_entity_matrix((char *)elevator_vars->basis, delta.v, old_rotation.v, moby->rotation);
    }
}

/* Draw stacked alternating rings with a rotating transformation and interpolated tint. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9960), where it is exact; names translated to the US level program. */

typedef float L16RingVector[4] __attribute__((aligned(16)));

typedef struct {
    L16RingVector position[4];
    int           color[4];
    struct {
        float u, v;
    } uv[4];
    long zero, texture, flags, mode;
} L16RingPacket;

typedef struct {
    L16RingVector basis[3];
    L16RingVector position;
} L16RingMatrix;

extern float level02_elevator_ring_segment_uvs[][2] __asm__("D_L02_001D3F50");
extern float fast_cos(float);
extern float fast_sin(float);
extern int   D_L02_0015F5CC;
extern float level02_elevator_ring_outer_radius_gp __asm__("D_L02_00161BEC") __attribute__((sda));
extern float level02_elevator_ring_vertical_spacing_gp __asm__("D_L02_00161BE8")
    __attribute__((sda));
extern float level02_elevator_ring_segment_half_height_gp __asm__("D_L02_00161BF0")
    __attribute__((sda));
extern float level02_elevator_ring_segment_angle_degrees_gp __asm__("D_L02_00161C10")
    __attribute__((sda));
int         D_L02_00161BF4 = 0;
int         D_L02_00161BF8 = 2;
int         D_L02_00161BFC = 0;
int         D_L02_00161C00 = 1;
int         D_L02_00161C04 = 0x206068;
int         D_L02_00161C08 = 0x40107020;
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, int);

void draw_elevator_rotating_rings(struct Moby *moby) {
    L16RingPacket packet;
    L16RingMatrix matrix;
    char         *moby_vars = (char *)moby->pvars;
    int           bound_index = (*(int *)((char *)moby_vars + 0xA0)) << 7;
    char         *entry = (char *)(bound_index + (int)level02_elevator_transform_table);
    float         translation = (*(float *)((char *)moby_vars + 0xB8));
    float         lower = (*(float *)((char *)entry + 0x38));
    float         half = (*(float *)((char *)entry + 0x28));
    float         upper = lower + half + translation;
    int           color;
    float(*uv)[2];
    L16RingVector *point;
    float         *v, *u;
    L16RingMatrix *transform;
    float          radius, index, phase;
    int            i, j, sign, period;
    lower = lower - half;
    lower += translation;
    color = fast_tween_color(D_L02_00161C04,
                             D_L02_00161C08,
                             absolute_float((*(float *)((char *)moby_vars + 0xB0))) /
                                 (level02_elevator_max_vertical_speed_gp * D_0015ED6C));
    copy_matrix_rotation(&matrix, &moby->rot_row0);
    qcopy(matrix.position, &moby->pos);
    matrix.position[2] = lower;
    matrix.position[3] = 1.0f;
    packet.texture = get_effect_texture(14);
    packet.flags = 0xFF9000000260L;
    packet.mode = (long)D_L02_00161BF4 | ((long)D_L02_00161BF8 << 2) | ((long)D_L02_00161BFC << 4) |
                  ((long)D_L02_00161C00 << 6) | 0x8000000000L;
    packet.zero = 0;
    uv = level02_elevator_ring_segment_uvs;
    point = packet.position;
    v = &packet.uv[0].v;
    u = &packet.uv[0].u;
    for (i = 0; i < 4; i++) {
        *u = (*uv)[0];
        *v = (*uv)[1];
        if (i & 1) {
            (*point)[2] = level02_elevator_ring_segment_half_height_gp;
            radius = level02_elevator_ring_outer_radius_gp;
        } else {
            (*point)[2] = -level02_elevator_ring_segment_half_height_gp;
            radius = level02_elevator_ring_outer_radius_gp - 0.05f;
        }
        index = (float)(i >> 1);
        (*point)[0] =
            fast_cos(index * (level02_elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD)) *
            radius;
        (*point)[1] =
            fast_sin(index * (level02_elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD)) *
            radius;
        (*point)[3] = 1.0f;
        v += 2;
        u += 2;
        uv++;
        point++;
    }
    transform = &matrix;
    sign = 1;
    if (matrix.position[2] < upper) {
        period = 120;
        do {
            phase = (float)(D_L02_0015F5CC % period) * 0.02617991715669632f;
            sign = -sign;
            for (j = 0; (float)j < 360.0f / level02_elevator_ring_segment_angle_degrees_gp; j++) {
                matrix.basis[0][0] = fast_cos(fast_normalize_angle(
                    (level02_elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j +
                    phase * (float)sign));
                matrix.basis[0][1] = fast_sin(fast_normalize_angle(
                    (level02_elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j +
                    phase * (float)sign));
                matrix.basis[0][2] = 0.0f;
                matrix.basis[1][0] = fast_cos(fast_normalize_angle(
                    (level02_elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j +
                    1.5707964f + phase * (float)sign));
                matrix.basis[1][1] = fast_sin(fast_normalize_angle(
                    (level02_elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j +
                    1.5707964f + phase * (float)sign));
                matrix.basis[1][2] = 0.0f;
                matrix.basis[2][2] = 1.0f;
                packet.color[3] = color;
                packet.color[2] = color;
                packet.color[1] = color;
                packet.color[0] = color;
                draw_geometry_quad(&packet, transform, 0);
            }
            matrix.position[2] += level02_elevator_ring_vertical_spacing_gp;
        } while (matrix.position[2] < upper);
    }
}
