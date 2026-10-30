/* Level 16 elevator. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/audio/voice_alloc.h"

extern float frame_time, D_0015EE70;
extern void  release_voice_slot(s32) __asm__("FUN_0022d798");

#include "rnc/overlay/quad.h"

extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");

/* Moves the platform between two heights above its joint, with a particle while it moves. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E93F8), where it is exact; names translated to the US level program. */

typedef union {
    OvlQuad q;
    float   v[4];
} L16PlatformVector;

typedef struct {
    char  pad00[0x60];
    float basis[4][4];
    int   joint;
    int   padA4;
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

extern char *elevator_reference_transform_table __asm__("D_L16_001600FC");
extern float elevator_height_offset __asm__("D_L16_00161EAC")
    __attribute__((sda)); /* no foreign declaration */
extern float elevator_height_acceleration __asm__("D_L16_00161EBC")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L16_00161EB8_gp __asm__("D_L16_00161EB8") __attribute__((sda));
void       draw_elevator_rotating_rings(struct Moby *elevator_moby) __asm__("FUN_L16_002e84e8");

extern void draw_elevator_rotating_rings(struct Moby *) __asm__("FUN_L16_002e84e8");

void elevator_update(L16PlatformMoby *moby) __asm__("FUN_L16_002e7f80");

void elevator_update(L16PlatformMoby *moby) {
    enum {
        ELEVATOR_INITIALIZE = 0,
        ELEVATOR_WAIT_FOR_HERO = 2,
        ELEVATOR_UPDATE_HEIGHT = 3
    };
    L16PlatformVector delta, old_rotation, motion;
    L16PlatformData  *elevator_data = moby->data;
    float            *position = moby->position;
    float            *rotation = moby->rotation;

    fast_vec_scale(delta.v, position, -1.0f);
    qcopy(old_rotation.v, rotation);
    if (elevator_data) {
        switch (moby->state) {
        case ELEVATOR_INITIALIZE:
            motion.q = 0;
            motion.v[2] = 1.0f;
            elevator_data->particle = -1;
            transform_vector_by_basis(motion.v,
                                      motion.v,
                                      elevator_reference_transform_table +
                                          (elevator_data->joint << 7));
            elevator_data->goal = motion.v[2] + *(float *)(elevator_reference_transform_table +
                                                           elevator_data->joint * 0x80 + 0x38);
            moby->position[2] = motion.v[2] +
                                *(float *)(elevator_reference_transform_table +
                                           elevator_data->joint * 0x80 + 0x38) +
                                elevator_height_offset;
            moby->reverse = 1;
            moby->state = 2;
            break;
        case ELEVATOR_WAIT_FOR_HERO:
            if (voice_is_playing(moby, elevator_data->particle)) {
                release_voice_slot(elevator_data->particle);
                elevator_data->particle = -1;
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
                                 elevator_reference_transform_table + (elevator_data->joint << 7));
                motion.v[2] = motion.v[2] +
                              *(float *)(elevator_reference_transform_table +
                                         elevator_data->joint * 0x80 + 0x38) +
                              elevator_height_offset;
                if (absolute_float(hero.motion.pos.f[2] - motion.v[2]) < 1.0f) {
                    moby->reverse = (moby->reverse + 1) & 1;
                    elevator_data->goal = motion.v[2];
                    elevator_data->speed = 0.0f;
                    moby->state = 3;
                    break;
                }
            }
            if (hero.ground_moby == moby && hero.air_frames.s == 0) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                moby->reverse = (moby->reverse + 1) & 1;
                transform_vector(motion.v,
                                 motion.v,
                                 elevator_reference_transform_table + (elevator_data->joint << 7));
                elevator_data->goal = motion.v[2] +
                                      *(float *)(elevator_reference_transform_table +
                                                 elevator_data->joint * 0x80 + 0x38) +
                                      elevator_height_offset;
                elevator_data->speed = 0.0f;
                moby->state = 3;
            }
            break;
        case ELEVATOR_UPDATE_HEIGHT: {
            float *hero_position = hero.motion.pos.f;
            float  speed;

            if (vector_distance_xy(hero_position,
                                   elevator_reference_transform_table +
                                       (elevator_data->joint << 7) + 0x30) > 2.25f ||
                absolute_float(moby->position[2] - hero_position[2] - 2.0f) > 1.0f ||
                moby->position[2] < elevator_data->goal) {
                speed = advance_accelerated_scalar(&moby->position[2],
                                                   &elevator_data->speed,
                                                   elevator_data->goal,
                                                   elevator_height_acceleration * frame_time_sq,
                                                   elevator_height_acceleration * frame_time_sq,
                                                   *(float *)&D_L16_00161EB8_gp * frame_time);
            } else {
                speed =
                    advance_accelerated_scalar(&moby->position[2],
                                               &elevator_data->speed,
                                               moby->position[2],
                                               elevator_height_acceleration * 4.0f * frame_time_sq,
                                               elevator_height_acceleration * 4.0f * frame_time_sq,
                                               *(float *)&D_L16_00161EB8_gp * frame_time);
            }
            clear_u64_value(motion.v);
            motion.v[2] = speed;
            update_entity_matrix(
                (char *)elevator_data->basis, motion.v, moby->rotation, moby->rotation);
            if (motion.v[2] != 0.0f) {
                if (!voice_is_playing(moby, elevator_data->particle)) {
                    elevator_data->particle = allocate_voice_for_target_entry(0, 4, moby);
                }
            } else if (voice_is_playing(moby, elevator_data->particle)) {
                release_voice_slot(elevator_data->particle);
                elevator_data->particle = -1;
            }
            if (moby->position[2] == elevator_data->goal &&
                (hero.ground_moby != moby || hero.air_frames.s != 0)) {
                clear_u64_value(elevator_data->basis[1]);
                moby->state = 2;
                break;
            }
            if (elevator_data->speed != 0.0f) {
                add_draw_callback(draw_elevator_rotating_rings, moby);
            }
            break;
        }
        }
        fast_vec_add(delta.v, delta.v, moby->position);
        update_entity_matrix((char *)elevator_data->basis, delta.v, old_rotation.v, moby->rotation);
    }
}

