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
int hud_screen_position(HudElem *rec, int *x, int *y) __asm__("FUN_L00_00235a70");
int hud_screen_position(HudElem *rec, int *x, int *y) {
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

void FUN_L00_00235ad8(HudElem *hud_element, int *x, int *y, int slide_index, int slide_step) {
    int dx = 0;
    int dy = 0;
    float slide_scale;

    if (hud_element->unk7C != 0) {
        slide_index -= slide_step;
    } else {
        slide_index += slide_step;
    }
    if (slide_index < 0) {
        slide_index = 0;
    }
    if (slide_index > 23) {
        slide_index = 23;
    }
    if (hud_element->unk7C != 0) {
        slide_scale = D_L00_0017E3A0[slide_index];
    } else {
        slide_scale = D_L00_0017E400[slide_index];
    }
    if (hud_element->flags & 1) {
        dy = -truncate_float_to_s32(slide_scale * (convert_integer_to_float(hud_element->h) + 52.0f) + 0.5f);
    } else if (hud_element->flags & 2) {
        dy = truncate_float_to_s32(slide_scale * (convert_integer_to_float(hud_element->h) + 52.0f) + 0.5f);
    } else if (hud_element->flags & 4) {
        dx = -truncate_float_to_s32(slide_scale * (convert_integer_to_float(hud_element->w) + 20.0f) + 0.5f);
    } else if (hud_element->flags & 8) {
        dx = truncate_float_to_s32(slide_scale * (convert_integer_to_float(hud_element->w) + 20.0f) + 0.5f);
    }
    *x += dx;
    *y += dy;
}
#include "sda.h"

/* updates numeric HUD values and dimensions from digit count */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236610), where it is exact; names translated to the US level program. */

void FUN_L00_00235c80(HudElem *hud_element) {
    int *value = hud_element->unk0C;
    int digits;
    int v, flags, width;
    if (value && !((int)value & 3)) {
        hud_element->unk78 = *value;
        if (hud_element->unk08 < hud_element->unk78)
            hud_element->unk78 = hud_element->unk08;
        hud_element->unk74 = hud_element->unk78;
    } else {
        hud_element->unk78 = 99999;
        hud_element->unk74 = 99999;
    }
    v = hud_element->unk08;
    digits = 0;
    flags = hud_element->flags;
    width = hud_element->h;
    while (v >= 10) {
        v /= 10;
        digits++;
    }
    if (!(flags & 3) && (flags & 12)) {
        hud_element->h = width + (digits + 1) * 12;
        if (hud_element->w < 14)
            hud_element->w = 14;
    } else {
        if (width < 12)
            hud_element->h = 12;
        hud_element->w += (digits + 1) * 14;
    }
}
/* Resets a HUD element: a timer of 30 + func_001F9850(180) (+0x7C),
   offsets 0x48 / 0x4A cleared, then func_L00_00236610. */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236710), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00235c80(HudElem *);

/* Sets unk7C to scale_game_frames(0xB4) + 0x1E, clears 0x48/0x4A, calls FUN_L00_00235c80. */
void FUN_L00_00235d80(HudElem *hud_element) {
    hud_element->unk7C = scale_game_frames(0xB4) + 0x1E;
    hud_element->unk48 = 0;
    hud_element->unk4A = 0;
    FUN_L00_00235c80(hud_element);
}
/* func_L00_00236710 for a 32 x 32 element. */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236750), where it is exact; names translated to the US level program. */

/* Sets unk7C to scale_game_frames(0xB4) + 0x1E, clears 0x48/0x4A, w/h 0x20, calls 00235c80. */
void FUN_L00_00235dc0(HudElem *hud_element) {
    hud_element->unk7C = scale_game_frames(0xB4) + 0x1E;
    hud_element->unk48 = 0;
    hud_element->unk4A = 0;
    hud_element->w = 0x20;
    hud_element->h = 0x20;
    FUN_L00_00235c80(hud_element);
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
    T_235e18 *hud_element;
    r = 1;
    for (i = 0; i < 13; i++) {
        if (D_L00_0017DC50_235e18[i].key == key)
            break;
    }
    if (i < 13 && D_L00_0017DC50_235e18[i].f68 == 0) {
        hud_element = D_L00_0017DC50_235e18 + i;
        hud_element->v = (val < hud_element->v) ? hud_element->v : val;
        return 0;
    }
    return 1;
}
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
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
                s = FUN_001fa6d0(square_root(convert_integer_to_float(d) / 25.0f) * 5.0f);
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

