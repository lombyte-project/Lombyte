/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00204130), where it is exact; names translated to the US level program. */

extern char D_L00_00179410[];
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void dismiss_help(void) __asm__("FUN_001fdc08");

/* Once (flag 0x38): starts a 0x3C-frame timer, dismisses help, sets the first word to 8 if 0. */
void jet_camera_mode(void) __asm__("FUN_L00_00203b18");
void jet_camera_mode(void) {
    char *g = D_L00_00179410;
    if (*(short *)(g + 0x38) == 0) {
        *(short *)(g + 0x38) = 1;
        *(int *)(g + 0x3C) = scale_game_frames(0x3C);
        dismiss_help();
        if (*(int *)g == 0)
            *(int *)g = 8;
    }
}
extern char D_L00_00179448[];
/* Clears the short at D_L00_00179448. */
void FUN_L00_00203b78(void) {
    *(short *)D_L00_00179448 = 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00205598), where it is exact; names translated to the US level program. */

extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_00232628(void);

/* Resets hero fields 0x220..0x234, 0x22CB, 0x2084 and 0xA94, then calls 00232628 and 002223f8. */
extern unsigned char D_0014161B[];
void FUN_L00_00204f80(void) {
    struct Hero *g = &hero;
    g->capsule_top = 0.8f;
    g->unk234 = 0.45f;
    g->unk228 = 0.8f;
    g->unk224 = 0.7f;
    g->unk22C = 0.7f;
    g->unk22CB = 0;
    g->state.current = 0;
    *(int *)(D_0014161B + 0x4D) = 0;
    g->unkA94 = 1.0f;
    FUN_L00_00232628();
    hero_set_state(HERO_STATE_IDLE, 1);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00205618), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x24];
    int v;
    char pad2[0x24];
} Rec;

extern Rec item_defs[] __asm__("D_L00_00179AC0");
extern int hero_slot_item_id(int) __asm__("FUN_L00_0020d498");

int hero_idle_sequence(int a) __asm__("FUN_L00_00205000");
int hero_idle_sequence(int a) {
    struct Hero *g = &hero;
    int c = g->vehicle_mode;
    if (c == 2 || (c < 3 ? c == 1 : c == 3)) {
        return 0;
    }
    {
        struct Hero *g2 = &hero;
        if (g2->health.hp == 1) {
            return 0x54;
        }
        if (g2->unk12E2 != 0) {
            return 0x6E;
        }
        if (a == 0) {
            return 0;
        }
        if (g2->unk20A8 == 0) {
            return 0;
        }
        if (g2->unk20AA == 0) {
            return 0;
        }
        if (g2->unk22C8 != 0) {
            return 0;
        }
    }
    {
        int i = hero_slot_item_id(0);
        return item_defs[i].v;
    }
}
/* Returns its argument unchanged. */
s32 FUN_L00_002050b8(s32 a) {
    return a;
}
void show_health_hud(void) __asm__("FUN_L00_00234e00");
/* Lowers hero.health.hp by one (not below 0) and calls 00234e00. */
void hero_change_health(s32 n) __asm__("FUN_L00_002050c0");
void hero_change_health(s32 n) {
    if (n) {
        struct Hero *g = &hero;
        int v;
        if (n >= 2)
            n = 1;
        v = g->health.hp - n;
        g->health.hp = v;
        if (v < 0)
            g->health.hp = 0;
        show_health_hud();
    }
}
extern s32 D_001413D0[];
extern s32 D_001404D4[];
extern s32 D_001403E0[];

/* Returns D_001413D0[0] for a = 0, 2, 3, 4, D_001404D4[0] for 1, D_001403E0[0] for 5, else 0. */
int FUN_L00_00205110(int a) {
    if (a == 0 || a == 2 || a == 3 || a == 4) {
        return D_001413D0[0];
    }
    if (a == 1) {
        return D_001404D4[0];
    }
    if (a == 5) {
        return D_001403E0[0];
    }
    return 0;
}
#include "qcopy.h"
extern u8 D_001413F4 __attribute__((section(".data")));
f32 absolute_float(f32) __asm__("FUN_001f99c0");
void clear_u64_value(void *) __asm__("FUN_001f99f8");
void detach_manipulator(s32, void *) __asm__("FUN_0020cb88");
void attach_manipulator(s32, s32, void *) __asm__("FUN_0020cb10");
/* FUN_L00_0025bc98 (eases an angle toward a target) with its int argument last. EABI
   passes ints and floats in separate registers, so the call is the same; this order
   gives retail's register allocation. */
f32 turn_k(f32 *, f32, f32 *, f32, f32, f32, s32) __asm__("FUN_L00_0025bc98");
void vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
void FUN_L00_00259f50(void *, void *);

/* Eases one record's rotation and position toward their targets and rebuilds its matrix;
   once targets are zero and it has settled, detaches it from its moby. */
void FUN_L00_00205168(struct HeroEase *e) {
    s32 moby = FUN_L00_00205110(e->kind);
    if (moby == 0)
        return;
    if (D_001413F4 == 0) {
        if (e->kind == 4 || e->kind == 2 || e->kind == 3)
            return;
    } else if (e->kind == 0) {
        return;
    }
    if (e->rot_target.f[0] == 0.0f && e->rot_target.f[1] == 0.0f && e->rot_target.f[2] == 0.0f &&
        e->pos_target.f[0] == 0.0f && e->pos_target.f[1] == 0.0f && e->pos_target.f[2] == 0.0f &&
        absolute_float(e->pos.f[0]) < 0.003f && absolute_float(e->pos.f[1]) < 0.003f &&
        absolute_float(e->pos.f[2]) < 0.003f && e->scale == 1.0f &&
        absolute_float(e->rot.f[0]) < 0.005f && absolute_float(e->rot.f[1]) < 0.005f &&
        absolute_float(e->rot.f[2]) < 0.005f) {
        if (e->attached)
            detach_manipulator(moby, e);
    } else {
        Vec4 *rot;
        Vec4 *pos;
        /* retail reads this flag at its literal address, with no relocation */
        if (*(u8 *)0x15EDB5) {
            e->rot_target.f[0] = -e->rot_target.f[0];
            e->rot_target.f[2] = -e->rot_target.f[2];
            e->pos.f[1] = -e->pos.f[1];
        }
        rot = &e->rot;
        pos = &e->pos;
        turn_k(&rot->f[0], e->rot_target.f[0], &e->rot_vel.f[0], e->accel * frame_scale_sq, e->max_speed * frame_scale_sq, 0.0f, 2);
        turn_k(&rot->f[1], e->rot_target.f[1], &e->rot_vel.f[1], e->accel * frame_scale_sq, e->max_speed * frame_scale_sq, 0.0f, 2);
        turn_k(&rot->f[2], e->rot_target.f[2], &e->rot_vel.f[2], e->accel * frame_scale_sq, e->max_speed * frame_scale_sq, 0.0f, 2);
        vendor_spring(&pos->f[0], &e->pos_vel.f[0], e->pos_target.f[0], e->accel, e->max_speed, 0.0f);
        vendor_spring(&pos->f[1], &e->pos_vel.f[1], e->pos_target.f[1], e->accel, e->max_speed, 0.0f);
        vendor_spring(&pos->f[2], &e->pos_vel.f[2], e->pos_target.f[2], e->accel, e->max_speed, 0.0f);
        if (!e->attached)
            attach_manipulator(moby, e->unkA0, e);
        FUN_L00_00259f50(&e->quat, rot);
        qcopy(&e->translation, pos);
        e->scale3.f[0] = e->scale;
        e->scale3.f[1] = e->scale;
        e->scale3.f[2] = e->scale;
    }
    clear_u64_value(&e->pos_target);
    clear_u64_value(&e->rot_target);
    e->scale = 1.0f;
}
extern struct HeroEase D_L00_0017A680_002054e8[] __asm__("D_L00_0017A680");
/* Runs FUN_L00_00205168 on each entry of D_L00_0017A680 (0x1550 bytes). */
void FUN_L00_002054e8(void) {
    struct HeroEase *p = D_L00_0017A680_002054e8;
    struct HeroEase *end = (struct HeroEase *)((u8 *)p + 0x1550);
    do {
        FUN_L00_00205168(p);
        p++;
    } while ((s32)p < (s32)end);
}
typedef struct {
    u8 unk0;
    u8 active;
    char pad[0x3E];
    char m40[0x20];
    char m60[0x30];
    char m90[0x12];
    s16 type;
    char pad2[8];
    f32 scale;
} Ent;
extern Ent D_L00_0017A680_c[] __asm__("D_L00_0017A680");
s32 FUN_L00_00205110_c(s32) __asm__("FUN_L00_00205110");

