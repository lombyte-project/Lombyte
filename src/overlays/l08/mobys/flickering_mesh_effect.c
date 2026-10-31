/* Level 08 moby class 0x29f: flickering mesh effect. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/shared/math/split_float_fraction.h"

/* Smooth noise: linearly interpolates between two entries of an 8-float ring at arg+0x90. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_002F7258), where it is exact; names translated to the US level program. */

float interpolate_sample_ring(float x, char *sample_ring_data) __asm__("FUN_L08_002f5d98");

float interpolate_sample_ring(float x, char *sample_ring_data) {
    float sample_position = x * 8.0f;
    int   i = truncate_float_to_s32(sample_position);
    float fr = sample_position - convert_integer_to_float(i);
    char *samples_base;
    int   j;
    i = i % 8;
    j = i + 1;
    j = j % 8;
    samples_base = sample_ring_data + 0x90;
    return *(float *)(samples_base - (-(i * 4))) * (1.0f - fr) +
           *(float *)(samples_base - (-(j * 4))) * fr;
}

/* A point light slot, 0x20 bytes apart. */
typedef struct {
    f32  red;
    f32  green;
    f32  blue;
    u8   padC[4];
    Vec4 pos;
} PointLight;

extern PointLight l08_point_light_slots[] __asm__("D_L08_00180AC0");

/* Builds the animated grid, draws its quads, and updates attached effects. */
/* Ported from rac1-decomp (nonmatching/l08_batalia/func_L08_002F7318.c: func_L08_002F7318). */

/* Flickering mesh / point light pvars, 0x160 bytes (flickering_mesh_effect_update
   initialises the ring and the light slot, draw_flickering_mesh_effect draws). */
typedef struct {
    f32  size_x;  /* 0x00: quad width; the larger of size_x/size_y sets the draw scale */
    f32  size_y;  /* 0x04 */
    u8   nx;      /* 0x08: grid columns */
    u8   ny;      /* 0x09: grid rows */
    u16  flags;   /* 0x0A: bit 0 = texture +0x28, bit 1 = particles, bit 2 = puffs, bit 15 = ... */
    u8   texture; /* 0x0C */
    u8   frames;  /* 0x0D: animation frames (0: static) */
    u8   frame_ticks; /* 0x0E */
    u8   current;     /* 0x0F: current texture */
    u8   red;         /* 0x10: quad colour */
    u8   blue;        /* 0x11 */
    u8   green;       /* 0x12 */
    u8   alpha;       /* 0x13 */
    f32  phase_start; /* 0x14 */
    f32  phase_end;   /* 0x18 */
    f32  amp_start;   /* 0x1C */
    f32  amp2_start;  /* 0x20 */
    f32  amp_end;     /* 0x24 */
    f32  amp2_end;    /* 0x28 */
    f32  wave_u;      /* 0x2C */
    f32  wave_v;      /* 0x30 */
    f32  wave2_v;     /* 0x34 */
    f32  wave2_u;     /* 0x38 */
    u8   blend0;      /* 0x3C: GS blend fields */
    u8   blend1;
    u8   blend2;
    u8   blend3;
    f32  u_rate;          /* 0x40: texture scroll */
    f32  u_amp;           /* 0x44 */
    f32  u_phase;         /* 0x48 */
    f32  v_rate;          /* 0x4C */
    f32  v_amp;           /* 0x50 */
    f32  v_phase;         /* 0x54 */
    f32  noise_amp;       /* 0x58 */
    f32  noise_speed;     /* 0x5C */
    f32  phase0;          /* 0x60 */
    f32  phase1;          /* 0x64 */
    f32  phase0_end;      /* 0x68 */
    f32  phase1_end;      /* 0x6C */
    f32  texture_w_count; /* 0x70: must be in (0, 8] */
    f32  texture_h_count; /* 0x74: must be in (0, 8] */
    f32  normal_u;        /* 0x78 */
    f32  normal_v;        /* 0x7C */
    f32  z0;              /* 0x80: bottom edge height */
    f32  z1;              /* 0x84: top edge height */
    f32  jitter;          /* 0x88: lower bound of the random flicker levels */
    f32  noise_rate;      /* 0x8C */
    f32  noise[8];        /* 0x90: flicker ring (interpolate_sample_ring), chases goal */
    f32  goal[8];         /* 0xB0 */
    f32  noise_u;         /* 0xD0 */
    f32  noise_v;         /* 0xD4 */
    f32  noise_bias;      /* 0xD8 */
    f32  particle_xy;     /* 0xDC */
    f32  particle_z;      /* 0xE0 */
    f32  particle_size0;  /* 0xE4 */
    f32  particle_size1;  /* 0xE8 */
    u8   particle_r;      /* 0xEC */
    u8   particle_g;
    u8   particle_b;
    u8   particle_a;
    s16  particle_life;   /* 0xF0 */
    u8   particle_count;  /* 0xF2 */
    u8   particle_ticks;  /* 0xF3 */
    f32  particle_range;  /* 0xF4 */
    s32  particle_timer;  /* 0xF8 */
    f32  particle_offset; /* 0xFC */
    f32  particle_height; /* 0x100 */
    f32  puff_xy;         /* 0x104 */
    f32  puff_z;          /* 0x108 */
    f32  puff_size0;      /* 0x10C */
    f32  puff_size1;      /* 0x110 */
    u8   puff_c0[4];      /* 0x114 */
    u8   puff_c1[4];      /* 0x118 */
    s16  puff_life0;      /* 0x11C */
    s16  puff_life1;      /* 0x11E */
    u8   puff_count;      /* 0x120 */
    u8   puff_ticks;      /* 0x121 */
    s16  puff_timer;      /* 0x122 */
    f32  puff_range;      /* 0x124 */
    f32  puff_blend;      /* 0x128 */
    f32  puff_offset;     /* 0x12C */
    f32  puff_height;     /* 0x130 */
    u8   light_red;       /* 0x134: point light colour */
    u8   light_green;     /* 0x135 */
    u8   light_blue;      /* 0x136 */
    u8   radius;          /* 0x137 */
    u8   falloff;         /* 0x138 */
    u8   light_range_x;   /* 0x139 */
    u8   light_range_y;   /* 0x13A */
    s8   light;           /* 0x13B: point light handle (-1: none) */
    f32  light_speed;     /* 0x13C */
    Vec4 light_pos;       /* 0x140 */
    f32  fade_range;      /* 0x150: distance over which the light fades out */
    f32  intensity;       /* 0x154 */
    f32  alpha_u;         /* 0x158 */
    f32  alpha_v;         /* 0x15C */
} FlickerLightVars;

