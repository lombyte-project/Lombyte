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
    E_288ec0 *e;
    f32 x, y;
    s32 t, u, c;
    D_L00_0016051C_288ec0->n = randomized_count + orbiting_count;
    for (i = 0; i < D_L00_0016051C_288ec0->n; i++) {
        e = &D_L00_0016051C_288ec0->list[i];
        if (i >= randomized_count) {
            s16 *h = &e->hC;
            e->b0 = 0;
            e->hC = rand() >> 16;
            h[1] = rand() >> 16;
            e->b2 = 1;
            e->b3 = 0x48;
            e->f1C = 0.16f;
        } else {
            e->b0 = 1;
            e->hC = random_integer_below(0x100);
            e->b2 = (rand() >> 16) & 1;
            e->b3 = 0x48;
            e->f8 = random_angle_radians();
            e->f1C = convert_integer_to_float(random_integer_below(0x30) + 0x20) * 0.00390625f;
            x = random_angle_radians();
            y = random_angle_radians();
            e->f10 = fast_cos(x) * fast_sin(y) * 50.0f;
            e->f14 = fast_sin(x) * fast_sin(y) * 50.0f;
            e->f18 = fast_cos(y) * fast_cos(y) * 36.0f + 16.0f;
            t = random_integer_below(0x18);
            u = random_integer_below(0x20) << 24;
            if ((rand() >> 16) & 1) {
                c = t << 16;
                c += 0x30505050;
                c = u - -c;
            } else {
                c = t << 8;
                c += 0x30505050;
                c = u - -c;
                c |= t;
            }
            *(u32 *)&e->hC = c;
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
        E_289108 *e = &D_L00_0016051C_289108->list[i];
        if (e->b0 == 0) {
            unsigned short *h = &e->hC;
            float a, b;
            e->hC++;
            h[1]++;
            a = convert_integer_to_float((e->hC & 0xFFF) - 0x800) * 0.0015339808f;
            b = convert_integer_to_float((h[1] & 0xFFF) - 0x800) * 0.0015339808f;
            e->f10 = fast_cos(a) * fast_sin(b) * 50.0f;
            e->f14 = fast_sin(a) * fast_sin(b) * 50.0f;
            e->f18 = absolute_float(fast_cos(b)) * 50.0f;
            if ((e->hC & 0x3F) < 8) {
                e->w4 = 0x702020F0;
            } else {
                e->w4 = 0x202020F0;
            }
        } else if (e->b0 == 1) {
            int r = rand() >> 16;
            int t = ((r & 0x1F00) << 10) - 0x202020;
            int c = *(int *)&e->hC;
            c += t;
            c += (r & 0x1F0) << 6;
            c += (r & 0x1F) << 2;
            e->w4 = c;
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
void FUN_L00_002895a0(void) {
    int i, j;
    SkyParticle *s;
    f32 a, b;
    int color_channel_offset, color_high_byte_offset, r;
    SKY->s4 = 0;
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    sky_draw_shell(1);
    if (SKY->count == 0) {
        SKY->count = 0x100;
        for (i = 0; i < SKY->count; i++) {
            s = &SKY->stars[i];
            if (i >= 0xF6) {
                u16 *h;
                s->type = 0;
                h = &s->c.h.a;
                h[0] = rand() >> 16;
                h[1] = rand() >> 16;
                s->b2 = 3;
                s->b3 = 0x48;
                s->f1C = 0.16f;
            } else {
                s->type = 3;
                s->c.h.a = random_integer_below(0x100);
                {
                    int q = rand() >> 16;
                    s->b3 = 0x48;
                    s->b2 = (q & 1) + 2;
                }
                s->f8 = random_angle_radians();
                s->f1C = convert_integer_to_float(random_integer_below(0x30) + 0x20) * (1.0f / 256.0f);
                a = random_angle_radians();
                b = random_angle_radians();
                s->x = fast_cos(a) * fast_sin(b) * 50.0f;
                s->y = fast_sin(a) * fast_sin(b) * 50.0f;
                s->z = fast_cos(b) * fast_cos(b) * 36.0f + 16.0f;
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
                    s->c.w = w;
                }
            }
        }
    }
    for (j = 0; j < SKY->count; j++) {
        s = &SKY->stars[j];
        if (s->type == 0) {
            u16 *h = &s->c.h.a;
            h[0]++;
            h[1]++;
            a = convert_integer_to_float((s->c.h.a & 0xFFF) - 0x800) * 0.0015339808f;
            b = convert_integer_to_float((h[1] & 0xFFF) - 0x800) * 0.0015339808f;
            s->x = fast_cos(a) * fast_sin(b) * 50.0f;
            s->y = fast_sin(a) * fast_sin(b) * 50.0f;
            s->z = absolute_float(fast_cos(b)) * 50.0f;
            if ((s->c.h.a & 0x3F) < 8)
                s->color = 0x702020F0;
            else
                s->color = 0x202020F0;
        } else if (s->type == 3) {
            r = rand() >> 16;
            {
                u32 u = ((r & 0x1F00) << 10) + 0xFFDFDFE0;
                u32 c = s->c.w + u;
                c += (r & 0x1F0) << 6;
                c += (r & 0x1F) << 2;
                s->color = c;
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
void FUN_L00_00289cb8(void) {
    Part *p;
    Part *p2;
    PU *q2;
    s32 i2;
    s32 k2;
    PU *q;
    s32 i;
    s32 k;
    s32 j;
    s32 r;
    s32 r1;
    s32 r2;
    f32 a;
    f32 b;

    D_L00_0016051C_c3->h4 = 0;
    set_identity_matrix(D_L00_001BD7E0_lui);
    sky_draw_shell(0);
    if (D_L00_0016051C_c3->count == 0) {
        D_L00_0016051C_c3->count = 0x100;
        for (i = 0; i < D_L00_0016051C_c3->count; i++) {
            p = &D_L00_0016051C_c3->parts[i];
            if (i >= 0xF0) {
                p->type = 0;
                q = &p->u;
                q->h.h0 = rand() >> 16;
                q->h.h2 = rand() >> 16;
                p->b3 = 0x48;
                p->b2 = 1;
                p->f1C = 0.18f;
                p->col = D_L00_00160500_gp[(rand() >> 16) & 3];
            } else if (i >= 0xEA) {
                r = i - 0xEA;
                p->b2 = 2;
                k = r >> 1;
                p->b3 = 0x48;
                j = r & 1;
                p->f8 = random_angle_radians();
                p->type = 2;
                p->col = D_L00_001604F0[k];
                qcopy(&p->f10, D_L00_001BD7B0 + k * 16);
                p->u.c.c0 = k;
                {
                    PU *qq = &p->u;
                    qq->c.c1 = j;
                    qq->c.c2 = j * 0x18;
                }
            } else {
                p->type = 3;
                p->u.h.h0 = random_integer_below(0x100);
                p->b2 = (rand() >> 16) & 1;
                p->b3 = 0x48;
                p->f8 = random_angle_radians();
                p->f1C = convert_integer_to_float(random_integer_below(0x30) + 0x20) * 0.00390625f;
                a = random_angle_radians();
                b = random_angle_radians();
                p->f10 = fast_cos(a) * fast_sin(b) * 50.0f;
                p->f14 = fast_sin(a) * fast_sin(b) * 50.0f;
                p->f18 = fast_cos(b) * 50.0f;
                r1 = random_integer_below(0x18) + 8;
                r2 = random_integer_below(0x20) << 24;
                if ((rand() >> 16) & 1) {
                    {
                        u32 t = (r1 << 16) + 0x30505050;
                        p->u.w = r2 + t;
                    }
                } else {
                    {
                        u32 t = (r1 << 8) + 0x30505050;
                        p->u.w = (r2 + t) | r1;
                    }
                }
            }
        }
    }
    for (i2 = 0; i2 < D_L00_0016051C_c3->count; i2++) {
        p2 = &D_L00_0016051C_c3->parts[i2];
        if (p2->type == 0) {
            q2 = &p2->u;
            p2->u.h.h0++;
            q2->h.h2++;
            a = convert_integer_to_float((p2->u.h.h0 & 0xFFF) - 0x800) * 0.0015339808f;
            b = convert_integer_to_float((q2->h.h2 & 0xFFF) - 0x800) * 0.0015339808f;
            p2->f10 = fast_cos(a) * fast_sin(b) * 50.0f;
            p2->f14 = fast_sin(a) * fast_sin(b) * 50.0f;
            p2->f18 = fast_cos(b) * 50.0f;
            p2->col &= 0xFFFFFF;
            if (((u16)p2->u.h.h0 & 0x3F) < 8) {
                p2->col |= 0x70000000;
            } else {
                p2->col |= 0x24000000;
            }
        } else if (p2->type == 2) {
            q2 = &p2->u;
            q2->c.c2 = (q2->c.c2 + 1) % 0x30;
            if (q2->c.c2 == 0) {
                p2->f8 = convert_integer_to_float((q2->c.c1 << 5) + ((rand() >> 16) & 0xF) - 8) *
                         0.024543693f;
            }
            k2 = q2->c.c2 * 4;
            if (k2 > 0x60)
                k2 = 0xC0 - k2;
            {
                s32 rr = rand() >> 16;
                u32 x;
                x = D_L00_001604F0[p2->u.c.c0];
                x += (rr & 0xF00) << 8;
                x += (rr & 0xF0) << 4;
                x += rr & 0xF;
                x += k2 << 24;
                p2->col = x;
            }
        } else if (p2->type == 3) {
            s32 rs = rand() >> 16;
            u32 x;
            {
                u32 y = ((rs & 0x1F00) << 10) + 0xFFDFDFE0;
                x = p2->u.w;
                x += y;
            }
            x += (rs & 0x1F0) << 6;
            x += (rs & 0x1F) << 2;
            p2->col = x;
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
void FUN_L00_0028ac88(void) {
    int i, j;
    SkyParticle *s;
    f32 a, b;
    int color_channel_offset, color_high_byte_offset, r;
    SKY->s4 = 0;
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    if (SKY->count == 0) {
        SKY->count = 0x100;
        for (i = 0; i < SKY->count; i++) {
            s = &SKY->stars[i];
            if (i >= 0xF0) {
                u16 *h;
                s->type = 0;
                h = &s->c.h.a;
                h[0] = rand() >> 16;
                h[1] = rand() >> 16;
                s->b2 = 1;
                s->b3 = 0x48;
                s->f1C = 0.18f;
                s->color = D_L00_00160500[(rand() >> 16) & 3];
            } else {
                s->type = 1;
                s->c.h.a = random_integer_below(0x100);
                {
                    int q = rand() >> 16;
                    s->b3 = 0x48;
                    s->b2 = q & 1;
                }
                s->f8 = random_angle_radians();
                s->f1C = convert_integer_to_float(random_integer_below(0x30) + 0x20) * (1.0f / 256.0f);
                a = random_angle_radians();
                b = random_angle_radians();
                s->x = fast_cos(a) * fast_sin(b) * 50.0f;
                s->y = fast_sin(a) * fast_sin(b) * 50.0f;
                s->z = fast_cos(b) * 50.0f;
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
                    s->c.w = w;
                }
            }
        }
    }
    for (j = 0; j < SKY->count; j++) {
        s = &SKY->stars[j];
        if (s->type == 0) {
            u16 *h = &s->c.h.a;
            h[0]++;
            h[1]++;
            a = convert_integer_to_float((s->c.h.a & 0xFFF) - 0x800) * 0.0015339808f;
            b = convert_integer_to_float((h[1] & 0xFFF) - 0x800) * 0.0015339808f;
            s->x = fast_cos(a) * fast_sin(b) * 50.0f;
            s->y = fast_sin(a) * fast_sin(b) * 50.0f;
            s->z = fast_cos(b) * 50.0f;
            s->color &= 0xFFFFFF;
            if ((s->c.h.a & 0x3F) < 8)
                s->color |= 0x70000000;
            else
                s->color |= 0x24000000;
        } else {
            r = rand() >> 16;
            {
                u32 u = ((r & 0x1F00) << 10) + 0xFFDFDFE0;
                u32 c = s->c.w + u;
                c += (r & 0x1F0) << 6;
                c += (r & 0x1F) << 2;
                s->color = c;
            }
        }
    }
    sky_sprite_proc();
    vu1_add_g_sregister(0x42, 0x8000000044L);
    sky_draw_shell(1);
    sky_draw_shell(2);
}
#undef SKY
