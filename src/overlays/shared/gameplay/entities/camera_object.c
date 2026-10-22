/* Shared moby classes: camera_object, vendor, bolt, wrench. */
#include "types.h"
#include "rnc/gameplay/entities/moby_class_id.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"

/* per-frame level ambience (wind/sway) state machine: picks a random target, then eases toward it */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A5B20), where it is exact; names translated to the US level program. */

struct Amb {
    float f0;
    float f4;
    float f8;
    short hC;
    short hE;
    float f10;
    float f14;
    float f18;
    short h1C;
    short h1E;
};

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_0015ED6C;
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern struct Amb D_L00_001CB7C0;
float absolute_float(float input) __asm__("func_001F99C0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L00_002a4890(void) {
    enum {
        AMBIENCE_WAIT_AND_SELECT_TARGET = 0,
        AMBIENCE_INTERPOLATE_TO_TARGET = 1
    };
    enum {
        AMBIENCE_WAIT_AND_SELECT_SECOND_TARGET = 2,
        AMBIENCE_INTERPOLATE_TO_SECOND_TARGET = 3
    };
    enum {
        AMBIENCE_HOLD_STATE = 1,
        AMBIENCE_SINGLE_TARGET_CYCLE = 2,
        AMBIENCE_TWO_TARGET_CYCLE = 3
    };
    struct Amb *ambience_state = &D_L00_001CB7C0;
    float interpolation_duration, target_scalar, target_rotation, scalar_magnitude, scalar_clamp_value;
    switch (ambience_state->hC) {
    case AMBIENCE_HOLD_STATE:
        break;
    case AMBIENCE_SINGLE_TARGET_CYCLE:
        switch (ambience_state->hE) {
        case AMBIENCE_WAIT_AND_SELECT_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = convert_integer_to_float(ambience_state->h1C) * random_float_between_alt(0.5f, 1.5f);
                target_rotation = fast_add_rotations(ambience_state->f0, random_float_between_alt(-1.0471976f, 1.0471976f));
                target_scalar = random_float_between_alt(ambience_state->f10 * 0.5f, ambience_state->f10 * 1.5f);
                ambience_state->f8 = fast_subtract_rotations(target_rotation, ambience_state->f4) / interpolation_duration;
                ambience_state->f18 = (target_scalar - ambience_state->f14) / interpolation_duration;
                ambience_state->h1E = func_001FA898_r(interpolation_duration);
                ambience_state->hE = 1;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E = func_001FA898_r(convert_integer_to_float(ambience_state->h1C) *
                                         random_float_between_alt(2.0f, 3.0f));
                ambience_state->hE = 0;
            } else {
                ambience_state->f4 = fast_add_rotations(ambience_state->f4, ambience_state->f8);
                ambience_state->f14 = ambience_state->f14 + ambience_state->f18;
                scalar_magnitude = absolute_float(ambience_state->f14);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->f14 < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->f14 = scalar_clamp_value;
                }
            }
            break;
        }
        break;
    case AMBIENCE_TWO_TARGET_CYCLE:
        switch (ambience_state->hE) {
        case AMBIENCE_WAIT_AND_SELECT_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = multiply_global_factor_ed64(random_float_between_alt(45.0f, 60.0f));
                target_rotation = fast_add_rotations(ambience_state->f0, random_float_between_alt(-1.5707964f, 1.5707964f));
                target_scalar = random_float_between_alt(ambience_state->f10 * 1.5f, ambience_state->f10 + ambience_state->f10);
                ambience_state->f8 = fast_subtract_rotations(target_rotation, ambience_state->f4) / interpolation_duration;
                ambience_state->f18 = (target_scalar - ambience_state->f14) / interpolation_duration;
                ambience_state->h1E = func_001FA898_r(interpolation_duration);
                ambience_state->hE = 1;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E = func_001FA898_r(convert_integer_to_float(ambience_state->h1C) *
                                         random_float_between_alt(1.0f, 1.5f));
                ambience_state->hE = 2;
            } else {
                ambience_state->f4 = fast_add_rotations(ambience_state->f4, ambience_state->f8);
                ambience_state->f14 = ambience_state->f14 + ambience_state->f18;
                scalar_magnitude = absolute_float(ambience_state->f14);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->f14 < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->f14 = scalar_clamp_value;
                }
            }
            break;
        case AMBIENCE_WAIT_AND_SELECT_SECOND_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = multiply_global_factor_ed64(random_float_between_alt(45.0f, 60.0f));
                target_rotation = fast_add_rotations(ambience_state->f0, random_float_between_alt(-0.785398f, 0.785398f));
                target_scalar = random_float_between_alt(ambience_state->f10 * 0.8f, ambience_state->f10 * 1.2f);
                ambience_state->f8 = fast_subtract_rotations(target_rotation, ambience_state->f4) / interpolation_duration;
                ambience_state->f18 = (target_scalar - ambience_state->f14) / interpolation_duration;
                ambience_state->h1E = func_001FA898_r(interpolation_duration);
                ambience_state->hE = 3;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_SECOND_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E = func_001FA898_r(convert_integer_to_float(ambience_state->h1C) *
                                         random_float_between_alt(3.0f, 5.0f));
                ambience_state->hE = 0;
            } else {
                ambience_state->f4 = fast_add_rotations(ambience_state->f4, ambience_state->f8);
                ambience_state->f14 = ambience_state->f14 + ambience_state->f18;
                scalar_magnitude = absolute_float(ambience_state->f14);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->f14 < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->f14 = scalar_clamp_value;
                }
            }
            break;
        }
        break;
    }
}
extern int D_L00_00161390;
extern int D_L00_00161368 __attribute__((sda));
extern int D_L00_0016136C __attribute__((sda));
extern int D_L00_00161370 __attribute__((sda));
extern int D_L00_0016137C_gp __asm__("D_L00_0016137C") __attribute__((sda));
extern int D_L00_00161380 __attribute__((sda));
extern int D_L00_00161388;
extern float D_L00_00161374_gp __asm__("D_L00_00161374") __attribute__((sda));
extern float D_L00_00161378 __attribute__((sda));
extern float D_L00_00161384_gp __asm__("D_L00_00161384") __attribute__((sda));
extern float D_L00_0016138C_gp __asm__("D_L00_0016138C") __attribute__((sda));
#include "rnc/globals.h"
int scale_game_frames(int) __asm__("FUN_001f96f8");
float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
void FUN_L00_002a4cd8(unsigned char *state_object) {
    enum {
        INITIALIZE_FRAME_TIMING = 0,
        UPDATE_WRAPPED_VALUE = 1
    };
    float one;
    int scaled_frame_count;
    switch (state_object[0x20]) {
    case INITIALIZE_FRAME_TIMING:
        D_L00_00161390 = 0;
        state_object[0x20] = 1;
        state_object[0x30] = 0xFF;
        one = 1.0f;
        D_L00_0016137C_gp = scale_game_frames(D_L00_00161368);
        scaled_frame_count = scale_game_frames(D_L00_0016136C);
        D_L00_00161380 = scaled_frame_count;
        D_L00_00161384_gp = one / convert_integer_to_float(scaled_frame_count);
        scaled_frame_count = scale_game_frames(D_L00_00161370);
        D_L00_00161388 = scaled_frame_count;
        D_L00_0016138C_gp = one / (float)scaled_frame_count;
        break;
    case UPDATE_WRAPPED_VALUE:
        D_L00_00161374_gp -= D_L00_00161378 * frame_scale;
        if (D_L00_00161374_gp <= -8.0f)
            D_L00_00161374_gp += 8.0f;
        break;
    }
}
int FUN_L00_002a4dc8(int addend) {
    int previous_value = D_L00_00161390;
    D_L00_00161390 = previous_value + addend;
    return previous_value;
}
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
unsigned char *create_moby(int) __asm__("FUN_0020c4f8");
void FUN_L00_00250df8(unsigned char *);
/* Creates a class 0x3EF moby at v and runs FUN_L00_00250df8 on it. */
unsigned char *create_camera_collision_moby(u128 *spawn_position) __asm__("FUN_L00_002a4de0");

