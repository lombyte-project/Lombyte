/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/hud.h"

#define NOT_SDA

#define MACRO_ADDR

/* The level's HUD bank reset, a shorter func_001FF6B8 (src/game/hud.c):
   for each of the 13 bank records at D_L00_0017DD50, func_001FFB38(i,
   0xFFFF, 0, 0, 0, 0, 1), +0x7C = 0, +0x6C = -6, then reloads it with
   func_001FFC48. */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236208), where it is exact; names translated to the US level program. */

typedef struct {
    int unk00, unk04;
    char pad08[0x18];
    int unk20, unk24;
    char pad28[0x3C];
    int unk64, unk68, unk6C;
    char pad70[0xC];
    int unk7C;
    char pad80[0x10];
} HudBank;

extern HudBank hud_elements[] __asm__("D_L00_0017DC50");
s32 queue_animation_update(s32 chan, s32 id, s32 fn, s32 d, s32 e, s32 c,
                           s32 b) __asm__("FUN_001ff308");
void apply_pending_animation(struct Anim *anim) __asm__("FUN_001ff418");

void FUN_L00_00235878(void) {
    int i;

    for (i = 0; i < 13; i++) {
        queue_animation_update(i, 0xFFFF, 0, 0, 0, 0, 1);
        hud_elements[i].unk7C = 0;
        hud_elements[i].unk6C = -6;
        apply_pending_animation(&hud_elements[i]);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236400), where it is exact; names translated to the US level program. */

/* Offsets x/y for the element's anchor flags (centre or right/bottom alignment). */
int FUN_L00_00235a70(HudElem *rec, int *x, int *y) {
    int w = rec->w;
    int h = rec->h;
    int flags = rec->flags;

    if ((flags ^ 1) & 1) {
        if (!(flags & 2)) {
            *y -= h >> 1;
        }
    }
    if (!(rec->flags & 4)) {
        if (rec->flags & 8) {
            *x -= w;
        } else {
            *x -= w >> 1;
        }
    }
    return 0;
}
/* Slides a HUD element in or out from its anchored edge: step T moves by
   D (back while the timer +0x7C runs, forward otherwise), clamped to
   0..23, picks a ramp value from D_L00_0017E4A0 (timer running) or
   D_L00_0017E500, and offsets *Y by it times (height + 52) for flags 1
   (up) / 2 (down), or *X by it times (width + 20) for flags 4 (left) /
   8 (right), rounded (func_001FA888 / func_001FA898 convert). */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236468), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_L00_0017E3A0[];
extern float D_L00_0017E400[];
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");

void FUN_L00_00235ad8(HudElem *e, int *x, int *y, int t, int d) {
    int dx = 0;
    int dy = 0;
    float s;

    if (e->unk7C != 0) {
        t -= d;
    } else {
        t += d;
    }
    if (t < 0) {
        t = 0;
    }
    if (t > 23) {
        t = 23;
    }
    if (e->unk7C != 0) {
        s = D_L00_0017E3A0[t];
    } else {
        s = D_L00_0017E400[t];
    }
    if (e->flags & 1) {
        dy = -truncate_float_to_s32(s * (convert_integer_to_float(e->h) + 52.0f) + 0.5f);
    } else if (e->flags & 2) {
        dy = truncate_float_to_s32(s * (convert_integer_to_float(e->h) + 52.0f) + 0.5f);
    } else if (e->flags & 4) {
        dx = -truncate_float_to_s32(s * (convert_integer_to_float(e->w) + 20.0f) + 0.5f);
    } else if (e->flags & 8) {
        dx = truncate_float_to_s32(s * (convert_integer_to_float(e->w) + 20.0f) + 0.5f);
    }
    *x += dx;
    *y += dy;
}
#include "sda.h"

/* updates numeric HUD values and dimensions from digit count */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236610), where it is exact; names translated to the US level program. */

