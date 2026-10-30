/* Effect particles: the update functions of the three-phase, drifting, damped, colliding, rotating and fading types. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "macros.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/ui/text/load_display_text_resource_entry.h"

typedef struct {
    char  p0[8];
    int   w8;
    int   wC;
    short h10, h12, h14, h16, h18;
} S_269290;

extern int  D_15F5CC_269290 __asm__("D_L00_0015F5CC");
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void free_effect_particle(void *) __asm__("FUN_L00_00267a08");

void update_three_phase_effect_particle(char *particle) __asm__("FUN_L00_00269290");

void update_three_phase_effect_particle(char *particle) {
    S_269290 *particle_data = (S_269290 *)(particle + 0x20);
    float     position_step[4] __attribute__((aligned(16)));
    float     initial_motion_step[4] __attribute__((aligned(16)));
    float     final_motion_step[4] __attribute__((aligned(16)));
    float     phase_remaining_ratio;

    if (D_15F5CC_269290 & 1) {
        *(unsigned char *)(particle + 8) += 1;
    }
    if (particle_data->h10 != 0) {
        *(volatile int *)0x10000000 = 0;
        load_display_text_resource_entry(position_step, particle_data);
        fast_vec_add(particle + 0x10, particle + 0x10, position_step);
        phase_remaining_ratio = convert_integer_to_float(*(short *)(particle + 0xA)) /
                                convert_integer_to_float(particle_data->h10);
        *(int *)(particle + 4) = fast_tween_color(
            particle_data->w8 & 0xFFFFFF, particle_data->w8, 1.0f - phase_remaining_ratio);
        *(float *)(particle + 0xC) = (float)(particle_data->h16 * 1000);
        if (fast_dec_timer_s16(particle + 0xA)) {
            particle_data->h10 = 0;
            *(short *)(particle + 0xA) = particle_data->h12;
        }
    } else if (particle_data->h12 != 0) {
        load_display_text_resource_entry(initial_motion_step, particle_data);
        load_display_text_resource_entry(final_motion_step, particle + 0x24);
        phase_remaining_ratio = convert_integer_to_float(*(short *)(particle + 0xA)) /
                                convert_integer_to_float(particle_data->h12);
        lerp_vector(position_step, final_motion_step, initial_motion_step, phase_remaining_ratio);
        fast_vec_add(particle + 0x10, particle + 0x10, position_step);
        *(float *)(particle + 0xC) =
            ((float)(particle_data->h16 - particle_data->h18) * phase_remaining_ratio +
             (float)particle_data->h18) *
            1000.0f;
        *(int *)(particle + 4) =
            fast_tween_color(particle_data->wC, particle_data->w8, phase_remaining_ratio);
        if (fast_dec_timer_s16(particle + 0xA)) {
            particle_data->h12 = 0;
            *(short *)(particle + 0xA) = particle_data->h14;
        }
    } else {
        int packed_rgb;
        load_display_text_resource_entry(position_step, particle + 0x24);
        fast_vec_add(particle + 0x10, particle + 0x10, position_step);
        packed_rgb = particle_data->wC & 0xFFFFFF;
        *(int *)(particle + 4) =
            fast_tween_color(packed_rgb,
                             particle_data->wC,
                             convert_integer_to_float(*(short *)(particle + 0xA)) /
                                 convert_integer_to_float(particle_data->h14));
        *(float *)(particle + 0xC) = (float)(particle_data->h18 * 1000);
        if (fast_dec_timer_s16(particle + 0xA)) {
            free_effect_particle(particle);
        }
    }
}

#include "qcopy.h"

/* part type 3 update: spawn, drift toward a target, fade, and expire */
/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026A398), where it is exact; names translated to the US level program. */

extern char  D_L00_00166DC0[];
extern float D_L00_001CB7C0[];
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern int   D_L00_0015F5CC;
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int   FUN_L00_00257b90(int, int);
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");

void update_drifting_effect_particle_with_distance_fade(char *particle) __asm__("FUN_L00_002694f8");

void update_drifting_effect_particle_with_distance_fade(char *particle) {
    char  *particle_data = particle + 0x20;
    float  position_scratch[4];
    float  reference_distance, reference_angle_delta;
    int    collision_check_counter, remaining_lifetime, k;
    float *drift_parameters;
    char  *reference_position;
    char  *B;
    char  *particle_position;
    collision_check_counter = *(int *)(particle_data + 0x1C);
    if (collision_check_counter >= 3) {
        remaining_lifetime = *(short *)(particle + 0xA);
        particle_position = particle + 0x10;
        if (remaining_lifetime > scale_game_frames(0x78) &&
            coll_sphere(7.99f, particle_position, 0x12, 0)) {
            *(int *)(particle_data + 0x14) = 1;
            *(short *)(particle + 0xA) = scale_game_frames(0x78);
            *(int *)(particle_data + 0x1C) = -1;
        } else {
            *(int *)(particle_data + 0x1C) = 0;
        }
    } else {
        particle_position = particle + 0x10;
        if (collision_check_counter < 0)
            goto after;
        *(int *)(particle_data + 0x1C) = collision_check_counter + 1;
    }
    if (*(int *)(particle_data + 0x1C) >= 0) {
        drift_parameters = D_L00_001CB7C0;
        *(float *)particle_data = fast_cos(drift_parameters[1]) * drift_parameters[5] / 2.5f;
        *(float *)(particle_data + 4) = fast_sin(drift_parameters[1]) * drift_parameters[5] / 2.5f;
        *(int *)(particle_data + 8) = 0;
        fast_vec_add(particle_position, particle_position, particle_data);
    }
after:
    reference_position = D_L00_00166DC0;
    B = reference_position - 0x140;
    reference_distance = fast_vec_dist(particle_position, reference_position);
    reference_angle_delta =
        fast_subtract_rotations(fast_arc_tan(*(float *)(particle + 0x10) - *(float *)(B + 0x140),
                                             *(float *)(particle + 0x14) - *(float *)(B + 0x144)),
                                *(float *)(B + 0x158));
    if (absolute_float(reference_angle_delta) > 1.0471976f) {
        if ((*(int *)(particle_data + 0x18))++ >= 6)
            goto kill;
        position_scratch[0] =
            fast_cos(fast_add_rotations(*(float *)(B + 0x158),
                                        reference_angle_delta > 0.0f ? -1.0471976f : 1.0471976f)) *
            reference_distance;
        position_scratch[1] =
            fast_sin(fast_add_rotations(*(float *)(B + 0x158),
                                        reference_angle_delta > 0.0f ? -1.0471976f : 1.0471976f)) *
            reference_distance;
        position_scratch[2] = *(float *)(particle + 0x18) - *(float *)(B + 0x148);
        fast_vec_add(particle_position, position_scratch, reference_position);
        qcopy(position_scratch, particle_position);
        position_scratch[2] = position_scratch[2] + 32.0f;
        if (collision_line(particle_position, position_scratch, 0x12, 0, 0))
            goto kill;
    }
    if ((D_L00_0015F5CC & 3) == 0)
        particle[8]++;
    remaining_lifetime = *(short *)(particle + 0xA);
    if (scale_game_frames(0x78) >= remaining_lifetime) {
        if (*(int *)(particle_data + 0x14)) {
            k = *(short *)(particle + 0xA) * 0x14 / scale_game_frames(0x78);
        } else {
            k = (scale_game_frames(0x78) - *(short *)(particle + 0xA)) * 0x14 /
                scale_game_frames(0x78);
        }
    } else {
        k = 0x14;
    }
    if (reference_distance < 8.0f || reference_distance > 96.0f)
        goto kill;
    if (reference_distance < 16.0f) {
        k = truncate_float_to_s32(convert_integer_to_float(k) * (reference_distance - 8.0f) *
                                  0.125f);
    } else if (reference_distance > 80.0f) {
        k = truncate_float_to_s32(convert_integer_to_float(k) * (96.0f - reference_distance) *
                                  0.0625f);
    }
    *(int *)(particle + 4) = (k << 24) | (*(int *)(particle + 4) & 0xFFFFFF);
    if (fast_dec_timer_s16(particle + 0xA)) {
        if (*(int *)(particle_data + 0x14)) {
        kill:
            **(char **)(particle_data + 0x10) = 0;
            free_effect_particle(particle);
        } else {
            *(int *)(particle_data + 0x14) = 1;
            *(short *)(particle + 0xA) =
                FUN_L00_00257b90(scale_game_frames(0x78) * 3, scale_game_frames(0x78) * 9);
        }
    }
}

#include "eetypes.h"

unsigned char                       *FUN_L00_002678b8_00269958(int) __asm__("FUN_L00_002678b8");
int                                  FUN_001160d8_00269958(void) __asm__("FUN_001160d8");
extern unsigned char *D_L00_001B2090 NOT_SDA;
void FUN_L00_00269958(u128 *a, u128 *b, int c, int d, int n, int s1, int s2, int flag);
void FUN_L00_00269958(u128 *spawn_position,
                      u128 *initial_velocity,
                      int   initial_color,
                      int   color_endpoint,
                      int   lifetime,
                      int   scalar_base,
                      int   scalar_delta,

                      int flag) {
    unsigned char *particle = FUN_L00_002678b8_00269958(4);
    if (particle) {
        int           *particle_data;
        unsigned char *particle_position;
        qcopy(particle + 0x10, spawn_position);
        particle_position = particle + 0x10;
        *(int *)(particle + 4) = initial_color;
        particle[9] = truncate_float_to_s32(4.0f) - 0x60;
        particle[3] = flag ? 0x48 : 0x44;
        particle[1] = 0;
        particle[2] = *D_L00_001B2090;
        particle[8] = FUN_001160d8_00269958();
        particle_data = (int *)(particle + 0x20);
        *(int *)(particle + 0xc) = 0;
        qcopy(particle_position, spawn_position);
        qcopy(particle_data, initial_velocity);
        *(short *)(particle + 0xa) = lifetime;
        particle_data[7] = lifetime;
        particle_data[4] = initial_color;
        particle_data[5] = color_endpoint;
        *(short *)((char *)particle_data + 0x18) = scalar_delta;
        *(short *)((char *)particle_data + 0x1a) = scalar_base;
    }
}

typedef struct {
    f32 v[4];
    s32 x10;
    s32 x14;
    s16 x18;
    s16 x1a;
    s32 x1c;
} S00269a70;

typedef struct {
    u8        p0[4];
    s32       x4;
    u8        x8;
    u8        p9;
    s16       xa;
    f32       xc;
    u8        p10[0x10];
    S00269a70 s;
} O00269a70;

extern f32 D_0015ED60;
s32        mine_blend_color(s32, s32, f32) __asm__("FUN_L00_002371e0");
void       free_effect_particle(void *) __asm__("FUN_L00_00267a08");
void       update_damped_effect_particle(O00269a70 *particle) __asm__("FUN_L00_00269a70");

void update_damped_effect_particle(O00269a70 *particle) {
    S00269a70 *s = &particle->s;
    particle->xc =
        convert_integer_to_float(s->x18 * (s->x1c - particle->xa) / s->x1c + s->x1a) * 1000.0f;
    fast_vec_scale(s, s, D_0015ED60 * -0.019999980926513672f + 1.0f);
    fast_vec_add(particle->p10, particle->p10, s);
    particle->x8++;
    particle->x4 =
        mine_blend_color(s->x14, s->x10, particle->xa / convert_integer_to_float(s->x1c));
    if (fast_dec_timer_s16(&particle->xa))
        free_effect_particle(particle);
}

/* spawns a type-5 particle at pos with packed color and a target */
/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026AA10), where it is exact; names translated to the US level program. */

extern int D_L00_001B2094 NOT_SDA;
extern void              *create_part(int) __asm__("FUN_L00_002678b8");

