/* Shared moby class particle_spawner_smoke__fire: a pool of smoke and fire particles. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/props/particle_spawner_smoke_fire_data/particle_spawner_smoke_fire_update_2.h"
#include "rnc/overlay/shared/props/particle_spawner_smoke_fire_data/particle_spawner_smoke_fire_update.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/overlay/shared/props/particle_spawner_smoke_fire_controller.h"

typedef struct {
    float x, y, z, w;
} V;

typedef struct {
    s16   start; /* 00 */
    s16   count; /* 02 */
    int   idx;   /* 04 */
    char  pad08[0x18];
    float unk20, unk24, unk28; /* 20 24 28 */
    s16   h2C, h2E;            /* 2C 2E */
    int   x30, x34;            /* 30 34 */
    int   col;                 /* 38 */
    u8    c0, c1, c2;          /* 3C 3D 3E */
    u8    pad3F;
    s16   extra;  /* 40 */
    s16   flag42; /* 42 */
    float f44, f48, f4C, f50;
    int   texture;                                     /* 54 */
    int   mesh_texture;                                /* 58 */
    u8    mesh_alpha, mesh_red, mesh_green, mesh_blue; /* 5C..5F */
} Q;

typedef struct {
    V     v0, v1, v2;
    float pos[3];
    float w;
    u8    pad[0x40];
} T;

typedef struct {
    u8  pad0[4];
    int x04;
    u8  pad08[2];
    u8  b0A;
    u8  b0B;
} E2;

typedef struct {
    u8  pad0[3];
    u8  b03;
    u8  pad4[6];
    s16 h0A;
    u8  pad0C[0x14];
    E2  sub;
} E;

typedef struct {
    u8 pad[0x10];
    V  pos;
    u8 state;
    u8 pad21[0xF];
    u8 b30;
    u8 pad31[0x47];
    Q *q;
} P;

typedef struct {
    float x, y, z, w;
} V_2d7d58;

extern s32  random_integer_below(s32) __asm__("FUN_00213260");
extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern int  random_integer_below(int) __asm__("FUN_00213260");

s32 random_integer_below(s32) __asm__("FUN_00213260");

extern T *D_L00_001600EC;

extern int smoke_fire_emission_attempts_per_update_gp __asm__("D_L00_001619BC")
    __attribute__((sda));

extern V D_L00_00166DC0;
void     mark_moby_for_removal(P *) __asm__("FUN_0020c828");
float    fast_vec_length(void *) __asm__("FUN_001f9af0");
int      random_integer_below(int) __asm__("FUN_00213260");
void     reset_smoke_fire_spawner_particle(P *spawner, int i) __asm__("FUN_L00_002d7bf0");

void  reset_smoke_fire_spawner_particle(P *, int) __asm__("FUN_L00_002d7bf0");
void  FUN_L00_002d7d58_c(P *, int) __asm__("FUN_L00_002d7d58");
float FUN_L00_00257c48(float, float);
int   FUN_L00_00257b90(int, int);
E    *FUN_L00_0026d000(V *, int, float, float, V *, float, int, float);
s16   FUN_001f96f8_c(int) __asm__("FUN_001f96f8");
void  draw_smoke_fire_spawner(P *) __asm__("FUN_L00_002d73e8");