extern int D_L16_0015F5CC;

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

extern float elevator_ring_quad_uvs[][2] __asm__("D_L16_001D9880");
extern float fast_cos(float);
extern float fast_sin(float);
extern float D_L16_00161EB8_gp __asm__("D_L16_00161EB8") __attribute__((sda));

#include "rnc/overlay/l16/mobys/elevator_globals.h"

extern float elevator_ring_outer_radius_gp __asm__("D_L16_00161EC4") __attribute__((sda));
extern float elevator_ring_half_height_gp __asm__("D_L16_00161EC8") __attribute__((sda));
extern float elevator_ring_segment_angle_degrees_gp __asm__("D_L16_00161EE8") __attribute__((sda));
int          D_L16_00161ECC = 0;
int          D_L16_00161ED0 = 2;
int          D_L16_00161ED4 = 0;
int          D_L16_00161ED8 = 1;
int          D_L16_00161EDC = 0x408000;
int          D_L16_00161EE0 = 0x40c08030;
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  draw_geometry_quad(void *, void *, int);

void draw_elevator_rotating_rings(struct Moby *elevator_moby) {
    L16RingPacket packet;
    L16RingMatrix matrix;
    char         *elevator_data = (char *)elevator_moby->pvars;
    int           bound_index = (*(int *)((char *)elevator_data + 0xA0)) << 7;
    char         *entry = (char *)(bound_index + (int)elevator_reference_transform_table);
    float         translation = (*(float *)((char *)elevator_data + 0xB8));
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
    color = fast_tween_color(D_L16_00161EDC,
                             D_L16_00161EE0,
                             absolute_float((*(float *)((char *)elevator_data + 0xB0))) /
                                 (D_L16_00161EB8_gp * frame_time));
    copy_matrix_rotation(&matrix, &elevator_moby->rot_row0);
    qcopy(matrix.position, &elevator_moby->pos);
    matrix.position[2] = lower;
    matrix.position[3] = 1.0f;
    packet.texture = get_effect_texture(14);
    packet.flags = 0xFF9000000260L;
    packet.mode = (long)D_L16_00161ECC | ((long)D_L16_00161ED0 << 2) | ((long)D_L16_00161ED4 << 4) |
                  ((long)D_L16_00161ED8 << 6) | 0x8000000000L;
    packet.zero = 0;
    uv = elevator_ring_quad_uvs;
    point = packet.position;
    v = &packet.uv[0].v;
    u = &packet.uv[0].u;
    for (i = 0; i < 4; i++) {
        *u = (*uv)[0];
        *v = (*uv)[1];
        if (i & 1) {
            (*point)[2] = elevator_ring_half_height_gp;
            radius = elevator_ring_outer_radius_gp;
        } else {
            (*point)[2] = -elevator_ring_half_height_gp;
            radius = elevator_ring_outer_radius_gp - 0.05f;
        }
        index = (float)(i >> 1);
        (*point)[0] =
            fast_cos(index * (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD)) * radius;
        (*point)[1] =
            fast_sin(index * (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD)) * radius;
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
            phase = (float)(D_L16_0015F5CC % period) * 0.02617991715669632f;
            sign = -sign;
            for (j = 0; (float)j < 360.0f / elevator_ring_segment_angle_degrees_gp; j++) {
                matrix.basis[0][0] = fast_cos(fast_normalize_angle(
                    (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j +
                    phase * (float)sign));
                matrix.basis[0][1] = fast_sin(fast_normalize_angle(
                    (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j +
                    phase * (float)sign));
                matrix.basis[0][2] = 0.0f;
                matrix.basis[1][0] = fast_cos(fast_normalize_angle(
                    (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j + 1.5707964f +
                    phase * (float)sign));
                matrix.basis[1][1] = fast_sin(fast_normalize_angle(
                    (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j + 1.5707964f +
                    phase * (float)sign));
                matrix.basis[1][2] = 0.0f;
                matrix.basis[2][2] = 1.0f;
                packet.color[3] = color;
                packet.color[2] = color;
                packet.color[1] = color;
                packet.color[0] = color;
                draw_geometry_quad(&packet, transform, 0);
            }
            matrix.position[2] += (*(float *)((char *)elevator_data + 0xBC));
        } while (matrix.position[2] < upper);
    }
}