void FUN_L00_00236128(HudElem *hud_element) {
    unsigned char *hud_counters = hud_element->cnt;
    int v;
    int cap;

    if (hud_element->unk0C != 0) {
        v = *hud_element->unk0C;
        cap = hud_element->unk08;
        if (v < 0) {
            v = 0;
        }
        hud_element->unk78 = v;
        if (cap < v) {
            hud_element->unk78 = cap;
        }
    }
    if (hud_element->unk74 != hud_element->unk78) {
        hud_element->unk7C = scale_game_frames(0xB4);
        if (hud_element->unk6C >= 0x18) {
            hud_element->unk74 = hud_element->unk78;
        }
    }
    if (hud_element->unk7C >= scale_game_frames(5)) {
        if (hud_counters[0] < scale_game_frames(8)) {
            hud_counters[0]++;
        } else if (hud_counters[1] < scale_game_frames(8)) {
            hud_counters[1]++;
        }
    } else {
        if (hud_counters[1] != 0 || hud_counters[0] != 0) {
            hud_element->unk6C = 1;
        }
        if (hud_counters[1] != 0) {
            hud_counters[1]--;
        } else if (hud_counters[0] != 0) {
            hud_counters[0]--;
        }
    }
    FUN_L00_0023aaa8((char *)hud_element + 0x40);
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

/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236F38). */

extern int D_L00_0015F760 __attribute__((sda));
extern int D_L00_0015F764 __attribute__((sda));
extern float D_L00_0015F768 __attribute__((sda));
extern float D_L00_0015F76C __attribute__((sda));
extern void append_screen_rect_packet(s32, s32, s32, s32, u64, s32) __asm__("FUN_00200e08");
extern float scale_game_seconds(float) __asm__("FUN_001f96b0");
extern void FUN_L00_00216de8(int, int);
extern s32 get_icon_frame(s32, s32) __asm__("FUN_001ff960");
extern void FUN_L00_0023bac0(int, int, int, int, int, int);
extern void FUN_L00_001ff348(void *, void *, void *);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void draw_hud_sprite_subpixel(s32, s32, s32, s32, s32, s32) __asm__("FUN_00200080");

