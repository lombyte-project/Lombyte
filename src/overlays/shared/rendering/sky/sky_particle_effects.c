/* Sky particle initialization, animation and particle-bearing render programs.
 * Keep the signed initialization and unsigned update views distinct: their
 * halfword accesses are part of the matching code. */
#include "types.h"
#include "asm.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/rendering/sky_shell.h"
#include "rnc/rendering/sky_particles.h"

s32 random_integer_below(s32 limit) __asm__("FUN_00213260");
f32 random_angle_radians(void) __asm__("FUN_00213308");
void vu1_add_g_sregister(s32 register_id, s64 value) __asm__("FUN_00233980");
s32 rand(void) __asm__("FUN_001160d8");
void set_identity_matrix(void *matrix) __asm__("FUN_001f9fc8");
void sky_sprite_proc(void) __asm__("FUN_0022bba0");

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    s32 w4;
    f32 f8;
    s16 hC;
    s16 hE;
    f32 f10, f14, f18;
    f32 f1C;
} E_288ec0;
typedef struct {
    u8 p0[8];
    s16 n;
    u8 pa[0x1C - 0xA];
    E_288ec0 *list;
} G_288ec0;
extern G_288ec0 *D_L00_0016051C_288ec0 __asm__("D_L00_0016051C");
void sky_particles_init(s32 randomized_count, s32 orbiting_count) __asm__("FUN_L00_00288ec0");
void sky_particles_init(s32 randomized_count, s32 orbiting_count) {
    s32 i;
    E_288ec0 *particle;
    f32 x, y;
    s32 color_channel_offset, color_high_byte_offset, packed_color;
    D_L00_0016051C_288ec0->n = randomized_count + orbiting_count;
    for (i = 0; i < D_L00_0016051C_288ec0->n; i++) {
        particle = &D_L00_0016051C_288ec0->list[i];
        if (i >= randomized_count) {
            s16 *phase_halves = &particle->hC;
            particle->b0 = 0;
            particle->hC = rand() >> 16;
            phase_halves[1] = rand() >> 16;
            particle->b2 = 1;
            particle->b3 = 0x48;
            particle->f1C = 0.16f;
        } else {
            particle->b0 = 1;
            particle->hC = random_integer_below(0x100);
            particle->b2 = (rand() >> 16) & 1;
            particle->b3 = 0x48;
            particle->f8 = random_angle_radians();
            particle->f1C = convert_integer_to_float(random_integer_below(0x30) + 0x20) * 0.00390625f;
            x = random_angle_radians();
            y = random_angle_radians();
            particle->f10 = fast_cos(x) * fast_sin(y) * 50.0f;
            particle->f14 = fast_sin(x) * fast_sin(y) * 50.0f;
            particle->f18 = fast_cos(y) * fast_cos(y) * 36.0f + 16.0f;
            color_channel_offset = random_integer_below(0x18);
            color_high_byte_offset = random_integer_below(0x20) << 24;
            if ((rand() >> 16) & 1) {
                packed_color = color_channel_offset << 16;
                packed_color += 0x30505050;
                packed_color = color_high_byte_offset - -packed_color;
            } else {
                packed_color = color_channel_offset << 8;
                packed_color += 0x30505050;
                packed_color = color_high_byte_offset - -packed_color;
                packed_color |= color_channel_offset;
            }
            *(u32 *)&particle->hC = packed_color;
        }
    }
}
typedef struct {
    unsigned char b0;
    unsigned char p1[3];
    int w4;
    int p8;
    unsigned short hC;
    unsigned short hE;
    float f10, f14, f18;
    int p1C;
} E_289108;
typedef struct {
    unsigned char p0[8];
    short n;
    unsigned char pa[0x1C - 0xA];
    E_289108 *list;
} G_289108;
extern G_289108 *D_L00_0016051C_289108 __asm__("D_L00_0016051C");
void sky_particles_update(void) __asm__("FUN_L00_00289108");
void sky_particles_update(void) {
    int i;
    for (i = 0; i < D_L00_0016051C_289108->n; i++) {
        E_289108 *particle = &D_L00_0016051C_289108->list[i];
        if (particle->b0 == 0) {
            unsigned short *phase_halves = &particle->hC;
            float azimuth_radians, polar_angle_radians;
            particle->hC++;
            phase_halves[1]++;
            azimuth_radians = convert_integer_to_float((particle->hC & 0xFFF) - 0x800) * 0.0015339808f;
            polar_angle_radians = convert_integer_to_float((phase_halves[1] & 0xFFF) - 0x800) * 0.0015339808f;
            particle->f10 = fast_cos(azimuth_radians) * fast_sin(polar_angle_radians) * 50.0f;
            particle->f14 = fast_sin(azimuth_radians) * fast_sin(polar_angle_radians) * 50.0f;
            particle->f18 = absolute_float(fast_cos(polar_angle_radians)) * 50.0f;
            if ((particle->hC & 0x3F) < 8) {
                particle->w4 = 0x702020F0;
            } else {
                particle->w4 = 0x202020F0;
            }
        } else if (particle->b0 == 1) {
            int random_color_bits = rand() >> 16;
            int packed_color_offset = ((random_color_bits & 0x1F00) << 10) - 0x202020;
            int packed_color = *(int *)&particle->hC;
            packed_color += packed_color_offset;
            packed_color += (random_color_bits & 0x1F0) << 6;
            packed_color += (random_color_bits & 0x1F) << 2;
            particle->w4 = packed_color;
        }
    }
}
/* Preserve the matrix declaration attributes inherited by these programs. */
extern char D_L00_001BD7E0[];
extern char D_L00_001BD7E0[] NOT_SDA;