extern float          D_L00_001D02E0[];
extern float          D_L00_001D15A0[];
extern float          D_L00_001D31C0[];
extern float          D_L00_001D4480[];
extern s16            D_L00_001D60A0[];
extern u8             D_L00_001D2D10[];
extern s16            D_L00_001D6A00[];
extern float          D_L00_00161374;
extern float          D_L00_00161384;
extern float          D_L00_0016138C;
extern unsigned short D_L00_00161388_gp __asm__("D_L00_00161388") __attribute__((sda));
extern int   smoke_fire_particle_steady_alpha_gp __asm__("D_L00_00161998") __attribute__((sda));
extern float D_L00_0016199C __attribute__((sda));
extern u8    D_L00_001619A0[4] __attribute__((sda));
extern u8    D_L00_001619A8[8] __attribute__((sda));
extern int   smoke_fire_mesh_quad_vertex_indices[][4] __asm__("D_L00_001E1380");
extern float smoke_fire_mesh_particle_local_positions[][4] __asm__("D_L00_001E12C0");
extern float smoke_fire_mesh_particle_v_coordinates[] __asm__("D_L00_001E1440");
extern float smoke_fire_mesh_particle_u_coordinates[] __asm__("D_L00_001E1444");
extern float smoke_fire_particle_quad_local_positions[][4] __asm__("D_L00_001E11A0");
extern float smoke_fire_particle_quad_texture_coordinates[] __asm__("D_L00_001E11E0");
extern float smoke_fire_base_mesh_local_positions[][4] __asm__("D_L00_001E1200");
extern float smoke_fire_base_mesh_v_coordinates[] __asm__("D_L00_001E13E0");
extern float smoke_fire_base_mesh_u_coordinates[] __asm__("D_L00_001E13E4");
extern void  vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");

extern u16 D_L00_00161380_2d7bf0 __asm__("D_L00_00161380");
extern int D_L00_0016137C __asm__("D_L00_0016137C");

extern V_2d7d58 D_L00_001CB7E0_lui[] __asm__("D_L00_001CB7E0") NOT_SDA;
extern float    D_L00_001D02E0_lui[] __asm__("D_L00_001D02E0") NOT_SDA;
extern float    D_L00_001D15A0_lui[] __asm__("D_L00_001D15A0") NOT_SDA;
extern float    D_L00_001D31C0_lui[] __asm__("D_L00_001D31C0") NOT_SDA;
extern float    D_L00_001D4480_lui[] __asm__("D_L00_001D4480") NOT_SDA;
extern u8       D_L00_001D2D10_lui[] __asm__("D_L00_001D2D10") NOT_SDA;

V D_L00_001CB7E0[1200] = {0};

u8 D_L00_001D2860[1200] = {0};

s16 D_L00_001D5740[1200] = {0};

/* Particle pool of 1200 entries, one array per field (more fields below FUN_L00_002d73e8). */

void particle_spawner_smoke_fire_update(P *p) __asm__("FUN_L00_002d6f28");