unsigned char *create_camera_collision_moby(u128 *spawn_position) {
    unsigned char *spawned_moby = create_moby(0x3ef);
    if (spawned_moby) {
        spawned_moby[0x30] = 0xff;
        *(unsigned short *)(spawned_moby + 0x34) |= 0x41;
        qcopy(spawned_moby + 0x10, spawn_position);
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
typedef struct {
    char p0[0x98];
    float f98;
    char p1[0x164 - 0x9C];
    float f164;
    char p2[0x2080 - 0x168];
    char *obj;
    char p3[0x208C - 0x2084];
    int w208C;
} G_2a4e48;
typedef struct {
    char p0[0x158];
    float f158;
    char p1[0x180 - 0x15C];
    char *c;
} C_2a4e48;
extern C_2a4e48 D_166C80_2a4e48 __asm__("D_L00_00166C80") __attribute__((section(".data")));
extern float D_0015ED6C_2a4e48 __asm__("D_0015ED6C");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern float dot_vectors_xyz(void *, void *) __asm__("FUN_001f9ab0");
extern void f9a68_2a4e48(void *, void *, float) __asm__("FUN_001f9a68");
extern float vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");

void camera_object_update(char *o) __asm__("FUN_L00_002a4e48");
void camera_object_update(char *camera_object) {
    OvlQuad difference_vector[1];
    OvlQuad scaled_vector[1];
    C_2a4e48 *C = &D_166C80_2a4e48;
    char *object_position;
    char *pos;
    G_2a4e48 *P;
    float dot_product_or_threshold;

    if (C->c != 0) {
        object_position = camera_object + 0x10;
        qcopy(object_position, C->c + 0x30);
        pos = (char *)&hero.motion.pos;
        subtract_vector_xyz(difference_vector, object_position, pos);
        P = (G_2a4e48 *)(pos - 0x80);
        dot_product_or_threshold = dot_vectors_xyz(difference_vector, P->obj + 0xE0);
        f9a68_2a4e48(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
        subtract_vector_xyz(difference_vector, difference_vector, scaled_vector);
        if (absolute_float(dot_product_or_threshold) < 2.5f) {
            dot_product_or_threshold = 0.1f;
            if (vector_length_xyz(difference_vector) < dot_product_or_threshold || fast_difference_between_rotations(P->f98, C->f158) < 2.3561945f ||
                P->f164 < D_0015ED6C_2a4e48 * dot_product_or_threshold) {
                *(float *)(camera_object + 0x18) = 15.0f;
                *(float *)(camera_object + 0x14) = 15.0f;
                *(float *)(camera_object + 0x10) = 15.0f;
            } else {
                dot_product_or_threshold = dot_vectors_xyz(object_position, P->obj + 0xE0);
                f9a68_2a4e48(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
                subtract_vector_xyz(difference_vector, object_position, scaled_vector);
                dot_product_or_threshold = dot_vectors_xyz(pos, P->obj + 0xE0);
                if (P->w208C != 0x11) {
                    f9a68_2a4e48(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
                    add_vector_xyz(object_position, difference_vector, scaled_vector);
                }
            }
        }
    } else {
        *(float *)(camera_object + 0x18) = 5.0f;
        *(float *)(camera_object + 0x14) = 5.0f;
        *(float *)(camera_object + 0x10) = 5.0f;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002a5040.s", FUN_L00_002a5040);
#else
extern float D_L00_00161394 __attribute__((sda));
extern float D_L00_00161398 __attribute__((sda));
extern float D_L00_001D7360[][4];
extern float D_L00_001D73F0[][2];
extern float D_L00_001D73F4[][2];
/* Fakematch: a second name for the table makes reload re-materialise its address as retail does. */
extern float D_L00_001D73F4_lui[][2] __asm__("D_L00_001D73F4");
extern int D_L00_001D7438[];
extern s32 D_L00_001D7460[];
extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");
extern void vu_euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void transform_point_2a5040(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 fast_cos_2a5040(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
void FUN_L00_00263618(void *, int, float, float);

typedef unsigned int Q_2a5040 __attribute__((mode(TI), aligned(16)));
typedef union { Q_2a5040 q; float f[4]; } V_2a5040;
typedef struct {
    float v[4][4];
    int col[4];
    float uv[4][2];
    unsigned long pk[4];
} Quad_2a5040;

void FUN_L00_002a5040(char *m) {
    char *p = *(char **)(m + 0x78);
    Quad_2a5040 qd;
    float q[4][4];
    float d0[4];
    float mtx[3][4];
    V_2a5040 M[4];
    V_2a5040 T[4];
    float off, zf;
    int k, i, i0, i1, i2, i3, col;
    C_2a4e48 *cam;
    float rz;

    cam = &D_166C80_2a4e48;
    rz = cam->f158;
    d0[0] = 0.0f;
    d0[1] = 0.0f;
    d0[2] = rz;
    vu_euler_rotation_basis(mtx, d0);

    D_L00_00161394 = D_L00_00161394 + 0.01f;
    if (1.0f < D_L00_00161394) {
        D_L00_00161394 = D_L00_00161394 - 1.0f;
    }

    qd.pk[2] = 1;
    qd.pk[3] = 0x4000000044UL;
    qd.pk[0] = 0;
    qd.pk[1] = get_effect_texture(0x18);

    for (k = 0; k < 4; k++) {
        float (*uvt)[2] = D_L00_001D73F4;
        float (*uvt2)[2] = D_L00_001D73F4_lui;
        transform_point_2a5040(q[0], D_L00_001D7360[((int (*)[4])D_L00_001D7460)[k][0]], mtx);
        transform_point_2a5040(q[1], D_L00_001D7360[((int (*)[4])D_L00_001D7460)[k][1]], mtx);
        transform_point_2a5040(q[2], D_L00_001D7360[((int (*)[4])D_L00_001D7460)[k][2]], mtx);
        transform_point_2a5040(q[3], D_L00_001D7360[((int (*)[4])D_L00_001D7460)[k][3]], mtx);

        qd.v[0][0] = *(float *)(m + 0x10) + q[0][0] * *(float *)(p + 0x90);
        qd.v[0][1] = *(float *)(m + 0x14) + q[0][1] * *(float *)(p + 0x90);
        qd.v[0][2] = *(float *)(m + 0x18) + q[0][2];
        qd.v[1][0] = *(float *)(m + 0x10) + q[1][0] * *(float *)(p + 0x90);
        qd.v[1][1] = *(float *)(m + 0x14) + q[1][1] * *(float *)(p + 0x90);
        qd.v[1][2] = *(float *)(m + 0x18) + q[1][2];
        qd.v[2][0] = *(float *)(m + 0x10) + q[2][0] * *(float *)(p + 0x90);
        qd.v[2][1] = *(float *)(m + 0x14) + q[2][1] * *(float *)(p + 0x90);
        qd.v[2][2] = *(float *)(m + 0x18) + q[2][2];
        qd.v[3][0] = *(float *)(m + 0x10) + q[3][0] * *(float *)(p + 0x90);
        qd.v[3][1] = *(float *)(m + 0x14) + q[3][1] * *(float *)(p + 0x90);
        qd.v[3][2] = *(float *)(m + 0x18) + q[3][2];

        i0 = ((int (*)[4])D_L00_001D7460)[k][0];
        i1 = ((int (*)[4])D_L00_001D7460)[k][1];
        i2 = ((int (*)[4])D_L00_001D7460)[k][2];
        i3 = ((int (*)[4])D_L00_001D7460)[k][3];
        off = D_L00_00161394;
        qd.col[0] = D_L00_001D7438[i0];
        qd.col[1] = D_L00_001D7438[i1];
        qd.col[2] = D_L00_001D7438[i2];
        qd.col[3] = D_L00_001D7438[i3];
        qd.uv[0][0] = D_L00_001D73F0[i0][0];
        qd.uv[0][1] = uvt[i0][0] + off;
        qd.uv[1][0] = D_L00_001D73F0[i1][0];
        qd.uv[1][1] = uvt[i1][0] + off;
        qd.uv[2][0] = D_L00_001D73F0[i2][0];
        qd.uv[2][1] = uvt2[i2][0] + off;
        qd.uv[3][0] = D_L00_001D73F0[i3][0];
        qd.uv[3][1] = uvt[i3][0] + off;
        draw_geometry_quad(&qd, 0, 1);
    }

    qd.pk[1] = get_effect_texture(0x1B);

    D_L00_00161398 = D_L00_00161398 + 0.01f;
    if (1.0f < D_L00_00161398) {
        D_L00_00161398 = 0.0f;
    }

    {
        float g = D_L00_00161398;
        float n = -g * 1.2f;
        float pp = g * 1.2f;
        float z = g * 1.4f + 1.1f;
        float zz = z - 0.2f;

        T[0].q = 0;
        T[0].f[0] = n * *(float *)(p + 0x90);
        T[0].f[1] = n * *(float *)(p + 0x90);
        T[0].f[2] = z;
        T[1].q = 0;
        T[1].f[0] = pp * *(float *)(p + 0x90);
        T[1].f[1] = n * *(float *)(p + 0x90);
        T[1].f[2] = zz;
        T[2].q = 0;
        T[2].f[0] = n * *(float *)(p + 0x90);
        T[2].f[1] = pp * *(float *)(p + 0x90);
        T[2].f[2] = z;
        T[3].q = 0;
        T[3].f[0] = pp * *(float *)(p + 0x90);
        T[3].f[1] = pp * *(float *)(p + 0x90);
        T[3].f[2] = zz;
        M[0].q = T[0].q;
        M[1].q = T[1].q;
        M[2].q = T[2].q;
        M[3].q = T[3].q;
    }

    /* Fakematch: setting cam a second time hides its base from alias analysis, so the
       yaw load waits for the orbit stores as in retail. */
    cam = &D_166C80_2a4e48;
    rz = cam->f158;
    d0[0] = 0.0f;
    d0[1] = 0.0f;
    d0[2] = rz;
    vu_euler_rotation_basis(mtx, d0);
    transform_point_2a5040(q[0], M[0].f, mtx);
    transform_point_2a5040(q[1], M[1].f, mtx);
    transform_point_2a5040(q[2], M[2].f, mtx);
    transform_point_2a5040(q[3], M[3].f, mtx);

    col = ((int)((1.0f - D_L00_00161398) * 128.0f) << 24) | 0x808080;
    qd.v[0][0] = *(float *)(m + 0x10) + q[0][0];
    qd.v[0][1] = *(float *)(m + 0x14) + q[0][1];
    qd.v[0][2] = *(float *)(m + 0x18) + q[0][2];
    qd.v[1][0] = *(float *)(m + 0x10) + q[1][0];
    qd.v[1][1] = *(float *)(m + 0x14) + q[1][1];
    qd.v[1][2] = *(float *)(m + 0x18) + q[1][2];
    qd.v[2][0] = *(float *)(m + 0x10) + q[2][0];
    qd.v[2][1] = *(float *)(m + 0x14) + q[2][1];
    qd.v[2][2] = *(float *)(m + 0x18) + q[2][2];
    qd.v[3][0] = *(float *)(m + 0x10) + q[3][0];
    qd.v[3][1] = *(float *)(m + 0x14) + q[3][1];
    qd.v[3][2] = *(float *)(m + 0x18) + q[3][2];
    qd.col[0] = col;
    qd.col[1] = col;
    qd.col[2] = col;
    qd.col[3] = col;
    qd.uv[0][0] = 0.0f;
    qd.uv[0][1] = 0.0f;
    qd.uv[1][0] = 1.0f;
    qd.uv[1][1] = 0.0f;
    qd.uv[2][0] = 0.0f;
    qd.uv[2][1] = 1.0f;
    qd.uv[3][0] = 1.0f;
    qd.uv[3][1] = 1.0f;
    draw_geometry_quad(&qd, 0, 1);

    zf = 0.0f;
    for (i = 0; i < 4; i++) {
        float ang;
        /* Fakematch: re-setting zf in the loop keeps the 0.0 in one register for the whole tail. */
        zf = 0.0f;
        ang = (float)i * 3.14159f * 0.5f - 3.14159f;
        T[0].f[0] = fast_cos_2a5040(ang) * 1.1f;
        T[0].f[1] = fast_sin(ang) * 1.1f;
        T[0].f[2] = 0.59f;
        transform_point_2a5040(T[0].f, T[0].f, m + 0xC0);
        add_vector_xyz(T[0].f, T[0].f, m + 0x10);
        FUN_L00_00263618(T[0].f, *(int *)(m + 0x90) & 0xFFFF0000, 0.1333f, zf);
    }
}
#endif
typedef struct {
    f32 f[4];
} V_2a57a8;
typedef struct {
    u8 p0[0x24];
    f32 f24;
} N_2a57a8;
typedef struct {
    u8 p0[0x10];
    f32 f10, f14, f18;
    u8 p1C[0x24 - 0x1C];
    N_2a57a8 *p24;
    u8 p28[4];
    f32 f2C;
    u8 p30[2];
    u16 h32;
    u16 h34;
    u8 p36[0x73 - 0x36];
    u8 b73;
} M_2a57a8;
typedef struct {
    s32 i0, i4;
    u8 p8[4];
    M_2a57a8 *mC;
    u8 p10[0x90 - 0x10];
    f32 f90;
} Q_2a57a8;
typedef struct {
    u8 p0[0x10];
    f32 f10, f14, f18;
    u8 p1C[4];
    u8 b20;
    u8 p21[0x52 - 0x21];
    u8 b52, b53;
    u8 p54[0x78 - 0x54];
    Q_2a57a8 *q;
    u8 p7C[0x90 - 0x7C];
    s32 i90;
} P_2a57a8;
typedef struct {
    u8 p0[0x80];
    f32 f80, f84;
    u8 p88[0x98 - 0x88];
    f32 f98;
    u8 p9C[0x2084 - 0x9C];
    s32 i2084;
    u8 p2088[4];
    s32 i208C;
    u8 p2090[0x20A4 - 0x2090];
    u8 b20A4;
} G_2a57a8;
extern struct Hero hero_data __asm__("D_0013F350") __attribute__((section(".data")));
extern V_2a57a8 hero_pos __asm__("D_0013F3D0") __attribute__((section(".data")));
extern s32 D_0013CAE4_2a57a8 __asm__("D_0013CAE4") __attribute__((section(".data")));
extern s32 D_L00_0015F5CC_2a57a8 __asm__("D_L00_0015F5CC");
f32 fast_sin(f32) __asm__("FUN_001f9de0");
M_2a57a8 *FUN_0020c4f8_2a57a8(s32) __asm__("FUN_0020c4f8");
void attach_manipulator(M_2a57a8 *, s32, void *) __asm__("FUN_0020cb10");
f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
f32 FUN_001f99c0_2a57a8(f32) __asm__("FUN_001f99c0");
void blend_moby_animation_u(P_2a57a8 *, s32, s32, s32) __asm__("FUN_00212f90");
void FUN_L00_002a5040_2a57a8(void) __asm__("FUN_L00_002a5040");
void enqueue_callback_list_1(void *, P_2a57a8 *) __asm__("FUN_001f4600");
void FUN_L00_001fff28_2a57a8(void *, s32, f32) __asm__("FUN_L00_001fff28");
f32 FUN_001fa580_2a57a8(f32, f32) __asm__("FUN_001fa580");
void upd_a(M_2a57a8 *) __asm__("FUN_L00_00250df8");
f32 angle_atan2(f32, f32) __asm__("FUN_001f9e90");
f32 FUN_001fa688_2a57a8(f32, f32) __asm__("FUN_001fa688");
s32 try_set_help_message(s32, s32) __asm__("FUN_00215130");
void FUN_L00_0029a6b0_2a57a8(P_2a57a8 *) __asm__("FUN_L00_0029a6b0");

extern f32 D_L00_001613A4 __attribute__((sda));
extern f32 D_L00_0016139C __attribute__((sda));
extern f32 D_L00_001613A0 __attribute__((sda));
void vendor_update(P_2a57a8 *p) __asm__("FUN_L00_002a57a8");
void vendor_update(P_2a57a8 *vendor_moby) {
    enum {
        VENDOR_INITIALIZE = 0,
        VENDOR_WAIT_FOR_HERO = 1,
        VENDOR_HERO_NEARBY = 2,
        VENDOR_ZERO_AUXILIARY_SCALE = 3
    };
    Q_2a57a8 *q = vendor_moby->q;
    s32 i, interaction_allowed, help_message_allowed, help_message_succeeded;
    u8 *v;
    f32 hero_distance, z;

    D_L00_001613A4 += 0.05f;
    if (D_L00_001613A4 > 3.14f)
        D_L00_001613A4 = -3.14f;
    vendor_moby->i90 =
        (((s32)(fast_sin(D_L00_001613A4) * 48.0f) + 0x60) * 0x10101) | 0x80000000;
    switch (vendor_moby->b20) {
    case VENDOR_INITIALIZE:
        vendor_moby->b20 = 1;
        q->i0 = 0;
        q->i4 = 0;
        q->mC = FUN_0020c4f8_2a57a8(0x477);
        q->mC->h34 |= 2;
        q->mC->h32 = 0x40;
        q->mC->f10 = vendor_moby->f10;
        q->mC->f14 = vendor_moby->f14;
        q->mC->f18 = vendor_moby->f18 + 2.95f;
        q->mC->b73 = 0x20;
        q->f90 = 0.0f;
        for (i = 0; i < 2; i++) {
            attach_manipulator(q->mC, i, (u8 *)q + 0x10 + i * 0x40);
        }
        upd_a(q->mC);
        break;
    case VENDOR_WAIT_FOR_HERO:
        if (q->f90 > 0.0f)
            q->f90 -= 0.1f;
        if (q->f90 <= 0.0f)
            q->mC->h34 |= 1;
        if (!(D_L00_0015F5CC_2a57a8 & 7) &&
            vector_distance(&vendor_moby->f10, &hero_pos) <= 16.0f &&
            FUN_001f99c0_2a57a8(vendor_moby->f18 - hero_pos.f[2]) <= 8.0f) {
            vendor_moby->b20 = 2;
            if (vendor_moby->b53 != 1)
                blend_moby_animation_u(vendor_moby, 1, 0, 10);
            q->mC->h34 &= 0xFFFE;
        }
        enqueue_callback_list_1(FUN_L00_002a5040_2a57a8, vendor_moby);
        FUN_L00_001fff28_2a57a8((u8 *)q + 0x20, 2, D_L00_0016139C);
        FUN_L00_001fff28_2a57a8((u8 *)q + 0x60, 2, D_L00_001613A0);
        D_L00_0016139C = FUN_001fa580_2a57a8(D_L00_0016139C, 0.01f);
        D_L00_001613A0 = FUN_001fa580_2a57a8(D_L00_001613A0, -0.02f);
        q->mC->f2C = q->f90 * q->mC->p24->f24 * 2.5f;
        upd_a(q->mC);
        break;
    case VENDOR_HERO_NEARBY:
        if (q->f90 < 1.0f)
            q->f90 += 0.1f;
        hero_distance = vector_distance(&vendor_moby->f10, &hero_pos);
        z = FUN_001f99c0_2a57a8(vendor_moby->f18 - hero_pos.f[2]);
        if (!(D_L00_0015F5CC_2a57a8 & 7) && (hero_distance > 18.0f || z > 10.0f)) {
            vendor_moby->b20 = 1;
            if (vendor_moby->b53)
                blend_moby_animation_u(vendor_moby, 0, 0, 10);
            break;
        }
        enqueue_callback_list_1(FUN_L00_002a5040_2a57a8, vendor_moby);
        interaction_allowed = 1;
        help_message_allowed = 1;
        FUN_L00_001fff28_2a57a8((u8 *)q + 0x20, 2, D_L00_0016139C);
        FUN_L00_001fff28_2a57a8((u8 *)q + 0x60, 2, D_L00_001613A0);
        D_L00_0016139C = FUN_001fa580_2a57a8(D_L00_0016139C, 0.01f);
        D_L00_001613A0 = FUN_001fa580_2a57a8(D_L00_001613A0, -0.02f);
        q->mC->f2C = q->f90 * q->mC->p24->f24 * 2.5f;
        upd_a(q->mC);
        if (hero_data.state.control_mode != 0 && hero_data.state.current != 3 && hero_data.state.control_mode != 1) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (hero_data.state.current == 0x1D || hero_data.state.current == 0x32) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (hero_data.vehicle_mode) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (vendor_moby->b52 != 1)
            interaction_allowed = 0;
        if (hero_distance > 4.0f || z > 2.0f) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (FUN_001fa688_2a57a8(hero_data.motion.rot.f[2], angle_atan2(vendor_moby->f10 - hero_data.motion.pos.f[0],
                                                                   vendor_moby->f14 - hero_data.motion.pos.f[1])) >
            1.5707964f) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        help_message_succeeded = 0;
        if (help_message_allowed)
            help_message_succeeded = try_set_help_message(1, 0x53E3) != 0;
        if ((D_0013CAE4_2a57a8 & 0x10) && interaction_allowed && help_message_succeeded) {
            FUN_L00_0029a6b0_2a57a8(vendor_moby);
            vendor_moby->b20 = 3;
        }
        break;
    case VENDOR_ZERO_AUXILIARY_SCALE:
        if (q->mC) {
            q->f90 = 0.0f;
            q->mC->f2C = q->mC->p24->f24 * q->f90 * 2.5f;
            upd_a(q->mC);
        }
        break;
    }
}
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/gameplay/hero.h"
#include "rnc/overlay/collision.h"

typedef struct {
    u8 pad0[0x24];
    f32 scale; /* 0x24 */
} BoltClass;

struct Bolt;

typedef struct {
    Vec4 pos;            /* 0x00 */
    Vec4 rot;            /* 0x10 */
    Vec4 vel;            /* 0x20 */
    Vec4 spin;           /* 0x30 */
    u8 pad40[8];
    f32 unk48;           /* 0x48 */
    f32 unk4C;           /* 0x4C */
    f32 unk50;           /* 0x50 */
    u8 landed;           /* 0x54 */
    u8 unk55;            /* 0x55 */
    u8 hop_frames;       /* 0x56 */
    u8 hop_left;         /* 0x57 */
    f32 unk58;           /* 0x58 */
    void *ground_moby;   /* 0x5C */
    struct Bolt *unk60;  /* 0x60 */
    f32 speed;           /* 0x64 */
    f32 ground_z;        /* 0x68 */
    s16 slot;            /* 0x6C */
    s16 slot_timer;      /* 0x6E */
} BoltVars;

typedef struct Bolt {
    u8 pad0[0x10];
    Vec4 pos;            /* 0x10 */
    u8 state;            /* 0x20 */
    u8 pad21[3];
    BoltClass *pclass;   /* 0x24 */
    u8 pad28[4];
    f32 scale;           /* 0x2C */
    u8 unk30;            /* 0x30 */
    u8 unk31;            /* 0x31 */
    u16 unk32;           /* 0x32 */
    u16 flags;           /* 0x34 */
    u8 pad36[0x3D];
    u8 unk73;            /* 0x73 */
    u8 pad74[4];
    BoltVars *vars;      /* 0x78 */
    u8 pad7C[0x2A];
    s16 oclass;          /* 0xA6 */
    u8 padA8[8];
    u8 unkB0;            /* 0xB0 */
    u8 padB1;
    s16 unkB2;           /* 0xB2 */
    u8 padB4[0xC];
    Vec4 rot;            /* 0xC0 */
    u8 padD0[0x10];
    Vec4 unkE0;          /* 0xE0 */
} Bolt;

typedef struct {
    u8 pad0[0x454];
    u8 killed[1];   /* 0x454: indexed by the moby save id */
} MobyKillTable;

typedef struct {
    Vec4 pos;
    s16 pad10[2];
    s16 moby;            /* 0x14 */
    u8 pad16[0xA];
} BoltTrailSlot;

extern s32 D_L00_0015F5C4;
extern CollisionHit D_L00_00173E40 __attribute__((section(".data")));
extern Vec4 D_L00_001D73D0[];
extern BoltTrailSlot glints[] __asm__("D_L00_0016EA40");
extern u8 *D_L00_0015FFDC;
extern s32 D_L00_0015FFBC;
extern u8 *D_L00_0015FFD8;
extern u8 mission_state_macro[] __asm__("D_L00_0015FC88") __attribute__((section(".sdata")));
extern s32 D_0015ED84;
extern MobyKillTable D_L00_001BA5D0 __attribute__((section(".data")));
extern MobyKillTable D_L00_001BB230 __attribute__((section(".data")));
extern u8 D_0014C050[][16];
extern f32 frame_time_sq __asm__("D_0015ED70");
extern f32 D_0015ED6C;

extern s32 FUN_00213260(s32);
extern f32 FUN_00213308(void);
extern f32 FUN_002132a8(f32, f32);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void lerp_vector(void *, void *, void *, f32) __asm__("FUN_001f9a40");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern f32 FUN_001f99c0(f32);
extern f32 multiply_global_factor_ed64(f32) __asm__("FUN_001f96b0");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void multiply_matrix3x3(void *, void *, void *) __asm__("FUN_001fa328");
extern void quaternion_nlerp(void *, void *, void *, f32) __asm__("FUN_001fa400");
extern void quaternion_to_rotation_basis(void *, void *) __asm__("FUN_001fa480");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern s32 FUN_001fa6d0(f32);
extern s32 FUN_001efa68(void *, void *, s32, s32, s32);
extern void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
extern void approach_value(void *, f32, f32) __asm__("FUN_00213ed8");
extern void FUN_0020c828(void *);
extern s32 FUN_L00_001f0d60(void *, s32, void *, f32);
extern void FUN_L00_001ff660(void *, void *, void *);
extern s32 allocate_glint_slot(void *, s32, f32) __asm__("FUN_L00_001fe990");
extern s32 FUN_L00_002603d0(void *);
extern void FUN_L00_00260668(void *, void *, void *, void *, void *, void *);
extern s32 FUN_L00_00257b90(s32, s32);
extern void FUN_L00_00259f50(void *, void *);
extern void FUN_L00_0025f730(void *, f32);
extern void FUN_L00_002a6de8_c(Bolt *, void *, s32) __asm__("FUN_L00_002a6de8");
extern void FUN_L00_002a6b70_c(Bolt *) __asm__("FUN_L00_002a6b70");
extern void FUN_L00_002a7210_c(Bolt *, void *, f32, f32) __asm__("FUN_L00_002a7210");

void bolt_update(Bolt *obj) __asm__("FUN_L00_002a5dd8");
void bolt_update(Bolt *bolt_moby) {
    enum {
        BOLT_INITIALIZE = 0x0,
        BOLT_FALLING = 0x1,
        BOLT_SETTLING = 0x2,
        BOLT_IDLE = 0x3,
        BOLT_MOVE_TO_HERO = 0x4
    };
    BoltVars *v = bolt_moby->vars;
    Vec4 geometry_scratch;
    Vec4 vector_scratch;
    Vec4 rotation_scratch;
    Vec4 collection_offset;
    Vec4 attraction_rotation_angles;
    Vec4 attraction_rotation_basis[3];
    s32 hit;
    f32 sc;
    f32 hr;
    f32 settling_inclination_limit;
    f32 k;
    f32 velocity_decay_step;
    f32 u;
    f32 settling_hop_phase;
    f32 hero_distance;
    f32 x;
    f32 rotation_blend_step;
    s32 flag;
    u8 i;

    if (D_L00_0015F5C4 == 2) {
        bolt_moby->unk31 = 0;
        bolt_moby->flags |= 1;
    } else {
        bolt_moby->unk31 = 1;
        bolt_moby->flags &= 0xFFFE;
    }
    switch (bolt_moby->state) {
    case BOLT_INITIALIZE:
        bolt_moby->state = 3;
        bolt_moby->unk73 = 0x14;
        bolt_moby->flags |= 0x100;
        bolt_moby->unk30 = 0x20;
        bolt_moby->unk32 = 0x20;
        if (FUN_00213260(2) == 0) {
            v->unk55 |= 1;
        }
        fast_vec_normalize(&geometry_scratch, &bolt_moby->unkE0, 0.2f);
        fast_vec_add(&geometry_scratch, &geometry_scratch, &bolt_moby->pos);
        fast_vec_normalize(&vector_scratch, &bolt_moby->unkE0, -3.0f);
        fast_vec_add(&vector_scratch, &vector_scratch, &bolt_moby->pos);
        hit = FUN_001efa68(&geometry_scratch, &vector_scratch, 0x22, 0, 0);
        if (v->landed == 0 && hit != 0) {
            qcopy(&v->pos, &D_L00_00173E40.point);
            clear_u64_value(&v->rot);
            v->rot.f[2] = fast_arc_tan(D_L00_00173E40.normal_x, D_L00_00173E40.normal_y);
            v->rot.f[1] = fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x));
            if (D_L00_00173E40.moby != 0 && FUN_L00_002603d0((void *)D_L00_00173E40.moby) != 0) {
                void *gm = (void *)D_L00_00173E40.moby;
                v->ground_moby = gm;
                FUN_L00_00260668(bolt_moby, gm, v, &v->rot, v, &v->rot);
            }
        } else {
            qcopy(&v->pos, &bolt_moby->pos);
            clear_u64_value(&v->rot);
        }
        v->unk50 = FUN_00213308();
        v->unk58 = FUN_00213308();
        v->ground_z = bolt_moby->pos.f[2];
        v->slot_timer = FUN_L00_00257b90(0, scale_game_frames(600));
        v->slot = -1;
        switch (bolt_moby->oclass) {
        case BOLT_CLASS_ONE:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.65f;
            break;
        case BOLT_CLASS_FIVE:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.7f;
            break;
        case BOLT_CLASS_TWENTY:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.75f;
            break;
        case BOLT_CLASS_FIFTY:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.6f;
            break;
        }
        break;
    case BOLT_FALLING:
        v->vel.f[2] -= frame_time_sq * 10.8f;
        fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &v->vel);
        quaternion_to_rotation_basis(&v->spin, &rotation_scratch);
        multiply_matrix3x3(&bolt_moby->rot, &rotation_scratch, &bolt_moby->rot);
        switch (bolt_moby->oclass) {
        case BOLT_CLASS_ONE:
            hr = 0.15f;
            break;
        case BOLT_CLASS_FIVE:
            hr = 0.23f;
            break;
        case BOLT_CLASS_TWENTY:
            hr = 0.23f;
            break;
        default:
            hr = 0.25f;
            break;
        }
        if (bolt_moby->pos.f[2] < v->ground_z - 2.0f && bolt_moby->pos.f[2] < hero.motion.pos.f[2] - 2.0f) {
            FUN_L00_002a7210_c(bolt_moby, hero.moby, FUN_002132a8(-15.0f, 15.0f) * 0.017453292f, 0.0f);
            return;
        }
        if (FUN_L00_001f0d60(&bolt_moby->pos, 0x22, bolt_moby, hr) == 0) {
            return;
        }
        qcopy(&v->pos, &D_L00_00173E40.point);
        clear_u64_value(&v->rot);
        v->rot.f[2] = fast_arc_tan(D_L00_00173E40.normal_x, D_L00_00173E40.normal_y);
        v->rot.f[1] = fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x));
        fast_vec_sub(&geometry_scratch, &bolt_moby->pos, &D_L00_00173E40.point);
        FUN_L00_001ff660(&v->vel, &v->vel, &geometry_scratch);
        if (vector_distance(&bolt_moby->pos, &hero.motion.pos) < hero.reach_distance &&
            FUN_001f99c0(bolt_moby->pos.f[2] - hero.motion.pos.f[2]) < hero.reach_height) {
            if (hero.health.hp == 0) {
                return;
            }
            fast_vec_normalize(&v->vel, &v->vel, FUN_002132a8(3.0f, 7.0f) * D_0015ED6C);
            FUN_L00_002a7210_c(bolt_moby, hero.moby, 0.0f, 0.0f);
            return;
        }
        settling_inclination_limit = 0.7853982f;
        if (!(fast_arc_tan(geometry_scratch.f[2], vector_length_xy(&geometry_scratch)) < settling_inclination_limit)) {
            return;
        }
        if (!(fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x)) < settling_inclination_limit)) {
            return;
        }
        bolt_moby->state = 2;
        v->hop_frames = v->hop_left = FUN_001fa6d0(multiply_global_factor_ed64(FUN_002132a8(30.0f, 40.0f)));
        if (D_L00_00173E40.moby != 0 && FUN_L00_002603d0((void *)D_L00_00173E40.moby) != 0) {
            void *gm = (void *)D_L00_00173E40.moby;
            v->ground_moby = gm;
            FUN_L00_00260668(bolt_moby, gm, v, &v->rot, v, &v->rot);
        }
        matrix_to_euler(&bolt_moby->rot, &vector_scratch);
        FUN_L00_00259f50(&v->spin, &vector_scratch);
        return;
    case BOLT_SETTLING:
        FUN_L00_002a6de8_c(bolt_moby, &vector_scratch, 0);
        k = 0.25f;
        vector_scratch.f[2] += k;
        matrix_to_euler(&bolt_moby->rot, &geometry_scratch);
        FUN_L00_00259f50(&rotation_scratch, &geometry_scratch);
        rotation_blend_step = 1.0f / convert_integer_to_float(v->hop_left);
        settling_hop_phase = convert_integer_to_float(v->hop_left) / convert_integer_to_float(v->hop_frames);
        quaternion_nlerp(&v->spin, &v->spin, &rotation_scratch, rotation_blend_step);
        lerp_vector(&bolt_moby->pos, &bolt_moby->pos, &vector_scratch, settling_hop_phase);
        settling_hop_phase -= 0.5f;
        bolt_moby->pos.f[2] += -(settling_hop_phase * settling_hop_phase - k) * ((f32)v->hop_frames * 0.025f);
        quaternion_to_rotation_basis(&v->spin, &bolt_moby->rot);
        if (--v->hop_left != 0) {
            return;
        }
        bolt_moby->state = 3;
        v->hop_frames = v->hop_left = FUN_001fa6d0(multiply_global_factor_ed64(FUN_002132a8(10.0f, 20.0f)));
        return;
    case BOLT_IDLE:
        hero_distance = vector_distance(&bolt_moby->pos, &hero.motion.pos);
        if (hero_distance < hero.reach_distance && FUN_001f99c0(bolt_moby->pos.f[2] - hero.motion.pos.f[2]) < hero.reach_height) {
            FUN_L00_002a7210_c(bolt_moby, hero.moby, 0.0f, 0.0f);
            return;
        }
        if ((u8 *)bolt_moby > D_L00_0015FFDC && D_L00_0015FFBC < 200 && 48.0f < hero_distance && bolt_moby->unk31 == 0) {
            FUN_0020c828(bolt_moby);
            return;
        }
        if (bolt_moby->unk31 != 0 || v->ground_moby != 0) {
        FUN_L00_002a6de8_c(bolt_moby, 0, 0);
        if (v->hop_left != 0) {
            u = convert_integer_to_float(v->hop_left) / convert_integer_to_float(v->hop_frames);
            u -= 0.5f;
            bolt_moby->pos.f[2] += -(u * u - 0.25f) * ((f32)v->hop_frames * 0.0125f);
            v->hop_left--;
        }
        if (v->slot >= 0) {
            transform_vector_by_basis(&geometry_scratch, &D_L00_001D73D0[bolt_moby->oclass], &bolt_moby->rot);
            fast_vec_add(&glints[v->slot], &geometry_scratch, &bolt_moby->pos);
            if (glints[v->slot].moby != (((u8 *)bolt_moby - D_L00_0015FFD8) >> 8)) {
                v->slot = -1;
            }
            return;
        }
        if (--v->slot_timer > 0) {
            return;
        }
        transform_vector_by_basis(&geometry_scratch, &D_L00_001D73D0[bolt_moby->oclass], &bolt_moby->rot);
        fast_vec_add(&geometry_scratch, &geometry_scratch, &bolt_moby->pos);
        v->slot = allocate_glint_slot(&geometry_scratch, ((u8 *)bolt_moby - D_L00_0015FFD8) >> 8, 1.4f);
        v->slot_timer = FUN_L00_00257b90(scale_game_frames(300), scale_game_frames(600));
        } else if (v->hop_left != 0) {
            v->hop_left--;
        }
        return;
    case BOLT_MOVE_TO_HERO:
        flag = 0;
        if (v->unk60->oclass == 0) {
            fast_vec_sub(&geometry_scratch, &bolt_moby->pos, &hero.ground_point);
            transform_vector(&geometry_scratch, &geometry_scratch, hero.unk40);
            if (geometry_scratch.f[2] < 0.0f) {
                flag = 1;
            }
        }
        if (flag) {
            fast_vec_normalize(&geometry_scratch, &hero.unk0[8], D_0015ED6C * 7.0f);
            fast_vec_sub(&vector_scratch, &geometry_scratch, &v->vel);
            FUN_L00_0025f730(&vector_scratch, frame_time_sq * 50.0f);
            fast_vec_add(&v->vel, &v->vel, &vector_scratch);
        } else {
            velocity_decay_step = frame_time_sq * 10.0f;
            if (velocity_decay_step < fast_vec_length(&v->vel)) {
                fast_vec_normalize(&geometry_scratch, &v->vel, velocity_decay_step);
                fast_vec_sub(&v->vel, &v->vel, &geometry_scratch);
            }
            approach_value(&v->speed, D_0015ED6C * 48.0f, frame_time_sq * 16.0f);
            x = D_0015ED6C * 8.0f;
            if (x < hero.motion.speed_xy) {
                fast_vec_normalize(&vector_scratch, &hero.motion.displacement, (hero.motion.speed_xy - x) * 0.5f);
                fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &vector_scratch);
            }
        }
        quaternion_to_rotation_basis(&v->spin, &geometry_scratch);
        multiply_matrix3x3(&bolt_moby->rot, &geometry_scratch, &bolt_moby->rot);
        fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &v->vel);
        fast_vec_sub(&collection_offset, &hero.motion.unkD0, &bolt_moby->pos);
        if (v->speed < fast_vec_length(&collection_offset)) {
            if (!(v->unk4C == 0.0f && v->unk48 == 0.0f)) {
                transform_vector_by_basis(&collection_offset, &collection_offset, hero.unk40);
                clear_u64_value(&attraction_rotation_angles);
                attraction_rotation_angles.f[1] = v->unk48;
                attraction_rotation_angles.f[2] = v->unk4C;
                euler_rotation_basis(attraction_rotation_basis, &attraction_rotation_angles);
                transform_vector_by_basis(&collection_offset, &collection_offset, attraction_rotation_basis);
                transform_vector_by_basis(&collection_offset, &collection_offset, hero.unk0);
            }
            fast_vec_normalize(&collection_offset, &collection_offset, v->speed);
            fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &collection_offset);
            return;
        }
        FUN_L00_002a6b70_c(bolt_moby);
        if ((u8 *)bolt_moby < D_L00_0015FFDC) {
            D_L00_001BA5D0.killed[bolt_moby->unkB2] = bolt_moby->unkB0 + 2;
            if (bolt_moby->unkB0 == 0xFF || (mission_state_macro[bolt_moby->unkB0] != 0xFF && D_0014C050[D_0015ED84][bolt_moby->unkB0] == 0xFF)) {
                D_L00_001BB230.killed[bolt_moby->unkB2] = bolt_moby->unkB0 + 2;
            }
        }
        FUN_0020c828(bolt_moby);
        return;
    }
}
/* What a bolt moby's unkB8 points at: byte 0xB1 is its source's per-level id. */
typedef struct {
    u8 pad_00[0xB1];
    u8 id;                         /* 0x80 set: no id */
} BoltSource;