int FUN_L00_002365a8(HudElem *element) {
    int x, y, i, previous, current, frames, image, star_width;
    float origin[4], direction[4], first_step[4], second_step[4], third_step[4], star_position[4];
    float seconds, sixteen, offset, thirty_two;

    {
        int width = D_L00_0015F760;
        x = element->unk50;
        y = element->unk54;
        element->w = width;
        element->h = D_L00_0015F764;
    }
    hud_screen_position(element, &x, &y);
    FUN_L00_00235ad8(element, &x, &y, element->unk6C, 0);

    {
        int left_inset = D_L00_0015F760 * 0xD0;
        int top_inset = D_L00_0015F764 * 0xA0;
        int bar_width = D_L00_0015F760 * 0x260;
        int bottom = D_L00_0015F764 * 0xEA0;
        int left = (x + element->unk48) << 4;
        int top = (y + element->unk4A) << 4;
        int right;
        left += left_inset / 64;
        top += top_inset / 256;
        right = left + bar_width / 64;
        bottom = top + bottom / 256;
        append_screen_rect_packet(left, top, right, bottom, 0x80684C2F, 1);
    }

    if (element->unk74 * 3 < 10001) {
        frames = scale_game_frames(1);
        seconds = scale_game_seconds(0.75f);
        previous = (D_L00_0015F3F8 - 1) % (int)((float)frames * 60.0f) < (int)(seconds * 60.0f);
    } else {
        previous = 0;
    }
    if (element->unk74 * 3 < 10001) {
        frames = scale_game_frames(1);
        seconds = scale_game_seconds(0.75f);
        current = D_L00_0015F3F8 % (int)((float)frames * 60.0f) < (int)(seconds * 60.0f);
    } else {
        current = 0;
    }
    if (!previous && current == 1) {
        FUN_L00_00216de8(0xB, 0);
    }
    star_width = 0;
    image = get_icon_frame(*(int *)element, current);
    sixteen = 16.0f;
    thirty_two = 32.0f;
    FUN_L00_0023bac0(image, x + element->unk48, y + element->unk4A, D_L00_0015F760, D_L00_0015F764, 0x80);

    {
        float center_x, center_y, half_w, half_h;
        half_w = (D_L00_0015F760 * D_L00_0015F768 * 0.5f) / 17.0f;
        half_h = -(D_L00_0015F764 * D_L00_0015F76C * 0.5f) / 116.0f;
        origin[2] = 1.0f;
        origin[3] = 1.0f;
        center_x = D_L00_0015F760 * D_L00_0015F768 * 0.5f + (x + element->unk48);
        center_y = D_L00_0015F764 * D_L00_0015F76C * 0.5f + (y + element->unk4A);
        direction[0] = (center_x + (1.0f - D_L00_0015F768) * 0.5f * D_L00_0015F760) * sixteen;
        direction[1] = (center_y + (1.0f - D_L00_0015F76C) * 0.5f * D_L00_0015F764) * sixteen;
        direction[2] = 0.0f;
        direction[3] = 0.0f;
        origin[0] = half_w * sixteen;
        origin[1] = half_h * sixteen;
    }
    offset = (float)(10000 - element->unk74) / 10000.0f;
    offset = 116.0f - (offset + offset) * 116.0f;
    image = get_icon_frame(*(int *)element, 2);

    clear_u64_value(first_step);
    clear_u64_value(second_step);
    clear_u64_value(third_step);
    {
        float limit0 = star_init_limit0_gp;
        float limit1 = star_init_limit1_gp;
        float limit2 = star_init_limit2_gp;
        float first = limit0 * sixteen;
        limit0 = limit1 - limit0;
        limit2 = limit2 - limit1;
        first_step[0] = first;
        first_step[1] = first;
        limit0 = limit0 * sixteen;
        second_step[0] = limit0;
        second_step[1] = limit0;
        limit2 = limit2 * sixteen;
        third_step[0] = limit2;
        third_step[1] = limit2;
    }

    for (i = 0; i < 100; i++) {
        if (i == 0) {
            float limit;
            fast_vec_sub(direction, direction, first_step);
            limit = star_init_limit0_gp;
            star_width = (int)(limit * thirty_two);
            offset = offset + limit;
        } else if (i == 10) {
            float limit, previous_limit, width;
            fast_vec_sub(direction, direction, second_step);
            limit = star_init_limit1_gp;
            previous_limit = star_init_limit0_gp;
            width = limit * thirty_two;
            limit = limit - previous_limit;
            star_width = (int)width;
            offset = offset + limit;
        } else if (i == 30) {
            float limit, previous_limit, width;
            fast_vec_sub(direction, direction, third_step);
            limit = star_init_limit2_gp;
            previous_limit = star_init_limit1_gp;
            width = limit * thirty_two;
            limit = limit - previous_limit;
            star_width = (int)width;
            offset = offset + limit;
        }
        if (offset < D_L00_0017E660[i].y) continue;
        FUN_L00_001ff348(star_position, &D_L00_0017E660[i], origin);
        fast_vec_add(star_position, star_position, direction);
        draw_hud_sprite_subpixel(image, (int)star_position[0], (int)star_position[1], star_width, star_width, 0x80);
    }
    return element->w;
}
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_002377E0), where it is exact; names translated to the US level program. */


extern int D_L00_0015F760 __attribute__((sda));
extern int D_L00_0015F764 __attribute__((sda));
extern float D_L00_0015F768 __attribute__((sda));
extern float D_L00_0015F76C __attribute__((sda));
extern s32 get_icon_frame(s32, s32) __asm__("func_001FF960");
extern void FUN_L00_0023bac0(int, int, int, int, int, int);
extern void FUN_L00_001ff348(void *, void *, void *);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void draw_hud_sprite_subpixel(s32, s32, s32, s32, s32, s32) __asm__("func_00200080");