void particle_spawner_smoke_fire_update(P *spawner) {
    enum {
        PARTICLE_SPAWNER_INITIALIZE = 0,
        PARTICLE_SPAWNER_ACTIVE = 1
    };
    Q    *q = spawner->q;
    u8   *emitter_transform;
    V     culling_sphere, particle_vector, b;
    float g, sampled_particle_scalar;
    int   i, j, particle_start, particle_end_or_emission_index, col, r;
    E    *ent;
    E2   *sub;

    switch (spawner->state) {
    case PARTICLE_SPAWNER_INITIALIZE:
        spawner->state = 1;
        spawner->b30 = 0xFF;
        if (q->idx == -1) {
            mark_moby_for_removal(spawner);
            return;
        }
        q->f48 = fast_vec_length(&D_L00_001600EC[q->idx].v1);
        g = fast_vec_length(&D_L00_001600EC[q->idx].v0);
        if (q->f48 < g)
            q->f48 = g;
        if (q->extra)
            q->f44 = q->f48;
        else
            q->f44 = 3.0f;
        q->f50 = fast_vec_length(&D_L00_001600EC[q->idx].v2) * 0.5f;
        particle_start = q->start = fetch_and_add_global_accumulator(q->count + q->extra);
        particle_end_or_emission_index = particle_start + q->count;
        for (i = particle_start; i < particle_end_or_emission_index; i++) {
            r = random_integer_below(2);
            D_L00_001CB7E0[i].w = 1.0f;
            D_L00_001D2860[i] = r;
            reset_smoke_fire_spawner_particle(spawner, i);
            D_L00_001D5740[i] = 0;
        }
        j = particle_end_or_emission_index;
        particle_end_or_emission_index = particle_end_or_emission_index + q->extra;
        for (i = j; i < particle_end_or_emission_index; i++) {
            r = random_integer_below(2);
            D_L00_001CB7E0[i].w = 1.0f;
            D_L00_001D2860[i] = r;
            FUN_L00_002d7d58_c(spawner, i);
        }
        D_L00_001600EC[q->idx].w = 1.0f;
        q->col = q->c2 << 16 | q->c1 << 8 | q->c0;
        return;
    case PARTICLE_SPAWNER_ACTIVE:
        emitter_transform = (u8 *)&D_L00_001600EC[q->idx];
        qcopy(&culling_sphere, emitter_transform + 0x30);
        culling_sphere.z += 9.0f;
        culling_sphere.w = 10.0f;
        if (fast_bsphere_check(&culling_sphere, smoke_fire_far_culling_distance_gp) < 0)
            return;
        if (q->flag42 && vector_distance_xy(&D_L00_00166DC0, &spawner->pos) < 128.0f) {
            for (particle_end_or_emission_index = 0;
                 particle_end_or_emission_index < smoke_fire_emission_attempts_per_update_gp;
                 particle_end_or_emission_index++) {
                particle_vector.x = FUN_L00_00257c48(0.0f, 1.0f);
                particle_vector.y = FUN_L00_00257c48(0.0f, 1.0f);
                particle_vector.z = 1.0f;
                particle_vector.w = 1.0f;
                transform_vector(&b, &particle_vector, emitter_transform);
                b.z += 0.5f;
                particle_vector.x = FUN_L00_00257c48(0.0f, frame_time * 0.25f);
                particle_vector.y = FUN_L00_00257c48(0.0f, frame_time * 0.25f);
                particle_vector.z = q->unk20 * frame_time;
                sampled_particle_scalar = random_float_between(q->unk24, q->unk28);
                col = FUN_L00_00257b90(q->h2C, q->h2E) << 24 |
                      smoke_fire_emitted_particle_base_color_gp;
                r = FUN_L00_00257b90(smoke_fire_emitted_particle_sign_randomized_sample_min_gp,
                                     smoke_fire_emitted_particle_sign_randomized_sample_max_gp);
                ent = FUN_L00_0026d000(&b,
                                       r,
                                       smoke_fire_emitted_particle_position_spread_gp,
                                       1.0f,
                                       &particle_vector,
                                       smoke_fire_emitted_particle_random_scalar_upper_bound_gp,
                                       col,
                                       sampled_particle_scalar);
                if (ent) {
                    sub = &ent->sub;
                    ent->h0A = FUN_001f96f8_c(FUN_L00_00257b90(q->x30, q->x34));
                    sub->x04 = 3;
                    sub->b0A = col >> 24;
                    sub->b0B = ent->h0A;
                    if (random_integer_below(2))
                        ent->b03 = 0x44;
                }
            }
        }
        add_draw_callback(draw_smoke_fire_spawner, spawner);
        return;
    }
}

/* Ported from rac1-decomp (nonmatching/shared/func_L00_002D8898.c: func_L00_002D8898). */