void FUN_L00_00269b70(void         *spawn_position,
                      unsigned char red,
                      unsigned char green,
                      unsigned char blue,
                      int           target,
                      float         particle_scalar_change,

                      float initial_particle_scalar) {
    char  *particle;
    float *particle_data;
    if (target) {
        particle = create_part(5);
        if (particle) {
            qcopy(particle + 0x10, spawn_position);
            *(int *)(particle + 4) = 0x7F000000 | (blue << 16) | (green << 8) | red;
            particle_data = (float *)(particle + 0x20);
            particle[9] = truncate_float_to_s32(4.0f) - 0x60;
            particle[3] = 0x48;
            *(float *)(particle + 0xC) = initial_particle_scalar;
            particle[1] = 0;
            particle[8] = 0;
            particle[2] = **(unsigned char **)&D_L00_001B2094;
            *(int *)(particle + 0x20) = target;
            particle_data[1] = particle_scalar_change / convert_integer_to_float(target);
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026AB10), where it is exact; names translated to the US level program. */

void update_scalar_and_fade_effect_particle(char *particle) __asm__("FUN_L00_00269c70");

void update_scalar_and_fade_effect_particle(char *particle) {
    float *particle_data;
    particle_data = (float *)(particle + 0x20);
    if (fast_dec_timer(particle_data)) {
        free_effect_particle(particle);
        return;
    }
    *(float *)(particle + 0xC) += particle_data[1];
    if (scale_game_frames(6) > *(int *)(particle + 0x20)) {
        *(int *)(particle + 4) = (*(int *)(particle + 4) & 0xFFFFFF) |
                                 ((*(int *)(particle + 0x20) * 127 / scale_game_frames(6)) << 24);
    }
}

extern int particle_collision_269d20(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void
subtract_homogeneous_scaled_vector_xyz(float, void *, void *, void *) __asm__("FUN_L00_0025c2b0");
extern void  particle_matrix_269d20(float, void *, void *) __asm__("FUN_001f9bf8");
extern float D_0015ED60_269d20 __asm__("D_0015ED60");

#include "rnc/overlay/collision.h"

void update_colliding_effect_particle(unsigned char *m) __asm__("FUN_L00_00269d20");

void update_colliding_effect_particle(unsigned char *m) {
    unsigned char *p = m + 0x20;
    unsigned char *e = *(unsigned char **)(p + 0xc);
    float          next[4] __attribute__((aligned(16)));
    float          matrix[4] __attribute__((aligned(16)));
    float          velocity[4] __attribute__((aligned(16)));
    float          collision[4] __attribute__((aligned(16)));
    float          rotation[16] __attribute__((aligned(16)));
    float          red, green, blue, limit;
    int            color;
    next[0] = *(float *)(m + 0x10) + *(float *)(p + 0x0);
    next[1] = *(float *)(m + 0x14) + *(float *)(p + 0x4);
    next[2] = *(float *)(m + 0x18) + *(float *)(p + 0x8);
    if ((*(unsigned int *)(e + 0x70) & 1) && particle_collision_269d20(m + 0x10, next, 4, m, 0)) {
        velocity[0] = *(float *)(p + 0x0);
        velocity[1] = *(float *)(p + 0x4);
        velocity[2] = *(float *)(p + 0x8);
        subtract_homogeneous_scaled_vector_xyz(
            *(float *)(e + 0x3c), matrix, velocity, coll_hit_normal.f);
        *(float *)(p + 0x0) = matrix[0];
        *(float *)(p + 0x4) = matrix[1];
        *(float *)(p + 0x8) = matrix[2];
        particle_matrix_269d20(0.055f, collision, coll_hit_normal.f);
        fast_vec_add(m + 0x10, coll_hit_normal.f - 8, collision);
    } else {
        /* Fakematch: retail reads $s0 on this path before setting it, so reorg sees it
           live at the else label and fills the beqz delay slot like retail. */
        REG_LIVE("$16");
        *(u128 *)(m + 0x10) = *(u128 *)next;
    }
    if (*(unsigned int *)(e + 0x70) & 2) {
        red = convert_integer_to_float((m[4] << 8) | p[0x18]) / 65535.0f;
        green = (float)truncate_float_to_s32((int)((*(unsigned int *)(m + 4) & 0xff00) | p[0x19])) /
                65535.0f;
        blue = (float)truncate_float_to_s32(
                   (int)(((*(unsigned int *)(m + 4) >> 8) & 0xff00) | p[0x1a])) /
               65535.0f;
        {
            float step;
            if (p[0x1b] & 0x10) {
                red -= *(float *)(e + 0x4c) * D_0015ED60_269d20;
                step = D_0015ED60_269d20;
            } else {
                red += *(float *)(e + 0x4c) * D_0015ED60_269d20;
                step = D_0015ED60_269d20;
            }
            if (p[0x1b] & 0x20)
                green -= *(float *)(e + 0x50) * step;
            else
                green += *(float *)(e + 0x50) * step;
        }
        if (p[0x1b] & 0x40)
            blue -= *(float *)(e + 0x54) * frame_scale;
        else
            blue += *(float *)(e + 0x54) * frame_scale;
        limit = *(float *)(e + 0x64);
        if (limit < red) {
            red = limit;
            if (*(unsigned int *)(e + 0x70) & 0x100)
                p[0x1b] ^= 0x10;
        }
        limit = *(float *)(e + 0x58);
        if (red < limit) {
            red = limit;
            if (*(unsigned int *)(e + 0x70) & 0x10)
                p[0x1b] ^= 0x10;
        }
        limit = *(float *)(e + 0x68);
        if (limit < green) {
            green = limit;
            if (*(unsigned int *)(e + 0x70) & 0x200)
                p[0x1b] ^= 0x20;
        }
        limit = *(float *)(e + 0x5c);
        if (green < limit) {
            green = limit;
            if (*(unsigned int *)(e + 0x70) & 0x20)
                p[0x1b] ^= 0x20;
        }
        limit = *(float *)(e + 0x6c);
        if (limit < blue) {
            blue = limit;
            if (*(unsigned int *)(e + 0x70) & 0x400)
                p[0x1b] ^= 0x40;
        }
        limit = *(float *)(e + 0x60);
        if (blue < limit) {
            blue = limit;
            if (*(unsigned int *)(e + 0x70) & 0x40)
                p[0x1b] ^= 0x40;
        }
        {
            /* Fakematch: retail keeps 65535.0f in $f21 and 255.0f in $f20. */
            register float to_u16 __asm__("$f21") = 65535.0f;
            register float to_u8 __asm__("$f20") = 255.0f;
            p[0x18] = truncate_float_to_s32(red * to_u16);
            p[0x19] = truncate_float_to_s32(green * to_u16);
            p[0x1a] = truncate_float_to_s32(blue * to_u16);
            color = (truncate_float_to_s32(red * to_u8) & 0xff) |
                    ((truncate_float_to_s32(green * to_u8) & 0xff) << 8) |
                    ((truncate_float_to_s32(blue * to_u8) & 0xff) << 16) |
                    (truncate_float_to_s32(*(float *)(p + 0x10) * to_u8) << 24);
        }
        *(int *)(m + 4) = color;
    } else if (*(unsigned int *)(e + 0x70) & 4) {
        color = truncate_float_to_s32(*(float *)(p + 0x10) * 255.0f);
        *(unsigned int *)(m + 4) = (*(unsigned int *)(m + 4) & 0xffffff) | (color << 24);
    }
    if (*(unsigned int *)(e + 0x70) & 4) {
        if (p[0x1b] & 0x80)
            *(float *)(p + 0x10) -= *(float *)(e + 0x38) * frame_scale;
        else
            *(float *)(p + 0x10) += *(float *)(e + 0x38) * frame_scale;
        if (*(float *)(p + 0x10) > *(float *)(e + 0x48)) {
            *(float *)(p + 0x10) = *(float *)(e + 0x48);
            if (*(unsigned int *)(e + 0x70) & 0x800)
                p[0x1b] ^= 0x80;
        }
        if (*(float *)(p + 0x10) < *(float *)(e + 0x44)) {
            *(float *)(p + 0x10) = *(float *)(e + 0x44);
            if (*(unsigned int *)(e + 0x70) & 0x80)
                p[0x1b] ^= 0x80;
        }
    }
    m[8] = truncate_float_to_s32(*(float *)(p + 0x14) * 255.0f);
    if (*(unsigned int *)(e + 0x70) & 8) {
        next[0] = *(float *)(p + 0x0);
        next[1] = *(float *)(p + 0x4);
        next[2] = *(float *)(p + 0x8);
        if (*(unsigned int *)(e + 0x70) & 0x8000) {
            euler_rotation_matrix(rotation, *(void **)(e + 0x7c) + 0x40);
            transform_vector(matrix, e, rotation);
        } else {
            *(u128 *)matrix = *(u128 *)e;
        }
        fast_vec_add(next, next, matrix);
        clamp_vector_length(next, next, *(float *)(e + 0x40));
        *(float *)(p + 0x0) = next[0];
        *(float *)(p + 0x4) = next[1];
        *(float *)(p + 0x8) = next[2];
    }
    *(float *)(m + 0xc) += *(float *)(e + 0x30) * D_0015ED60_269d20;
    if (*(float *)(m + 0xc) < *(float *)(e + 0x74))
        *(float *)(m + 0xc) = *(float *)(e + 0x74);
    if (*(float *)(m + 0xc) > *(float *)(e + 0x78))
        *(float *)(m + 0xc) = *(float *)(e + 0x78);
    *(float *)(p + 0x14) += *(float *)(e + 0x34) * D_0015ED60_269d20;
    if (*(float *)(p + 0x10) <= 0.0f || fast_dec_timer_s16(m + 0xa) ||
        *(float *)(m + 0x10) < *(float *)(e + 0x10) ||
        *(float *)(m + 0x14) < *(float *)(e + 0x14) ||
        *(float *)(m + 0x18) < *(float *)(e + 0x18) ||
        *(float *)(e + 0x20) < *(float *)(m + 0x10) ||
        *(float *)(e + 0x24) < *(float *)(m + 0x14) || *(float *)(e + 0x28) < *(float *)(m + 0x18))
        free_effect_particle(m);
}

#include "rnc/overlay/quad.h"

extern unsigned char *D_L00_001B20A0_0026a4c8 __asm__("D_L00_001B20A0")
    __attribute__((section(".data")));
unsigned char *FUN_L00_002678b8_0026a4c8(int) __asm__("FUN_L00_002678b8");
void           FUN_L00_0026a4c8(u128 *a, u128 *b, int c, int d, int n, float f);
void           FUN_L00_0026a4c8(u128 *spawn_position,
                                u128 *initial_velocity,
                                int   initial_color,
                                int   final_color,
                                int   lifetime,

                                float particle_scalar_change) {
    if (lifetime) {
        unsigned char *particle = FUN_L00_002678b8_0026a4c8(8);
        if (particle) {
            int *particle_data;
            qcopy(particle + 0x10, spawn_position);
            particle_data = (int *)(particle + 0x20);
            *(int *)(particle + 4) = initial_color;
            particle[9] = truncate_float_to_s32(4.0f) - 0x60;
            particle[3] = 0x48;
            particle[1] = 0;
            *(int *)(particle + 0xc) = 0;
            particle[8] = 0;
            particle[2] = *D_L00_001B20A0_0026a4c8;
            qcopy(particle_data, initial_velocity);
            *(short *)(particle + 0xa) = lifetime;
            particle_data[6] = lifetime;
            *(float *)&particle_data[7] =
                particle_scalar_change / convert_integer_to_float(lifetime);
            particle_data[4] = initial_color;
            particle_data[5] = final_color;
        }
    }
}

extern unsigned char *D_L00_001B20A0_0026a5b8 __asm__("D_L00_001B20A0")
    __attribute__((section(".data")));
void FUN_L00_00267a08_0026a5b8(unsigned char *) __asm__("FUN_L00_00267a08");
int  FUN_001fa6e0_0026a5b8(int, int, float) __asm__("FUN_001fa6e0");
int  FUN_001f96f8_0026a5b8(int) __asm__("FUN_001f96f8");
void update_animated_fading_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026a5b8");

void update_animated_fading_effect_particle(unsigned char *particle) {
    int *particle_data = (int *)(particle + 0x20);
    fast_vec_add(particle + 0x10, particle + 0x10, particle_data);
    if (fast_dec_timer_s16(particle + 0xa)) {
        FUN_L00_00267a08_0026a5b8(particle);
        return;
    }
    *(float *)(particle + 0xc) += *(float *)&particle_data[7];
    *(int *)(particle + 4) = FUN_001fa6e0_0026a5b8(particle_data[5],
                                                   particle_data[4],
                                                   *(short *)(particle + 0xa) /
                                                       convert_integer_to_float(particle_data[6]));
    if (*(short *)(particle + 0xa) < FUN_001f96f8_0026a5b8(6)) {
        unsigned char alpha_before_fade = particle[7];
        *(int *)(particle + 4) =
            (*(int *)(particle + 4) & 0xFFFFFF) |
            ((*(short *)(particle + 0xa) * alpha_before_fade / FUN_001f96f8_0026a5b8(6)) << 24);
    }
    particle[2] = D_L00_001B20A0_0026a5b8[(particle_data[6] - *(short *)(particle + 0xa)) * 10 /
                                          particle_data[6]];
}

void FUN_L00_00267a08_0026a6f0(unsigned char *) __asm__("FUN_L00_00267a08");
int  FUN_001f96f8_0026a6f0(int) __asm__("FUN_001f96f8");

#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle_globals.h"

extern int rotating_grayscale_particle_fade_reference_duration_gp __asm__("D_L00_001601A4")
    __attribute__((sda));
void update_rotating_grayscale_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026a6f0");

void update_rotating_grayscale_effect_particle(unsigned char *particle) {
    int *particle_data = (int *)(particle + 0x20);
    int  faded_alpha;
    if (fast_dec_timer(particle_data)) {
        FUN_L00_00267a08_0026a6f0(particle);
        return;
    }
    *(float *)&particle_data[3] =
        fast_add_rotations(*(float *)&particle_data[3], *(float *)&particle_data[2]);
    particle[8] = truncate_float_to_s32(*(float *)&particle_data[3] * 128.0f / 3.1415927f) - 0x80;
    if (FUN_001f96f8_0026a6f0(rotating_grayscale_particle_fade_reference_duration_gp) * 3 / 4 <
        particle_data[0]) {
        int scaled_fade_duration =
            FUN_001f96f8_0026a6f0(rotating_grayscale_particle_fade_reference_duration_gp);
        faded_alpha =
            particle_data[4] * (scaled_fade_duration - particle_data[0]) /
            (FUN_001f96f8_0026a6f0(rotating_grayscale_particle_fade_reference_duration_gp) / 4);
    } else if (particle_data[0] <
               FUN_001f96f8_0026a6f0(rotating_grayscale_particle_fade_reference_duration_gp) / 4) {
        faded_alpha =
            particle_data[4] * particle_data[0] /
            (FUN_001f96f8_0026a6f0(rotating_grayscale_particle_fade_reference_duration_gp) / 4);
    } else {
        faded_alpha = particle_data[4];
    }
    *(int *)(particle + 4) = (faded_alpha << 24) + (particle[3] == 0x48 ? 0x4F4F4F : 0x7F7F7F);
}

extern float D_0015ED60_0026a880 __asm__("D_0015ED60");
void         FUN_L00_00267a08_0026a880(unsigned char *) __asm__("FUN_L00_00267a08");
void         FUN_001f9a68_0026a880(void *, void *, float) __asm__("FUN_001f9a68");
void update_damped_blue_shift_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026a880");

void update_damped_blue_shift_effect_particle(unsigned char *particle) {
    float *particle_data = (float *)(particle + 0x20);
    int    blue_byte, alpha_byte;
    float  dt;
    particle_data[7] += D_0015ED60_0026a880 * -0.04f;
    if (particle_data[7] <= 0.0f || fast_dec_timer_s16(particle + 0xa)) {
        FUN_L00_00267a08_0026a880(particle);
        return;
    }
    dt = D_0015ED60_0026a880;
    *(float *)(particle + 0xc) += dt * 12000.0f;
    if (particle_data[6] < 0.225f)
        particle_data[6] += dt * 0.0484f;
    blue_byte = truncate_float_to_s32(particle_data[6] * 255.0f) & 0xFF;
    alpha_byte = truncate_float_to_s32(particle_data[7] * 255.0f);
    *(int *)(particle + 4) =
        *(unsigned short *)(particle + 4) | (blue_byte << 16) | (alpha_byte << 24);
    fast_vec_add(particle + 0x10, particle + 0x10, particle_data);
    FUN_001f9a68_0026a880(particle_data, particle_data, D_0015ED60_0026a880 * -0.100000024f + 1.0f);
    particle[8]--;
}

typedef union {
    OvlQuad q;
    float   f[4];
} V_26a9f0;

extern float   D_L00_0015F5D0;
unsigned char *FUN_L00_002678b8_26a9f0(int) __asm__("FUN_L00_002678b8");
int            FUN_001160d8_26a9f0(void) __asm__("FUN_001160d8");
void           FUN_001f9bf8_26a9f0(void *, void *, float) __asm__("FUN_001f9bf8");

extern unsigned char *D_L00_001B20AC NOT_SDA;
void                                 FUN_L00_0026a9f0(
                                    void *pos, void *dir, int col, int d, int n, int b1A, int b18, int b19, float x, float y);
void FUN_L00_0026a9f0(void *spawn_position,
                      void *base_velocity,
                      int   col,
                      int   color_endpoint,
                      int   lifetime,
                      int   b1A,
                      int   spawn_generation,
                      int   behavior_flags,
                      float x,

                      float y) {
    unsigned char *particle;
    unsigned char *q;
    V_26a9f0       velocity_offset;
    V_26a9f0       random_direction;

    if (0 == lifetime) {
        return;
    }
    if (0.85f < D_L00_0015F5D0 || 0.85f < D_L00_0015F5D4) {
        if (random_integer_below(3) == 0) {
            return;
        }
    }
    if (0.9f < D_L00_0015F5D0 || 0.9f < D_L00_0015F5D4) {
        if (random_integer_below(2) == 0) {
            return;
        }
    }
    if (1.0f < D_L00_0015F5D0 || 1.0f < D_L00_0015F5D4) {
        if (random_integer_below(1) == 0) {
            return;
        }
    }
    particle = FUN_L00_002678b8_26a9f0(0xB);
    if (particle == 0) {
        return;
    }
    qcopy(particle + 0x10, spawn_position);
    *(int *)(particle + 4) = col;
    particle[9] = truncate_float_to_s32(4.0f) - 0x60;
    particle[3] = (random_integer_below(100) < 0x14) ? 0x44 : 0x48;
    particle[1] = 0;
    q = particle + 0x20;
    *(int *)(particle + 0xC) = 0;
    particle[8] = FUN_001160d8_26a9f0();
    particle[2] = *D_L00_001B20AC;
    random_direction.q = 0;
    random_direction.f[0] = random_float_between(-1.0f, 1.0f);
    random_direction.f[1] = random_float_between(-1.0f, 1.0f);
    random_direction.f[2] = random_float_between(-1.0f, 1.0f);
    velocity_offset.q = random_direction.q;
    FUN_001f9bf8_26a9f0(&velocity_offset, &velocity_offset, y);
    fast_vec_add(q, base_velocity, &velocity_offset);
    *(float *)(q + 0xC) = y * 0.75f;
    *(short *)(particle + 0xA) = lifetime;
    q[0x1B] = lifetime;
    q[0x18] = b1A;
    *(float *)(q + 0x1C) = x;
    *(int *)(q + 0x10) = col;
    *(int *)(q + 0x14) = color_endpoint;
    q[0x1A] = spawn_generation;
    q[0x19] = behavior_flags;
}
/* Per-tick update of a particle type 11: moves, drags, fades and kills it. */
/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026BB18), where it is exact; names translated to the US level program. */

extern int  FUN_L00_001fefc8(void *);
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_0026a9f0_u(void *, void *, int, int, int, int, int, float, int, float) __asm__("FUN_L00_0026a9f0");

void update_branching_and_fading_effect_particle(char *particle) __asm__("FUN_L00_0026ac78");

void update_branching_and_fading_effect_particle(char *particle) {
    char *particle_data = particle + 0x20;
    particle[8]++;
    if ((particle_data[0x19] ^ 1) & 1) {
        int i;
        fast_vec_add(particle + 0x10, particle + 0x10, particle_data);
        if (particle_data[0x19] & 4) {
            *(float *)(particle_data + 8) += frame_time_sq * 9.8f;
        }
        {
            float s = 0.8f;
            if (particle_data[0x19] & 2)
                s = 0.6f;
            fast_vec_scale(particle_data, particle_data, s);
        }
        if (*(short *)(particle + 0xA) ==
                truncate_float_to_s32((float)(unsigned char)particle_data[0x1B] * 0.85f) &&
            (unsigned char)particle_data[0x1A] < 2 && !(particle_data[0x19] & 4)) {
            for (i = 4; i >= 0; i--) {
                int   a = truncate_float_to_s32((float)(unsigned char)particle_data[0x1B] *
                                              random_float_between(0.9f, 1.1f));
                int   k = (unsigned char)particle_data[0x18];
                float child_velocity_offset_scale = random_float_between(0.85f, 1.0f);
                FUN_L00_0026a9f0_u(particle + 0x10,
                                   particle_data,
                                   *(int *)(particle_data + 0x10),
                                   *(int *)(particle_data + 0x14),
                                   a,
                                   k,
                                   (unsigned char)particle_data[0x1A] + 1,
                                   *(float *)(particle_data + 0x1C),
                                   (unsigned char)particle_data[0x19],
                                   *(float *)(particle_data + 0xC) * child_velocity_offset_scale);
            }
        }
        if (!(particle_data[0x19] & 4)) {
            float x = *(float *)(particle_data + 0x1C);
            float A = convert_integer_to_float((unsigned char)particle_data[0x1B]);
            float B = (A * 8.0f - A * 3.0f * x) / (x * 8.0f * x + x * 8.0f);
            float C = (A * 3.0f + B * 8.0f * x) / (x * -8.0f);
            float F = convert_integer_to_float(*(short *)(particle + 0xA));
            float s = square_root(B * B - C * 4.0f * (A - F));
            float quadratic_root_plus = (-B + s) / (C + C);
            float quadratic_root_minus = (-B - s) / (C + C);
            float reference_distance;
            if (quadratic_root_minus < quadratic_root_plus)
                *(float *)(particle + 0xC) = quadratic_root_plus;
            else
                *(float *)(particle + 0xC) = quadratic_root_minus;
            reference_distance = fast_vec_dist(particle + 0x10, D_L00_00166DC0);
            if (reference_distance < 10.0f) {
                *(float *)(particle + 0xC) *= reference_distance * 0.075f + 0.25f;
            }
        }
        if (particle_data[0x19] & 2) {
            *(int *)(particle + 4) = fast_tween_color(
                *(int *)(particle_data + 0x10),
                0xFFFFFFF,
                (float)*(short *)(particle + 0xA) / (float)(unsigned char)particle_data[0x1B]);
        } else {
            *(int *)(particle + 4) = fast_tween_color(
                *(int *)(particle_data + 0x14),
                *(int *)(particle_data + 0x10),
                (float)*(short *)(particle + 0xA) / (float)(unsigned char)particle_data[0x1B]);
        }
        FUN_L00_001fefc8(particle + 0x38);
        if (fast_dec_timer_s16(particle + 0xA)) {
            if (particle_data[0x19] & 2) {
                int tripled_lifetime = (unsigned char)particle_data[0x1B] * 3;
                *(short *)(particle + 0xA) = tripled_lifetime;
                particle_data[0x1B] = tripled_lifetime;
                particle_data[0x19] = (particle_data[0x19] & 0xFD) | 4;
                fast_vec_normalize(particle_data,
                                   particle_data,
                                   *(float *)(particle_data + 0xC) *
                                       random_float_between(1.6f, 2.0f));
            } else {
                int fade_lifetime;
                *(int *)(particle_data + 0x10) = *(int *)(particle_data + 0x14);
                *(int *)(particle_data + 0x14) =
                    ((unsigned char)particle[3] == 0x48) ? 0 : 0xF2F3F3F;
                particle_data[0x19] |= 1;
                fade_lifetime = FUN_L00_00257b90((unsigned char)particle_data[0x1B] * 2,
                                                 (unsigned char)particle_data[0x1B] * 3);
                if (particle_data[0x19] & 8)
                    fade_lifetime *= 5;
                particle_data[0x1B] = fade_lifetime;
                *(short *)(particle + 0xA) = (unsigned char)fade_lifetime;
            }
        }
    } else {
        int fade_phase_duration =
            truncate_float_to_s32((float)(unsigned char)particle_data[0x1B] * 0.75f) + 1;
        int color_blend_duration =
            truncate_float_to_s32((float)(unsigned char)particle_data[0x1B] * 0.25f) + 1;
        int life;
        *(float *)(particle_data + 8) += frame_time_sq * 3.0f;
        fast_vec_add(particle + 0x10, particle + 0x10, particle_data);
        fast_vec_scale(particle_data, particle_data, 0.98f);
        life = *(short *)(particle + 0xA);
        if (fade_phase_duration < life) {
            *(int *)(particle + 4) =
                fast_tween_color(*(int *)(particle_data + 0x14),
                                 *(int *)(particle_data + 0x10),
                                 (float)(life - fade_phase_duration) / (float)color_blend_duration);
        } else if ((unsigned char)particle[3] == 0x48) {
            *(short *)(particle + 0xA) = 0;
        } else {
            *(int *)(particle + 4) =
                (*(int *)(particle + 4) & 0xFFFFFF) | ((life * 15 / fade_phase_duration) << 24);
        }
        if (D_L00_0015F5D0 > 1.0f)
            fast_dec_timer_s16(particle + 0xA);
        if (fast_dec_timer_s16(particle + 0xA))
            free_effect_particle(particle);
    }
}

extern unsigned char *create_part2(int, int) __asm__("FUN_L00_002678c0");
extern float          scale_time(float) __asm__("FUN_001f96b0");
extern int            FUN_001fa6e0_26b230(int, int, float) __asm__("FUN_001fa6e0");
extern unsigned       swap_packed_color_channels(unsigned, int) __asm__("FUN_L00_0025c0e8");
extern unsigned char *effect_texture_ids __asm__("D_L00_001B20B0") NOT_SDA;

extern unsigned char item_text_variant[] __asm__("D_0013E520");

extern int spinning_particle_default_initial_color_blend_source_gp __asm__("D_L00_001601A8")
    __attribute__((sda));
extern int spinning_particle_default_initial_color_blend_target_gp __asm__("D_L00_001601AC")
    __attribute__((sda));
extern int spinning_particle_default_final_color_blend_source_gp __asm__("D_L00_001601B0")
    __attribute__((sda));
extern int spinning_particle_default_final_color_blend_target_gp __asm__("D_L00_001601B4")
    __attribute__((sda));
extern float D_L00_001601B8_gp __asm__("D_L00_001601B8") __attribute__((sda));
extern int   spinning_particle_default_lifetime_lower_bound_gp __asm__("D_L00_001601C0")
    __attribute__((sda));
extern int spinning_particle_default_lifetime_upper_bound_gp __asm__("D_L00_001601C4")
    __attribute__((sda));
extern int spinning_particle_alternate_initial_color_blend_source_gp __asm__("D_L00_001601C8")
    __attribute__((sda));
extern int spinning_particle_alternate_initial_color_blend_target_gp __asm__("D_L00_001601CC")
    __attribute__((sda));
extern int spinning_particle_alternate_final_color_blend_source_gp __asm__("D_L00_001601D0")
    __attribute__((sda));
extern int spinning_particle_alternate_final_color_blend_target_gp __asm__("D_L00_001601D4")
    __attribute__((sda));
extern float D_L00_001601D8_gp __asm__("D_L00_001601D8") __attribute__((sda));
extern int   spinning_particle_alternate_lifetime_lower_bound_gp __asm__("D_L00_001601E0")
    __attribute__((sda));
extern int spinning_particle_alternate_lifetime_upper_bound_gp __asm__("D_L00_001601E4")
    __attribute__((sda));
extern unsigned char *D_L00_001B20E0 NOT_SDA;
unsigned char                       *spawn_spinning_and_fading_effect_particle(void         *spawn_position,
                                                                               void         *initial_velocity,
                                                                               unsigned char flags,
                                                                               float size) __asm__("FUN_L00_0026b230");

unsigned char *spawn_spinning_and_fading_effect_particle(void         *spawn_position,
                                                         void         *initial_velocity,
                                                         unsigned char flags,

                                                         float size) {
    void          *source = spawn_position;
    unsigned char *particle = create_part2(0xC, 0);
    if (particle) {
        unsigned char *motion = particle + 0x20;
        qcopy(particle + 0x10, source);
        qcopy(motion, initial_velocity);
        if (flags & 1) {
            float one = 1.0f;
            float zero = 0.0f;
            float scale = D_L00_001601D8_gp;
            float hi = (float)spinning_particle_alternate_lifetime_upper_bound_gp;
            float lo = (float)spinning_particle_alternate_lifetime_lower_bound_gp;
            *(int *)(particle + 4) = spinning_particle_alternate_initial_color_blend_source_gp;
            *(float *)(particle + 0xC) = scale * 210000.0f;
            *(short *)(particle + 0xA) =
                truncate_float_to_s32(scale_time(random_float_between(lo, hi)));
            *(int *)(motion + 0x14) =
                FUN_001fa6e0_26b230(spinning_particle_alternate_initial_color_blend_source_gp,
                                    spinning_particle_alternate_initial_color_blend_target_gp,
                                    random_float_between(zero, one));
            *(int *)(motion + 0x18) =
                FUN_001fa6e0_26b230(spinning_particle_alternate_final_color_blend_source_gp,
                                    spinning_particle_alternate_final_color_blend_target_gp,
                                    random_float_between(zero, one));
            {
                unsigned char texture = *D_L00_001B20E0;
                particle[3] = 0x48;
                particle[2] = texture;
            }
        } else {
            int   offset = 0;
            float one = 1.0f;
            float zero = 0.0f;
            float scale = D_L00_001601B8_gp;
            float hi = (float)spinning_particle_default_lifetime_upper_bound_gp;
            float lo = (float)spinning_particle_default_lifetime_lower_bound_gp;
            *(int *)(particle + 4) = spinning_particle_default_initial_color_blend_source_gp;
            *(float *)(particle + 0xC) = scale * 210000.0f;
            *(short *)(particle + 0xA) =
                truncate_float_to_s32(scale_time(random_float_between(lo, hi)));
            *(int *)(motion + 0x14) =
                FUN_001fa6e0_26b230(spinning_particle_default_initial_color_blend_source_gp,
                                    spinning_particle_default_initial_color_blend_target_gp,
                                    random_float_between(zero, one));
            *(int *)(motion + 0x18) =
                FUN_001fa6e0_26b230(spinning_particle_default_final_color_blend_source_gp,
                                    spinning_particle_default_final_color_blend_target_gp,
                                    random_float_between(zero, one));
            if (flags & 4) {
                offset = 8;
                if (!item_text_variant[0x10])
                    offset = 0;
            }
            particle[2] = effect_texture_ids[random_integer_below(8) + offset];
            particle[3] = 0x44;
        }
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[1] = 0;
        particle[8] = random_integer_below(0xFF);
        motion[0x1E] = truncate_float_to_s32(randf_sym(1.0f, 4.0f));
        *(float *)(motion + 0x10) = size;
        motion[0x1F] = flags;
        if (flags & 4) {
            *(short *)(particle + 0xA) =
                *(short *)(particle + 0xA) * (item_text_variant[0x10] + 2) / 2;
            swap_packed_color_channels(*(int *)(particle + 4), item_text_variant[0x10]);
            swap_packed_color_channels(*(unsigned *)(motion + 0x14), item_text_variant[0x10]);
            swap_packed_color_channels(*(unsigned *)(motion + 0x18), item_text_variant[0x10]);
        }
        motion[0x1D] = particle[0xA];
    }
    return particle;
}
/* per-frame update of a spinning particle: fades, scales and kills it when out of range */
/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026C3A0), where it is exact; names translated to the US level program. */

extern f32   advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float D_L00_001601B8 __attribute__((sda));
extern float spinning_particle_default_final_size_gp __asm__("D_L00_001601BC") __attribute__((sda));
extern float D_L00_001601D8 __attribute__((sda));
extern float spinning_particle_alternate_final_size_gp __asm__("D_L00_001601DC")
    __attribute__((sda));
extern float D_L00_001601E8 __attribute__((sda));
extern float spinning_particle_vertical_step_scale_gp __asm__("D_L00_001601EC")
    __attribute__((sda));
extern unsigned char D_0013E05A[];

void update_spinning_and_fading_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026b500");

void update_spinning_and_fading_effect_particle(unsigned char *particle) {
    unsigned char *particle_data = particle + 0x20;
    float          negated_particle_speed;
    float          size_endpoint_scale = 1.0f;
    float          ratio;
    particle[8] = particle[8] + particle_data[0x1E];
    if (particle_data[0x1F] & 4) {
        size_endpoint_scale =
            convert_integer_to_float(D_0013E05A[0x4D6]) * 0.5f + size_endpoint_scale;
    }
    fast_vec_add(particle + 0x10, particle + 0x10, particle_data);
    negated_particle_speed = -fast_vec_length(particle_data);
    advance_accelerated_scalar((float *)(particle + 0x30),
                               &negated_particle_speed,
                               0.0f,
                               0.0f,
                               D_L00_001601E8 * D_0015ED70,
                               negated_particle_speed);
    fast_vec_normalize(particle_data, particle_data, -negated_particle_speed);
    *(float *)(particle + 0x18) =
        *(float *)(particle + 0x18) + spinning_particle_vertical_step_scale_gp * D_0015ED6C;
    ratio = convert_integer_to_float(*(short *)(particle + 0xA)) /
            convert_integer_to_float(particle_data[0x1D]);
    if (particle_data[0x1F] & 1) {
        float lo = spinning_particle_alternate_final_size_gp * size_endpoint_scale;
        *(float *)(particle + 0xC) = ((D_L00_001601D8 - lo) * ratio + lo) * 210000.0f;
    } else {
        float lo = spinning_particle_default_final_size_gp * size_endpoint_scale;
        *(float *)(particle + 0xC) = ((D_L00_001601B8 - lo) * ratio + lo) * 210000.0f;
    }
    *(int *)(particle + 4) =
        fast_tween_color(*(int *)(particle_data + 0x18), *(int *)(particle_data + 0x14), ratio);
    if (*(short *)(particle + 0xA) < scale_game_frames(15)) {
        float remaining_lifetime_float = convert_integer_to_float(*(short *)(particle + 0xA));
        float fade_ratio =
            remaining_lifetime_float / convert_integer_to_float(scale_game_frames(10));
        *(int *)(particle + 4) =
            fast_tween_color(*(int *)(particle + 4) & 0xFFFFFF, *(int *)(particle + 4), fade_ratio);
    }
    if (*(float *)(particle + 0x10) < 2.0f || *(float *)(particle + 0x10) > 1021.0f ||
        *(float *)(particle + 0x14) < 2.0f || *(float *)(particle + 0x14) > 1021.0f ||
        *(float *)(particle + 0x18) < 2.0f || *(float *)(particle + 0x18) > 1021.0f ||
        fast_dec_timer_s16(particle + 0xA) != 0) {
        free_effect_particle(particle);
    } else if (*(float *)(particle_data + 0x10) < 0.01f) {
        *(short *)(particle + 0xA) = *(short *)(particle + 0xA) - 1;
    }
}

unsigned char                       *FUN_L00_002678b8_26b790(int) __asm__("FUN_L00_002678b8");
int                                  FUN_001f96f8_26b790(int) __asm__("FUN_001f96f8");
extern unsigned char *D_L00_001B20B4 NOT_SDA;
unsigned char                       *spawn_spinning_fade_in_out_effect_particle(void *spawn_origin,
                                                                                int   spd,
                                                                                int   color,
                                                                                float j,
                                                                                float lo,
                                                                                float hi,
                                                                                float vertical_position_step,
                                                                                float size) __asm__("FUN_L00_0026b790");

unsigned char *spawn_spinning_fade_in_out_effect_particle(void *spawn_origin,
                                                          int   spd,
                                                          int   color,
                                                          float j,
                                                          float lo,
                                                          float hi,
                                                          float vertical_position_step,

                                                          float size) {
    unsigned char *particle = FUN_L00_002678b8_26b790(0xD);
    if (particle) {
        unsigned char *particle_data;
        qcopy(particle + 0x10, spawn_origin);
        particle_data = particle + 0x20;
        *(int *)(particle + 4) = color;
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[3] = 0x48;
        *(float *)(particle + 0xC) = size;
        particle[1] = 0;
        particle[8] = 0;
        particle[2] = *D_L00_001B20B4;
        *(float *)(particle + 0x10) += randf_sym(0.0f, j);
        *(float *)(particle + 0x14) += randf_sym(0.0f, j);
        *(float *)(particle + 0x18) += randf_sym(0.0f, j);
        *(short *)(particle + 0xA) = FUN_001f96f8_26b790(10);
        *(int *)(particle_data + 4) = 0;
        if (random_integer_below(2)) {
            *(int *)(particle_data + 8) = spd;
        } else {
            *(int *)(particle_data + 8) = -spd;
        }
        *(int *)(particle_data + 0x14) = color & 0xFFFFFF;
        *(float *)(particle_data + 0xC) = random_float_between(lo, hi);
        *(float *)(particle_data + 0x10) = vertical_position_step;
    }
    return particle;
}

typedef struct {
    u8  p0[4];
    int w4;
    u8  b8;
    u8  p9[3];
    f32 fC;
    f32 f10;
    u32 w14;
} Body;

typedef struct {
    u8   p0[4];
    u32  w4;
    u8   b8;
    u8   p9;
    s16  hA;
    f32  fC;
    u8   p10[8];
    f32  f18;
    u8   p1C[4];
    Body body;
} Part_26b910;

int  FUN_001f96f8_0026b910(int) __asm__("FUN_001f96f8");
void FUN_L00_00267a08_0026b910(void *) __asm__("FUN_L00_00267a08");
void update_spinning_fade_in_out_effect_particle(Part_26b910 *particle) __asm__("FUN_L00_0026b910");

void update_spinning_fade_in_out_effect_particle(Part_26b910 *particle) {
    Body *particle_data = &particle->body;
    particle->b8 += particle_data->b8;
    particle->f18 += particle_data->f10;
    particle->fC *= particle_data->fC;
    if (particle_data->w4 == 0) {
        if (fast_dec_timer_s16(&particle->hA)) {
            particle_data->w4 = 1;
            particle->hA = FUN_001f96f8_0026b910(30);
            particle->w4 = particle_data->w14 | 0x7F000000;
        } else {
            int   fade_in_duration = FUN_001f96f8_0026b910(10);
            float k = 96.0f / (float)FUN_001f96f8_0026b910(10);
            particle->w4 =
                ((truncate_float_to_s32((float)(fade_in_duration - particle->hA) * k) + 0x20)
                 << 24) |
                particle_data->w14;
        }
    } else {
        if (fast_dec_timer_s16(&particle->hA)) {
            FUN_L00_00267a08_0026b910(particle);
            return;
        }
        particle->w4 = (truncate_float_to_s32((float)particle->hA *
                                              (127.0f / (float)FUN_001f96f8_0026b910(30)))
                        << 24) |
                       particle_data->w14;
    }
}

extern char D_L00_00166DC0_0026ba68[] __asm__("D_L00_00166DC0");
void        FUN_001f9bf8_0026ba68(void *, void *, float) __asm__("FUN_001f9bf8");
void        FUN_L00_00267a08_0026ba68(unsigned char *) __asm__("FUN_L00_00267a08");
int         FUN_001f96f8_0026ba68(int) __asm__("FUN_001f96f8");
void update_attached_or_fading_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026ba68");

void update_attached_or_fading_effect_particle(unsigned char *particle) {
    u128 position_offset;
    int *particle_data = (int *)(particle + 0x20);
    particle[8] += particle[0x20];
    if (particle_data[1] == 0) {
        u128 *particle_position = (u128 *)(particle + 0x10);
        qcopy(particle_position, (char *)particle_data[2] + 0x10);
        fast_vec_sub(&position_offset, D_L00_00166DC0_0026ba68, particle_position);
        FUN_001f9bf8_0026ba68(&position_offset, &position_offset, 0.01f);
        fast_vec_add(particle_position, particle_position, &position_offset);
    } else if (fast_dec_timer_s16(particle + 0xa))
        FUN_L00_00267a08_0026ba68(particle);
    else
        *(int *)(particle + 4) =
            (truncate_float_to_s32(*(short *)(particle + 0xa) * (127.0f / FUN_001f96f8_0026ba68(6)))
             << 24) |
            0xffffff;
}

extern unsigned char *FUN_L00_002678b8_26bb70(int) __asm__("FUN_L00_002678b8");
extern int            FUN_001160d8_26bb70(void) __asm__("FUN_001160d8");

extern unsigned char *D_L00_001B20BC NOT_SDA;
unsigned char *spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
    void *spawn_position,
    void *initial_velocity,
    int   initial_color,
    int   final_color,
    float initial_size,
    int   lifetime,
    int   secondary_spawn_enabled,
    int   texture_id,
    int   blend_mode) __asm__("FUN_L00_0026bb70");

