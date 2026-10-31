/* Shared moby class metal_detector. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/math/rotations/matrix_to_quaternion.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/input/pad_state.h"
#include "qzero.h"
#include "rnc/overlay/shared/weapons/metal_detector_data/draw_metal_detector_trail.h"
#include "rnc/overlay/shared/weapons/metal_detector_data/metal_detector_update.h"

typedef struct PadState PadState;

typedef float Vec_2D3F40[4] __attribute__((aligned(16)));

typedef int U128_2D3F40 __attribute__((mode(TI)));

typedef union Vu {
    U128_2D3F40 q;
    Vec_2D3F40  f;
} Vu;

typedef struct Quad {
    Vec_2D3F40 v[4];
    int        c[4];
    float      uv[8];
    long       g[4];
} Quad;

typedef struct Data {
    char       pad0[0x10];
    Vec_2D3F40 pos; /* 0x10 */
    char       pad20[4];
    float      t;    /* 0x24 */
    float      tmax; /* 0x28 */
    char       pad2C[4];
    Vec_2D3F40 dir; /* 0x30 */
} Data;

typedef struct Moby_2D3F40 {
    char  pad0[0x78];
    Data *data; /* 0x78 */
    char  pad7C[0x14];
    int   rgba; /* 0x90 */
} Moby_2D3F40;

typedef f32 Mtx2d2ee8[4][4] __attribute__((aligned(16)));
typedef f32 Vec2d2ee8[4] __attribute__((aligned(16)));

typedef struct Vars2d2ee8 {
    s32  voice0; /* 0x00 */
    s32  voice1; /* 0x04 */
    s32  unk8;   /* 0x08: 0x7FFF */
    s32  voice2; /* 0x0C: distance hum */
    Vec4 pos;    /* 0x10 */
    u8   unk20;  /* 0x20 */
    u8   pad21[3];
    s32  unk24; /* 0x24 */
    s32  unk28; /* 0x28 */
    u8   pad2C[4];
    Vec4 unk30; /* 0x30 */
} Vars2d2ee8;

typedef struct TargetVars2d2ee8 {
    u8  pad0[0x18];
    s32 count; /* 0x18: drained by 5 per step */
} TargetVars2d2ee8;

typedef struct VoiceSlot2d2ee8 {
    u8           pad0[0x74];
    u8           active; /* 0x74 */
    u8           pad75[0x13];
    struct Moby *owner; /* 0x88 */
} VoiceSlot2d2ee8;

extern int D_0015ED84;
float      absolute_float(float input) __asm__("func_001F99C0");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

extern float D_0015ED6C;

extern int metal_detector_trail_initial_quad_color_gp __asm__("D_L00_00161914")
    __attribute__((sda));