/* Per-level, per-source collection counts. */
typedef struct {
    u16 unk0;
    u16 bolts;
} BoltSourceStats;

extern s32 current_bolt_count __asm__("D_0015ED98");
extern s32 D_0015EE2C;                       /* bolts counted when the bolt's pvars[0x40] is set or it is an early moby */
extern s32 current_level_index __asm__("D_0015ED84") MACRO_ADDR;
/* second name for D_0015ED84: retail loads the level index again here */
extern s32 current_level_index_b __asm__("D_0015ED84") MACRO_ADDR;
extern BoltSourceStats bolt_source_stats[][64] __asm__("D_0014D590");
extern s32 level_bolts[] __asm__("D_0013DF38");  /* bolts collected per level */
extern s32 D_L00_0015F598;
extern void *D_L00_0015FFDC_u __asm__("D_L00_0015FFDC");
extern f32 D_0015ED6C;                       /* frame-rate scale */
extern void FUN_L00_00239cc8(void);          /* bolt counter HUD: init */
extern void FUN_L00_00239d00(void);          /* update */
extern void FUN_L00_00239df8(void);          /* draw */
extern void queue_animation_update(s32, s32, void *, void *, void *, void *, s32) __asm__("FUN_001ff308");
extern void random_vector(void *, f32, f32) __asm__("FUN_L00_00257d78");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
/* spawns a sparkle particle */
extern u8 *FUN_L00_00272060(void *pos, s32 life, f32 size0, f32 size1, s32 color, s32 mode, s32 spin,
                            void *vel, f32 f2);

/*
 * CollectBolt: adds the bolt moby's value (1, or 5 / 20 / 50 for classes
 * 0xE / 0xF / 0x10) to the bolt count, the level's total and its source's
 * stats, shows the bolt counter and spawns two sparkles at the bolt.
 */
void FUN_L00_002a6b70(struct Moby *bolt)
{
    Vec4 sparkle_initial_velocity;
    Vec4 pos;
    s32 value;
    s32 spin;
    f32 k;
    f32 size;
    BoltSource *bolt_source;

    switch (bolt->oclass) {
    default:
    case 0:
        value = 1;
        break;
    case BOLT_CLASS_FIVE:
        value = 5;
        break;
    case BOLT_CLASS_TWENTY:
        value = 20;
        break;
    case BOLT_CLASS_FIFTY:
        value = 50;
        break;
    }

    bolt_source = bolt->parent;
    if (bolt_source && !(bolt_source->id & 0x80))
        bolt_source_stats[current_level_index][bolt_source->id].bolts += value;
    current_bolt_count += value;
    if (bolt->pvars[0x40] || (D_L00_0015F598 && (void *)bolt < D_L00_0015FFDC_u))
        D_0015EE2C += value;
    level_bolts[current_level_index_b] += value;

    k = 0.7f;
    queue_animation_update(2, 0x754E, FUN_L00_00239cc8, FUN_L00_00239d00, FUN_L00_00239df8,
                           &current_bolt_count, 9999999);

    /* two sparkles 10 units out along a random direction, spinning opposite ways */
    random_vector(&sparkle_initial_velocity, D_0015ED6C * k, D_0015ED6C);
    scale_vector_xyz(&pos, &sparkle_initial_velocity, 10.0f);
    add_vector_xyz(&pos, &pos, &bolt->pos);
    spin = random_integer_below(2);
    if (spin == 0)
        spin--;
    size = random_float_between(0.4f, 0.5f);
    FUN_L00_00272060(&pos, scale_game_frames(0x19), size * 0.2f, size, 0x7F207F7F, 0, spin, &sparkle_initial_velocity, 0.0f);
    spin = -spin;
    FUN_L00_00272060(&pos, scale_game_frames(0x19), size * 0.14f, size * k, 0x7F7F7F7F, 1, spin, &sparkle_initial_velocity, 0.0f);
}
#define NOT_SDA