unsigned char *
spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(void *spawn_position,
                                                                  void *initial_velocity,
                                                                  int   initial_color,
                                                                  int   final_color,
                                                                  float initial_size,
                                                                  int   lifetime,
                                                                  int   secondary_spawn_enabled,
                                                                  int   texture_id,

                                                                  int blend_mode) {
    unsigned char *particle;
    unsigned char *particle_data;
    if (lifetime == 0) {
        return 0;
    }
    particle = FUN_L00_002678b8_26bb70(0xF);
    if (particle) {
        qcopy(particle + 0x10, spawn_position);
        *(int *)(particle + 4) = initial_color;
        particle[9] = truncate_float_to_s32(4.0f) - 0x60;
        if (blend_mode != -1) {
            particle[3] = blend_mode;
        } else {
            particle[3] = 0x48;
        }
        particle[1] = 0;
        *(float *)(particle + 0xC) = initial_size;
        particle[8] = FUN_001160d8_26bb70();
        if (texture_id == -1) {
            particle[2] = *D_L00_001B20BC;
        } else {
            particle[2] = texture_id;
        }
        particle_data = particle + 0x20;
        *(s16 *)(particle + 0xA) = lifetime;
        *(s16 *)(particle_data + 0x18) = lifetime;
        *(float *)(particle_data + 0x1C) = initial_size;
        *(int *)(particle_data + 0x10) = initial_color;
        *(int *)(particle_data + 0x14) = final_color;
        *(s16 *)(particle_data + 0x1A) = secondary_spawn_enabled;
        qcopy(particle_data, initial_velocity);
    }
    return particle;
}

typedef union {
    u128  q;
    float f[4];
} V26bca8;

extern float   D_0015ED70_26bca8 __asm__("D_0015ED70");
extern int     D_L00_0015F5CC_26bca8 __asm__("D_L00_0015F5CC");
void           FUN_L00_00267a08_26bca8(void *) __asm__("FUN_L00_00267a08");
int            mine_blend_color(int, int, float) __asm__("FUN_L00_002371e0");
void           FUN_001f9a68_26bca8(void *, void *, float) __asm__("FUN_001f9a68");
float          FUN_001f9af0_26bca8(void *) __asm__("FUN_001f9af0");
void           FUN_001f9bf8_26bca8(void *, void *, float) __asm__("FUN_001f9bf8");
int            FUN_001f96f8_26bca8(int) __asm__("FUN_001f96f8");
unsigned char *spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
    void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
void update_shrinking_effect_particle_with_gravity_and_secondary_spawns(
    unsigned char *particle) __asm__("FUN_L00_0026bca8");

void update_shrinking_effect_particle_with_gravity_and_secondary_spawns(unsigned char *particle) {
    V26bca8        updated_position_copy, spawn_velocity, random_direction;
    unsigned char *particle_data = particle + 0x20;
    unsigned char *particle_position;
    float          lifetime_size_scratch, initial_lifetime_or_speed_scratch;
    if (fast_dec_timer_s16(particle + 0xA)) {
        FUN_L00_00267a08_26bca8(particle);
        return;
    }
    particle_position = particle + 0x10;
    lifetime_size_scratch = convert_integer_to_float(*(short *)(particle + 0xA));
    initial_lifetime_or_speed_scratch = convert_integer_to_float(*(short *)(particle_data + 0x18));
    lifetime_size_scratch *= *(float *)(particle_data + 0x1C);
    lifetime_size_scratch /= initial_lifetime_or_speed_scratch;
    *(float *)(particle + 0xC) = lifetime_size_scratch;
    lifetime_size_scratch = convert_integer_to_float(*(short *)(particle + 0xA));
    *(int *)(particle + 4) = mine_blend_color(
        *(int *)(particle_data + 0x14),
        *(int *)(particle_data + 0x10),
        lifetime_size_scratch / convert_integer_to_float(*(short *)(particle_data + 0x18)));
    fast_vec_add(particle_position, particle_position, particle_data);
    *(float *)(particle_data + 8) -= D_0015ED70_26bca8 * 14.6f;
    qcopy(&updated_position_copy, particle_position);
    if (*(short *)(particle_data + 0x1A) && (D_L00_0015F5CC_26bca8 & 1)) {
        random_direction.q = 0;
        random_direction.f[0] = random_float_between(-1.0f, 1.0f);
        random_direction.f[1] = random_float_between(-1.0f, 1.0f);
        random_direction.f[2] = random_float_between(-1.0f, 1.0f);
        spawn_velocity.q = random_direction.q;
        initial_lifetime_or_speed_scratch = FUN_001f9af0_26bca8(particle_data);
        FUN_001f9bf8_26bca8(&spawn_velocity,
                            &spawn_velocity,
                            initial_lifetime_or_speed_scratch * random_float_between(0.15f, 0.25f));
        fast_vec_add(&spawn_velocity, &spawn_velocity, particle_data);
        FUN_001f9a68_26bca8(&spawn_velocity, &spawn_velocity, random_float_between(0.75f, 0.95f));
        spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
            particle_position,
            &spawn_velocity,
            *(int *)(particle + 4),
            *(int *)(particle_data + 0x14),
            *(float *)(particle + 0xC),
            FUN_001f96f8_26bca8(random_int_between(0xF, 0x1E)),
            0,
            particle[2],
            particle[3]);
    } else {
        FUN_001f9a68_26bca8(particle_data, particle_data, 0.96f);
    }
}

extern unsigned char *FUN_L00_002678b8_26bed0(int) __asm__("FUN_L00_002678b8");
extern int            FUN_001160d8_26bed0(void) __asm__("FUN_001160d8");

extern unsigned char *D_L00_001B20C0 NOT_SDA;
unsigned char *
mine_debris(void *a, void *b, int c, int d, float f, int n, int k) __asm__("FUN_L00_0026bed0");
unsigned char *mine_debris(void *spawn_position,
                           void *initial_velocity,
                           int   initial_color,
                           int   final_color,
                           float initial_size,
                           int   lifetime,

                           int k) {
    unsigned char *particle;
    unsigned char *particle_data;
    if (lifetime == 0) {
        return 0;
    }
    if (D_L00_0015F5D0 > 0.9f && random_integer_below(3) == 0)
        return 0;
    if (D_L00_0015F5D0 > 0.95f && random_integer_below(2) == 0)
        return 0;
    if (D_L00_0015F5D0 > 1.0f && random_integer_below(1) == 0)
        return 0;
    particle = FUN_L00_002678b8_26bed0(0x10);
    if (particle) {
        qcopy(particle + 0x10, spawn_position);
        *(int *)(particle + 4) = initial_color;
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[1] = 0;
        *(float *)(particle + 0xC) = initial_size;
        particle[3] = 0x44;
        particle[8] = FUN_001160d8_26bed0();
        {
            unsigned char *particle_byte_table = D_L00_001B20C0;
            particle[2] = *(k != 3 ? particle_byte_table + k : particle_byte_table);
        }
        particle_data = particle + 0x20;
        *(s16 *)(particle + 0xA) = lifetime;
        *(s16 *)(particle_data + 0x18) = lifetime;
        *(float *)(particle_data + 0x1C) = initial_size;
        *(int *)(particle_data + 0x10) = initial_color;
        *(int *)(particle_data + 0x14) = final_color;
        *(s16 *)(particle_data + 0x1A) = k;
        qcopy(particle_data, initial_velocity);
        *(float *)(particle_data + 0xC) = ((float *)spawn_position)[2] - 0.5f;
    }
    return particle;
}
/* Ported from rac1-decomp src/overlays/shared/partupd_0026A130.c (func_L00_0026CF28) */

/* Updates a type 16 particle: fades it by age and distance, applies its gravity, and on landing spawns the next kind. */

typedef struct {
    unsigned char b0, b1, b2, b3;
    int           col;
    unsigned char b8, b9;
    short         timer;
    float         fC;
    float         pos[4];
} Part;

typedef struct {
    float vel[4];
    int   c10;
    int   c14;
    short life;
    short kind;
    float f1C;
} Data;

extern int mine_blend_color(int, int, float) __asm__("FUN_L00_002371e0");

void update_fading_debris_particle_with_landing_spawn(Part *particle) __asm__("FUN_L00_0026c088");

void update_fading_debris_particle_with_landing_spawn(Part *particle) {
    float saved[4];
    float grav[4] = {
        D_0015ED70 * 4.8f, D_0015ED70 * 24.5f, D_0015ED70 * 9.8f, -(D_0015ED70 * 0.24f)};
    Data *particle_data = (Data *)(particle + 1);
    int   alpha, spawn_lifetime, k;
    float reference_distance_or_spawn_size;

    if (particle_data->kind == 3 && D_L00_0015F5D0 > 0.95f)
        fast_dec_timer_s16(&particle->timer);
    if (fast_dec_timer_s16(&particle->timer)) {
        free_effect_particle(particle);
        return;
    }
    {
        float remaining_lifetime_float = convert_integer_to_float(particle->timer);
        particle->col = mine_blend_color(particle_data->c14,
                                         particle_data->c10,
                                         remaining_lifetime_float /
                                             convert_integer_to_float(particle_data->life));
        alpha = particle->col >> 24;
        reference_distance_or_spawn_size = fast_vec_dist(particle->pos, D_L00_00166DC0);
        if (particle->timer < scale_game_frames(6))
            alpha = alpha * particle->timer / scale_game_frames(6);
        if (reference_distance_or_spawn_size < 4.0f) {
            free_effect_particle(particle);
            return;
        }
        if (reference_distance_or_spawn_size < 8.0f)
            alpha = truncate_float_to_s32(convert_integer_to_float(alpha) *
                                          (reference_distance_or_spawn_size - 4.0f) * 0.25f);
        particle->col = (alpha << 24) | (particle->col & 0xFFFFFF);
        fast_vec_add(particle->pos, particle->pos, particle_data);
        k = particle_data->kind;
        particle_data->vel[2] -= grav[k];
        qcopy(saved, particle->pos);
        if (particle->pos[2] < particle_data->vel[3]) {
            if (k == 1) {
                int spawn_initial_color, spawn_final_color, minimum_spawn_lifetime;
                spawn_initial_color =
                    mine_blend_color(0x7F000000, 0x7F182030, random_float_between(0.25f, 1.0f));
                spawn_final_color = mine_blend_color(0, 0x5F5F5F, random_float_between(0.5f, 1.0f));
                particle_data->vel[2] = 0;
                reference_distance_or_spawn_size = random_float_between(200000.0f, 300000.0f);
                minimum_spawn_lifetime = scale_game_frames(0x5A);
                spawn_lifetime = FUN_L00_00257b90(minimum_spawn_lifetime, scale_game_frames(0x96));
                mine_debris(particle->pos,
                            particle_data,
                            spawn_initial_color,
                            spawn_final_color,
                            reference_distance_or_spawn_size,
                            spawn_lifetime,
                            3);
            }
            free_effect_particle(particle);
        } else
            particle->b8++;
    }
}

#include "qzero.h"

extern unsigned char *FUN_L00_002678b8_26c3d0(int) __asm__("FUN_L00_002678b8");
extern int            FUN_001160d8_26c3d0(void) __asm__("FUN_001160d8");
extern int            FUN_001f96f8_26c3d0(int) __asm__("FUN_001f96f8");

extern unsigned char *D_L00_001B20C8 NOT_SDA;
void                                 spawn_effect_particle_with_transformed_random_offset(
                                    void *spawn_origin, u128 *spawn_offset_matrix) __asm__("FUN_L00_0026c3d0");

void spawn_effect_particle_with_transformed_random_offset(void *spawn_origin,

                                                          u128 *spawn_offset_matrix) {
    unsigned char *particle = FUN_L00_002678b8_26c3d0(0x12);
    if (particle) {
        float *particle_data = (float *)(particle + 0x20);
        u128   out[1];
        float  random_spawn_offset[4] __attribute__((aligned(16)));
        u128   mt[4];
        qzero(&mt[3]);
        mt[0] = spawn_offset_matrix[0];
        mt[1] = spawn_offset_matrix[1];
        mt[2] = spawn_offset_matrix[2];
        random_spawn_offset[0] = random_float_between(-0.1f, 0.1f);
        random_spawn_offset[1] = random_float_between(-0.3f, 0.3f);
        random_spawn_offset[2] = random_float_between(-0.3f, 0.3f);
        transform_vector(out, random_spawn_offset, mt);
        fast_vec_add(particle + 0x10, out, spawn_origin);
        *(int *)(particle + 4) = 0x806040;
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[1] = 0;
        particle[3] = 0x48;
        particle[8] = FUN_001160d8_26c3d0();
        particle[2] = *D_L00_001B20C8;
        particle_data[1] = 0.08f;
        *(float *)(particle + 0xC) = random_float_between(27993.002f, 49980.0f);
        *(s16 *)(particle + 0xA) = FUN_001f96f8_26c3d0(0xD2);
        particle_data[3] = 1.0f;
    }
}

typedef float f32_26c558;

typedef struct {
    f32_26c558 f0, f4, f8, fC;
} Q_26c558;

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned int  w4;
    unsigned char b8, b9;
    short         hA;
    f32_26c558    fC;
    f32_26c558    f10, f14, f18, f1C;
    Q_26c558      q;
} P_26c558;

extern f32_26c558 D_0015ED60;
extern void       kill_26c558(void *) __asm__("FUN_L00_00267a08");
void update_descending_fade_in_out_effect_particle(P_26c558 *particle) __asm__("FUN_L00_0026c558");

void update_descending_fade_in_out_effect_particle(P_26c558 *particle) {
    Q_26c558 *q = &particle->q;
    q->f4 += D_0015ED60 * 0.013f * q->fC;
    if (q->f4 <= 0.0f || fast_dec_timer_s16(&particle->hA)) {
        kill_26c558(particle);
        return;
    }
    if (0.22f <= q->f4) {
        q->fC = -q->fC;
        q->f4 = q->f4 + D_0015ED60 * 0.013f * q->fC;
    }
    particle->fC += D_0015ED60 * 8190.0f;
    particle->w4 = (particle->w4 & 0xFFFFFF) | (truncate_float_to_s32(q->f4 * 255.0f) << 24);
    q->f8 += D_0015ED60 * 0.012f;
    if (1.0f < q->f8) {
        q->f8 -= 1.0f;
    }
    particle->b8 = (int)(q->f8 * 255.0f);
    particle->f18 -= 0.02f;
}

typedef struct {
    u8  x0;
    u8  x1;
    u8  x2;
    u8  x3;
    u32 x4;
    u8  x8;
    u8  x9;
    u8  pa[2];
    u32 xc;
    f32 a[4];
    f32 b[4];
    f32 c[4];
} E0026c6e8;

extern u8 *D_L00_001B20CC NOT_SDA;
E0026c6e8                *FUN_L00_002678b8_u(s32) __asm__("FUN_L00_002678b8");
void                      spawn_two_point_effect_particle(void *spawn_position,
                                                          void *initial_velocity) __asm__("FUN_L00_0026c6e8");

void spawn_two_point_effect_particle(void *pos, void *vel) __asm__("FUN_L00_0026c6e8");

void spawn_two_point_effect_particle(void *spawn_position, void *initial_velocity) {
    E0026c6e8 *particle = FUN_L00_002678b8_u(0x13);
    u32        initial_packed_color;
    if (particle == 0)
        return;
    initial_packed_color = (random_integer_below(6) * 6 + 0x40) | 0x78404000;
    particle->x4 = initial_packed_color;
    particle->xc = initial_packed_color;
    particle->x9 = truncate_float_to_s32(2.0f) + 0x20;
    particle->x3 = 0x48;
    particle->x1 = 3;
    particle->x2 = *D_L00_001B20CC;
    qcopy(particle->a, spawn_position);
    qcopy(particle->c, initial_velocity);
    fast_vec_sub(particle->b, spawn_position, particle->c);
    fast_vec_sub(particle->b, particle->b, particle->c);
    particle->a[3] = 0.09f;
    particle->b[3] = 0.5f;
}

/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026D690), where it is exact; names translated to the US level program. */

/* Moves both points by the vector at 0x30; lowers the top byte at a+4 by 6, 00267a08 below 0. */
void move_and_fade_two_point_particle(char *particle) __asm__("FUN_L00_0026c7f0");

void move_and_fade_two_point_particle(char *particle) {
    int faded_color;
    fast_vec_add(particle + 0x10, particle + 0x10, particle + 0x30);
    fast_vec_add(particle + 0x20, particle + 0x20, particle + 0x30);
    faded_color = *(int *)(particle + 4) + 0xFA000000;
    *(int *)(particle + 4) = faded_color;
    *(int *)(particle + 0xC) = faded_color;
    if (faded_color < 0)
        free_effect_particle(particle);
}

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V_26c860;

typedef struct {
    OvlQuad q[4];
} M3;

typedef struct {
    M3 *p0;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
    f32 f14;
    u8 *p18;
    f32 f1C;
} R_26c860;

typedef struct {
    u8       p0[4];
    s32      i4;
    u8       b8;
    u8       p9;
    s16      hA;
    f32      fC;
    V_26c860 v10;
    R_26c860 r;
} M_26c860;

void FUN_L00_001ff500_26c860(void *, void *, f32) __asm__("FUN_L00_001ff500");
f32  compute_transformed_delta_component(void *, void *, void *, s32) __asm__("FUN_L00_0025c230");
void FUN_L00_00267a08_26c860(void *) __asm__("FUN_L00_00267a08");
void update_spiraling_fade_in_out_effect_particle(M_26c860 *particle) __asm__("FUN_L00_0026c860");

void update_spiraling_fade_in_out_effect_particle(M_26c860 *particle) {
    R_26c860 *r = &particle->r;
    V_26c860  reference_position, position_offset_scratch, transformed_position_offset;
    M3        position_offset_matrix;
    if (r->f10 != 0.0f) {
        f32 zero = 0.0f;
        position_offset_matrix.q[0] = r->p0->q[0];
        position_offset_matrix.q[1] = r->p0->q[1];
        position_offset_matrix.q[2] = r->p0->q[2];
        position_offset_scratch.y = fast_sin(r->f14) * r->f10;
        position_offset_scratch.z = fast_cos(r->f14) * r->f10;
        position_offset_scratch.x = zero;
        position_offset_scratch.w = zero;
        transform_vector(
            &transformed_position_offset, &position_offset_scratch, &position_offset_matrix);
        fast_vec_sub(&particle->v10, &particle->v10, &transformed_position_offset);
        r->f1C += frame_scale * 0.06001f;
        moby_point_position(r->p18, 0, &reference_position);
        fast_vec_sub(&position_offset_scratch, &reference_position, &particle->v10);
        FUN_L00_001ff500_26c860(&position_offset_scratch, &position_offset_scratch, r->f1C);
        fast_vec_add(&particle->v10, &particle->v10, &position_offset_scratch);
        r->f14 = fast_add_rotations(r->f14, frame_scale * 0.5235988f);
        r->f10 *= frame_scale * -0.10000002384185791f + 1.0f;
        position_offset_scratch.y = fast_sin(r->f14) * r->f10;
        position_offset_scratch.z = fast_cos(r->f14) * r->f10;
        position_offset_scratch.x = zero;
        position_offset_scratch.w = zero;
        transform_vector(
            &transformed_position_offset, &position_offset_scratch, &position_offset_matrix);
        fast_vec_add(&particle->v10, &particle->v10, &transformed_position_offset);
        if (zero < compute_transformed_delta_component(
                       &reference_position, r->p18 + 0xC0, &particle->v10, 1)) {
            FUN_L00_00267a08_26c860(particle);
            return;
        }
    }
    r->f4 += frame_scale * 0.011f * r->fC;
    if (r->f4 <= 0.0f || fast_dec_timer_s16(&particle->hA)) {
        FUN_L00_00267a08_26c860(particle);
        return;
    }
    if (1.0f <= r->f4) {
        r->fC = -r->fC;
        r->f4 += frame_scale * 0.011f * r->fC;
    }
    particle->fC += frame_scale * -2940.0f;
    particle->i4 = (particle->i4 & 0xFFFFFF) | (truncate_float_to_s32(r->f4 * 255.0f) << 24);
    r->f8 += frame_scale * 0.006f;
    if (1.0f < r->f8)
        r->f8 -= 1.0f;
    particle->b8 = r->f8 * 255.0f;
}
/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026DA50), where it is exact; names translated to the US level program. */

extern s32   rand();
extern void *FUN_L00_002678b8_c(int) __asm__("FUN_L00_002678b8");

extern unsigned char *D_L00_001B20D4 NOT_SDA;
char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
char *FUN_L00_0026cbb0(void *spawn_position,
                       void *initial_velocity,
                       int   initial_color,
                       int   final_color,
                       int   lifetime,
                       int   k,

                       float initial_particle_scalar) {
    char *particle;
    char *particle_data;
    int   initial_header_byte;
    if (lifetime == 0)
        return 0;
    particle = FUN_L00_002678b8_c(0x15);
    if (particle) {
        qcopy(particle + 0x10, spawn_position);
        *(int *)(particle + 4) = initial_color;
        initial_header_byte = truncate_float_to_s32(4.0f) - 0x60;
        particle[1] = 0;
        *(float *)(particle + 0xC) = initial_particle_scalar;
        particle[3] = 0x48;
        particle[9] = initial_header_byte;
        particle[8] = rand();
        particle_data = particle + 0x20;
        particle[2] = *D_L00_001B20D4;
        *(short *)(particle + 0xA) = lifetime;
        *(short *)(particle_data + 0x18) = lifetime;
        *(float *)(particle_data + 0x1C) = initial_particle_scalar;
        *(int *)(particle_data + 0x10) = initial_color;
        *(int *)(particle_data + 0x14) = final_color;
        *(short *)(particle_data + 0x1A) = k;
        qcopy(particle_data, initial_velocity);
    }
    return particle;
}

typedef struct {
    f32 v[4];
    s32 x10;
    s32 x14;
    s16 x18;
    s16 x1a;
    f32 x1c;
} S_0026ccb0_0026ccb0;