void draw_smoke_fire_spawner(P *spawner) {
    Vec4                position, up;
    Vec4                facing[4], flipped[4];
    struct GeometryQuad quad;
    Q                  *q = spawner->q;
    T                  *volume;
    int                 mesh_alpha, mesh_color, end;
    Vec4               *transform;
    int                 alpha, i, k, j, index, particle_quad_vertex_index;
    float               width, height, phase, one;
    float              *vertex;
    Vec4               *front;
    int(*rows)[4];

    mesh_alpha = q->mesh_alpha << 24;
    mesh_color = q->mesh_blue << 16 | q->mesh_green << 8 | q->mesh_red;
    volume = &D_L00_001600EC[q->idx];
    end = q->start + q->count + q->extra;
    qcopy(&position, &spawner->pos);
    up.q = 0;
    position.f[3] = 1.0f;
    up.f[2] = 1.0f;
    fast_vec_sub(&facing[0], &D_L00_00166DC0, &position);
    fast_vec_normalize(&facing[0], &facing[0], 1.0f);
    fast_vec_cross(&facing[1], &facing[0], &up);
    fast_vec_normalize(&facing[1], &facing[1], 1.0f);
    fast_vec_cross(&facing[2], &facing[1], &facing[0]);
    qcopy(&facing[3], &position);
    /* Fakematch: the do-while(0) gives flipped's address s1 and facing's s2 as retail does. */
    do {
        fast_vec_scale(&flipped[0], &facing[0], -1.0f);
        fast_vec_cross(&flipped[1], &flipped[0], &up);
        fast_vec_normalize(&flipped[1], &flipped[1], 1.0f);
        fast_vec_cross(&flipped[2], &flipped[1], &flipped[0]);
        qcopy(&flipped[3], &facing[3]);
    } while (0);
    /* Fakematch: called as returning int (the draw above needs the s64 prototype) so
     * the texture is sign-extended into the quad as retail does. */
    quad.texture = ((int (*)(int))get_effect_texture)(q->texture + 0x28);
    quad.texture_state = 0xFF9000000260L;
    quad.primitive = 0x8000000048L;
    quad.reserved.value = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    /* Fakematch: the front copy gives the GCSE copy of facing's address a source with
     * no s2 preference, so the final loop's pointers rotate as retail. */
    front = facing;
    for (i = q->start; i < end; i++) {
        phase = D_L00_001D31C0[i] - D_L00_001D4480[i] * frame_scale;
        D_L00_001D31C0[i] = phase;
        transform = D_L00_001D2860[i] ? flipped : front;
        if (phase <= -8.0f) {
            D_L00_001D31C0[i] = phase + 8.0f;
        }
        transform_vector(&transform[3], &D_L00_001CB7E0[i], volume);
        width = D_L00_001D02E0[i];
        height = D_L00_001D15A0[i];
        if (D_L00_001D2D10[i]) {
            /* Fakematch: reading the index table through rows sets its address ahead of the
             * primitive constant, which then takes v1 as retail. */
            rows = smoke_fire_mesh_quad_vertex_indices;
            quad.primitive = 0x8000000044L;
            one = 1.0f;
            for (k = 2; k < 4; k++) {
                for (j = 0; j < 4; j++) {
                    index = rows[k][j];
                    vertex = smoke_fire_mesh_particle_local_positions[index];
                    quad.positions[j].f[0] = vertex[0];
                    quad.positions[j].f[1] = vertex[1] * width;
                    quad.positions[j].f[2] = vertex[2] * height;
                    quad.positions[j].f[3] = one;
                    transform_vector(&quad.positions[j], &quad.positions[j], transform);
                    quad.texture_coordinates[j][0] =
                        smoke_fire_mesh_particle_u_coordinates[index * 2] + D_L00_001D31C0[i];
                    quad.texture_coordinates[j][1] =
                        smoke_fire_mesh_particle_v_coordinates[index * 2];
                    quad.colors[j] = mesh_alpha * D_L00_001619A8[index] | mesh_color;
                }
                draw_geometry_quad(&quad, 0, 0);
            }
        } else {
            if (D_L00_001D5740[i]) {
                fast_dec_timer_s16(&D_L00_001D5740[i]);
                alpha = truncate_float_to_s32(
                    (1.0f - convert_integer_to_float(D_L00_001D5740[i]) * D_L00_00161384) *
                    D_L00_0016199C);
            } else if (D_L00_001D60A0[i]) {
                if (fast_dec_timer_s16(&D_L00_001D60A0[i])) {
                    reset_smoke_fire_spawner_particle(spawner, i);
                    alpha = 0;
                    D_L00_001D2860[i] = random_integer_below(2);
                } else {
                    alpha = truncate_float_to_s32(convert_integer_to_float(D_L00_001D60A0[i]) *
                                                  D_L00_0016138C * D_L00_0016199C);
                }
            } else {
                if (fast_dec_timer_s16(&D_L00_001D6A00[i])) {
                    D_L00_001D60A0[i] = D_L00_00161388_gp;
                }
                alpha = smoke_fire_particle_steady_alpha_gp;
            }
            alpha <<= 24;
            quad.primitive = 0x8000000048L;
            one = 1.0f;
            for (particle_quad_vertex_index = 0; particle_quad_vertex_index < 4;
                 particle_quad_vertex_index++) {
                quad.positions[particle_quad_vertex_index].f[0] =
                    smoke_fire_particle_quad_local_positions[particle_quad_vertex_index][0];
                quad.positions[particle_quad_vertex_index].f[1] =
                    smoke_fire_particle_quad_local_positions[particle_quad_vertex_index][1] * width;
                quad.positions[particle_quad_vertex_index].f[2] =
                    smoke_fire_particle_quad_local_positions[particle_quad_vertex_index][2] *
                    height;
                quad.positions[particle_quad_vertex_index].f[3] = one;
                quad.texture_coordinates[particle_quad_vertex_index][0] =
                    smoke_fire_particle_quad_texture_coordinates[particle_quad_vertex_index * 2] -
                    D_L00_001D31C0[i];
                quad.texture_coordinates[particle_quad_vertex_index][1] =
                    smoke_fire_particle_quad_texture_coordinates[particle_quad_vertex_index * 2 +
                                                                 1];
                quad.colors[particle_quad_vertex_index] =
                    alpha * D_L00_001619A0[particle_quad_vertex_index] | q->col;
                transform_vector(&quad.positions[particle_quad_vertex_index],
                                 &quad.positions[particle_quad_vertex_index],
                                 transform);
            }
            draw_geometry_quad(&quad, 0, 0);
        }
    }
    quad.texture = ((int (*)(int))get_effect_texture)(q->mesh_texture + 0x28);
    quad.primitive = 0x8000000044L;
    qcopy(&facing[3], volume->pos);
    facing[3].f[2] -= q->f50;
    if (q->flag42 == 0) {
        for (k = 0; k < 6; k++) {
            for (j = 0; j < 4; j++) {
                index = smoke_fire_mesh_quad_vertex_indices[k][j];
                vertex = smoke_fire_base_mesh_local_positions[index];
                quad.positions[j].f[0] = vertex[0];
                quad.positions[j].f[1] = vertex[1] * q->f48;
                quad.positions[j].f[2] = vertex[2] * q->f4C;
                quad.positions[j].f[3] = 1.0f;
                transform_vector(&quad.positions[j], &quad.positions[j], facing);
                quad.texture_coordinates[j][0] =
                    smoke_fire_base_mesh_u_coordinates[index * 2] + D_L00_00161374;
                quad.texture_coordinates[j][1] = smoke_fire_base_mesh_v_coordinates[index * 2];
                quad.colors[j] = D_L00_001619A8[index] * 0xFF000000 | mesh_color;
            }
            draw_geometry_quad(&quad, 0, 0);
        }
    }
}