#define MACRO_ADDR

/* Steers a thrown moby along its path: builds its orientation from the data's angles and, when done, releases the held target. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A8088), where it is exact; names translated to the US level program. */

extern char *FUN_L00_002603d0_c(void *) __asm__("FUN_L00_002603d0");
extern float D_0015ED6C;
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int transform_attachment_pose(int x, char *o, float *p, float *q, float *r, float *s) __asm__("FUN_L00_002604f0");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void multiply_matrix3x3(void *, void *, void *) __asm__("FUN_001fa328");
extern void FUN_001f9bf8_u(float *, float *, float) __asm__("FUN_001f9bf8");

void FUN_L00_002a6de8(char *moby, void *out1, void *out2) {
    char *data = *(char **)(moby + 0x78);
    float local_rotation_basis[12];
    float base_rotation_basis[12];
    float local_rotation_angles[4];
    float rotated_position_offset[4];
    float A[4];
    float B[4];
    float D[4];
    float C[4];
    float rotation_angle_step, c2, base_axis_offset_distance, rotated_axis_offset_distance, ground_collision_radius;

    if (*(char **)(data + 0x5C) != 0) {
        transform_attachment_pose((int)moby, *(char **)(data + 0x5C), (float *)data, (float *)(data + 0x10),
                         A, B);
    } else {
        qcopy(A, data);
        qcopy(B, data + 0x10);
    }
    switch (*(short *)(moby + 0xA6)) {
    case BOLT_CLASS_ONE:
        rotation_angle_step = 0.04f;
        c2 = -0.25f;
        base_axis_offset_distance = 0.04875f;
        rotated_axis_offset_distance = 0.325f;
        ground_collision_radius = 0.15f;
        break;
    case BOLT_CLASS_FIVE:
        rotation_angle_step = 0.03f;
        c2 = -0.5f;
        base_axis_offset_distance = 0.0245f;
        rotated_axis_offset_distance = 0.35f;
        ground_collision_radius = 0.23f;
        break;
    case BOLT_CLASS_TWENTY:
        rotation_angle_step = 0.025f;
        c2 = -0.4f;
        base_axis_offset_distance = 0.05625f;
        rotated_axis_offset_distance = 0.375f;
        ground_collision_radius = 0.23f;
        break;
    default:
        rotation_angle_step = 0.0175f;
        c2 = -0.35f;
        base_axis_offset_distance = 0.135f;
        rotated_axis_offset_distance = 0.3f;
        ground_collision_radius = 0.25f;
        break;
    }
    if (*(unsigned char *)(data + 0x55) & 1) {
        *(float *)(data + 0x50) = fast_add_rotations(*(float *)(data + 0x50), rotation_angle_step);
        *(float *)(data + 0x58) = fast_add_rotations(*(float *)(data + 0x58), -0.0125f);
    } else {
        *(float *)(data + 0x50) = fast_subtract_rotations(*(float *)(data + 0x50), rotation_angle_step);
        *(float *)(data + 0x58) = fast_subtract_rotations(*(float *)(data + 0x58), -0.0125f);
    }
    qcopy(local_rotation_angles, data + 0x50);
    local_rotation_angles[1] = c2;
    euler_rotation_basis(local_rotation_basis, local_rotation_angles);
    euler_rotation_basis(base_rotation_basis, B);
    multiply_matrix3x3(moby + 0xC0, base_rotation_basis, local_rotation_basis);
    fast_vec_scale(moby + 0x10, &base_rotation_basis[8], base_axis_offset_distance);
    fast_vec_scale(rotated_position_offset, moby + 0xC0, rotated_axis_offset_distance);
    fast_vec_add(moby + 0x10, moby + 0x10, rotated_position_offset);
    fast_vec_add(moby + 0x10, moby + 0x10, A);
    if (out1 != 0)
        qcopy(out1, A);
    if (out2 != 0)
        qcopy(out2, B);
    if (*(unsigned char *)(moby + 0x20) == 3) {
        char *ground_moby = *(char **)(data + 0x5C);
        if (ground_moby != 0 && *(unsigned char *)(data + 0x57) == 0) {
            int should_detach_from_ground;
            char *x;
            if ((unsigned char)ground_moby[0x20] == 0xFE ||
                (*(unsigned char **)(data + 0x5C))[0x20] == 0xFD) {
                should_detach_from_ground = 1;
            } else {
                should_detach_from_ground = coll_sphere(ground_collision_radius, moby + 0x10, 0x22, moby) == 0;
            }
            if (should_detach_from_ground) {
                moby[0x20] = 1;
                clear_u64_value(data + 0x30);
                *(float *)(data + 0x3C) = 1.0f;
                x = FUN_L00_002603d0_c(*(char **)(data + 0x5C));
                if (x != 0) {
                    qcopy(data + 0x20, x + 0x10);
                } else {
                    clear_u64_value(data + 0x20);
                }
                FUN_001f9bf8_u(C, &base_rotation_basis[8], D_0015ED6C);
                FUN_001f9bf8_u(D, (float *)(moby + 0xC0), D_0015ED6C);
                fast_vec_add(C, C, D);
                fast_vec_add(data + 0x20, data + 0x20, C);
                fast_vec_add(moby + 0x10, moby + 0x10, data + 0x20);
                *(char **)(data + 0x5C) = 0;
            }
        }
    }
}
extern int D_15F5CC_2a7210 __asm__("D_L00_0015F5CC");
extern float D_15ED6C_2a7210 __asm__("D_0015ED6C");
extern int fr_2a7210(int) __asm__("FUN_001f96f8");
extern void allocate_voice_for_target_entry(int, int, char *) __asm__("FUN_0022da68");
extern float rndf_2a7210(float, float) __asm__("FUN_002132a8");
extern void vu_euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void build_quaternion_from_axis_angle(void *, void *, float) __asm__("FUN_00214530");

extern int D_L00_001613AC __attribute__((sda));
extern float D_L00_001613A8 __attribute__((sda));
int FUN_L00_002a7210(void *ov, void *tv, float attraction_yaw_angle, float attraction_pitch_angle) {
    char *bolt_moby = ov;
    char *target_moby = tv;
    char *bolt_vars = *(char **)(bolt_moby + 0x78);
    int sound_cooldown_frames, last, now;
    float k;
    float m[3][4] __attribute__((aligned(16)));

    if (*(unsigned char *)(bolt_moby + 0x20) == 4) {
        return 0;
    }
    sound_cooldown_frames = fr_2a7210(3);
    last = D_L00_001613AC;
    now = D_15F5CC_2a7210;
    if (last + sound_cooldown_frames < now || now < last) {
        D_L00_001613AC = now;
        allocate_voice_for_target_entry(0, 0x20, bolt_moby);
    } else {
        if (attraction_yaw_angle == 0.0f)
            attraction_yaw_angle = rndf_2a7210(-30.0f, 30.0f) * DEG_TO_RAD;
        if (attraction_pitch_angle == 0.0f)
            attraction_pitch_angle = rndf_2a7210(0.0f, 30.0f) * DEG_TO_RAD;
    }
    if (*(unsigned char *)(bolt_moby + 0x20) == 3) {
        vu_euler_rotation_basis(m, bolt_vars + 0x10);
        normalize_vector_xyz(bolt_vars + 0x20, m[2], rndf_2a7210(3.0f, 7.0f) * D_15ED6C_2a7210);
    }
    {
        *(float *)(bolt_vars + 0x4C) = attraction_yaw_angle;
        *(float *)(bolt_vars + 0x48) = attraction_pitch_angle;
        *(int *)(bolt_vars + 0x64) = 0;
        *(char **)(bolt_vars + 0x60) = target_moby;
        subtract_vector_xyz(m[1], target_moby + 0x10, bolt_moby + 0x10);
        cross_vectors_xyz(m[0], bolt_vars + 0x20, m[1]);
        normalize_vector_xyz(bolt_vars + 0x30, m[0], 1.0f);
        k = rndf_2a7210(D_L00_001613A8 * 0.333000004f, D_L00_001613A8) * DEG_TO_RAD;
        build_quaternion_from_axis_angle(bolt_vars + 0x30, bolt_vars + 0x30, k * D_15ED6C_2a7210);
    }
    *(unsigned char *)(bolt_moby + 0x20) = 4;
    return 1;
}
/* Vars of a bolt moby (classes 0xD..0x10). */
typedef struct {
    u8 pad_00[0x20];
    f32 vel[4] __attribute__((aligned(16)));   /* 0x20 */
    f32 spin[4] __attribute__((aligned(16)));  /* 0x30: per-frame rotation (quaternion) */
    u8 counted;                    /* 0x40: CollectBolt also adds the bolt to D_0015EE2C */
    u8 pad_41[0xF];
    f32 unk50;                     /* random angle */
    u8 pad_54;
    u8 unk55;                      /* bit 0 set at random */
    u8 pad_56[2];
    f32 unk58;                     /* random angle */
    u8 pad_5C[0xC];
    f32 start_z;                   /* 0x68 */
    s16 unk6C;
    s16 unk6E;                     /* random 0 .. 600 frames */
} BoltVars_u;

extern void *D_001413D0 __attribute__((section(".data")));
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");       /* sets the moby colour */
extern void FUN_L00_001ff500(void *, void *, float);

/*
 * Spawns a bolt worth `value` (1, 5, 20 or 50) at `pos` with velocity `vel`,
 * coloured like `src`, spinning about vel x (src - pos). flags & 2 sends it
 * toward D_001413D0 (& 0x10 with a random angle). When no moby is free the
 * value goes straight into the bolt count.
 */
struct Moby *FUN_L00_002a7438(struct Moby *bolt_source, void *pos, void *vel, int flags, int value, int counted)
{
    enum {
        BOLT_VALUE_ONE = 1,
        BOLT_VALUE_FIVE = 5,
        BOLT_VALUE_TWENTY = 20,
        BOLT_VALUE_FIFTY = 50
    };
    f32 dir[4] __attribute__((aligned(16)));
    f32 tmp[4] __attribute__((aligned(16)));
    struct Moby *spawned_bolt;
    BoltVars_u *bolt_vars;
    struct Moby *root;
    u8 *color;
    float scale;
    float spin;
    float angle;

    switch (value) {
    default:
    case BOLT_VALUE_ONE:
        spawned_bolt = (struct Moby *)create_moby(0xD);
        scale = 0.65f;
        break;
    case BOLT_VALUE_FIVE:
        spawned_bolt = (struct Moby *)create_moby(0xE);
        scale = 0.7f;
        break;
    case BOLT_VALUE_TWENTY:
        spawned_bolt = (struct Moby *)create_moby(0xF);
        scale = 0.75f;
        break;
    case BOLT_VALUE_FIFTY:
        spawned_bolt = (struct Moby *)create_moby(0x10);
        scale = 0.6f;
        break;
    }
    if (0 != spawned_bolt) {
        bolt_vars = (BoltVars_u *)spawned_bolt->pvars;
        bolt_vars->counted = counted;
        /* the bolt source is the root of src's unkB8 chain */
        root = bolt_source;
        if (bolt_source != 0) {
            while (root->parent != 0)
                root = root->parent;
        }
        spawned_bolt->parent = root;
        spawned_bolt->unk73 = 20;
        spawned_bolt->flags |= 0x100;
        spawned_bolt->update_dist = 0x40;
        spawned_bolt->draw_dist = 0x40;
        spawned_bolt->visible = 1;
        spawned_bolt->state = 1;
        spawned_bolt->scale = spawned_bolt->pclass->scale * scale;
        euler_rotation_basis(&spawned_bolt->rot_row0, &spawned_bolt->rot);
        if ((bolt_source->flags & 0x20) && (color = *(u8 **)(bolt_source->pvars + 0xC)) != 0)
            set_moby_color(spawned_bolt, color[4], color[5], color[6]);
        else
            spawned_bolt->spawn_frame = bolt_source->spawn_frame;
        bolt_vars->unk6E = random_int_between(0, scale_game_frames(600));
        bolt_vars->unk6C = -1;
        qcopy(&spawned_bolt->pos, pos);
        qcopy(bolt_vars->vel, vel);
        if (random_integer_below(2))
            bolt_vars->unk55 |= 1;
        bolt_vars->unk50 = random_angle_radians();
        bolt_vars->unk58 = random_angle_radians();
        bolt_vars->start_z = spawned_bolt->pos.z;
        spin = random_float_between(0.0f, D_L00_001613A8) * 0.017453292f * D_0015ED6C;
        subtract_vector_xyz(tmp, &bolt_source->pos, &spawned_bolt->pos);
        cross_vectors_xyz(dir, bolt_vars->vel, tmp);
        FUN_L00_001ff500(bolt_vars->spin, dir, 1.0f);
        build_quaternion_from_axis_angle(bolt_vars->spin, bolt_vars->spin, spin);
        if (flags & 2) {
            angle = 0.0f;
            if (flags & 0x10)
                angle = random_float_between(-30.0f, 30.0f) * 0.017453292f;
            FUN_L00_002a7210(spawned_bolt, D_001413D0, angle, 0.0f);
        }
        FUN_L00_00250df8((u8 *)spawned_bolt);
    } else {
        current_bolt_count += value;
    }
    return spawned_bolt;
}
#include "qcopy.h"
typedef int q_2a7780 __attribute__((mode(TI)));
typedef struct {
    s16 sel[4];
    u32 col[4][4];
    f32 f48;
    q_2a7780 *verts;
    s16 (*idx)[4][2];
    f32 *uv;
    s16 n;
    f32 f5C;
} E_2a7780;
typedef struct {
    q_2a7780 m[3];
    q_2a7780 m30;
    u8 pad[0x30];
    f32 f70;
    u8 pad2[4];
    s16 h78;
} R_2a7780;
typedef struct { u8 pad[0x78]; R_2a7780 *r; } O_2a7780;
typedef struct {
    u8 pad[0x2080];
    u8 *p2080;
} P_2a7780;
typedef struct {
    q_2a7780 pos[4];
    u32 col[4];
    f32 uv[4][2];
    u64 tag[4];
} V_2a7780;
extern P_2a7780 D_0013F350_2a7780 __asm__("D_0013F350");
extern int frame_2a7780 __asm__("D_L00_0015F5CC");
extern int FUN_001f44b8_2a7780(int) __asm__("FUN_001f44b8");
extern s32 FUN_001fa6d0_2a7780(f32) __asm__("FUN_001fa6d0");
extern void FUN_001f9a68_2a7780(void *, void *, f32) __asm__("FUN_001f9a68");
extern void FUN_001f7d30_2a7780(void *, void *, int) __asm__("FUN_001f7d30");
#define P_2a7780_G D_0013F350_2a7780