/* A textured quad for draw_geometry_quad: four vertices, colours, uvs, GS registers. */
typedef struct {
    Vec4 vertex[4];
    u32  color[4];
    f32  uv[8];
    s64  gs[4];
} FlickerQuad;

/* interpolate_sample_ring(float, char *) with the arguments the other way round: the
   real prototype evaluates the operands in the other order and moves two words. */
extern float interpolate_sample_ring_data_first(char *, float) __asm__("FUN_L08_002f5d98");
extern int   l08_effect_animation_time __asm__("D_L08_0015F3F8");
extern f32   normalize_angle(f32) __asm__("FUN_L00_0025e310");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern char  D_L08_001675C0[];
/* spawn_colored_effect (rendering/00279f00.c) with size before life: the real order
   (size last) reorders the argument evaluation and moves six words. */
extern unsigned char *spawn_colored_effect_size_first(void *,
                                                      void *,
                                                      void *,
                                                      float,
                                                      short,
                                                      unsigned char,
                                                      unsigned char,
                                                      unsigned char,
                                                      unsigned char) __asm__("FUN_L08_0027b450");

void draw_flickering_mesh_effect(struct Moby *m) __asm__("FUN_L08_002f5e58");

void draw_flickering_mesh_effect(struct Moby *moby) {
    Vec4              corner[4], step, left_step, right_step, left, right, point, edge, normal;
    float             matrix[16];
    FlickerQuad       quad;
    Vec4              sv0, sv1, sv2, sv3, sv4;
    float             fraction;
    FlickerLightVars *flickering_mesh_vars;
    if (!moby || !(flickering_mesh_vars = (FlickerLightVars *)moby->pvars))
        return;
    {
        int            row_bytes = (flickering_mesh_vars->nx + 1) * sizeof(Vec4);
        Vec4           grid[flickering_mesh_vars->ny + 1][flickering_mesh_vars->nx + 1];
        unsigned char *points = (unsigned char *)grid;
        int            remaining;
        float          inv_x, inv_y;
        inv_x = 1.0f / convert_integer_to_float(flickering_mesh_vars->nx);
        inv_y = 1.0f / convert_integer_to_float(flickering_mesh_vars->ny);
        {
            float        u[flickering_mesh_vars->nx + 1], v[flickering_mesh_vars->ny + 1];
            float        phase, phase_step, amp, amp_step, amp2, amp2_step;
            float        phase0_step, phase1_step, time, z0, z1, bias;
            int          alpha, frame;
            float        row_noise, phase0, phase1;
            int          i, j;
            unsigned int color;
            time = convert_integer_to_float(l08_effect_animation_time);
            for (remaining = 0; remaining < 8; remaining++) {
                float *goal;
                if (flickering_mesh_vars->noise[remaining] <
                    *(goal = &flickering_mesh_vars->goal[remaining])) {
                    flickering_mesh_vars->noise[remaining] +=
                        flickering_mesh_vars->jitter * flickering_mesh_vars->noise_rate;
                    if (!(*goal <= flickering_mesh_vars->noise[remaining]))
                        continue;
                } else {
                    flickering_mesh_vars->noise[remaining] -=
                        flickering_mesh_vars->jitter * flickering_mesh_vars->noise_rate;
                    if (!(flickering_mesh_vars->noise[remaining] <= *goal))
                        continue;
                }
                *goal = random_float_between(1.0f - flickering_mesh_vars->jitter, 1.0f);
            }
            alpha = truncate_float_to_s32(flickering_mesh_vars->intensity *
                                          convert_integer_to_float(flickering_mesh_vars->alpha) *
                                          convert_integer_to_float(moby->alpha) * 0.0078125f) &
                    0xFF;
            if (flickering_mesh_vars->frames) {
                frame = l08_effect_animation_time;
                frame /= flickering_mesh_vars->frame_ticks + 1;
                flickering_mesh_vars->current =
                    flickering_mesh_vars->texture + frame % flickering_mesh_vars->frames;
            } else
                flickering_mesh_vars->current = flickering_mesh_vars->texture;
            if (flickering_mesh_vars->flags & 1)
                quad.gs[1] = get_effect_texture(flickering_mesh_vars->current);
            else
                quad.gs[1] = get_effect_texture(flickering_mesh_vars->current + 0x28);
            quad.gs[0] = 0;
            quad.gs[2] = 0xFF9000000260L;
            quad.gs[3] = (long)flickering_mesh_vars->blend0 |
                         ((long)flickering_mesh_vars->blend1 << 2) |
                         ((long)flickering_mesh_vars->blend2 << 4) |
                         ((long)flickering_mesh_vars->blend3 << 6) |
                         ((long)flickering_mesh_vars->alpha << 32);
            color = (flickering_mesh_vars->alpha << 24) | (flickering_mesh_vars->blue << 16) |
                    (flickering_mesh_vars->green << 8) | flickering_mesh_vars->red;
            quad.color[0] = color;
            quad.color[1] = color;
            quad.color[2] = color;
            quad.color[3] = color;
            qzero(&corner[0]);
            qzero(&corner[1]);
            qzero(&corner[2]);
            qzero(&corner[3]);
            {
                float half_width = flickering_mesh_vars->size_x * 0.5f;
                corner[1].f[0] = corner[3].f[0] = half_width;
                corner[0].f[0] = corner[2].f[0] = -half_width;
            }
            corner[0].f[2] = flickering_mesh_vars->z0 + 0.01f;
            corner[1].f[2] = flickering_mesh_vars->z1 + 0.01f;
            euler_rotation_matrix(matrix, &moby->rot);
            transform_vector(&corner[0], &corner[0], matrix);
            transform_vector(&corner[1], &corner[1], matrix);
            transform_vector(&corner[2], &corner[2], matrix);
            transform_vector(&corner[3], &corner[3], matrix);
            fast_vec_sub(&left_step, &corner[2], &corner[0]);
            fast_vec_scale(&left_step, &left_step, inv_y);
            fast_vec_sub(&right_step, &corner[3], &corner[1]);
            fast_vec_scale(&right_step, &right_step, inv_y);
            phase_step =
                (flickering_mesh_vars->phase_end - flickering_mesh_vars->phase_start) * inv_y;
            amp_step = (flickering_mesh_vars->amp_end - flickering_mesh_vars->amp_start) * inv_y;
            amp2_step = (flickering_mesh_vars->amp2_end - flickering_mesh_vars->amp2_start) * inv_y;
            phase0_step = (flickering_mesh_vars->phase0_end - flickering_mesh_vars->phase0) * inv_x;
            phase1_step = (flickering_mesh_vars->phase1_end - flickering_mesh_vars->phase1) * inv_y;
            z0 = flickering_mesh_vars->size_y - flickering_mesh_vars->z0;
            z1 = flickering_mesh_vars->size_y - flickering_mesh_vars->z1;
            fast_vec_sub(&edge, &corner[1], &corner[0]);
            fast_vec_cross(&normal, &edge, &left_step);
            fast_vec_normalize(&normal, &normal, 1.0f);
            qcopy(&left, &corner[0]);
            qcopy(&right, &corner[1]);
            bias = flickering_mesh_vars->noise[0];
            phase = flickering_mesh_vars->phase_start;
            amp = flickering_mesh_vars->amp_start;
            amp2 = flickering_mesh_vars->amp2_start;
            phase0 = flickering_mesh_vars->phase0;
            phase1 = flickering_mesh_vars->phase1;
            {
                /* Fakematch: 30 unused locals before i, so i+1 hashes after &data->puff_timer. */
                int pad0, pad1, pad2, pad3, pad4, pad5, pad6, pad7, pad8, pad9, pad10, pad11, pad12,
                    pad13, pad14, pad15, pad16, pad17, pad18, pad19, pad20, pad21, pad22, pad23,
                    pad24, pad25, pad26, pad27, pad28, pad29;
                int          i;
                register int j;
                for (i = 0; i <= flickering_mesh_vars->ny; phase += phase_step,
                    amp += amp_step,
                    amp2 += amp2_step,
                    phase0 += phase0_step,
                    phase1 += phase1_step,
                    i++) {
                    float y = convert_integer_to_float(i) /
                              convert_integer_to_float(flickering_mesh_vars->ny);
                    qcopy_nc(&point, &left);
                    j = 0;
                    fast_vec_sub(&step, &right, &left);
                    fast_vec_scale(&step, &step, inv_x);
                    row_noise = interpolate_sample_ring_data_first((char *)flickering_mesh_vars, y);
                    for (; j <= flickering_mesh_vars->nx; j++) {
                        float x = convert_integer_to_float(j) /
                                  convert_integer_to_float(flickering_mesh_vars->nx);
                        float theta;
                        float wave =
                            time * phase +
                            flickering_mesh_vars->noise_amp *
                                fast_sin(normalize_angle(flickering_mesh_vars->noise_speed * time));
                        float noise =
                            interpolate_sample_ring_data_first((char *)flickering_mesh_vars, x);
                        float angle0, angle1, shape, drop, a, b, normal_angle0, normal_angle1;
                        drop = z0 * x + z1 * (1.0f - x);
                        theta = x * 3.1415927f;
                        wave += noise * flickering_mesh_vars->noise_u +
                                row_noise * flickering_mesh_vars->noise_v +
                                bias * flickering_mesh_vars->noise_bias;
                        angle0 = normalize_angle(wave + phase0 + flickering_mesh_vars->wave_u * x +
                                                 flickering_mesh_vars->wave2_v * y);
                        angle1 = normalize_angle(wave + phase1 + flickering_mesh_vars->wave_v * y +
                                                 flickering_mesh_vars->wave2_u * x);
                        shape = fast_sin(theta);
                        shape = square_root(shape);
                        drop *= shape;
                        drop *= 1.0f - y;
                        fast_vec_normalize(&sv0, &step, amp * fast_cos(angle0) * shape * noise);
                        fast_vec_normalize(
                            &sv1, &left_step, amp2 * fast_sin(angle1) * shape * noise);
                        a = fast_sin(y * 3.1415927f);
                        b = fast_sin(theta);
                        normal_angle0 =
                            fast_sin(normalize_angle(wave + flickering_mesh_vars->wave_u * x));
                        normal_angle1 =
                            fast_sin(normalize_angle(wave + flickering_mesh_vars->wave2_v * y));
                        fast_vec_normalize(&sv3,
                                           &normal,
                                           a * b *
                                               (flickering_mesh_vars->normal_u * normal_angle0 +
                                                flickering_mesh_vars->normal_v * normal_angle1));
                        fast_vec_normalize(&sv4, &left_step, drop);
                        fast_vec_add(&sv2, &sv0, &sv1);
                        fast_vec_add(&sv2, &sv2, &sv3);
                        fast_vec_sub(&sv2, &sv2, &sv4);
                        fast_vec_add((Vec4 *)(points + i * row_bytes) + j, &point, &sv2);
                        *(float *)(points + (i * row_bytes + j * sizeof(Vec4)) + 12) =
                            1.0f / ((1.0f - flickering_mesh_vars->alpha_u +
                                     flickering_mesh_vars->alpha_u * shape * shape * shape) *
                                    (1.0f - flickering_mesh_vars->alpha_v +
                                     flickering_mesh_vars->alpha_v * y));
                        fast_vec_add(&point, &point, &step);
                    }
                    fast_vec_add(&left, &left, &left_step);
                    fast_vec_add(&right, &right, &right_step);
                }
            }
            {
                int   i;
                float f = split_float_fraction(
                    &fraction,
                    time * flickering_mesh_vars->u_rate +
                        flickering_mesh_vars->u_amp *
                            fast_sin(normalize_angle(time * flickering_mesh_vars->u_phase)));
                for (i = 0; i <= flickering_mesh_vars->nx; i++) {
                    u[i] = f;
                    f += inv_x / flickering_mesh_vars->texture_w_count;
                }
                f = split_float_fraction(
                    &fraction,
                    time * flickering_mesh_vars->v_rate +
                        flickering_mesh_vars->v_amp *
                            fast_sin(normalize_angle(time * flickering_mesh_vars->v_phase)));
                for (i = 0; i <= flickering_mesh_vars->ny; i++) {
                    v[i] = f;
                    f += inv_y / flickering_mesh_vars->texture_h_count;
                }
            }
            {
                int i;
                for (i = 0; i < flickering_mesh_vars->ny; i++) {
                    int j;
                    for (j = 0; j < flickering_mesh_vars->nx; j++) {
                        int          j1 = j + 1;
                        int          i1 = i + 1;
                        float       *vi = &v[i], *vi1 = &v[i1];
                        FlickerQuad *mesh_quad = &quad;
                        int          row_offset = i * row_bytes, next_offset = i1 * row_bytes;
                        Vec4        *row = (Vec4 *)(points + row_offset),
                             *next_row = (Vec4 *)(points + next_offset);
                        quad.uv[0] = u[j];
                        quad.uv[1] = *vi;
                        quad.uv[2] = u[j1];
                        quad.uv[3] = *vi;
                        quad.uv[4] = u[j];
                        quad.uv[5] = *vi1;
                        quad.uv[6] = u[j1];
                        quad.uv[7] = *vi1;
                        fast_vec_add(&mesh_quad->vertex[0], &row[j], &moby->pos);
                        fast_vec_add(&quad.vertex[1], &row[j + 1], &moby->pos);
                        fast_vec_add(&quad.vertex[2], &next_row[j], &moby->pos);
                        fast_vec_add(&quad.vertex[3], &next_row[j + 1], &moby->pos);
                        quad.color[0] =
                            ((alpha /
                              truncate_float_to_s32(
                                  *(float *)(points + (row_offset + (j) * sizeof(Vec4)) + 12)))
                             << 24) |
                            (flickering_mesh_vars->blue << 16) |
                            (flickering_mesh_vars->green << 8) | flickering_mesh_vars->red;
                        quad.color[1] =
                            ((alpha / truncate_float_to_s32(*(
                                          float *)(points + (row_offset + j1 * sizeof(Vec4)) + 12)))
                             << 24) |
                            (flickering_mesh_vars->blue << 16) |
                            (flickering_mesh_vars->green << 8) | flickering_mesh_vars->red;
                        quad.color[2] =
                            ((alpha /
                              truncate_float_to_s32(
                                  *(float *)(points + (next_offset + (j) * sizeof(Vec4)) + 12)))
                             << 24) |
                            (flickering_mesh_vars->blue << 16) |
                            (flickering_mesh_vars->green << 8) | flickering_mesh_vars->red;
                        quad.color[3] =
                            ((alpha /
                              truncate_float_to_s32(
                                  *(float *)(points + (next_offset + j1 * sizeof(Vec4)) + 12)))
                             << 24) |
                            (flickering_mesh_vars->blue << 16) |
                            (flickering_mesh_vars->green << 8) | flickering_mesh_vars->red;
                        draw_geometry_quad(mesh_quad, 0, 0);
                    }
                }
            }
            {
                /* Fakematch: a never-taken branch (the cmov folds at combine, jump2 deletes it).
               Retail had about 50 more insns at gcse time that vanish later; they size the PRE
               expression hash table (n_insns/2|1 = 583 buckets), whose walk order numbers the
               PRE pseudos and so orders the spill slots. 42 one-insn calls (41..44 equal). */
                int dead = 0;
                if (flickering_mesh_vars->flags & 0x8000)
                    dead = 0;
                if (dead) {
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                    font_queue_vu_state();
                }
            }
            if (flickering_mesh_vars->flags & 2) {
                if (fast_dec_timer(&flickering_mesh_vars->particle_timer)) {
                    flickering_mesh_vars->particle_timer =
                        scale_game_frames(flickering_mesh_vars->particle_ticks);
                    for (i = 0; i < flickering_mesh_vars->particle_count; i++) {
                        float width = flickering_mesh_vars->size_x * 0.5f;
                        float x = random_float_between(-1.0f, 1.0f) *
                                  flickering_mesh_vars->particle_range;
                        float fraction = random_float_between(0.0f, x);
                        float size;
                        short life;
                        int   r, g, b, a;
                        Vec4 *particle_motion = &sv0;
                        fast_vec_normalize(&sv2,
                                           &left_step,
                                           -(flickering_mesh_vars->particle_offset +
                                             square_root(fast_cos(fraction * 3.1415927f)) *
                                                 flickering_mesh_vars->particle_height));
                        fast_vec_normalize(&sv1, &step, fraction * width);
                        fast_vec_add(&sv1, &sv1, &moby->pos);
                        fast_vec_add(&sv1, &sv1, &sv2);
                        sv0.f[0] = random_float_between(-flickering_mesh_vars->particle_xy,
                                                        flickering_mesh_vars->particle_xy) *
                                   frame_scale;
                        sv0.f[1] = random_float_between(-flickering_mesh_vars->particle_xy,
                                                        flickering_mesh_vars->particle_xy) *
                                   frame_scale;
                        sv0.f[2] = random_float_between(0.0f, flickering_mesh_vars->particle_z) *
                                   frame_scale;
                        size = random_float_between(flickering_mesh_vars->particle_size0,
                                                    flickering_mesh_vars->particle_size1);
                        life = scale_game_frames(flickering_mesh_vars->particle_life);
                        r = flickering_mesh_vars->particle_r;
                        g = flickering_mesh_vars->particle_g;
                        b = flickering_mesh_vars->particle_b;
                        a = truncate_float_to_s32(
                            convert_integer_to_float(flickering_mesh_vars->particle_a) * fraction);
                        spawn_colored_effect_size_first(
                            moby, &sv1, particle_motion, size, life, r, g, b, a);
                    }
                }
                if (fast_dec_timer_s16(&flickering_mesh_vars->puff_timer)) {
                    flickering_mesh_vars->puff_timer =
                        scale_game_frames(flickering_mesh_vars->puff_ticks);
                    for (i = 0; i < flickering_mesh_vars->puff_count; i++) {
                        int puff_spawn_color = (flickering_mesh_vars->puff_c0[3] << 24) |
                                               (flickering_mesh_vars->puff_c0[2] << 16) |
                                               (flickering_mesh_vars->puff_c0[1] << 8) |
                                               flickering_mesh_vars->puff_c0[0];
                        int puff_payload_color = (flickering_mesh_vars->puff_c1[3] << 24) |
                                                 (flickering_mesh_vars->puff_c1[2] << 16) |
                                                 (flickering_mesh_vars->puff_c1[1] << 8) |
                                                 flickering_mesh_vars->puff_c1[0];
                        float width = flickering_mesh_vars->size_x * 0.5f;
                        float size = random_float_between(flickering_mesh_vars->puff_size0,
                                                          flickering_mesh_vars->puff_size1);
                        int   life = scale_game_frames(random_int_between(
                            flickering_mesh_vars->puff_life0, flickering_mesh_vars->puff_life1));
                        float fraction =
                            random_float_between(-1.0f, 1.0f) * flickering_mesh_vars->puff_range;
                        float shape;
                        fraction = random_float_between(-fraction, fraction);
                        shape = square_root(fast_cos(fraction * 3.1415927f));
                        fast_vec_normalize(&sv2,
                                           &left_step,
                                           -(flickering_mesh_vars->puff_offset +
                                             shape * flickering_mesh_vars->puff_height));
                        fast_vec_normalize(&sv1, &step, fraction * width);
                        fast_vec_add(&sv1, &sv1, &moby->pos);
                        fast_vec_add(&sv1, &sv1, &sv2);
                        sv0.f[0] = random_float_between(-flickering_mesh_vars->puff_xy,
                                                        flickering_mesh_vars->puff_xy) *
                                   frame_scale;
                        sv0.f[1] = random_float_between(-flickering_mesh_vars->puff_xy,
                                                        flickering_mesh_vars->puff_xy) *
                                   frame_scale;
                        sv0.f[2] =
                            random_float_between(0.0f, flickering_mesh_vars->puff_z) * frame_scale;
                        fast_vec_scale(&sv0,
                                       &sv0,
                                       1.0f - flickering_mesh_vars->puff_blend +
                                           shape * flickering_mesh_vars->puff_blend);
                        spawn_effect_particle_with_target_scalar(
                            &sv1, &sv0, puff_spawn_color, puff_payload_color, size, life);
                    }
                }
                if (flickering_mesh_vars->light != -1) {
                    PointLight *light = &l08_point_light_slots[flickering_mesh_vars->light];
                    if (fast_vec_dist(&flickering_mesh_vars->light_pos, &light->pos) <
                        flickering_mesh_vars->light_speed * frame_scale) {
                        float x = (random_float_between(-(int)flickering_mesh_vars->light_range_x,
                                                        flickering_mesh_vars->light_range_x) +
                                   flickering_mesh_vars->size_x) *
                                  0.5f;
                        float y = (random_float_between(-(int)flickering_mesh_vars->light_range_y,
                                                        flickering_mesh_vars->light_range_y) +
                                   flickering_mesh_vars->size_y) *
                                  0.5f;
                        fast_vec_normalize(&sv0, &step, x);
                        fast_vec_normalize(&sv1, &left_step, -y);
                        fast_vec_add(&flickering_mesh_vars->light_pos, &sv0, &sv1);
                        fast_vec_add(&flickering_mesh_vars->light_pos,
                                     &flickering_mesh_vars->light_pos,
                                     &corner[0]);
                        fast_vec_add(&flickering_mesh_vars->light_pos,
                                     &flickering_mesh_vars->light_pos,
                                     &moby->pos);
                    } else {
                        fast_vec_sub(&sv0, &flickering_mesh_vars->light_pos, &light->pos);
                        fast_vec_normalize(
                            &sv0, &sv0, flickering_mesh_vars->light_speed * frame_scale);
                        fast_vec_add(&light->pos, &light->pos, &sv0);
                        light->pos.f[3] = convert_integer_to_float(flickering_mesh_vars->radius);
                    }
                }
                flickering_mesh_vars->flags ^= 2;
            }
        }
    }
}