void FUN_L00_00235c80(HudElem *e) {
    int *value = e->unk0C;
    int digits;
    int v, flags, width;
    if (value && !((int)value & 3)) {
        e->unk78 = *value;
        if (e->unk08 < e->unk78)
            e->unk78 = e->unk08;
        e->unk74 = e->unk78;
    } else {
        e->unk78 = 99999;
        e->unk74 = 99999;
    }
    v = e->unk08;
    digits = 0;
    flags = e->flags;
    width = e->h;
    while (v >= 10) {
        v /= 10;
        digits++;
    }
    if (!(flags & 3) && (flags & 12)) {
        e->h = width + (digits + 1) * 12;
        if (e->w < 14)
            e->w = 14;
    } else {
        if (width < 12)
            e->h = 12;
        e->w += (digits + 1) * 14;
    }
}
/* Resets a HUD element: a timer of 30 + func_001F9850(180) (+0x7C),
   offsets 0x48 / 0x4A cleared, then func_L00_00236610. */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236710), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00235c80(HudElem *);

/* Sets unk7C to scale_game_frames(0xB4) + 0x1E, clears 0x48/0x4A, calls FUN_L00_00235c80. */
void FUN_L00_00235d80(HudElem *e) {
    e->unk7C = scale_game_frames(0xB4) + 0x1E;
    e->unk48 = 0;
    e->unk4A = 0;
    FUN_L00_00235c80(e);
}
/* func_L00_00236710 for a 32 x 32 element. */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236750), where it is exact; names translated to the US level program. */

/* Sets unk7C to scale_game_frames(0xB4) + 0x1E, clears 0x48/0x4A, w/h 0x20, calls 00235c80. */
void FUN_L00_00235dc0(HudElem *e) {
    e->unk7C = scale_game_frames(0xB4) + 0x1E;
    e->unk48 = 0;
    e->unk4A = 0;
    e->w = 0x20;
    e->h = 0x20;
    FUN_L00_00235c80(e);
}
typedef struct {
    u8 pad[0x64];
    s32 key;
    s32 f68;
    u8 pad2[0x7C - 0x6C];
    s32 v;
    u8 pad3[0x90 - 0x80];
} T_235e18;
extern T_235e18 D_L00_0017DC50_235e18[] __asm__("D_L00_0017DC50") __attribute__((section(".data")));
s32 hud_keep_visible(s32 key, s32 val) __asm__("FUN_L00_00235e18");
s32 hud_keep_visible(s32 key, s32 val) {
    s32 i;
    s32 r;
    T_235e18 *e;
    r = 1;
    for (i = 0; i < 13; i++) {
        if (D_L00_0017DC50_235e18[i].key == key)
            break;
    }
    if (i < 13 && D_L00_0017DC50_235e18[i].f68 == 0) {
        e = D_L00_0017DC50_235e18 + i;
        e->v = (val < e->v) ? e->v : val;
        return 0;
    }
    return 1;
}
extern float FUN_001fa6c0(int);
extern int FUN_001fa6d0(float);
extern float square_root(float) __asm__("FUN_001f9988");
extern void FUN_L00_0023aaa8(void *);

void FUN_L00_00235ea0(e) HudElem *e;
{
    unsigned char *c = e->cnt;
    int v;
    int cap;
    int d;
    int s;

    if (e->unk0C != 0) {
        v = *e->unk0C;
        cap = e->unk08;
        if (v < 0) {
            v = 0;
        }
        e->unk78 = v;
        if (cap < v) {
            e->unk78 = cap;
        }
    }
    if (e->unk74 != e->unk78) {
        e->unk7C = scale_game_frames(0xB4);
        if (e->unk6C >= 0x18) {
            int t = e->unk74 - e->unk78;
            d = t < 0 ? -t : t;
            if (d != 0) {
                s = FUN_001fa6d0(square_root(FUN_001fa6c0(d) / 25.0f) * 5.0f);
                s = s < d * scale_game_frames(2) / scale_game_frames(10)
                        ? d * scale_game_frames(2) / scale_game_frames(10)
                        : s;
                if (s >= 0x7A) {
                    s = 0x79;
                } else if (s <= 0) {
                    s = 1;
                }
                c[3] += s;
                if (c[3] > scale_game_frames(2)) {
                    s = c[3] / scale_game_frames(2);
                    if (e->unk78 < e->unk74) {
                        e->unk74 = e->unk74 - s;
                    } else {
                        e->unk74 = e->unk74 + s;
                    }
                    c[3] -= s * scale_game_frames(2);
                }
            }
        }
    }
    if (e->unk7C >= scale_game_frames(5)) {
        if (c[0] < scale_game_frames(8)) {
            c[0]++;
        } else if (c[1] < scale_game_frames(8)) {
            c[1]++;
        }
    } else {
        if (c[1] != 0 || c[0] != 0) {
            e->unk6C = 1;
        }
        if (c[1] != 0) {
            c[1]--;
        } else if (c[0] != 0) {
            c[0]--;
        }
    }
    FUN_L00_0023aaa8((char *)e + 0x40);
}
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236AB8), where it is exact; names translated to the US level program. */

