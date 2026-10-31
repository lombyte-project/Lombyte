/* Shared moby class spiderbot. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_line.h"
#include "sda.h"
#include "rnc/overlay/quad.h"

typedef int u128_2A8090 __attribute__((mode(TI)));

typedef struct {
    float          v[2];
    float          f8;
    float          fC;
    unsigned char *moby;
    int            flags;
    unsigned char  b18;
    unsigned char  b19;
    short          h1A;
    float          f1C;
    int            i20;
    int            pad24[3];
} Ray;

typedef struct {
    float v[4][4];
    int   col[4];
    float uv[4][2];
    long  a, b, c, d;
} Quad;

extern float D_L15_001673C0[4];
extern float fast_sin(float);
extern int   random_integer_below(int);
extern void  add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern float D_0015ED6C;
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern float D_0015ED6C MACRO_ADDR;
extern void             draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

extern float          FUN_L04_001f1978(float);
extern s32            rand(void);
int                   spiderbot_effect_spawn_count_gp __asm__("D_L15_00161558") = 40;
float                 spiderbot_effect_scale_gp __asm__("D_L15_0016155C") = 0.75f;
float                 spiderbot_effect_lifetime_range_end_gp __asm__("D_L15_00161560") = 5.0f;
extern u8             D_L15_00174460[];
extern void           draw_spiderbot_effect_quad() __asm__("FUN_L15_002a7628");
void                  normalize_vector_triplet(f32 *vectors) __asm__("FUN_00214128");
extern unsigned char *func_L00_00273F80_r(float        *pos,
                                          char         *vel,
                                          int           color,
                                          unsigned char life,
                                          unsigned char b,
                                          int           mode,
                                          float         scale) __asm__("FUN_L00_002730e0");

extern float spiderbot_effect_quad_uvs[] __asm__("D_L15_001E27F0");
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");

void draw_spiderbot_effect_quad(char *moby) __asm__("FUN_L15_002a7628");

/* Spiderbot update: steps the walk/turn state, then probes ahead and sprays sparks along the path. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A8090), where it is exact; names translated to the US level program. */

void spiderbot_update(unsigned char *moby) __asm__("FUN_L15_002a6e68");