/* Resets particle i of the emitter: random direction, random speeds from
 * the emitter's ranges, and a start delay staggered by index. */

void reset_smoke_fire_spawner_particle(P *spawner, int i) {
    char *spawner_vars = (char *)spawner->q;
    V    *particle_position;
    int   emitter_particle_index;
    float sampled_x = FUN_L00_00257c48(0.0f, 1.0f);
    particle_position = &D_L00_001CB7E0[i];
    particle_position->x = sampled_x;
    particle_position->y = FUN_L00_00257c48(0.0f, 1.0f);
    particle_position->z = FUN_L00_00257c48(0.0f, 1.0f);
    D_L00_001D02E0[i] =
        random_float_between(*(float *)(spawner_vars + 0x10), *(float *)(spawner_vars + 0x14));
    D_L00_001D15A0[i] =
        random_float_between(*(float *)(spawner_vars + 0x18), *(float *)(spawner_vars + 0x1C));
    D_L00_001D31C0[i] = 0.0f;
    D_L00_001D4480[i] =
        random_float_between(*(float *)(spawner_vars + 0x8), *(float *)(spawner_vars + 0xC));
    D_L00_001D5740[i] = D_L00_00161380_2d7bf0;
    D_L00_001D60A0[i] = 0;
    emitter_particle_index = i - *(s16 *)spawner_vars;
    D_L00_001D6A00[i] = (emitter_particle_index + 1) * D_L00_0016137C;
    D_L00_001D2D10[i] = 0;
}