typedef struct {
    u8                  p0[4];
    s32                 x4;
    u8                  x8;
    u8                  p9;
    s16                 xa;
    f32                 xc;
    u128                p10;
    S_0026ccb0_0026ccb0 s;
} O_0026ccb0_0026ccb0;

typedef struct {
    f32 x, y, z, w_0026ccb0;
} V_0026ccb0_0026ccb0;

extern s32 D_L00_0015F5CC_0026ccb0 __asm__("D_L00_0015F5CC");
void       FUN_L00_00267a08_0026ccb0(void *) __asm__("FUN_L00_00267a08");
float      FUN_001f9af0_0026ccb0(void *) __asm__("FUN_001f9af0");
void       FUN_001f9bf8_0026ccb0(void *, void *, float) __asm__("FUN_001f9bf8");
int        FUN_001f96f8_0026ccb0(int) __asm__("FUN_001f96f8");
void       spawn_burst(void *, void *, s32, s32, f32, int, int) __asm__("FUN_L00_0026cbb0");
void update_shrinking_effect_particle_with_secondary_spawns(O_0026ccb0_0026ccb0 *particle) __asm__("FUN_L00_0026ccb0");

void update_shrinking_effect_particle_with_secondary_spawns(O_0026ccb0_0026ccb0 *particle) {
    S_0026ccb0_0026ccb0 *s = &particle->s;
    u128                *particle_position;
    V_0026ccb0_0026ccb0  updated_position_copy, spawn_velocity_storage, random_direction;
    V_0026ccb0_0026ccb0 *spawn_velocity;
    f32                  particle_speed;
    if (fast_dec_timer_s16(&particle->xa)) {
        FUN_L00_00267a08_0026ccb0(particle);
    } else {
        particle_position = &particle->p10;
        particle->xc =
            convert_integer_to_float(particle->xa) * s->x1c / convert_integer_to_float(s->x18);
        particle->x4 = mine_blend_color(s->x14,
                                        s->x10,
                                        convert_integer_to_float(particle->xa) /
                                            convert_integer_to_float(s->x18));
        fast_vec_add(particle_position, particle_position, s);
        s->v[2] -= frame_scale_sq * 0.001f;
        qcopy(&updated_position_copy, particle_position);
        if (s->x1a != 0 && (D_L00_0015F5CC_0026ccb0 & 1)) {
            *(u128 *)&random_direction = 0;
            random_direction.x = random_float_between(-1.0f, 1.0f);
            random_direction.y = random_float_between(-1.0f, 1.0f);
            random_direction.z = random_float_between(-1.0f, 1.0f);
            *(u128 *)&spawn_velocity_storage = *(u128 *)&random_direction;
            particle_speed = FUN_001f9af0_0026ccb0(s);
            spawn_velocity = &spawn_velocity_storage;
            FUN_001f9bf8_0026ccb0(spawn_velocity,
                                  spawn_velocity,
                                  particle_speed * random_float_between(0.21f, 0.25f));
            fast_vec_add(spawn_velocity, spawn_velocity, s);
            fast_vec_scale(spawn_velocity, spawn_velocity, random_float_between(0.75f, 0.95f));
            spawn_burst(particle_position,
                        spawn_velocity,
                        particle->x4,
                        s->x14,
                        particle->xc,
                        FUN_001f96f8_0026ccb0(random_int_between(15, 20)),
                        0);
        } else {
            fast_vec_scale(s, s, 0.96f);
        }
    }
}

extern float   D_0015ED70_0026ced0 __asm__("D_0015ED70");
unsigned char *FUN_L00_002678b8_0026ced0(int) __asm__("FUN_L00_002678b8");
int            FUN_001160d8_0026ced0(void) __asm__("FUN_001160d8");

extern unsigned char *D_L00_001B20D8 NOT_SDA;
void                                 spawn_effect_particle_with_target_scalar(
                                    u128 *spawn_position,
                                    u128 *initial_velocity,
                                    int   initial_color,
                                    int   secondary_color,
                                    int   lifetime,
                                    float target_particle_scalar) __asm__("FUN_L00_0026ced0");

void spawn_effect_particle_with_target_scalar(u128 *spawn_position,
                                              u128 *initial_velocity,
                                              int   initial_color,
                                              int   secondary_color,
                                              int   lifetime,

                                              float target_particle_scalar) {
    unsigned char *particle = FUN_L00_002678b8_0026ced0(0x16);
    if (particle) {
        unsigned char *particle_data = particle + 0x20;
        float          lifetime_float;
        *(int *)(particle + 4) = initial_color;
        particle[9] = truncate_float_to_s32(4.0f) + 0x70;
        particle[1] = 0;
        particle[3] = 0x44;
        *(float *)(particle + 0xc) = target_particle_scalar * 0.2f;
        particle[8] = FUN_001160d8_0026ced0();
        particle[2] = *D_L00_001B20D8;
        *(short *)(particle + 0xA) = lifetime;
        *(short *)(particle_data + 0x18) = lifetime;
        lifetime_float = convert_integer_to_float(lifetime);
        *(float *)(particle_data + 0x1c) =
            (target_particle_scalar - *(float *)(particle + 0xc)) / lifetime_float;
        *(int *)(particle_data + 0x10) = initial_color;
        *(int *)(particle_data + 0x14) = secondary_color;
        qcopy(particle_data, initial_velocity);
        qcopy(particle + 0x10, spawn_position);
        *(float *)(particle_data + 0xc) = D_0015ED70_0026ced0 * 1.24f;
    }
}

extern float   D_0015ED60_26d000 __asm__("D_0015ED60");
unsigned char *FUN_L00_002678b8_26d000(int) __asm__("FUN_L00_002678b8");
int            FUN_001f96f8_26d000(int) __asm__("FUN_001f96f8");
unsigned char *FUN_L00_0026d000(void *spawn_position,
                                int   spin_rate,
                                void *initial_velocity,
                                int   color,
                                float position_jitter_scale,
                                float lo,
                                float hi,

                                float size) {
    unsigned char *particle = FUN_L00_002678b8_26d000(0x17);
    if (particle) {
        unsigned char *particle_data;
        float          z;
        qcopy(particle + 0x10, spawn_position);
        *(int *)(particle + 4) = color | 0x20000000;
        z = 0.0f;
        particle_data = particle + 0x20;
        particle[9] = truncate_float_to_s32(4.0f) + 0x70;
        particle[3] = 0x48;
        *(float *)(particle + 0xC) = size;
        particle[1] = 0;
        particle[8] = 0;
        particle[2] = *D_L00_001B20DC;
        *(float *)(particle + 0x10) += randf_sym(z, position_jitter_scale);
        *(float *)(particle + 0x14) += randf_sym(z, position_jitter_scale);
        *(float *)(particle + 0x18) += randf_sym(z, position_jitter_scale);
        *(short *)(particle + 0xA) = FUN_001f96f8_26d000(0xA);
        *(int *)(particle_data + 4) = 0;
        *(int *)(particle_data + 0) = color;
        if (random_integer_below(2))
            *(short *)(particle_data + 8) = truncate_float_to_s32(D_0015ED60_26d000 * spin_rate);
        else
            *(short *)(particle_data + 8) = truncate_float_to_s32(-spin_rate * D_0015ED60_26d000);
        *(float *)(particle_data + 0xC) = random_float_between(lo, hi);
        qcopy(particle_data + 0x10, initial_velocity);
    }
    return particle;
}

extern float D_0015ED60_26d1b0 __asm__("D_0015ED60");
extern int   FUN_001f96f8_26d1b0(int) __asm__("FUN_001f96f8");
extern void  FUN_L00_00267a08_26d1b0(void *) __asm__("FUN_L00_00267a08");

void update_moving_spinning_fade_in_out_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026d1b0");

void update_moving_spinning_fade_in_out_effect_particle(unsigned char *particle) {
    unsigned char *particle_data = particle + 0x20;
    float          alpha_fade_scratch;
    int            phase_duration;
    int            x;

    particle[8] += particle_data[8];
    fast_vec_add(particle + 0x10, particle + 0x10, particle + 0x30);
    *(float *)(particle + 0xC) *=
        (*(float *)(particle_data + 0xC) - 1.0f) * D_0015ED60_26d1b0 + 1.0f;
    if (*(int *)(particle_data + 4) == 0) {
        if (fast_dec_timer_s16(particle + 0xA)) {
            *(int *)(particle_data + 4) = 1;
            *(short *)(particle + 0xA) = FUN_001f96f8_26d1b0(0x14);
            *(int *)(particle + 4) = *(int *)particle_data | 0x7F000000;
        } else {
            phase_duration = FUN_001f96f8_26d1b0(10);
            x = truncate_float_to_s32((float)(phase_duration - *(short *)(particle + 0xA)) *
                                      (96.0f / (float)FUN_001f96f8_26d1b0(10)));
            *(int *)(particle + 4) = ((x + 0x20) << 24) | *(int *)particle_data;
        }
    } else if (*(int *)(particle_data + 4) == 2) {
        if (fast_dec_timer_s16(particle + 0xA))
            goto kill;
        alpha_fade_scratch = convert_integer_to_float(particle_data[0xB]);
        x = truncate_float_to_s32((float)particle_data[0xA] *
                                      ((float)*(short *)(particle + 0xA) / alpha_fade_scratch) +
                                  0.0f);
        *(int *)(particle + 4) = (x << 24) | (*(int *)particle_data & 0xFFFFFF);
    } else if (*(int *)(particle_data + 4) == 3) {
        fast_dec_timer_s16(particle + 0xA);
        alpha_fade_scratch =
            convert_integer_to_float(particle_data[0xB] - *(short *)(particle + 0xA));
        alpha_fade_scratch =
            alpha_fade_scratch / convert_integer_to_float(FUN_001f96f8_26d1b0(0x1E));
        if (1.0f < alpha_fade_scratch)
            alpha_fade_scratch = 1.0f;
        x = truncate_float_to_s32((float)particle_data[0xA] * alpha_fade_scratch + 0.0f);
        *(int *)(particle + 4) = (x << 24) | (*(int *)particle_data & 0xFFFFFF);
        if (particle_data[0xB] - *(short *)(particle + 0xA) >= FUN_001f96f8_26d1b0(0x1E)) {
            phase_duration = FUN_001f96f8_26d1b0(0x1E);
            *(int *)(particle_data + 4) = 2;
            particle_data[0xB] -= phase_duration;
        }
    } else {
        if (fast_dec_timer_s16(particle + 0xA)) {
        kill:
            FUN_L00_00267a08_26d1b0(particle);
            return;
        }
        x = truncate_float_to_s32((float)*(short *)(particle + 0xA) *
                                  (127.0f / (float)FUN_001f96f8_26d1b0(0x14)));
        *(int *)(particle + 4) = (x << 24) | *(int *)particle_data;
    }
}

typedef struct {
    u8   b0;
    u8   b1;
    u8   b2;
    u8   b3;
    u8   b4;
    u8   b5;
    u8   b6;
    u8   b7;
    u8   b8;
    u8   b9;
    s16  hA;
    f32  fC;
    u128 pos;
} Src;

typedef struct {
    Src *e;
    f32  f4;
    f32  f8;
    f32  fC;
    f32  f10;
    f32  f14;
} Body_26d470;

typedef struct {
    u8          b0;
    u8          b1;
    u8          b2;
    u8          b3;
    u32         w4;
    u8          b8;
    u8          b9;
    s16         hA;
    f32         fC;
    u128        pos;
    Body_26d470 body;
} Part_26d470;

void FUN_L00_00267a08_0026d470(void *) __asm__("FUN_L00_00267a08");
void update_source_bound_effect_particle(Part_26d470 *particle) __asm__("FUN_L00_0026d470");

void update_source_bound_effect_particle(Part_26d470 *particle) {
    Src         *e;
    Body_26d470 *particle_data;
    float        updated_alpha_offset;
    u32          packed_rgb;
    if (!particle)
        return;
    e = particle->body.e;
    particle_data = &particle->body;
    if (e == 0 || (e->b1 & 0x80)) {
        FUN_L00_00267a08_0026d470(particle);
        return;
    }
    qcopy(&particle->pos, &e->pos);
    particle->b8 = e->b8;
    particle->fC = e->fC * particle_data->f4 + particle_data->f8;
    updated_alpha_offset = particle_data->fC + particle_data->f14;
    particle_data->fC = updated_alpha_offset;
    if (updated_alpha_offset < 0.0f) {
        particle_data->fC = 0.0f;
        return;
    }
    if (1.0f < updated_alpha_offset)
        particle_data->fC = 1.0f;
    packed_rgb = particle->w4 & 0xFFFFFF;
    particle->w4 =
        packed_rgb | (truncate_float_to_s32((convert_integer_to_float(particle->body.e->b7) *
                                                 (particle_data->f10 * 0.00390625f) +
                                             particle_data->fC) *
                                            256.0f)
                      << 24);
}

extern unsigned char *FUN_L00_002678b8_26d598(int) __asm__("FUN_L00_002678b8");
extern int FUN_L00_0025bfe0_26d598(float, float, float, float) __asm__("FUN_L00_0025bfe0");
extern int FUN_001f96f8_26d598(int) __asm__("FUN_001f96f8");

extern unsigned char *D_L00_001B20E4 NOT_SDA;
void                                 spawn_color_decaying_effect_particle(void *spawn_position,
                                                                          void *initial_velocity,
                                                                          int   flag) __asm__("FUN_L00_0026d598");