int FUN_L00_00236e50(HudElem *hud_element) {
    float origin[4], direction[4], first_step[4], second_step[4], third_step[4], star_position[4];
    int x, y;
    float offset;
    s32 image;
    int star_width, i;

    x = hud_element->unk50;
    y = hud_element->unk54;
    hud_element->w = D_L00_0015F760;
    hud_element->h = D_L00_0015F764;
    hud_screen_position(hud_element, &x, &y);
    FUN_L00_00235ad8(hud_element, &x, &y, hud_element->unk6C, 0);
    image = get_icon_frame(*(int *)hud_element, 0);
    FUN_L00_0023bac0(image, x + hud_element->unk48, y + hud_element->unk4A, D_L00_0015F760, D_L00_0015F764, 0x80);
    {
        float center_x, center_y, half_w, half_h;
        half_w = (D_L00_0015F760 * D_L00_0015F768 * 0.5f) / 17.0f;
        half_h = -(D_L00_0015F764 * D_L00_0015F76C * 0.5f) / 116.0f;
        origin[2] = 1.0f;
        origin[3] = 1.0f;
        center_x = D_L00_0015F760 * D_L00_0015F768 * 0.5f + (x + hud_element->unk48);
        center_y = D_L00_0015F764 * D_L00_0015F76C * 0.5f + (y + hud_element->unk4A);
        direction[0] = (center_x + (1.0f - D_L00_0015F768) * 0.5f * D_L00_0015F760) * 16.0f;
        direction[1] = (center_y + (1.0f - D_L00_0015F76C) * 0.5f * D_L00_0015F764) * 16.0f;
        direction[2] = 0.0f;
        direction[3] = 0.0f;
        origin[0] = half_w * 16.0f;
        origin[1] = half_h * 16.0f;
    }
    offset = (10000 - hud_element->unk74) / 10000.0f;
    offset = 116.0f - (offset + offset) * 116.0f;
    image = get_icon_frame(*(int *)hud_element, 1);
    clear_u64_value(first_step);
    clear_u64_value(second_step);
    clear_u64_value(third_step);
    first_step[1] = first_step[0] = star_init_limit0_gp * 16.0f;
    second_step[1] = second_step[0] = (star_init_limit1_gp - star_init_limit0_gp) * 16.0f;
    third_step[1] = third_step[0] = (star_init_limit2_gp - star_init_limit1_gp) * 16.0f;
    star_width = 0;
    for (i = 0; i < 100; i++) {
        if (i == 0) {
            fast_vec_sub(direction, direction, first_step);
            star_width = (int)(star_init_limit0_gp * 32.0f);
            offset += star_init_limit0_gp;
        }
        if (offset < D_L00_0017E660[i].y) continue;
        FUN_L00_001ff348(star_position, &D_L00_0017E660[i], origin);
        fast_vec_add(star_position, star_position, direction);
        draw_hud_sprite_subpixel(image, (int)star_position[0], (int)star_position[1], star_width, star_width, 0x80);
    }
    return hud_element->w;
}
void FUN_L00_00235ea0();
/* Calls FUN_L00_00235ea0. */
void mine_blend_color(void) __asm__("FUN_L00_002371e0");
void mine_blend_color(void) {
    FUN_L00_00235ea0();
}
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
extern s32 D_L00_0015F770_gp __asm__("D_L00_0015F770") __attribute__((sda));
extern s32 D_L00_0015F648_macro __asm__("D_L00_0015F648") MACRO_ADDR;
extern u8 D_L00_0017E530[] NOT_SDA;
extern char D_0013F3D0[];
extern s32 freeze_stats_time_ref[] __asm__("D_0015EE58") __attribute__((section(".sdata")));
extern s32 freeze_stats_score_ref_a[] __asm__("D_0015EE68") __attribute__((section(".sdata")));
extern char freeze_stats_time_format[] __asm__("D_L00_001E84C0") NOT_SDA;
extern char freeze_stats_score_format[] __asm__("D_L00_0015F778") NOT_SDA;
extern s32 pal_mode __asm__("D_0015ED80");
extern s32 current_level_index __asm__("D_0015ED84");
extern s32 measure_text_width_regular(u8 *, s32) __asm__("FUN_001f6250");
extern void draw_hud_sprite(s32, s32, s32, s32, s32, s32) __asm__("FUN_001ffc30");
extern void draw_hud_sprite_flipped(s32, s32, s32, s32, s32, s32) __asm__("FUN_001ffe18");
extern s32 font_print_center(s32, s32, s32, s32, s32) __asm__("FUN_001f6af0");
extern f32 fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern char *get_help_message_text(s32) __asm__("FUN_001fdd10");
extern s32 sprintf(char *, const char *, ...) __asm__("FUN_00116248");
extern void draw_framed_text(s32, s32, s32, char *) __asm__("FUN_00201200");