void FUN_L00_00205538(void) {
    s32 i;
    char *tbl = (char *)D_L00_0017A680_c;
    char *c = tbl + 0x90;
    char *b = tbl + 0x60;
    for (i = 0; i < 31; i++) {
        Ent *e = &D_L00_0017A680_c[i];
        if (e->active != 0) {
            s32 r = FUN_L00_00205110_c(e->type);
            if (r != 0)
                detach_manipulator(r, e);
        }
        clear_u64_value(D_L00_0017A680_c[i].m40);
        clear_u64_value(b);
        b += sizeof(Ent);
        clear_u64_value(c);
        c += sizeof(Ent);
        e->scale = 1.0f;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00205C18), where it is exact; names translated to the US level program. */

extern int D_L00_0015F5CC;
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

void FUN_L00_00205600(void) {
    struct Hero *g = &hero;
    if (g->rand_timer.fired == 0) {
        if (g->rand_timer.range == 0) {
            g->rand_timer.deadline = D_L00_0015F5CC + scale_game_frames(30);
        } else if (D_L00_0015F5CC > g->rand_timer.deadline) {
            int a, b;
            g->rand_timer.fired = 1;
            a = scale_game_frames(20);
            b = random_integer_below(g->rand_timer.range);
            g->rand_timer.deadline =
                D_L00_0015F5CC + a + b + random_integer_below(g->rand_timer.range);
        }
    }
}
typedef u32 Quad_56a8 __attribute__((mode(TI), aligned(16)));
extern u8 D_L00_0017A680_56a8[] __asm__("D_L00_0017A680");


extern Quad_56a8 D_L00_0017C040[];
extern Quad_56a8 D_L00_0017C0B0[];
extern Quad_56a8 D_L00_0017C120[];
extern s32 D_L00_0017C1C0[];
extern f32 D_L00_0017C190[];
void FUN_L00_002056a8(void) {
    u8 *e;

    {
        u8 *e = D_L00_0017A680_56a8;
        *(f32 *)(e + 0x8a8) = *(f32 *)(e + 0x278) * 0.25f;
        if (*(f32 *)(e + 0x8a8) > 0.14835298f)
            *(f32 *)(e + 0x8a8) = 0.14835298f;
        else if (*(f32 *)(e + 0x8a8) < -0.14835298f)
            *(f32 *)(e + 0x8a8) = -0.14835298f;
    }
    e = D_L00_0017A680_56a8;
    *(f32 *)(e + 0x8a4) = *(f32 *)(e + 0x274) / 2.8f;
    if (*(f32 *)(e + 0x8a4) > 0.29670596f)
        *(f32 *)(e + 0x8a4) = 0.29670596f;
    if (*(f32 *)(e + 0x8a4) < -0.17453292f)
        *(f32 *)(e + 0x8a4) = -0.17453292f;
    if ((u32)(*((u8 *)hero.moby + 0x53) - 1) < 2) {
        if (*(f32 *)(e + 0x8a8) > 0.034906585f)
            *(f32 *)(e + 0x8a8) = 0.034906585f;
        if (*(f32 *)(e + 0x8a8) < -0.034906585f)
            *(f32 *)(e + 0x8a8) = -0.034906585f;
        if (*(f32 *)(e + 0x8a4) > 0.12217305f)
            *(f32 *)(e + 0x8a4) = 0.12217305f;
        if (*(f32 *)(e + 0x8a4) < -0.12217305f)
            *(f32 *)(e + 0x8a4) = -0.12217305f;
    }
    FUN_L00_00205600();
    {
        if (hero.rand_timer.fired == 0)
            return;
        hero.rand_timer.fired++;
        if (hero.rand_timer.fired > 11)
            hero.rand_timer.fired = 0;
        if (hero.rand_timer.fired == 0) {
            s32 i;
            for (i = 0; i < 7; i++) {
                if (hero.manipulators[i].attached != 0)
                    detach_manipulator((s32)hero.moby, &hero.manipulators[i]);
            }
        } else {
            s32 i;
            s32 j;
            for (i = 0; i < 7; i++) {
                if (hero.manipulators[i].attached == 0) {
                    attach_manipulator((s32)hero.moby, D_L00_0017C1C0[i], &hero.manipulators[i]);
                    hero.manipulators[i].unk3 = 1;
                    qcopy(&hero.manipulators[i].quat, &D_L00_0017C040[i]);
                    qcopy(&hero.manipulators[i].scale3, &D_L00_0017C0B0[i]);
                    qcopy(&hero.manipulators[i].translation, &D_L00_0017C120[i]);
                }
            }
            for (j = 0; j < 7; j++)
                hero.manipulators[j].unkC = D_L00_0017C190[hero.rand_timer.fired];
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002059d8.s", FUN_L00_002059d8);
#else
typedef u32 Quad_59d8 __attribute__((mode(TI), aligned(16)));
extern u8 D_0015EDB3 __asm__("D_0015EDB3");
extern f32 D_0015EE18 __asm__("D_0015EE18");
extern u8 D_0013F350_2059d8[] __asm__("D_0013F350");
extern s32 D_0015F5CC_2059d8 __asm__("D_L00_0015F5CC");
extern u8 D_L00_0017C1E0_2059d8[] __asm__("D_L00_0017C1E0");
extern u8 D_L00_0017C220_2059d8[] __asm__("D_L00_0017C220");
extern u8 D_L00_0017C260_2059d8[] __asm__("D_L00_0017C260");
extern f32 D_L00_0017C2A0_2059d8[] __asm__("D_L00_0017C2A0");
extern void FUN_L00_0025b868(f32, f32, void *);
extern s32 FUN_L00_001fef20(s32);
extern f32 FUN_L00_00200240(s32);
extern s32 FUN_L00_00200250(f32);
extern f32 FUN_L00_001ff800(f32);
extern s32 FUN_L00_001fef98(void *);
extern s32 random_int_between_2059d8(s32, s32) __asm__("FUN_L00_00257b90");
extern void FUN_L00_0024f590(s32, s32, void *);
extern void FUN_L00_0024f608(s32, void *);
extern void transform_help_quad_2059d8(f32, void *, void *) __asm__("FUN_L00_001ff300");

void FUN_L00_002059d8(void) {
    u8 *g;
    u8 *moby;
    s32 i;

    if (D_0015EDB3 != 0)
        FUN_L00_0025b868(1.8f, 0.05f, &D_0015EE18);
    else
        FUN_L00_0025b868(1.0f, 0.05f, &D_0015EE18);

    if ((u8)(D_0013F350_2059d8[0x20A4] - 1) < 2)
        moby = *(u8 **)(D_0013F350_2059d8 + 0x2080);
    else {
        if (*(s16 *)(D_0013F350_2059d8 + 0x22D8) != 0)
            return;
        moby = *(u8 **)(D_0013F350_2059d8 + 0x1184);
    }
    if (moby == 0)
        return;
    g = (u8 *)&hero;
    *(u16 *)(moby + 0x34) |= 0x10;
    {
        s32 i, k, r, a, b, low, high;
        f32 f, c, t;
        a = 0x38;
        if (*(s32 *)(g + 0x22A8) == 1)
            a = 0x88;
        k = FUN_L00_001fef20(0x6E);
        f = FUN_L00_00200240(k);
        c = FUN_L00_001ff800(((f32)(D_0015F5CC_2059d8 % k) / f * 2.0f) * 3.1415927f + -3.1415927f);
        t = c * 24.0f;
        r = FUN_L00_00200250(t);
        b = FUN_L00_00200250(c * 48.0f);
        low = a + r + 0xC;
        high = b + 0xA0;
        b += 0x18;
        a = FUN_L00_00200250(t);
        i = a + 0x4C;
        if (*(s16 *)(g + 0x1EE) != 0) {
            s32 five, twenty, forty_five;
            f = 1.0f;
            five = FUN_L00_001fef20(5);
            twenty = FUN_L00_001fef20(20);
            forty_five = FUN_L00_001fef20(45);
            if (forty_five - five < *(s16 *)(g + 0x1EE)) {
                forty_five = FUN_L00_001fef20(45);
                f = FUN_L00_00200240(forty_five - *(s16 *)(g + 0x1EE)) /
                    FUN_L00_00200240(five);
            }
            if (*(s16 *)(g + 0x1EE) < twenty) {
                f = FUN_L00_00200240(*(s16 *)(g + 0x1EE)) /
                    FUN_L00_00200240(twenty);
            }
            low -= FUN_L00_00200250((f32)(r + 0xC) * f);
            high -= FUN_L00_00200250((f32)b * f);
            i -= FUN_L00_00200250((f32)(a + 0xC) * f);
            low += FUN_L00_00200250(f * 112.0f);
            high -= FUN_L00_00200250(f * 72.0f);
        }
        *(u32 *)(moby + 0x90) = ((0x80U << 24) | (i << 16)) | ((high << 8) | low);
    }
    if (g[0x20A4] != 0)
        return;
    if (FUN_L00_001fef98(g + 0xFFC) && moby[0x53] == 1) {
        s32 low, high, a;
        low = FUN_L00_001fef20(50);
        high = FUN_L00_001fef20(200);
        a = random_int_between_2059d8(low, high);
        *(s16 *)(g + 0xFFE) = 1;
        *(s16 *)(g + 0xFFC) = a;
    }
    if (*(s16 *)(g + 0xFFE) != 0) {
        *(s16 *)(g + 0xFFE) += 1;
        if (*(s16 *)(g + 0xFFE) > 21)
            *(s16 *)(g + 0xFFE) = 0;
        if (*(s16 *)(g + 0xFFE) == 0) {
            u8 *slot = g + 0xEF0;
            u8 *active = g + 0xEF1;
            for (i = 3; i >= 0; slot += 0x40, i--, active += 0x40)
                if (*active)
                    FUN_L00_0024f608((s32)moby, slot);
        } else {
            u32 i;
            u8 *slot = g + 0xEF0;
            u8 *active = g + 0xEF1;
            u8 *dst_a = g + 0xF00;
            u8 *dst_b = g + 0xF10;
            u8 *dst_c = g + 0xF20;
            Quad_59d8 *src_b = (Quad_59d8 *)D_L00_0017C220_2059d8;
            Quad_59d8 *src_c = (Quad_59d8 *)D_L00_0017C260_2059d8;
            Quad_59d8 *src_a = (Quad_59d8 *)D_L00_0017C1E0_2059d8;
            s32 *kind = (s32 *)(g + 0x17A8);
            for (i = 0; i < 4; i++, dst_c += 0x40, src_c++,
                 src_a++, src_b++, dst_a += 0x40, kind++,
                 slot += 0x40, active += 0x40, dst_b += 0x40) {
                if (*active == 0) {
                    FUN_L00_0024f590((s32)moby, *kind, slot);
                    active[2] = 1;
                    qcopy(dst_a, src_a);
                    qcopy(dst_b, src_b);
                    qcopy(dst_c, src_c);
                    transform_help_quad_2059d8(*(f32 *)(moby + 0x2C), dst_c, dst_c);
                }
            }
            {
                u8 *g = (u8 *)&hero;
                s32 i;
                f32 *yaw = (f32 *)(g + 0xEFC);
                for (i = 3; i >= 0; i--) {
                    *yaw = D_L00_0017C2A0_2059d8[*(s16 *)(g + 0xFFE)];
                    yaw = (f32 *)((u8 *)yaw + 0x40);
                }
            }
        }
    }
}
#endif
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_002064C0), where it is exact; names translated to the US level program. */

extern unsigned char D_0013D4D5[];
#include "rnc/data_labels.h"
extern void FUN_L00_00235dc0(void);
extern void FUN_L00_0023a218(void);
extern void FUN_L00_0023a2d0(void);
s32 queue_animation_update(s32 chan, s32 id, s32 fn, s32 d, s32 e, s32 c,
                           s32 b) __asm__("FUN_001ff308");

void FUN_L00_00205ea8(void) {
    struct Hero *g;
    if (D_0013D4D5[6] == 0) {
        *(int *)(((unsigned char *)&D_0013E533) + 0x2E65) = 0;
        return;
    }
    g = &hero;
    if (g->unk2044 < 20.0f && g->unk2040 != 0) {
        g->unk2048 = 1;
    } else {
        struct Hero *g3 = &hero;
        g3->unk2048 = 0;
    }
    {
        struct Hero *g2 = &hero;
        g2->unk2044 = 100000.0f;
        g2->unk2040 = 0;
        if (g2->unk2048 != 0) {
            queue_animation_update(7, 0x753A, (int)FUN_L00_00235dc0, (int)FUN_L00_0023a218,
                                   (int)FUN_L00_0023a2d0, (int)(&g2->unk2048), 1);
        }
    }
}
extern void FUN_L00_0028de68_205f60(int, char *, int, int, int) __asm__("FUN_L00_0028de68");
extern void add_vector_xyz_cf(int, char *, float *) __asm__("FUN_L00_0024f7c8");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float probe_ground_height(float *, int, float) __asm__("FUN_00213508");
extern void FUN_L00_00274cf8_205f60(float *, int, float) __asm__("FUN_L00_00274cf8");

void FUN_L00_00205f60(int a0, char *a1, int a2) {
    float v[4];
    if (a2) {
        struct Hero *g = &hero;
        int c = 0;
        if (((char *)g->items[1].moby)) {
            c = *(s16 *)(((char *)g->items[1].moby) + 0xA6) == 0xAD;
        }
        FUN_L00_0028de68_205f60(a0, a1, c, 0, ((int)g->moby));
    }
    if (current_level_index == 12) {
        struct Hero *g = &hero;
        if (g->unk12ED == 0 && ((int)g->ground_moby) == 0) {
            float k = 0.15f;
            add_vector_xyz_cf(((int)g->moby), a1 + 0x16, v);
            v[0] += fast_cos(g->motion.rot.f[2]) * k;
            v[1] += fast_sin(g->motion.rot.f[2]) * k;
            v[2] = probe_ground_height(v, 0, 0.5f) + 0.04f;
            FUN_L00_00274cf8_205f60(v, a1 == 0, g->motion.rot.f[2]);
        }
    }
}
extern int FUN_001f96f8_206080(int) __asm__("FUN_001f96f8");
extern int FUN_L00_00211870_206080(float) __asm__("FUN_L00_00211870");
extern void FUN_L00_00205f60_206080(int, char *, int) __asm__("FUN_L00_00205f60");

void FUN_L00_00206080(void) {
    struct Hero *g = &hero;
    int k;
    if (g->state.current == 2 && g->state_timer < FUN_001f96f8_206080(0xF)) {
        return;
    }
    k = g->unk12ED;
    if (g->state.current == 2 && g->state_timer == FUN_001f96f8_206080(0x16)) {
        FUN_L00_00205f60_206080(k, 0, 1);
    }
    switch (((unsigned char *)hero.moby)[0x53]) {
    case 3:
        if (hero.unkA9C) {
            return;
        }
        if (FUN_L00_00211870_206080(49.5f)) {
            FUN_L00_00205f60_206080(k, 0, 1);
        }
        if (FUN_L00_00211870_206080(17.0f)) {
            FUN_L00_00205f60_206080(k, (char *)1, 1);
        }
        break;
    case 4:
        if (hero.unkA9C) {
            return;
        }
        if (FUN_L00_00211870_206080(12.5f)) {
            FUN_L00_00205f60_206080(k, 0, 1);
        }
        if (FUN_L00_00211870_206080(1.0f)) {
            FUN_L00_00205f60_206080(k, (char *)1, 1);
        }
        break;
    }
}
#include "qcopy.h"
typedef float M4_2061f0[16] __attribute__((aligned(16)));
void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
int scale_game_frames(int) __asm__("FUN_001f96f8");
void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
void advance_moby_animation(void *) __asm__("FUN_0020d580");
void FUN_L00_00250df8(void *);
void FUN_L00_002061f0(void) {
    M4_2061f0 m;
    struct Hero *g = &hero;
    char *o = ((char *)g->unkA88);
    char *s;
    o[0x7F] = 0;
    s = ((char *)g->moby);
    if ((*(unsigned short *)(s + 0x34) ^ 1) & 1) {
        build_moby_bone_transform(s, 5, m);
        qcopy(o + 0x10, &m[12]);
        copy_matrix3x4(o + 0xC0, m);
        matrix_to_euler(m, o + 0x40);
        *(unsigned short *)(o + 0x34) &= 0xFFBE;
        blend_moby_animation(o, 0x81, 0, scale_game_frames(10));
        advance_moby_animation(o);
        FUN_L00_00250df8(o);
    } else {
        *(unsigned short *)(o + 0x34) |= 0x41;
    }
}
#include "rnc/overlay/quad.h"
extern char D_0013F350_002062b0[] __asm__("D_0013F350");
extern f32 D_0015EE18_macro __asm__("D_0015EE18") __attribute__((section(".sdata")));
extern volatile f32 frame_time_macro __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern f32 frame_scale_macro __asm__("D_0015ED60") __attribute__((section(".sdata")));
extern f32 D_0015ED60_gp __asm__("D_0015ED60_gp") __attribute__((sda));
extern f32 D_0015EE18_gp __attribute__((sda));
extern u8 D_0015EDB3_macro __asm__("D_0015EDB3") __attribute__((section(".sdata")));
extern s32 D_L00_0015F5CC __attribute__((section(".sdata")));
extern f32 D_L00_0017B85C_002062b0 __asm__("D_L00_0017B85C") __attribute__((section(".data")));
extern void FUN_L00_00232670(void);
extern void FUN_L00_00211670(void);
extern void FUN_L00_0020b960(void);
extern void FUN_L00_0020bed0_u(void) __asm__("FUN_L00_0020bed0");
extern void FUN_L00_00215b68(void);
extern void FUN_L00_00229b70(void);
extern void FUN_L00_00216f10(void);
extern void FUN_L00_002054e8(void);
extern void FUN_L00_00208a10(void);
extern void FUN_L00_00207c48(void);
extern void FUN_L00_00207a58(void);
extern void FUN_L00_002059d8(void);
extern void FUN_L00_00207d00(void);
extern void FUN_L00_00208b60(void);
extern void FUN_L00_002484e0(void *);
extern void FUN_L00_00257024(int);
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_0024f7c8(void *, s32, void *);
extern void build_moby_bone_transform(void *, s32, void *) __asm__("FUN_0020cca8");
extern void *create_moby(s32) __asm__("FUN_0020c4f8");
extern void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void blend_moby_animation_2062b0(void *, s32, s32, s32) __asm__("FUN_L00_00257880");
extern int FUN_L00_0028dbe8(void *, s32, void *);
extern void FUN_L00_00216de8(s32, s32);
extern s32 FUN_L00_00200298(f32, s32, s32);
extern f32 FUN_L00_001ff800(f32);
extern f32 FUN_L00_002000e8(f32, f32);
extern s32 FUN_L00_00200250(f32);
extern f32 FUN_L00_00200240(s32);
extern s32 FUN_L00_001fef20(s32);
extern s32 FUN_L00_001fef98(void *);
extern void FUN_L00_0025b868(f32, f32, void *);

void FUN_L00_002062b0(void) {
    char *g;
    char *moby;
    /* The two transform paths reuse the retail 0xA0-byte scratch region. */
    union {
        OvlQuad rotation[10];
        struct {
            OvlQuad reserved;
            OvlQuad transform[4];
            OvlQuad offset;
            OvlQuad inverse[4];
        } help;
    } scratch;
    f32 phase, scale, wave;
    s32 i, span, red, green, blue, alpha;
    s32 color, color2;

    FUN_L00_00232670();
    FUN_L00_00211670();
    FUN_L00_0020b960();
    g = (char *)&hero;
    if (*(u8 *)(g + 0x20a4) == 1) {
        FUN_L00_0020bed0_u();
        FUN_L00_00215b68();
    }
    FUN_L00_00229b70();
    if (*(u8 *)(g + 0x20a4) == 0) return;
    FUN_L00_00216f10();
    FUN_L00_002054e8();
    if (*(u8 *)(g + 0x20a4) == 0) return;
    FUN_L00_00208a10();
    FUN_L00_00207c48();
    FUN_L00_00207a58();
    if (*(u8 *)(g + 0x20a4) == 1) {
        D_L00_0017B85C_002062b0 = D_0015EE18_macro;
        FUN_L00_002059d8();
        if (*(char **)(g + 0x1624) == 0) {
            *(char **)(g + 0x1624) = create_moby(0x4b4);
            if (*(char **)(g + 0x1624)) {
                char *moby = *(char **)(g + 0x1624);
                *(u32 *)(moby + 0x90) = 0x801432d7;
                *(u32 *)(g + 0x2300) = 0x301432d7;
                moby = *(char **)(g + 0x1624);
                qcopy(moby + 0x10, g + 0x80);
                qcopy(moby + 0x40, g + 0x90);
                *(u16 *)(moby + 0x32) = 0x40;
                (*(char **)(g + 0x1624))[0x31] = 1;
                (*(char **)(g + 0x1624))[0x30] = 0;
                *(u16 *)(*(char **)(g + 0x1624) + 0x34) = 0;
                *(f32 *)(*(char **)(g + 0x1624) + 0x2c) *= 1.7f;
                *(u16 *)(*(char **)(g + 0x1624) + 0x34) |= 0x10;
                FUN_L00_00250df8(*(char **)(g + 0x1624));
            }
        }
        {
            char *g = (char *)&hero;
            if ((*(u16 *)(*(char **)(g + 0x2080) + 0x34) ^ 1) & 1) {
                FUN_L00_0024f7c8(*(char **)(g + 0x2080), 7, scratch.rotation);
                qcopy(g + 0x1d80, &scratch.rotation[0]);
                FUN_L00_00207d00();
                if (*(char **)(g + 0x1624) != 0) {
                    build_moby_bone_transform(*(char **)(g + 0x2080), 7, scratch.help.transform);
                    qcopy_nc(*(char **)(g + 0x1624) + 0x10, &scratch.help.transform[3]);
                    copy_matrix3x4(*(char **)(g + 0x1624) + 0xc0, scratch.help.transform);
                    scratch.help.offset = 0;
                    ((f32 *)&scratch.help.offset)[2] = 0.0257f;
                    transform_vector_by_basis(&scratch.help.offset, &scratch.help.offset, *(char **)(g + 0x1624) + 0xc0);
                    *(f32 *)(*(char **)(g + 0x1624) + 0x18) -= ((f32 *)&scratch.help.offset)[2];
                    FUN_L00_00250df8(*(char **)(g + 0x1624));
                    scale = 1.7f;
                    if (D_0015EDB3_macro) scale = 3.1f;
                    if (*(s16 *)(g + 0x22fc)) scale *= 1.4f;
                    FUN_L00_0025b868(*(f32 *)(*(char **)(*(char **)(g + 0x1624) + 0x24) + 0x24) * scale, 0.02f, *(char **)(g + 0x1624) + 0x2c);
                }
            }
        }
        {
            char *g = (char *)&hero;
            color = 0x281414c8;
            color2 = 0x1e00004b;
            {
                char *g = (char *)&hero;
                *(f32 *)(g + 0x1628) = FUN_L00_002000e8(*(f32 *)(g + 0x1628), frame_time_macro * 2.9670596f);
            }
            if (FUN_L00_001fef98(g + 0x22fc) == 0) {
                char *g = (char *)&hero;
                *(f32 *)(g + 0x1628) = FUN_L00_002000e8(*(f32 *)(g + 0x1628), frame_time_macro * 12.217304f);
                switch (*(s16 *)(g + 0x22fe)) {
                case 1: color = 0x2814c814; color2 = 0x1e004b00; break;
                case 2: color = 0x2814c8c8; color2 = 0x1e004b4b; break;
                case 3: color = 0x323232fa; color2 = 0x1e000078; break;
                case 4: color = 0x28f01414; color2 = 0x1e4b0000; break;
                }
            }
            {
                char *g = (char *)&hero;
                s32 tint;
                wave = FUN_L00_001ff800(*(f32 *)(g + 0x1628));
                tint = FUN_L00_00200298(wave * 0.5f + 0.5f, color, color2);
                if (*(char **)(g + 0x1624)) *(u32 *)(*(char **)(g + 0x1624) + 0x90) = FUN_L00_00200298(D_0015ED60_gp * 0.1f, *(u32 *)(*(char **)(g + 0x1624) + 0x90), tint);
                *(u32 *)(g + 0x2300) = FUN_L00_00200298(frame_scale_macro * 0.1f, *(u32 *)(g + 0x2300), tint);
                if (*(char **)(g + 0x1620) == 0) {
                    *(char **)(g + 0x1620) = create_moby(0x47a);
                    if (*(char **)(g + 0x1620)) {
                        char *moby = *(char **)(g + 0x1620);
                        qcopy(moby + 0x10, g + 0x80);
                        qcopy(moby + 0x40, g + 0x90);
                        *(u16 *)(moby + 0x32) = 0x40;
                        (*(char **)(g + 0x1620))[0x31] = 1;
                        (*(char **)(g + 0x1620))[0x30] = 0;
                        *(u16 *)(*(char **)(g + 0x1620) + 0x34) = 0;
                        FUN_L00_00250df8(*(char **)(g + 0x1620));
                    }
                }
                if (*(char **)(g + 0x1620)) {
                    build_moby_bone_transform(*(char **)(g + 0x2080), 5, scratch.rotation);
                    {
                        OvlQuad *position = &scratch.rotation[3];
                        char *moby = *(char **)(g + 0x1620);
                        qcopy_nc(moby + 0x10, position);
                        FUN_L00_00250df8(moby);
                    }
                    clear_u64_value((scratch.rotation + 4));
                    ((f32 *)(scratch.rotation + 4))[2] = -*(f32 *)(g + 0x98);
                    euler_rotation_matrix(scratch.help.inverse, (scratch.rotation + 4));
                    multiply_matrix(scratch.rotation, scratch.rotation, scratch.help.inverse);
                    copy_matrix3x4(*(char **)(g + 0x1620) + 0xc0, scratch.rotation);
                    *(u64 *)(*(char **)(g + 0x1620) + 0x38) = *(u64 *)(*(char **)(g + 0x2080) + 0x38);
                    if (*(s32 *)(g + 0x2084) == 0x4f) {
                        if (*(u8 *)(*(char **)(g + 0x1620) + 0x53) != 1)
                            blend_moby_animation_2062b0(*(char **)(g + 0x1620), 1, 0, FUN_L00_001fef20(10));
                    } else if (*(u8 *)(*(char **)(g + 0x1620) + 0x53) != 0) {
                        blend_moby_animation_2062b0(*(char **)(g + 0x1620), 0, 0, FUN_L00_001fef20(0x11));
                    }
                }
            }
        }
        {
            char *g = (char *)&hero;
            if (*(char **)(g + 0x22c0)) {
                FUN_L00_0028dbe8(*(char **)(g + 0x22c0) + 0x10, 0, *(char **)(g + 0x2080));
                *(s16 *)(g + 0x22fe) = *(u16 *)(g + 0x22c0);
                *(s16 *)(g + 0x22fc) = FUN_L00_001fef20(0x39);
            }
            *(u32 *)(g + 0x22c0) = 0;
        }
    }
    FUN_L00_002484e0(&hero.motion.pos);
    if (hero.vehicle_mode == 2) {
        char *g = (char *)&hero;
        if (D_0015EDB3_macro) *(f32 *)((char *)&D_L00_0017B85C_002062b0 + 0x210) = 1.4f * D_0015EE18_gp;
        FUN_L00_002059d8();
        FUN_L00_002061f0();
        g = (char *)&hero;
        if ((*(u16 *)(*(char **)(g + 0x2080) + 0x34) ^ 1) & 1) {
            FUN_L00_0024f7c8(*(char **)(g + 0x2080), 6, scratch.rotation);
            /* Retail uses &hero.motion.pos + 0x1d00, hence hero + 0x1d80. */
            qcopy(g + 0x1d80, &scratch.rotation[0]);
            FUN_L00_00207d00();
        }
    }
    if (hero.vehicle_mode == 3) {
        s32 color;
        /* D_0015EDB1 is the byte two bytes before the checked D_0015EDB3 base. */
        if (*((u8 *)&D_0015EDB3_macro - 2)) *(f32 *)((char *)&D_L00_0017B85C_002062b0 + 0x370) = 1.9f;
        {
            char *g = (char *)&hero;
            alpha = 0x80;
            span = FUN_L00_001fef20(0xaa);
            if (*(s16 *)(g + 0x22e0) == 2) span = FUN_L00_001fef20(0x32);
            else if (*(s16 *)(g + 0x22e0) == 1) span = FUN_L00_001fef20(0x5a);
            phase = FUN_L00_00200240(span);
            wave = FUN_L00_001ff800((((f32)(D_L00_0015F5CC % span) / phase) * 2.0f) * 3.1415927f + -3.1415927f);
        }
        red = FUN_L00_00200250(wave * 20.0f) + 0x32;
        green = FUN_L00_00200250(wave * 70.0f) + 0xb4;
        blue = FUN_L00_00200250(wave * 10.0f) + 0x14;
        {
            char *g = (char *)&hero;
            {
                s32 span = red < 0x100;
                s32 i = green < 0x100;
                green = i ? green : 0xff;
                *(u16 *)(*(char **)(g + 0x2080) + 0x34) |= 0x10;
                red = span ? red : 0xff;
            }
            if (*(s16 *)(g + 0x22e0) == 2)
                color = ((u32)alpha << 24) | ((u32)blue << 16) | ((u32)(red / 2) << 8) | (u32)green;
            else
                color = ((u32)alpha << 24) | ((u32)blue << 16) | ((u32)green << 8) | (u32)red;
        }
        {
            char *g;
            {
                char *g = (char *)&hero;
                *(u32 *)(*(char **)(g + 0x2080) + 0x90) = FUN_L00_00200298(0.1f, *(u32 *)(*(char **)(g + 0x2080) + 0x90), color);
            }
            g = (char *)&hero;
            for (i = 0; i < 3; i++)
                FUN_L00_0024f7c8(hero.moby, i, g + 0x1d50 + i * 0x10);
            g = (char *)&hero;
            moby = *(char **)(g + 0x2080);
            /* Retail snapshots these channels before the alpha helper calls. */
            blue = *(u8 *)(moby + 0x92);
            green = *(u8 *)(moby + 0x91);
            red = *(u8 *)(moby + 0x90);
            alpha = 0x30;
            if (*(s16 *)(g + 0x22de))
                alpha = FUN_L00_00200250(FUN_L00_00200240(*(s16 *)(g + 0x22de)) / FUN_L00_00200240(0x12) * (f32)alpha);
            *(u32 *)(g + 0x22e4) = (alpha << 24) | ((u32)blue << 16) | ((u32)green << 8) | (u32)red;
            FUN_L00_00207d00();
            *(s16 *)(g + 0x22e0) = 0;
            if (*(u8 *)(g + 0x20a4) == 3) {
                if (*(s32 *)(g + 0x20b8) != 0 && *(s32 *)(g + 0x20b8) != 0x1f) {
                    if (*(s16 *)(g + 0x22de) == 0) {
                        FUN_L00_00216de8(0x19, 0);
                        *(s16 *)(g + 0x22de) = 0x12;
                        *(s16 *)(g + 0x22dc) = 1;
                    }
                } else {
                    char *g = (char *)&hero;
                    *(s32 *)(g + 0x20b8) = 0;
                    if (*(u8 *)(g + 0x12e7) && *(s32 *)(g + 0x300) && *(u8 *)(D_0013F350_002062b0 - 0x1e74) && *(s16 *)(g + 0x22de) == 0) {
                        FUN_L00_00216de8(0x19, 0);
                        *(s16 *)(g + 0x22de) = 0x12;
                        *(s16 *)(g + 0x22dc) = 1;
                    }
                }
            }
        }
    }
    FUN_L00_00208b60();
    FUN_L00_00257024((s32)hero.moby);
}
#include "rnc/audio/voice_pool.h"

extern void release_voice_slot(s32) __asm__("FUN_0022d798");

/* Stops the sounds the hero still owns in its 8 sound slots and clears them. */
void FUN_L00_00206c08(void) {
    s32 *slot;
    s32 i;
    s32 off;
    s32 voice;
    struct Moby *moby;
    s32 active;
    struct Hero *h;
    struct Moby **mobys;

    h = &hero;
    mobys = h->unk2238;
    slot = &h->unk2218;
    off = 0;
    for (i = 7; i >= 0; i--) {
        moby = *(struct Moby **)(off + (s32)mobys);
        voice = *slot;
        if (moby != 0) {
            if (voice == -1 || voice_pool.voices[voice].owner != (VoiceMoby *)moby) {
                goto clear;
            }
            active = voice_pool.voices[voice].state;
        } else {
            if (voice == -1 || voice_pool.voices[voice].owner != (VoiceMoby *)h->moby) {
                goto clear;
            }
            active = voice_pool.voices[voice].state;
        }
        if (active) {
            release_voice_slot(voice);
        }
    clear:
        *slot = -1;
        slot++;
        /* Fakematch: the empty block makes retail step slot before off. */
        do {
        } while (0);
        off += 4;
    }
    i = 0;
}
#include "sda.h"
#include "rnc/overlay/quad.h"
typedef struct {
    u8 p[8];
    s32 x8;
    u8 pc[0x40];
} E00206cf8;
typedef struct {
    u8 p[0x280];
    u16 x280;
    u8 p282[0x13E];
    u16 x3c0;
} G00206cf8;
extern G00206cf8 help_stats __asm__("D_00141968");
extern s32 D_L00_0015F5CC_00206cf8 __asm__("D_L00_0015F5CC");
extern E00206cf8 D_L00_00179AC0_00206cf8[] __asm__("D_L00_00179AC0");
extern u8 item_available[] __asm__("D_0013D4C0") NOT_SDA;
s32 FUN_L00_0020d568_00206cf8(void) __asm__("FUN_L00_0020d568");
s32 give_reward(s32, s32) __asm__("FUN_L00_00203908");
extern u16 D_001418F0 __attribute__((section(".data")));
void FUN_L00_00206cf8(void) {
    s32 i, n;
    if (help_stats.x3c0 == 0 && item_available[4] != 0 &&
        FUN_L00_0020d568_00206cf8() != 0) {
        give_reward(0x4E2E, 0x78);
    }
    if (D_L00_0015F5CC_00206cf8 <= 0)
        return;
    if (D_L00_0015F5CC_00206cf8 & 0x7F)
        return;
    if (help_stats.x280 != 0)
        return;
    if (D_001418F0 != 0)
        return;
    n = 0;
    for (i = 0; i < 0x25; i++) {
        if (D_L00_00179AC0_00206cf8[i].x8 == 0 && i != 8 && i != 0x18 &&
            item_available[i] != 0)
            n++;
    }
    if (n >= 9)
        give_reward(0x4E26, 0x50);
}
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00207418), where it is exact; names translated to the US level program. */

extern int alternate_animation_sequence __asm__("D_0015EE20") __attribute__((section(".sdata")));
extern int D_0015EEA0 __attribute__((section(".sdata")));
extern s32 scale_game_frames_c2(s32) __asm__("func_001F96F8");
extern unsigned char D_0015EDB0[] __attribute__((section(".sdata")));
extern unsigned char D_0015EDB4[4] __attribute__((section(".sdata")));
extern unsigned char D_0015EDC0[] __attribute__((section(".sdata")));
extern void FUN_L00_00263d40(int, int);

void FUN_L00_00206e00(void) {
    char *g;
    int s;
    int i;
    if (D_0015EEA0 == 0 && alternate_animation_sequence == 0) return;
    g = (char *)&hero;
    s = 0;
    if (*(int *)(g + 0x198) == scale_game_frames_c2(0xF)) {
        if (*(int *)(g + 0x208C) == 4) {
            int b = *(int *)(g + 0x2084);
            if (b == 0xB) {
                int c = *(int *)(g + 0x450);
                if (c == 0) {
                    s = 1;
                    if (D_0015EDB4[0]) s = 2;
                } else if (c == 1) {
                    s = 2;
                    if (D_0015EDB4[0]) s = 1;
                } else if (c == 3) {
                    s = 3;
                }
            } else if (b == 0x11) {
                s = 9;
            } else if (b == 0xA) {
                s = 0xA;
            } else if (b == 0xE) {
                s = 5;
            }
        } else if (*(int *)(g + 0x208C) == 6) {
            int b = *(int *)(g + 0x2084);
            if (b == 0x14) {
                s = 6;
            } else if (b == 0x13) {
                s = *(int *)(g + 0xA60) + 0xC;
            } else if (b == 0x15) {
                s = 4;
            }
        } else if (*(int *)(g + 0x2084) == 0x22) {
            s = 0xB;
        }
    } else if (*(int *)(g + 0x198) == scale_game_frames_c2(0x3C)) {
        if (*(int *)(g + 0x2084) == 4) {
            s = 7;
        } else if (*(int *)(g + 0x2084) == 8) {
            s = 8;
        }
    }
    if (s == 0) return;
    {
    char *q = (char *)&hero;
    unsigned char *b0 = (unsigned char *)q + 0x21C4;
    b0[*(int *)(q + 0x21D4)] = s;
    *(int *)(q + 0x21D4) += 1;
    for (i = 0; i < 12; i++) {
        char *g2 = (char *)&hero;
        unsigned char *b = (unsigned char *)g2 + 0x21C4;
        int n = 0;
        int j;
        int k;
        while (D_L00_00179700[n + i * 16] != 0) n++;
        if (n == 0) continue;
        k = *(int *)(g2 + 0x21D4) - n;
        if (k < 0) k += 16;
        for (j = 0; j < n; j++) {
            if (*(unsigned char *)(k + (int)b) != D_L00_00179700[j + i * 16]) {
                j = -1;
                break;
            }
            k++;
            if (k >= 16) k -= 16;
        }
        if (j == n) {
            int t = D_0015EDB0[i] == 0;
            D_0015EDC0[i] = 1;
            D_0015EDB0[i] = t;
            FUN_L00_00263d40(t ? 0x4FBE : 0x4FBF, -1);
            break;
        }
    }
    }
    {
        char *r = (char *)&hero;
        if (*(int *)(r + 0x21D4) >= 16) *(int *)(r + 0x21D4) = 0;
    }
}
int scale_game_frames(int) __asm__("FUN_001f96f8");
void fade_to_black(int) __asm__("FUN_001f4a58");
void FUN_L00_002062b0(void);
void FUN_L00_00232670(void);
void hero_update(void) __asm__("FUN_L00_00211670");
void FUN_L00_0020b960(void);
void FUN_L00_0020bed0_u(void) __asm__("FUN_L00_0020bed0");
void find_grind_rail(void) __asm__("FUN_L00_0020cf58");
void FUN_L00_0020d330(void);
void hero_update_transitions(void) __asm__("FUN_L00_00229b70");
void FUN_L00_00216f10(void);
void FUN_L00_0020b1d8_u(void) __asm__("FUN_L00_0020b1d8");
void FUN_L00_0020a318(void);
void FUN_L00_0020a540_u(void) __asm__("FUN_L00_0020a540");
void FUN_L00_0020ac98_u(void) __asm__("FUN_L00_0020ac98");
void FUN_L00_0020a9e8_u(void) __asm__("FUN_L00_0020a9e8");
void FUN_L00_002056a8(void);
void FUN_L00_002059d8(void);
void FUN_L00_00214fe8(void);
void FUN_L00_002054e8(void);
void FUN_L00_00208820(void);
void FUN_L00_00209240(int);
void FUN_L00_00210928(void);
void begin_draw_frame(void) __asm__("FUN_001f7978");
void FUN_L00_00207c48(void);
void FUN_L00_00207a58(void);
void FUN_L00_00206cf8(void);
void FUN_L00_00205ea8(void);
void FUN_L00_00206080(void);
void FUN_L00_00206e00_u(void) __asm__("FUN_L00_00206e00");
void noop_main_callback(int) __asm__("FUN_001e93e8");
void FUN_L00_002484e0(void *);
void FUN_L00_00257024(int);
void FUN_L00_002076a8(void);
void FUN_L00_00208b60_u(void) __asm__("FUN_L00_00208b60");
void FUN_L00_002070d0(void) {
    struct Hero *g = &hero;
    if (g->motion.pos.f[0] < 2.0f || g->motion.pos.f[1] < 2.0f ||
        1022.0f < g->motion.pos.f[0] || 1022.0f < g->motion.pos.f[1]) {
        fade_to_black(scale_game_frames(0x10));
        {
            struct Hero *g2 = &hero;
            g2->unk20B1 = 1;
        }
        return;
    }
    g->unk1EB2 = 0;
    g->unk1EB0 = 0;
    if (g->vehicle_mode) {
        FUN_L00_002062b0();
        return;
    }
    FUN_L00_00232670();
    hero_update();
    FUN_L00_0020b960();
    FUN_L00_0020bed0_u();
    find_grind_rail();
    FUN_L00_0020d330();
    hero_update_transitions();
    if (g->vehicle_mode)
        return;
    FUN_L00_00216f10();
    FUN_L00_0020b1d8_u();
    FUN_L00_0020a318();
    FUN_L00_0020a540_u();
    FUN_L00_0020ac98_u();
    FUN_L00_0020a9e8_u();
    FUN_L00_002056a8();
    FUN_L00_002059d8();
    FUN_L00_00214fe8();
    FUN_L00_002054e8();
    FUN_L00_00208820();
    FUN_L00_00209240(0);
    FUN_L00_00210928();
    begin_draw_frame();
    FUN_L00_00207c48();
    FUN_L00_00207a58();
    FUN_L00_00206cf8();
    FUN_L00_00205ea8();
    FUN_L00_00206080();
    FUN_L00_00206e00_u();
    noop_main_callback((int)g->moby);
    if (g->state.control_mode != 0x16 && g->state.current != 0x32)
        FUN_L00_002484e0(&g->motion.pos);
    {
        struct Hero *g2 = &hero;
        FUN_L00_00257024((int)g2->moby);
    }
    FUN_L00_002076a8();
    FUN_L00_00208b60_u();
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_002078E0), where it is exact; names translated to the US level program. */


void FUN_L00_002072c8(void) {
    int i, j;
    char *g = ((char *)&hero);
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 2; j++) {
            char *e = g + i * 0x50;
            int p = *(int *)(e + 0x1090);
            if (j == 1) {
                p = *(int *)(e + 0x1094);
            }
            if (p != 0) {
                *(long *)(p + 0x38) = *(long *)(*(int *)(g + 0x2080) + 0x38);
            }
        }
    }
}
/* Tint the effect from the moby's colour bytes, then set the four shape floats from its vector sum. */
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00207948), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} Vx __attribute__((aligned(16)));