void spawn_color_decaying_effect_particle(void *spawn_position, void *initial_velocity, int flag) {
    unsigned char *particle = FUN_L00_002678b8_26d598(0x19);
    if (particle) {
        float *particle_data = (float *)(particle + 0x20);
        qcopy(particle + 0x10, spawn_position);
        if (flag == 0) {
            float one = 1.0f;
            *(int *)(particle + 4) = FUN_L00_0025bfe0_26d598(one, one, one, 0.6f);
            particle_data[4] = one;
            particle_data[5] = one;
            particle_data[6] = one;
        } else {
            *(int *)(particle + 4) = FUN_L00_0025bfe0_26d598(1.0f, 1.0f, 1.0f, 0.6f);
            particle_data[4] = -1.0f;
            particle_data[5] = -1.0f;
            particle_data[6] = -1.0f;
        }
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[1] = 0;
        particle[3] = 0x48;
        *(float *)(particle + 0xC) = random_float_between(5000.0f, 30000.0f);
        particle[8] = 0;
        particle[2] = *D_L00_001B20E4;
        particle_data[7] = 0.6f;
        *(s16 *)(particle + 0xA) = FUN_001f96f8_26d598(0x50);
        qcopy(particle_data, initial_velocity);
    }
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_26d700;

typedef struct {
    float x0, x4, x8, xc, r, g, b, a;
} S_26d700;

typedef struct {
    char          pad0[4];
    int           x4;
    unsigned char x8;
    char          pad9;
    short         xa;
    float         xc;
    OvlQuad       pos;
    S_26d700      s;
} P_26d700;

extern float   D_0015ED60_26d700 __asm__("D_0015ED60");
extern int     D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern OvlQuad D_L00_00173E60_lui __asm__("D_L00_00173E60") NOT_SDA;
int            coll_26d700(void *, void *, int, int, int) __asm__("FUN_001efa68");
void           kill_26d700(P_26d700 *) __asm__("FUN_L00_00267a08");
int            rgba_26d700(float, float, float, float) __asm__("FUN_L00_0025bfe0");
void           update_color_decaying_effect_particle_with_collision(P_26d700 *particle) __asm__("FUN_L00_0026d700");

void update_color_decaying_effect_particle_with_collision(P_26d700 *particle) {
    V_26d700  next_position;
    S_26d700 *s;
    float     r, g, b, dt;
    int       neg;
    if (particle == 0)
        return;
    s = &particle->s;
    r = s->r;
    neg = 0;
    g = s->g;
    b = s->b;
    if (r < 0.0f || g < 0.0f || b < 0.0f) {
        r = -r;
        neg = 1;
        g = -g;
        b = -b;
    }
    s->a += D_0015ED60_26d700 * 0.0f;
    particle->xc += D_0015ED60_26d700 * -100.0f;
    fast_vec_add(&next_position, &particle->pos, s);
    if (s->x0 != 0.0f || s->x4 != 0.0f || s->x8 != 0.0f) {
        if (coll_26d700(&particle->pos, &next_position, 2, D_001413D0_lui, 0)) {
            particle->pos = D_L00_00173E60_lui;
            clear_u64_value(s);
            particle->xc += particle->xc;
        } else {
            particle->pos = *(OvlQuad *)&next_position;
            if (s->xc != 0.0f)
                s->x8 += s->xc;
            else
                s->x8 += frame_scale_sq * -0.0063f;
        }
    }
    if (s->a <= 0.0f || fast_dec_timer_s16(&particle->xa) || particle->xc < 0.0f ||
        next_position.f[0] < 0.01f || next_position.f[1] < 0.01f || next_position.f[2] < 0.01f) {
        kill_26d700(particle);
        return;
    }
    if (!neg) {
        r += D_0015ED60_26d700 * -0.01f;
        dt = D_0015ED60_26d700;
        if (r < 0.0f)
            r = 0.0f;
        g += dt * -0.03001f;
        if (g < 0.0f)
            g = 0.0f;
    } else {
        g += D_0015ED60_26d700 * -0.01f;
        dt = D_0015ED60_26d700;
        if (g < 0.0f)
            g = 0.0f;
        r += dt * -0.03001f;
        if (r < 0.0f)
            r = 0.0f;
    }
    b += dt * -0.05f;
    if (b < 0.0f)
        b = 0.0f;
    s->a += dt * 0.0f;
    if (s->a < 0.0f)
        s->a = 0.0f;
    particle->x4 = rgba_26d700(r, g, b, s->a);
    if (neg > 0) {
        s->r = -r;
        s->g = -g;
        s->b = -b;
    } else {
        s->r = r;
        s->g = g;
        s->b = b;
    }
    particle->x8--;
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_0026daa0;

typedef struct {
    char *o;
    short h4;
    short h6;
    int   i8;
    int   iC;
} E_0026daa0;

extern char    g_debug_camera_lui[] __asm__("D_L00_00166DC0") NOT_SDA;
unsigned char *FUN_L00_002678b8_0026daa0(int) __asm__("FUN_L00_002678b8");
int            FUN_001160d8_0026daa0(void) __asm__("FUN_001160d8");
void           FUN_001f9a28_0026daa0(V_0026daa0 *, void *, V_0026daa0 *) __asm__("FUN_001f9a28");
void           FUN_001f9bf8_0026daa0(V_0026daa0 *, V_0026daa0 *, float) __asm__("FUN_001f9bf8");
void           FUN_001f9a10_0026daa0(void *, V_0026daa0 *, V_0026daa0 *) __asm__("FUN_001f9a10");
extern unsigned char *D_L00_001B20E8 NOT_SDA;
unsigned char                       *spawn_source_attached_fading_effect_particle(
                          char *source_moby,
                          int   color,
                          int   lifetime,
                          int   source_position_selector,
                          float initial_particle_scalar) __asm__("FUN_L00_0026daa0");

unsigned char *spawn_source_attached_fading_effect_particle(char *source_moby,
                                                            int   color,
                                                            int   lifetime,
                                                            int   source_position_selector,

                                                            float initial_particle_scalar) {
    unsigned char *particle = FUN_L00_002678b8_0026daa0(0x1A);
    if (particle) {
        E_0026daa0 *particle_data = (E_0026daa0 *)(particle + 0x20);
        V_0026daa0  source_position, position_offset;
        particle_data->o = source_moby;
        particle_data->iC = color >> 24;
        particle_data->h6 = source_position_selector;
        particle_data->i8 = lifetime;
        particle_data->h4 = lifetime;
        if ((short)source_position_selector == -1) {
            qcopy(particle + 0x10, source_moby + 0x10);
        } else {
            moby_point_position(
                particle_data->o, (short)source_position_selector, &source_position);
            FUN_001f9a28_0026daa0(&position_offset, g_debug_camera_lui, &source_position);
            FUN_001f9bf8_0026daa0(&position_offset, &position_offset, 0.4f);
            FUN_001f9a10_0026daa0(particle + 0x10, &source_position, &position_offset);
        }
        particle[8] = FUN_001160d8_0026daa0();
        *(int *)(particle + 4) = color;
        particle[9] = truncate_float_to_s32(4.0f) + 0x70;
        particle[3] = 0x48;
        *(float *)(particle + 0xc) = initial_particle_scalar;
        particle[1] = 0;
        particle[2] = *D_L00_001B20E8;
    }
    return particle;
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_0026dbe0;

typedef struct {
    char *o;
    short h4;
    short h6;
    int   i8;
    int   iC;
} E_0026dbe0;

extern char D_L00_00166DC0_0026dbe0[] __asm__("D_L00_00166DC0") NOT_SDA;
void        FUN_L00_00267a08_0026dbe0(void *) __asm__("FUN_L00_00267a08");
void        FUN_001f9a28_0026dbe0(V_0026dbe0 *, void *, V_0026dbe0 *) __asm__("FUN_001f9a28");
void        FUN_001f9bf8_0026dbe0(V_0026dbe0 *, V_0026dbe0 *, float) __asm__("FUN_001f9bf8");
void        FUN_001f9a10_0026dbe0(void *, V_0026dbe0 *, V_0026dbe0 *) __asm__("FUN_001f9a10");
void        update_source_attached_fading_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026dbe0");

void update_source_attached_fading_effect_particle(unsigned char *particle) {
    E_0026dbe0 *particle_data;
    V_0026dbe0  source_position, position_offset;
    if (particle == 0)
        return;
    particle_data = (E_0026dbe0 *)(particle + 0x20);
    if (fast_dec_timer_s16(particle + 0x24) || particle_data->o == 0 ||
        particle_data->o[0x20] < 0) {
        FUN_L00_00267a08_0026dbe0(particle);
        return;
    }
    if (particle_data->h4 < particle_data->i8 / 5) {
        *(int *)(particle + 4) =
            (*(int *)(particle + 4) & 0xFFFFFF) +
            ((particle_data->iC * particle_data->h4 / (particle_data->i8 / 3)) << 24);
    }
    if (particle_data->h6 == -1) {
        qcopy(particle + 0x10, *(char *volatile *)&particle_data->o + 0x10);
        return;
    }
    moby_point_position(particle_data->o, particle_data->h6, &source_position);
    FUN_001f9a28_0026dbe0(&position_offset, D_L00_00166DC0_0026dbe0, &source_position);
    FUN_001f9bf8_0026dbe0(&position_offset, &position_offset, 0.4f);
    FUN_001f9a10_0026dbe0(particle + 0x10, &source_position, &position_offset);
}

char                 *FUN_L00_002678b8_0026dd20(int) __asm__("FUN_L00_002678b8");
int                   FUN_001160d8_0026dd20(void) __asm__("FUN_001160d8");
extern unsigned char *l00_lifetime_fading_particle_texture_id_source[] __asm__("D_L00_001B20EC");
char                 *FUN_L00_0026dd20(void *pos, void *dir, int a, int b, float f);
char                 *FUN_L00_0026dd20(void *spawn_position,
                                       void *initial_velocity,
                                       int   initial_color,
                                       int   lifetime,

                                       float initial_particle_scalar) {
    char *particle = FUN_L00_002678b8_0026dd20(0x1B);
    if (particle) {
        char *particle_data = particle + 0x20;
        qcopy(particle + 0x10, spawn_position);
        particle[8] = FUN_001160d8_0026dd20();
        *(int *)(particle + 4) = initial_color;
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[3] = 0x48;
        *(float *)(particle + 0xC) = initial_particle_scalar;
        particle[1] = 0;
        particle[2] = l00_lifetime_fading_particle_texture_id_source[0][0];
        *(int *)(particle_data + 0x18) = particle[7];
        qcopy(particle_data, initial_velocity);
        *(int *)(particle_data + 0x10) = lifetime;
        *(int *)(particle_data + 0x14) = lifetime;
    }
    return particle;
}

/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026ECA0), where it is exact; names translated to the US level program. */

void move_and_fade_effect_particle_by_lifetime(char *particle) __asm__("FUN_L00_0026de00");

void move_and_fade_effect_particle_by_lifetime(char *particle) {
    char *particle_data = particle + 0x20;
    fast_vec_add(particle + 0x10, particle + 0x10, particle_data);
    *(unsigned *)(particle + 4) =
        (*(unsigned *)(particle + 4) & 0xFFFFFF) +
        (((*(int *)(particle_data + 0x18) * *(int *)(particle_data + 0x10)) /
          *(int *)(particle_data + 0x14))
         << 24);
    if (fast_dec_timer(particle + 0x30))
        free_effect_particle(particle);
}

typedef struct {
    f32 f[4];
} __attribute__((aligned(16))) V_26de90;

typedef struct {
    f32      q0;
    f32      q4;
    f32      q8;
    f32      qC;
    V_26de90 v30;
} Q_26de90;

typedef struct {
    u8       b0;
    u8       b1;
    u8       b2;
    u8       b3;
    s32      i4;
    u8       b8;
    u8       b9;
    s16      hA;
    f32      fC;
    V_26de90 v10;
    Q_26de90 q;
} M_26de90;

typedef struct {
    f32 f[16];
} __attribute__((aligned(16))) X_26de90;

M_26de90                 *FUN_L00_002678b8_26de90(s32) __asm__("FUN_L00_002678b8");
s32                       FUN_001160d8_26de90(void) __asm__("FUN_001160d8");
extern u8 *D_L00_001B20F0 NOT_SDA;
void                      spawn_jittered_effect_particle_with_optional_basis(void *spawn_origin,
                                                                             void *optional_spawn_basis,
                                                                             f32   size) __asm__("FUN_L00_0026de90");

void spawn_jittered_effect_particle_with_optional_basis(void *spawn_origin,
                                                        void *optional_spawn_basis,

                                                        f32 size) {
    M_26de90 *particle;
    V_26de90  transformed_spawn_offset, spawn_offset, local_velocity;
    X_26de90  x;
    s32       r, g, b;
    Q_26de90 *q;
    particle = FUN_L00_002678b8_26de90(0x1C);
    if (particle) {
        q = &particle->q;
        spawn_offset.f[0] = random_float_between(-0.03f, 0.03f);
        spawn_offset.f[1] = random_float_between(-0.03f, 0.03f);
        spawn_offset.f[2] = 0.0f;
        if (optional_spawn_basis) {
            copy_matrix_rotation(&x, optional_spawn_basis);
            transform_vector(&transformed_spawn_offset, &spawn_offset, &x);
            fast_vec_add(&particle->v10, &transformed_spawn_offset, spawn_origin);
        } else {
            fast_vec_add(&particle->v10, &spawn_offset, spawn_origin);
        }
        particle->v10.f[2] -= 0.05f;
        r = FUN_001160d8_26de90() % 0x60;
        if (!r)
            r = 1;
        g = FUN_001160d8_26de90() % r % 64;
        if (!g)
            g = 1;
        b = FUN_001160d8_26de90() % g % 0x30;
        particle->i4 = (r + 0x8F) | ((g + 0x8F) << 8) | ((b + 0x6F) << 16);
        particle->b9 = truncate_float_to_s32(4.0f) + 0x40;
        particle->b3 = 0x48;
        particle->b1 = 0;
        particle->b8 = 0;
        particle->b2 = *D_L00_001B20F0;
        q->q4 = random_float_between(0.0f, 0.0223456f) + 1e-05f;
        particle->fC = random_float_between(10080.0f, 19950.0f);
        q->q8 = 0.0f;
        particle->hA = scale_game_frames(0xFC);
        q->qC = 1.0f;
        qzero(&local_velocity);
        if (optional_spawn_basis) {
            local_velocity.f[0] = size;
            local_velocity.f[2] = -0.0025f;
            transform_vector_by_basis(&q->v30, &local_velocity, optional_spawn_basis);
        } else {
            q->v30.f[0] = random_float_between(-size, size);
            q->v30.f[1] = random_float_between(-size, size);
            q->v30.f[2] = -0.0025f;
        }
    }
}

typedef struct {
    unsigned char pad0[4];
    unsigned int  i4;
    unsigned char b8;
    unsigned char pad9;
    unsigned char cA[2];
    float         fC;
    float         pos[4];
    struct Q_26e150 {
        float f0;
        float size;
        float phase;
        float vel;
        float dv[4];
    } q;
} M_26e150;

extern float D_0015ED60_26e150 __asm__("D_0015ED60");
extern void  FUN_L00_00267a08_26e150(void *) __asm__("FUN_L00_00267a08");

void update_moving_fade_in_out_effect_particle(M_26e150 *particle) __asm__("FUN_L00_0026e150");

void update_moving_fade_in_out_effect_particle(M_26e150 *particle) {
    struct Q_26e150 *q = &particle->q;
    float            vel = q->vel;
    float            alpha_before_reversal;
    float            update_scale;

    if (0.0f < vel) {
        q->size += D_0015ED60_26e150 * 0.007f * vel;
    } else if (0.03f < q->size) {
        q->size = q->size + D_0015ED60_26e150 * 0.007f * vel;
    } else {
        update_scale = D_0015ED60_26e150;
        q->size = q->size + update_scale * 0.007f * vel * 0.2f;
        particle->fC += update_scale * 5460.0f;
    }
    if (q->size <= 0.0244f || fast_dec_timer_s16(particle->cA) != 0) {
        FUN_L00_00267a08_26e150(particle);
        return;
    }
    alpha_before_reversal = q->size;
    if (0.12f <= alpha_before_reversal) {
        q->vel = -q->vel;
        q->size = alpha_before_reversal + D_0015ED60_26e150 * 0.007f * q->vel;
    }
    particle->fC += D_0015ED60_26e150 * 5460.0f;
    particle->i4 = (particle->i4 & 0xFFFFFF) | (truncate_float_to_s32(q->size * 255.0f) << 24);
    q->phase += D_0015ED60_26e150 * 0.002f;
    if (1.0f < q->phase) {
        q->phase -= 1.0f;
    }
    particle->b8 = (int)(q->phase * 255.0f);
    fast_vec_add(particle->pos, particle->pos, q->dv);
}

typedef struct {
    u8 pad0[0x20];
    s8 b20;
} T_26e3a8;

typedef struct {
    T_26e3a8 *m;
    f32       f4;
    f32       f8;
    s16       hC;
    s16       hE;
    f32       f10;
    f32       f14;
} E_26e3a8;

typedef struct {
    u8       b0, b1, b2, b3;
    u32      w4;
    u8       b8, b9;
    s16      hA;
    f32      fC;
    f32      pos[4] __attribute__((aligned(16)));
    E_26e3a8 e;
} P_26e3a8;

extern u8  *D_L00_001B20F4_lui __asm__("D_L00_001B20F4") NOT_SDA;
extern f32  g_debug_camera[4] __asm__("D_L00_00166DC0") NOT_SDA;
extern void kill_26e3a8(void *) __asm__("FUN_L00_00267a08");
void        update_animated_source_attached_fade_in_out_effect_particle(P_26e3a8 *particle) __asm__("FUN_L00_0026e3a8");

void update_animated_source_attached_fade_in_out_effect_particle(P_26e3a8 *particle) {
    E_26e3a8 *e;
    f32       alpha_reversal_threshold;
    f32       source_attachment_position[4] __attribute__((aligned(16)));
    f32       attachment_position_offset[4] __attribute__((aligned(16)));
    if (particle == 0) {
        return;
    }
    e = &particle->e;
    if (fast_dec_timer_s16(&particle->hA) || e->m == 0 || e->m->b20 < 0) {
        goto kill;
    }
    e->hC = (e->hC + 1) & 0xF;
    if (particle->fC < 159600.0f) {
        particle->fC += D_0015ED60 * 3989.99976f;
        if (159600.0f < particle->fC) {
            particle->fC = 159600.0f;
        }
    } else if (159600.0f < particle->fC) {
        particle->fC -= D_0015ED60 * 15959.999f;
        if (particle->fC < 159600.0f) {
            particle->fC = 159600.0f;
        }
    }
    e->f8 += D_0015ED60 * 0.001f;
    particle->b8 = truncate_float_to_s32(e->f8 * 255.0f);
    particle->b2 = D_L00_001B20F4_lui[e->hC];
    if (particle->fC <= 159600.0f || 0.0f < e->f10) {
        f32 *alpha_value = &e->f4;
        *alpha_value += alpha_value[3];
    }
    {
        f32 x;
        if (particle->fC <= 159600.0f) {
            x = e->f4;
            alpha_reversal_threshold = 0.3f;
        } else {
            x = e->f4;
            alpha_reversal_threshold = 0.15f;
        }
        if (!(alpha_reversal_threshold < x)) {
            if (x < 0.0f) {
            kill:
                kill_26e3a8(particle);
                return;
            }
        } else {
            e->f10 = -e->f10;
            e->f4 = x + e->f10;
        }
    }
    particle->w4 = (particle->w4 & 0xFFFFFF) + (truncate_float_to_s32(e->f4 * 256.0f) << 24);
    e->f14 += (1.0f - e->f14) * (D_0015ED60 * 0.175f);
    if (e->hE != -1) {
        moby_point_position(e->m, e->hE, source_attachment_position);
        fast_vec_sub(attachment_position_offset, g_debug_camera, source_attachment_position);
        fast_vec_normalize(attachment_position_offset, attachment_position_offset, e->f14);
        fast_vec_add(particle->pos, source_attachment_position, attachment_position_offset);
    }
}

typedef union {
    OvlQuad q_26e670;
    f32     f[4];
} V_26e670_26e670;

typedef struct O_26e670_26e670 O_26e670_26e670;

typedef struct {
    f32              v[2];
    f32              f08;
    u8               pC[4];
    O_26e670_26e670 *tgt;
    O_26e670_26e670 *link;
    u32              x18;
    s16              h1C;
    u16              h1E;
} P_26e670_26e670;

struct O_26e670_26e670 {
    u8              p0[4];
    u32             i04;
    u8              b08;
    u8              p9[7];
    V_26e670_26e670 pos;
    P_26e670_26e670 p;
    u8              p40[4];
    s32             x44;
};

extern V_26e670_26e670 D_L00_00166DC0_26e670 __asm__("D_L00_00166DC0");
s32                    FUN_L00_001f0d60_x(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
void                   FUN_L00_001ff660_x(void *, void *, void *) __asm__("FUN_L00_001ff660");
void                   FUN_L00_00267a08_x(O_26e670_26e670 *) __asm__("FUN_L00_00267a08");

void update_linked_effect_particle_with_collision_and_fade(O_26e670_26e670 *particle) __asm__("FUN_L00_0026e670");

void update_linked_effect_particle_with_collision_and_fade(O_26e670_26e670 *particle) {
    V_26e670_26e670  next_position, position_displacement, acc, projected_next_position;
    P_26e670_26e670 *P = &particle->p;

    if ((P->h1E ^ 1) & 1) {
        O_26e670_26e670 *linked_particle = P->link;
        P_26e670_26e670 *linked_particle_data = 0;
        f32              k = 2.0f, linked_velocity_scale_scratch;
        if (linked_particle)
            linked_particle_data = &linked_particle->p;
        clear_u64_value(&acc);
        linked_velocity_scale_scratch = 0.15f;
        if (P->h1C == 0 && linked_particle != 0 &&
            !(*(long long *)&linked_particle_data->x18 & 0x1FFFF00000000LL)) {
            f32 kk;
            f32 linked_displacement_strength = 0.2f, link_rank_step = 1.0f,
                per_link_gain_decay = 0.999f;
            do {
                kk = k * k;
                fast_vec_sub(&next_position, &linked_particle->pos, &particle->pos);
                k += link_rank_step;
                linked_velocity_scale_scratch *= per_link_gain_decay;
                fast_vec_normalize(
                    &next_position, &next_position, linked_displacement_strength / kk);
                fast_vec_add(&acc, &acc, &next_position);
                linked_particle = linked_particle_data->link;
                if (linked_particle)
                    linked_particle_data = &linked_particle->p;
                if (!linked_particle)
                    break;
            } while (!(*(long long *)&linked_particle_data->x18 & 0x1FFFF00000000LL));
        }
        linked_velocity_scale_scratch = (0.15f - linked_velocity_scale_scratch) + 1.07f;
        if (linked_particle_data == 0 || linked_particle_data->h1C == 0)
            fast_vec_add(P, P, &acc);
        else
            fast_vec_scale(P, P, 0.5f);
        if (P->link)
            fast_vec_scale(P, P, linked_velocity_scale_scratch);
        else
            fast_vec_scale(P, P, 0.97f);
        P->f08 -= frame_scale_sq * 0.02f;
        fast_vec_add(&next_position, &particle->pos, P);
    } else {
        qcopy(&next_position, &particle->pos);
    }
    if (P->tgt) {
        fast_vec_sub(&acc, &next_position, &P->tgt->pos);
        if (0.1f < absolute_float(fast_vec_length(&acc) - 0.15f)) {
            fast_vec_normalize(&acc, &acc, 0.15f);
            fast_vec_add(&next_position, &acc, &P->tgt->pos);
        }
        if (FUN_L00_001f0d60_x(&next_position, 0, 0, 0.15f)) {
            if (P->h1C == 0)
                qcopy(&next_position, &coll_hit_position);
            else
                qcopy(&next_position, &particle->pos);
            P->h1C = 1;
        } else {
            P->h1C = 0;
        }
        fast_vec_sub(&acc, &P->tgt->pos, &D_L00_00166DC0_26e670);
        transform_vector(&acc, &acc, (u8 *)&D_L00_00166DC0_26e670 - 0x80);
        fast_vec_scale(&acc, &acc, 1.0f / acc.f[3]);
        fast_vec_sub(&projected_next_position, &next_position, &D_L00_00166DC0_26e670);
        transform_vector(&projected_next_position,
                         &projected_next_position,
                         (u8 *)&D_L00_00166DC0_26e670 - 0x80);
        fast_vec_scale(&projected_next_position,
                       &projected_next_position,
                       1.0f / projected_next_position.f[3]);
        fast_vec_sub(&acc, &acc, &projected_next_position);
        particle->b08 =
            truncate_float_to_s32(fast_arc_tan(acc.f[0], acc.f[1]) * 128.0f / 3.1415927f) - 0x20;
        if (P->h1C) {
            FUN_L00_001ff660_x(P, P, &coll_hit_normal);
            fast_vec_scale(P, P, 0.95f);
        } else {
            fast_vec_sub(&position_displacement, &next_position, &particle->pos);
            fast_vec_add(P, P, &position_displacement);
            fast_vec_scale(P, P, 0.5f);
        }
        fast_vec_sub(&position_displacement, &next_position, &particle->pos);
        fast_vec_add(P, P, &position_displacement);
        fast_vec_scale(P, P, 0.5f);
    }
    qcopy(&particle->pos, &next_position);
    if (P->h1E & 2) {
        s32 faded_alpha = (particle->i04 >> 24) - 2;
        if (faded_alpha > 0) {
            particle->i04 = (particle->i04 & 0xFFFFFF) | (faded_alpha << 24);
        } else if (P->link == 0) {
            if (P->tgt)
                P->tgt->p.link = 0;
            FUN_L00_00267a08_x(particle);
        }
    }
}

extern float D_0015ED60_26eb88 __asm__("D_0015ED60");
extern void  kill_26eb88(void *) __asm__("FUN_L00_00267a08");
extern float two_point_particle_first_point_target_offset_scale_gp __asm__("D_L00_001601F4")
    __attribute__((sda));
extern float two_point_particle_second_point_target_offset_scale_gp __asm__("D_L00_001601F8")
    __attribute__((sda));
void update_target_relative_two_point_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026eb88");

void update_target_relative_two_point_effect_particle(unsigned char *particle) {
    OvlQuad        position_scratch;
    OvlQuad        target_offset_scratch[1];
    unsigned char *target_object = *(unsigned char **)(particle + 0x30);
    if (target_object == 0 || target_object[0x20] == 0xFE || target_object[0x20] == 0xFD ||
        fast_dec_timer_s16(particle + 0xA)) {
        kill_26eb88(particle);
        return;
    }
    fast_vec_sub(&position_scratch, *(unsigned char **)(particle + 0x30) + 0x10, particle + 0x10);
    target_offset_scratch[0] = position_scratch;
    fast_vec_scale(
        target_offset_scratch,
        target_offset_scratch,
        (two_point_particle_first_point_target_offset_scale_gp - 1.0f) * D_0015ED60_26eb88 + 1.0f);
    fast_vec_add(
        &position_scratch, target_offset_scratch, *(unsigned char **)(particle + 0x30) + 0x10);
    *(OvlQuad *)(particle + 0x10) = position_scratch;
    fast_vec_sub(&position_scratch, *(unsigned char **)(particle + 0x30) + 0x10, particle + 0x20);
    target_offset_scratch[0] = position_scratch;
    fast_vec_scale(
        target_offset_scratch,
        target_offset_scratch,
        (two_point_particle_second_point_target_offset_scale_gp - 1.0f) * D_0015ED60_26eb88 + 1.0f);
    fast_vec_add(
        &position_scratch, target_offset_scratch, *(unsigned char **)(particle + 0x30) + 0x10);
    *(OvlQuad *)(particle + 0x20) = position_scratch;
    if (fast_vec_dist(particle + 0x10, particle + 0x20) < 0.1f ||
        fast_vec_dist(particle + 0x10, *(unsigned char **)(particle + 0x30) + 0x10) < 1.0f) {
        kill_26eb88(particle);
    }
}

typedef struct {
    void *a;
    f32   f4;
    f32   f8;
    f32   fC;
    f32   f10;
    f32   f14;
    f32   f18;
} Body_26ed28;

typedef struct {
    u8          b0;
    u8          b1;
    u8          b2;
    u8          b3;
    s32         w4;
    u8          b8;
    u8          b9;
    s16         hA;
    f32         fC;
    u128        pos;
    Body_26ed28 body;
} Part_26ed28;

extern void *FUN_L00_002678b8_0026ed28(s32) __asm__("FUN_L00_002678b8");
extern s32   FUN_L00_0025bfe0_0026ed28(f32, f32, f32, f32) __asm__("FUN_L00_0025bfe0");
extern u8   *D_L00_001B20A0_lui[] __asm__("D_L00_001B20A0") NOT_SDA;
Part_26ed28 *spawn_moby_bound_effect_particle(u8 *a) __asm__("FUN_L00_0026ed28");

Part_26ed28 *spawn_moby_bound_effect_particle(u8 *a) {
    Part_26ed28 *particle;
    Body_26ed28 *particle_data;
    particle = FUN_L00_002678b8_0026ed28(0x20);
    if (particle) {
        particle_data = &particle->body;
        particle_data->a = a;
        particle_data->f4 = 0.0f;
        particle_data->f8 = frame_scale * 0.016f;
        particle_data->fC = random_float_between(particle_data->f4, 1.0f);
        particle_data->f10 = 1.0f;
        particle_data->f14 = 1.0f;
        particle_data->f18 = 1.0f;
        qcopy(&particle->pos, a + 0x10);
        particle->w4 = FUN_L00_0025bfe0_0026ed28(1.0f, 1.0f, 1.0f, particle_data->f4);
        particle->b9 = truncate_float_to_s32(4.0f) + 0x40;
        particle->b1 = 0;
        particle->b3 = 0x48;
        particle->fC = random_float_between(50000.0f, 50000.0f);
        particle->b8 = truncate_float_to_s32(particle_data->fC * 255.0f);
        particle->b2 = *D_L00_001B20A0_lui[0];
    }
    return particle;
}

typedef struct {
    unsigned char *o;
    float          f4, f8, fC, f10, f14, f18;
} B_26ee48;

extern float D_0015ED60_26ee48 __asm__("D_0015ED60");
void         FUN_L00_00267a08_26ee48(void *) __asm__("FUN_L00_00267a08");
int          FUN_L00_0025bfe0_26ee48(float, float, float, float) __asm__("FUN_L00_0025bfe0");
void         update_moby_bound_fade_in_out_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026ee48");

void update_moby_bound_fade_in_out_effect_particle(unsigned char *particle) {
    B_26ee48      *particle_data;
    unsigned char *o;
    if (particle == 0)
        return;
    particle_data = (B_26ee48 *)(particle + 0x20);
    o = particle_data->o;
    if (o == 0 || (o[0x20] & 0xF0) == 0xF0)
        goto kill;
    qcopy(particle + 0x10, o + 0x10);
    particle_data->f4 += particle_data->f8;
    if (particle_data->f4 < 0.0f) {
    kill:
        FUN_L00_00267a08_26ee48(particle);
        return;
    }
    if (0.6f < particle_data->f4) {
        particle_data->f8 = -particle_data->f8;
        particle_data->f4 += particle_data->f8;
    }
    particle_data->f10 += D_0015ED60_26ee48 * -0.0174f;
    if (particle_data->f10 < 0.0f)
        particle_data->f10 = 0.0f;
    particle_data->f14 += D_0015ED60_26ee48 * -0.01446f;
    if (particle_data->f14 < 0.0f)
        particle_data->f14 = 0.0f;
    particle_data->f18 += D_0015ED60_26ee48 * -0.0015f;
    if (particle_data->f18 < 0.0f)
        particle_data->f18 = 0.0f;
    {
        int packed_color = FUN_L00_0025bfe0_26ee48(
            particle_data->f10, particle_data->f14, particle_data->f18, particle_data->f4);
        float phase_increment = D_0015ED60_26ee48 * 0.0011f;
        *(int *)(particle + 4) = packed_color;
        particle_data->fC += phase_increment;
    }
    if (1.0f < particle_data->fC)
        particle_data->fC -= 1.0f;
    particle[8] = truncate_float_to_s32(particle_data->fC * 255.0f);
    *(float *)(particle + 0xC) =
        *(float *)(*(unsigned char **)(particle + 0x20) + 0x2C) * 100000.0f * 30.0f + 50000.0f;
}

/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_0026FEE8), where it is exact; names translated to the US level program. */

/* Calls free_effect_particle on a once fast_dec_timer_s16(a + 10) returns non-zero. */
void tick_effect_particle_lifetime(char *particle) __asm__("FUN_L00_0026f048");

void tick_effect_particle_lifetime(char *particle) {
    if (fast_dec_timer_s16(particle + 10) != 0) {
        free_effect_particle(particle);
    }
}

typedef struct {
    unsigned char pad0[0x88];
    float         f88;
    unsigned char pad8C[0x2F0 - 0x8C];
    float         f2F0;
    unsigned char pad2F4[0x2084 - 0x2F4];
    int           i2084;
} P_26f080;

unsigned char                       *FUN_L00_002678b8_26f080(int) __asm__("FUN_L00_002678b8");
int                                  FUN_001f96f8_26f080(int) __asm__("FUN_001f96f8");
extern unsigned char *D_L00_001B2108 NOT_SDA;
unsigned char                       *FUN_L00_0026f080(void *pos, float *v, float x, float y);

unsigned char *FUN_L00_0026f080(void *pos, float *initial_velocity, float x, float y) {
    unsigned char *particle = FUN_L00_002678b8_26f080(0x22);
    if (particle) {
        float *particle_data = (float *)(particle + 0x20);
        int    minimum_lifetime;
        *(int *)(particle + 4) = 0x88A0A0A0;
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[1] = 0;
        particle[3] = 0x44;
        particle_data[3] = random_float_between(0.75f, 1.25f) * x;
        *(float *)(particle + 0xC) = 5250.0f;
        particle[8] = random_int_between(-10, 10) - 0x80;
        particle[2] = *D_L00_001B2108;
        minimum_lifetime = FUN_001f96f8_26f080(0x46);
        *(short *)(particle + 0xA) =
            random_int_between(minimum_lifetime, FUN_001f96f8_26f080(0x8C));
        if (y == -1.0f) {
            struct Hero *hero_ptr = &hero;
            if (hero_ptr->state.current == 0x31) {
                particle_data[4] = hero_ptr->motion.pos.f[2] + 0.4f;
            } else {
                particle_data[4] = hero_ptr->water_surface_z;
            }
        } else {
            particle_data[4] = y;
        }
        particle_data[5] = random_angle_radians();
        particle_data[6] = random_float_between(frame_time * 1.7453293f, frame_time * 6.981317f);
        particle_data[7] = random_float_between(frame_time * 0.5f, frame_time * 1.4f);
        particle_data[0] = initial_velocity[0];
        particle_data[1] = initial_velocity[1];
        particle_data[2] = initial_velocity[2];
        qcopy(particle + 0x10, pos);
    }
    return particle;
}
/* Updates a particle that swirls on a circle: fades with age, spins, and moves along its orbit. */
/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_00270120), where it is exact; names translated to the US level program. */

extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float D_0015ED60_c __asm__("D_0015ED60");
extern float D_L00_00166DD8_lui __asm__("D_L00_00166DD8") NOT_SDA;

void update_swaying_and_fading_effect_particle(char *particle) __asm__("FUN_L00_0026f280");

void update_swaying_and_fading_effect_particle(char *particle) {
    char *particle_data = particle + 0x20;
    float motion_vector_scratch[3];
    char *q;
    float offset_angle, k;
    if (fast_dec_timer_s16(particle + 0xA)) {
        free_effect_particle(particle);
        return;
    }
    if (*(float *)(particle + 0x18) > *(float *)(particle_data + 0x10)) {
        int s;
        *(int *)(particle_data + 8) = 0;
        s = *(short *)(particle + 0xA);
        if (scale_game_frames(0xE) - 2 < s)
            *(short *)(particle + 0xA) = scale_game_frames(0xE) - 2;
    }
    if (*(float *)(particle_data + 0x10) <
        *(float *)(particle + 0x18) + frame_time * 1.3f * 40.0f) {
        if (random_integer_below(0x14) == 0) {
            int s = *(short *)(particle + 0xA);
            if (scale_game_frames(0xE) - 2 < s)
                *(short *)(particle + 0xA) = scale_game_frames(0xE) - 2;
        }
    }
    if (*(short *)(particle + 0xA) < scale_game_frames(0xE)) {
        int alpha_before_fade = *(unsigned char *)(particle + 7);
        *(int *)(particle + 4) =
            ((alpha_before_fade * *(short *)(particle + 0xA) / scale_game_frames(0xE)) << 24) |
            (*(int *)(particle + 4) & 0xFFFFFF);
    }
    *(float *)(particle + 0xC) += D_0015ED60_c * 840.00006f;
    if (*(float *)(particle_data + 0xC) < *(float *)(particle + 0xC))
        *(float *)(particle + 0xC) = *(float *)(particle_data + 0xC);
    motion_vector_scratch[0] = *(float *)(particle_data + 0);
    motion_vector_scratch[1] = *(float *)(particle_data + 4);
    motion_vector_scratch[2] = *(float *)(particle_data + 8);
    {
        float horizontal_speed = vector_length_xy(motion_vector_scratch);
        set_vector_length_xy(motion_vector_scratch,
                             motion_vector_scratch,
                             D_0015ED60_c * -0.029999971f * horizontal_speed + horizontal_speed);
    }
    fast_vec_add(particle + 0x10, particle + 0x10, motion_vector_scratch);
    approach_value(motion_vector_scratch + 2, frame_time * 1.3f, frame_time_sq * 5.0f);
    *(float *)(particle_data + 0) = motion_vector_scratch[0];
    *(float *)(particle_data + 4) = motion_vector_scratch[1];
    *(float *)(particle_data + 8) = motion_vector_scratch[2];
    *(float *)(particle_data + 0x14) =
        fast_add_rotations(*(float *)(particle_data + 0x14), *(float *)(particle_data + 0x18));
    offset_angle = fast_add_rotations(D_L00_00166DD8_lui, 1.5707964f);
    k = fast_sin(*(float *)(particle_data + 0x14)) * *(float *)(particle_data + 0x1C);
    motion_vector_scratch[0] = fast_cos(offset_angle) * k;
    motion_vector_scratch[1] = fast_sin(offset_angle) * k;
    motion_vector_scratch[2] = 0;
    fast_vec_add(particle + 0x10, particle + 0x10, motion_vector_scratch);
}

extern float   D_0015ED70_0026f548 __asm__("D_0015ED70");
unsigned char *FUN_L00_002678b8_0026f548(int) __asm__("FUN_L00_002678b8");

extern unsigned char *D_L00_001B210C NOT_SDA;

void FUN_L00_0026f548(u128 *spawn_position, u128 *initial_velocity, int k, int j) {
    unsigned char *particle = FUN_L00_002678b8_0026f548(0x23);
    if (particle) {
        unsigned char *particle_data;
        qcopy(particle + 0x10, spawn_position);
        particle_data = particle + 0x20;
        *(int *)(particle + 4) = 0x80808080;
        particle[9] = truncate_float_to_s32(2.0f) + 0x10;
        particle[1] = 0;
        particle[3] = 0x44;
        *(float *)(particle + 0xc) = random_float_between(10500.0f, 16800.0f);
        particle[2] = *D_L00_001B210C;
        qcopy(particle_data, initial_velocity);
        *(int *)(particle_data + 0x14) = j;
        *(int *)(particle_data + 0x10) = k;
        *(float *)(particle_data + 0x18) = *(float *)(particle + 0x18);
        if (k == 2) {
            *(int *)(particle_data + 0x18) = 0;
        }
        *(float *)(particle_data + 0x1c) =
            random_float_between(D_0015ED70_0026f548 * 10.5f, D_0015ED70_0026f548 * 18.0f);
    }
}
/* Moves a particle by its velocity according to its mode, bouncing or killing it on contact. */
/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_00270500), where it is exact; names translated to the US level program. */