void initialize_smoke_fire_spawner_mesh_particle(char *spawner, int i) __asm__("FUN_L00_002d7d58");

void initialize_smoke_fire_spawner_mesh_particle(char *spawner, int i) {
    char     *spawner_vars = *(char **)(spawner + 0x78);
    float     base_particle_height;
    V_2d7d58 *particle_position;
    float     sampled_x = randf_sym(0.0f, 1.0f);
    particle_position = &D_L00_001CB7E0_lui[i];
    particle_position->x = sampled_x;
    particle_position->y = randf_sym(0.0f, 1.0f);
    particle_position->z = randf_sym(0.0f, 1.0f);
    base_particle_height = *(float *)(spawner_vars + 0x4C);
    D_L00_001D02E0_lui[i] = random_float_between(*(float *)(spawner_vars + 0x44),
                                                 *(float *)(spawner_vars + 0x44) + 2.0f);
    D_L00_001D15A0_lui[i] = random_float_between(base_particle_height, base_particle_height + 4.0f);
    D_L00_001D31C0_lui[i] = 0.0f;
    D_L00_001D4480_lui[i] = random_float_between(0.003f, 0.005f);
    D_L00_001D2D10_lui[i] = 1;
}

V D_L01_001CBC60[1200] = {0};

V D_L14_001CBEE0[1200] = {0};

u8 D_L01_001D2CE0[1200] = {0};

u8 D_L14_001D2F60[1200] = {0};

s16 D_L01_001D5BC0[1200] = {0};

s16 D_L14_001D5E40[1200] = {0};

s16 D_L00_001D60A0[1200] = {0};

s16 D_L01_001D6520[1200] = {0};

s16 D_L14_001D67A0[1200] = {0};

s16 D_L00_001D6A00[1200] = {0};

s16 D_L01_001D6E80[1200] = {0};

s16 D_L14_001D7100[1200] = {0};

float D_L00_001D02E0[1200] = {0};

float D_L01_001D0760[1200] = {0};

float D_L14_001D09E0[1200] = {0};

float D_L00_001D15A0[1200] = {0};

float D_L01_001D1A20[1200] = {0};

float D_L14_001D1CA0[1200] = {0};

float D_L00_001D31C0[1200] = {0};

float D_L01_001D3640[1200] = {0};

float D_L14_001D38C0[1200] = {0};

float D_L00_001D4480[1200] = {0};

float D_L01_001D4900[1200] = {0};

float D_L14_001D4B80[1200] = {0};

u8 D_L00_001D2D10[1200] = {0};

u8 D_L01_001D3190[1200] = {0};

u8 D_L14_001D3410[1200] = {0};

f32 D_L01_001FA840[1] __attribute__((section(".data"))) = {0};

f32 D_L01_001FA880[1] __attribute__((section(".data"))) = {0};

f32 D_L14_001E04D0[1] __attribute__((section(".data"))) = {0};

f32 D_L14_001E0510[1] __attribute__((section(".data"))) = {0};

int   D_L01_00161CB4 = 1;
int   D_L01_00161C90 = 0x7f;
float D_L01_00161C94 = 127.0f;
u8    D_L01_00161C98[4] = {0, 1, 0, 1};

u8 D_L01_00161CA0[8] = {0};

int   D_L14_00161D54 = 1;
int   D_L14_00161D30 = 0x7f;
float D_L14_00161D34 = 127.0f;
u8    D_L14_00161D38[4] = {0, 1, 0, 1};

u8 D_L14_00161D40[8] = {0};