/* Shared layout for the two programs using unsigned phase halves. */
typedef struct {
    u8 type;
    u8 b1;
    u8 b2;
    u8 b3;
    u32 color;
    f32 f8;
    union {
        struct {
            u16 a, b;
        } h;
        u32 w;
    } c;
    f32 x;
    f32 y;
    f32 z;
    f32 f1C;
} SkyParticle;
typedef struct SkyParticleSystem {
    u8 pad0[4];
    s16 s4;
    u8 pad6[2];
    s16 count;
    u8 padA[0x12];
    SkyParticle *stars;
} Sky2895a0;
extern Sky2895a0 *D_L00_0016051C_2895a0 __asm__("D_L00_0016051C");
extern char D_L00_001BD7E0[] __asm__("D_L00_001BD7E0");
#define SKY D_L00_0016051C_2895a0
/* Render four sky layers with an intervening particle pass. */
void draw_four_sky_shells_with_animated_particles(void) __asm__("FUN_L00_002895a0");

void draw_four_sky_shells_with_animated_particles(void) {
    int i, j;
    SkyParticle *particle;
    f32 a, polar_angle_radians;
    int color_channel_offset, color_high_byte_offset, random_color_bits;
    SKY->s4 = 0;
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    sky_draw_shell(1);
    if (SKY->count == 0) {
        SKY->count = 0x100;
        for (i = 0; i < SKY->count; i++) {
            particle = &SKY->stars[i];
            if (i >= 0xF6) {
                u16 *h;
                particle->type = 0;
                h = &particle->c.h.a;
                h[0] = rand() >> 16;
                h[1] = rand() >> 16;
                particle->b2 = 3;
                particle->b3 = 0x48;
                particle->f1C = 0.16f;
            } else {
                particle->type = 3;
                particle->c.h.a = random_integer_below(0x100);
                {
                    int random_high_half = rand() >> 16;
                    particle->b3 = 0x48;
                    particle->b2 = (random_high_half & 1) + 2;
                }
                particle->f8 = random_angle_radians();
                particle->f1C = convert_integer_to_float(random_integer_below(0x30) + 0x20) * (1.0f / 256.0f);
                a = random_angle_radians();
                polar_angle_radians = random_angle_radians();
                particle->x = fast_cos(a) * fast_sin(polar_angle_radians) * 50.0f;
                particle->y = fast_sin(a) * fast_sin(polar_angle_radians) * 50.0f;
                particle->z = fast_cos(polar_angle_radians) * fast_cos(polar_angle_radians) * 36.0f + 16.0f;
                color_channel_offset = random_integer_below(0x18);
                color_high_byte_offset = random_integer_below(0x20) << 24;
                {
                    u32 u, w;
                    if ((rand() >> 16) & 1) {
                        u = (color_channel_offset << 16) + 0x30505050;
                        w = color_high_byte_offset + u;
                    } else {
                        u = (color_channel_offset << 8) + 0x30505050;
                        w = color_high_byte_offset + u;
                        w |= color_channel_offset;
                    }
                    particle->c.w = w;
                }
            }
        }
    }
    for (j = 0; j < SKY->count; j++) {
        particle = &SKY->stars[j];
        if (particle->type == 0) {
            u16 *h = &particle->c.h.a;
            h[0]++;
            h[1]++;
            a = convert_integer_to_float((particle->c.h.a & 0xFFF) - 0x800) * 0.0015339808f;
            polar_angle_radians = convert_integer_to_float((h[1] & 0xFFF) - 0x800) * 0.0015339808f;
            particle->x = fast_cos(a) * fast_sin(polar_angle_radians) * 50.0f;
            particle->y = fast_sin(a) * fast_sin(polar_angle_radians) * 50.0f;
            particle->z = absolute_float(fast_cos(polar_angle_radians)) * 50.0f;
            if ((particle->c.h.a & 0x3F) < 8)
                particle->color = 0x702020F0;
            else
                particle->color = 0x202020F0;
        } else if (particle->type == 3) {
            random_color_bits = rand() >> 16;
            {
                u32 u = ((random_color_bits & 0x1F00) << 10) + 0xFFDFDFE0;
                u32 c = particle->c.w + u;
                c += (random_color_bits & 0x1F0) << 6;
                c += (random_color_bits & 0x1F) << 2;
                particle->color = c;
            }
        }
    }
    sky_sprite_proc();
    vu1_add_g_sregister(0x42, 0x8000000044L);
    sky_draw_shell(2);
    sky_draw_shell(3);
}
#undef SKY