typedef struct {
    int a[4];
} V __attribute__((aligned(16)));

extern V    D_L00_00173E60;
extern int  collision_hit_material(void) __asm__("FUN_001f0b58");
extern void spawn_moving_resizing_height_snapped_alpha_fade_particle(
    void *, int, int, float, float) __asm__("FUN_L00_002715e8");
extern void
spawn_random_planar_drift_particle(void *, int, float, float) __asm__("FUN_L00_002741a0");

void update_ballistic_effect_particle_with_contact_spawns(char *particle) __asm__("FUN_L00_0026f660");

void update_ballistic_effect_particle_with_contact_spawns(char *particle) {
    char *particle_data = particle + 0x20;
    int   mode = *(int *)(particle_data + 0x10);
    V     next_position;
    char *particle_position;

    if (mode == 0) {
        fast_vec_add(particle + 0x10, particle + 0x10, particle_data);
    } else if (mode == 1) {
        particle_position = particle + 0x10;
        fast_vec_add(&next_position, particle_position, particle_data);
        if (collision_line(particle_position, &next_position, 2, 0, 0)) {
            if (collision_hit_material() == 0) {
                spawn_moving_resizing_height_snapped_alpha_fade_particle(
                    &D_L00_00173E60, 0, -1, 0.2f, 5250.0f);
            }
            free_effect_particle(particle);
            return;
        }
        qcopy(particle_position, &next_position);
    } else if (mode == 3) {
        float water_surface_z;
        particle_position = particle + 0x10;
        fast_vec_add(particle_position, particle_position, particle_data);
        water_surface_z = hero.water_surface_z;
        if (*(float *)(particle + 0x18) < water_surface_z) {
            *(float *)(particle + 0x18) = water_surface_z;
            spawn_moving_resizing_height_snapped_alpha_fade_particle(
                particle_position, 0, -1, 0.2f, 5250.0f);
            free_effect_particle(particle);
            return;
        }
    } else {
        particle_position = particle + 0x10;
        fast_vec_add(&next_position, particle_position, particle_data);
        if (collision_line(particle_position, &next_position, 2, 0, 0)) {
            if (collision_hit_material() != 0) {
                int *collision_data = D_L00_00173E40;
                *(float *)((char *)collision_data + 0x28) += 0.05f;
                spawn_random_planar_drift_particle(collision_data + 8, -1, 0.25f, 3150.0f);
            }
            free_effect_particle(particle);
            return;
        }
        qcopy(particle_position, &next_position);
    }
    *(float *)(particle_data + 8) -= *(float *)(particle_data + 0x1C);
    *(unsigned char *)(particle + 8) += 2;
    *(int *)(particle_data + 0x14) -= 1;
    if (*(int *)(particle_data + 0x14) <= 0 ||
        *(float *)(particle + 0x18) < *(float *)(particle_data + 0x18)) {
        *(int *)(particle + 4) += 0xF0000000;
        if (*(int *)(particle + 4) < 0) {
            free_effect_particle(particle);
        }
    }
}