extern E_2a7780 effects[] __asm__("D_L00_001D92E8");
extern int last_frame __asm__("D_L00_001613C0");
extern f32 uv_scale __asm__("D_L00_001613BC") __attribute__((sda));
/* Draws the animated quad-strip effect attached to o; the effect set is chosen by state 0x17..0x19 / 0x2B. */
void FUN_L00_002a7780(O_2a7780 *effect_owner) {
    R_2a7780 *r;
    E_2a7780 *effect_config;
    V_2a7780 quad;
    s32 sel0, sel1, sel2, sel3;
    u32 color;
    f32 scale, alpha;
    s32 effect_texture_id, idx, nv; s32 k;
    s32 i, j, elapsed_frames, current_frame;
    f32 effect_progress, uv_v_offset;

    r = effect_owner->r;
    if (r == 0) return;
    if (r->f70 == 0.0f) return;
    if (r->f70 == 1.0f) {
        if (P_2a7780_G.p2080[0x52] != P_2a7780_G.p2080[0x53]) { r->f70 = 0.0f; return; }
        if (P_2a7780_G.p2080[0x52] == 0xFF) { r->f70 = 0.0f; return; }
    }
    scale = 1.0f;
    effect_texture_id = 0x14;
    nv = 1;
    color = 0x808080;
    alpha = scale;
    if (!((u32)(P_2a7780_G.p2080[0x52] - 0x17) < 3 || P_2a7780_G.p2080[0x52] == 0x2B)) k = r->h78;
    else k = P_2a7780_G.p2080[0x52];
    if (k == 0x17) { r->h78 = k; idx = 0; }
    else if (k == 0x18) { r->h78 = k; idx = 1; }
    else if (k == 0x19) { r->h78 = k; idx = 2; }
    else {
        r->h78 = 0x2B;
        alpha = 0.7f;
        idx = 3;
        scale = 1.25f;
        effect_texture_id = 0xC;
        nv = 2;
        color = 0x408080;
    }
    effect_progress = r->f70;
    effect_config = &effects[idx];
    sel0 = effect_config->sel[0];
    sel1 = effect_config->sel[1];
    sel2 = effect_config->sel[2];
    sel3 = effect_config->sel[3];
    if (effect_progress == 1.0f) {
        u8 *hero_moby = P_2a7780_G.p2080;
        qcopy(&r->m[0], hero_moby + 0xC0);
        qcopy(&r->m[1], hero_moby + 0xD0);
        qcopy(&r->m[2], hero_moby + 0xE0);
        last_frame = frame_2a7780;
    }
    current_frame = frame_2a7780;
    elapsed_frames = current_frame - last_frame;
    if (elapsed_frames <= 0) elapsed_frames = 1;
    r->f70 = effect_progress + (f32)elapsed_frames * effect_config->f48;
    last_frame = current_frame;
    if ((f32)(effect_config->n * 2) <= r->f70) {
        s32 k2 = P_2a7780_G.p2080[0x52];
        if ((u32)(k2 - 0x17) < 3) return;
        if (k2 != 0x2B) r->f70 = 0.0f;
        return;
    }
    quad.tag[1] = FUN_001f44b8_2a7780(effect_texture_id);
    quad.tag[2] = 0xFF9000000260LL;
    quad.tag[0] = 5;
    quad.tag[3] = (nv << 2) | 0x8000000040LL;
    qcopy(&r->m30, P_2a7780_G.p2080 + 0x10);
    for (i = 0; i < effect_config->n; i++) {
        for (j = 0; j < 4; j++) {
            s32 vi = effect_config->idx[i][j][0];
            f32 *uv;
            if (i == sel0) quad.col[j] = effect_config->col[0][j];
            else if (i == sel1) quad.col[j] = effect_config->col[1][j];
            else if (i == sel2) quad.col[j] = effect_config->col[2][j];
            else if (i == sel3) quad.col[j] = effect_config->col[3][j];
            else quad.col[j] = 0x50808080;
            quad.col[j] = (FUN_001fa6d0_2a7780(((s32)quad.col[j] >> 24) * alpha) << 24) | color;
            FUN_001f9a68_2a7780(&quad.pos[j], &effect_config->verts[vi], scale);
            uv = (f32 *)(effect_config->idx[i][j][1] * 8 + (s32)effect_config->uv);
            uv_v_offset = 1.0f - r->f70 * effect_config->f5C;
            quad.uv[j][0] = (uv[0] - 0.5f) * uv_scale + 0.5f;
            quad.uv[j][1] = (uv[1] - 0.5f) * uv_scale + 0.5f + uv_v_offset;
        }
        FUN_001f7d30_2a7780(&quad, r, 0);
    }
}
#undef P_2a7780_G
typedef struct {
    char p0[0x50];
    u8 b50;
    u8 b51;
    u8 b52;
    u8 b53;
    f32 f54;
} O_002a7c70;
typedef struct {
    char p0[0x198];
    s32 i198;
    char p1[0x2080 - 0x19C];
    O_002a7c70 *p2080;
    s32 i2084;
    char p2[8];
    s32 i2090;
} G_002a7c70;
extern struct Hero hero_data2 __asm__("D_0013F350") __attribute__((section(".data")));
s32 FUN_001f96f8_002a7c70(s32) __asm__("FUN_001f96f8");
void FUN_00212f90_002a7c70(void *, s32, s32, s32) __asm__("FUN_00212f90");
void update_moby_animation_state(void *) __asm__("FUN_0020c880");
void FUN_L00_002a7c70(O_002a7c70 *wrench_moby) {
    struct Hero *hero_context = &hero_data2;
    if (hero_context->state.current == 0x3B) {
        u8 hero_current_animation = ((O_002a7c70 *)hero_context->moby)->b53;
        s32 k = 0xB;
        if (hero_current_animation == 0x40)
            k = 0x13;
        if (hero_current_animation == 0x41)
            k = 0x12;
        if (k == 0x12 || k == 0x13) {
            wrench_moby->b52 = k;
            wrench_moby->b53 = k;
            wrench_moby->f54 = ((O_002a7c70 *)hero_context->moby)->f54;
            wrench_moby->b50 = ((O_002a7c70 *)hero_context->moby)->b50;
            wrench_moby->b51 = ((O_002a7c70 *)hero_context->moby)->b51;
            if (((O_002a7c70 *)hero_context->moby)->b52 != ((O_002a7c70 *)hero_context->moby)->b53)
                wrench_moby->b50 = wrench_moby->b51;
            update_moby_animation_state(wrench_moby);
        } else {
            FUN_00212f90_002a7c70(wrench_moby, k, 0, FUN_001f96f8_002a7c70(7));
        }
    } else if (hero_context->state.prev == 0x3B && hero_context->state_timer == 1) {
        FUN_00212f90_002a7c70(wrench_moby, 1, 0, FUN_001f96f8_002a7c70(0x11));
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A9030), where it is exact; names translated to the US level program. */

extern char *FUN_002141f8_002a7d90(void *) __asm__("FUN_002141f8");
extern void *D_L00_00173E58_002a7d90 __asm__("D_L00_00173E58") __attribute__((section(".data")));

/* True when the record for D_L00_00173E58 (suck_get_aim_height) has byte 9 equal to 1. */
int get_wrench_hit_sound_variant(void) __asm__("FUN_L00_002a7d90");

int get_wrench_hit_sound_variant(void) {
    int record_byte_9_is_one = 0;
    char *x = 0;
    if (D_L00_00173E58_002a7d90 != 0)
        x = FUN_002141f8_002a7d90(D_L00_00173E58_002a7d90);
    if (x != 0)
        record_byte_9_is_one = *(unsigned char *)(x + 9) == 1;
    return record_byte_9_is_one;
}
extern int D_L00_00173E58 __attribute__((section(".data")));
char *FUN_L00_0025c338(int);
/* True when FUN_L00_0025c338(D_L00_00173E58) is set and has flag 2 at 0x1E. */
int should_trigger_wrench_hit_reaction(void) __asm__("FUN_L00_002a7de0");

int should_trigger_wrench_hit_reaction(void) {
    char *flag_record = FUN_L00_0025c338(D_L00_00173E58);
    if (flag_record && (*(unsigned short *)(flag_record + 0x1e) & 2))
        return 1;
    return 0;
}
extern float rndf_2a7e20(float, float) __asm__("FUN_002132a8");
extern int rndi_2a7e20(int) __asm__("FUN_00213260");
extern float rnda_2a7e20(void) __asm__("FUN_00213308");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern float spread_k(float, float) __asm__("FUN_L00_00257c48");
extern int fr_2a7e20(int) __asm__("FUN_001f96f8");
extern void emit_2a7e20(float *, int, int, int, int, void *, float, float,
                        float) __asm__("FUN_L00_00272060");

void FUN_L00_002a7e20(float *sparkle_spawn_position, int emission_pair_count) {
    OvlQuad sparkle_initial_velocity[1];
    OvlQuad old[1];
    int i;
    for (i = 0; i < emission_pair_count; i++) {
        float base_sparkle_size = rndf_2a7e20(0.1f, 2.0f);
        int k = rndi_2a7e20(2);
        float azimuth_angle, elevation_angle;
        if (k == 0)
            k--;
        azimuth_angle = rnda_2a7e20();
        elevation_angle = rndf_2a7e20(0.34906584f, 1.53588974f);
        build_spherical_offset(sparkle_initial_velocity, rndf_2a7e20(0.05f, 0.1f), azimuth_angle, elevation_angle);
        qcopy(old, sparkle_spawn_position);
        sparkle_spawn_position[0] += spread_k(0.0f, 0.15f);
        sparkle_spawn_position[1] += spread_k(0.0f, 0.15f);
        sparkle_spawn_position[2] += spread_k(0.0f, 0.15f);
        emit_2a7e20(sparkle_spawn_position, fr_2a7e20(0x1E), 0x7F207F7F, 0, k, sparkle_initial_velocity, base_sparkle_size * 0.1f, base_sparkle_size, 0.003f);
        k = -k;
        emit_2a7e20(sparkle_spawn_position, fr_2a7e20(0x1E), 0x7F7F7F7F, 1, k, sparkle_initial_velocity, base_sparkle_size * 0.07f, base_sparkle_size * 0.7f, 0.003f);
    }
}
typedef union {
    u128 q;
    f32 f[4];
} V2a8028;
extern V2a8028 g_debug_camera __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern V2a8028 D_L00_00173E60_2a8028 __asm__("D_L00_00173E60") __attribute__((section(".data")));
extern f32 D_L00_00173E40_2a8028[] __asm__("D_L00_00173E40") __attribute__((section(".data")));
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 rnd_2a8028(f32, f32) __asm__("FUN_L00_00257c48");
extern f32 ang_2a8028(f32, f32) __asm__("FUN_001fa580");
extern void dir_2a8028(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern f32 rndf_2a8028(f32, f32) __asm__("FUN_002132a8");
extern void spawn_two_point_effect_particle(void *, void *) __asm__("FUN_L00_0026c6e8");
extern s32 fr_2a8028(s32) __asm__("FUN_001f96f8");
extern s32 rndi_2a8028(s32, s32) __asm__("FUN_L00_00257b90");
extern void spawn_burst(void *, void *, s32, s32, s32, s32, f32) __asm__("FUN_L00_0026cbb0");
extern void FUN_L00_002a7e20_c(void *, s32) __asm__("FUN_L00_002a7e20");
void spawn_wrench_impact_effects(u8 *wrench_moby) __asm__("FUN_L00_002a8028");

void spawn_wrench_impact_effects(u8 *wrench_moby) {
    V2a8028 effect_direction_scratch;
    V2a8028 effect_spawn_position;
    f32 pitch;
    f32 randomized_direction_angle;
    s32 i;
    s32 random_frame_lower_bound;
    subtract_vector_xyz(&effect_direction_scratch, &g_debug_camera, &D_L00_00173E60_2a8028);
    pitch = -angle_atan2(vector_length_xy(&effect_direction_scratch), effect_direction_scratch.f[2]);
    angle_atan2(effect_direction_scratch.f[0], effect_direction_scratch.f[1]);
    for (i = 19; i >= 0; i--) {
        randomized_direction_angle = ang_2a8028(rnd_2a8028(0.0f, 3.1415927f), pitch);
        dir_2a8028(&effect_direction_scratch, 0.2f, ang_2a8028(rnd_2a8028(0.0f, 3.1415927f), pitch), randomized_direction_angle);
        scale_vector_xyz(&effect_spawn_position, &effect_direction_scratch, rndf_2a8028(1.0f, 3.0f));
        add_vector_xyz(&effect_spawn_position, &effect_spawn_position, &D_L00_00173E60_2a8028);
        spawn_two_point_effect_particle(&effect_spawn_position, &effect_direction_scratch);
    }
    for (i = 9; i >= 0; i--) {
        effect_spawn_position.f[0] = D_L00_00173E40_2a8028[8] + rnd_2a8028(0.0f, 0.1f);
        effect_spawn_position.f[1] = D_L00_00173E40_2a8028[9] + rnd_2a8028(0.0f, 0.1f);
        effect_spawn_position.f[2] = D_L00_00173E40_2a8028[10] + rnd_2a8028(0.0f, 0.1f);
        randomized_direction_angle = ang_2a8028(rnd_2a8028(0.0f, 3.1415927f), pitch);
        dir_2a8028(&effect_direction_scratch, 0.05f, ang_2a8028(rnd_2a8028(0.0f, 3.1415927f), pitch), randomized_direction_angle);
        random_frame_lower_bound = fr_2a8028(10);
        spawn_burst(&effect_spawn_position, &effect_direction_scratch, 0x4F007FFF, 0x1FFFFFFF, rndi_2a8028(random_frame_lower_bound, fr_2a8028(20)), 1, 20000.0f);
    }
    if (*(s16 *)(*(u8 **)(wrench_moby + 0x78) + 0x78) == 0x2B) {
        FUN_L00_002a7e20_c(&D_L00_00173E60_2a8028, 10);
    }
}
typedef struct N_2a82c0 {
    char p0[0x20];
    unsigned char type;
    char p1[3];
    char *info;
    struct N_2a82c0 *next;
} N_2a82c0;
extern N_2a82c0 *D_L00_0015FFE4;
extern float FUN_001f9b48(void *, void *);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int FUN_L00_002a7210(void *, void *, float, float);
extern int FUN_L00_0025e3f8(void *);
extern int try_collect_ammo_pickup(void *) __asm__("FUN_L00_002c5be8");

void process_mobies_near_returning_wrench(unsigned char *wrench_moby) __asm__("FUN_L00_002a82c0");

void process_mobies_near_returning_wrench(unsigned char *wrench_moby) {
    N_2a82c0 *scanned_moby = D_L00_0015FFE4;
    struct Hero *hero_context;
    int moby_processing_result;
    if (scanned_moby == 0)
        return;
    hero_context = &hero;
    do {
        if (scanned_moby->type == 0xFE)
            return;
        if (scanned_moby->type == 0xFD)
            return;
        if (scanned_moby && scanned_moby->info && *(short *)(scanned_moby->info + 0x46) == 0x13) {
            if (FUN_001f9b48((char *)scanned_moby + 0x10, wrench_moby + 0x10) < 2.7f) {
                moby_processing_result = FUN_L00_002a7210(scanned_moby, ((void *)hero_context->moby),
                                     random_float_between(-30.0f, 30.0f) * DEG_TO_RAD, 0.0f);
                goto check;
            }
        } else if (FUN_L00_0025e3f8(scanned_moby) && FUN_001f9b48((char *)scanned_moby + 0x10, wrench_moby + 0x10) < 2.7f) {
            moby_processing_result = try_collect_ammo_pickup(scanned_moby);
        check:
            if (moby_processing_result)
                return;
        }
        scanned_moby = scanned_moby->next;
    } while (scanned_moby);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A96B8), where it is exact; names translated to the US level program. */

extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void FUN_001f9bf8_u(float *, float *, float) __asm__("FUN_001f9bf8");

void scale_wrench_collision_vector_by_heading(float *in_out_vector) __asm__("FUN_L00_002a8418");

void scale_wrench_collision_vector_by_heading(float *in_out_vector) {
    float k = 1.8325957f;
    float vector_heading_angle = fast_arc_tan(in_out_vector[0], in_out_vector[1]);
    float clamped_heading_difference = fast_difference_between_rotations(hero.motion.rot.f[2], vector_heading_angle);
    if (clamped_heading_difference > k)
        clamped_heading_difference = k;
    FUN_001f9bf8_u(in_out_vector, in_out_vector, vector_length_xyz(in_out_vector) * ((k - clamped_heading_difference) / k * 0.3f + 0.7f));
}
#include "rnc/gameplay/entities/moby.h"

#include "sda.h"

#include "qcopy.h"

/* Updates normal wrench swings and its thrown/returning collision sweep. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A9768), where it is exact; names translated to the US level program. */

typedef unsigned int WrenchQuad __attribute__((mode(TI)));

typedef float WrenchVec[4] __attribute__((aligned(16)));

typedef struct WrenchMoby WrenchMoby;

typedef struct { char pad0[0x46]; short kind; } WrenchClass;

typedef struct {
    char pad0[0x40]; WrenchVec direction,target;
    float speed,acceleration; int pad68,timer; float hit_blend;
    short animation_count,collision,animation78,pad7A,previous_count,aim_hit;
} WrenchVars;

struct WrenchMoby {
    char pad0[0x10]; WrenchVec position; unsigned char state,pad21[3]; WrenchClass *model;
    char pad28[0x28]; unsigned char frame,frame_end,animation,current_animation;
    float animation_time,animation_scale; char pad5C[0x14]; unsigned char animation_flags;
    char pad71[7]; WrenchVars *data; char pad7C[0x2A]; unsigned short class_id;
    char padA8[0x10]; WrenchMoby *parent;
};

typedef struct {
    char pad0[0x80]; WrenchVec position; char pad90[8]; float yaw; char pad9C[0xA4]; WrenchVec velocity; char pad150[0x48];
    int state_timer; char pad19C[0x854]; WrenchVec sweep_start,sweep_end,previous_start,previous_end;
    char padA30[0x30]; int attack_index,attack_cooldown; float reaction_yaw; char padA6C[0x10];
    unsigned short hit_index; char padA7E[0x12]; float animation_scale; char padA94[8]; int blocked;
    char padAA0[8]; float animation_frame; unsigned char padAAC[4]; int selected_weapon,weapon_animation;
    char padAB8[0x5F2]; unsigned char thrown,pad10AB,wrench_state; char pad10AD[7]; int mode;
    char pad10B8[0xFC8]; WrenchMoby *moby; int state,pad2088,move_mode;
    char pad2090[0x1E]; unsigned char alternate; char pad20AF[4]; unsigned char no_ground_adjust;
    char pad20B4[0x168]; int thrown_sound;
} WrenchHero;

typedef struct { char pad0[0x140]; WrenchVec position; float pad150,pitch,yaw; } PlayerCamera;

typedef struct { int pad0,side; char pad8[0x14]; int first_frame,last_frame; char pad24[8]; } WrenchAttack;

typedef struct { char pad0[0x18]; WrenchMoby *moby; int surface; WrenchVec position; } WrenchHit;

typedef struct { char pad0[0x14]; short hit_index; char pad16[6]; short hit_count; } MobyHitState;

typedef struct {
    char pad0[8]; float force,range; char pad10[8]; unsigned char flags,enabled; unsigned short class_id;
} HitQuery;

typedef union {
    WrenchVec v[8]; WrenchQuad q[8]; HitQuery query;
    struct { WrenchVec pad0; HitQuery query; } shifted;
} WrenchScratch;

extern WrenchAttack attack_records[] __asm__("D_L00_0017BC28");
extern WrenchMoby *D_L00_00177F00[];
extern char *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern char D_0013E533[];
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C MACRO_ADDR;
extern float frame_time_sq __asm__("D_0015ED70") MACRO_ADDR;
extern float frame_time_cube __asm__("D_0015ED74") MACRO_ADDR;
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float approach_value_alt(float*,float,float) __asm__("FUN_00213ed8");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int D_L00_00173E40_u[] __asm__("D_L00_00173E40");
extern int FUN_001efa68(void*,void*,int,int,int);
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern int collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern int FUN_L00_00211870(float);
extern int FUN_L00_00216de8(int,int);
extern int FUN_L00_002223f8(int,int);
extern int sweep_hit_segment(void*,void*,void*,void*,void*,void*,int) __asm__("FUN_L00_00259d08");
extern int FUN_L00_0025e368(char*);
extern int FUN_L00_0025e3b8(int);
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(int*) __asm__("FUN_001f9740");
extern void set_vector_length_xy(void*,void*,float) __asm__("FUN_001f9c48");
extern void FUN_L00_00232640(int);
extern void FUN_L00_002336e8(float*,float*,float);
extern void moby_point_position(void*,int,void*) __asm__("FUN_L00_0024f7c8");
extern void make_hit_descriptor(void*,void*,int,float,void*) __asm__("FUN_L00_00259888");
extern void spawn_nearby_effect(void*,int,int,void*,void*,float) __asm__("FUN_L00_00259bc8");
extern void FUN_L00_0025b6b8(void*,void*,void*,float);
extern void compose_target_euler_rotation(char*,float,float,float) __asm__("FUN_L00_00262480");
extern void update_moby_transform_history(void*,int) __asm__("FUN_L00_00262608");
extern void FUN_L00_00262840(char*);
extern void FUN_L00_002a7780_cb(void) __asm__("FUN_L00_002a7780");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
int get_wrench_hit_sound_variant(void) __asm__("FUN_L00_002a7d90");
int should_trigger_wrench_hit_reaction(void) __asm__("FUN_L00_002a7de0");
void suck_draw_reticle(int a, int b, void *pos, int kind, int mode, float x, float y, float z) __asm__("FUN_L00_001edf60");
void FUN_L00_002a7c70_c(void *m) __asm__("FUN_L00_002a7c70");
void FUN_L00_002a7e20(float *pos, int n);
void spawn_wrench_impact_effects(u8 *o) __asm__("FUN_L00_002a8028");
void process_mobies_near_returning_wrench(unsigned char *m) __asm__("FUN_L00_002a82c0");
void scale_wrench_collision_vector_by_heading(float *v) __asm__("FUN_L00_002a8418");
extern char D_0013CAE0[];
extern char D_00140B00[];
extern char D_001403C0[];
extern PlayerCamera player_camera __asm__("D_L00_00166C80");
extern WrenchHit collision_hit __asm__("D_L00_00173E40");
extern void draw_target_reticle(int,int,float,float,int,int,int,float,int) __asm__("FUN_L00_001edf60");
extern int collide_sphere(void*,float,int,void*,void*) __asm__("FUN_L00_001f2868");

extern char D_0013F350[];
extern unsigned char D_0013E550[];
static __inline__ WrenchHero *wrench_hero(void) { return (WrenchHero*)D_0013F350; }
static __inline__ float *sweep_end(void) { return (float*)(D_0013E533+0x181D); }
static __inline__ unsigned long combo_step(int animation) { return (unsigned char)(animation-15); }
static __inline__ float *hero_velocity(void) { return (float*)(D_0013E533+0xF5D); }
static __inline__ unsigned char *voice_slot(int slot) { return D_0013E550+slot*0x70; }
static __inline__ WrenchHero *sweep_hero(void) { return (WrenchHero*)((char*)sweep_end()-0xA00); }

void wrench_update(WrenchMoby *m) __asm__("FUN_L00_002a84c8");
void wrench_update(WrenchMoby *wrench_moby) {
    enum {
        WRENCH_NORMAL = 0,
        WRENCH_THROW_OUTBOUND = 10,
        WRENCH_THROW_RETURNING = 11
    };
    WrenchVars *wrench_vars=wrench_moby->data;
    WrenchScratch work;
    WrenchHero *player=wrench_hero();
    player->wrench_state=0;
    if (player->mode==3) return;
    switch (wrench_moby->state) {
    case WRENCH_NORMAL: {
        int player_state;
        int anim;
        WrenchMoby *player_moby;
        WrenchAttack *attack;
        int active;
        int reaction;
        float difference,yaw;
        wrench_moby->parent=player->moby;
        player_state=player->state;
        if (player_state==1) {
            if (*(int*)(((char *)&D_0013CAE0))&0xA)
                draw_target_reticle((int)wrench_moby,0xFF917267,1.0f,0.0f,0,0x25,-1,90.0f,4);
            wrench_vars->aim_hit=0;
            build_spherical_offset(work.v[0],10.0f,player_camera.yaw,-player_camera.pitch);
            add_vector_xyz(work.v[0],work.v[0],player_camera.position);
            if (FUN_001efa68(player_camera.position,work.v[0],4,(int)player->moby,0)) {
                wrench_vars->aim_hit=player_state; qcopy(wrench_vars->target,collision_hit.position);
            }
        }
        enqueue_callback_list_1_alt(FUN_L00_002a7780_cb,wrench_moby);
        anim=wrench_moby->current_animation;
        if ((unsigned char)anim!=0) {
            if ((unsigned char)anim==14) {
                if (wrench_moby->animation_flags&2) blend_moby_animation(wrench_moby,1,0,scale_game_frames_alt(5));
                return;
            } else if (wrench_hero()->alternate) {
                if (combo_step(anim)>=2U) blend_moby_animation(wrench_moby,15,0,scale_game_frames_alt(4));
                else if ((wrench_moby->animation_flags&2) && (unsigned char)anim==15)
                    blend_moby_animation(wrench_moby,16,0,scale_game_frames_alt(5));
                return;
            } else if (combo_step(anim)<2U) {
                blend_moby_animation(wrench_moby,14,0,scale_game_frames_alt(2)); return;
            }
        }
        if ((wrench_moby->animation_flags&2) && wrench_hero()->state!=0x3B && wrench_moby->current_animation!=1)
            blend_moby_animation(wrench_moby,1,0,scale_game_frames_alt(5));
        FUN_L00_002a7c70_c((char*)wrench_moby);
        {
        int current_state=wrench_hero()->state;
        if (current_state!=19 && current_state!=33 && current_state!=43 && current_state!=112 && current_state!=20) {
            int current=wrench_moby->current_animation;
            if ((unsigned int)current>=3 && wrench_moby->current_animation<6)
                blend_moby_animation(wrench_moby,1,0,scale_game_frames_alt(4));
        }
        }
        player_moby=wrench_hero()->moby;
        if (player_moby->current_animation!=player_moby->animation || (int)player_moby->frame_end-player_moby->frame>=3)
            wrench_vars->animation_count++;
        wrench_moby->animation_scale=wrench_hero()->animation_scale;
        attack=&attack_records[wrench_hero()->attack_index];
        if (wrench_hero()->move_mode!=6 && wrench_hero()->state!=43) break;
        active=1; reaction=0;
        if (wrench_hero()->blocked || wrench_hero()->animation_frame<(float)attack->first_frame || (float)attack->last_frame<wrench_hero()->animation_frame) active=0;
        {
            int reset=0;
            if (wrench_vars->previous_count!=wrench_vars->animation_count) {
                WrenchMoby *pm=wrench_hero()->moby;
                int player_animation=pm->animation;
                if (player_animation==pm->current_animation) {
                    if (player_animation==23) reset=0.0f<=wrench_hero()->animation_frame;
                    else if (player_animation==24) reset=0.0f<=wrench_hero()->animation_frame;
                    else if (player_animation==25) reset=12.0f<=wrench_hero()->animation_frame;
                    else if (player_animation==43) reset=23.0f<=wrench_hero()->animation_frame;
                }
            }
            if (reset) { wrench_vars->hit_blend=1.0f; wrench_vars->previous_count=wrench_vars->animation_count; }
        }
        difference=0.0f; yaw=wrench_hero()->yaw;
        qcopy(wrench_hero()->previous_end,wrench_hero()->sweep_end);
        qcopy(wrench_hero()->previous_start,wrench_hero()->sweep_start);
        moby_point_position(wrench_moby,1,wrench_hero()->sweep_end);
        moby_point_position(wrench_hero()->moby,0,wrench_hero()->sweep_start);
        if (wrench_hero()->state==19 || wrench_hero()->state==112 || wrench_hero()->state==43) {
            yaw=fast_arc_tan(wrench_hero()->sweep_end[0]-wrench_hero()->position[0],wrench_hero()->sweep_end[1]-wrench_hero()->position[1]);
            if (attack_records[wrench_hero()->attack_index].side==1)
                yaw=fast_add_rotations(yaw,-1.5707964f);
            else yaw=fast_add_rotations(yaw,1.5707964f);
        }
        subtract_vector_xyz(work.v[0],sweep_end(),(sweep_end()-4));
        normalize_vector_xyz(work.v[0],work.v[0],vector_length_xyz(work.v[0])+0.17f);
        if (sweep_hero()->move_mode!=15 && sweep_hero()->state!=112 && sweep_hero()->state!=20) {
            difference=fast_difference_between_rotations(sweep_hero()->yaw,fast_arc_tan(sweep_hero()->sweep_end[0]-sweep_hero()->position[0],sweep_hero()->sweep_end[1]-sweep_hero()->position[1]));
            if (difference>1.5707964f) active=0;
            scale_wrench_collision_vector_by_heading(work.v[0]);
        }
        add_vector_xyz(sweep_end(),(sweep_end()-4),work.v[0]);
        if (sweep_hero()->state!=43 && sweep_hero()->state!=112 && difference<1.2217305f && scale_game_frames_alt(10)<sweep_hero()->state_timer) {
            float angle;
            qcopy(work.v[0],sweep_hero()->position); work.v[0][2]+=0.5f;
            angle=fast_arc_tan(sweep_hero()->sweep_end[0]-sweep_hero()->position[0],sweep_hero()->sweep_end[1]-sweep_hero()->position[1]);
            qcopy(work.v[1],sweep_hero()->position); work.v[1][2]+=0.5f;
            work.v[2][0]=fast_cos(angle)*1.4f; work.v[2][1]=fast_sin(angle)*1.4f; work.v[2][2]=0.0f;
            scale_wrench_collision_vector_by_heading(work.v[2]); add_vector_xyz(work.v[1],work.v[1],work.v[2]);
            if (!sweep_hero()->attack_cooldown && FUN_001efa68(work.v[0],work.v[1],2,0,0) && collision_hit_material()) {
                if (!collision_hit.moby || FUN_L00_0025e368((char*)collision_hit.moby)) {
                    sweep_hero()->attack_cooldown=1; allocate_voice_for_target_entry_alt(2,0,(int)wrench_moby); spawn_wrench_impact_effects((char*)wrench_moby);
                    if (collision_hit.moby) {
                        work.v[3][0]=fast_cos(yaw); work.v[3][1]=fast_sin(yaw); work.v[3][2]=0.0f;
                        spawn_nearby_effect(collision_hit.moby,(int)wrench_moby,0x10000,collision_hit.position,work.v[3],1.0f);
                    }
                }
            }
        }
        if (wrench_hero()->state==20 && FUN_L00_00211870(27.0f)) {
            allocate_voice_for_target_entry_alt(2,0,(int)wrench_moby);
            qcopy(work.v[0],wrench_hero()->sweep_end); qcopy(work.v[1],work.v[0]);
            FUN_L00_002336e8(work.v[0],work.v[0],0.7f); FUN_L00_002336e8(work.v[1],work.v[1],-0.9f);
            if (FUN_001efa68(work.v[0],work.v[1],2,0,0)) spawn_wrench_impact_effects((char*)wrench_moby);
        }
        if (active) {
            int count=1;
            float scale=1.0f,radius;
            if (wrench_hero()->state==20) { scale=1.55f; count=2; }
            work.v[3][0]=fast_cos(yaw); work.v[3][1]=fast_sin(yaw); work.v[3][2]=0.0f;
            make_hit_descriptor(work.v[0],wrench_moby,0x10000,(float)count,work.v[3]); set_vector_length_xy(work.v[0],work.v[0],scale);
            work.query.force=1.0f; work.query.range=5627.925f; work.query.enabled=1; work.query.class_id=wrench_moby->class_id; work.query.flags=0;
            if (sweep_hit_segment(wrench_hero()->sweep_start,wrench_hero()->sweep_end,wrench_hero()->previous_start,wrench_hero()->previous_end,wrench_hero()->moby,&work,5) && collision_hit.moby) {
                if (should_trigger_wrench_hit_reaction()) reaction=1;
                else if (!wrench_hero()->attack_cooldown) {
                    if (!collision_hit.moby || !collision_hit.moby->model || collision_hit.moby->model->kind!=20) {
                        wrench_hero()->attack_cooldown=1; allocate_voice_for_target_entry_alt(get_wrench_hit_sound_variant(),0,(int)wrench_moby); spawn_wrench_impact_effects((char*)wrench_moby);
                    }
                }
            }
            subtract_vector_xyz(work.v[5],sweep_end(),(sweep_end()-4));
            normalize_vector_xyz(work.v[5],work.v[5],vector_length_xyz(work.v[5])-0.085f);
            add_vector_xyz(work.v[4],work.v[5],(sweep_end()-4));
            radius=0.35f;
            if (sweep_hero()->move_mode==15) radius=0.7f;
            if (sweep_hero()->state==20) radius=0.47f;
            if (collide_sphere(work.v[4],radius,0,sweep_hero()->moby,work.v[0]) && (!D_L00_00177F00[0] || !D_L00_00177F00[0]->model || D_L00_00177F00[0]->model->kind!=18)) {
                if (should_trigger_wrench_hit_reaction()) reaction=1;
                else if (!wrench_hero()->attack_cooldown) { wrench_hero()->attack_cooldown=1; allocate_voice_for_target_entry_alt(get_wrench_hit_sound_variant(),0,(int)wrench_moby); spawn_wrench_impact_effects((char*)wrench_moby); }
            }
        }
        if (reaction) {
            FUN_L00_002223f8(33,1);
            if (collision_hit.moby) {
                WrenchMoby *target=collision_hit.moby;
                wrench_hero()->reaction_yaw=fast_arc_tan(wrench_hero()->position[0]-target->position[0],wrench_hero()->position[1]-target->position[1]);
            } else wrench_hero()->reaction_yaw=fast_add_rotations(wrench_hero()->yaw,3.1415927f);
        }
        break;
    }
    case WRENCH_THROW_OUTBOUND: case WRENCH_THROW_RETURNING: {
        int collided;
        float *position;
        float radius;
        {
        char *control=((char *)&D_00140B00);
        WrenchHero *control_player=(WrenchHero*)(control-0x17B0);
        update_moby_transform_history(control,0);
        wrench_moby->parent=control_player->moby; control_player->wrench_state=2;
        if (wrench_moby->state==10 && control_player->thrown_sound==-1) control_player->thrown_sound=allocate_voice_for_target_entry_alt(14,4,(int)control_player->moby);
        }
        compose_target_euler_rotation((char*)wrench_moby,0.0f,0.0f,D_0015ED6C*24.434608f);
        if (wrench_hero()->attack_cooldown && tick_countdown_32_alt(&wrench_vars->timer)) wrench_hero()->attack_cooldown=0;
        position=wrench_moby->position;
        if (!wrench_hero()->no_ground_adjust) {
            float ground=probe_ground_height(position,0,0.5f);
            if (wrench_moby->position[2]-ground<0.8f) {
                ground+=0.55f;
                if (wrench_moby->position[2]<ground) approach_value_alt(&wrench_moby->position[2],ground,D_0015ED6C*4.0f);
                else approach_value_alt(&wrench_moby->position[2],ground,D_0015ED6C);
            }
        }
        qcopy(work.v[0],position);
        if (wrench_moby->state==10) {
            qcopy(work.v[1],hero_velocity());
            scale_vector_xyz(work.v[1],work.v[1],0.7f);
            add_vector_xyz(position,position,work.v[1]);
            normalize_vector_xyz(work.v[1],wrench_vars->direction,wrench_vars->speed); add_vector_xyz(position,position,work.v[1]);
            wrench_vars->acceleration+=frame_time_cube*170.0f;
            approach_value_alt(&wrench_vars->speed,0.0f,wrench_vars->acceleration);
            if (wrench_vars->speed==0.0f) wrench_moby->state=11;
        } else {
            int arrived=0;
            float length,steps;
            process_mobies_near_returning_wrench((char*)wrench_moby);
            subtract_vector_xyz(work.v[1],((char *)&D_001403C0),position);
            wrench_vars->acceleration+=frame_time_sq*0.9f; wrench_vars->speed+=wrench_vars->acceleration;
            length=vector_length_xyz(work.v[1]); steps=wrench_vars->speed; steps=length/steps;
            if (steps<(float)scale_game_frames_alt(5) && wrench_moby->current_animation!=1) blend_moby_animation(wrench_moby,1,0,scale_game_frames_alt(5));
            if (length<=wrench_vars->speed) { wrench_vars->speed=length; arrived=1; }
            normalize_vector_xyz(work.v[1],work.v[1],wrench_vars->speed); add_vector_xyz(position,position,work.v[1]);
            if (length/wrench_vars->speed<(float)scale_game_frames_alt(4) && wrench_hero()->selected_weapon!=-1) {
                int sound;
                FUN_L00_00232640(2); wrench_hero()->weapon_animation=26;
                sound=wrench_hero()->thrown_sound;
                if (sound!=-1) { unsigned char *audio=voice_slot(sound); if (*(WrenchMoby**)(audio+0x88)==wrench_hero()->moby && audio[0x74]) release_voice_slot(sound); }
                wrench_hero()->thrown_sound=-1;
            }
            if (arrived) {
                int sound;
                FUN_L00_00216de8(5,0);
                sound=wrench_hero()->thrown_sound;
                if (sound!=-1) { unsigned char *audio=voice_slot(sound); if (*(WrenchMoby**)(audio+0x88)==wrench_hero()->moby && audio[0x74]) release_voice_slot(sound); }
                wrench_hero()->thrown_sound=-1; FUN_L00_00262840((char*)wrench_hero()+0x17B0); wrench_hero()->thrown=0; wrench_moby->state=0;
            }
        }
        collided=0;
        if (FUN_001efa68(work.v[0],position,2,0,0) && collision_hit_material() && (!collision_hit.moby || !FUN_L00_0025e3b8((int)collision_hit.moby))) collided=collision_hit.surface!=0;
        if (collided || wrench_vars->collision) {
            wrench_vars->collision=0;
            if (!wrench_hero()->attack_cooldown) { wrench_vars->timer=scale_game_frames_alt(30); wrench_hero()->attack_cooldown=1; allocate_voice_for_target_entry_alt(2,0,(int)wrench_moby); }
            if (wrench_moby->state==10) { spawn_wrench_impact_effects((char*)wrench_moby); wrench_vars->speed*=0.5f; wrench_moby->state=11; }
        }
        radius=1.2f; if (wrench_moby->state==11) radius=-0.37f;
        normalize_vector_xyz(work.v[4],wrench_vars->direction,radius);
        make_hit_descriptor(work.v[1],wrench_moby,0x10000,1.0f,work.v[4]);
        work.shifted.query.force=1.0f; work.shifted.query.range=5627.925f;
        work.shifted.query.enabled=1; work.shifted.query.class_id=wrench_hero()->hit_index+1; work.shifted.query.flags=0;
        moby_point_position(wrench_moby,1,work.v[5]);
        if (collide_sphere(work.v[5],0.4f,0,wrench_hero()->moby,work.v[1]) || (moby_point_position(wrench_moby,0,work.v[5]),collide_sphere(work.v[5],0.4f,0,wrench_hero()->moby,work.v[1]))) {
            if (!wrench_hero()->attack_cooldown && collision_hit.moby && (!collision_hit.moby->model || collision_hit.moby->model->kind!=20)) {
                MobyHitState *hit=(MobyHitState*)suck_get_aim_height(collision_hit.moby);
                if (!hit || hit->hit_count<2 || hit->hit_index!=work.shifted.query.class_id) {
                    work.q[7]=*(WrenchQuad*)collision_hit.moby->position;
                    FUN_L00_0025b6b8(work.v[6],work.v[5],work.v[7],0.5f); FUN_L00_002a7e20(work.v[6],5);
                    wrench_vars->timer=scale_game_frames_alt(30); wrench_hero()->attack_cooldown=1; allocate_voice_for_target_entry_alt(get_wrench_hit_sound_variant(),0,(int)wrench_moby);
                }
            }
        }
        break;
    }
    default: break;
    }
}
/* Spawns a homing bolt (class 0x100) from owner at pos, aimed at tgt or at whatever lies along its heading. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AA998), where it is exact; names translated to the US level program. */

typedef float V[4] __attribute__((aligned(16)));

typedef int Q_2aa998 __attribute__((mode(TI)));

typedef union { Q_2aa998 q; float f[4]; } BoltVector_2AA998;

typedef struct { char pad[0x10]; BoltVector_2AA998 position; } BoltOwner_2AA998;

extern V D_L00_00173E60;
extern char *FUN_L00_002630c8(void *, void *, void *, float *, float *, float *, float);
extern char *func_0020D348_m(int);
extern char D_0013F490[];
extern void *D_001413D0;
extern f32 fast_vec_length(void *) __asm__("func_001F9AF0");
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern float D_L00_001613C4 __attribute__((sda));
extern short D_L00_001B0770[];
extern void FUN_L00_00250df8_c(void *) __asm__("FUN_L00_00250df8");
extern void FUN_L00_0025f780(void *, short *, int);

char *FUN_L00_002a96f8(void *owner, void *pos, float a, float b, float c, char *tgt, float rz, float ry) {
    char *m;
    char *d;
    BoltVector_2AA998 t;
    BoltVector_2AA998 p;
    m = (char *)func_0020D348_m(0x100);
    if (m != 0) {
        ((unsigned char *)m)[0x30] = 0xFF;
        m[0x31] = 1;
        m[0x20] = 0;
        *(float *)(m + 0x2C) *= D_L00_001613C4;
        *(short *)(m + 0x32) = 0xFF;
        d = *(char **)(m + 0x78);
        *(float *)(m + 0x48) = rz;
        *(float *)(m + 0x44) = ry;
        if (tgt == 0) {
            build_spherical_offset(&t, D_0015ED6C * 20.0f, rz, ry);
            p.q = *(Q_2aa998 *)pos;
            tgt = FUN_L00_002630c8(owner, &p, &t, &a, &b, &c, 0.34906584f);
        }
        *(void **)(d + 0x10) = owner;
        qcopy(m + 0x10, pos);
        *(float *)(d + 0x34) = D_0015ED6C * 100.0f;
        *(int *)(d + 0x38) = 1;
        *(int *)(d + 0x24) = 0;
        *(int *)(d + 0x28) = 0;
        *(int *)(d + 0x20) = 0;
        *(char **)(d + 0x18) = tgt;
        if (tgt != 0) {
            unsigned char *e = (unsigned char *)suck_get_aim_height(tgt);
            if (e != 0) {
                *(unsigned short *)(e + 0x1E) |= 0x80;
                if (e[0xB] != 0) *(int *)(d + 0x38) = 0;
                if (e[0xC] != 0) *(float *)(d + 0x34) = (float)e[0xC] * D_0015ED6C;
            }
        }
        *(float *)(d + 0x2C) = a;
        *(float *)(d + 0x30) = b;
        *(int *)(d + 0x14) = scale_game_frames_c(0x96);
        if (tgt != 0) {
            qcopy(d, tgt + 0x10);
            *(float *)(d + 8) += c;
            *(float *)(d + 0x1C) = c;
        }
        {
            int r;
            if (fast_vec_length((float *)(D_0013F490)) > D_0015ED6C * 0.1f) {
                r = scale_game_frames_c(0x3C);
                *(short *)(d + 0x3E) = r;
                *(short *)(d + 0x3C) = r;
            } else {
                r = scale_game_frames_c(1);
                *(short *)(d + 0x3E) = r;
                *(short *)(d + 0x3C) = r;
            }
        }
        t.q = ((BoltOwner_2AA998 *)owner)->position.q;
        t.f[2] = ((float *)pos)[2];
        if (FUN_001efa68(&t, pos, 0, *(int *)(((char *)&D_001413D0)), 0) != 0) {
            m[0xBC] = 1;
            qcopy(m + 0x10, &D_L00_00173E60);
        }
        FUN_L00_00250df8_c(m);
        FUN_L00_0025f780(m, D_L00_001B0770, 0x1F);
        *(float *)(m + 0x2C) += *(float *)(m + 0x2C);
    }
    return m;
}
/* Creates a moby at a position with a scale, colour and a few data fields. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAC50), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");

void *spawn_class_593_moby(int unused, void *spawn_position, int initial_paired_value, float scale, float initial_private_scalar, float b) __asm__("FUN_L00_002a99b0");

void *spawn_class_593_moby(int unused, void *spawn_position, int initial_paired_value, float scale, float initial_private_scalar, float b) {
    char *spawned_moby = func_0020D348_m(0x593);
    if (spawned_moby != 0) {
        char *moby_vars;
        float doubled_scale;
        qcopy(spawned_moby + 0x10, spawn_position);
        set_moby_color(spawned_moby, 0x80, 0x80, 0x70);
        moby_vars = *(char **)(spawned_moby + 0x78);
        doubled_scale = scale + scale;
        *(float *)moby_vars = initial_private_scalar;
        *(float *)(spawned_moby + 0x2C) = doubled_scale;
        *(float *)(spawned_moby + 0x2C) = doubled_scale * *(float *)(*(char **)(spawned_moby + 0x24) + 0x24);
        *(int *)(moby_vars + 4) = initial_paired_value;
        *(int *)(moby_vars + 8) = initial_paired_value;
        *(float *)(moby_vars + 0xC) = b;
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        *(unsigned char *)(spawned_moby + 0x31) = 1;
        *(unsigned char *)(spawned_moby + 0x20) = 0;
    }
    return spawned_moby;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAD40), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m_q(int) __asm__("func_0020D348_m");
extern int D_L00_0015F320;
extern int D_L00_0015F330;
extern int FUN_L00_0023e5e0_q(float *pos, float radius, float intensity,
                              int color) __asm__("FUN_L00_0023e5e0");
extern short D_L00_001613C8_q __asm__("D_L00_001613C8") __attribute__((sda));

extern int D_L00_0015F324;
extern int D_L00_0015F334;
extern long D_L00_0015F328;
extern long D_L00_0015F338;
void *FUN_L00_002a9aa0(int unused, float *spawn_position) {
    char *spawned_moby = func_0020D348_m_q(0x5F3);
    if (spawned_moby != 0 && *(int *)&D_L00_001613C8_q == 0) {
        char *moby_vars;
        spawned_moby[0x20] = 0;
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        moby_vars = *(char **)(spawned_moby + 0x78);
        *(OvlQuad *)(spawned_moby + 0x10) = *(OvlQuad *)spawn_position;
        D_L00_0015F324 = 0xFF;
        D_L00_0015F334 = 0xFFFFFF;
        D_L00_0015F338 = 0x1F00000042L;
        *(int *)&D_L00_001613C8_q = (int)spawned_moby;
        D_L00_0015F328 = 0;
        D_L00_0015F320 = 0;
        D_L00_0015F330 = 0;
        *(int *)(moby_vars + 0x10) = 0;
        *(short *)(moby_vars + 0x2C) = FUN_L00_0023e5e0_q(spawn_position, 0.0f, 0.0f, 0);
    }
    return spawned_moby;
}
extern u8 *D_L00_001613C8 __attribute__((sda));
void FUN_L00_002a9aa0_u(void *, OvlQuad *) __asm__("FUN_L00_002a9aa0");
void set_tracked_class_5f3_moby_position(void *unused_creation_arg, OvlQuad *position) __asm__("FUN_L00_002a9b80");

void set_tracked_class_5f3_moby_position(void *unused_creation_arg, OvlQuad *position) {
    OvlQuad position_copy = *position;
    u8 *moby;
    u8 *moby_vars;
    u8 *tracked_moby;
    tracked_moby = D_L00_001613C8;
    if (tracked_moby == 0) {
        FUN_L00_002a9aa0_u(unused_creation_arg, &position_copy);
        tracked_moby = D_L00_001613C8;
        if (tracked_moby == 0)
            return;
    }
    moby = D_L00_001613C8;
    moby_vars = *(u8 **)(moby + 0x78);
    if (*(s16 *)(moby + 0xA6) == 0x5F3) {
        *(OvlQuad *)(moby + 0x10) = position_copy;
        *(s16 *)(moby_vars + 0x2E) = 1;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAE80), where it is exact; names translated to the US level program. */

extern int D_L00_0015F320;
extern int D_L00_0015F330;
extern short D_L00_001613C8_d __asm__("D_L00_001613C8") __attribute__((sda));
extern void FUN_L00_0023e838(int);
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void FUN_L00_002a9be0(void) {
    char *tracked_moby = *(char **)&D_L00_001613C8_d;
    if (tracked_moby != 0) {
        if (*(short *)(tracked_moby + 0xA6) == 0x5F3) {
            char *data;
            int point_light_slot_index;
            if (*(unsigned char *)(tracked_moby + 0x20) != 0)
                return;
            data = *(char **)(tracked_moby + 0x78);
            D_L00_0015F320 = 0;
            D_L00_0015F330 = 0;
            point_light_slot_index = *(short *)(data + 0x2C);
            if (point_light_slot_index != -1)
                FUN_L00_0023e838(point_light_slot_index);
            mark_moby_for_removal(*(char **)&D_L00_001613C8_d);
        }
        *(int *)&D_L00_001613C8_d = 0;
    }
}
typedef union {
    u128 q;
    f32 f[4];
} V_2a9c50;
typedef struct {
    V_2a9c50 corner[4];
    u32 color[4];
    f32 uv[8];
    s64 unk70;
    s64 tex;
    s64 unk80;
    s64 unk88;
} Q_2a9c50;
typedef struct {
    u8 pad0[0x10];
    V_2a9c50 pos;
    f32 f20, f24, f28;
} PV_2a9c50;
typedef struct {
    u8 pad0[0x78];
    PV_2a9c50 *pv;
} M_2a9c50;
extern s32 D_L00_0015F5C4;
extern s32 D_L00_0015F5CC;
/* Spin angles of the second quad: x is read through $gp, the rest as an
   array through the unsplit address macro (from rac1-decomp's form). */
extern f32 spin_angles_x __asm__("D_L00_001613E0") __attribute__((sda));
extern f32 spin_angles[] __asm__("D_L00_001613E0") __attribute__((section(".sdata")));
extern f32 seg_sqrt(f32) __asm__("FUN_001f9988");
extern f32 appr_2a9c50(f32, f32) __asm__("FUN_001fa580");
int scale_game_frames(int) __asm__("FUN_001f96f8");
float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");
extern f32 fast_normalize_angle(f32) __asm__("FUN_001fa610");
extern void rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void mmul_2a9c50(void *, void *, void *) __asm__("FUN_001fa378");
extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");
void FUN_L00_002a9c50(M_2a9c50 *moby) {
    Q_2a9c50 quad;
    V_2a9c50 quad_transform[4];
    V_2a9c50 mz[4];
    V_2a9c50 direction_rotation_angles;
    PV_2a9c50 *pv = moby->pv;
    s32 i;
    s32 odd;
    f32 direction_inclination, spin_angle_numerator;
    if (D_L00_0015F5C4 == 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        odd = i & 1;
        direction_inclination = angle_atan2(seg_sqrt(pv->f20 * pv->f20 + pv->f28 * pv->f28), pv->f24);
        direction_rotation_angles.f[0] = appr_2a9c50(-direction_inclination, 1.57079637f);
        direction_rotation_angles.f[1] = angle_atan2(pv->f28, pv->f20);
        direction_rotation_angles.f[2] = 0.0f;
        direction_rotation_angles.f[3] = 0.0f;
        spin_angles_x = 0.0f;
        spin_angle_numerator = convert_integer_to_float(D_L00_0015F5CC % scale_game_frames(180));
        {
            f32 spin_period = scale_time(180.0f);
            f32 quad_spin_angle;
            spin_angle_numerator *= 6.28318548f;
            quad_spin_angle = fast_normalize_angle(spin_angle_numerator / spin_period);
            if (odd) {
                quad_spin_angle = -quad_spin_angle;
            }
            spin_angles[1] = quad_spin_angle;
        }
        spin_angles[2] = 0.0f;
        spin_angles[3] = 0.0f;
        rotation_matrix(mz, &spin_angles_x);
        rotation_matrix(quad_transform, &direction_rotation_angles);
        mmul_2a9c50(quad_transform, quad_transform, mz);
        qcopy(&quad_transform[3], &pv->pos);
        quad.color[0] = 0x80808080;
        quad.color[1] = 0x80808080;
        quad.color[2] = 0x80808080;
        quad.color[3] = 0x80808080;
        quad.tex = get_effect_texture(odd ? 0x12 : 0x11);
        quad.unk80 = 0xFF9000000260L;
        quad.unk88 = 0x8000000048L;
        quad.unk70 = 0;
        quad.corner[0].f[0] = -1.0f;
        quad.corner[0].f[1] = 0.0f;
        quad.corner[0].f[2] = 1.0f;
        quad.corner[0].f[3] = 1.0f;
        quad.corner[1].f[0] = 1.0f;
        quad.corner[1].f[1] = 0.0f;
        quad.corner[1].f[2] = 1.0f;
        quad.corner[1].f[3] = 1.0f;
        quad.corner[2].f[0] = -1.0f;
        quad.corner[2].f[1] = 0.0f;
        quad.corner[2].f[2] = -1.0f;
        quad.corner[2].f[3] = 1.0f;
        quad.corner[3].f[0] = 1.0f;
        quad.corner[3].f[1] = 0.0f;
        quad.corner[3].f[2] = -1.0f;
        quad.corner[3].f[3] = 1.0f;
        quad.uv[0] = 0.0f;
        quad.uv[1] = 1.0f;
        quad.uv[2] = 1.0f;
        quad.uv[3] = 1.0f;
        quad.uv[4] = 0.0f;
        quad.uv[5] = 0.0f;
        quad.uv[6] = 1.0f;
        quad.uv[7] = 0.0f;
        draw_geometry_quad(&quad, quad_transform, 1);
    }
}
extern float D_0015ED6C_2a9ed0 __asm__("D_0015ED6C");
extern int D_L00_0015F5CC_2a9ed0 __asm__("D_L00_0015F5CC");
float FUN_002132a8_2a9ed0(float, float) __asm__("FUN_002132a8");
void FUN_L00_00250df8_2a9ed0(void *) __asm__("FUN_L00_00250df8");
unsigned char *spawn_mine(int owner, OvlQuad *pos) __asm__("FUN_L00_002a9ed0");
unsigned char *spawn_mine(int owner, OvlQuad *spawn_position) {
    unsigned char *mine_moby = create_moby(0x4A);
    if (mine_moby) {
        unsigned char *mine_vars = *(unsigned char **)(mine_moby + 0x78);
        mine_moby[0x30] = 0xFF;
        *(short *)(mine_moby + 0x32) = 0x7F;
        mine_moby[0x31] = 1;
        *(float *)(mine_moby + 0x2C) = *(float *)(mine_moby + 0x2C) * 0.5f;
        mine_moby[0x20] = 0;
        *(int *)(mine_vars + 0x30) = owner;
        *(short *)(mine_vars + 0x36) = 0;
        *(int *)(mine_vars + 0x78) = 0;
        qcopy(mine_moby + 0x10, spawn_position);
        clear_u64_value(mine_vars);
        {
            float a =
                FUN_002132a8_2a9ed0(D_0015ED6C_2a9ed0 * 1.0471976f, D_0015ED6C_2a9ed0 * 3.1415927f);
            float frame_rate_scale = D_0015ED6C_2a9ed0;
            float b;
            int x;
            float z;
            *(float *)(mine_vars + 0x38) = a;
            b = FUN_002132a8_2a9ed0(frame_rate_scale * 1.0471976f, frame_rate_scale * 3.1415927f);
            x = D_L00_0015F5CC_2a9ed0;
            z = 0.0f;
            *(float *)(mine_vars + 0x3C) = b;
            *(float *)(mine_vars + 0x64) = z;
            *(float *)(mine_vars + 0x60) = z;
            *(float *)(mine_vars + 0x70) = z;
            *(float *)(mine_vars + 0x6C) = z;
            *(int *)(mine_vars + 0x5C) = x;
            *(int *)(mine_vars + 0x74) = 0;
            *(int *)(mine_vars + 0x68) = -1;
            *(int *)(mine_moby + 0x94) = 0;
        }
        FUN_L00_00250df8_2a9ed0(mine_moby);
    }
    return mine_moby;
}
typedef union {
    u128 q;
    f32 f[4];
} V_2aa008;
typedef struct {
    u8 pad[0x5C];
    s32 i5C;
} D_2aa008;
typedef struct {
    u8 pad[0x34];
    s16 h34;
} M_2aa008;
typedef struct N_2aa008 {
    u8 pad[0x10];
    V_2aa008 v10;
    u8 b20;
    u8 pad2[0x28 - 0x21];
    struct N_2aa008 *p28;
    f32 f2C;
    u8 pad3[0x78 - 0x30];
    void *p78;
    u8 pad4[0xA6 - 0x7C];
    s16 hA6;
} N_2aa008;
typedef struct {
    u8 pad[0x10];
    V_2aa008 v10;
} Q_2aa008;
extern f32 D_0015ED6C_2aa008 __asm__("D_0015ED6C");
extern f32 frame_time_sq __asm__("D_0015ED70");
extern N_2aa008 *D_L00_0015FFE4_2aa008 __asm__("D_L00_0015FFE4");
extern V_2aa008 D_L00_00173E60_2aa008[] __asm__("D_L00_00173E60") __attribute__((section(".data")));
f32 FUN_002132a8_2aa008(f32, f32) __asm__("FUN_002132a8");
void FUN_001f9bf8_2aa008(void *, void *, f32) __asm__("FUN_001f9bf8");
s32 FUN_L00_002603d0_2aa008(void *) __asm__("FUN_L00_002603d0");
s32 FUN_001f96f8_2aa008(s32) __asm__("FUN_001f96f8");
s32 collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
void FUN_L00_001ff660_2aa008(void *, void *, void *) __asm__("FUN_L00_001ff660");
void FUN_L00_002aa008(V_2aa008 *requested_position, N_2aa008 *self, void *base_motion_vector) {
    V_2aa008 random_offset;
    V_2aa008 vector_scratch;
    V_2aa008 collision_position_offset;
    M_2aa008 *moby_vars;
    struct Hero *P;
    N_2aa008 *scanned_moby;
    s32 count;
    N_2aa008 *best;
    s32 bestv;
    V_2aa008 *k;
    vector_scratch.q = 0;
    moby_vars = self->p78;
    vector_scratch.f[0] = FUN_002132a8_2aa008(-1.0f, 1.0f);
    vector_scratch.f[1] = FUN_002132a8_2aa008(-1.0f, 1.0f);
    vector_scratch.f[2] = FUN_002132a8_2aa008(-1.0f, 1.0f);
    self->b20 = 1;
    self->f2C = self->f2C / 0.65f;
    random_offset.q = vector_scratch.q;
    FUN_001f9bf8_2aa008(&random_offset, &random_offset,
                        FUN_002132a8_2aa008(D_0015ED6C_2aa008 * 0.01f, D_0015ED6C_2aa008 * 0.1f));
    add_vector_xyz(moby_vars, &random_offset, base_motion_vector);
    P = &hero;
    if (P->ground_moby && FUN_L00_002603d0_2aa008(P->ground_moby)) {
        add_vector_xyz(moby_vars, moby_vars, &P->motion.displacement);
    }
    moby_vars->h34 = FUN_001f96f8_2aa008(0x78);
    count = 0;
    best = 0;
    bestv = 0;
    for (scanned_moby = D_L00_0015FFE4_2aa008; scanned_moby; scanned_moby = scanned_moby->p28) {
        if (scanned_moby->b20 != 0xFE)
            if (scanned_moby->b20 != 0xFD)
                if (scanned_moby->hA6 == self->hA6)
                    if (scanned_moby != self) {
                        if (best == 0 || ((D_2aa008 *)scanned_moby->p78)->i5C < bestv) {
                            bestv = ((D_2aa008 *)scanned_moby->p78)->i5C;
                            best = scanned_moby;
                        }
                        count++;
                    }
    }
    if (count >= 8)
        ((D_2aa008 *)best->p78)->i5C = -1;
    qcopy(&self->v10, requested_position);
    vector_scratch.q = ((Q_2aa008 *)hero.moby)->v10.q;
    vector_scratch.f[2] = requested_position->f[2];
    if (collision_line(&vector_scratch, requested_position, 0, hero.moby, 0)) {
        k = D_L00_00173E60_2aa008;
        qcopy(&self->v10, &k[0]);
        FUN_001f9bf8_2aa008(&collision_position_offset, &k[2], frame_time_sq * 9.7f);
        add_vector_xyz(&self->v10, &self->v10, &collision_position_offset);
        FUN_L00_001ff660_2aa008(moby_vars, moby_vars, &k[2]);
        scale_vector_xyz(moby_vars, moby_vars, 0.6f);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AB548), where it is exact; names translated to the US level program. */

extern V D_L00_00166DC0;
extern char *FUN_L00_002603d0_u(void *) __asm__("FUN_L00_002603d0");
extern char D_0013F350[];
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern float frame_time_sq __asm__("D_0015ED70");
extern float D_L00_00173E80[];
extern int D_L00_0015F5C4_c __asm__("D_L00_0015F5C4");
extern int D_L00_00173E40_u[] __asm__("D_L00_00173E40");
extern int FUN_001efa68(void*,void*,int,int,int);
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer(int*) __asm__("FUN_001f9740");
extern void *D_L00_00173E58_c[] __asm__("D_L00_00173E58");
extern void FUN_L00_002a9c50_c(void) __asm__("FUN_L00_002a9c50");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");

int predict_mine_impact(char *trajectory_state, char *moby, int mode) __asm__("FUN_L00_002aa2a8");

int predict_mine_impact(char *trajectory_state, char *moby, int mode) {
    float trajectory_position[4];
    float B[4];
    float C[4];
    float box[4];
    int x;
    char *base = D_0013F350;
    int i;
    char *owner_moby_vars;
    if (*(int *)(base + 0x2084) == 0x72) return 0;
    if (D_L00_0015F5C4_c != 0) return 0;
    qcopy(C, moby + 0x10);
    if (*(unsigned char *)(moby + 0x20) == 0) {
        trajectory_position[0] = fast_cos(fast_add_rotations_c(-0.36196801f, fast_arc_tan(*(float *)(base + 0x670), *(float *)(base + 0x674)))) * 0.859039f;
        trajectory_position[1] = fast_sin(fast_add_rotations_c(-0.36196801f, fast_arc_tan(*(float *)(base + 0x670), *(float *)(base + 0x674)))) * 0.859039f;
        *(int *)&trajectory_position[2] = 0;
        add_vector_xyz(trajectory_position, trajectory_position, base + 0x80);
        trajectory_position[2] = trajectory_position[2] + 0.49082f;
        qcopy(B, trajectory_state);
        if (*(int *)(base + 0x2FC) != 0 && FUN_L00_002603d0_u((void *)*(int *)(base + 0x2FC)) != 0 && mode != 0) {
            add_vector_xyz(B, B, base + 0x100);
        }
        x = scale_game_frames_alt(0x78);
    } else {
        owner_moby_vars = *(char **)(*(char **)(trajectory_state + 0x30) + 0x78);
        qcopy(trajectory_position, moby + 0x10);
        qcopy(B, trajectory_state);
        {
            short xs = *(short *)(trajectory_state + 0x34);
            int owner_prediction_stop_signal = *(int *)(owner_moby_vars + 0x50);
            x = xs;
            if (owner_prediction_stop_signal != 0) *(short *)(trajectory_state + 0x36) = 1;
        }
    }
    {
        char *base2 = D_0013F350;
        char *hero_moby = *(char **)(base2 + 0x2080);
        *(u128 *)box = *(u128 *)(hero_moby + 0x10);
        box[2] = trajectory_position[2];
        if (FUN_001efa68(box, trajectory_position, 0x10, (int)hero_moby, 0) != 0) {
            if (collision_hit_material() != 0 && D_L00_00173E40_u[7] > 0) {
                qcopy(trajectory_state + 0x10, (char *)D_L00_00173E40_u + 0x20);
            }
        }
    }
    while (fast_dec_timer(&x) == 0) {
        qcopy(C, trajectory_position);
        i = 9;
        do {
            add_vector_xyz(trajectory_position, trajectory_position, B);
            B[2] = B[2] - frame_time_sq * 9.8f;
        } while (--i >= 0);
        if (FUN_001efa68(C, trajectory_position, 0x12, (int)moby, 0) == 0) continue;
        if (collision_hit_material() == 0 && 0.0f < B[2]) continue;
        if (D_L00_00173E40_u[7] > 0) {
            qcopy(trajectory_state + 0x10, (char *)D_L00_00173E40_u + 0x20);
            break;
        }
    }
    if (x != 0) {
        if (mode == 0) {
            if (D_L00_00173E58_c[0] == 0) return 0;
            if (FUN_L00_002603d0_u(D_L00_00173E58_c[0]) == 0) return 0;
        }
        if (mode != 2 || D_L00_00173E58_c[0] == 0 || FUN_L00_002603d0_u(D_L00_00173E58_c[0]) == 0) {
            enqueue_callback_list_1_alt(FUN_L00_002a9c50_c, moby);
        }
        {
            char *camera_position = (char *)&D_L00_00166DC0;
            char *hit_position = trajectory_state + 0x10;
            *(u128 *)(trajectory_state + 0x20) = *(u128 *)D_L00_00173E80;
            subtract_vector_xyz(C, hit_position, camera_position);
            scale_vector_xyz_c(C, C, 0.95f);
            add_vector_xyz(hit_position, C, camera_position);
        }
        return 1;
    }
    return 0;
}

s32 D_L00_001D7460[1] __attribute__((section(".data"))) = {0};
s16 D_L01_001CBC4C[1] __attribute__((section(".data"))) = {0};
s32 D_L01_001D78E0[1] __attribute__((section(".data"))) = {0};
s16 D_L02_001CBDCC[1] __attribute__((section(".data"))) = {0};
s32 D_L02_001CBEE0[1] __attribute__((section(".data"))) = {0};
s16 D_L03_001CB8CC[1] __attribute__((section(".data"))) = {0};
s32 D_L03_001CB9E0[1] __attribute__((section(".data"))) = {0};
s32 D_L04_001CBA60[1] __attribute__((section(".data"))) = {0};
s16 D_L05_001CBC4C[1] __attribute__((section(".data"))) = {0};
s32 D_L05_001CBD60[1] __attribute__((section(".data"))) = {0};
s16 D_L06_001CBF4C[1] __attribute__((section(".data"))) = {0};
s32 D_L06_001CC060[1] __attribute__((section(".data"))) = {0};
s16 D_L07_001CB84C[1] __attribute__((section(".data"))) = {0};
s32 D_L07_001CB960[1] __attribute__((section(".data"))) = {0};
s16 D_L08_001CBFCC[1] __attribute__((section(".data"))) = {0};
s32 D_L08_001CC0E0[1] __attribute__((section(".data"))) = {0};
s16 D_L09_001CB94C[1] __attribute__((section(".data"))) = {0};
s32 D_L09_001CBA60[1] __attribute__((section(".data"))) = {0};
s16 D_L10_001CBC4C[1] __attribute__((section(".data"))) = {0};
s32 D_L10_001CBD60[1] __attribute__((section(".data"))) = {0};
s16 D_L11_001CC1CC[1] __attribute__((section(".data"))) = {0};
s32 D_L11_001CC2E0[1] __attribute__((section(".data"))) = {0};
s16 D_L12_001CBC4C[1] __attribute__((section(".data"))) = {0};
s32 D_L12_001CBD60[1] __attribute__((section(".data"))) = {0};
s16 D_L13_001CBACC[1] __attribute__((section(".data"))) = {0};
s32 D_L13_001CBBE0[1] __attribute__((section(".data"))) = {0};
s16 D_L14_001CBECC[1] __attribute__((section(".data"))) = {0};
s32 D_L14_001D87D0[1] __attribute__((section(".data"))) = {0};
s16 D_L15_001CBDCC[1] __attribute__((section(".data"))) = {0};
s32 D_L15_001CBEE0[1] __attribute__((section(".data"))) = {0};
s16 D_L16_001CBC4C[1] __attribute__((section(".data"))) = {0};
s32 D_L16_001CBD60[1] __attribute__((section(".data"))) = {0};
s16 D_L17_001CC0CC[1] __attribute__((section(".data"))) = {0};
s32 D_L17_001CC1E0[1] __attribute__((section(".data"))) = {0};
s16 D_L18_001CC1CC[1] __attribute__((section(".data"))) = {0};
s32 D_L18_001CC2E0[1] __attribute__((section(".data"))) = {0};