extern char D_L00_001801D0[] __attribute__((section(".data")));
extern void FUN_0020d510(void *, void *);
extern void pack_render_command_fields(void *, s32, s32, s32, s32) __asm__("FUN_0020d4f0");
extern void FUN_L00_002503c0(void *, void *);

void FUN_L00_00207330(void *a, char *m) {
    Vx v;
    float *f;
    float s;
    unsigned char *q = m + 0x38;
    int c = q[4] + q[5] + q[6];
    pack_render_command_fields(a, c * 4 / 10 + ((c * 3 / 10) << 8), 12, 12, 0);
    FUN_0020d510(m, D_L00_001801D0);
    FUN_L00_002503c0(m, &v);
    f = (float *)(D_L00_001801D0 - 0x310);
    s = v.x + v.y + v.z;
    f[0xC0] = s * 0.5f;
    f[0xC1] = s * 0.4f;
    f[0xC2] = s * 0.0f;
    f[0xC3] = -0.3f;
}
extern int D_15F5CC_207430 __asm__("D_L00_0015F5CC");
extern unsigned char D_0015EDB3_207430 __asm__("D_0015EDB3") __attribute__((section(".data")));
extern float D_0015EE18 __asm__("D_0015EE18");
extern unsigned char D_17C380_207430[] __asm__("D_L00_0017C380") __attribute__((section(".data")));
extern unsigned char D_17C3C0_207430[] __asm__("D_L00_0017C3C0") __attribute__((section(".data")));
extern unsigned char D_17C400_207430[] __asm__("D_L00_0017C400") __attribute__((section(".data")));
extern int fr_207430(int) __asm__("FUN_001f96f8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int f2i_207430(float) __asm__("FUN_001fa6d0");
extern void f4f7c8_207430(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float fabs_207430(float) __asm__("FUN_001f99c0");
extern void FUN_L00_00208038_c(void *, int, float, float) __asm__("FUN_L00_00208038");
extern void fcb10_207430(void *, int, void *) __asm__("FUN_0020cb10");

extern float D_0015EE18_gp __attribute__((sda));
void FUN_L00_00207430(char *o) {
    union {
        OvlQuad q;
        float f[4];
    } v;
    int k, r, g;
    float f, c, s;
    unsigned char *t;

    *(unsigned short *)(o + 0x34) |= 0x10;
    k = fr_207430(100);
    f = convert_integer_to_float(k);
    c = fast_sin((float)(D_15F5CC_207430 % k) / f * 2.0f * 3.1415927f + -3.1415927f);
    s = c * 24.0f;
    r = f2i_207430(s);
    g = f2i_207430(c * 52.0f);
    {
        int w = ((f2i_207430(s) + 0x4C) << 16) | 0x80000000;
        w |= (g + 0xA2) << 8;
        w |= r + 0x44;
        *(int *)(o + 0x90) = w;
    }
    if (*(short *)(o + 0xA6) == 0x1A3) {
        f4f7c8_207430(o, 6, &v);
        v.f[2] -= 0.07f;
        if (2.0f < fabs_207430(*(float *)(o + 0x18) - v.f[2])) {
            FUN_L00_00208038_c(&v, 0x200000C0, 0.25f, 0.15f);
        }
    } else {
        f4f7c8_207430(o, 1, &v);
        v.f[2] -= 0.00700000022f;
        FUN_L00_00208038_c(&v, 0x200000C0, 0.0469999984f, 0.15f);
    }
    t = D_17C380_207430;
    if (*(short *)(o + 0xA6) == 0x1A3) {
        t = D_17C3C0_207430;
    } else if (*(short *)(o + 0xA6) == 0x555) {
        t = D_17C400_207430;
    }
    if (*(unsigned char *)0x15EDB3 && t[1] == 0) {
        float x;
        if (*(short *)(o + 0xA6) == 0x1A3) {
            fcb10_207430(o, 1, t);
            x = D_0015EE18_gp;
        } else {
            fcb10_207430(o, 2, t);
            x = D_0015EE18;
        }
        *(float *)(t + 0x20) = x;
        *(float *)(t + 0x24) = x;
        *(float *)(t + 0x28) = x;
    }
}
extern void glow_n(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void get_moby_color(void *, int *, int *, int *) __asm__("FUN_L00_00250320");
extern u8 item_text_variant[] __asm__("D_0013E520");

void FUN_L00_002076a8(void) {
    struct Hero *player = &hero;
    int blink;

    if (player->unk22F4 != 0) {
        int color = player->unk22F8;
        int red = color & 0xff;
        int blue = (color >> 16) & 0xff;
        int green = (color >> 8) & 0xff;
        red -= 7;
        green -= 7;
        blue -= 7;
        if (red < 0) red = 0;
        if (green < 0) green = 0;
        if (blue < 0) blue = 0;
        {
            int alpha = (unsigned int)color >> 24;
            glow_n(((void *)player->moby), red, green, blue);
            player->unk22F8 = alpha << 24 | blue << 16 | green << 8 | red;
        }
    }

    blink = 0;
    if (hero.state.current == 0x80 || hero.state.current == 0x82) {
        blink = hero.state_timer < scale_game_frames(0x1e);
    }
    if ((current_level_index == 0xf || current_level_index == 0x11) &&
        hero.state.current == 0x76 && hero.state_timer < scale_game_frames(0x14)) {
        blink = 1;
    }
    if (blink) {
        int red, green, blue;
        get_moby_color(((void *)hero.moby), &red, &green, &blue);
        if (hero.state_timer % 4 < 3) {
            red = 0;
            green = 0;
            blue = 0;
        } else {
            red = 0x90;
            green = 0x90;
            blue = 0xf0;
        }
        glow_n(((void *)hero.moby), red, green, blue);
        FUN_L00_002072c8();
    }
    if (hero.items[0].item_id >= 0 &&
        item_text_variant[hero.items[0].item_id] != 0 &&
        ((int)hero.items[0].moby) != 0) {
        FUN_L00_00207330(((void *)hero.items[0].moby), ((char *)hero.moby));
    }
}
typedef unsigned int u128_2078a8 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2078a8 q;
    float f[4];
} Vec4_2078a8;
typedef struct {
    u8 p0[0x6A8];
    float f6A8;
    float f6AC;
    u8 p1[0x86C - 0x6B0];
    char *o86C;
    u8 p2[0x8BC - 0x870];
    s16 h8BC;
    u8 p3[0x2080 - 0x8BE];
    char *w2080;
    int w2084;
} S13F350_2078a8;
extern float D_0015ED6C;
void FUN_L00_00262b00_002078a8(char *, float *, float *, float, float) __asm__("FUN_L00_00262b00");
void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
void rotation_matrix(void *, void *) __asm__("FUN_001fa050");
void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern char D_L00_0017A6C0[] __attribute__((section(".data")));
void FUN_L00_002078a8(void);
void FUN_L00_002078a8(void) {
    struct Hero *P = &hero;
    Vec4_2078a8 v;
    float m[16] __attribute__((aligned(16)));
    if (P->state.current == 0x7F) {
        FUN_L00_00262b00_002078a8(P->moby, &P->unk6A8, &P->unk6AC, 0.3f,
                                  D_0015ED6C * 6.981317f);
    }
    if (P->state.current == 0x6B && P->unk8BC == 0) {
        FUN_L00_00262b00_002078a8(P->moby, &P->unk6A8, &P->unk6AC, 1.1f,
                                  D_0015ED6C * 5.2359877f);
        if (P->unk86C) {
            char *s = P->moby;
            qcopy(((char *)P->unk86C) + 0x10, s + 0x10);
            v.q = 0;
            v.f[2] = 0.21f;
            transform_vector_by_basis(&v, &v, s + 0xC0);
            rotation_matrix(m, D_L00_0017A6C0);
            transform_vector(&v, &v, m);
            add_vector_xyz(((char *)P->unk86C) + 0x10, ((char *)P->unk86C) + 0x10, &v);
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00208000), where it is exact; names translated to the US level program. */

extern void FUN_L00_00262b80(int, char *, char *, float, float, float);

void FUN_L00_002079e8(void);
void FUN_L00_002079e8(void) {
    struct Hero *g = &hero;
    if (g->state.current == 0x7F) {
        FUN_L00_00262b80(((int)g->moby), &g->unk6B0, &g->unk6B4, 0.1745329201221466f,
                         frame_time * 1.5707963705062866f, frame_time * 2.094395160675049f);
    }
}
void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
void gravity_set_z(void *, void *, float) __asm__("FUN_L00_00233b20");

extern float D_L00_001797D8[] __attribute__((section(".data")));
void FUN_L00_00207a58(void) {
    struct Hero *G = &hero;
    OvlQuad t[1];
    float s, f;
    int i, j;
    unsigned char *o;
    if (G->unk22DC) {
        s = D_L00_001797D8[++G->unk22DC];
        if (s == -1.0f) {
            G->unk22DC = 0;
            return;
        }
        scale_vector_xyz(((unsigned char *)G->moby) + 0xC0,
                            ((unsigned char *)G->moby) + 0xC0, s);
        scale_vector_xyz(((unsigned char *)G->moby) + 0xD0,
                            ((unsigned char *)G->moby) + 0xD0, s);
        if (G->vehicle_mode == 0) {

            for (i = 0; i < 7; i++) {
                for (j = 0; j < 3; j++) {
                    struct Hero *h = &hero;
                    o = *(unsigned char **)&h->items[i].moby;
                    if (j == 1)
                        o = *(unsigned char **)&h->items[i].moby2;
                    f = s;
                    if (i == 0 && h->vehicle_mode == 0 && 1.0f < s)
                        f = 1.0f;
                    if (o) {
                        scale_vector_xyz(o + 0xC0, o + 0xC0, f);
                        scale_vector_xyz(o + 0xD0, o + 0xD0, f);
                        subtract_vector_xyz(t, o + 0x10, &hero.motion.unkD0);
                        gravity_set_z(t, t, 0.0f);
                        subtract_vector_xyz(o + 0x10, o + 0x10, t);
                        scale_vector_xyz(t, t, s);
                        add_vector_xyz(o + 0x10, o + 0x10, t);
                    }
                }
            }
        }
    }
}
extern int D_L00_0015F680 __attribute__((sda));
extern int D_L00_0015F684 __attribute__((sda));
extern float D_L00_0015F67C __attribute__((sda));
extern float D_L00_0015F3FC;
float FUN_00213ed8(float *, float, float);
void FUN_L00_00207c48(void) {
    if (D_L00_0015F680 == 0)
        return;
    if (D_L00_0015F684 == 0) {
        FUN_00213ed8(&D_L00_0015F3FC, 1.0f, D_L00_0015F67C);
        if (D_L00_0015F3FC == 1.0f)
            D_L00_0015F684 = 1;
    } else if (D_L00_0015F684 == 1) {
        D_L00_0015F684 = 2;
    } else if (D_L00_0015F684 == 2) {
        FUN_00213ed8(&D_L00_0015F3FC, 0.0f, D_L00_0015F67C);
        if (D_L00_0015F3FC == 0.0f)
            D_L00_0015F680 = 0;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00208318), where it is exact; names translated to the US level program. */

extern void FUN_L00_00207d40_u(void) __asm__("FUN_L00_00207d40");
void enqueue_callback_list_1(s32 arg0, s32 arg1) __asm__("FUN_001f4600");

/* Once (flag at ((char *)&hero)+0x1EB2) queues FUN_L00_00207d40 with the object at +0x2080. */
void FUN_L00_00207d00(void) {
    struct Hero *g = &hero;
    if (g->unk1EB2 == 0) {
        g->unk1EB2 = 1;
        enqueue_callback_list_1(FUN_L00_00207d40_u, ((int)g->moby));
    }
}
typedef struct {
    f32 f[4];
} V_207d40;
typedef struct {
    u8 p0[0x162C];
    f32 f162C;
    u8 p1[0x1D50 - 0x1630];
    V_207d40 v1D50[3];
    V_207d40 v1D80;
    V_207d40 v1D90[4];
    V_207d40 v1DD0[8];
    s32 i1E50[8];
    f32 f1E70[8];
    f32 f1E90[8];
    s16 h1EB0;
    u8 p2[0x20A4 - 0x1EB2];
    u8 b20A4;
    u8 p3[0x22D8 - 0x20A5];
    s16 h22D8;
    u8 p4[0x22E4 - 0x22DA];
    s32 i22E4;
    u8 p5[0x22FC - 0x22E8];
    s16 h22FC;
    u8 p6[2];
    s32 i2300;
} P_207d40;
extern P_207d40 D_0013F350_207d40 __asm__("D_0013F350") __attribute__((section(".data")));
extern void FUN_L00_00263618_207d40(void *, s32, f32, f32) __asm__("FUN_L00_00263618");
extern int FUN_L00_0020d498_207d40(int) __asm__("FUN_L00_0020d498");
extern void FUN_00213ed8_207d40(void *, f32, f32) __asm__("FUN_00213ed8");
#define P D_0013F350_207d40

void FUN_L00_00207d40(void) {
    f32 k;
    int i;
    int st;
    int n;
    int mode;
    V_207d40 *v;

    mode = D_L00_0015F5C4;
    if (mode == 2 || mode == 6) {
        for (i = 0; i < P.h1EB0; i++)
            FUN_L00_00263618_207d40(&P.v1DD0[i], P.i1E50[i], P.f1E70[i], P.f1E90[i]);
        return;
    }
    st = P.b20A4;
    if (st == 3) {
        int j;
        n = P.i22E4;
        v = P.v1D50;
        for (j = 2; j >= 0; j--) {
            FUN_L00_00263618_207d40(v, n, 0.2f, 0.08f);
            v++;
        }
        return;
    }
    if (st == 0) {
        if ((FUN_L00_0020d498_207d40(3) == 3 || FUN_L00_0020d498_207d40(3) == 2) && P.h22D8 == 0) {
            P.v1D80.f[2] += 0.015f;
            FUN_L00_00263618_207d40(&P.v1D80, 0x280000C0, 0.057f, 0.08f);
            P.v1D80.f[2] -= 0.015f;
        }
        for (i = 0; i < P.h1EB0; i++)
            FUN_L00_00263618_207d40(&P.v1DD0[i], P.i1E50[i], P.f1E70[i], P.f1E90[i]);
        return;
    }
    if (st == 2) {
        P.v1D80.f[2] -= 0.07f;
        FUN_L00_00263618_207d40(&P.v1D80, 0x300000C0, 0.25f, 0.08f);
        P.v1D80.f[2] += 0.07f;
        return;
    }
    if (st == 1) {
        k = 0.07f;
        if (P.h22FC)
            k = 0.101500004529953f;
        if (*(u8 *)0x15EDB3) {
            P.v1D80.f[2] -= 0.01f;
            k *= 1.7f;
        } else {
            P.v1D80.f[2] -= 0.02f;
        }
        FUN_00213ed8_207d40(&P.f162C, k, frame_scale * 0.003f);
        FUN_L00_00263618_207d40(&P.v1D80, P.i2300, P.f162C, 0.08f);
    }
}
#include "eetypes.h"
typedef struct {
    char pad[0x1dd0];
    u128 pos[8];
    int b[8];
    float x[8];
    float y[8];
    short n;
} G_00208038;
extern G_00208038 D_0013F350_00208038 __asm__("D_0013F350");
void FUN_L00_00207d00_00208038(void) __asm__("FUN_L00_00207d00");
/* Appends v, b, x and y to the list in ((char *)&hero) and calls FUN_L00_00207d00. */
void FUN_L00_00208038(u128 *v, int b, float x, float y) {
    G_00208038 *g = &D_0013F350_00208038;
    short n = g->n;
    u128 *pos = g->pos;
    qcopy(&pos[n], v);
    g->b[n] = b;
    g->x[g->n] = x;
    g->y[g->n] = y;
    FUN_L00_00207d00_00208038();
    g->n++;
}
#include "rnc/gameplay/entities/moby.h"

typedef int u128_2080b0 __attribute__((mode(TI)));
typedef union { u128_2080b0 q; float f[4]; } Vec4_2080b0 __attribute__((aligned(16)));
extern void moby_point_position_2080b0(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void queue_glow_sprite_2080b0(void *vec, int a, float f0, float f1) __asm__("FUN_L00_00208038");
extern float fast_add_rotations_2080b0(float, float) __asm__("FUN_001fa580");
extern float fast_sin_2080b0(float) __asm__("FUN_001f9de0");
extern int fast_tween_color_2080b0(int, int, float) __asm__("FUN_001fa6e0");
extern void transform_vector_by_basis_2080b0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_vector_xyz_2080b0(void *, void *, void *) __asm__("FUN_001f9a10");
extern int truncate_float_to_s32_2080b0(float) __asm__("FUN_001fa6d0");
extern int scale_game_frames_2080b0(int) __asm__("FUN_001f96f8");
extern float D_0015ED6C_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));

// Hero gadget glow: pulses the colour of the hero's gadget moby (hero state 13, 16, 17, 19, 20) and
// queues its glow sprites. c and g are shared by cases 17 and 20 (one pseudo each, retail's $s0/$s1).
void FUN_L00_002080b0(void) {
    Vec4_2080b0 v;
    struct Moby *m;
    int i;
    int c, g;

    if (hero.items[2].item_id == 5) {
        struct Moby *w = hero.items[2].moby;
        if (w != 0 && ((w->flags ^ 1) & 1)) {
            for (i = 0; i < 2; i++) {
                moby_point_position_2080b0(w, i, &v);
                queue_glow_sprite_2080b0(&v, 0x301EAA1E, 0.057f, 0.15f);
            }
        }
    }

    hero.glow_phase = fast_add_rotations_2080b0(hero.glow_phase, D_0015ED6C_gp * 2.26892805f);
    hero.moby->glow_color = fast_tween_color_2080b0(0x806E8C6E, 0x805A3232, fast_sin_2080b0(hero.glow_phase) * 0.5f + 0.5f);

    m = hero.items[0].moby;
    if (m == 0) return;
    if (!((m->flags ^ 1) & 1)) return;

    switch (hero.items[0].item_id) {
    case 17: {
        int c5;
        int x1, x2, col;
        float fr;
        float f = fast_add_rotations_2080b0(hero.items[0].glow_phase, D_0015ED6C_gp * 4.36332321f);
        c5 = 0x28C8C8C8;
        hero.items[0].glow_phase = f;
        if (0.0f < f && f < 2.26892805f) c5 = 0x280A0A0A;
        m->glow_color = fast_tween_color_2080b0(m->glow_color, c5, 0.07f);
        v.q = 0;
        v.f[0] = 0.0173452497f;
        v.f[1] = -0.0391772911f;
        v.f[2] = 0.0962345004f;
        transform_vector_by_basis_2080b0(&v, &v, &m->rot_row0);
        add_vector_xyz_2080b0(&v, &v, &m->pos);
        c = (*(unsigned char *)&m->glow_color);
        g = ((unsigned char *)&m->glow_color)[1];
        fr = (float)c;
        c = ((unsigned char *)&m->glow_color)[2];
        x1 = truncate_float_to_s32_2080b0(fr * 0.2f);
        x2 = truncate_float_to_s32_2080b0((float)c * 0.2f);
        col = ((g << 5) / 200) << 24;
        col |= x2 << 16;
        g <<= 8;
        col |= g;
        col |= x1;
        queue_glow_sprite_2080b0(&v, col, 0.15f, 0.1f);
        break;
    }
    case 16:
        if (scale_game_frames_2080b0(50) < hero.items[0].unk20) {
            float k, f;
            int r, g, b, a, col, col1, bs, gs;
            k = D_0015ED6C_gp * 3.14159274f;
            if (hero.unk20A8 != 0) k = D_0015ED6C_gp * 4.71238899f;
            f = fast_add_rotations_2080b0(hero.items[0].glow_phase, k);
            hero.items[0].glow_phase = f;
            r = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 20.0f) + 30;
            g = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 90.0f) + 150;
            b = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 20.0f) + 30;
            if (hero.unk20A8 != 0) {
                int tmp = r;
                r = g;
                g = tmp;
            }
            bs = b << 16;
            col1 = bs | 0x20000000;
            gs = g << 8;
            col1 |= gs;
            col1 |= r;
            m->glow_color = fast_tween_color_2080b0(m->glow_color, col1, 0.13f);
            a = hero.items[0].unk20 - scale_game_frames_2080b0(50);
            if (a > 32) a = 32;
            col = (a << 24) | bs | gs | r;
            moby_point_position_2080b0(m, 1, &v);
            queue_glow_sprite_2080b0(&v, col, 0.15f, 0.04f);
        }
        break;
    case 19: {
        float k, f;
        int r, g, b, col;
        k = D_0015ED6C_gp * 2.26892805f;
        if (hero.unk20A8 != 0) k = D_0015ED6C_gp * 7.33038282f;
        f = fast_add_rotations_2080b0(hero.items[0].glow_phase, k);
        hero.items[0].glow_phase = f;
        if (hero.unk20A8 != 0) {
            r = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 30.0f) + 50;
            g = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 30.0f) + 50;
            b = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 90.0f) + 170;
        } else {
            r = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 70.0f) + 100;
            g = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 40.0f) + 70;
            b = truncate_float_to_s32_2080b0(fast_sin_2080b0(f) * 40.0f) + 70;
        }
        col = b << 16;
        col |= 0x80000000;
        col |= g << 8;
        col |= r;
        m->glow_color = fast_tween_color_2080b0(m->glow_color, col, 0.15f);
        break;
    }
    case 20: {
        int c5;
        int x1, x2, col;
        float fr;
        float f = fast_add_rotations_2080b0(hero.items[0].glow_phase, D_0015ED6C_gp * 4.36332321f);
        c5 = 0xDCDCDC;
        hero.items[0].glow_phase = f;
        if (0.0f < f && f < 2.44346094f) c5 = 0xA0A0A;
        m->glow_color = fast_tween_color_2080b0(m->glow_color, c5, 0.07f);
        v.q = 0;
        v.f[0] = 0.0171042103f;
        v.f[1] = -0.0939194188f;
        v.f[2] = 0.140954122f;
        transform_vector_by_basis_2080b0(&v, &v, &m->rot_row0);
        add_vector_xyz_2080b0(&v, &v, &m->pos);
        c = (*(unsigned char *)&m->glow_color);
        g = ((unsigned char *)&m->glow_color)[1];
        fr = (float)c;
        c = ((unsigned char *)&m->glow_color)[2];
        x1 = truncate_float_to_s32_2080b0(fr * 0.2f);
        x2 = truncate_float_to_s32_2080b0((float)c * 0.2f);
        col = ((g << 5) / 200) << 24;
        col |= x2 << 16;
        g <<= 8;
        col |= g;
        col |= x1;
        queue_glow_sprite_2080b0(&v, col, 0.17f, 0.1f);
        break;
    }
    case 13: {
        int c5;
        float f = fast_add_rotations_2080b0(hero.items[0].glow_phase, D_0015ED6C_gp * 4.71238899f);
        c5 = 0x2814D214;
        hero.items[0].glow_phase = f;
        if (0.0f < f && f < 2.09439516f) c5 = 0x281414D2;
        m->glow_color = fast_tween_color_2080b0(m->glow_color, c5, frame_scale * 0.05f);
        moby_point_position_2080b0(m, 0, &v);
        queue_glow_sprite_2080b0(&v, m->glow_color, 0.1f, 0.03f);
        break;
    }
    }
}
extern unsigned char D_0015EDB5_208820 __asm__("D_0015EDB5");
extern void FUN_L00_00232fe8(void);
extern void FUN_L00_002330d0(void);
extern void FUN_L00_002078a8(void);
extern void FUN_L00_002079e8(void);
extern void ff090_208820(void *, void *, int) __asm__("FUN_001f9838");
extern void upd_a(void *) __asm__("FUN_L00_00250df8");
extern void FUN_L00_002336e8(void *, void *, float);
extern void FUN_L00_0024eed8(void *, int, void *, float, float);
extern int FUN_L00_0020d568(void);