extern u128  D_L00_00166DC0_26f898 __asm__("D_L00_00166DC0");
extern float FUN_001f9af0_26f898(void *) __asm__("FUN_001f9af0");
extern void  FUN_001f9bf8_26f898(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  FUN_001f9a68_26f898(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001f9de0_26f898(float) __asm__("FUN_001f9de0");
extern void  FUN_L00_00267a08_26f898(void *) __asm__("FUN_L00_00267a08");

typedef struct {
    int  *flag;
    float ang;
    float lim;
    int   arg;
    u128  v10;
} Q_26f898;

void update_flag_controlled_pulsing_effect_particle(unsigned char *particle) __asm__("FUN_L00_0026f898");

void update_flag_controlled_pulsing_effect_particle(unsigned char *particle) {
    u128      position_offset;
    Q_26f898 *particle_data;
    float     angle_sine;
    fast_vec_sub(&position_offset, &D_L00_00166DC0_26f898, particle + 0x30);
    particle_data = (Q_26f898 *)(particle + 0x20);
    if (1.0f < FUN_001f9af0_26f898(&position_offset)) {
        FUN_001f9bf8_26f898(&position_offset, &position_offset, 0.25f);
    } else {
        FUN_001f9a68_26f898(&position_offset, &position_offset, 0.25f);
    }
    fast_vec_add(particle + 0x10, &particle_data->v10, &position_offset);
    particle_data->ang =
        fast_add_rotations(particle_data->ang, *(float *)((char *)particle_data + 0x1C));
    angle_sine = FUN_001f9de0_26f898(particle_data->ang);
    if (fast_dec_timer_s16(particle + 0xA) &&
        (angle_sine < particle_data->lim || angle_sine < -0.9f)) {
        goto kill;
    }
    if (*particle_data->flag == 0) {
    kill:
        *particle_data->flag = 0;
        FUN_L00_00267a08_26f898(particle);
        return;
    }
    if (particle_data->lim < angle_sine) {
        *(int *)(particle + 4) = mine_blend_color(
            0, particle_data->arg, (angle_sine - particle_data->lim) / (1.0f - particle_data->lim));
    } else {
        *(int *)(particle + 4) = 0;
    }
}

typedef float V4[4] __attribute__((aligned(16)));
float         fast_vec_length(void *) __asm__("FUN_001f9af0");
void          fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void          fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
void          update_offset_and_pulse_effect_particle(char *particle) __asm__("FUN_L00_0026fa00");

void update_offset_and_pulse_effect_particle(char *particle) {
    V4   position_offset;
    int *particle_data = (int *)(particle + 0x20);
    int  color_blend_peak_ticks, k;
    if (fast_dec_timer_s16(particle + 0xA)) {
        free_effect_particle(particle);
        return;
    }
    fast_vec_sub(position_offset, D_L00_00166DC0, particle_data);
    if (5.0f < fast_vec_length(position_offset))
        fast_vec_normalize(position_offset, position_offset, 1.25f);
    else
        fast_vec_scale(position_offset, position_offset, 0.25f);
    fast_vec_add(particle + 0x10, particle_data, position_offset);
    color_blend_peak_ticks = particle_data[5];
    k = *(short *)(particle + 0xA) - color_blend_peak_ticks;
    k = k < 0 ? -k : k;
    *(int *)(particle + 4) = mine_blend_color(
        0, particle_data[4], (float)(color_blend_peak_ticks - k) / (float)color_blend_peak_ticks);
}

typedef struct {
    float v[4];
    float f10;
    float f14;
} Q_26fb00;

typedef struct {
    char     pad[4];
    int      c4;
    u8       b8;
    u8       b9;
    s16      ha;
    float    fc;
    float    v10[4];
    Q_26fb00 q;
} P_26fb00;

extern float D_0015ED60_26fb00 __asm__("D_0015ED60");
extern void  FUN_L00_00267a08_26fb00(void *) __asm__("FUN_L00_00267a08");
extern void  FUN_001f9a68_26fb00(void *, void *, float) __asm__("FUN_001f9a68");

void update_damped_and_fading_effect_particle(P_26fb00 *particle) __asm__("FUN_L00_0026fb00");

void update_damped_and_fading_effect_particle(P_26fb00 *particle) {
    Q_26fb00 *q = &particle->q;
    float     zero, x, y;
    if (fast_dec_timer_s16(&particle->ha)) {
        goto kill;
    }
    particle->fc += q->f10;
    particle->b8++;
    zero = 0.0f;
    x = q->f14 - D_0015ED60_26fb00 * 0.01f;
    q->f14 = x;
    y = zero;
    if (!(x < zero))
        y = x;
    q->f14 = y;
    particle->c4 &= 0xFFFFFF;
    particle->c4 |= truncate_float_to_s32(q->f14 * 255.0f) << 24;
    if (q->f14 <= zero) {
    kill:
        FUN_L00_00267a08_26fb00(particle);
        return;
    }
    fast_vec_add(particle->v10, particle->v10, q);
    FUN_001f9a68_26fb00(q, q, D_0015ED60_26fb00 * -0.019999981f + 1.0f);
}

typedef struct {
    f32 v[4];
} V0026fc38 __attribute__((aligned(16)));

typedef struct {
    s32 x0;
    u8 *x4;
    s32 x8;
} S0026fc38;

typedef struct {
    u8        p0[2];
    u8        x2;
    u8        p3;
    s32       x4;
    u8        x8;
    u8        p9;
    s16       xa;
    f32       xc;
    u8        p10[0x10];
    S0026fc38 s;
} O0026fc38;

extern u8  D_L00_00166DC0_0026fc38[] __asm__("D_L00_00166DC0");
void       FUN_L00_00267a08_0026fc38(void *) __asm__("FUN_L00_00267a08");
extern f32 animated_offset_particle_scalar_gp __asm__("D_L00_00160200") __attribute__((sda));
extern s32 animated_offset_particle_packed_color_gp __asm__("D_L00_00160204") __attribute__((sda));
extern f32 animated_offset_particle_offset_distance_gp __asm__("D_L00_00160208")
    __attribute__((sda));
extern u8 animated_offset_particle_rotation_step_gp __asm__("D_L00_0016020C") __attribute__((sda));
extern u8 *D_L00_001B211C NOT_SDA;
void update_animated_offset_effect_particle(O0026fc38 *particle) __asm__("FUN_L00_0026fc38");

void update_animated_offset_effect_particle(O0026fc38 *particle) {
    V0026fc38  position_offset;
    S0026fc38 *s = &particle->s;
    particle->xc = animated_offset_particle_scalar_gp;
    particle->x4 = animated_offset_particle_packed_color_gp;
    particle->x8 += animated_offset_particle_rotation_step_gp;
    fast_vec_sub(&position_offset, s->x4 + 0x10, D_L00_00166DC0_0026fc38);
    fast_vec_normalize(
        &position_offset, &position_offset, animated_offset_particle_offset_distance_gp);
    fast_vec_add(particle->p10, s->x4 + 0x10, &position_offset);
    particle->x2 = D_L00_001B211C[truncate_float_to_s32((f32)(s->x8 / 2))];
    s->x8++;
    if (s->x8 >= 0x1F)
        s->x8 = 0;
    if (particle->xa == 0)
        FUN_L00_00267a08_0026fc38(particle);
}

/* NEAR: only diff is a Ps2EeAs nop between mul.s and div.s after the div-check label (assembler wall). */
typedef struct {
    u128 vel;
    f32  f10;
    s16  h14;
    s16  h16;
    u8   b18;
    u8   pad19[3];
    s32  w1c;
} E_26fd28;

typedef struct {
    u8       b0, b1, b2, b3;
    s32      w4;
    u8       b8, b9;
    s16      hA;
    f32      fC;
    u128     pos;
    E_26fd28 e;
} P_26fd28;

extern P_26fd28      *alloc_26fd28(s32) __asm__("FUN_L00_002678b8");
extern s32            FUN_L00_0025bfe0(f32, f32, f32, f32);
extern s32            rnd_26fd28(void) __asm__("FUN_001160d8");
extern unsigned char *effect_texture_ids __asm__("D_L00_001B20B0") NOT_SDA;
P_26fd28             *spawn_splitting_effect_particle_with_collision_response_and_fade(
                u128 *pos,
                u128 *initial_velocity,
                u8    initial_flags,
                s32   collision_context) __asm__("FUN_L00_0026fd28");

P_26fd28 *spawn_splitting_effect_particle_with_collision_response_and_fade(u128 *pos,
                                                                           u128 *initial_velocity,
                                                                           u8    initial_flags,

                                                                           s32 collision_context) {
    P_26fd28 *particle;
    E_26fd28 *e;
    f32       initial_rotation_phase;
    s32       lifetime_random_value;
    particle = alloc_26fd28(0x28);
    if (particle != 0) {
        e = &particle->e;
        qcopy(&particle->pos, pos);
        qcopy(e, initial_velocity);
        particle->w4 = FUN_L00_0025bfe0(0.7f, 0.7f, 0.7f, 0.08f);
        particle->b9 = truncate_float_to_s32(4.0f) + 0x40;
        particle->b1 = 0;
        particle->b3 = 0x44;
        initial_rotation_phase = random_angle_radians();
        e->f10 = initial_rotation_phase;
        particle->b8 = (s32)(initial_rotation_phase * 256.0f);
        particle->b2 = effect_texture_ids[rnd_26fd28() & 7];
        e->b18 = initial_flags;
        e->h14 = 0;
        e->h16 = 0;
        if (initial_flags < 5) {
            particle->fC = random_float_between(7000.0f, 10000.0f);
            lifetime_random_value = rnd_26fd28();
            {
                s32 base_lifetime = (s32)(8.0f / (D_0015ED60 * 0.443f));
                particle->hA = base_lifetime - lifetime_random_value % (base_lifetime * 6 / 7);
            }
        } else {
            particle->fC = 10000.0f;
            particle->hA = (s32)(8.0f / (D_0015ED60 * 0.443f));
        }
        e->w1c = collision_context;
    }
    return particle;
}

typedef unsigned int U128 __attribute__((mode(TI)));

typedef union {
    U128 q_26ff10;
    f32  f[4];
} VV;

typedef struct PV {
    f32   x, y, z, w_26ff10;
    f32   t;
    s16   f14;
    s16   f16;
    u8    f18;
    char  pad19[3];
    void *f1C;
} PV;

typedef struct Moby_26ff10 {
    u8   b00_26ff10;
    char pad01;
    u8   b02;
    char pad03;
    u32  f04;
    u8   f08;
    char pad09;
    s16  h0A;
    f32  f0C;
    VV   posv;
    PV   pv;
} Moby_26ff10;

typedef struct {
    VV    v_26ff10;
    void *f10;
    s32   f14;
    s32   f18;
    f32   f1C;
    s32   unk20;
    char  pad[0xC];
} S_26ff10;

extern unsigned char *effect_texture_ids __asm__("D_L00_001B20B0") NOT_SDA;

extern Moby_26ff10             *
FUN_L00_0026fd28_26ff10(void *, void *, s32, void *) __asm__("FUN_L00_0026fd28");
extern s32  FUN_001efa68_26ff10(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void FUN_L00_00267a08_26ff10(Moby_26ff10 *) __asm__("FUN_L00_00267a08");

void update_splitting_effect_particle_with_collision_response_and_fade(
    Moby_26ff10 *particle) __asm__("FUN_L00_0026ff10");

void update_splitting_effect_particle_with_collision_response_and_fade(Moby_26ff10 *particle) {
    VV           v0u;
    VV           v1u;
    S_26ff10     collision_parameters;
    f32          collision_offset[4];
    PV          *pv;
    Moby_26ff10 *spawned_particle;
    u8          *tab;
    u8           fl;
    s32          fm;
    s64          hit;
    s32          k, mode, kk2, t;
    void        *aux, *col, *pa, *pb;
    f32          f, rotation_direction_sign, alpha_factor;

    pv = &particle->pv;
    if (particle->h0A != 0 && fast_dec_timer_s16(&particle->h0A) != 0) {
        f32 sp, lo, hi, sc, zero, nsp;
        pv->f18 |= 0x88;
        lo = 0.5f;
        sp = particle->f0C / 900000.0f;
        hi = 0.7f;
        sc = 0.2f;
        particle->f0C = particle->f0C * 0.8f;
        zero = 0.0f;
        f = random_float_between(lo, hi);
        nsp = -sp;
        fast_vec_scale(v0u.f, pv, f);
        v1u.f[0] = particle->posv.f[0] + random_float_between(nsp, sp);
        v1u.f[1] = particle->posv.f[1] + random_float_between(nsp, sp);
        v1u.f[2] = particle->posv.f[2] + random_float_between(nsp, sp);
        v0u.f[0] = v0u.f[0] + random_float_between(-particle->pv.x * sc, particle->pv.x * sc);
        v0u.f[1] = v0u.f[1] + random_float_between(-pv->y * sc, pv->y * sc);
        v0u.f[2] = v0u.f[2] + random_float_between(-pv->z * sc, zero);
        spawned_particle = FUN_L00_0026fd28_26ff10(v1u.f, v0u.f, 0x88, pv->f1C);
        if (spawned_particle != 0) {
            spawned_particle->h0A = scale_game_frames(2);
            spawned_particle->f0C = particle->f0C;
        }
        fast_vec_scale(pv, pv, random_float_between(lo, hi));
        particle->pv.x =
            particle->pv.x + random_float_between(-particle->pv.x * sc, particle->pv.x * sc);
        pv->y = pv->y + random_float_between(-pv->y * sc, pv->y * sc);
        pv->z = pv->z + random_float_between(-pv->z * sc, zero);
        particle->posv.f[0] = particle->posv.f[0] + random_float_between(nsp, sp);
        particle->posv.f[1] = particle->posv.f[1] + random_float_between(nsp, sp);
        particle->posv.f[2] = particle->posv.f[2] + random_float_between(nsp, sp);
    }
    tab = effect_texture_ids;
    particle->b02 = tab[(particle->h0A + pv->f16 + (particle->b02 - tab[0])) & 7];
    fast_vec_add(v0u.f, particle->posv.f, pv);
    k = pv->f14;
    if (scale_game_frames(3) < k) {
        fast_vec_sub(v1u.f, particle->posv.f, pv);
    } else {
        v1u.q_26ff10 = particle->posv.q_26ff10;
    }
    fast_vec_scale(&collision_parameters, &collision_parameters, 0.01f);
    fl = pv->f18;
    if (fl & 8) {
        if ((particle->f04 & 0xFF000000) == 0)
            goto kill;
        pv->f18 = fl | 8;
        col = pv->f1C;
        pa = v1u.f;
        pb = v0u.f;
        mode = 4;
        aux = 0;
    } else {
        collision_parameters.f10 = pv->f1C;
        collision_parameters.f14 = 0x10001;
        collision_parameters.f1C = 1.0f;
        collision_parameters.v_26ff10.q_26ff10 = *(U128 *)pv;
        if (fl & 0x40)
            clear_u64_value(&collision_parameters);
        collision_parameters.unk20 = 1;
        col = pv->f1C;
        aux = &collision_parameters;
        pa = v1u.f;
        pb = v0u.f;
        mode = 0;
    }
    hit = FUN_001efa68_26ff10(pa, pb, mode, col, aux) != 0;
    if (hit) {
        fast_vec_normalize(collision_offset, coll_hit_normal.f, 1.0f);
        reflect_vector(pv, pv, coll_hit_normal.f);
        fast_vec_scale(pv, pv, 0.02f);
        fast_vec_normalize(collision_offset, coll_hit_normal.f, 0.055f);
        fast_vec_add(particle->posv.f, coll_hit_normal.f - 8, collision_offset);
        if (particle->f0C < 200000.0f && (pv->f18 & 8) == 0) {
            particle->f0C = particle->f0C + particle->f0C;
        }
        pv->f18 |= 8;
    } else {
        qcopy(particle->posv.f, v0u.f);
    }
    if (pv->f18 & 8) {
        f32 d = frame_scale;
        f32 decayed_particle_scalar;
        kk2 = 8.0f / (d * 0.443f);
        pv->f16++;
        decayed_particle_scalar = particle->f0C - 260000.0f / (f32)kk2 * d * 1.25f;
        particle->f0C = decayed_particle_scalar;
        if (decayed_particle_scalar <= 0.0f) {
        kill:
            FUN_L00_00267a08_26ff10(particle);
            return;
        }
    } else {
        f32 d = frame_scale;
        kk2 = 8.0f / (d * 0.443f);
        particle->f0C = particle->f0C + 260000.0f / (f32)kk2 * d;
    }
    f = convert_integer_to_float(pv->f14);
    {
        f32 d = frame_scale;
        f32 kk;
        kk2 = 8.0f / (d * 0.443f);
        kk = (f32)kk2;
        alpha_factor = 0.08f / kk * d * (f / kk) + 0.08f;
        if (alpha_factor < 0.0f)
            alpha_factor = 0.0f;
        if (0.08f < alpha_factor)
            alpha_factor = 0.08f;
    }
    if (pv->f18 & 8) {
        alpha_factor = alpha_factor / ((f32)pv->f16 / 7.0f + 1.0f);
    }
    particle->f04 =
        (particle->f04 & 0xFFFFFF) | (truncate_float_to_s32(alpha_factor * 256.0f) << 24);
    pv->z = pv->z + frame_scale_sq * 0.008f;
    t = pv->f14;
    if (scale_game_frames(25) < t || (pv->f18 & 8)) {
        pv->x = pv->x * 0.95f;
        pv->y = pv->y * 0.95f;
    }
    fm = pv->f18;
    rotation_direction_sign = -1.0f;
    if (fm & 1)
        rotation_direction_sign = 1.0f;
    if (fm & 8) {
        pv->z = pv->z + frame_scale_sq * 0.008f;
        f = convert_integer_to_float(pv->f16);
        pv->t = pv->t + rotation_direction_sign * (frame_scale * -0.01f) / f;
    } else {
        pv->t = pv->t + rotation_direction_sign * (frame_scale * -0.01f);
    }
    if (1.0f < pv->t)
        pv->t = pv->t - 1.0f;
    if (pv->t < 0.0f)
        pv->t = pv->t + 1.0f;
    particle->f08 = (s32)(pv->t * 256.0f);
}

/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_002715B0), where it is exact; names translated to the US level program. */

/* Calls free_effect_particle on a once fast_dec_timer_s16(a + 0xA) returns non-zero; skips id -1. */
void tick_finite_effect_particle_lifetime(char *particle) __asm__("FUN_L00_00270710");

void tick_finite_effect_particle_lifetime(char *particle) {
    if (*(short *)(particle + 0xA) != -1) {
        if (fast_dec_timer_s16(particle + 0xA) != 0) {
            free_effect_particle(particle);
        }
    }
}
/* Spawns a particle of type 0x2A at a with velocity b, class c and tag d. */
/* Ported from rac1-decomp (src/overlays/shared/partupd_0026A130.c: func_L00_002715F8), where it is exact; names translated to the US level program. */

extern float          D_0015ED60_c2 __asm__("D_0015ED60");
extern int            FUN_L00_0025bfe0(float, float, float, float);
extern unsigned char *effect_texture_ids __asm__("D_L00_001B20B0") NOT_SDA;
extern void          *FUN_L00_002678b8_c2(int) __asm__("FUN_L00_002678b8");

void *spawn_splitting_effect_particle_with_collision_and_fade(
    char         *spawn_position,
    char         *initial_velocity,
    unsigned char initial_flags,
    int           collision_context) __asm__("FUN_L00_00270758");

void *spawn_splitting_effect_particle_with_collision_and_fade(char         *spawn_position,
                                                              char         *initial_velocity,
                                                              unsigned char initial_flags,

                                                              int collision_context) {
    char *particle = FUN_L00_002678b8_c2(0x2A);

    if (particle != 0) {
        char *particle_data = particle + 0x20;
        float initial_rotation_phase;
        int   rnd;

        qcopy(particle + 0x10, spawn_position);
        qcopy(particle_data, initial_velocity);
        *(int *)(particle + 4) = FUN_L00_0025bfe0(0.7f, 0.7f, 0.7f, 0.08f);
        particle[9] = (char)(truncate_float_to_s32(4.0f) + 0x40);
        particle[1] = 0;
        particle[3] = 0x44;
        initial_rotation_phase = random_float_between(0.0f, 1.0f);
        *(float *)(particle_data + 0x10) = initial_rotation_phase;
        particle[8] = (char)(int)(initial_rotation_phase * 256.0f);
        particle[2] = effect_texture_ids[rand() & 7];
        particle_data[0x18] = initial_flags;
        *(short *)(particle_data + 0x14) = 0;
        *(short *)(particle_data + 0x16) = 0;
        if (initial_flags < 5) {
            int base_lifetime;
            int y;

            *(float *)(particle + 0xC) = random_float_between(7000.0f, 10000.0f);
            rnd = rand();
            base_lifetime = (int)(8.0f / (D_0015ED60_c2 * 0.443f));
            y = base_lifetime * 6 / 7;
            *(short *)(particle + 0xA) = base_lifetime - rnd % y;
        } else {
            *(short *)(particle + 0xA) = (int)(8.0f / (D_0015ED60_c2 * 0.443f));
            *(float *)(particle + 0xC) = 10000.0f;
        }
        *(int *)(particle_data + 0x1C) = collision_context;
    }
    return particle;
}

typedef unsigned int Q_270948 __attribute__((mode(TI)));

typedef union {
    Q_270948 q;
    f32      f[4];
} V_270948;

typedef struct {
    V_270948 v;
    f32      x10;
    s16      h14;
    s16      h16;
    u8       b18;
    u8       pad[3];
    s32      x1C;
} E_270948;

typedef struct {
    u8       pad[2];
    u8       b2;
    u8       pad3;
    u32      x4;
    u8       b8;
    u8       pad9;
    s16      hA;
    f32      fC;
    V_270948 pos;
    E_270948 e;
} M_270948;

typedef struct {
    V_270948 v;
    s32      x10;
    s32      x14;
    u8       pad[4];
    f32      x1C;
    s32      x20;
} C_270948;

extern unsigned char *effect_texture_ids __asm__("D_L00_001B20B0") NOT_SDA;
extern s32            coll_line_fix(void *, void *, s32, s32, void *) __asm__("FUN_001efa68");

void update_splitting_effect_particle_with_collision_and_fade(M_270948 *particle) __asm__("FUN_L00_00270948");

void update_splitting_effect_particle_with_collision_and_fade(M_270948 *particle) {
    V_270948  v, position_scratch;
    C_270948  collision_parameters;
    E_270948 *e = &particle->e;
    M_270948 *spawned_particle;
    f32       position_jitter_extent, nr, alpha_factor, alpha_input_or_rotation_sign;
    s32       hit;
    u8        fl;

    if (particle->hA != 0 && fast_dec_timer_s16(&particle->hA)) {
        e->b18 |= 0x88;
        position_jitter_extent = particle->fC / 900000.0f;
        particle->fC *= 0.8f;
        nr = -position_jitter_extent;
        fast_vec_scale(&v, e, random_float_between(0.5f, 0.7f));
        position_scratch.f[0] =
            particle->pos.f[0] + random_float_between(nr, position_jitter_extent);
        position_scratch.f[1] =
            particle->pos.f[1] + random_float_between(nr, position_jitter_extent);
        position_scratch.f[2] =
            particle->pos.f[2] + random_float_between(nr, position_jitter_extent);
        v.f[0] += random_float_between(-e->v.f[0] * 0.2f, e->v.f[0] * 0.2f);
        v.f[1] += random_float_between(-e->v.f[1] * 0.2f, e->v.f[1] * 0.2f);
        v.f[2] += random_float_between(-e->v.f[2] * 0.2f, 0.0f);
        spawned_particle = spawn_splitting_effect_particle_with_collision_and_fade(
            &position_scratch, &v, 0x88, e->x1C);
        if (spawned_particle) {
            spawned_particle->hA = scale_game_frames(2);
            spawned_particle->fC = particle->fC;
        }
        fast_vec_scale(e, e, random_float_between(0.5f, 0.7f));
        e->v.f[0] += random_float_between(-e->v.f[0] * 0.2f, e->v.f[0] * 0.2f);
        e->v.f[1] += random_float_between(-e->v.f[1] * 0.2f, e->v.f[1] * 0.2f);
        e->v.f[2] += random_float_between(-e->v.f[2] * 0.2f, 0.0f);
        particle->pos.f[0] += random_float_between(nr, position_jitter_extent);
        particle->pos.f[1] += random_float_between(nr, position_jitter_extent);
        particle->pos.f[2] += random_float_between(nr, position_jitter_extent);
    }
    particle->b2 =
        effect_texture_ids[(particle->hA + e->h16 + (particle->b2 - effect_texture_ids[0])) & 7];
    fast_vec_add(&v, &particle->pos, e);
    hit = 0;
    if (e->h14 > scale_game_frames(3)) {
        fast_vec_sub(&position_scratch, &particle->pos, e);
    } else {
        position_scratch.q = particle->pos.q;
    }
    fast_vec_scale(&collision_parameters, &collision_parameters, 0.01f);
    fl = e->b18;
    if (fl & 8) {
        if (!(particle->x4 & 0xFF000000))
            goto kill;
        e->b18 = fl | 8;
        if (!random_integer_below(3))
            hit = coll_line_fix(&position_scratch, &v, 0, e->x1C, 0) != 0;
    } else {
        collision_parameters.x10 = e->x1C;
        collision_parameters.x1C = 1.0f;
        collision_parameters.v.q = e->v.q;
        collision_parameters.x14 = 1;
        collision_parameters.x20 = 1;
        if (!random_integer_below(3))
            hit = coll_line_fix(&position_scratch, &v, 0, e->x1C, &collision_parameters) != 0;
    }
    if (hit)
        e->b18 |= 8;
    else
        qcopy(&particle->pos, &v);
    if (e->b18 & 8) {
        e->h16++;
        particle->fC -= 260000.0f / (f32)(s32)(8.0f / (D_0015ED60 * 0.443f)) * D_0015ED60 * 1.25f;
        if (particle->fC <= 0.0f) {
        kill:
            free_effect_particle(particle);
            return;
        }
    } else {
        particle->fC += 260000.0f / (f32)(s32)(8.0f / (D_0015ED60 * 0.443f)) * D_0015ED60;
    }
    alpha_input_or_rotation_sign = convert_integer_to_float(e->h14);
    {
        f32 alpha_normalization_divisor = (f32)(s32)(8.0f / (D_0015ED60 * 0.443f));
        alpha_factor = 0.08f / alpha_normalization_divisor * D_0015ED60 *
                           (alpha_input_or_rotation_sign / alpha_normalization_divisor) +
                       0.08f;
    }
    if (alpha_factor < 0.0f)
        alpha_factor = 0.0f;
    if (0.08f < alpha_factor)
        alpha_factor = 0.08f;
    if (e->b18 & 8)
        alpha_factor /= (f32)e->h16 / 7.0f + 1.0f;
    particle->x4 = (particle->x4 & 0xFFFFFF) | (truncate_float_to_s32(alpha_factor * 256.0f) << 24);
    e->v.f[2] += frame_scale_sq * 0.008f;
    if (e->h14 > scale_game_frames(0x19) || (e->b18 & 8)) {
        e->v.f[0] *= 0.95f;
        e->v.f[1] *= 0.95f;
    }
    {
        u8 flags = e->b18;
        alpha_input_or_rotation_sign = -1.0f;
        if (flags & 1)
            alpha_input_or_rotation_sign = 1.0f;
        if (flags & 8) {
            e->v.f[2] += frame_scale_sq * 0.008f;
            e->x10 += alpha_input_or_rotation_sign * (D_0015ED60 * -0.01f) /
                      convert_integer_to_float(e->h16);
        } else {
            e->x10 += alpha_input_or_rotation_sign * (D_0015ED60 * -0.01f);
        }
    }
    if (1.0f < e->x10)
        e->x10 -= 1.0f;
    if (e->x10 < 0.0f)
        e->x10 += 1.0f;
    particle->b8 = (s32)(e->x10 * 256.0f);
}

void FUN_L00_00267a08_2710a0(void *) __asm__("FUN_L00_00267a08");
void update_accelerating_fade_in_out_effect_particle(u8 *particle) __asm__("FUN_L00_002710a0");

void update_accelerating_fade_in_out_effect_particle(u8 *particle) {
    u8 *particle_data = particle + 0x20;
    s16 elapsed_ticks;
    f32 fade_ratio, x;
    u8  r, g, b, faded_alpha;
    elapsed_ticks = ++*(s16 *)(particle + 0xA);
    if (*(s16 *)(particle_data + 0x18) < elapsed_ticks) {
        FUN_L00_00267a08_2710a0(particle);
        return;
    }
    fade_ratio = convert_integer_to_float(elapsed_ticks);
    fade_ratio = fade_ratio / convert_integer_to_float(*(s16 *)(particle_data + 0x18));
    fade_ratio = 1.0f - fade_ratio;
    r = truncate_float_to_s32(convert_integer_to_float(particle_data[0x14]) + fade_ratio * -0.001f);
    g = truncate_float_to_s32(convert_integer_to_float(particle_data[0x15]) +
                              fade_ratio * -0.0011f);
    b = truncate_float_to_s32(convert_integer_to_float(particle_data[0x16]) +
                              fade_ratio * -0.00111f);
    if (*(s16 *)(particle + 0xA) < scale_game_frames(0x1E)) {
        fade_ratio = convert_integer_to_float(*(s16 *)(particle + 0xA));
        fade_ratio = fade_ratio / convert_integer_to_float(scale_game_frames(0x1E));
        x = convert_integer_to_float(particle_data[0x17]);
        faded_alpha = truncate_float_to_s32(x * fade_ratio);
    } else {
        fade_ratio = convert_integer_to_float(*(s16 *)(particle + 0xA) - scale_game_frames(0x1E));
        fade_ratio = fade_ratio / convert_integer_to_float(*(s16 *)(particle_data + 0x18) -
                                                           scale_game_frames(0x1E));
        x = convert_integer_to_float(particle_data[0x17]);
        fade_ratio = 1.0f - fade_ratio;
        faded_alpha = truncate_float_to_s32(x * fade_ratio);
    }
    {
        u32 packed_color = faded_alpha << 24;
        packed_color |= b << 16;
        packed_color |= g << 8;
        packed_color |= r;
        *(u32 *)(particle + 4) = packed_color;
    }
    particle[8]++;
    fast_vec_add(particle + 0x10, particle + 0x10, particle_data);
    *(f32 *)(particle_data + 8) += frame_scale_sq * 0.0005f;
}

s32 D_L01_001B2548[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2554[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2558[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2560[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2564[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2568[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B256C[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2570[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2588[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2690[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26B0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26BC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26C8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26D4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26D8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26E0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26E4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26E8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26EC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26F0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B26F4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2708[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2190[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B21B0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B21BC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B21C8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B21CC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B21D4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B21D8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B21F0[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2210[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2514[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2530[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B253C[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2548[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2554[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2558[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2560[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2564[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2568[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B256C[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2570[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2588[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2810[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2814[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2830[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2834[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B283C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2848[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2854[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2858[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2860[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2864[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2868[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B286C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2870[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2888[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2110[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2134[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2154[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2158[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2188[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2890[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28B0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28B4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28BC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28C8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28D4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28D8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28E0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28E4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28E8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28EC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28F0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2908[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2210[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2514[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2530[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B253C[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2548[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2554[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2558[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2560[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2564[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2568[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B256C[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2570[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2588[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2AC8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2AD4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2AD8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2AE0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2AE4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2AE8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2AEC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2AF0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2B08[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2514[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2530[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B253C[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2548[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2554[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2558[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2560[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2564[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2568[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B256C[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2570[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2588[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23C8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23D4[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23D8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23E0[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23E4[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23E8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23EC[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23F0[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B23F4[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B2408[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2790[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2794[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B27BC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B27D4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B27D8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B27E0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B27E4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B27E8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B27EC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B27F0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2808[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2690[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26B0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26B4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26BC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26C8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26D4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26D8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26E0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26E4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26E8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26EC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26F0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26F4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2708[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2514[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2530[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B253C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2548[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2554[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2558[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2560[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2564[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2568[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B256C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2570[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2588[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2990[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29B0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29B4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29BC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29C8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29D4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29D8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29E0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29E4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29E8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29EC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29F0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2A08[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2A90[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AB0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2ABC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AC8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AD4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AD8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AE0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AE4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AE8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AEC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2AF0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2B08[1] __attribute__((section(".data"))) = {0};

unsigned char *D_L01_001B2510 NOT_SDA = 0;
int D_L01_001B2514            NOT_SDA = 0;
unsigned char *D_L01_001B2520 NOT_SDA = 0;
int                           D_L01_001601A4 = 0x258;
unsigned char *D_L01_001B252C NOT_SDA = 0;
unsigned char *D_L01_001B2530 NOT_SDA = 0;
int                           D_L01_001601A8 = 0x20202080;
int                           D_L01_001601AC = 0x20208080;
int                           D_L01_001601B0 = 0x1820c0c0;
int                           D_L01_001601B4 = 0x182080c0;
float                         D_L01_001601B8 = 0.25f;
int                           D_L01_001601C0 = 0x14;
int                           D_L01_001601C4 = 0x1e;
int                           D_L01_001601C8 = 0x8002040;
int                           D_L01_001601CC = 0x8004040;
int                           D_L01_001601D0 = 0x10004040;
int                           D_L01_001601D4 = 0x10002040;
float                         D_L01_001601D8 = 1.0f;
int                           D_L01_001601E0 = 0x19;
int                           D_L01_001601E4 = 0x23;
float                         D_L01_001601BC = 1.25f;
float                         D_L01_001601DC = 3.0f;
float                         D_L01_001601EC = 1.0f;
unsigned char *D_L01_001B2534 NOT_SDA = 0;
unsigned char *D_L01_001B253C NOT_SDA = 0;
unsigned char *D_L01_001B2540 NOT_SDA = 0;
u8 *D_L01_001B254C            NOT_SDA = 0;
u8 *D_L01_001B2574            NOT_SDA = 0;
float                         D_L01_001601F4 = 0.93f;
float                         D_L01_001601F8 = 0.929f;
unsigned char *D_L01_001B258C NOT_SDA = 0;
f32                           D_L01_00160200 = 1.4e+05f;
s32                           D_L01_00160204 = 0x20ffff20;
f32                           D_L01_00160208 = 0;
u8                            D_L01_0016020C = 1;
u8 *D_L01_001B259C            NOT_SDA = 0;
int D_L02_001B2694            NOT_SDA = 0;
unsigned char *D_L02_001B26A0 NOT_SDA = 0;
int                           D_L02_001601A4 = 0x258;
unsigned char *D_L02_001B26AC NOT_SDA = 0;
int                           D_L02_001601A8 = 0x20202080;
int                           D_L02_001601AC = 0x20208080;
int                           D_L02_001601B0 = 0x1820c0c0;
int                           D_L02_001601B4 = 0x182080c0;
float                         D_L02_001601B8 = 0.25f;
int                           D_L02_001601C0 = 0x14;
int                           D_L02_001601C4 = 0x1e;
int                           D_L02_001601C8 = 0x8002040;
int                           D_L02_001601CC = 0x8004040;
int                           D_L02_001601D0 = 0x10004040;
int                           D_L02_001601D4 = 0x10002040;
float                         D_L02_001601D8 = 1.0f;
int                           D_L02_001601E0 = 0x19;
int                           D_L02_001601E4 = 0x23;
float                         D_L02_001601BC = 1.25f;
float                         D_L02_001601DC = 3.0f;
float                         D_L02_001601EC = 1.0f;
unsigned char *D_L02_001B26B4 NOT_SDA = 0;
unsigned char *D_L02_001B26C0 NOT_SDA = 0;
u8 *D_L02_001B26CC            NOT_SDA = 0;
float                         D_L02_001601F4 = 0.93f;
float                         D_L02_001601F8 = 0.929f;
unsigned char *D_L02_001B270C NOT_SDA = 0;
f32                           D_L02_00160200 = 1.4e+05f;
s32                           D_L02_00160204 = 0x20ffff20;
f32                           D_L02_00160208 = 0;
u8                            D_L02_0016020C = 1;
u8 *D_L02_001B271C            NOT_SDA = 0;
int D_L03_001B2194            NOT_SDA = 0;
unsigned char *D_L03_001B21A0 NOT_SDA = 0;
int                           D_L03_001601A4 = 0x258;
unsigned char *D_L03_001B21AC NOT_SDA = 0;
int                           D_L03_001601A8 = 0x20202080;
int                           D_L03_001601AC = 0x20208080;
int                           D_L03_001601B0 = 0x1820c0c0;
int                           D_L03_001601B4 = 0x182080c0;
float                         D_L03_001601B8 = 0.25f;
int                           D_L03_001601C0 = 0x14;
int                           D_L03_001601C4 = 0x1e;
int                           D_L03_001601C8 = 0x8002040;
int                           D_L03_001601CC = 0x8004040;
int                           D_L03_001601D0 = 0x10004040;
int                           D_L03_001601D4 = 0x10002040;
float                         D_L03_001601D8 = 1.0f;
int                           D_L03_001601E0 = 0x19;
int                           D_L03_001601E4 = 0x23;
unsigned char *D_L03_001B21E0 NOT_SDA = 0;
float                         D_L03_001601BC = 1.25f;
float                         D_L03_001601DC = 3.0f;
float                         D_L03_001601EC = 1.0f;
unsigned char *D_L03_001B21B4 NOT_SDA = 0;
unsigned char *D_L03_001B21C0 NOT_SDA = 0;
unsigned char *D_L03_001B21E4 NOT_SDA = 0;
unsigned char *D_L03_001B21E8 NOT_SDA = 0;
u8 *D_L03_001B21F4            NOT_SDA = 0;
float                         D_L03_001601F4 = 0.93f;
float                         D_L03_001601F8 = 0.929f;
unsigned char *D_L03_001B2208 NOT_SDA = 0;
unsigned char *D_L03_001B220C NOT_SDA = 0;
f32                           D_L03_00160200 = 1.4e+05f;
s32                           D_L03_00160204 = 0x20ffff20;
f32                           D_L03_00160208 = 0;
u8                            D_L03_0016020C = 1;
u8 *D_L03_001B221C            NOT_SDA = 0;
int D_L04_001B2214            NOT_SDA = 0;
unsigned char *D_L04_001B2220 NOT_SDA = 0;
int                           D_L04_001601A4 = 0x258;
unsigned char *D_L04_001B222C NOT_SDA = 0;
unsigned char *D_L04_001B2230 NOT_SDA = 0;
int                           D_L04_001601A8 = 0x20202080;
int                           D_L04_001601AC = 0x20208080;
int                           D_L04_001601B0 = 0x1820c0c0;
int                           D_L04_001601B4 = 0x182080c0;
float                         D_L04_001601B8 = 0.25f;
int                           D_L04_001601C0 = 0x14;
int                           D_L04_001601C4 = 0x1e;
int                           D_L04_001601C8 = 0x8002040;
int                           D_L04_001601CC = 0x8004040;
int                           D_L04_001601D0 = 0x10004040;
int                           D_L04_001601D4 = 0x10002040;
float                         D_L04_001601D8 = 1.0f;
int                           D_L04_001601E0 = 0x19;
int                           D_L04_001601E4 = 0x23;
unsigned char *D_L04_001B2260 NOT_SDA = 0;
float                         D_L04_001601BC = 1.25f;
float                         D_L04_001601DC = 3.0f;
float                         D_L04_001601EC = 1.0f;
unsigned char *D_L04_001B2234 NOT_SDA = 0;
unsigned char *D_L04_001B223C NOT_SDA = 0;
unsigned char *D_L04_001B2240 NOT_SDA = 0;
unsigned char *D_L04_001B2248 NOT_SDA = 0;
u8 *D_L04_001B224C            NOT_SDA = 0;
unsigned char *D_L04_001B2254 NOT_SDA = 0;
unsigned char *D_L04_001B2258 NOT_SDA = 0;
unsigned char *D_L04_001B2264 NOT_SDA = 0;
unsigned char *D_L04_001B2268 NOT_SDA = 0;
u8 *D_L04_001B2270            NOT_SDA = 0;
u8 *D_L04_001B2274            NOT_SDA = 0;
float                         D_L04_001601F4 = 0.93f;
float                         D_L04_001601F8 = 0.929f;
unsigned char *D_L04_001B2288 NOT_SDA = 0;
unsigned char *D_L04_001B228C NOT_SDA = 0;
f32                           D_L04_00160200 = 1.4e+05f;
s32                           D_L04_00160204 = 0x20ffff20;
f32                           D_L04_00160208 = 0;
u8                            D_L04_0016020C = 1;
u8 *D_L04_001B229C            NOT_SDA = 0;
unsigned char *D_L05_001B2510 NOT_SDA = 0;
unsigned char *D_L05_001B2520 NOT_SDA = 0;
int                           D_L05_001601A4 = 0x258;
unsigned char *D_L05_001B252C NOT_SDA = 0;
int                           D_L05_001601A8 = 0x20202080;
int                           D_L05_001601AC = 0x20208080;
int                           D_L05_001601B0 = 0x1820c0c0;
int                           D_L05_001601B4 = 0x182080c0;
float                         D_L05_001601B8 = 0.25f;
int                           D_L05_001601C0 = 0x14;
int                           D_L05_001601C4 = 0x1e;
int                           D_L05_001601C8 = 0x8002040;
int                           D_L05_001601CC = 0x8004040;
int                           D_L05_001601D0 = 0x10004040;
int                           D_L05_001601D4 = 0x10002040;
float                         D_L05_001601D8 = 1.0f;
int                           D_L05_001601E0 = 0x19;
int                           D_L05_001601E4 = 0x23;
float                         D_L05_001601BC = 1.25f;
float                         D_L05_001601DC = 3.0f;
float                         D_L05_001601EC = 1.0f;
unsigned char *D_L05_001B2534 NOT_SDA = 0;
unsigned char *D_L05_001B2540 NOT_SDA = 0;
u8 *D_L05_001B254C            NOT_SDA = 0;
u8 *D_L05_001B2574            NOT_SDA = 0;
float                         D_L05_001601F4 = 0.93f;
float                         D_L05_001601F8 = 0.929f;
unsigned char *D_L05_001B258C NOT_SDA = 0;
f32                           D_L05_00160200 = 1.4e+05f;
s32                           D_L05_00160204 = 0x20ffff20;
f32                           D_L05_00160208 = 0;
u8                            D_L05_0016020C = 1;
u8 *D_L05_001B259C            NOT_SDA = 0;
unsigned char *D_L06_001B2820 NOT_SDA = 0;
int                           D_L06_001601A4 = 0x258;
unsigned char *D_L06_001B282C NOT_SDA = 0;
int                           D_L06_001601A8 = 0x20202080;
int                           D_L06_001601AC = 0x20208080;
int                           D_L06_001601B0 = 0x1820c0c0;
int                           D_L06_001601B4 = 0x182080c0;
float                         D_L06_001601B8 = 0.25f;
int                           D_L06_001601C0 = 0x14;
int                           D_L06_001601C4 = 0x1e;
int                           D_L06_001601C8 = 0x8002040;
int                           D_L06_001601CC = 0x8004040;
int                           D_L06_001601D0 = 0x10004040;
int                           D_L06_001601D4 = 0x10002040;
float                         D_L06_001601D8 = 1.0f;
int                           D_L06_001601E0 = 0x19;
int                           D_L06_001601E4 = 0x23;
float                         D_L06_001601BC = 1.25f;
float                         D_L06_001601DC = 3.0f;
float                         D_L06_001601EC = 1.0f;
unsigned char *D_L06_001B2840 NOT_SDA = 0;
u8 *D_L06_001B284C            NOT_SDA = 0;
u8 *D_L06_001B2874            NOT_SDA = 0;
float                         D_L06_001601F4 = 0.93f;
float                         D_L06_001601F8 = 0.929f;
unsigned char *D_L06_001B288C NOT_SDA = 0;
f32                           D_L06_00160200 = 1.4e+05f;
s32                           D_L06_00160204 = 0x20ffff20;
f32                           D_L06_00160208 = 0;
u8                            D_L06_0016020C = 1;
u8 *D_L06_001B289C            NOT_SDA = 0;
int D_L07_001B2114            NOT_SDA = 0;
unsigned char *D_L07_001B2120 NOT_SDA = 0;
int                           D_L07_001601A4 = 0x258;
unsigned char *D_L07_001B212C NOT_SDA = 0;
unsigned char *D_L07_001B2130 NOT_SDA = 0;
int                           D_L07_001601A8 = 0x20202080;
int                           D_L07_001601AC = 0x20208080;
int                           D_L07_001601B0 = 0x1820c0c0;
int                           D_L07_001601B4 = 0x182080c0;
float                         D_L07_001601B8 = 0.25f;
int                           D_L07_001601C0 = 0x14;
int                           D_L07_001601C4 = 0x1e;
int                           D_L07_001601C8 = 0x8002040;
int                           D_L07_001601CC = 0x8004040;
int                           D_L07_001601D0 = 0x10004040;
int                           D_L07_001601D4 = 0x10002040;
float                         D_L07_001601D8 = 1.0f;
int                           D_L07_001601E0 = 0x19;
int                           D_L07_001601E4 = 0x23;
unsigned char *D_L07_001B2160 NOT_SDA = 0;
float                         D_L07_001601BC = 1.25f;
float                         D_L07_001601DC = 3.0f;
float                         D_L07_001601EC = 1.0f;
unsigned char *D_L07_001B213C NOT_SDA = 0;
unsigned char *D_L07_001B2140 NOT_SDA = 0;
unsigned char *D_L07_001B2148 NOT_SDA = 0;
u8 *D_L07_001B214C            NOT_SDA = 0;
unsigned char *D_L07_001B2168 NOT_SDA = 0;
u8 *D_L07_001B2170            NOT_SDA = 0;
u8 *D_L07_001B2174            NOT_SDA = 0;
float                         D_L07_001601F4 = 0.93f;
float                         D_L07_001601F8 = 0.929f;
unsigned char *D_L07_001B218C NOT_SDA = 0;
f32                           D_L07_00160200 = 1.4e+05f;
s32                           D_L07_00160204 = 0x20ffff20;
f32                           D_L07_00160208 = 0;
u8                            D_L07_0016020C = 1;
u8 *D_L07_001B219C            NOT_SDA = 0;
int D_L08_001B2894            NOT_SDA = 0;
unsigned char *D_L08_001B28A0 NOT_SDA = 0;
int                           D_L08_001601A4 = 0x258;
unsigned char *D_L08_001B28AC NOT_SDA = 0;
int                           D_L08_001601A8 = 0x20202080;
int                           D_L08_001601AC = 0x20208080;
int                           D_L08_001601B0 = 0x1820c0c0;
int                           D_L08_001601B4 = 0x182080c0;
float                         D_L08_001601B8 = 0.25f;
int                           D_L08_001601C0 = 0x14;
int                           D_L08_001601C4 = 0x1e;
int                           D_L08_001601C8 = 0x8002040;
int                           D_L08_001601CC = 0x8004040;
int                           D_L08_001601D0 = 0x10004040;
int                           D_L08_001601D4 = 0x10002040;
float                         D_L08_001601D8 = 1.0f;
int                           D_L08_001601E0 = 0x19;
int                           D_L08_001601E4 = 0x23;
float                         D_L08_001601BC = 1.25f;
float                         D_L08_001601DC = 3.0f;
float                         D_L08_001601EC = 1.0f;
unsigned char *D_L08_001B28C0 NOT_SDA = 0;
u8 *D_L08_001B28CC            NOT_SDA = 0;
u8 *D_L08_001B28F4            NOT_SDA = 0;
float                         D_L08_001601F4 = 0.93f;
float                         D_L08_001601F8 = 0.929f;
unsigned char *D_L08_001B290C NOT_SDA = 0;
u8                            D_L08_0016020C = 1;
int D_L09_001B2214            NOT_SDA = 0;
unsigned char *D_L09_001B2220 NOT_SDA = 0;
int                           D_L09_001601A4 = 0x258;
unsigned char *D_L09_001B222C NOT_SDA = 0;
unsigned char *D_L09_001B2230 NOT_SDA = 0;
int                           D_L09_001601A8 = 0x20202080;
int                           D_L09_001601AC = 0x20208080;
int                           D_L09_001601B0 = 0x1820c0c0;
int                           D_L09_001601B4 = 0x182080c0;
float                         D_L09_001601B8 = 0.25f;
int                           D_L09_001601C0 = 0x14;
int                           D_L09_001601C4 = 0x1e;
int                           D_L09_001601C8 = 0x8002040;
int                           D_L09_001601CC = 0x8004040;
int                           D_L09_001601D0 = 0x10004040;
int                           D_L09_001601D4 = 0x10002040;
float                         D_L09_001601D8 = 1.0f;
int                           D_L09_001601E0 = 0x19;
int                           D_L09_001601E4 = 0x23;
unsigned char *D_L09_001B2260 NOT_SDA = 0;
float                         D_L09_001601BC = 1.25f;
float                         D_L09_001601DC = 3.0f;
float                         D_L09_001601EC = 1.0f;
unsigned char *D_L09_001B2234 NOT_SDA = 0;
unsigned char *D_L09_001B223C NOT_SDA = 0;
unsigned char *D_L09_001B2240 NOT_SDA = 0;
unsigned char *D_L09_001B2248 NOT_SDA = 0;
u8 *D_L09_001B224C            NOT_SDA = 0;
unsigned char *D_L09_001B2254 NOT_SDA = 0;
unsigned char *D_L09_001B2258 NOT_SDA = 0;
unsigned char *D_L09_001B2264 NOT_SDA = 0;
unsigned char *D_L09_001B2268 NOT_SDA = 0;
u8 *D_L09_001B2270            NOT_SDA = 0;
u8 *D_L09_001B2274            NOT_SDA = 0;
float                         D_L09_001601F4 = 0.93f;
float                         D_L09_001601F8 = 0.929f;
unsigned char *D_L09_001B2288 NOT_SDA = 0;
unsigned char *D_L09_001B228C NOT_SDA = 0;
f32                           D_L09_00160200 = 1.4e+05f;
s32                           D_L09_00160204 = 0x20ffff20;
f32                           D_L09_00160208 = 0;
u8                            D_L09_0016020C = 1;
u8 *D_L09_001B229C            NOT_SDA = 0;
unsigned char *D_L10_001B2510 NOT_SDA = 0;
unsigned char *D_L10_001B2520 NOT_SDA = 0;
int                           D_L10_001601A4 = 0x258;
unsigned char *D_L10_001B252C NOT_SDA = 0;
int                           D_L10_001601A8 = 0x20202080;
int                           D_L10_001601AC = 0x20208080;
int                           D_L10_001601B0 = 0x1820c0c0;
int                           D_L10_001601B4 = 0x182080c0;
float                         D_L10_001601B8 = 0.25f;
int                           D_L10_001601C0 = 0x14;
int                           D_L10_001601C4 = 0x1e;
int                           D_L10_001601C8 = 0x8002040;
int                           D_L10_001601CC = 0x8004040;
int                           D_L10_001601D0 = 0x10004040;
int                           D_L10_001601D4 = 0x10002040;
float                         D_L10_001601D8 = 1.0f;
int                           D_L10_001601E0 = 0x19;
int                           D_L10_001601E4 = 0x23;
float                         D_L10_001601BC = 1.25f;
float                         D_L10_001601DC = 3.0f;
float                         D_L10_001601EC = 1.0f;
unsigned char *D_L10_001B2534 NOT_SDA = 0;
unsigned char *D_L10_001B2540 NOT_SDA = 0;
u8 *D_L10_001B254C            NOT_SDA = 0;
u8 *D_L10_001B2574            NOT_SDA = 0;
float                         D_L10_001601F4 = 0.93f;
float                         D_L10_001601F8 = 0.929f;
unsigned char *D_L10_001B258C NOT_SDA = 0;
f32                           D_L10_00160200 = 1.4e+05f;
s32                           D_L10_00160204 = 0x20ffff20;
f32                           D_L10_00160208 = 0;
u8                            D_L10_0016020C = 1;
u8 *D_L10_001B259C            NOT_SDA = 0;
unsigned char *D_L11_001B2A90 NOT_SDA = 0;
int D_L11_001B2A94            NOT_SDA = 0;
unsigned char *D_L11_001B2AA0 NOT_SDA = 0;
int                           D_L11_001601A4 = 0x258;
unsigned char *D_L11_001B2AAC NOT_SDA = 0;
unsigned char *D_L11_001B2AB0 NOT_SDA = 0;
int                           D_L11_001601A8 = 0x20202080;
int                           D_L11_001601AC = 0x20208080;
int                           D_L11_001601B0 = 0x1820c0c0;
int                           D_L11_001601B4 = 0x182080c0;
float                         D_L11_001601B8 = 0.25f;
int                           D_L11_001601C0 = 0x14;
int                           D_L11_001601C4 = 0x1e;
int                           D_L11_001601C8 = 0x8002040;
int                           D_L11_001601CC = 0x8004040;
int                           D_L11_001601D0 = 0x10004040;
int                           D_L11_001601D4 = 0x10002040;
float                         D_L11_001601D8 = 1.0f;
int                           D_L11_001601E0 = 0x19;
int                           D_L11_001601E4 = 0x23;
float                         D_L11_001601BC = 1.25f;
float                         D_L11_001601DC = 3.0f;
float                         D_L11_001601EC = 1.0f;
unsigned char *D_L11_001B2AB4 NOT_SDA = 0;
unsigned char *D_L11_001B2ABC NOT_SDA = 0;
unsigned char *D_L11_001B2AC0 NOT_SDA = 0;
u8 *D_L11_001B2ACC            NOT_SDA = 0;
u8 *D_L11_001B2AF4            NOT_SDA = 0;
float                         D_L11_001601F4 = 0.93f;
float                         D_L11_001601F8 = 0.929f;
unsigned char *D_L11_001B2B0C NOT_SDA = 0;
f32                           D_L11_00160200 = 1.4e+05f;
s32                           D_L11_00160204 = 0x20ffff20;
f32                           D_L11_00160208 = 0;
u8                            D_L11_0016020C = 1;
u8 *D_L11_001B2B1C            NOT_SDA = 0;
unsigned char *D_L12_001B2510 NOT_SDA = 0;
unsigned char *D_L12_001B2520 NOT_SDA = 0;
int                           D_L12_001601A4 = 0x258;
unsigned char *D_L12_001B252C NOT_SDA = 0;
int                           D_L12_001601A8 = 0x20202080;
int                           D_L12_001601AC = 0x20208080;
int                           D_L12_001601B0 = 0x1820c0c0;
int                           D_L12_001601B4 = 0x182080c0;
float                         D_L12_001601B8 = 0.25f;
int                           D_L12_001601C0 = 0x14;
int                           D_L12_001601C4 = 0x1e;
int                           D_L12_001601C8 = 0x8002040;
int                           D_L12_001601CC = 0x8004040;
int                           D_L12_001601D0 = 0x10004040;
int                           D_L12_001601D4 = 0x10002040;
float                         D_L12_001601D8 = 1.0f;
int                           D_L12_001601E0 = 0x19;
int                           D_L12_001601E4 = 0x23;
float                         D_L12_001601BC = 1.25f;
float                         D_L12_001601DC = 3.0f;
float                         D_L12_001601EC = 1.0f;
unsigned char *D_L12_001B2534 NOT_SDA = 0;
unsigned char *D_L12_001B2540 NOT_SDA = 0;
u8 *D_L12_001B254C            NOT_SDA = 0;
u8 *D_L12_001B2574            NOT_SDA = 0;
float                         D_L12_001601F4 = 0.93f;
float                         D_L12_001601F8 = 0.929f;
unsigned char *D_L12_001B258C NOT_SDA = 0;
f32                           D_L12_00160200 = 1.4e+05f;
s32                           D_L12_00160204 = 0x20ffff20;
f32                           D_L12_00160208 = 0;
u8                            D_L12_0016020C = 1;
u8 *D_L12_001B259C            NOT_SDA = 0;
unsigned char *D_L13_001B2390 NOT_SDA = 0;
int D_L13_001B2394            NOT_SDA = 0;
unsigned char *D_L13_001B23A0 NOT_SDA = 0;
int                           D_L13_001601A4 = 0x258;
unsigned char *D_L13_001B23AC NOT_SDA = 0;
unsigned char *D_L13_001B23B0 NOT_SDA = 0;
int                           D_L13_001601A8 = 0x20202080;
int                           D_L13_001601AC = 0x20208080;
int                           D_L13_001601B0 = 0x1820c0c0;
int                           D_L13_001601B4 = 0x182080c0;
float                         D_L13_001601B8 = 0.25f;
int                           D_L13_001601C0 = 0x14;
int                           D_L13_001601C4 = 0x1e;
int                           D_L13_001601C8 = 0x8002040;
int                           D_L13_001601CC = 0x8004040;
int                           D_L13_001601D0 = 0x10004040;
int                           D_L13_001601D4 = 0x10002040;
float                         D_L13_001601D8 = 1.0f;
int                           D_L13_001601E0 = 0x19;
int                           D_L13_001601E4 = 0x23;
float                         D_L13_001601BC = 1.25f;
float                         D_L13_001601DC = 3.0f;
float                         D_L13_001601EC = 1.0f;
unsigned char *D_L13_001B23B4 NOT_SDA = 0;
unsigned char *D_L13_001B23BC NOT_SDA = 0;
unsigned char *D_L13_001B23C0 NOT_SDA = 0;
u8 *D_L13_001B23CC            NOT_SDA = 0;
float                         D_L13_001601F4 = 0.93f;
float                         D_L13_001601F8 = 0.929f;
unsigned char *D_L13_001B240C NOT_SDA = 0;
f32                           D_L13_00160200 = 1.4e+05f;
s32                           D_L13_00160204 = 0x20ffff20;
f32                           D_L13_00160208 = 0;
u8                            D_L13_0016020C = 1;
u8 *D_L13_001B241C            NOT_SDA = 0;
unsigned char *D_L14_001B27A0 NOT_SDA = 0;
int                           D_L14_001601A4 = 0x258;
unsigned char *D_L14_001B27AC NOT_SDA = 0;
unsigned char *D_L14_001B27B0 NOT_SDA = 0;
int                           D_L14_001601A8 = 0x20202080;
int                           D_L14_001601AC = 0x20208080;
int                           D_L14_001601B0 = 0x1820c0c0;
int                           D_L14_001601B4 = 0x182080c0;
float                         D_L14_001601B8 = 0.25f;
int                           D_L14_001601C0 = 0x14;
int                           D_L14_001601C4 = 0x1e;
int                           D_L14_001601C8 = 0x8002040;
int                           D_L14_001601CC = 0x8004040;
int                           D_L14_001601D0 = 0x10004040;
int                           D_L14_001601D4 = 0x10002040;
float                         D_L14_001601D8 = 1.0f;
int                           D_L14_001601E0 = 0x19;
int                           D_L14_001601E4 = 0x23;
float                         D_L14_001601BC = 1.25f;
float                         D_L14_001601DC = 3.0f;
float                         D_L14_001601EC = 1.0f;
unsigned char *D_L14_001B27B4 NOT_SDA = 0;
unsigned char *D_L14_001B27C0 NOT_SDA = 0;
unsigned char *D_L14_001B27C8 NOT_SDA = 0;
u8 *D_L14_001B27CC            NOT_SDA = 0;
u8 *D_L14_001B27F4            NOT_SDA = 0;
float                         D_L14_001601F4 = 0.93f;
float                         D_L14_001601F8 = 0.929f;
unsigned char *D_L14_001B280C NOT_SDA = 0;
f32                           D_L14_00160200 = 1.4e+05f;
s32                           D_L14_00160204 = 0x20ffff20;
f32                           D_L14_00160208 = 0;
u8                            D_L14_0016020C = 1;
u8 *D_L14_001B281C            NOT_SDA = 0;
int D_L15_001B2694            NOT_SDA = 0;
unsigned char *D_L15_001B26A0 NOT_SDA = 0;
int                           D_L15_001601A4 = 0x258;
int                           D_L15_001601A8 = 0x20202080;
int                           D_L15_001601AC = 0x20208080;
int                           D_L15_001601B0 = 0x1820c0c0;
int                           D_L15_001601B4 = 0x182080c0;
float                         D_L15_001601B8 = 0.25f;
int                           D_L15_001601C0 = 0x14;
int                           D_L15_001601C4 = 0x1e;
int                           D_L15_001601C8 = 0x8002040;
int                           D_L15_001601CC = 0x8004040;
int                           D_L15_001601D0 = 0x10004040;
int                           D_L15_001601D4 = 0x10002040;
float                         D_L15_001601D8 = 1.0f;
int                           D_L15_001601E0 = 0x19;
int                           D_L15_001601E4 = 0x23;
float                         D_L15_001601BC = 1.25f;
float                         D_L15_001601DC = 3.0f;
float                         D_L15_001601EC = 1.0f;
unsigned char *D_L15_001B26C0 NOT_SDA = 0;
u8 *D_L15_001B26CC            NOT_SDA = 0;
float                         D_L15_001601F4 = 0.93f;
float                         D_L15_001601F8 = 0.929f;
unsigned char *D_L15_001B270C NOT_SDA = 0;
f32                           D_L15_00160200 = 1.4e+05f;
s32                           D_L15_00160204 = 0x20ffff20;
f32                           D_L15_00160208 = 0;
u8                            D_L15_0016020C = 1;
u8 *D_L15_001B271C            NOT_SDA = 0;
unsigned char *D_L16_001B2510 NOT_SDA = 0;
unsigned char *D_L16_001B2520 NOT_SDA = 0;
int                           D_L16_001601A4 = 0x258;
unsigned char *D_L16_001B252C NOT_SDA = 0;
int                           D_L16_001601A8 = 0x20202080;
int                           D_L16_001601B0 = 0x1820c0c0;
int                           D_L16_001601B4 = 0x182080c0;
float                         D_L16_001601B8 = 0.25f;
int                           D_L16_001601C0 = 0x14;
int                           D_L16_001601C4 = 0x1e;
int                           D_L16_001601C8 = 0x8002040;
int                           D_L16_001601CC = 0x8004040;
int                           D_L16_001601D0 = 0x10004040;
int                           D_L16_001601D4 = 0x10002040;
float                         D_L16_001601D8 = 1.0f;
int                           D_L16_001601E0 = 0x19;
int                           D_L16_001601E4 = 0x23;
float                         D_L16_001601BC = 1.25f;
float                         D_L16_001601DC = 3.0f;
float                         D_L16_001601EC = 1.0f;
unsigned char *D_L16_001B2534 NOT_SDA = 0;
unsigned char *D_L16_001B2540 NOT_SDA = 0;
u8 *D_L16_001B254C            NOT_SDA = 0;
u8 *D_L16_001B2574            NOT_SDA = 0;
float                         D_L16_001601F4 = 0.93f;
float                         D_L16_001601F8 = 0.929f;
unsigned char *D_L16_001B258C NOT_SDA = 0;
f32                           D_L16_00160200 = 1.4e+05f;
s32                           D_L16_00160204 = 0x20ffff20;
f32                           D_L16_00160208 = 0;
u8                            D_L16_0016020C = 1;
u8 *D_L16_001B259C            NOT_SDA = 0;
int D_L17_001B2994            NOT_SDA = 0;
unsigned char *D_L17_001B29A0 NOT_SDA = 0;
int                           D_L17_001601A4 = 0x258;
unsigned char *D_L17_001B29AC NOT_SDA = 0;
int                           D_L17_001601A8 = 0x20202080;
int                           D_L17_001601AC = 0x20208080;
int                           D_L17_001601B0 = 0x1820c0c0;
int                           D_L17_001601B4 = 0x182080c0;
float                         D_L17_001601B8 = 0.25f;
int                           D_L17_001601C0 = 0x14;
int                           D_L17_001601C4 = 0x1e;
int                           D_L17_001601C8 = 0x8002040;
int                           D_L17_001601CC = 0x8004040;
int                           D_L17_001601D0 = 0x10004040;
int                           D_L17_001601D4 = 0x10002040;
float                         D_L17_001601D8 = 1.0f;
int                           D_L17_001601E0 = 0x19;
int                           D_L17_001601E4 = 0x23;
float                         D_L17_001601BC = 1.25f;
float                         D_L17_001601DC = 3.0f;
float                         D_L17_001601EC = 1.0f;
unsigned char *D_L17_001B29C0 NOT_SDA = 0;
u8 *D_L17_001B29CC            NOT_SDA = 0;
u8 *D_L17_001B29F4            NOT_SDA = 0;
float                         D_L17_001601F4 = 0.93f;
float                         D_L17_001601F8 = 0.929f;
unsigned char *D_L17_001B2A0C NOT_SDA = 0;
f32                           D_L17_00160200 = 1.4e+05f;
s32                           D_L17_00160204 = 0x20ffff20;
f32                           D_L17_00160208 = 0;
u8                            D_L17_0016020C = 1;
u8 *D_L17_001B2A1C            NOT_SDA = 0;
int D_L18_001B2A94            NOT_SDA = 0;
unsigned char *D_L18_001B2AA0 NOT_SDA = 0;
int                           D_L18_001601A4 = 0x258;
int                           D_L18_001601A8 = 0x20202080;
int                           D_L18_001601AC = 0x20208080;
int                           D_L18_001601B0 = 0x1820c0c0;
int                           D_L18_001601B4 = 0x182080c0;
float                         D_L18_001601B8 = 0.25f;
int                           D_L18_001601C0 = 0x14;
int                           D_L18_001601C4 = 0x1e;
int                           D_L18_001601C8 = 0x8002040;
int                           D_L18_001601CC = 0x8004040;
int                           D_L18_001601D0 = 0x10004040;
int                           D_L18_001601D4 = 0x10002040;
float                         D_L18_001601D8 = 1.0f;
int                           D_L18_001601E0 = 0x19;
int                           D_L18_001601E4 = 0x23;
float                         D_L18_001601BC = 1.25f;
float                         D_L18_001601DC = 3.0f;
float                         D_L18_001601EC = 1.0f;
unsigned char *D_L18_001B2AB4 NOT_SDA = 0;
unsigned char *D_L18_001B2AC0 NOT_SDA = 0;
u8 *D_L18_001B2ACC            NOT_SDA = 0;
u8 *D_L18_001B2AF4            NOT_SDA = 0;
float                         D_L18_001601F4 = 0.93f;
float                         D_L18_001601F8 = 0.929f;
unsigned char *D_L18_001B2B0C NOT_SDA = 0;
f32                           D_L18_00160200 = 1.4e+05f;
s32                           D_L18_00160204 = 0x20ffff20;
f32                           D_L18_00160208 = 0;
u8                            D_L18_0016020C = 1;
u8 *D_L18_001B2B1C            NOT_SDA = 0;

unsigned char *D_L00_001B2090 NOT_SDA = 0;

int D_L00_001B2094 NOT_SDA = 0;

int spinning_particle_default_initial_color_blend_target_gp __asm__("D_L00_001601AC") = 0x20208080;

int spinning_particle_default_final_color_blend_target_gp __asm__("D_L00_001601B4") = 0x182080c0;

int spinning_particle_alternate_initial_color_blend_target_gp __asm__("D_L00_001601CC") = 0x8004040;

int spinning_particle_alternate_final_color_blend_target_gp __asm__("D_L00_001601D4") = 0x10002040;

unsigned char *D_L00_001B20E0 NOT_SDA = 0;

unsigned char *D_L00_001B20B4 NOT_SDA = 0;

unsigned char *D_L00_001B20BC NOT_SDA = 0;

unsigned char *D_L00_001B20C0 NOT_SDA = 0;

unsigned char *D_L00_001B20C8 NOT_SDA = 0;

u8 *D_L00_001B20CC NOT_SDA = 0;

unsigned char *D_L00_001B20D4 NOT_SDA = 0;

unsigned char *D_L00_001B20D8 NOT_SDA = 0;

unsigned char *D_L00_001B20E4 NOT_SDA = 0;

unsigned char *D_L00_001B20E8 NOT_SDA = 0;

u8 *D_L00_001B20F0 NOT_SDA = 0;

u8 *D_L00_001B20F4_lui __asm__("D_L00_001B20F4") NOT_SDA = 0;

unsigned char *D_L00_001B2108 NOT_SDA = 0;

unsigned char *D_L00_001B210C NOT_SDA = 0;

u8 *D_L00_001B211C NOT_SDA = 0;