extern void FUN_L00_0023aaa8(void *);

void FUN_L00_00236128(HudElem *e) {
    unsigned char *c = e->cnt;
    int v;
    int cap;

    if (e->unk0C != 0) {
        v = *e->unk0C;
        cap = e->unk08;
        if (v < 0) {
            v = 0;
        }
        e->unk78 = v;
        if (cap < v) {
            e->unk78 = cap;
        }
    }
    if (e->unk74 != e->unk78) {
        e->unk7C = scale_game_frames(0xB4);
        if (e->unk6C >= 0x18) {
            e->unk74 = e->unk78;
        }
    }
    if (e->unk7C >= scale_game_frames(5)) {
        if (c[0] < scale_game_frames(8)) {
            c[0]++;
        } else if (c[1] < scale_game_frames(8)) {
            c[1]++;
        }
    } else {
        if (c[1] != 0 || c[0] != 0) {
            e->unk6C = 1;
        }
        if (c[1] != 0) {
            c[1]--;
        } else if (c[0] != 0) {
            c[0]--;
        }
    }
    FUN_L00_0023aaa8((char *)e + 0x40);
}
extern float star_init_limit0_gp __asm__("D_L00_0015F750") __attribute__((sda));
extern float star_init_limit1_gp __asm__("D_L00_0015F754") __attribute__((sda));
extern float star_init_limit2_gp __asm__("D_L00_0015F758") __attribute__((sda));
/* Fills the 100 hud stars with random positions and drift. */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236BF8), where it is exact; names translated to the US level program. */

extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
typedef struct {
    float x, y, vx, vy;
} Star_236458;

extern Star_236458 D_L00_0017E660[];
extern int D_L00_0015F75C_gp __asm__("D_L00_0015F75C") __attribute__((sda));

void FUN_L00_00236268(HudElem *elem) {
    int i;
    float margin, lo, band, hi;
    int bands;

    FUN_L00_00235d80(elem);
    for (i = 0; i < 100; i++) {
        margin = star_init_limit0_gp;
        if (i >= 10) {
            margin = star_init_limit1_gp;
            if (i >= 20) margin = star_init_limit2_gp;
        }
        bands = D_L00_0015F75C_gp;
        band = (232.0f - (margin + margin)) / (float)bands;
        lo = (margin - 116.0f) + (float)(i % bands) * band;
        hi = lo + band;
        D_L00_0017E660[i].x = random_float_between(margin - 17.0f, 17.0f - margin);
        D_L00_0017E660[i].y = random_float_between(lo, hi);
        D_L00_0017E660[i].vx = random_float_between(-0.4f, 0.4f);
        D_L00_0017E660[i].vy = random_float_between(-0.6f, 0.6f);
        D_L00_0017E660[i].vx += random_float_between(-0.4f, 0.4f);
        D_L00_0017E660[i].vy += random_float_between(-0.6f, 0.6f);
        D_L00_0017E660[i].vx += random_float_between(-0.4f, 0.4f);
        D_L00_0017E660[i].vy += random_float_between(-0.6f, 0.6f);
    }
    /* Fakematch: keeping bands live past the loop gives retail's schedule of the loop body. */
    bands++;
    bands--;
}


extern int D_L00_0015F3F8;
void FUN_L00_00235ea0();

/* Moves every other star by its velocity and bounces it off a box whose size
 * depends on the star index. */