extern int  metal_detector_trail_texture_selector_gp __asm__("D_L00_00161918") __attribute__((sda));
extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void func_L00_001FF4B0_2D3F40(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_L00_001FD1D8_2D3F40(void *, void *, int) __asm__("FUN_001f7d30");

extern unsigned char D_0014BF10[];

extern u8                  voice_pool_lui[] __asm__("D_0013E550") NOT_SDA;
extern u128 D_L00_001E1160 NOT_SDA;
extern u8                  D_L00_001E1170[] NOT_SDA;

extern void attach_manipulator(void *, s32, void *) __asm__("FUN_0020cb10");
extern void noop_main_callback(void *, void *) __asm__("FUN_001e93e8");
extern void build_quaternion_from_axis_rotations(void *, void *) __asm__("FUN_L00_00259f50");
extern void engine_voice_set_pitch(s32, s32) __asm__("FUN_L00_0028df38");
extern void add_random_offset_to_vector_xyz(void *, f32) __asm__("FUN_L00_0025f8e0");
extern void spawn_bolt_by_value(void *, void *, void *, s32, s32, s32) __asm__("FUN_L00_002a7438");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void FUN_L00_002d2a90_u(void) __asm__("FUN_L00_002d2a90");

extern struct PadState controller_state_lui __asm__("D_0013C940") NOT_SDA;

/* Ported from rac1-decomp src/overlays/shared/vendor_002D1168.c (func_L00_002D3F40) */

/* Draws the trail of fading ring sprites for a moby. */

void draw_metal_detector_trail(Moby_2D3F40 *moby) __asm__("FUN_L00_002d2a90");

void draw_metal_detector_trail(Moby_2D3F40 *moby) {
    Quad  q;
    Vu    tt[6];
    Data *trail_data = moby->data;
    float t, unwrapped_phase, k;
    int   col;
    int   trail_quad_texture;

    trail_quad_texture = get_effect_texture(metal_detector_trail_texture_selector_gp);
    q.c[0] = q.c[1] = q.c[2] = q.c[3] = metal_detector_trail_initial_quad_color_gp;
    q.g[1] = trail_quad_texture;
    q.g[2] = 0xFF9000000260L;
    q.g[0] = 4;
    q.g[3] = (long)metal_detector_trail_blend_minuend_selector_gp |
             ((long)metal_detector_trail_blend_subtrahend_selector_gp << 2) |
             ((long)metal_detector_trail_blend_alpha_factor_selector_gp << 4) |
             ((long)metal_detector_trail_blend_addend_selector_gp << 6) |
             ((long)metal_detector_trail_blend_fixed_alpha_gp << 32);
    q.uv[4] = q.uv[0] = 0.0f;
    q.uv[6] = q.uv[2] = 1.0f;
    q.uv[3] = q.uv[1] = 0.0f;
    q.uv[7] = q.uv[5] = 1.0f;
    trail_data->t = trail_data->t + D_0015ED6C;
    if (1.0f < trail_data->t) {
        trail_data->t = trail_data->t - 1.0f;
    }
    if (trail_data->t > trail_data->tmax) {
        trail_data->tmax = trail_data->t;
    }
    t = trail_data->t;
    tt[1].q = 0;
    tt[1].f[0] = 1.0f;
    func_L00_001FF4B0_2D3F40(tt[0].f, trail_data->dir, 1.0f);
    fast_vec_cross(tt[2].f, tt[0].f, tt[1].f);
    func_L00_001FF4B0_2D3F40(tt[2].f, tt[2].f, 1.0f);
    fast_vec_cross(tt[1].f, tt[0].f, tt[2].f);
    func_L00_001FF4B0_2D3F40(tt[1].f, tt[1].f, 1.0f);
    for (unwrapped_phase = trail_data->t; trail_data->t - 1.0f < unwrapped_phase;
         unwrapped_phase -= 0.1f) {
        if (t <= trail_data->tmax) {
            k = t * 0.5f + 0.23f;
            func_L00_001FF4B0_2D3F40(tt[3].f, tt[0].f, t);
            func_L00_001FF4B0_2D3F40(tt[4].f, tt[1].f, k);
            func_L00_001FF4B0_2D3F40(tt[5].f, tt[2].f, k);
            fast_vec_sub(q.v[0], trail_data->pos, tt[3].f);
            qcopy(q.v[1], q.v[0]);
            qcopy(q.v[2], q.v[0]);
            qcopy(q.v[3], q.v[0]);
            fast_vec_sub(q.v[0], q.v[0], tt[4].f);
            fast_vec_add(q.v[1], q.v[1], tt[4].f);
            fast_vec_sub(q.v[2], q.v[2], tt[4].f);
            fast_vec_add(q.v[3], q.v[3], tt[4].f);
            fast_vec_sub(q.v[0], q.v[0], tt[5].f);
            fast_vec_add(q.v[2], q.v[2], tt[5].f);
            fast_vec_sub(q.v[1], q.v[1], tt[5].f);
            fast_vec_add(q.v[3], q.v[3], tt[5].f);
            col = (moby->rgba & 0xFFFFFF) | (truncate_float_to_s32((1.0f - t) * 255.0f) << 24);
            q.c[0] = q.c[1] = q.c[2] = q.c[3] = col;
            func_L00_001FD1D8_2D3F40(&q, 0, 1);
        }
        t -= 0.1f;
        if (t < 0.0f) {
            t += 1.0f;
        }
    }
}

/* increments the selected packed nibble, saturating at fifteen */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D42D8), where it is exact; names translated to the US level program. */