void FUN_L00_00208820(void) {
    OvlQuad v[1];
    float m, f, d;
    unsigned char *o;

    ((unsigned char *)hero.moby)[0xA4] = 0xFF;
    if (hero.unk20A5) {
        FUN_L00_00232fe8();
    } else {
        FUN_L00_002330d0();
    }
    qcopy(((unsigned char *)hero.moby) + 0x10, &hero.motion.pos);
    FUN_L00_002078a8();
    qcopy(((unsigned char *)hero.moby) + 0x40, &hero.motion.rot);
    FUN_L00_002079e8();
    ff090_208820(((unsigned char *)hero.moby) + 0xC0, &hero, 0x30);
    upd_a(((unsigned char *)hero.moby));
    if (hero.unk1CC == 0 && *(int *)(((unsigned char *)hero.moby) + 0x94) != 0) {
        qcopy(v, &hero.motion.pos);
        m = 0.05f;
        FUN_L00_002336e8(v, v, hero.unk224);
        d = hero.capsule_top - hero.unk224;
        if (d < m)
            d = m;
        FUN_L00_0024eed8(((unsigned char *)hero.moby), 0, v, hero.unk234, d);
        f = hero.unk234 + m;
        if (hero.state.control_mode == 4)
            f += 0.15f;
        if (FUN_L00_0020d568())
            f = hero.unk224;
        qcopy(v, &hero.motion.pos);
        FUN_L00_002336e8(v, v, f);
        d = hero.capsule_top - f;
        if (d < m)
            d = m;
        FUN_L00_0024eed8(((unsigned char *)hero.moby), 1, v, hero.unk234, d);
    }
    o = ((unsigned char *)hero.moby);
    if (*(short *)(o + 0xA6) == 0) {
        if (D_0015EDB5_208820) {
            *(unsigned short *)(o + 0x34) |= 0x8000;
        } else {
            *(unsigned short *)(o + 0x34) &= 0x7FFF;
        }
    }
}
extern void FUN_L00_00233210_00208a10(void) __asm__("FUN_L00_00233210");
extern void FUN_L00_00233248_00208a10(void) __asm__("FUN_L00_00233248");
extern void FUN_L00_00232fe8_00208a10(void) __asm__("FUN_L00_00232fe8");
extern void FUN_001f9838_00208a10(void *, void *, int) __asm__("FUN_001f9838");
extern void FUN_L00_002336e8_00208a10(void *, void *, float) __asm__("FUN_L00_002336e8");
extern void FUN_L00_0024eed8_00208a10(void *, int, void *, float,
                                      float) __asm__("FUN_L00_0024eed8");