int FUN_L00_00237200(HudElem *hud_element) {
    int y, i;
    int alpha, color, level;
    float fade;
    Vec4 points[2];
    Vec4 point;
    char text[64];
    u8 *opacity = hud_element->cnt;
    int text_y;
    int text_color;
    int never = 0;

    if (pal_mode != 0) {
        y = D_L00_0015F770_gp + 0xa;
    } else {
        y = D_L00_0015F770_gp + 0x12;
    }
    fade = convert_integer_to_float(*opacity);
    fade /= scale_game_seconds(8.0f);
    if (fade < 0.0f) fade = 0.0f;
    if (fade > 1.0f) fade = 1.0f;
    text_y = y + 8;
    alpha = truncate_float_to_s32(fade * 128.0f);
    color = (alpha << 24) | 0x40f040;
    i = 0;
    if (D_L00_0017E530[0] > 1) {
        do {
            i++;
        } while (D_L00_0017E530[i] > 1 && i < 0x80);
    }
    if (D_L00_0017E530[i] == 1) {
        int width, left, second_width, second_left, x, w, right;
        s32 frame;
        D_L00_0017E530[i] = 0;
        width = measure_text_width_regular(D_L00_0017E530, -1);
        left = 0xe0 - (width >> 1);
        second_width = measure_text_width_regular(&D_L00_0017E530[1] + i, -1);
        second_left = 0xe0 - (second_width >> 1);
        if (left >= second_left) {
            x = second_left;
            w = second_width;
        } else {
            x = left;
            w = width;
        }
        draw_hud_sprite(get_icon_frame(0x7580, 0), x + 0x20, y, w, 0x36, alpha);
        text_color = color;
        draw_hud_sprite(get_icon_frame(0x7580, 1), x, y, 0x20, 0x36, alpha);
        frame = get_icon_frame(0x7580, 1);
        right = w + 0x20;
        draw_hud_sprite_flipped(frame, x + right, y, 0x20, 0x36, alpha);
        font_print_center(0x100, text_y, text_color, (s32)D_L00_0017E530, -1);
        font_print_center(0x100, y + 0x1b, text_color, (s32)(&D_L00_0017E530[1] + i), -1);
    } else {
        int width, left, right;
        s32 frame;
        width = measure_text_width_regular(D_L00_0017E530, -1);
        left = 0xe0 - (width >> 1);
        draw_hud_sprite(get_icon_frame(0x7580, 1), left, y, 0x20, 0x20, alpha);
        draw_hud_sprite(get_icon_frame(0x7580, 0), left + 0x20, y, width, 0x20, alpha);
        frame = get_icon_frame(0x7580, 1);
        right = width + 0x20;
        draw_hud_sprite_flipped(frame, left + right, y, 0x20, 0x20, alpha);
        font_print_center(0x100, text_y, color, (s32)D_L00_0017E530, -1);
    }
    level = current_level_index;
    if (level == 5 || level == 0x10) {
        points[0].q = 0;
        points[1].q = 0;
        points[0].f[0] = 294.5f;
        points[0].f[1] = 225.4f;
        points[0].f[2] = 100.0f;
        points[1].f[0] = 115.7f;
        points[1].f[1] = 287.0f;
        points[1].f[2] = 119.5f;
        if (level == 5) {
            qcopy(&point, &points[0]);
        } else {
            qcopy(&point, &points[1]);
        }
        if (fast_vec_dist(D_0013F3D0, &point) < 5.0f) {
            int elapsed, score, interval, unit, minutes, rest, seconds, hundredths;
            D_L00_0015F648_macro = y + 100;
            elapsed = freeze_stats_time_ref[current_level_index == 0x10];
            score = freeze_stats_score_ref_a[current_level_index == 0x10];
            if (elapsed != 0) {
                interval = pal_mode != 0 ? 3000 : 3600;
                unit = interval / 60;
                minutes = elapsed / interval;
                rest = elapsed - minutes * interval;
                seconds = rest / unit;
                rest = rest - seconds * unit;
                hundredths = rest * 100 / unit;
                sprintf(text, freeze_stats_time_format, get_help_message_text(0x531b), minutes, seconds, hundredths);
                draw_framed_text(0x100, y + 0x28, color, text);
                if (score != 0) {
                    sprintf(text, freeze_stats_score_format, get_help_message_text(0x531c), score);
                    draw_framed_text(0x100, y + 0x48, color, text);
                }
            }
        }
    }
    /* Fakematch: the never-taken text_color use keeps the then-branch color copy in its own register. */
    return hud_element->w + (text_color & never);
}