/* Moby class 123 */
void spiderbot_update(unsigned char *moby) {
    enum {
        SPIDERBOT_INITIALIZE = 0,
        SPIDERBOT_CIRCULAR_MOTION = 1,
        SPIDERBOT_ARC_OSCILLATION = 2,
        SPIDERBOT_LINEAR_OSCILLATION = 3
    };
    float          previous_moby_position[4];
    float          A[4];
    float          B[4];
    float          C[4];
    unsigned char *moby_data = ((unsigned char *)MOBY(moby)->pvars);
    *(u128_2A8090 *)previous_moby_position = *(u128_2A8090 *)(moby + 0x10);
    {
        float D[4];
        float M[16];
        float T[4];
        switch (moby[0x20]) {
        case SPIDERBOT_INITIALIZE:
            *(u128_2A8090 *)moby_data = *(u128_2A8090 *)previous_moby_position;
            MOBY(moby)->anim_speed = *(float *)(moby_data + 0x30) / 1.5f;
            *(float *)(moby_data + 0x28) = *(float *)(moby_data + 0x28) * 0.017453292f;
            if (*(int *)(moby_data + 0x20) == 0) {
                moby[0x20] = 1;
                *(float *)(moby_data + 0x2C) = MOBY(moby)->rot.z;
            } else if (*(int *)(moby_data + 0x20) >= 3) {
                moby[0x20] = 2;
                *(float *)(moby_data + 0x2C) = MOBY(moby)->rot.z;
            } else {
                moby[0x20] = 3;
            }
            break;
        case SPIDERBOT_CIRCULAR_MOTION: {
            *(u128_2A8090 *)C = 0;
            C[2] = *(float *)(moby_data + 0x24);
            *(u128_2A8090 *)D = 0;
            *(u128_2A8090 *)B = *(u128_2A8090 *)C;
            *(float *)(moby_data + 0x28) = fast_add_rotations(
                *(float *)(moby_data + 0x28),
                360.0f /
                    (*(float *)(moby_data + 0x24) * 6.2831855f / *(float *)(moby_data + 0x30)) *
                    0.017453292f * D_0015ED6C);
            MOBY(moby)->anim_speed = *(float *)(moby_data + 0x30) / 1.5f;
            C[0] = fast_cos(*(float *)(moby_data + 0x2C));
            C[1] = fast_sin(*(float *)(moby_data + 0x2C));
            C[2] = 0.0f;
            build_look_at_matrix(A, B, C, *(float *)(moby_data + 0x28));
            fast_vec_add(T, moby_data, A);
            *(u128_2A8090 *)(moby + 0x10) = *(u128_2A8090 *)T;
            D[2] = fast_add_rotations(*(float *)(moby_data + 0x2C), 1.5707964f);
            D[1] = fast_add_rotations(-*(float *)(moby_data + 0x28), 3.1415927f);
            euler_rotation_matrix(M, D);
            copy_matrix3x4(moby + 0xC0, M);
            normalize_vector_triplet(moby + 0xC0);
            MOBY(moby)->flags |= 0x100;
            break;
        }
        case SPIDERBOT_ARC_OSCILLATION: {
            float previous_arc_angle;
            float current_arc_angle;
            *(u128_2A8090 *)C = 0;
            C[2] = -*(float *)(moby_data + 0x24);
            *(u128_2A8090 *)D = 0;
            *(u128_2A8090 *)B = *(u128_2A8090 *)C;
            previous_arc_angle = fast_add_rotations(
                fast_normalize_angle(fast_cos(*(float *)(moby_data + 0x28)) * 1.5707964f),
                *(int *)(moby_data + 0x20) == 4 ? 1.5707964f : 0.0f);
            *(float *)(moby_data + 0x28) = fast_add_rotations(
                *(float *)(moby_data + 0x28),
                360.0f /
                    (*(float *)(moby_data + 0x24) * 6.2831855f / *(float *)(moby_data + 0x30)) *
                    0.017453292f * D_0015ED6C);
            current_arc_angle = fast_add_rotations(
                fast_normalize_angle(fast_cos(*(float *)(moby_data + 0x28)) * 1.5707964f),
                *(int *)(moby_data + 0x20) == 4 ? 1.5707964f : 0.0f);
            MOBY(moby)->anim_speed =
                *(float *)(moby_data + 0x24) *
                fast_subtract_rotations(current_arc_angle, previous_arc_angle) /
                (D_0015ED6C * 1.5f);
            C[0] = fast_cos(*(float *)(moby_data + 0x2C));
            C[1] = fast_sin(*(float *)(moby_data + 0x2C));
            C[2] = 0.0f;
            build_look_at_matrix(A, B, C, current_arc_angle);
            fast_vec_add(T, moby_data, A);
            *(u128_2A8090 *)(moby + 0x10) = *(u128_2A8090 *)T;
            D[2] = fast_add_rotations(*(float *)(moby_data + 0x2C), 1.5707964f);
            D[1] = fast_add_rotations(-current_arc_angle, 0.0f);
            euler_rotation_matrix(M, D);
            copy_matrix3x4(moby + 0xC0, M);
            normalize_vector_triplet(moby + 0xC0);
            MOBY(moby)->flags |= 0x100;
            break;
        }
        case SPIDERBOT_LINEAR_OSCILLATION: {
            float previous_linear_displacement;
            float x;
            previous_linear_displacement =
                fast_cos(*(float *)(moby_data + 0x28)) * *(float *)(moby_data + 0x24);
            *(float *)(moby_data + 0x28) = fast_add_rotations(
                *(float *)(moby_data + 0x28),
                360.0f /
                    (*(float *)(moby_data + 0x24) * 6.2831855f / *(float *)(moby_data + 0x30)) *
                    0.017453292f * D_0015ED6C);
            x = fast_cos(*(float *)(moby_data + 0x28)) * *(float *)(moby_data + 0x24);
            MOBY(moby)->anim_speed = (x - previous_linear_displacement) / (D_0015ED6C * 1.5f);
            build_spherical_offset(A,
                                   x,
                                   *(int *)(moby_data + 0x20) != 2 ? MOBY(moby)->rot.z : 0.0f,
                                   *(int *)(moby_data + 0x20) == 2 ? 1.5707964f : 0.0f);
            fast_vec_add(B, moby_data, A);
            *(u128_2A8090 *)(moby + 0x10) = *(u128_2A8090 *)B;
            break;
        }
        }
    }
    {
        Ray   R;
        float T2[4];
        int   i;
        *(u128_2A8090 *)B = 0;
        B[2] = 12.0f;
        transform_vector_by_basis(B, B, moby + 0xC0);
        fast_vec_add(R.v, moby + 0x10, B);
        *(u128_2A8090 *)A = *(u128_2A8090 *)R.v;
        R.flags = 0x10001;
        R.moby = moby;
        R.f1C = 1.0f;
        R.i20 = 1;
        fast_vec_normalize(R.v, B, 1.0f);
        R.h1A = ((unsigned short)MOBY(moby)->oclass);
        R.f8 = 1.0f;
        R.fC = 5627.9248f;
        R.b18 = 1;
        R.b19 = 1;
        fast_vec_sub(T2, moby + 0x10, previous_moby_position);
        *(u128_2A8090 *)C = *(u128_2A8090 *)T2;
        collision_line(moby + 0x10, A, 9, moby, &R);
        if (collision_line(moby + 0x10, A, 2, 0, 0)) {
            *(u128_2A8090 *)(moby_data + 0x10) = *(u128_2A8090 *)D_L15_00174460;
        } else {
            *(u128_2A8090 *)(moby_data + 0x10) = *(u128_2A8090 *)A;
        }
        for (i = 0; i < spiderbot_effect_spawn_count_gp; i++) {
            float         effect_direction_sign;
            float         effect_segment_length;
            int           k;
            unsigned char life;
            effect_direction_sign = -1.0f;
            if (rand() & 1)
                effect_direction_sign = 1.0f;
            k = truncate_float_to_s32(
                FUN_L04_001f1978(random_float_between(spiderbot_effect_lifetime_range_end_gp * 0.5f,
                                                      spiderbot_effect_lifetime_range_end_gp)));
            fast_vec_sub(T2, moby_data + 0x10, moby + 0x10);
            *(u128_2A8090 *)B = *(u128_2A8090 *)T2;
            fast_vec_scale(B, B, effect_direction_sign);
            effect_segment_length = fast_vec_length(B);
            *(u128_2A8090 *)A = effect_direction_sign < 0.0f ? *(u128_2A8090 *)(moby_data + 0x10)
                                                             : *(u128_2A8090 *)(moby + 0x10);
            life = k;
            fast_vec_scale(B, B, random_float_between(0.0f, 0.9f));
            add_vectors(T2, A, B);
            fast_vec_normalize(
                B,
                B,
                random_float_between(D_0015ED6C * 10.0f,
                                     effect_segment_length / convert_integer_to_float(k * 4)));
            func_L00_00273F80_r(A,
                                (char *)B,
                                0x604040FF,
                                life,
                                random_integer_below(0xFF),
                                0,
                                spiderbot_effect_scale_gp);
        }
    }
    add_draw_callback(draw_spiderbot_effect_quad, moby);
}