/* This program has signed phases and a byte-wise state view. Its qcopy
 * writes all 16 bytes starting at f10, including f1C. */

typedef union {
    struct {
        s16 h0;
        s16 h2;
    } h;
    struct {
        u8 c0;
        u8 c1;
        u8 c2;
        u8 c3;
    } c;
    u32 w;
} PU;

typedef struct {
    u8 type;
    u8 b1;
    u8 b2;
    u8 b3;
    u32 col;
    f32 f8;
    PU u;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
} Part;

typedef struct {
    u8 pad0[4];
    s16 h4;
    u8 pad6[2];
    s16 count;
    u8 padA[0x1C - 0xA];
    Part *parts;
} Sys;

extern Sys *D_L00_0016051C_c3 __asm__("D_L00_0016051C");
extern u32 D_L00_00160500_gp[1] __asm__("D_L00_00160500") __attribute__((sda));
extern u32 D_L00_001604F0[1] __attribute__((sda));
extern char D_L00_001BD7E0_lui[] __asm__("D_L00_001BD7E0") __attribute__((section(".data")));
extern u8 D_L00_001BD7B0[] __attribute__((section(".data")));


/* Render particles with phase, byte-state and randomized-color variants. */
void draw_three_sky_shells_with_three_particle_types(void) __asm__("FUN_L00_00289cb8");