void increment_metal_detector_collection_counter(unsigned char *moby) __asm__("FUN_L00_002d2e28");

void increment_metal_detector_collection_counter(unsigned char *moby) {
    int row = D_0015ED84;
    if (row < 20) {
        int index = moby[0xBC] - 1;
        if (index & 1) {
            unsigned char *p = D_0014BF10 + (index / 2 + row * 16);
            unsigned int   value = *p;
            int            n = (value & 15) + 1;
            if (n >= 16)
                n = 15;
            *p = (value & 240) | n;
        } else {
            unsigned char *p = D_0014BF10 + (index / 2 + row * 16);
            unsigned int   value = *p;
            int            n = (value >> 4) + 1;
            if (n >= 16)
                n = 15;
            *p = (value & 15) | (n << 4);
        }
    }
}

/* metal_detector_update: update for a moby that tracks hero.metal_detector_target, the nearest
 * oclass 0x25D moby. It orients itself towards it within the
 * D_L00_00161950/54 limits (degrees), tints itself by the distance
 * (hero.detector_target_dist) while the 0x20 pad button is held, and at under 1.0 drains
 * the target's count in steps of 5, then removes it. */

/* the unit declares mark_moby_for_removal() without a prototype further down */

void metal_detector_update(struct Moby *moby) __asm__("FUN_L00_002d2ee8");