void FUN_L00_00208a10(void) {
    u128 v;
    float f;
    unsigned char *o;
    if (hero.unk20A5) {
        FUN_L00_00233210_00208a10();
    } else {
        FUN_L00_00233248_00208a10();
    }
    FUN_L00_00232fe8_00208a10();
    ((unsigned char *)hero.moby)[0xA4] = 0xFF;
    o = ((unsigned char *)hero.moby);
    qcopy(o + 0x10, &hero.motion.pos);
    qcopy(o + 0x40, &hero.motion.rot);
    FUN_001f9838_00208a10(o + 0xC0, &hero, 0x30);
    upd_a(((unsigned char *)hero.moby));
    if (hero.unk1CC == 0 && *(int *)(((unsigned char *)hero.moby) + 0x94) != 0) {
        qcopy(&v, &hero.motion.pos);
        FUN_L00_002336e8_00208a10(&v, &v, hero.unk224);
        FUN_L00_0024eed8_00208a10(((unsigned char *)hero.moby), 0, &v, hero.unk234,
                                  hero.capsule_top - hero.unk224);
        f = hero.unk234 + 0.05f;
        qcopy(&v, &hero.motion.pos);
        FUN_L00_002336e8_00208a10(&v, &v, f);
        FUN_L00_0024eed8_00208a10(((unsigned char *)hero.moby), 1, &v, hero.unk234,
                                  hero.capsule_top - f);
    }
}
typedef union {
    OvlQuad q;
    f32 f[4];
} V_208b60;
typedef struct {
    u8 p0[0xC];
    f32 fC;
    u8 p10[0x34 - 0x10];
    u16 h34;
    u8 p36[0x84 - 0x36];
    f32 f84;
    f32 f88;
    u8 p8C[0xBD - 0x8C];
    u8 bBD;
} M_208b60;
typedef struct {
    u8 p0[0x2D8];
    f32 f2D8;
    u8 p2DC[0x2F0 - 0x2DC];
    f32 f2F0;
    f32 f2F4;
    u8 p2F8[0x30E - 0x2F8];
    s16 h30E;
    u8 p310[0xD20 - 0x310];
    f32 fD20;
    u8 pD24[0x2080 - 0xD24];
    M_208b60 *m2080;
    s32 i2084;
    u8 p2088[4];
    s32 i208C;
} G_208b60;
typedef struct {
    f32 x, y, z, w;
} T_208b60;
typedef struct {
    u8 p0[0x28];
    f32 f28;
} U_208b60;
extern G_208b60 D_0013F350_208b60 __asm__("D_0013F350");
extern int D_L00_00173E40[];
f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
void FUN_0020d510_208b60(M_208b60 *, V_208b60 *) __asm__("FUN_0020d510");
f32 angle_atan2(f32, f32) __asm__("FUN_001f9e90");
f32 FUN_001f9dc8_208b60(f32) __asm__("FUN_001f9dc8");
f32 FUN_001f9de0_208b60(f32) __asm__("FUN_001f9de0");
void FUN_001f9a68_208b60(V_208b60 *, M_208b60 *, f32) __asm__("FUN_001f9a68");
void normalize_vector_xyz(V_208b60 *, V_208b60 *, f32) __asm__("FUN_001f9bf8");
void FUN_001f9a10_208b60(V_208b60 *, V_208b60 *, V_208b60 *) __asm__("FUN_001f9a10");
s32 collision_line(V_208b60 *, V_208b60 *, s32, s32, s32) __asm__("FUN_001efa68");
f32 FUN_001f99d0_208b60(f32, f32) __asm__("FUN_001f99d0");
f32 FUN_001f99c8_208b60(f32, f32) __asm__("FUN_001f99c8");
#define G D_0013F350_208b60
extern V_208b60 D_L00_001AEB90;
extern T_208b60 D_L00_0017C300[];
void FUN_L00_00208b60(void) {
    V_208b60 v, w, x;
    f32 k, d, a, c, s, h, hi, lo, sc;
    T_208b60 *t;
    V_208b60 *pw, *px;
    s32 i;
    if (G.i208C == 0x12 || G.i208C == 0x15 || G.i2084 == 0x7D || G.i208C == 0x16) {
        G.m2080->h34 &= 0xFBFF;
        return;
    }
    G.m2080->h34 |= 0x400;
    G.m2080->bBD = 1;
    k = 0.97f;
    if (G.h30E != 0 || G.i208C == 4)
        k = 1.5f;
    d = fast_subtract_rotations(k, G.fD20);
    if (0.052f < d)
        d = 0.052f;
    else if (d < -0.052f)
        d = -0.052f;
    G.fD20 = fast_add_rotations(G.fD20, d);
    FUN_0020d510_208b60(G.m2080, &v);
    a = angle_atan2(v.f[0], v.f[1]);
    c = FUN_001f9dc8_208b60(a);
    s = FUN_001f9de0_208b60(a);
    {
        f32 e = FUN_001f9dc8_208b60(G.fD20);
        v.f[0] = c * e;
        v.f[1] = s * e;
    }
    v.f[2] = -FUN_001f9de0_208b60(G.fD20);
    qcopy(&D_L00_001AEB90, &v);
    h = G.f2D8;
    if (h < 1.0f) {
        f32 t = G.f2F0;
        if (1.0f <= t)
            h = t;
        else {
            t = G.f2F4;
            if (1.0f < t)
                h = t;
            else {
                G.m2080->f84 = 0.0f;
                G.m2080->f88 = 0.0f;
                return;
            }
        }
    }
    hi = h;
    lo = hi;
    pw = &w;
    px = &x;
    t = D_L00_0017C300;
    for (i = 3; i >= 0; i--, t++) {
        M_208b60 *m;
        U_208b60 *u = &(*(U_208b60 *)D_L00_00173E40);
        m = G.m2080;
        sc = m->fC * 0.0009765625f;
        FUN_001f9a68_208b60(pw, m, 0.0009765625f);
        w.f[0] += c * t->x * sc;
        w.f[1] += s * t->y * sc;
        w.f[2] += t->z * sc;
        normalize_vector_xyz(px, &v, ((w.f[2] - h) + 0.5f) / -v.f[2]);
        FUN_001f9a10_208b60(px, pw, px);
        if (collision_line(pw, px, 0x22, 0, 0)) {
            lo = FUN_001f99d0_208b60(lo, u->f28);
            hi = FUN_001f99c8_208b60(hi, u->f28);
        }
    }
    G.m2080->f84 = lo - 0.12f;
    G.m2080->f88 = hi + 0.24f;
    {
        M_208b60 *m = G.m2080;
        f32 z = m->f84 + 3.0f;
        if (z < m->f88)
            m->f88 = z;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00209598), where it is exact; names translated to the US level program. */

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");

f32 FUN_L00_00208f20(f32 a, f32 b, f32 c, f32 d) {
    struct Hero *g = &hero;
    f32 thresh = g->anim_frame;
    f32 step;
    f32 negone = -1.0f;

    if (thresh < a) {
        step = a - thresh;
    } else {
        s32 flag = g->unkA9C;
        f32 rate = 2.0f;

        if (flag == 0) {
            rate = 0.5f / g->unkA94;
        }

        {
            void *obj = ((void *)g->moby);
            s32 idx = *(u8 *)((u8 *)obj + 0x53);
            u8 *arr = *(u8 **)((u8 *)obj + 0x24);
            void *entry = *(void **)(arr + (idx << 2) + 0x48);
            s32 cnt = *(u8 *)((u8 *)entry + 0x10);
            step = a + (rate * (f32)cnt - thresh);
        }
    }

    if (b > 0.0f) {
        struct Hero *g2 = &hero;
        f32 rate2 = g2->unkA94;

        if (g2->unkA9C != 0) {
            void *obj2 = ((void *)g2->moby);
            rate2 = *(f32 *)(*(void **)((u8 *)obj2 + 0x6C));
        }

        {
            f32 target = c * step;
            f32 speed = b * rate2;
            f32 result = target / speed;

            if (d == negone) {
                g2->anim_speed = result;
                return negone;
            }
            return approach_value((f32 *)(&g2->anim_speed), result, d);
        }
    }

    return 0.0f;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00209690), where it is exact; names translated to the US level program. */

extern float split_float_fraction(void *, float) __asm__("FUN_L00_00200228");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
#include "rnc/data_labels.h"

float FUN_L00_00209018(float *a, int n);
float FUN_L00_00209018(float *a, int n) {
    char buf[16] __attribute__((aligned(16)));
    char *d = (char *)((unsigned char *)&D_0013E533) + 0xE1D;
    int i = truncate_float_to_s32(*(float *)(d + 0xAA8));
    int m = n - 1;
    int p = (i + m) % n;
    float w;
    if (*(int *)(d + 0xA98) & 2) {
        p = m;
    }
    w = split_float_fraction(buf, *(float *)(d + 0xAA8));
    return w * a[i] + (*(float *)(d + 0xAAC) - w) * a[p];
}
typedef unsigned int u128_002090d0 __attribute__((mode(TI), aligned(16)));
typedef struct {
    float x, y, z, w_002090d0;
} __attribute__((aligned(16))) Vec4_002090d0;
typedef struct {
    Vec4_002090d0 r[4];
} Mtx_002090d0;
void FUN_001fa050_002090d0(Mtx_002090d0 *, Vec4_002090d0 *) __asm__("FUN_001fa050");
void FUN_001f9d20_002090d0(Vec4_002090d0 *, Vec4_002090d0 *,
                           Mtx_002090d0 *) __asm__("FUN_001f9d20");
void FUN_001fa378_002090d0(Mtx_002090d0 *, Mtx_002090d0 *, Mtx_002090d0 *) __asm__("FUN_001fa378");
void FUN_001f9a28_002090d0(Vec4_002090d0 *, Vec4_002090d0 *,
                           Vec4_002090d0 *) __asm__("FUN_001f9a28");
void FUN_001f9a10_002090d0(Vec4_002090d0 *, Vec4_002090d0 *,
                           Vec4_002090d0 *) __asm__("FUN_001f9a10");
void FUN_00214598_002090d0(Mtx_002090d0 *, Vec4_002090d0 *) __asm__("FUN_00214598");
extern Vec4_002090d0 D_0013F3E0;
void FUN_L00_002090d0(float *inf, float x, float y, float z) {
    u128_002090d0 *in = (u128_002090d0 *)inf;
    struct {
        Vec4_002090d0 p;
        Vec4_002090d0 a;
        Vec4_002090d0 b;
        Vec4_002090d0 d;
        Mtx_002090d0 m;
        Mtx_002090d0 r;
        Vec4_002090d0 e;
    } L_002090d0;
    Vec4_002090d0 *cam = &D_0013F3E0;
    *(u128_002090d0 *)&L_002090d0.p = *in;
    FUN_001fa050_002090d0(&L_002090d0.m, cam);
    FUN_001f9d20_002090d0(&L_002090d0.a, &L_002090d0.p, &L_002090d0.m);
    L_002090d0.e.x = x;
    L_002090d0.e.y = y;
    L_002090d0.e.z = z;
    FUN_001fa050_002090d0(&L_002090d0.r, &L_002090d0.e);
    FUN_001fa378_002090d0(&L_002090d0.m, &L_002090d0.m, &L_002090d0.r);
    FUN_001f9d20_002090d0(&L_002090d0.b, &L_002090d0.p, &L_002090d0.m);
    FUN_001f9a28_002090d0(&L_002090d0.d, &L_002090d0.a, &L_002090d0.b);
    FUN_001f9a10_002090d0(cam - 1, cam - 1, &L_002090d0.d);
    FUN_00214598_002090d0(&L_002090d0.m, cam);
}
#define NOT_SDA

#define MACRO_ADDR

/* Builds a {0,0,0.6,0} param vector and forwards it (plus the caller's
   position) to func_L00_00209748. */
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00209850), where it is exact; names translated to the US level program. */

typedef float V[4] __attribute__((aligned(16)));

extern void FUN_L00_002090d0(float *, float, float, float);

/* Calls FUN_L00_002090d0 with a vector (0, 0, 0.6) and x, y, z. */
void FUN_L00_002091d8(float x, float y, float z) {
    V buf;

    clear_u64_value(buf);
    buf[2] = 0.6f;
    FUN_L00_002090d0(buf, x, y, z);
}
#include "sda.h"
typedef struct {
    s32 v[9];
} T00209240;
extern T00209240 D_L00_001E7C10;
extern int D_001413D0[] __attribute__((section(".data")));
void FUN_L00_0024f8f0(s32, s32, T00209240 *, void *);
/* Calls 0024f8f0 with mode 9 and a copy of D_L00_001E7C10 on a, or on D_001413D0 when a is 0. */
void FUN_L00_00209240(s32 a) {
    T00209240 t;
    if (a == 0)
        a = (*(s32 *)D_001413D0);
    t = D_L00_001E7C10;
    FUN_L00_0024f8f0(a, 9, &t, (((char *)&hero) + 0xac0));
}
/* A packed pose: after this header come three lists of 64-bit entries, four
   saturated s16 each. Rotations (quaternions * 32768) start with the root at
   0x10; scales (4096 = 1.0) and translations follow, each entry tagged with
   its joint index in the top s16 (scales also with 0x8000). */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 size;               /* 0x6: quadwords from 0x10 to the end */
    u16 scale_offset;       /* 0x8: bytes from 0x10 to the scale list */
    u16 scale_count;        /* 0xA */
    u16 translation_offset; /* 0xC: bytes from 0x10 to the translation list */
    u16 translation_count;  /* 0xE */
    u64 root_rotation;      /* 0x10 */
} PackedPose;

/* The identity quaternion, packed: w = 0x7FFF. */
#define PACKED_IDENTITY 0x7fff000000000000ULL
/* A packed scale of 1.0 (x = y = z = 0x1000), which the scale list leaves out. */
#define PACKED_UNIT_SCALE 0x100010001000ULL
#define PACKED_XYZ_MASK 0xffffffffffffULL

/* Scratchpad joint records, 0x40 bytes each: rotation, scale, translation. */
#define SPR_JOINT_ROTATION 0x70000000
#define SPR_JOINT_SCALE 0x70000010
#define SPR_JOINT_TRANSLATION 0x70000020

extern u64 pack_vector_s16(s32 vector, f32 scale) __asm__("FUN_L00_001ff768");
extern void FUN_00254b58_002092c8(s32) __asm__("FUN_L00_00254b58");

/* Packs the scratchpad joints listed in joints[1..] (ended by a negative
   entry) into pose: every rotation, padding identity rotations, the scales
   that are not 1.0 and the translations that differ from the bind pose. */
void pack_pose(s32 *joints, PackedPose *pose, u8 *model, s32 padding, s32 context) __asm__("FUN_L00_002092c8");
void pack_pose(s32 *joints, PackedPose *pose, u8 *model, s32 padding, s32 context) {
    u64 *out;
    s32 rotation_count;
    s32 scale_count;
    s32 translation_count;
    s32 i;
    s32 joint;
    s32 rotation_size;
    u64 value;

    if (context == 0)
        context = (*(s32 *)D_001413D0);
    out = (u64 *)(pose + 1);
    FUN_00254b58_002092c8(context);
    pose->root_rotation = PACKED_IDENTITY;
    rotation_count = 1;
    for (i = 1; joints[i] >= 0; i++) {
        *out++ = pack_vector_s16((joints[i] << 6) + SPR_JOINT_ROTATION, 32768.0f);
        rotation_count++;
    }
    if (padding > 0) {
        s32 remaining = padding;
        do {
            *out = PACKED_IDENTITY;
            remaining--;
            out++;
            rotation_count++;
        } while (remaining != 0);
    }
    scale_count = 0;
    i = 1;
    rotation_size = rotation_count << 3;
    for (; joints[i] >= 0; i++) {
        value = pack_vector_s16(SPR_JOINT_SCALE + (joints[i] << 6), 4096.0f);
        if ((value & PACKED_XYZ_MASK) != PACKED_UNIT_SCALE) {
            *out = value & PACKED_XYZ_MASK;
            scale_count++;
            *(u16 *)((u8 *)out + 6) = i | 0x8000;
            out++;
        }
    }
    translation_count = 0;
    for (joint = 1; joints[joint] >= 0; joint++) {
        value = pack_vector_s16(SPR_JOINT_TRANSLATION + (joints[joint] << 6), 1.0f);
        if ((value & PACKED_XYZ_MASK) !=
            (pack_vector_s16(*(s32 *)(model + 0x18) + (joint << 4), 1.0f) & PACKED_XYZ_MASK)) {
            *out = value & PACKED_XYZ_MASK;
            translation_count++;
            *(u16 *)((u8 *)out + 6) = joint;
            out++;
        }
    }
    if ((u32)out & 0xf)
        *out++ = 0;
    pose->translation_count = translation_count;
    pose->scale_offset = rotation_size;
    pose->size = ((u8 *)out - (u8 *)&pose->root_rotation) >> 4;
    pose->scale_count = scale_count;
    pose->translation_offset = (rotation_count + scale_count) << 3;
    pose->unk2 = 0;
    pose->unk4 = 0;
    pose->unk0 = 0;
}
typedef struct {
    float x, y, z, w_00209540;
} V4_00209540;
typedef struct {
    char pad[0x80];
    float x, y;
    char pad2[0x2f0 - 0x88];
    V4_00209540 pos;
    char pad3[0x904 - 0x300];
    short cnt;
} G_00209540;
extern G_00209540 D_0013F350_00209540 __asm__("D_0013F350");
float FUN_002132a8_00209540(float, float) __asm__("FUN_002132a8");
void FUN_L00_002715e8_00209540(float *, V4_00209540 *, int, float,
                               float) __asm__("FUN_L00_002715e8");
int FUN_00213260_00209540(int) __asm__("FUN_00213260");
void FUN_L00_00209540(int a, int lim) {
    G_00209540 *g = &D_0013F350_00209540;
    if (lim < ++g->cnt) {
        float v[4];
        float lo = -0.05f;
        float hi = 0.05f;
        v[0] = g->x + FUN_002132a8_00209540(lo, hi);
        v[1] = g->y + FUN_002132a8_00209540(lo, hi);
        v[2] = g->pos.x;
        {
            float s = FUN_002132a8_00209540(0.4f, 0.6f);
            FUN_L00_002715e8_00209540(v, &g->pos, -1, s, 5250.0f);
        }
        g->cnt = FUN_00213260_00209540(a);
    }
}
extern float D_0015ED6C;
extern float FUN_001fa580_209638(float, float) __asm__("FUN_001fa580");
extern float FUN_001fa5c8_209638(float, float) __asm__("FUN_001fa5c8");
extern float FUN_002132a8_209638(float, float) __asm__("FUN_002132a8");
extern int FUN_00213260_209638(int) __asm__("FUN_00213260");
extern void FUN_L00_002718d0_209638(void *, void *, void *, float,
                                    float) __asm__("FUN_L00_002718d0");