void draw_three_sky_shells_with_three_particle_types(void) {
    Part *init_particle;
    Part *update_particle;
    PU *update_state;
    s32 i2;
    s32 k2;
    PU *init_phases;
    s32 i;
    s32 k;
    s32 j;
    s32 r;
    s32 color_channel_offset;
    s32 color_high_byte_offset;
    f32 azimuth_radians;
    f32 polar_angle_radians;

    D_L00_0016051C_c3->h4 = 0;
    set_identity_matrix(D_L00_001BD7E0_lui);
    sky_draw_shell(0);
    if (D_L00_0016051C_c3->count == 0) {
        D_L00_0016051C_c3->count = 0x100;
        for (i = 0; i < D_L00_0016051C_c3->count; i++) {
            init_particle = &D_L00_0016051C_c3->parts[i];
            if (i >= 0xF0) {
                init_particle->type = 0;
                init_phases = &init_particle->u;
                init_phases->h.h0 = rand() >> 16;
                init_phases->h.h2 = rand() >> 16;
                init_particle->b3 = 0x48;
                init_particle->b2 = 1;
                init_particle->f1C = 0.18f;
                init_particle->col = D_L00_00160500_gp[(rand() >> 16) & 3];
            } else if (i >= 0xEA) {
                r = i - 0xEA;
                init_particle->b2 = 2;
                k = r >> 1;
                init_particle->b3 = 0x48;
                j = r & 1;
                init_particle->f8 = random_angle_radians();
                init_particle->type = 2;
                init_particle->col = D_L00_001604F0[k];
                qcopy(&init_particle->f10, D_L00_001BD7B0 + k * 16);
                init_particle->u.c.c0 = k;
                {
                    PU *qq = &init_particle->u;
                    qq->c.c1 = j;
                    qq->c.c2 = j * 0x18;
                }
            } else {
                init_particle->type = 3;
                init_particle->u.h.h0 = random_integer_below(0x100);
                init_particle->b2 = (rand() >> 16) & 1;
                init_particle->b3 = 0x48;
                init_particle->f8 = random_angle_radians();
                init_particle->f1C = convert_integer_to_float(random_integer_below(0x30) + 0x20) * 0.00390625f;
                azimuth_radians = random_angle_radians();
                polar_angle_radians = random_angle_radians();
                init_particle->f10 = fast_cos(azimuth_radians) * fast_sin(polar_angle_radians) * 50.0f;
                init_particle->f14 = fast_sin(azimuth_radians) * fast_sin(polar_angle_radians) * 50.0f;
                init_particle->f18 = fast_cos(polar_angle_radians) * 50.0f;
                color_channel_offset = random_integer_below(0x18) + 8;
                color_high_byte_offset = random_integer_below(0x20) << 24;
                if ((rand() >> 16) & 1) {
                    {
                        u32 t = (color_channel_offset << 16) + 0x30505050;
                        init_particle->u.w = color_high_byte_offset + t;
                    }
                } else {
                    {
                        u32 t = (color_channel_offset << 8) + 0x30505050;
                        init_particle->u.w = (color_high_byte_offset + t) | color_channel_offset;
                    }
                }
            }
        }
    }
    for (i2 = 0; i2 < D_L00_0016051C_c3->count; i2++) {
        update_particle = &D_L00_0016051C_c3->parts[i2];
        if (update_particle->type == 0) {
            update_state = &update_particle->u;
            update_particle->u.h.h0++;
            update_state->h.h2++;
            azimuth_radians = convert_integer_to_float((update_particle->u.h.h0 & 0xFFF) - 0x800) * 0.0015339808f;
            polar_angle_radians = convert_integer_to_float((update_state->h.h2 & 0xFFF) - 0x800) * 0.0015339808f;
            update_particle->f10 = fast_cos(azimuth_radians) * fast_sin(polar_angle_radians) * 50.0f;
            update_particle->f14 = fast_sin(azimuth_radians) * fast_sin(polar_angle_radians) * 50.0f;
            update_particle->f18 = fast_cos(polar_angle_radians) * 50.0f;
            update_particle->col &= 0xFFFFFF;
            if (((u16)update_particle->u.h.h0 & 0x3F) < 8) {
                update_particle->col |= 0x70000000;
            } else {
                update_particle->col |= 0x24000000;
            }
        } else if (update_particle->type == 2) {
            update_state = &update_particle->u;
            update_state->c.c2 = (update_state->c.c2 + 1) % 0x30;
            if (update_state->c.c2 == 0) {
                update_particle->f8 = convert_integer_to_float((update_state->c.c1 << 5) + ((rand() >> 16) & 0xF) - 8) *
                         0.024543693f;
            }
            k2 = update_state->c.c2 * 4;
            if (k2 > 0x60)
                k2 = 0xC0 - k2;
            {
                s32 rr = rand() >> 16;
                u32 x;
                x = D_L00_001604F0[update_particle->u.c.c0];
                x += (rr & 0xF00) << 8;
                x += (rr & 0xF0) << 4;
                x += rr & 0xF;
                x += k2 << 24;
                update_particle->col = x;
            }
        } else if (update_particle->type == 3) {
            s32 rs = rand() >> 16;
            u32 x;
            {
                u32 y = ((rs & 0x1F00) << 10) + 0xFFDFDFE0;
                x = update_particle->u.w;
                x += y;
            }
            x += (rs & 0x1F0) << 6;
            x += (rs & 0x1F) << 2;
            update_particle->col = x;
        }
    }
    sky_sprite_proc();
    vu1_add_g_sregister(0x42, 0x8000000044LL);
    sky_draw_shell(1);
    sky_draw_shell(2);
}