/* Moby class 585 */
void metal_detector_update(struct Moby *moby) {
    enum {
        METAL_DETECTOR_INITIALIZE = 0,
        METAL_DETECTOR_ACTIVE = 1,
        METAL_DETECTOR_TRACK_AND_COLLECT = 2
    };
    Vars2d2ee8       *metal_detector_vars;
    struct Moby      *target_moby;
    TargetVars2d2ee8 *tv;
    Vec2d2ee8         u;
    Vec2d2ee8         v;
    Vec2d2ee8         scratch_vector;
    Mtx2d2ee8         aim_basis;
    Mtx2d2ee8         normalized_bone_transform;
    Mtx2d2ee8         transposed_bone_rotation;
    Mtx2d2ee8         aim_rotation_product;
    f32               yaw;
    f32               pitch;
    f32               abs_yaw;
    f32               abs_pitch;
    f32               aim_angle_limit_radians;
    f32               zero;
    f32               one;

    if (moby == 0) {
        return;
    }
    metal_detector_vars = (Vars2d2ee8 *)moby->pvars;
    if (metal_detector_vars == 0) {
        return;
    }
    moby_point_position(moby, 0, &metal_detector_vars->pos);
    switch (moby->state) {
    case METAL_DETECTOR_INITIALIZE:
        hero.detector_target_dist = 100000.0f;
        hero.metal_detector_target = 0;
        metal_detector_vars->voice0 = -1;
        metal_detector_vars->voice1 = -1;
        metal_detector_vars->voice2 = -1;
        metal_detector_vars->unk8 = 0x7FFF;
        metal_detector_vars->unk20 = controller_state_lui.unk189;
        metal_detector_vars->unk24 = 0;
        metal_detector_vars->unk28 = 0;
        qzero(&D_L00_001E1160);
        attach_manipulator(moby, 3, &D_L00_001E1160);
        moby->state = 1;
        break;
    case 3:
    case 4:
        break;
    case METAL_DETECTOR_ACTIVE:
    case METAL_DETECTOR_TRACK_AND_COLLECT:
        one = 1.0f;
        *(u128 *)v = 0;
        v[2] = one;
        if (hero.metal_detector_target != 0 && hero.metal_detector_target->oclass == 0x25D &&
            hero.metal_detector_target->state != 0xFE &&
            hero.metal_detector_target->state != 0xFD) {
            pitch = vector_distance_xy(&metal_detector_vars->pos, &hero.metal_detector_target->pos);
            pitch = fast_arc_tan(pitch,
                                 hero.metal_detector_target->pos.z - metal_detector_vars->pos.f[2]);
            yaw = fast_arc_tan(hero.metal_detector_target->pos.x - metal_detector_vars->pos.f[0],
                               hero.metal_detector_target->pos.y - metal_detector_vars->pos.f[1]);
            yaw = fast_subtract_rotations(yaw, hero.motion.rot.f[2]);
            pitch = fast_subtract_rotations(pitch, -1.5707964f);
            abs_yaw = absolute_float(yaw);
            abs_pitch = absolute_float(pitch);
            aim_angle_limit_radians = metal_detector_aim_yaw_limit_degrees_gp * 0.017453292f;
            if (aim_angle_limit_radians < abs_yaw) {
                yaw /= abs_yaw;
                yaw *= aim_angle_limit_radians;
            }
            aim_angle_limit_radians = metal_detector_aim_pitch_limit_degrees_gp * 0.017453292f;
            if (aim_angle_limit_radians < abs_pitch) {
                pitch /= abs_pitch;
                pitch *= aim_angle_limit_radians;
            }
            zero = 0.0f;
            yaw = fast_add_rotations(yaw, hero.motion.rot.f[2]);
            pitch = fast_add_rotations(pitch, -1.5707964f);
            build_spherical_offset(v, -3.0f, yaw, pitch);
            fast_vec_add(scratch_vector, &metal_detector_vars->pos, v);
            noop_main_callback(&metal_detector_vars->pos, scratch_vector);
            fast_vec_normalize(aim_basis[metal_detector_aim_direction_row_index_gp],
                               v,
                               (f32)metal_detector_aim_direction_normalization_length_gp);
            fast_vec_cross(aim_basis[metal_detector_aim_perpendicular_row_index_gp],
                           aim_basis[metal_detector_aim_direction_row_index_gp],
                           &metal_detector_aim_cross_reference_x_gp);
            fast_vec_normalize(aim_basis[metal_detector_aim_perpendicular_row_index_gp],
                               aim_basis[metal_detector_aim_perpendicular_row_index_gp],
                               (f32)metal_detector_aim_perpendicular_normalization_length_gp);
            fast_vec_cross(aim_basis[metal_detector_aim_perpendicular_cross_direction_row_index_gp],
                           aim_basis[metal_detector_aim_perpendicular_row_index_gp],
                           aim_basis[metal_detector_aim_direction_row_index_gp]);
            aim_basis[3][2] = zero;
            aim_basis[3][1] = zero;
            aim_basis[3][0] = zero;
            aim_basis[3][3] = one;
            build_moby_bone_transform(moby, 2, normalized_bone_transform);
            fast_vec_normalize(normalized_bone_transform[0], normalized_bone_transform[0], one);
            fast_vec_normalize(normalized_bone_transform[1], normalized_bone_transform[1], one);
            fast_vec_normalize(normalized_bone_transform[2], normalized_bone_transform[2], one);
            transpose_rotation_matrix(transposed_bone_rotation, normalized_bone_transform);
            transposed_bone_rotation[3][2] = zero;
            transposed_bone_rotation[3][1] = zero;
            transposed_bone_rotation[3][0] = zero;
            transposed_bone_rotation[3][3] = one;
            multiply_matrix(aim_rotation_product, transposed_bone_rotation, aim_basis);
            matrix_to_quaternion(u, aim_rotation_product);
        } else {
            *(u128 *)scratch_vector = 0;
            build_quaternion_from_axis_rotations(u, scratch_vector);
        }
        qcopy(&metal_detector_vars->unk30, v);
        quaternion_nlerp(D_L00_001E1170, D_L00_001E1170, u, 0.1f);
        build_moby_bone_transform(moby, 3, scratch_vector);
        qcopy(&metal_detector_vars->unk30, scratch_vector);
        if (controller_state_lui.pressed & 0x20) {
            metal_detector_vars->unk24 = 0;
            metal_detector_vars->unk28 = 0;
        }
        if ((controller_state_lui.held & 0x20) &&
            hero.items[0].ready_frames > scale_game_frames(0xF)) {
            if (hero.detector_target_dist < 10.0f) {
                if (voice_is_playing(moby, metal_detector_vars->voice2) == 0 &&
                    metal_detector_vars->voice2 == -1) {
                    metal_detector_vars->voice2 = allocate_voice_for_target_entry(1, 4, moby);
                }
                if (metal_detector_vars->voice2 != -1) {
                    engine_voice_set_pitch(
                        metal_detector_vars->voice2,
                        truncate_float_to_s32(
                            (10.0f - (hero.detector_target_dist + hero.detector_target_dist)) *
                            32767.0f / 60.0f) -
                            0x1FFF);
                }
                {
                    s32 distance_tint_component =
                        truncate_float_to_s32(hero.detector_target_dist * 255.0f / 10.0f);
                    s32 col = (distance_tint_component << 8) | 0xFF000000;

                    moby->glow_color = ((distance_tint_component << 16) | col) | 0xFF;
                }
                target_moby = hero.metal_detector_target;
                if (target_moby != 0 && target_moby->oclass == 0x25D &&
                    target_moby->state != 0xFE && target_moby->state != 0xFD &&
                    hero.detector_target_dist < 1.0f) {
                    tv = (TargetVars2d2ee8 *)target_moby->pvars;
                    while (tv->count > 0) {
                        scratch_vector[0] = random_float_between(1.0f, 10.0f) * frame_time;
                        scratch_vector[1] = random_float_between(-10.0f, 10.0f) * frame_time;
                        scratch_vector[2] = random_float_between(1.0f, 12.0f) * frame_time;
                        transform_vector_by_basis(scratch_vector, scratch_vector, &hero);
                        {
                            Vec4f *target_position = &hero.metal_detector_target->pos;

                            qcopy(target_position, &metal_detector_vars->pos);
                            add_random_offset_to_vector_xyz(target_position, 0.2f);
                        }
                        target_moby = hero.metal_detector_target;
                        spawn_bolt_by_value(
                            target_moby, &target_moby->pos, scratch_vector, 0x13, 5, 0);
                        target_moby = hero.metal_detector_target;
                        qcopy(&target_moby->pos, &metal_detector_vars->pos);
                        if (tv != 0) {
                            if (tv->count > 0) {
                                tv->count -= 5;
                                target_moby = hero.metal_detector_target;
                            }
                            increment_metal_detector_collection_counter((u8 *)target_moby);
                        } else {
                            increment_metal_detector_collection_counter((u8 *)target_moby);
                        }
                    }
                    mark_moby_for_removal(hero.metal_detector_target);
                    hero.metal_detector_target = 0;
                    hero.detector_target_dist = 100000.0f;
                    metal_detector_vars->unk8 = 0x7FFF;
                    allocate_voice_for_target_entry(3, 0, moby);
                    if (metal_detector_vars->voice2 != -1) {
                        VoiceSlot2d2ee8 *e =
                            (VoiceSlot2d2ee8 *)(voice_pool_lui +
                                                metal_detector_vars->voice2 * 0x70);

                        if (e->owner == moby && e->active != 0) {
                            release_voice_slot(metal_detector_vars->voice2);
                        }
                    }
                    metal_detector_vars->voice2 = -1;
                }
            } else {
                moby->glow_color = 0x80808080;
                if (metal_detector_vars->voice0 != -1) {
                    VoiceSlot2d2ee8 *e =
                        (VoiceSlot2d2ee8 *)(voice_pool_lui + metal_detector_vars->voice0 * 0x70);

                    if (e->owner == moby && e->active != 0) {
                        release_voice_slot(metal_detector_vars->voice0);
                    }
                }
                metal_detector_vars->voice0 = -1;
                if (metal_detector_vars->voice1 != -1) {
                    VoiceSlot2d2ee8 *e =
                        (VoiceSlot2d2ee8 *)(voice_pool_lui + metal_detector_vars->voice1 * 0x70);

                    if (e->owner == moby && e->active != 0) {
                        release_voice_slot(metal_detector_vars->voice1);
                    }
                }
                metal_detector_vars->voice1 = -1;
                if (metal_detector_vars->voice2 != -1) {
                    VoiceSlot2d2ee8 *e =
                        (VoiceSlot2d2ee8 *)(voice_pool_lui + metal_detector_vars->voice2 * 0x70);

                    if (e->owner == moby && e->active != 0) {
                        release_voice_slot(metal_detector_vars->voice2);
                    }
                }
                metal_detector_vars->voice2 = -1;
            }
            add_draw_callback(FUN_L00_002d2a90_u, moby);
            return;
        }
        moby->glow_color = 0x80808080;
        if (metal_detector_vars->voice0 != -1) {
            VoiceSlot2d2ee8 *e =
                (VoiceSlot2d2ee8 *)(voice_pool_lui + metal_detector_vars->voice0 * 0x70);

            if (e->owner == moby && e->active != 0) {
                release_voice_slot(metal_detector_vars->voice0);
            }
        }
        metal_detector_vars->voice0 = -1;
        if (metal_detector_vars->voice1 != -1) {
            VoiceSlot2d2ee8 *e =
                (VoiceSlot2d2ee8 *)(voice_pool_lui + metal_detector_vars->voice1 * 0x70);

            if (e->owner == moby && e->active != 0) {
                release_voice_slot(metal_detector_vars->voice1);
            }
        }
        metal_detector_vars->voice1 = -1;
        if (metal_detector_vars->voice2 != -1) {
            VoiceSlot2d2ee8 *e =
                (VoiceSlot2d2ee8 *)(voice_pool_lui + metal_detector_vars->voice2 * 0x70);

            if (e->owner == moby && e->active != 0) {
                release_voice_slot(metal_detector_vars->voice2);
            }
        }
        metal_detector_vars->voice2 = -1;
        break;
    }
}