void FUN_L00_00209638(int seed, int limit);
void FUN_L00_00209638(int seed, int limit) {
    float v[4];
    float pos[4];
    float ang;
    int i;
    struct Hero *p;

    if (limit < ++hero.unk908) {
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                ang = FUN_001fa580_209638(hero.motion.rot.f[2], 0.785398f);
            } else {
                ang = FUN_001fa5c8_209638(hero.motion.rot.f[2], 0.785398f);
            }
            p = &hero;
            v[0] = fast_cos(ang) * (D_0015ED6C * 0.6f);
            v[1] = fast_sin(ang) * (D_0015ED6C * 0.6f);
            v[2] = 0.0f;
            pos[0] = p->motion.pos.f[0] + v[0] * 18.0f;
            pos[1] = p->motion.pos.f[1] + v[1] * 18.0f;
            pos[2] = p->water_surface_z;
            v[0] -= fast_cos(p->motion.rot.f[2]) * (D_0015ED6C * 1.5f);
            v[1] -= fast_sin(p->motion.rot.f[2]) * (D_0015ED6C * 1.5f);
            {
                float r = FUN_002132a8_209638(0.4f, 0.5f);
                float d = -2.0f;
                if (i == 0)
                    d = 2.0f;
                FUN_L00_002718d0_209638(pos, v, &p->water_surface_z, r, d);
            }
            if (seed) {
                p->unk908 = FUN_00213260_209638(seed);
            } else {
                p->unk908 = 0;
            }
        }
    }
}
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_209848;
extern float D_0015ED6C;
float FUN_002132a8_209848(float, float) __asm__("FUN_002132a8");
float FUN_00213308_209848(void) __asm__("FUN_00213308");
int random_int_between(int, int) __asm__("FUN_L00_00257b90");
void FUN_L00_0026f548_209848(V_209848 *, V_209848 *, int, int) __asm__("FUN_L00_0026f548");
int FUN_00213260_209848(int) __asm__("FUN_00213260");
void FUN_L00_00209848(int n, int lim) {
    struct Hero *P = &hero;
    if (lim < ++P->unk906) {
        V_209848 t, v;
        float a, r;
        int k;
        t.x = P->motion.pos.f[0] + FUN_002132a8_209848(-0.15f, 0.15f) + P->motion.displacement.f[0] * 8.0f;
        {
            float y = P->motion.pos.f[1] + FUN_002132a8_209848(-0.15f, 0.15f) + P->motion.displacement.f[1] * 8.0f;
            t.z = P->water_surface_z + 0.1f;
            t.y = y;
        }
        a = FUN_00213308_209848();
        r = FUN_002132a8_209848(D_0015ED6C * 0.0f, D_0015ED6C * 3.0f);
        v.x = fast_cos(a) * r + P->motion.displacement.f[0] * 0.75f;
        v.y = fast_sin(a) * r + P->motion.displacement.f[1] * 0.75f;
        v.z = FUN_002132a8_209848(D_0015ED6C * 3.0f, D_0015ED6C * 6.5f);
        k = random_int_between(0x5A, 0x78);
        if (P->unk22A4 < 0.25f)
            FUN_L00_0026f548_209848(&t, &v, 3, k);
        else
            FUN_L00_0026f548_209848(&t, &v, 1, k);
        {
            short h = FUN_00213260_209848(n);
            struct Hero *Q = &hero;
            Q->unk906 = h;
        }
    }
}
typedef union {
    OvlQuad q;
    float f[4];
} V_209a40;
extern float D_0015ED6C;
extern float FUN_002132a8_209a40(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_0024f7c8_209a40(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int random_integer_below_cf(void *, void *, float, float) __asm__("FUN_L00_0026f080");

void FUN_L00_00209a40(int n, int mode) {
    float pos[4];
    V_209a40 v;
    int i;
    struct Hero *p;
    V_209a40 *src;

    for (i = 0; i < n; i++) {
        p = &hero;
        src = (V_209a40 *)&p->motion.displacement;
        if (mode == 0) {
            pos[0] = p->motion.pos.f[0] + FUN_002132a8_209a40(-0.1f, 0.1f);
            pos[1] = p->motion.pos.f[1] + FUN_002132a8_209a40(-0.1f, 0.1f);
            pos[2] = p->motion.pos.f[2] + FUN_002132a8_209a40(-0.5f, 0.1f);
        } else if (mode == 1) {
            if (!(i & 1)) {
                FUN_L00_0024f7c8_209a40(p->moby, 0, pos);
            } else {
                FUN_L00_0024f7c8_209a40(p->moby, 0xE, pos);
            }
        } else if (mode == 2) {
            if (!(i & 1)) {
                FUN_L00_0024f7c8_209a40(p->moby, 0x17, pos);
            } else {
                FUN_L00_0024f7c8_209a40(p->moby, 0x16, pos);
            }
        }
        qcopy(&v, src);
        if (p->state.current == 0x34) {
            scale_vector_xyz(&v, &v, 1.15f);
        } else {
            scale_vector_xyz(&v, &v, 0.7f);
        }
        v.f[0] += FUN_002132a8_209a40(D_0015ED6C * -0.4f, D_0015ED6C * 0.4f);
        v.f[1] += FUN_002132a8_209a40(D_0015ED6C * -0.4f, D_0015ED6C * 0.4f);
        v.f[2] += FUN_002132a8_209a40(-D_0015ED6C, D_0015ED6C * 0.0f);
        random_integer_below_cf(pos, &v, FUN_002132a8_209a40(4200.0f, 7350.0f), -1.0f);
    }
}
#include "rnc/math/circle_offset.h"

extern float D_0015ED6C;
extern f32 random_float_between(f32 a, f32 b) __asm__("FUN_002132a8");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern s32 FUN_L00_00257b90(s32, s32);
extern void FUN_L00_002d7e90(Vec4 *, f32);
extern void FUN_L00_002715e8(Vec4 *, f32 *, s32, f32, f32);
extern void FUN_L00_0026f548(Vec4 *, Vec4 *, s32, s32);

/* Kicks up dust at the hero's feet (hero.water_surface_z): with flash set,
   first a flash at the hero; then `puffs` puffs within 0.3 of the hero and
   `debris` bits thrown out in random directions. The debris start point adds
   the x velocity to both x and y, as in the original. */
void FUN_L00_00209ca8(s32 puffs, s32 debris, s32 flash) {
    Vec4 v;
    Vec4 start;
    f32 ang;
    f32 speed;
    s32 color;
    s32 life;
    s32 i;
    s32 j;

    if (flash) {
        qcopy(&v, &hero.motion.pos);
        v.f[2] = hero.water_surface_z;
        FUN_L00_002d7e90(&v, 2.25f);
    }
    for (j = 0; j < puffs; j++) {
        v.f[0] = hero.motion.pos.f[0] + random_float_between(-0.3f, 0.3f);
        v.f[1] = hero.motion.pos.f[1] + random_float_between(-0.3f, 0.3f);
        v.f[2] = hero.water_surface_z;
        FUN_L00_002715e8(&v, &hero.water_surface_z, -1, random_float_between(0.3f, 0.6f), 5250.0f);
    }
    for (i = 0; i < debris; i++) {
        ang = random_angle_radians();
        speed = random_float_between(D_0015ED6C * 0.0f, D_0015ED6C * 3.0f);
        v.f[0] = fast_cos(ang) * speed;
        v.f[1] = fast_sin(ang) * speed;
        v.f[2] = random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 8.0f);
        start.f[0] = hero.motion.pos.f[0] + v.f[0] * 8.0f;
        start.f[1] = hero.motion.pos.f[1] + v.f[0] * 8.0f;
        start.f[2] = hero.water_surface_z - random_float_between(0.0f, 0.2f);
        color = random_integer_below(2);
        life = FUN_L00_00257b90(0x5A, 0x78);
        FUN_L00_0026f548(&start, &v, color, life);
    }
}
extern struct Hero hero_data __asm__("D_0013F350") __attribute__((section(".data")));
extern float D_0015ED6C;
int fast_dec_timer(void *) __asm__("FUN_001f9770");
float FUN_001fa580_209ec8(float, float) __asm__("FUN_001fa580");
float spread_k(float, float) __asm__("FUN_L00_00257c48");
void FUN_L00_0026d598_209ec8(void *, void *, int) __asm__("FUN_L00_0026d598");
int FUN_001f96f8_209ec8(int) __asm__("FUN_001f96f8");
int FUN_001fa6d0_209ec8(float) __asm__("FUN_001fa6d0");
float FUN_002132a8_209ec8(float, float) __asm__("FUN_002132a8");
float FUN_00213308_209ec8(void) __asm__("FUN_00213308");
void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
void FUN_L00_00269958(void *, void *, int, int, int, int, int,
                             int);
extern char D_0013F7EE[] __attribute__((section(".data")));
void FUN_L00_00209ec8(void) {
    OvlQuad v[1];
    OvlQuad w[1];
    char *g;
    char *base;
    float a, r, r0, f, theta, phi;
    int t, c, col, life, n1, n2, k;
    OvlQuad *pt;
    g = D_0013F7EE;
    if (fast_dec_timer(g)) {
        qcopy(v, g - 0x41E);
        base = g - 0x49E;
        a = FUN_001fa580_209ec8(*(float *)(base + 0x98), 1.5707963705062866f);
        r0 = spread_k(D_0015ED6C * 0.2f, D_0015ED6C * 0.7f);
        ((float *)w)[0] = fast_cos(a) * r0;
        ((float *)w)[1] = fast_sin(a) * r0;
        ((int *)w)[2] = 0;
        ((float *)w)[3] = frame_time_sq * -8.0f;
        FUN_L00_0026d598_209ec8(v, w, 0);
        *(short *)(base + 0x49E) = random_int_between(2, 3);
    }
    if (!fast_dec_timer(g - 2))
        return;
    t = FUN_001f96f8_209ec8(0x28);
    f = (float)(t - *(int *)(g - 0x306)) / (float)FUN_001f96f8_209ec8(0x28);
    if (1.0f < f)
        f = 1.0f;
    if (f < 0.0f)
        f = 0.0f;
    c = FUN_001fa6d0_209ec8(f * 240.0f);
    c = c < 0xA1 ? c : 0xA0;
    c = 0x1F < c ? c : 0x20;
    col = (c << 24) | 0xC0F0;
    life = FUN_001f96f8_209ec8(0x28);
    pt = w;
    for (k = 0x38; k >= 0; k--) {
        n1 = FUN_001fa6d0_209ec8(FUN_002132a8_209ec8(4200.0f, 7350.0f) / 1000.0f);
        n2 = FUN_001fa6d0_209ec8(FUN_002132a8_209ec8(21000.0f, 31500.001953125f) / 1000.0f);
        r = FUN_002132a8_209ec8(D_0015ED6C * 0.15f, D_0015ED6C * 0.57f);
        qcopy(v, g - 0x41E);
        theta = FUN_00213308_209ec8();
        phi = FUN_00213308_209ec8();
        build_spherical_offset(pt, r, theta, phi);
        FUN_L00_00269958(v, pt, col, 0x70F0, life, n1, n2, 1);
    }
    t = random_int_between(FUN_001f96f8_209ec8(3), FUN_001f96f8_209ec8(7));
    {
        struct Hero *b2 = &hero_data;
        b2->unk49C = t;
    }
}
typedef struct {
    char pad[0xA4];
    float a;
    float b;
} S0020a1e0;
extern S0020a1e0 D_L00_0017A680;
/* Stores the two floats in D_L00_0017A680. */
void FUN_L00_0020a1e0(float a, float b) {
    S0020a1e0 *p = &D_L00_0017A680;
    p->a = a;
    p->b = b;
}
typedef struct V0020a240 {
    f32 x, y, z, w;
} V0020a240;
typedef struct G0020a240 {
    u8 pad[0x80];
    u8 v80[0x1b4];
    f32 f234;
} G0020a240;
extern u8 D_L00_00173E60[];
extern void FUN_L00_002335c8(V0020a240 *, f32, f32, f32);
extern s32 FUN_001efa68(V0020a240 *, V0020a240 *, s32, s32, s32);
extern f32 vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
s32 FUN_L00_0020a240(f32 *out, f32 a, f32 b) {
    G0020a240 *g = (G0020a240 *)&hero;
    V0020a240 p;
    V0020a240 q;

    FUN_L00_002335c8(&p, g->f234 - 0.02f, 0.0f, a);
    FUN_L00_002335c8(&q, b, 0.0f, a);
    if (FUN_001efa68(&p, &q, 2, 0, 0)) {
        if (out != 0)
            *out = vector_distance_xy(g->v80, D_L00_00173E60);
        return 1;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020A990), where it is exact; names translated to the US level program. */

extern V collision_hit_point __asm__("D_L00_00173E60") __attribute__((section(".data")));
extern char D_L00_0017A680_d[] __asm__("D_L00_0017A680") __attribute__((section(".data")));
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern int FUN_L00_0020a240_u(int, float, float) __asm__("FUN_L00_0020a240");
extern int FUN_L00_0025e3b8(void *);
extern void *D_L00_00173E58_d __asm__("D_L00_00173E58") __attribute__((section(".data")));

void FUN_L00_0020a318(void) {
    char *g = (char *)((unsigned char *)&D_0013E533) + 0xE1D;
    char *p;
    char *p2;
    char *h;
    char *g2;
    float f12, f13, f, s, s2;
    *(short *)(g + 0x22C8) = 0;
    if (*(unsigned char *)(g + 0x20AB) == 0)
        return;
    if (hero_slot_item_id(0) == 0x1A)
        return;
    if (*(int *)(g + 0x2084) == 0x81) {
        p = D_L00_0017A680_d;
        s = frame_scale_sq;
        *(float *)(p + 0x694) = -0.349065840f;
        *(float *)(p + 0x6D4) = s * 0.05f;
        *(float *)(p + 0x6D8) = s * 0.3f;
    }
    f13 = 1.29999995f;
    if (*(short *)(g + 0x22C8) != 0)
        f13 = 1.5999999f;
    f12 = 0.569999993f;
    if (*(unsigned char *)(g + 0x20A8) != 0)
        f12 = 0.699999988f;
    if (FUN_L00_0020a240_u(0, f12, f13) == 0)
        return;
    if (*(unsigned char *)(g + 0x20A8) != 0) {
        if (D_L00_00173E58_d != 0) {
            if (FUN_L00_0025e3b8(D_L00_00173E58_d) != 0)
                return;
        }
    }
    s2 = frame_scale_sq;
    p2 = D_L00_0017A680_d;
    *(float *)(p2 + 0x6D4) = s2 * 0.05f;
    *(float *)(p2 + 0x6D8) = s2 * 0.3f;
    h = (char *)((unsigned char *)&D_0013E533) + 0xE9D;
    f = fast_vec_dist(h, collision_hit_point);
    g2 = (char *)((unsigned char *)&D_0013E533) + 0xE1D;
    f = f - *(float *)(g2 + 0x234);
    f = (1.29999995f - f) / 1.29999995f;
    f = f * 1.25f;
    f = f * -1.22173047f;
    if (f > -0.523598790f)
        f = -0.523598790f;
    if (f < -1.22173047f)
        f = -1.22173047f;
    *(float *)(p2 + 0x694) = f;
    *(short *)(g2 + 0x22C8) = 1;
}
extern char D_0013F350_0020a500[] __asm__("D_0013F350");
/* Clears the words at 0x1004/0x1008 of ((char *)&hero) and four 0x10-byte slots from 0x1024. */
void FUN_L00_0020a500(void) {
    char *p = D_0013F350_0020a500;
    char *q;
    int i;
    *(int *)(p + 0x1004) = 0;
    *(int *)(p + 0x1008) = 0;
    q = p;
    for (i = 3; i >= 0; i--) {
        *(int *)(q + 0x1024) = 0;
        *(int *)(q + 0x1028) = 0;
        q += 0x10;
    }
}
typedef struct {
    char p000[0x248];
    f32 f248;
    char p24c[0x258 - 0x24C];
    f32 f258;
    f32 f25c;
    s32 i260;
    char p264[0x1004 - 0x264];
    f32 f1004;
    f32 f1008;
    char p100c[4];
    s32 i1010;
    f32 f1014;
    f32 f1018;
    char p101c[0x1184 - 0x101C];
    s32 i1184;
    char p1188[0x2080 - 0x1188];
    void *p2080;
    s32 i2084;
    char p2088[0x2274 - 0x2088];
    void *p2274;
} G_a540;

typedef struct {
    char p000[0x114];
    f32 f114;
    f32 f118;
    char p11c[0x274 - 0x11C];
    f32 f274;
    f32 f278;
    char p27c[0xC5C - 0x27C];
    f32 fc5c;
    char pc60[0xD0C - 0xC60];
    f32 fd0c;
} P_a540;

extern G_a540 D_0013F350_a540 __asm__("D_0013F350");
extern P_a540 D_L00_0017A680_a540 __asm__("D_L00_0017A680");
extern f32 D_0015EE14 __asm__("D_0015EE14");
extern f32 D_0015EE18 __asm__("D_0015EE18");

extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern void FUN_00213ed8_a540(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern void FUN_L00_0020a1e0_a540(f32, f32) __asm__("FUN_L00_0020a1e0");
extern void FUN_L00_0020a1e0_a540_0020a1f8(f32, f32) __asm__("FUN_L00_0020a1f8");
extern void FUN_L00_0020a1e0_a540_0020a228(f32, f32) __asm__("FUN_L00_0020a228");
extern s32 FUN_L00_00257b90_a540(s32, s32) __asm__("FUN_L00_00257b90");
extern f32 FUN_L00_00257c48_a540(f32, f32) __asm__("FUN_L00_00257c48");
extern f32 FUN_L00_0025b6a8_a540(f32, f32, f32) __asm__("FUN_L00_0025b6a8");

extern u8 D_0015EDB3 __asm__("D_0015EDB3");
#define G D_0013F350_a540
#define P D_L00_0017A680_a540

void FUN_L00_0020a540(void) {
    G_a540 *g;
    G_a540 *h;
    P_a540 *p;
    f32 f, t, k;
    s32 v;
    s32 a;

    if (D_0015EDB3 != 0 && G.i1184 != 0) {
        P.fd0c = D_0015EE18;
    }
    P.fc5c = D_0015EE14;
    if (*(u8 *)0x15EDB1 != 0) {
        FUN_00213ed8_a540(&D_0015EE14, 1.57f, 0.05f);
    } else {
        FUN_00213ed8_a540(&D_0015EE14, 0.92f, 0.05f);
    }
    if (G.i260 != 0 && 1.4f < G.f258 && 1.0f < G.f248) {

        FUN_L00_0020a1e0_a540_0020a228(frame_scale_sq * 0.015f, frame_scale_sq * 0.3f);
        f = G.f258 * 1.5f * DEG_TO_RAD + 0.19198622f;
        if (0.47123888f < f)
            f = 0.47123888f;
        if ((1.0f - G.f25c) * 35.0f * DEG_TO_RAD < f)
            f = (1.0f - G.f25c) * 35.0f * DEG_TO_RAD;
        P.f274 = f;
        FUN_L00_0020a1e0_a540_0020a1f8(frame_scale_sq * 0.008f, frame_scale_sq * 0.3f);
        P.f114 = P.f274 * 0.7f;
        return;
    }
    if (G.i2084 != 0)
        return;
    v = *(u8 *)((char *)G.p2080 + 0x53);
    if ((u8)v >= 3 && (u8)v != 0x3A)
        return;
    if (G.p2274 != 0)
        return;
    if ((u32)(v - 1) < 2) {
        a = scale_game_frames(40);
        G.i1010 = FUN_L00_00257b90_a540(a, scale_game_frames(70));
    }
    if (tick_countdown_32_alt(&G.i1010) != 0) {
        G.f1004 = -random_float_between(-0.15707964f, 0.43633232f);
        G.f1008 = FUN_L00_00257c48_a540(0.34906584f, 0.99483764f);
        t = random_float_between(0.0f, 1.0f);
        G.f1014 = FUN_L00_0025b6a8_a540(0.015f, 0.007f, t);
        G.f1018 = FUN_L00_0025b6a8_a540(0.29f, 0.3f, t);
        a = scale_game_frames(90);
        G.i1010 = FUN_L00_00257b90_a540(a, scale_game_frames(200));
        G.i1010 += truncate_float_to_s32((f32)scale_game_frames(80) * t);
    }
    if ((u32) * (u8 *)((char *)G.p2080 + 0x53) - 1 < 2) {
        if (0.34906584f < G.f1008)
            G.f1008 = 0.34906584f;
        else if (G.f1008 < -0.34906584f)
            G.f1008 = -0.34906584f;
        if (0.2617994f < G.f1004)
            G.f1004 = 0.2617994f;
        if (G.f1004 < 0.0f)
            G.f1004 = 0.0f;
    }
    FUN_L00_0020a1e0_a540_0020a228(G.f1014, G.f1018);
    P.f278 = G.f1008;
    P.f274 = G.f1004;
    FUN_L00_0020a1e0_a540_0020a1f8(frame_scale_sq * 0.008f, frame_scale_sq * 0.3f);
    P.f118 = P.f278 * 0.52f;
    P.f114 = P.f274 * 0.55f;
}
/* Re-rolls four random motion parameters and copies each pair into its 0xB0-byte record. */
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020B060), where it is exact; names translated to the US level program. */

extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float FUN_L00_00257c48(float lo, float hi);
extern int tick_countdown_32_alt_c(int *) __asm__("FUN_001f9740");
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern char D_001403B0[];

void FUN_L00_0020a9e8(void) {
    char *base = (char *)((char *)&D_0013F350_002062b0);
    int *q;
    char *g;
    int i, j;
    char *tbl;

    if (*(int *)(base + 0x2084) != 0) return;
    if (*(unsigned char *)(*(int *)(base + 0x2080) + 0x53) != 0) return;
    q = (int *)(base + 0x1060);
    g = base;
    tbl = ((char *)D_L00_0017A680_c);
    for (i = 0; i < 4; i++) {
        char *f;
        if (tick_countdown_32_alt_c(&q[i])) {
            if (i == 0) {
                int *s;
                *(float *)(g + 0x1028) = FUN_L00_00257c48(0.17453292f, 0.5235988f);
                s = (int *)(((char *)&D_001403B0));
                *s = FUN_L00_00257b90(scale_game_frames_c(0x46), scale_game_frames_c(0x96));
            } else if (i == 1) {
                *(float *)((char *)q - 0x28) = FUN_L00_00257c48(0.17453292f, 0.5235988f);
                q[1] = FUN_L00_00257b90(scale_game_frames_c(0x28), scale_game_frames_c(0x5A));
            } else if (i == 2) {
                *(float *)((char *)q - 0x18) = FUN_L00_00257c48(0.2617994f, 0.87266463f);
                q[2] = FUN_L00_00257b90(scale_game_frames_c(0x28), scale_game_frames_c(0x5A));
            } else {
                float old, nw;
                *(float *)(g + i * 0x10 + 0x1024) = random_float_between_c(-0.08726646f, 0.5235988f);
                old = *(float *)(g + i * 0x10 + 0x1028);
                nw = FUN_L00_00257c48(0.34906584f, 0.95993108f);
                *(float *)(g + i * 0x10 + 0x1028) = nw;
                if ((old <= 0 && nw <= 0) || (old >= 0 && nw >= 0))
                    *(float *)(g + i * 0x10 + 0x1028) = -*(float *)(g + i * 0x10 + 0x1028);
                q[i] = FUN_L00_00257b90(scale_game_frames_c(0x23), scale_game_frames_c(0x46));
            }
        }
        f = g + i * 0x10;
        j = i + 13;
        base = (char *)(j * 0xB0);
        {
            char *r = tbl + (int)base;
            *(float *)(r + 0x68) = *(float *)(f + 0x1028);
            *(float *)(r + 0x64) = *(float *)(f + 0x1024);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020B310), where it is exact; names translated to the US level program. */

extern char *D_L00_001ABD80[] __asm__("D_L00_001ABA00");
extern char D_L00_0017A780[] __asm__("D_L00_0017A680");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_c(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern int D_L00_0015F6B0 __asm__("D_L00_0015F5CC");
extern int FUN_001efa68_ac98(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int suck_get_aim_height(char *) __asm__("FUN_002141f8");
extern unsigned char hero_pos[] __asm__("D_0013F3D0") NOT_SDA;
extern void FUN_L00_00233660(float *, float, float, float);
float absolute_float(float input) __asm__("FUN_001f99c0");
void FUN_L00_0020a1e0(float a, float b);
void FUN_L00_0020a228(float a, float b);
void FUN_L00_0020a210(float a, float b);

void FUN_L00_0020ac98(void) {
    char *g1 = (char *)hero_pos - 0x80;
    char *g2, *g3, *g4, *g5, *g6;
    int i;
    char *m;
    char *best;
    float bestPri, bestDist;
    float v[8];
    char *q;
    char *info, *info2, *p, *t;
    float yaw_delta, pitch, s;
    int mode = *(int *)(g1 + 0x208C);
    if (mode != 1 && mode != 4 && mode != 2 && mode != 7 && mode != 0) {
        *(int *)(g1 + 0x2274) = 0;
        return;
    }
    g2 = (char *)hero_pos - 0x80;
    if (*(unsigned char *)(*(int *)(g2 + 0x2080) + 0x53) == 0x54)
        return;
    if (D_L00_0015F6B0 % 7 == 0) {
        bestDist = 100000000.0f;
        bestPri = 0.0f;
        best = 0;
        for (i = 0; (m = D_L00_001ABD80[i]) != 0; i++) {
            info = (char *)suck_get_aim_height(m);
            if (info != 0) {
                float d, a, sc, pri;
                d = vector_distance(hero_pos + 0x0, m + 0x10);
                if ((float)*(unsigned char *)(info + 0x38) < d)
                    continue;
                g3 = (char *)hero_pos - 0x80;
                a = fast_difference_between_rotations(
                    FUN_001f9e90_c(*(float *)(m + 0x10) - *(float *)(g3 + 0x80),
                                   *(float *)(m + 0x14) - *(float *)(g3 + 0x84)),
                    *(float *)(g3 + 0x98));
                if (1.91986215f < a)
                    continue;
                if (1.04719758f <
                    absolute_float(FUN_001f9e90_c(vector_distance(m + 0x10, hero_pos + 0x0),
                                                 *(float *)(g3 + 0x88) - *(float *)(m + 0x18))))
                    continue;
                sc = a * d + d;
                if ((int)m == *(int *)(g3 + 0x2274)) {
                    sc -= 1.5f;
                    if (sc < 0.0f)
                        sc = 0.0f;
                }
                pri = (float)*(unsigned char *)(info + 0x39);
                if (bestPri < pri)
                    goto upd;
                if (sc < bestDist) {
                upd:
                    if (pri > bestPri)
                        bestPri = pri;
                    bestDist = sc;
                    best = m;
                }
            }
        }
        g4 = (char *)hero_pos - 0x80;
        *(char **)(g4 + 0x2274) = best;
    }
    g5 = (char *)hero_pos - 0x80;
    if (*(int *)(g5 + 0x2274) == 0)
        return;
    FUN_L00_00233660(v, 0.0f, 0.0f, 1.2f);
    q = (char *)&v[4];
    qcopy(q, *(char **)(g5 + 0x2274) + 0x10);
    v[6] += 0.5f;
    if (FUN_001efa68_ac98(v, q, 2, 0, 0))
        *(int *)(g5 + 0x2274) = 0;
    t = *(char **)(g5 + 0x2274);
    if (t == 0)
        return;
    qcopy(v, t + 0x10);
    info2 = (char *)suck_get_aim_height(t);
    if (info2 != 0)
        v[2] += (float)*(unsigned char *)(info2 + 0x3A) * 0.125f;
    FUN_L00_00233660(&v[4], 0.3f, 0.0f, 0.85f);
    yaw_delta = fast_subtract_rotations(FUN_001f9e90_c(v[0] - v[4], v[1] - v[5]), *(float *)(g5 + 0x98));
    pitch = FUN_001f9e90_c(vector_distance(v, q), v[6] - v[2]);
    if (yaw_delta > 1.02974427f)
        yaw_delta = 1.02974427f;
    else if (yaw_delta < -1.02974427f)
        yaw_delta = -1.02974427f;
    if (pitch > 0.610865235f)
        pitch = 0.610865235f;
    if (pitch < -0.436332315f)
        pitch = -0.436332315f;
    g6 = (char *)hero_pos - 0x80;
    if (scale_game_frames(0x4D) - scale_game_frames(0x32) < *(int *)(g6 + 0x1C0)) {
        if (yaw_delta > 0.296705961f)
            yaw_delta = 0.296705961f;
        else if (yaw_delta < -0.296705961f)
            yaw_delta = -0.296705961f;
        pitch = 0.0f;
    }
    FUN_L00_0020a228(frame_scale_sq * 0.021f, frame_scale_sq * 0.27f);
    p = D_L00_0017A780;
    *(float *)(p + 0x274) = pitch;
    *(float *)(p + 0x278) = yaw_delta * 0.7f;
    FUN_L00_0020a210(frame_scale_sq * 0.014f, frame_scale_sq * 0.3f);
    *(float *)(p + 0x1C8) = *(float *)(p + 0x278) * 0.55f;
}
typedef struct {
    u8 p0[0x2C0];
    f32 f2C0, f2C4, f2C8, f2CC, f2D0, f2D4;
    u8 p2D8[0x308 - 0x2D8];
    s16 h308;
    u8 p30A[0x418 - 0x30A];
    s32 i418;
    u8 p41C[0x10E0 - 0x41C];
    s32 i10E0;
    u8 p10E4[0x12A0 - 0x10E4];
    f32 f12A0, f12A4;
    s32 i12A8;
    s16 h12AC;
    u16 h12AE;
    u8 p12B0[0x2080 - 0x12B0];
    u8 *p2080;
    s32 i2084;
    u8 p2088[4];
    s32 i208C;
} P_20b1d8;
typedef struct {
    u8 p0[0x320];
    f32 f320, f324;
    u8 p328[0x36C - 0x328];
    f32 f36C;
    u8 p370[0x3D0 - 0x370];
    f32 f3D0, f3D4;
    u8 p3D8[0x41C - 0x3D8];
    f32 f41C;
    u8 p420[0x480 - 0x420];
    f32 f480;
    u8 p484[0x530 - 0x484];
    f32 f530;
} A_20b1d8;
extern P_20b1d8 D_0013F350_20b1d8 __asm__("D_0013F350");
extern A_20b1d8 D_L00_0017A680_20b1d8 __asm__("D_L00_0017A680");
#define P D_0013F350_20b1d8
#define A D_L00_0017A680_20b1d8
extern s32 FUN_L00_0020d568(void);
extern f32 FUN_001f99c0(f32);
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern void FUN_L00_00233288(s32, s32, s32, s32, f32, f32);
extern void moby_point_position(void *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_0026de90(void *, void *, f32);
void FUN_L00_0020b1d8(void) {
    u8 buf[0x10] __attribute__((aligned(16)));
    f32 a, b;
    u8 *q;
    s32 i;
    if (FUN_L00_0020d568())
        return;
    if (0.03f < FUN_001f99c0(P.f2C0) || 0.03f < FUN_001f99c0(P.f2C4) ||
        0.06981317f < FUN_001f99c0(P.f2C8) || 0.06981317f < FUN_001f99c0(P.f2CC) ||
        0.06981317f < FUN_001f99c0(P.f2D0) || 0.06981317f < FUN_001f99c0(P.f2D4)) {
        if (P.h308 != 1) {
            A.f324 = -P.f2C8;
            A.f3D4 = -P.f2CC;
            a = fast_arc_tan(0.2f, P.f2C0) * 1.2f;
            b = -fast_arc_tan(0.2f, P.f2C4) * 1.2f;
            if (0.7853982f < a)
                a = 0.7853982f;
            if (a < -0.17453292f)
                a = -0.17453292f;
            if (0.17453292f < b)
                b = 0.17453292f;
            if (b < -0.7853982f)
                b = -0.7853982f;
            A.f480 = a;
            A.f530 = b;
            A.f320 = -fast_subtract_rotations(P.f2D0, -a);
            A.f3D0 = -fast_subtract_rotations(P.f2D4, -b);
        }
    }
    if (P.i10E0) {
        A.f36C = 0.01f;
        A.f41C = 0.01f;
    }
    if (!(P.h12AE & 0x80)) {
        if (P.i2084 == 3)
            FUN_L00_00233288(1, 2, 1, 0, 0.021f, 0.002f);
        if (P.i208C == 4 && P.i418 > 0 && P.i418 < scale_game_frames(12))
            FUN_L00_00233288(1, 2, 0, 1, 0.016f, 0.002f);
        if (P.i12A8 <= 0)
            return;
        if (P.h12AC == 0)
            return;
        q = 0;
        if (P.h12AE & 1)
            q = P.p2080 + 0xC0;
        P.i12A8--;
        moby_point_position(P.p2080, 0x16, buf);
        for (i = 0; i < P.h12AC; i++)
            FUN_L00_0026de90(buf, q, P.f12A0 + random_float_between(-P.f12A4, P.f12A4));
        moby_point_position(P.p2080, 0x17, buf);
        for (i = 0; i < P.h12AC; i++)
            FUN_L00_0026de90(buf, q, P.f12A0 + random_float_between(-P.f12A4, P.f12A4));
    } else
        P.i12A8 = 0;
}
typedef unsigned int u128_20b5b8 __attribute__((mode(TI)));
typedef union {
    u128_20b5b8 q;
    f32 f[4];
} V4_20b5b8;
typedef struct {
    u8 pad[0x40];
    f32 x, y;
} S2_20b5b8;

extern float D_0015ED6C;
extern V4_20b5b8 coll_hit_normal[] __asm__("D_L00_00173E80");

extern f32 gravity_z(void *) __asm__("FUN_L00_00233a78");
extern s32 FUN_L00_0020c758_20b5b8(void *, void *) __asm__("FUN_L00_0020c758");
extern s32 FUN_L00_0020a240_20b5b8(s32, f32, f32) __asm__("FUN_L00_0020a240");
extern void FUN_L00_002335c8_20b5b8(void *, f32, f32, f32) __asm__("FUN_L00_002335c8");
extern s32 FUN_001efa68_20b5b8(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");
extern s32 collision_hit_material(void) __asm__("FUN_001f0b58");
extern f32 FUN_L00_00233d30_20b5b8(void *) __asm__("FUN_L00_00233d30");
extern f32 FUN_001fa688_20b5b8(f32, f32) __asm__("FUN_001fa688");
extern f32 FUN_001f9b80_20b5b8(void *, void *) __asm__("FUN_001f9b80");

void FUN_L00_0020b5b8(void) {
    V4_20b5b8 v, w, z;
    struct Hero *g;
    struct Hero *h;
    struct Hero *k2;
    u8 *pz;
    s32 t;
    s32 n;
    f32 f;
    f32 r;
    f32 s;
    f32 a, b, c, d;

    g = &hero;
    if (g->ground_distance < 0.4f) {
        return;
    }
    if (gravity_z(&g->motion.velocity) < D_0015ED6C * -20.0f) {
        return;
    }
    if (g->unk4E8 != 0) {
        g->unk1B4 = 0;
        return;
    }
    qcopy(&v, &g->motion.pos);
    t = scale_game_frames(0x14);
    {
        f32 st = (f32)t;
        a = g->unk4A0 * st;
        b = g->motion.velocity.f[2] * st;
        c = g->motion.displacement.f[0] * st;
        d = g->motion.displacement.f[1] * st;
        v.f[2] = v.f[2] + b - a * st * 0.5f;
    }
    v.f[0] = v.f[0] + c;
    v.f[1] = v.f[1] + d;
    if (FUN_L00_0020c758_20b5b8(&v, &g->motion.rot.f[2]) != 0) {
        g->unk1B4 = 0;
        g->unk1F0 = t + 2;
        return;
    }
    if (FUN_L00_0020a240_20b5b8(0, 0.3f, 1.0f) == 0) {
        return;
    }
    r = 1.7f;
    if (g->state.current != 0x11) {
        if (g->unk255 == 0 || g->unk254 != 0) {
            r = 3.0f;
        }
    }
    k2 = &hero;
    FUN_L00_002335c8_20b5b8(&v, 0.0f, 0.0f, r);
    FUN_L00_002335c8_20b5b8(&w, k2->unk234 + 1.4f, 0.0f, r);
    if (FUN_001efa68_20b5b8(&v, &w, 2, 0, 0) == 0) {
        return;
    }
    n = collision_hit_material();
    if (n == 0xA) {
        return;
    }
    if (n == 0xC) {
        return;
    }
    pz = (u8 *)coll_hit_normal;
    if (FUN_L00_00233d30_20b5b8(pz) < 1.3089969f) {
        return;
    }
    transform_vector(&z, pz, k2->unk40);
    f = angle_atan2(z.f[0], z.f[1]);
    f = FUN_001fa688_20b5b8(f, 3.1415927f);
    if (0.87266463f < f) {
        return;
    }
    if (k2->state.current == 0x11) {
        s = 0.95f;
    } else {
        s = 0.72f;
    }
    f = FUN_001f9b80_20b5b8(pz - 0x20, &v);
    if (s < f) {
        return;
    }
    {
        S2_20b5b8 *q2;
        k2->unk460.q = *(u128_20b5b8 *)pz;
        q2 = (S2_20b5b8 *)(pz - 0x40);
        f = angle_atan2(q2->x, q2->y);
    }
    k2->unk460.f[3] = f;
    if (k2->state.control_mode != 4 && k2->state.prev_control_mode != 4 && k2->state.prev2_control_mode != 4) {
        return;
    }
    h = &hero;
    if (h->unk22B4 != 0) {
        f = FUN_001fa688_20b5b8(h->unk470.f[3], h->unk460.f[3]);
        if (f < 2.9670596f) {
            return;
        }
    }
    if (h->state.current == 0x11) {
        h->unk1B4 = scale_game_frames(7);
    } else {
        h->unk1B4 = scale_game_frames(6);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020BFA8), where it is exact; names translated to the US level program. */

extern void FUN_L00_00210b30(void);

/* Calls FUN_L00_00210b30 when the byte at 0x20A4 of ((char *)&hero) is 3. */
void FUN_L00_0020b930(void) {
    if (hero.vehicle_mode == 3) {
        FUN_L00_00210b30();
    }
}
typedef struct {
    u8 pad0[0x88];
    f32 f88;
    u8 pad8C[0x7C];
    f32 f108;
    u8 pad10C[0x8C];
    s32 i198;
    u8 pad19C[0x98];
    f32 f234;
    u8 pad238[0xA0];
    f32 f2D8;
    f32 f2DC;
    u8 pad2E0[0x10];
    f32 f2F0;
    f32 f2F4;
    u8 pad2F8[0x8];
    s32 i300;
    u8 pad304[0x4];
    s16 h308;
    u8 pad30A[0x2];
    s16 h30C;
    u8 pad30E[0x110];
    s16 h41E;
    u8 pad420[0xCC0];
    u8 *p10E0;
    u8 pad10E4[0x1FC];
    s16 h12E0;
    u8 b12E2;
    u8 b12E3;
    u8 b12E4;
    u8 b12E5;
    u8 b12E6;
    u8 b12E7;
    u8 pad12E8[0x2];
    u8 b12EA;
    u8 b12EB;
    u8 b12EC;
    u8 b12ED;
    u8 b12EE;
    u8 pad12EF[0xD95];
    s32 i2084;
    u8 pad2088[0x4];
    s32 i208C;
    u8 pad2090[0x14];
    u8 b20A4;
    u8 pad20A5[0x4];
    u8 b20A9;
    u8 pad20AA[0x1FA];
    f32 f22A4;
    s32 i22A8;
} P20b960;
extern P20b960 D_0013F350_20b960 __asm__("D_0013F350");
void fast_mem_zero16(void *, int) __asm__("FUN_001f9810");
int coll_sphere(void *, int, int, f32) __asm__("FUN_L00_001f0d60");
int FUN_001f0b58_20b960(void) __asm__("FUN_001f0b58");
f32 FUN_001f99c0_20b960(f32) __asm__("FUN_001f99c0");
void FUN_L00_0020b930_20b960(void) __asm__("FUN_L00_0020b930");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
#define P D_0013F350_20b960
void FUN_L00_0020b960(void) {
    u8 save = P.b12ED;
    s16 t = P.h12E0;
    f32 d;
    int s;
    fast_mem_zero16(&P.h12E0, 0x10);
    P.b12ED = save;
    P.h12E0 = -1;
    P.b20A9 = 0;
    P.h308 = 0;
    if (t == -1)
        return;
    if (t == 2) {
        if (P.h30C == 0 || P.f2DC < 0.3f) {
            P.b12E7 = 1;
            if (P.b20A4 == 0 && P.p10E0 != 0 && *(s16 *)(P.p10E0 + 0xA6) == 0xAD)
                P.h308 = 1;
        }
    }
    if (t == 14) {
        P.b12E4 = 1;
        P.f22A4 = 0.2f;
        P.f2F0 = P.f2D8 + 0.2f;
    }
    if (t == 0) {
        d = P.f2F0 - P.f2D8;
        P.f22A4 = d;
        if (d < 0.85f && 0.25f < d)
            P.b20A9 = 1;
        P.b12E4 = 1;
    }
    if (t == 3)
        P.b12E6 = 1;
    if (current_level_index == 13 && P.i2084 != 0x7B &&
        coll_sphere((u8 *)&P + 0xD0, 2, 0, P.f234 + 0.03f) &&
        FUN_001f0b58_20b960() == 11) {
        goto s7B;
    }
    if (t == 11)
        P.b12EB = 1;
    if (t == 2)
        P.b12E7 = 1;
    if (t == 4)
        P.b12E3 = 1;
    if (t == 7)
        P.b12E2 = 1;
    if (t == 13)
        P.b12EC = 1;
    if (t == 1)
        P.b12E5 = 1;
    if (t == 8)
        P.b12EA = 1;
    if (t == 9)
        P.b12EE = 1;
    if (t == 12)
        P.b12EA = 1;
    if (P.b12E3 && P.i300 && P.i208C != 0x10 && P.i208C != 0x14 && P.i208C != 7) {
        s = 0x31;
        goto call;
    }
    if (P.b12E5 && P.i300 && P.b20A4 == 1 && P.i2084 != 0x7D) {
        s = 0x7D;
        goto call;
    }
    if (P.b12E5 && P.i300 && (P.i2084 != 0x3C || P.h41E != 0)) {
        FUN_L00_0020b930_20b960();
        if (P.i22A8)
            goto call3c;
        s = 0x7C;
        goto call;
    }
    if (P.b12EC && P.i2084 != 0x7F && FUN_001f99c0_20b960(P.f2F0 - (P.f88 + 0.25f)) < 1.0f &&
        0.0f < P.f2F0 - P.f88 && P.f108 < 0.0f) {
        FUN_L00_0020b930_20b960();
        s = 0x7F;
        goto call;
    }
    if (P.b12E6 && P.i2084 != 0x68 && P.i2084 != 0x7B &&
        FUN_001f99c0_20b960(P.f2F4 - (P.f88 + 0.25f)) < 1.0f && 0.0f < P.f2F4 - P.f88 &&
        (P.i2084 != 0x69 || P.h41E != 0) && P.f108 < 0.0f) {
        FUN_L00_0020b930_20b960();
        if (P.i22A8) {
            s = 0x68;
            goto call;
        }
    s7B:
        s = 0x7B;
    call:
        hero_set_state(s, 1);
        return;
    call3c:
        s = 0x3C;
        goto call;
    }
    if (P.b12EB && P.i2084 != 0x7B && FUN_001f99c0_20b960(P.f2F4 - (P.f88 + 0.25f)) < 1.0f &&
        0.0f < P.f2F4 - P.f88 && P.f108 < 0.0f) {
        FUN_L00_0020b930_20b960();
        hero_set_state(0x7B, 1);
    }
}
#undef P
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

/* the ledge probe of the hero: looks for an edge ahead to grab, and where it finds one stores the grab point and direction */
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020C568), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    char p0[0x18];
    char *m18;
    char p1C[4];
    float v20[4];
    char p30[0x10];
    float v40[4];
} Hit_20C568;

typedef struct {
    char p0[0x1E];
    unsigned short h1E;
    char p20[0x1C];
    int i3C;
} Obj_20C568;

extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED6C;
extern float fast_difference_between_rotations_c(float, float) __asm__("func_001FA688");
extern int FUN_001efa68_c(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern int rig_probe() __asm__("FUN_L00_001f19a0");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern unsigned char D_0013F350[];
f32 FUN_001f9e90_c2(f32, f32) __asm__("FUN_001f9e90");
void FUN_L00_00259508(void *a, void *b, void *c, int d, float f);
extern char D_0013F450_20C568[] __asm__("D_0013F350");
extern int D_L00_00173E40[];
extern float D_0015ED6C;
extern int func_001F9850_20C568(int) __asm__("FUN_001f96f8");
extern void func_L00_0025A540_20C568(void *a, void *b, void *c, int d, float f) __asm__("FUN_L00_00259508");
extern float func_001F9F90_20C568(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_20C568(float) __asm__("FUN_001f9de0");
extern int func_L00_001EFFF0_20C568(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int func_L00_001F3958_20C568(void) __asm__("FUN_001f0b58");
extern float func_001F9CE8_20C568(void *) __asm__("FUN_001f9b20");
extern float func_L00_001FF860_20C568(float, float) __asm__("FUN_001f9e90");
extern Obj_20C568 *func_L00_0025D390_20C568(char *) __asm__("FUN_002141f8");
extern Obj_20C568 *FUN_L00_002603d0(char *);
extern int func_L00_001F1D20_20C568(float, float, void *, int, int) __asm__("FUN_L00_001f19a0");
extern float func_001FA748_20C568(float, float) __asm__("FUN_001fa580");
extern float func_001FA850_20C568(float, float) __asm__("FUN_001fa688");

void FUN_L00_0020bed0(void) {
    float probe[4], ledge[4], ray_end[4], hero_level[4], clearance[4], edge_start[4], edge_end[4], grab_point[4];
    float down, fwd, up, top, drop, rise, back;
    float speed, ledge_yaw, offset;
    int can_grab, found, edge, i, j, surface, grabbable, frames;
    Obj_20C568 *moby_info;
    Obj_20C568 *moby_props;

    if (((Hero *)D_0013F450_20C568)->unk308 == 2) return;
    ((Hero *)D_0013F450_20C568)->unk4E8 = 0;
    if (((Hero *)D_0013F450_20C568)->unk1B0 != 0) return;
    can_grab = 0;
    if (((Hero *)D_0013F450_20C568)->vehicle_mode == 0) {
        if ((((Hero *)D_0013F450_20C568)->state.control_mode == 4 && (((Hero *)D_0013F450_20C568)->unk41E != 0 || ((Hero *)D_0013F450_20C568)->motion.velocity.f[2] < D_0015ED6C * 3.5f)) || ((Hero *)D_0013F450_20C568)->state.control_mode == 2 || ((Hero *)D_0013F450_20C568)->state.current == 8 || ((Hero *)D_0013F450_20C568)->state.current == 0x1A || ((Hero *)D_0013F450_20C568)->state.current == 0x1B) can_grab = 1;
        if (((Hero *)D_0013F450_20C568)->state.current == 0xE && ((Hero *)D_0013F450_20C568)->anim_frame < 37.0f) can_grab = 0;
    }
    if (((Hero *)D_0013F450_20C568)->vehicle_mode == 1) {
        if ((((Hero *)D_0013F450_20C568)->state.control_mode == 4 && (((Hero *)D_0013F450_20C568)->unk41E != 0 || ((Hero *)D_0013F450_20C568)->motion.velocity.f[2] < D_0015ED6C * 2.5f)) || ((Hero *)D_0013F450_20C568)->state.control_mode == 2 || ((Hero *)D_0013F450_20C568)->state.current == 0x4F || ((Hero *)D_0013F450_20C568)->state.current == 0x4D || ((Hero *)D_0013F450_20C568)->state.current == 0x4E) can_grab = 1;
    }
    if (!can_grab) return;

    down = -0.7f;
    fwd = 0.3f;
    up = 1.5f;
    top = 1.15f;
    drop = -1.43f;
    rise = 1.8f;
    back = 0.45f;
    if (((Hero *)D_0013F450_20C568)->vehicle_mode == 1) {
        down = -0.4f;
        fwd = 0.25f;
        up = 0.725f;
        top = 0.7f;
        drop = -0.71f;
        rise = 0.7f;
        back = 0.32f;
    }
    speed = ((Hero *)D_0013F450_20C568)->unk940;
    if (((Hero *)D_0013F450_20C568)->state.control_mode == 4) speed = ((Hero *)D_0013F450_20C568)->unk4A0;
    frames = func_001F9850_20C568(2);
    func_L00_0025A540_20C568(probe, ((Hero *)D_0013F450_20C568)->motion.pos.f, ((Hero *)D_0013F450_20C568)->motion.unk110.f, frames, speed);
    found = 0;
    probe[0] += func_001F9F90_20C568(((Hero *)D_0013F450_20C568)->motion.rot.f[2]) * ((Hero *)D_0013F450_20C568)->unk234;
    probe[1] += func_001F9FA8_20C568(((Hero *)D_0013F450_20C568)->motion.rot.f[2]) * ((Hero *)D_0013F450_20C568)->unk234;
    for (i = 0; !found && i < 4; i++) {
        probe[0] += func_001F9F90_20C568(((Hero *)D_0013F450_20C568)->motion.rot.f[2]) * fwd * 0.25f;
        probe[1] += func_001F9FA8_20C568(((Hero *)D_0013F450_20C568)->motion.rot.f[2]) * fwd * 0.25f;
        qcopy(ledge, probe);
        ledge[2] += up + 0.15f;
        qcopy(ray_end, probe);
        ray_end[2] += top;
        if (func_L00_001EFFF0_20C568(ledge, ray_end, 4, (int)((Hero *)D_0013F450_20C568)->moby, 0)) {
            surface = func_L00_001F3958_20C568();
            if (surface != 9 && surface != 12 && surface != 0) {
                if (func_L00_001FF860_20C568(((Hit_20C568 *)((char *)D_L00_00173E40))->v40[2], func_001F9CE8_20C568(((Hit_20C568 *)((char *)D_L00_00173E40))->v40)) < 0.3490658402442932f) found = 1;
            }
        }
    }
    if (!found) return;

    if (((Hit_20C568 *)((char *)D_L00_00173E40))->m18 != 0) {
        grabbable = 0;
        moby_info = func_L00_0025D390_20C568(((Hit_20C568 *)((char *)D_L00_00173E40))->m18);
        moby_props = FUN_L00_002603d0(((Hit_20C568 *)((char *)D_L00_00173E40))->m18);
        if (moby_info != 0 && (moby_info->h1E & 1)) {
            grabbable = 1;
        } else if (moby_props != 0) {
            grabbable = moby_props->i3C & 1;
        }
        if (!grabbable) return;
    }
    ((Hero *)D_0013F450_20C568)->unk4F8 = (s32)(((Hit_20C568 *)((char *)D_L00_00173E40))->m18);
    if (((Hero *)D_0013F450_20C568)->motion.pos.f[2] + up < ((Hit_20C568 *)((char *)D_L00_00173E40))->v20[2]) return;
    qcopy(ledge, ((Hit_20C568 *)((char *)D_L00_00173E40))->v20);
    qcopy(ray_end, ((Hit_20C568 *)((char *)D_L00_00173E40))->v20);
    ray_end[2] += 0.05f;
    qcopy_nc(hero_level, ((Hero *)D_0013F450_20C568)->motion.pos.f);
    hero_level[2] = ray_end[2];
    if (func_L00_001EFFF0_20C568(hero_level, ray_end, 4, (int)((Hero *)D_0013F450_20C568)->moby, 0)) return;
    qcopy(clearance, ((Hit_20C568 *)((char *)D_L00_00173E40))->v20);
    clearance[2] += 0.6f;
    if (func_L00_001F1D20_20C568(0.45f, 0.6f, clearance, 4, (int)((Hero *)D_0013F450_20C568)->moby)) {
        ((Hero *)D_0013F450_20C568)->unk4F4 |= 1;
    } else {
        ((Hero *)D_0013F450_20C568)->unk4F4 &= 0xFFFFFFFE;
    }
    ledge_yaw = func_L00_001FF860_20C568(ledge[0] - ((Hero *)D_0013F450_20C568)->motion.pos.f[0], ledge[1] - ((Hero *)D_0013F450_20C568)->motion.pos.f[1]);
    ((Hero *)D_0013F450_20C568)->unk4E0 = ledge[2];
    if (ledge[2] - ((Hero *)D_0013F450_20C568)->ground_z.f < rise) return;

    edge = 0;
    for (j = 0; j < 5; j++) {
        float len;

        qcopy(edge_start, ((Hero *)D_0013F450_20C568)->motion.pos.f);
        qcopy(edge_end, ledge);
        len = ((Hero *)D_0013F450_20C568)->unk234 * 2.0f;
        edge_end[0] += func_001F9F90_20C568(ledge_yaw) * len;
        edge_end[1] += func_001F9FA8_20C568(ledge_yaw) * len;
        edge_end[2] += down * (1.0f - (float)j / 5.0f);
        edge_start[2] = edge_end[2];
        if (func_L00_001EFFF0_20C568(edge_start, edge_end, 2, 0, 0)) {
            edge = 1;
            ((Hero *)D_0013F450_20C568)->unk4E4 = func_L00_001FF860_20C568(((Hit_20C568 *)((char *)D_L00_00173E40))->v40[0], ((Hit_20C568 *)((char *)D_L00_00173E40))->v40[1]);
            break;
        }
    }
    if (!edge) return;
    if (!(func_001FA850_20C568(((Hero *)D_0013F450_20C568)->motion.rot.f[2], func_001FA748_20C568(((Hero *)D_0013F450_20C568)->unk4E4, 3.1415927f)) < 1.1344640f)) return;
    {
        float max = 2.4f;
        float step = 0.03f;

        for (offset = 0.0f; offset < max; offset += step) {
            qcopy(edge_end, ledge);
            edge_end[2] = ledge[2];
            edge_end[0] += func_001F9F90_20C568(((Hero *)D_0013F450_20C568)->unk4E4) * offset;
            edge_end[1] += func_001F9FA8_20C568(((Hero *)D_0013F450_20C568)->unk4E4) * offset;
            qcopy(edge_start, edge_end);
            edge_end[2] += 0.5f;
            edge_start[2] -= 0.5f;
            if (!func_L00_001EFFF0_20C568(edge_end, edge_start, 4, (int)((Hero *)D_0013F450_20C568)->moby, 0)) {
                qcopy(grab_point, edge_end);
                offset = step * 0.5f;
                offset = -offset;
                grab_point[2] = ledge[2];
                grab_point[0] += func_001F9F90_20C568(((Hero *)D_0013F450_20C568)->unk4E4) * offset;
                grab_point[1] += func_001F9FA8_20C568(((Hero *)D_0013F450_20C568)->unk4E4) * offset;
                qcopy(((Hero *)D_0013F450_20C568)->unk4D0.f, grab_point);
                ((Hero *)D_0013F450_20C568)->unk4D0.f[0] += func_001F9F90_20C568(((Hero *)D_0013F450_20C568)->unk4E4) * back;
                ((Hero *)D_0013F450_20C568)->unk4D0.f[1] += func_001F9FA8_20C568(((Hero *)D_0013F450_20C568)->unk4E4) * back;
                ((Hero *)D_0013F450_20C568)->unk4E8 = 1;
                ((Hero *)D_0013F450_20C568)->unk4D0.f[2] += drop;
                return;
            }
        }
    }
}

s16 D_L00_0017A722[1] __attribute__((section(".data"))) = {0};
f32 D_L00_0017A724[1] __attribute__((section(".data"))) = {0};
f32 D_L00_0017A728[1] __attribute__((section(".data"))) = {0};
s32 D_L01_001798CC[1] __attribute__((section(".data"))) = {0};
s16 D_L01_0017ABA2[1] __attribute__((section(".data"))) = {0};
f32 D_L01_0017ABA4[1] __attribute__((section(".data"))) = {0};
f32 D_L01_0017ABA8[1] __attribute__((section(".data"))) = {0};
s32 D_L02_00179A4C[1] __attribute__((section(".data"))) = {0};
f32 D_L02_0017AD24[1] __attribute__((section(".data"))) = {0};
f32 D_L02_0017AD28[1] __attribute__((section(".data"))) = {0};
s32 D_L03_0017954C[1] __attribute__((section(".data"))) = {0};
s16 D_L03_0017A822[1] __attribute__((section(".data"))) = {0};
f32 D_L03_0017A824[1] __attribute__((section(".data"))) = {0};
f32 D_L03_0017A828[1] __attribute__((section(".data"))) = {0};
s16 D_L04_0017A8A2[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0017A8A4[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0017A8A8[1] __attribute__((section(".data"))) = {0};
s32 D_L05_0017984C[1] __attribute__((section(".data"))) = {0};
s16 D_L05_0017AB82[1] __attribute__((section(".data"))) = {0};
f32 D_L05_0017AB84[1] __attribute__((section(".data"))) = {0};
f32 D_L05_0017AB88[1] __attribute__((section(".data"))) = {0};
s16 D_L06_0017AEA2[1] __attribute__((section(".data"))) = {0};
f32 D_L06_0017AEA4[1] __attribute__((section(".data"))) = {0};
f32 D_L06_0017AEA8[1] __attribute__((section(".data"))) = {0};
s32 D_L07_001794CC[1] __attribute__((section(".data"))) = {0};
s16 D_L07_0017A7A2[1] __attribute__((section(".data"))) = {0};
f32 D_L07_0017A7A4[1] __attribute__((section(".data"))) = {0};
f32 D_L07_0017A7A8[1] __attribute__((section(".data"))) = {0};
s32 D_L08_00179C4C[1] __attribute__((section(".data"))) = {0};
f32 D_L08_0017AF24[1] __attribute__((section(".data"))) = {0};
f32 D_L08_0017AF28[1] __attribute__((section(".data"))) = {0};
s32 D_L09_001795CC[1] __attribute__((section(".data"))) = {0};
s16 D_L09_0017A8A2[1] __attribute__((section(".data"))) = {0};
f32 D_L09_0017A8A4[1] __attribute__((section(".data"))) = {0};
f32 D_L09_0017A8A8[1] __attribute__((section(".data"))) = {0};
s16 D_L10_0017ABA2[1] __attribute__((section(".data"))) = {0};
f32 D_L10_0017ABA4[1] __attribute__((section(".data"))) = {0};
f32 D_L10_0017ABA8[1] __attribute__((section(".data"))) = {0};
s32 D_L11_00179E4C[1] __attribute__((section(".data"))) = {0};
s32 D_L12_001798CC[1] __attribute__((section(".data"))) = {0};
s16 D_L12_0017ABA2[1] __attribute__((section(".data"))) = {0};
f32 D_L12_0017ABA4[1] __attribute__((section(".data"))) = {0};
f32 D_L12_0017ABA8[1] __attribute__((section(".data"))) = {0};
f32 D_L13_0017AA24[1] __attribute__((section(".data"))) = {0};
f32 D_L13_0017AA28[1] __attribute__((section(".data"))) = {0};
s32 D_L14_00179B4C[1] __attribute__((section(".data"))) = {0};
f32 D_L14_0017AE24[1] __attribute__((section(".data"))) = {0};
f32 D_L14_0017AE28[1] __attribute__((section(".data"))) = {0};
f32 D_L15_0017AD24[1] __attribute__((section(".data"))) = {0};
f32 D_L15_0017AD28[1] __attribute__((section(".data"))) = {0};
s32 D_L16_0017984C[1] __attribute__((section(".data"))) = {0};
s16 D_L16_0017AB82[1] __attribute__((section(".data"))) = {0};
f32 D_L16_0017AB84[1] __attribute__((section(".data"))) = {0};
f32 D_L16_0017AB88[1] __attribute__((section(".data"))) = {0};
f32 D_L17_0017B024[1] __attribute__((section(".data"))) = {0};
f32 D_L17_0017B028[1] __attribute__((section(".data"))) = {0};