/* Second render program using the shared unsigned-phase layout. */
extern Sky2895a0 *D_L00_0016051C_c __asm__("D_L00_0016051C");
extern u32 D_L00_00160500[2] __attribute__((sda));
#define SKY D_L00_0016051C_c
/* Render three sky layers with an intervening particle pass. */
void render_three_shell_particle_sky(void) __asm__("FUN_L00_0028ac88");

void render_three_shell_particle_sky(void) {
    int i, j;
    SkyParticle *particle;
    f32 a, polar_angle_radians;
    int color_channel_offset, color_high_byte_offset, random_color_bits;
    SKY->s4 = 0;
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    if (SKY->count == 0) {
        SKY->count = 0x100;
        for (i = 0; i < SKY->count; i++) {
            particle = &SKY->stars[i];
            if (i >= 0xF0) {
                u16 *h;
                particle->type = 0;
                h = &particle->c.h.a;
                h[0] = rand() >> 16;
                h[1] = rand() >> 16;
                particle->b2 = 1;
                particle->b3 = 0x48;
                particle->f1C = 0.18f;
                particle->color = D_L00_00160500[(rand() >> 16) & 3];
            } else {
                particle->type = 1;
                particle->c.h.a = random_integer_below(0x100);
                {
                    int random_high_half = rand() >> 16;
                    particle->b3 = 0x48;
                    particle->b2 = random_high_half & 1;
                }
                particle->f8 = random_angle_radians();
                particle->f1C = convert_integer_to_float(random_integer_below(0x30) + 0x20) * (1.0f / 256.0f);
                a = random_angle_radians();
                polar_angle_radians = random_angle_radians();
                particle->x = fast_cos(a) * fast_sin(polar_angle_radians) * 50.0f;
                particle->y = fast_sin(a) * fast_sin(polar_angle_radians) * 50.0f;
                particle->z = fast_cos(polar_angle_radians) * 50.0f;
                color_channel_offset = random_integer_below(0x18) + 8;
                color_high_byte_offset = random_integer_below(0x20) << 24;
                {
                    u32 u, w;
                    if ((rand() >> 16) & 1) {
                        u = (color_channel_offset << 16) + 0x30505050;
                        w = color_high_byte_offset + u;
                    } else {
                        u = (color_channel_offset << 8) + 0x30505050;
                        w = color_high_byte_offset + u;
                        w |= color_channel_offset;
                    }
                    particle->c.w = w;
                }
            }
        }
    }
    for (j = 0; j < SKY->count; j++) {
        particle = &SKY->stars[j];
        if (particle->type == 0) {
            u16 *h = &particle->c.h.a;
            h[0]++;
            h[1]++;
            a = convert_integer_to_float((particle->c.h.a & 0xFFF) - 0x800) * 0.0015339808f;
            polar_angle_radians = convert_integer_to_float((h[1] & 0xFFF) - 0x800) * 0.0015339808f;
            particle->x = fast_cos(a) * fast_sin(polar_angle_radians) * 50.0f;
            particle->y = fast_sin(a) * fast_sin(polar_angle_radians) * 50.0f;
            particle->z = fast_cos(polar_angle_radians) * 50.0f;
            particle->color &= 0xFFFFFF;
            if ((particle->c.h.a & 0x3F) < 8)
                particle->color |= 0x70000000;
            else
                particle->color |= 0x24000000;
        } else {
            random_color_bits = rand() >> 16;
            {
                u32 u = ((random_color_bits & 0x1F00) << 10) + 0xFFDFDFE0;
                u32 c = particle->c.w + u;
                c += (random_color_bits & 0x1F0) << 6;
                c += (random_color_bits & 0x1F) << 2;
                particle->color = c;
            }
        }
    }
    sky_sprite_proc();
    vu1_add_g_sregister(0x42, 0x8000000044L);
    sky_draw_shell(1);
    sky_draw_shell(2);
}
#undef SKY