int               D_L01_00161AA4 = 0xffffff;
int               D_L01_00161AA8 = 8;
u8 D_L01_001E1610 NOT_SDA = 0;
int               D_L02_00161A14 = 0xffffff;
int               D_L02_00161A18 = 8;
u8 D_L02_001D32E0 NOT_SDA = 0;
int               D_L03_00161944 = 0xffffff;
int               D_L03_00161948 = 8;
u8 D_L03_001D2D40 NOT_SDA = 0;
int               D_L04_00161994 = 0xffffff;
int               D_L04_00161998 = 8;
u8 D_L04_001D2F40 NOT_SDA = 0;
int               D_L05_00161A24 = 0xffffff;
int               D_L05_00161A28 = 8;
u8 D_L05_001D30E0 NOT_SDA = 0;
int               D_L06_00161914 = 0xffffff;
int               D_L06_00161918 = 8;
u8 D_L06_001D49F0 NOT_SDA = 0;
int               D_L07_001618E4 = 0xffffff;
int               D_L07_001618E8 = 8;
u8 D_L07_001D7550 NOT_SDA = 0;
int               D_L08_00161D74 = 0xffffff;
int               D_L08_00161D78 = 8;
u8 D_L08_001DB4D0 NOT_SDA = 0;
int               D_L09_001619A4 = 0xffffff;
int               D_L09_001619A8 = 8;
u8 D_L09_002008F0 NOT_SDA = 0;
int               D_L10_00161904 = 0xffffff;
int               D_L10_00161908 = 8;
u8 D_L10_001D46F0 NOT_SDA = 0;
int               D_L11_00161A74 = 0xffffff;
int               D_L11_00161A78 = 8;
u8 D_L11_001D3680 NOT_SDA = 0;
int               D_L12_00161AD4 = 0xffffff;
int               D_L12_00161AD8 = 8;
u8 D_L12_001F5C90 NOT_SDA = 0;
int               D_L13_001619D4 = 0xffffff;
int               D_L13_001619D8 = 8;
u8 D_L13_001D36B0 NOT_SDA = 0;
int               D_L14_00161BB4 = 0xffffff;
int               D_L14_00161BB8 = 8;
u8 D_L14_001DFCF0 NOT_SDA = 0;
int               D_L15_00161B74 = 0xffffff;
int               D_L15_00161B78 = 8;
u8 D_L15_001D36C0 NOT_SDA = 0;
int               D_L16_00161A24 = 0xffffff;
int               D_L16_00161A28 = 8;
u8 D_L16_001D31E0 NOT_SDA = 0;
int               D_L17_00161A04 = 0xffffff;
int               D_L17_00161A08 = 8;
u8 D_L17_001D3940 NOT_SDA = 0;
int               D_L18_001619C4 = 0xffffff;
int               D_L18_001619C8 = 8;
u8 D_L18_001D36A0 NOT_SDA = 0;