/* Builds a screen-space quad from the moby's offset and draws it. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A8850), where it is exact; names translated to the US level program. */

void draw_spiderbot_effect_quad(char *moby) {
    float B[4];
    float A[4];
    Quad  effect_quad;
    float F[4];
    float E[4];
    float C[4];
    float D[4];
    char *d = ((char *)MOBY(moby)->pvars);
    int   i;
    fast_vec_sub(A, d + 0x10, moby + 0x10);
    *(OvlQuad *)B = *(OvlQuad *)A;
    fast_vec_normalize(B, B, 0.5f);
    *(OvlQuad *)C = *(OvlQuad *)&MOBY(moby)->pos;
    add_vectors(D, C, B);
    fast_vec_sub(E, C, d + 0x10);
    fast_vec_sub(A, C, D_L15_001673C0);
    fast_vec_sub(F, d + 0x10, D_L15_001673C0);
    fast_vec_cross(A, A, E);
    fast_vec_normalize(A, A, 0.1f);
    fast_vec_cross(F, F, E);
    fast_vec_normalize(F, F, 0.1f);
    effect_quad.b = get_effect_texture(0xE);
    effect_quad.c = 0xFF9000000260L;
    effect_quad.d = 0x8000000048L;
    effect_quad.a = 0;
    effect_quad.col[3] = 0x80243278;
    effect_quad.col[2] = 0x80243278;
    effect_quad.col[1] = 0x80243278;
    effect_quad.col[0] = 0x80243278;
    for (i = 0; i < 4; i++) {
        effect_quad.uv[i][0] = spiderbot_effect_quad_uvs[i * 2];
        effect_quad.uv[i][1] = spiderbot_effect_quad_uvs[i * 2 + 1];
    }
    fast_vec_sub(D, C, A);
    *(OvlQuad *)effect_quad.v[0] = *(OvlQuad *)D;
    fast_vec_add(D, C, A);
    *(OvlQuad *)effect_quad.v[1] = *(OvlQuad *)D;
    fast_vec_sub(D, d + 0x10, F);
    *(OvlQuad *)effect_quad.v[2] = *(OvlQuad *)D;
    fast_vec_add(D, d + 0x10, F);
    *(OvlQuad *)effect_quad.v[3] = *(OvlQuad *)D;
    draw_geometry_quad(&effect_quad, 0, 0);
}

int   D_L17_001614F0 = 0x28;
float D_L17_001614F4 = 0.75f;
float D_L17_001614F8 = 5.0f;