void FUN_L00_00236458(void) {
    Star_236458 *p;
    int i;
    float lim, hx, hy, nhx, nhy;

    FUN_L00_00235ea0();
    p = &D_L00_0017E660[D_L00_0015F3F8 % 2];
    i = D_L00_0015F3F8 % 2;
    for (; i < 100; i += 2, p += 2) {
        if (i < 10) {
            lim = star_init_limit0_gp;
        } else if (i < 30) {
            lim = star_init_limit1_gp;
        } else {
            lim = star_init_limit2_gp;
        }
        hx = lim - 17.0f;
        hy = lim - 116.0f;
        nhx = -hx;
        nhy = -hy;
        p->x += p->vx;
        p->y += p->vy;
        if (p->x < hx) {
            p->x = hx;
            p->vx = -p->vx;
        }
        if (nhx < p->x) {
            p->x = nhx;
            p->vx = -p->vx;
        }
        if (p->y < hy) {
            p->y = hy;
            p->vy = -p->vy;
        }
        if (nhy < p->y) {
            p->y = nhy;
            p->vy = -p->vy;
        }
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002365a8.s", FUN_L00_002365a8);
#else
extern int D_L00_0015F760 __attribute__((sda));
extern int D_L00_0015F764 __attribute__((sda));
extern float D_L00_0015F768 __attribute__((sda));
extern float D_L00_0015F76C __attribute__((sda));
extern void FUN_L00_0023caa8(int, int, int, int, unsigned long, int) __asm__("FUN_00200e08");
extern int FUN_001fef20(int) __asm__("FUN_001f96f8");
extern float FUN_001fef10(float) __asm__("FUN_001f96b0");
extern void FUN_L00_00216de8(int, int);
extern void *FUN_0023b208(int, int) __asm__("FUN_001ff960");
extern void FUN_L00_0023bac0(void *, int, int, int, int, int);
extern void FUN_L00_0023bd20(void *, int, int, int, int, int) __asm__("FUN_00200080");
extern void FUN_L00_001ff348(void *, void *, void *);
extern void FUN_001ff260(void *) __asm__("FUN_001f99f8");
extern void FUN_001ff2a8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001ff278(void *, void *, void *) __asm__("FUN_001f9a10");

typedef struct { float x, y, z, w; } HudStarVec;

int FUN_L00_002365a8(HudElem *e) {
    int x = e->unk50, y = e->unk54;
    int w, h, left, top, right, bottom, xbase, ybase;
    int previous, current, period, threshold;
    int size, i, star_width;
    float fade, half_w, half_h, offset;
    HudStarVec origin, direction, first_step, second_step, third_step, star_position;
    void *image;

    e->w = D_L00_0015F760;
    e->h = D_L00_0015F764;
    FUN_L00_00235a70(e, &x, &y);
    FUN_L00_00235ad8(e, &x, &y, e->unk6C, 0);
    w = D_L00_0015F760;
    h = D_L00_0015F764;
    xbase = (x + e->unk48) << 4;
    ybase = (y + e->unk4A) << 4;
    left = xbase + (w * 0xd0) / 64;
    top = ybase + (h * 0xa0) / 256;
    right = left + (w * 0x260) / 64;
    bottom = top + (h * 0xea0) / 256;
    FUN_L00_0023caa8(left, top, right, bottom, 0x80684c2fUL, 1);

    if (e->unk74 * 3 < 10001) {
        size = FUN_001fef20(1);
        half_w = FUN_001fef10(0.75f);
        previous = (D_L00_0015F3F8 - 1) % (int)((float)size * 60.0f) <
                   (int)(half_w * 60.0f);
    } else {
        previous = 0;
    }
    if (e->unk74 * 3 < 10001) {
        size = FUN_001fef20(1);
        half_w = FUN_001fef10(0.75f);
        current = D_L00_0015F3F8 % (int)((float)size * 60.0f) <
                  (int)(half_w * 60.0f);
    } else {
        current = 0;
    }
    if (!previous && current == 1) FUN_L00_00216de8(11, 0);
    image = FUN_0023b208(*(int *)e, current);
    FUN_L00_0023bac0(image, x + e->unk48, y + e->unk4A,
                       D_L00_0015F760, D_L00_0015F764, 0x80);

    origin.z = 1.0f;
    origin.w = 1.0f;
    half_h = (float)D_L00_0015F764 * D_L00_0015F76C * 0.5f;
    offset = (float)(10000 - e->unk74) / 10000.0f;
    half_w = (float)D_L00_0015F760 * D_L00_0015F768 * 0.5f;
    origin.x = half_w / 17.0f;
    origin.y = -half_h / 116.0f;
    direction.z = 0.0f;
    direction.w = 0.0f;
    direction.y = half_h + (float)(e->unk4A + y);
    direction.x = half_w + (float)(x + e->unk48);
    direction.x += (1.0f - D_L00_0015F768) * 0.5f * (float)D_L00_0015F760;
    direction.y += (1.0f - D_L00_0015F76C) * 0.5f * (float)D_L00_0015F764;
    direction.x *= 16.0f;
    direction.y *= 16.0f;
    origin.x *= 16.0f;
    origin.y *= 16.0f;
    offset = 116.0f - (offset + offset) * 116.0f;
    image = FUN_0023b208(*(int *)e, 2);
    FUN_001ff260(&first_step);
    FUN_001ff260(&second_step);
    FUN_001ff260(&third_step);
    first_step.y = first_step.x = star_init_limit0_gp * 16.0f;
    second_step.y = second_step.x = (star_init_limit1_gp - star_init_limit0_gp) * 16.0f;
    third_step.y = third_step.x = (star_init_limit2_gp - star_init_limit1_gp) * 16.0f;
    star_width = 0;
    for (i = 0; i < 100; i++) {
        if (i == 0) {
            FUN_001ff2a8(&direction, &direction, &first_step);
            offset += star_init_limit0_gp;
            star_width = (int)(star_init_limit0_gp * 32.0f);
        } else if (i == 10) {
            FUN_001ff2a8(&direction, &direction, &second_step);
            star_width = (int)(star_init_limit1_gp * 32.0f);
            offset += star_init_limit1_gp - star_init_limit0_gp;
        } else if (i == 30) {
            FUN_001ff2a8(&direction, &direction, &third_step);
            star_width = (int)(star_init_limit2_gp * 32.0f);
            offset += star_init_limit2_gp - star_init_limit1_gp;
        }
        if (!(offset < D_L00_0017E660[i].y)) {
            FUN_L00_001ff348(&star_position, &D_L00_0017E660[i], &origin);
            FUN_001ff278(&star_position, &star_position, &direction);
            FUN_L00_0023bd20(image, (int)star_position.x, (int)star_position.y,
                              star_width, star_width, 0x80);
        }
    }
    return e->w;
}
#endif
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00236e50.s", FUN_L00_00236e50);
#else
extern int D_L00_0015F760 __attribute__((sda));
extern int D_L00_0015F764 __attribute__((sda));
extern float D_L00_0015F768 __attribute__((sda));
extern float D_L00_0015F76C __attribute__((sda));
extern void *FUN_0023b208(int, int) __asm__("FUN_001ff960");
extern void FUN_L00_0023bac0(void *, int, int, int, int, int);
extern void FUN_L00_0023bd20(void *, int, int, int, int, int) __asm__("FUN_00200080");
extern void FUN_L00_001ff348(void *, void *, void *);
extern void FUN_001ff260(void *) __asm__("FUN_001f99f8");
extern void FUN_001ff2a8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001ff278(void *, void *, void *) __asm__("FUN_001f9a10");
int FUN_L00_00236e50(HudElem *e) {
    int x = e->unk50, y = e->unk54;
    int i, star_width = 0;
    float half_h, offset, half_w;
    int xbase, ybase;
    struct { float x, y, z, w; } origin, direction, first_step, second_step, third_step, star_position;
    void *image;

    e->w = D_L00_0015F760;
    e->h = D_L00_0015F764;
    FUN_L00_00235a70(e, &x, &y);
    FUN_L00_00235ad8(e, &x, &y, e->unk6C, 0);
    image = FUN_0023b208(*(int *)e, 0);
    FUN_L00_0023bac0(image, x + e->unk48, y + e->unk4A,
                       D_L00_0015F760, D_L00_0015F764, 0x80);
    origin.z = 1.0f;
    origin.w = 1.0f;
    half_h = (float)D_L00_0015F764 * D_L00_0015F76C * 0.5f;
    offset = (float)(10000 - e->unk74) / 10000.0f;
    half_w = (float)D_L00_0015F760 * D_L00_0015F768 * 0.5f;
    xbase = x + e->unk48;
    ybase = y + e->unk4A;
    origin.x = half_w / 17.0f;
    origin.y = -half_h / 116.0f;
    direction.z = 0.0f;
    direction.w = 0.0f;
    direction.y = half_h + (float)ybase;
    direction.x = half_w + (float)xbase;
    direction.x += (1.0f - D_L00_0015F768) * 0.5f * (float)D_L00_0015F760;
    direction.y += (1.0f - D_L00_0015F76C) * 0.5f * (float)D_L00_0015F764;
    direction.x *= 16.0f;
    direction.y *= 16.0f;
    origin.x *= 16.0f;
    origin.y *= 16.0f;
    offset = 116.0f - (offset + offset) * 116.0f;
    image = FUN_0023b208(*(int *)e, 1);
    FUN_001ff260(&first_step);
    FUN_001ff260(&second_step);
    FUN_001ff260(&third_step);
    first_step.y = first_step.x = star_init_limit0_gp * 16.0f;
    second_step.y = second_step.x = (star_init_limit1_gp - star_init_limit0_gp) * 16.0f;
    third_step.y = third_step.x = (star_init_limit2_gp - star_init_limit1_gp) * 16.0f;
    for (i = 0; i < 100; i++) {
        if (i == 0) {
            FUN_001ff2a8(&direction, &direction, &first_step);
            offset += star_init_limit0_gp;
            star_width = (int)(star_init_limit0_gp * 32.0f);
        }
        if (!(offset < D_L00_0017E660[i].y)) {
            FUN_L00_001ff348(&star_position, &D_L00_0017E660[i], &origin);
            FUN_001ff278(&star_position, &star_position, &direction);
            FUN_L00_0023bd20(image, (int)star_position.x, (int)star_position.y,
                              star_width, star_width, 0x80);
        }
    }
    return e->w;
}
#endif
void FUN_L00_00235ea0();
/* Calls FUN_L00_00235ea0. */
void FUN_L00_002371e0(void) {
    FUN_L00_00235ea0();
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00237200.s", FUN_L00_00237200);
#else
extern int pal_mode __asm__("D_0015ED80");
extern int current_level_index_sda __asm__("D_0015ED84") __attribute__((sda));
extern int D_L00_0015F770 __attribute__((sda));
extern unsigned char D_L00_0017E530[];
extern int freeze_stats_time_ref[] __asm__("D_0015EE58");
extern int freeze_stats_score_ref_a[] __asm__("D_0015EE68");
extern float D_0013F3D0[];
extern void *memcpy(void *, const void *, unsigned int);

extern float FUN_00200240(unsigned int) __asm__("FUN_001fa6c0");
extern float FUN_001fef10(float) __asm__("FUN_001f96b0");
extern int FUN_00200250(float) __asm__("FUN_001fa6d0");
extern int FUN_001fb090(const char *, int) __asm__("FUN_L00_001fb090");
extern void *FUN_0023b208(int, int) __asm__("FUN_001ff960");
extern void FUN_0023b4d8(void *, int, int, int, int, int) __asm__("FUN_001ffc30");
extern void FUN_0023b860(void *, int, int, int, int, int) __asm__("FUN_001ffe18");
extern void FUN_001fb9a8(int, int, int, const char *, int) __asm__("FUN_001f6af0");
extern float FUN_001ff450(float *, float *) __asm__("FUN_001f9b48");
extern const char freeze_stats_score_format[] __asm__("D_L00_0015F778");
extern const char freeze_stats_time_format[] __asm__("D_L00_001E84C0");
extern char *FUN_00203ad8(int) __asm__("FUN_001fdd10");
extern int sprintf(char *, const char *, ...) __asm__("FUN_00116248");
extern void FUN_0023cf80(int, int, int, char *) __asm__("FUN_00201200");

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) HudPoint_237200;
int FUN_L00_00237200(HudElem *e) {
    int y, i, width, left, other_width, other_left;
    int alpha, color, level, elapsed, extra, interval, minute_scale, remainder, seconds;
    float fade;
    HudPoint_237200 points[2];
    HudPoint_237200 point;
    char text[64];
    const char *line;
    unsigned char *opacity = (unsigned char *)e + 0x70;
    int text_y;

    if (pal_mode != 0) {
        y = D_L00_0015F770 + 0xa;
    } else {
        y = D_L00_0015F770 + 0x12;
    }
    fade = FUN_00200240(*opacity);
    fade /= FUN_001fef10(8.0f);
    if (fade < 0.0f) fade = 0.0f;
    if (fade > 1.0f) fade = 1.0f;
    text_y = y + 8;
    alpha = FUN_00200250(fade * 128.0f);
    color = (alpha << 24) | 0x40f040;
    i = 0;
    if (D_L00_0017E530[0] > 1) {
        do {
            i++;
        } while (D_L00_0017E530[i] > 1 && i < 0x80);
    }
    if (D_L00_0017E530[i] == 1) {
        D_L00_0017E530[i] = 0;
        width = FUN_001fb090((const char *)D_L00_0017E530, -1);
        left = 0xe0 - (width >> 1);
        {
            int other_width = FUN_001fb090((const char *)&D_L00_0017E530[1] + i, -1);
            other_left = 0xe0 - (other_width >> 1);
            if (left >= other_left) {
                width = other_width;
            } else {
                other_left = left;
            }
            left = other_left;
        }
        other_left = left + 0x20;
        FUN_0023b4d8(FUN_0023b208(0x7580, 0), other_left, y, width, 0x36, alpha);
        other_left = color;
        FUN_0023b4d8(FUN_0023b208(0x7580, 1), left, y, 0x20, 0x36, alpha);
        FUN_0023b860(FUN_0023b208(0x7580, 1), (width + 0x20) + left, y, 0x20, 0x36, alpha);
        FUN_001fb9a8(0x100, text_y, other_left, (const char *)D_L00_0017E530, -1);
        FUN_001fb9a8(0x100, y + 0x1b, other_left, (const char *)&D_L00_0017E530[1] + i, -1);
        level = current_level_index_sda;
    } else {
        int width, left;
        width = FUN_001fb090((const char *)D_L00_0017E530, -1);
        left = 0xe0 - (width >> 1);
        FUN_0023b4d8(FUN_0023b208(0x7580, 1), left, y, 0x20, 0x20, alpha);
        FUN_0023b4d8(FUN_0023b208(0x7580, 0), left + 0x20, y, width, 0x20, alpha);
        line = FUN_0023b208(0x7580, 1);
        width += 0x20;
        FUN_0023b860((void *)line, left + width, y, 0x20, 0x20, alpha);
        FUN_001fb9a8(0x100, text_y, color, (const char *)D_L00_0017E530, -1);
        level = *(int *)0x15ed84;
    }
    if (level == 5 || level == 0x10) {
        typedef unsigned int quadword __attribute__((mode(TI), aligned(16)));
        *(quadword *)&points[0] = 0;
        *(quadword *)&points[1] = 0;
        points[0].x = 294.5f;
        points[0].y = 225.4f;
        points[0].z = 100.0f;
        points[1].x = 115.7f;
        points[1].y = 287.0f;
        points[1].z = 119.5f;
        {
            float *point_data;
            line = level == 5 ? memcpy(&point, &points[0], 16) : memcpy(&point, &points[1], 16);
            point_data = (float *)line;
            if (FUN_001ff450(D_0013F3D0, point_data) < 5.0f) {
                *(int *)0x15f648 = y + 100;
                extra = current_level_index_sda == 0x10 ? 1 : 0;
                elapsed = freeze_stats_time_ref[extra];
                extra = freeze_stats_score_ref_a[extra];
                if (elapsed != 0) {
                    const char *message;
                    interval = pal_mode != 0 ? 3000 : 3600;
                    seconds = elapsed / interval;
                    other_width = interval;
                    interval /= 60;
                    remainder = elapsed - seconds * other_width;
                    other_left = remainder / interval;
                    remainder = (remainder - other_left * interval) * 100 / interval;
                    line = freeze_stats_time_format;
                    message = FUN_00203ad8(0x531b);
                    sprintf(text, line, message, seconds, other_left, remainder);
                    FUN_0023cf80(0x100, y + 0x28, color, text);
                    if (extra != 0) {
                        sprintf(text, freeze_stats_score_format, FUN_00203ad8(0x531c), extra);
                        FUN_0023cf80(0x100, y + 0x48, color, text);
                    }
                }
            }
        }
    }
    return *(int *)((char *)e + 0x58);
}
#endif