extern char flickering_mesh_texture_width_zero_message[] __asm__("D_L08_001FC220");
char        D_L08_001FC250[] =
    "M671 Warning: texture h count (%f) is greater than max texture tile count (8)\n";
extern char flickering_mesh_texture_height_zero_message[] __asm__("D_L08_001FC2A0");
extern char flickering_mesh_texture_width_tile_limit_warning[] __asm__("D_L08_001FC2D0");
extern s32  DebugPrint();
extern int  allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");

void flickering_mesh_effect_update(struct Moby *moby) __asm__("FUN_L08_002f70a0");

/* Moby class 671 */
void flickering_mesh_effect_update(struct Moby *moby) {
    FlickerLightVars *light_vars;
    Vec4              pos;
    f32               view, draw_fade_distance, fade;
    s32               hidden;
    PointLight       *point_light_slot;
    Vec4f            *at;
    int               i;

    if (moby == NULL)
        return;
    light_vars = (FlickerLightVars *)moby->pvars;
    if (light_vars == NULL)
        return;
    hidden = 0;
    at = &moby->pos;
    if (moby->state == 0) {
        light_vars->light = -1;
        light_vars->particle_timer = light_vars->particle_ticks;
        qcopy(&light_vars->light_pos, at);
        if (light_vars->texture_w_count == 0.0f) {
            DebugPrint(flickering_mesh_texture_width_zero_message);
            light_vars->texture_w_count = 1.0f;
        } else if (8.0f < light_vars->texture_h_count) {
            DebugPrint(D_L08_001FC250, light_vars->texture_h_count);
        }
        if (light_vars->texture_h_count == 0.0f) {
            DebugPrint(flickering_mesh_texture_height_zero_message);
            light_vars->texture_h_count = 1.0f;
        } else if (8.0f < light_vars->texture_w_count) {
            DebugPrint(flickering_mesh_texture_width_tile_limit_warning,
                       light_vars->texture_w_count);
        }
        for (i = 0; i < 8; i++) {
            light_vars->noise[i] = random_float_between(light_vars->jitter, 1.0f);
            light_vars->goal[i] = random_float_between(light_vars->jitter, 1.0f);
        }
        moby->state = 1;
    }
    moby->scale =
        moby->pclass->scale *
        (light_vars->size_x > light_vars->size_y ? light_vars->size_x : light_vars->size_y) * 0.25f;
    view = convert_integer_to_float(moby->draw_dist);
    draw_fade_distance = fast_vec_dist(at, D_L08_001675C0);
    fade = view - light_vars->fade_range;
    if (fade < 0.0f)
        fade = 0.0f;
    if (view < draw_fade_distance) {
        light_vars->intensity = 0.0f;
        hidden = 1;
    } else {
        if (fade < draw_fade_distance) {
            draw_fade_distance -= fade;
            draw_fade_distance /= light_vars->fade_range;
            fade = 1.0f - draw_fade_distance;
        } else
            fade = 1.0f;
        light_vars->intensity = fade;
    }
    pos.q = *(u128 *)at;
    pos.f[3] = convert_integer_to_float(light_vars->radius);
    if ((moby->visible != 0 || fast_bsphere_check((char *)&pos, 255.0f) != -1) && !hidden) {
        add_draw_callback(draw_flickering_mesh_effect, moby);
        if (light_vars->flags & 4) {
            if (light_vars->light == -1) {
                light_vars->light = allocate_colored_effect_slot(
                    light_vars->light_pos.f,
                    (f32)light_vars->radius,
                    convert_integer_to_float(light_vars->falloff) / 100.0f,
                    light_vars->intensity * convert_integer_to_float(light_vars->light_red) /
                        100.0f,
                    light_vars->intensity * convert_integer_to_float(light_vars->light_green) /
                        100.0f,
                    light_vars->intensity * convert_integer_to_float(light_vars->light_blue) /
                        100.0f);
            } else {
                point_light_slot = &l08_point_light_slots[light_vars->light];
                point_light_slot->red = light_vars->intensity *
                                        convert_integer_to_float(light_vars->light_red) / 100.0f;
                point_light_slot->green = light_vars->intensity *
                                          convert_integer_to_float(light_vars->light_green) /
                                          100.0f;
                point_light_slot->blue = light_vars->intensity *
                                         convert_integer_to_float(light_vars->light_blue) / 100.0f;
            }
        }
    } else if (light_vars->light != -1) {
        release_point_light_slot(light_vars->light);
        light_vars->light = -1;
    }
    light_vars->flags |= 2;
}
