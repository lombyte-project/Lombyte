/* Shared moby classes: trespasser, swingshot_hook, doom_bot, mine_glove, bomb_glove, decoy, devestator_ammo, swingshot, glove_of_doom. */
#include "types.h"
#include "rnc/gameplay/state/item_state.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/quad.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/collision.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C2A20), where it is exact; names translated to the US level program. */

extern unsigned char D_0013A4E0[];
extern unsigned char D_0013E533[];
extern void FUN_L00_002c1778_u(void *) __asm__("FUN_L00_002c1778");

/* Runs FUN_L00_002c1778 when the moby data word 4 is set, else sets byte 0xBC from a mask test. */
void FUN_L00_002c1718(char *m) {
    if (*(int *)(*(char **)(m + 0x78) + 4) != 0) {
        FUN_L00_002c1778_u(m);
    } else if (*(int *)(D_0013A4E0 + 0x2600) & *(int *)(D_0013E533 + 0x1EBD)) {
        m[0xBC] = 1;
    } else {
        m[0xBC] = 0;
    }
}
extern float FUN_001f9b48_2c1928(void *, void *) __asm__("FUN_001f9b48");
extern void FUN_001f9a28_2c1928(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9bf8_2c1928(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_001f9a10_2c1928(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001f9b80_2c1928(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_2c1928(float, float) __asm__("FUN_001f9e90");
extern float FUN_001f9988_2c1928(float) __asm__("FUN_001f9988");
extern float FUN_001f9dc8_2c1928(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2c1928(float) __asm__("FUN_001f9de0");

void FUN_L00_002c1928(float *p, OvlQuad *q, float *out, float a, float b, float c) {
    OvlQuad v[1];
    float half = a * c * 0.5f;
    float d, dz, ang, t, s;

    if (c < FUN_001f9b48_2c1928(p, q)) {
        FUN_001f9a28_2c1928(v, q, p);
        FUN_001f9bf8_2c1928(v, v, c - 0.01f);
        FUN_001f9a10_2c1928(v, v, p);
    } else {
        v[0] = *q;
    }
    d = FUN_001f9b80_2c1928(p, v);
    dz = ((float *)v)[2] - p[2];
    ang = FUN_001f9e90_2c1928(((float *)v)[0] - p[0], ((float *)v)[1] - p[1]);
    if (d < b)
        d = b;
    if (d <= dz)
        dz = d - 0.01f;
    t = a * d * d / ((d - dz) + (d - dz));
    if (half < t)
        t = half;
    s = FUN_001f9988_2c1928(t);
    out[0] = FUN_001f9dc8_2c1928(ang) * s;
    out[1] = FUN_001f9de0_2c1928(ang) * s;
    out[2] = s;
}
#include "qcopy.h"
float FUN_002132a8_2c3440(float, float) __asm__("FUN_002132a8");
int FUN_001160d8_2c3440(void) __asm__("FUN_001160d8");
int FUN_001f96f8_2c3440(int) __asm__("FUN_001f96f8");
void FUN_L00_00269b70_2c3440(float *, unsigned char, unsigned char, unsigned char, float, float,
                             int) __asm__("FUN_L00_00269b70");
void FUN_L00_0025f800_2c3440(void *, void *) __asm__("FUN_L00_0025f800");
void FUN_0020c828_2c3440(void *) __asm__("FUN_0020c828");
extern s16 D_L00_001B0830_lui[] __asm__("D_L00_001B0830") __attribute__((section(".data")));
void FUN_L00_002c3440(unsigned char *m) {
    float v[4] __attribute__((aligned(16)));
    int i;
    int r, g;
    unsigned char b;
    float s;
    for (i = 0; i < 10; i++) {
        qcopy(v, m + 0x10);
        v[0] += FUN_002132a8_2c3440(-0.2f, 0.2f);
        v[1] += FUN_002132a8_2c3440(-0.2f, 0.2f);
        v[2] += FUN_002132a8_2c3440(-0.2f, 0.2f);
        r = (FUN_001160d8_2c3440() + 0x30) & 0x3F;
        g = (FUN_001160d8_2c3440() + 0x20) & 0x3F;
        b = FUN_001160d8_2c3440() & 0x2F;
        s = FUN_002132a8_2c3440(100000.0f, 600000.0f);
        FUN_L00_00269b70_2c3440(v, r, g, b, s, 0.0f,
                                FUN_001f96f8_2c3440(FUN_001160d8_2c3440() % 40 + 10));
    }
    FUN_L00_0025f800_2c3440(m, D_L00_001B0830_lui);
    FUN_0020c828_2c3440(m);
}
typedef struct {
    u8 pad[0x60];
    f32 f60;
    f32 f64;
    s32 i68;
} O_2c40a0;
typedef struct {
    u8 pad[0x10];
    u8 v10[8];
    f32 f18;
    u8 pad2[0x78 - 0x1C];
    O_2c40a0 *p78;
} S_2c40a0;
typedef struct {
    u8 pad[0x80];
    u8 v80[8];
    f32 f88;
    u8 pad2[0x1E2 - 0x8C];
    s16 h1E2;
} P_2c40a0;
extern P_2c40a0 D_0013F350_2c40a0 __asm__("D_0013F350");
extern f32 D_0015ED6C_2c40a0 __asm__("D_0015ED6C");
extern f32 D_0015ED70_2c40a0 __asm__("D_0015ED70");
f32 FUN_001f9b80_2c40a0(void *, void *) __asm__("FUN_001f9b80");
f32 FUN_001f99c0_2c40a0(f32) __asm__("FUN_001f99c0");
void FUN_001f9a28_2c40a0(void *, void *, void *) __asm__("FUN_001f9a28");
f32 FUN_001f9b20_2c40a0(void *) __asm__("FUN_001f9b20");
f32 FUN_001f9af0_2c40a0(void *) __asm__("FUN_001f9af0");
f32 FUN_L00_0025b8c0_2c40a0(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
f32 FUN_00213508_2c40a0(void *, f32, s32) __asm__("FUN_00213508");
void FUN_001f9bf8_2c40a0(void *, void *, f32) __asm__("FUN_001f9bf8");
void FUN_L00_00258830_2c40a0(void *, void *, f32, f32, f32, int) __asm__("FUN_L00_00258830");
void FUN_L00_002c40a0(S_2c40a0 *self) {
    O_2c40a0 *o = self->p78;
    P_2c40a0 *P = &D_0013F350_2c40a0;
    s16 h = P->h1E2;
    f32 v[4];
    f32 s, len, g;
    if (o->i68 == 0) {
        if (h == 0)
            return;
        if (!(FUN_001f9b80_2c40a0(P->v80, self->v10) < 1.0f))
            return;
        if (!(FUN_001f99c0_2c40a0(P->f88 - self->f18) < 1.3f))
            return;
        o->i68 = 1;
    }
    FUN_001f9a28_2c40a0(v, self->v10, P->v80);
    v[2] = 0.0f;
    s = FUN_001f9b20_2c40a0(v) - 1.4f;
    if (s > 0.0f)
        s = 0.0f;
    if (h == 0)
        s = 0.0f;
    FUN_L00_0025b8c0_2c40a0(&s, 0.0f, &o->f60, frame_scale_sq * 0.02f, frame_scale_sq * 0.3f,
                            D_0015ED6C_2c40a0 * 4.0f);
    if (s == 0.0f && o->f60 == 0.0f) {
        if (FUN_001f99c0_2c40a0(self->f18 - FUN_00213508_2c40a0(self->v10, 0.5f, 0)) < 0.02f)
            o->i68 = 0;
    }
    len = FUN_001f9af0_2c40a0(v);
    if (len < o->f60)
        o->f60 = len;
    if (o->f60 < len)
        FUN_001f9bf8_2c40a0(v, v, o->f60);
    FUN_L00_00258830_2c40a0(self, v, 0.5f, 0.4f, 0.0f, 0);
    g = FUN_00213508_2c40a0(self->v10, 0.5f, 0);
    if (self->f18 < g) {
        self->f18 = g;
        o->f64 = 0.0f;
        return;
    }
    o->f64 -= D_0015ED70_2c40a0 * 24.0f;
    self->f18 += o->f64;
    if (self->f18 < g)
        self->f18 = g;
}
s32 FUN_001f0b58();
void FUN_L00_002c3440();
/* Calls FUN_L00_002c3440 on m for collision_hit_material results 0, 1, 3, 8, 11, 12 and 13. */
void FUN_L00_002c4328(void *m) {
    switch (FUN_001f0b58()) {
    case 0:
    case 1:
    case 3:
    case 8:
    case 11:
    case 12:
    case 13:
        FUN_L00_002c3440(m);
        break;
    }
}
#include "eetypes.h"
#include "qcopy.h"
typedef struct {
    u8 pad[0x3A];
    s16 h3A;
    u8 pad2[0x10];
} S179AC0_2c52a8;
extern S179AC0_2c52a8 D_L00_00179AC0_002c52a8[] __asm__("D_L00_00179AC0")
    __attribute__((section(".data")));
unsigned char *FUN_0020c4f8_002c52a8(int) __asm__("FUN_0020c4f8");
int FUN_001f96f8_002c52a8(int) __asm__("FUN_001f96f8");
void FUN_L00_00250df8_002c52a8(void *) __asm__("FUN_L00_00250df8");
void FUN_L00_0025d1b8_002c52a8(void *) __asm__("FUN_L00_0025d1b8");
unsigned char *FUN_L00_002c52a8(u128 *a, int b, int c, int d) {
    unsigned char *m = 0;
    {
        S179AC0_2c52a8 *e = (S179AC0_2c52a8 *)(b * 0x4C + (u8 *)D_L00_00179AC0_002c52a8);
        if (e->h3A > 0)
            m = FUN_0020c4f8_002c52a8(e->h3A);
    }
    if (m) {
        int *q;
        m[0x30] = 0xff;
        *(short *)(m + 0x32) = 0xff;
        q = *(int **)(m + 0x78);
        m[0x31] = 1;
        m[0x20] = 1;
        m[0xbc] = 0;
        qcopy(m + 0x10, a);
        if (c == -1)
            q[0] = ((VendorItemInfo *)(b * 0x18 + (u8 *)D_L00_001C40B0))->hC;
        else
            q[0] = c;
        q[2] = b;
        *(float *)(q + 3) = (float)d;
        q[8] = FUN_001f96f8_002c52a8(0x10);
        *(float *)(q + 10) = *(float *)(m + 0x18);
        FUN_L00_00250df8_002c52a8(m);
        FUN_L00_0025d1b8_002c52a8(m);
    }
    return m;
}
typedef struct {
    char pad[0x34];
    short h34;
    short h36;
    char pad2[0x14];
} E_2c5be8;
extern E_2c5be8 D_179AC0_2c5be8[] __asm__("D_L00_00179AC0") __attribute__((section(".data")));
extern float D_0015ED60_2c5be8 __asm__("D_0015ED60");
extern float D_0015ED6C_2c5be8 __asm__("D_0015ED6C");
extern int D_15F5CC_2c5be8 __asm__("D_L00_0015F5CC");
extern int f33e40_2c5be8(int, int) __asm__("FUN_L00_00233e40");
extern void f63db0_2c5be8(int, int, int) __asm__("FUN_L00_00263db0");
extern void f99f8_2c5be8(void *) __asm__("FUN_001f99f8");
extern int f99a8_2c5be8(int) __asm__("FUN_001f99a8");
extern int f96f8_2c5be8(int) __asm__("FUN_001f96f8");
extern int f8dc90_2c5be8(int, int, void *, int) __asm__("FUN_L00_0028dc90");

extern int D_0013DE08[];
extern int D_L00_00161838;
int FUN_L00_002c5be8(char *o) {
    int *p = *(int **)(o + 0x78);
    int before;
    float f;
    int d;

    if (*(unsigned char *)(o + 0x20) == 2) {
    fail:
        return 0;
    }
    before = weapon_ammo_counts[p[2]];
    f = f33e40_2c5be8(p[2], p[0]);
    if (before >= weapon_ammo_counts[p[2]]) {
        goto fail;
    }
    d = weapon_ammo_counts[p[2]] - before;
    f63db0_2c5be8(d == 1 ? ((E_2c5be8 *)(p[2] * 0x4C + (char *)D_179AC0_2c5be8))->h36
                         : ((E_2c5be8 *)(p[2] * 0x4C + (char *)D_179AC0_2c5be8))->h34,
                  d, -1);
    if (f < (float)p[0]) {
        D_0013DE08[p[2]] += weapon_ammo_counts[p[2]] - before;
        *(unsigned char *)(o + 0x20) = 2;
        *(float *)(o + 0x2C) *= D_0015ED60_2c5be8 * 0.200000048f + 1.0f;
        f99f8_2c5be8(p + 4);
        {
            int t = D_15F5CC_2c5be8 - D_L00_00161838;
            int r;
            *(float *)(p + 9) = D_0015ED6C_2c5be8 * 8.0f;
            r = f99a8_2c5be8(t);
            if (f96f8_2c5be8(10) < r) {
                f8dc90_2c5be8(0, 0, o, 0xD5);
                D_L00_00161838 = D_15F5CC_2c5be8;
            }
        }
    }
    return 1;
}
typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2c5dc8;
typedef struct {
    float u, v;
} UV_2c5dc8;
typedef struct {
    V_2c5dc8 corner[4];
    unsigned int color[4];
    UV_2c5dc8 uv[4];
    long x70, tex, x80, x88;
} Quad_2c5dc8;
typedef struct {
    char pad0[0x14];
    char *x14;
    float x18, x1c, x20;
} Vars_2c5dc8;
extern V_2c5dc8 D_L00_00166DC0_2c5dc8 __asm__("D_L00_00166DC0") __attribute__((section(".data")));
long tex_2c5dc8(int) __asm__("FUN_001f44b8");
void pos_2c5dc8(void *, int, void *) __asm__("FUN_L00_0024f7c8");
void sub_2c5dc8(void *, void *, void *) __asm__("FUN_001f9a28");
void add_2c5dc8(void *, void *, void *) __asm__("FUN_001f9a10");
float len_2c5dc8(void *) __asm__("FUN_001f9af0");
void cross_2c5dc8(void *, void *, void *) __asm__("FUN_001f9ad8");
float f9dc8_2c5dc8(float) __asm__("FUN_001f9dc8");
float f9de0_2c5dc8(float) __asm__("FUN_001f9de0");
float fa610_2c5dc8(float) __asm__("FUN_001fa610");
void scale_2c5dc8(void *, void *, float) __asm__("FUN_001f9bf8");
void draw_2c5dc8(void *, int, int) __asm__("FUN_001f7d30");
void FUN_L00_002c5dc8(char *m) {
    Quad_2c5dc8 q;
    V_2c5dc8 b, c, a, d, f, g, e, h, k;
    Vars_2c5dc8 *v;
    unsigned int col;
    float half = 0.05f;
    float len, t, step, x, s, amp, wob;
    if (m == 0)
        return;
    v = *(Vars_2c5dc8 **)(m + 0x78);
    if (v == 0)
        return;
    q.tex = tex_2c5dc8(0xF);
    col = 0x80808080;
    q.color[3] = col;
    q.color[2] = col;
    q.color[1] = col;
    q.color[0] = col;
    q.x80 = 0xFF9000000260L;
    q.x70 = 5;
    q.x88 = 0xFF00000044L;
    q.uv[0].u = 0.0f;
    q.uv[0].v = 0.0f;
    q.uv[1].u = 1.0f;
    q.uv[1].v = 0.0f;
    q.uv[2].u = 0.0f;
    q.uv[2].v = 1.0f;
    q.uv[3].u = 1.0f;
    q.uv[3].v = 1.0f;
    pos_2c5dc8(m, 0, &a);
    qcopy(&b, v->x14 + 0x10);
    qcopy(&c, &b);
    sub_2c5dc8(&d, &a, &b);
    t = 0.0f;
    len = len_2c5dc8(&d);
    qcopy(&e, &d);
    e.f[2] -= 0.1f;
    cross_2c5dc8(&f, &d, &e);
    cross_2c5dc8(&g, &d, &f);
    qcopy(&q.corner[0], &b);
    qcopy(&q.corner[2], &b);
    q.corner[0].f[2] -= half;
    q.corner[2].f[2] += half;
    wob = f9dc8_2c5dc8(fa610_2c5dc8(v->x18));
    while (t < len) {
        step = 0.2f;
        if (len - t < step)
            step = len - t;
        t += step;
        x = t * 3.1415927f;
        s = f9de0_2c5dc8(x / len);
        amp = f9de0_2c5dc8(fa610_2c5dc8(x / v->x1c)) * s * wob * v->x20;
        scale_2c5dc8(&d, &d, step);
        add_2c5dc8(&c, &c, &d);
        qcopy(&q.corner[1], &c);
        qcopy(&q.corner[3], &c);
        sub_2c5dc8(&h, &D_L00_00166DC0_2c5dc8, &c);
        cross_2c5dc8(&k, &h, &d);
        scale_2c5dc8(&k, &k, 0.05f);
        add_2c5dc8(&q.corner[1], &q.corner[1], &k);
        sub_2c5dc8(&q.corner[3], &q.corner[3], &k);
        scale_2c5dc8(&e, &f, amp);
        add_2c5dc8(&q.corner[1], &q.corner[1], &e);
        add_2c5dc8(&q.corner[3], &q.corner[3], &e);
        draw_2c5dc8(&q, 0, 1);
        qcopy(&b, &c);
        qcopy(&q.corner[0], &q.corner[1]);
        qcopy(&q.corner[2], &q.corner[3]);
    }
}
#include "eetypes.h"
unsigned char *create_moby(int) __asm__("FUN_0020c4f8");
void FUN_L00_002502f0(unsigned char *, int, int, int);
void moby_point_position(unsigned char *, int, void *) __asm__("FUN_L00_0024f7c8");
int scale_game_frames(int) __asm__("FUN_001f96f8");
void blend_moby_animation(unsigned char *, int, int, int) __asm__("FUN_00212f90");
void FUN_L00_00250df8(unsigned char *);
unsigned char *FUN_L00_002c6f88(unsigned char *src, float f) {
    unsigned char *m = create_moby(0xd1);
    if (m) {
        char *q = *(char **)(m + 0x78);
        *(float *)(q + 4) = f;
        *(unsigned char **)q = src;
        m[0x30] = 0xff;
        *(short *)(m + 0x32) = 0xff;
        m[0x31] = 1;
        FUN_L00_002502f0(m, 0x80, 0x80, 0x80);
        moby_point_position(src, 0, m + 0x10);
        *(u128 *)(m + 0xc0) = *(u128 *)(src + 0xc0);
        *(u128 *)(m + 0xd0) = *(u128 *)(src + 0xd0);
        *(u128 *)(m + 0xe0) = *(u128 *)(src + 0xe0);
        if (m[0x53])
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
        FUN_L00_00250df8(m);
        m[0x20] = 0;
        *(unsigned short *)(m + 0x34) |= 4;
    }
    return m;
}
int FUN_001f96f8_2c7070(int) __asm__("FUN_001f96f8");
void FUN_00212f90_2c7070(void *, int, int, int) __asm__("FUN_00212f90");
void FUN_0020c828_2c7070(void *) __asm__("FUN_0020c828");
void FUN_0020e098_2c7070(void *) __asm__("FUN_0020e098");
extern unsigned char D_001413FF __attribute__((section(".data")));
void FUN_L00_002c7070(unsigned char *m) {
    unsigned char **d;
    unsigned char *o;
    if (m == 0)
        return;
    d = *(unsigned char ***)(m + 0x78);
    if (d == 0)
        return;
    o = d[0];
    if (o == 0 || *(short *)(o + 0xA6) != 0xD0 || o[0x20] == 0xFE || o[0x20] == 0xFD ||
        *(unsigned char **)(*(unsigned char **)(o + 0x78) + 0x14) != m) {
        goto kill;
    }
    if ((*(unsigned short *)(o + 0x34) & 1) || D_001413FF) {
        *(unsigned short *)(m + 0x34) |= 0x41;
    } else {
        *(unsigned short *)(m + 0x34) &= ~0x41;
    }
    switch (m[0x20]) {
    case 0:
        if (m[0x70] & 2) {
            if (m[0x53] != 1)
                FUN_00212f90_2c7070(m, 1, 0, FUN_001f96f8_2c7070(10));
            m[0x20] = 1;
            *(float *)(m + 0x58) = 1.0f;
        }
        break;
    case 1:
        if (d[0][0x20] == 2) {
            if (m[0x53] != 3)
                FUN_00212f90_2c7070(m, 3, 0, FUN_001f96f8_2c7070(10));
            m[0x20] = 3;
            *(float *)(m + 0x58) = 1.0f;
        } else if (d[0][0x20] == 8) {
            if (m[0x53] != 2)
                FUN_00212f90_2c7070(m, 2, 0, FUN_001f96f8_2c7070(10));
            m[0x20] = 2;
            *(float *)(m + 0x58) = 1.0f;
        }
        break;
    case 2:
        if (m[0x70] & 2) {
        kill:
            FUN_0020c828_2c7070(m);
            return;
        }
        break;
    case 3:
        if (m[0x70] & 2) {
            if (m[0x53] != 4)
                FUN_00212f90_2c7070(m, 4, 0, FUN_001f96f8_2c7070(10));
            m[0x20] = 4;
        }
        break;
    case 4:
        if ((unsigned char)(d[0][0x20] - 3) < 3) {
            if (m[0x53] != 5)
                FUN_00212f90_2c7070(m, 5, 0, FUN_001f96f8_2c7070(10));
            m[0x20] = 5;
        }
        break;
    case 5:
        if (m[0x70] & 2)
            *(int *)(m + 0x58) = 0;
        if ((unsigned char)(d[0][0x20] - 6) < 2) {
            *(float *)(m + 0x58) = 1.0f;
            if (m[0x53] != 7)
                FUN_00212f90_2c7070(m, 7, 0, FUN_001f96f8_2c7070(10));
            m[0x20] = 7;
        }
        break;
    case 6:
        break;
    case 7:
        if (!(m[0x70] & 2))
            break;
    default:
        if (m[0x53] != 1)
            FUN_00212f90_2c7070(m, 1, 0, FUN_001f96f8_2c7070(10));
        m[0x20] = 1;
        break;
    }
    FUN_0020e098_2c7070(m);
}
typedef union {
    u128 q;
    f32 f[4];
    s32 i[4];
} V2c7320;
extern s32 D_0013CAE0_2c7320[] __asm__("D_0013CAE0") __attribute__((section(".data")));
extern f32 D_0015ED6C_2c7320 __asm__("D_0015ED6C");
extern f32 dist_2c7320(void *, void *) __asm__("FUN_001f9b48");
extern void sub_2c7320(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_2c7320(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_2c7320(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 atan_2c7320(f32, f32) __asm__("FUN_001f9e90");
extern f32 adiff_2c7320(f32, f32) __asm__("FUN_001fa688");
extern f32 dist2_2c7320(void *, void *) __asm__("FUN_001f9b80");
extern f32 cos_2c7320(f32) __asm__("FUN_001f9dc8");
extern f32 sin_2c7320(f32) __asm__("FUN_001f9de0");
extern f32 fn_2c7320(void *, void *, f32, f32, s32) __asm__("FUN_L00_0025abf0");
void FUN_L00_002c7320(V2c7320 *pos, V2c7320 *tgt, f32 *out, f32 speed, f32 unused, f32 range) {
    V2c7320 t;
    V2c7320 u;
    f32 ang;
    f32 s;
    f32 k;
    f32 v;
    struct Hero *P;
    if (range < dist_2c7320(pos, tgt)) {
        sub_2c7320(&t, tgt, pos);
        scale_2c7320(&t, &t, range - 0.01f);
        add_2c7320(&t, &t, pos);
    } else {
        t.q = tgt->q;
    }
    ang = atan_2c7320(t.f[0] - pos->f[0], t.f[1] - pos->f[1]);
    P = &hero;
    if (adiff_2c7320(P->motion.rot.f[2], ang) > 0.87266463f || dist2_2c7320(pos, &t) < 1.5f) {
        ang = P->motion.rot.f[2];
        u.f[0] = cos_2c7320(ang) * 1.5f;
        u.f[1] = sin_2c7320(P->motion.rot.f[2]) * 1.5f;
        u.i[2] = 0;
        add_2c7320(&u, &u, pos);
        t.f[0] = u.f[0];
        t.f[1] = u.f[1];
    }
    s = D_0015ED6C_2c7320 * 8.5f;
    out[0] = cos_2c7320(ang) * s;
    out[1] = sin_2c7320(ang) * s;
    *(s32 *)&out[2] = 0;
    out[2] = fn_2c7320(pos, &t, s, -speed, 0);
    if (D_0013CAE0_2c7320[0] & 5) {
        k = 3.5f / s;
    } else {
        k = 2.5f / s;
    }
    v = speed * k * 0.5f;
    if (v < out[2]) {
        out[2] = v;
    }
}
#include "qcopy.h"
s32 FUN_001f0b58();
float random_float_between(float, float) __asm__("FUN_002132a8");
int rand(void) __asm__("FUN_001160d8");
int scale_game_frames(int) __asm__("FUN_001f96f8");
void FUN_L00_00269b70(float *, unsigned char, unsigned char, unsigned char, float, float, int);
void FUN_L00_002c8078(unsigned char *m) {
    float v[4] __attribute__((aligned(16)));
    int i;
    int r, g;
    unsigned char b;
    float s;
    switch (FUN_001f0b58()) {
    case 0:
    case 1:
    case 3:
    case 8:
    case 11:
    case 12:
    case 13:
        for (i = 0; i < 20; i++) {
            qcopy(v, m + 0x10);
            v[0] += random_float_between(-0.3f, 0.3f);
            v[1] += random_float_between(-0.3f, 0.3f);
            v[2] += random_float_between(-0.3f, 0.3f);
            r = (rand() + 0x30) & 0x3F;
            g = (rand() + 0x20) & 0x3F;
            b = rand() & 0x2F;
            s = random_float_between(100000.0f, 800000.0f);
            FUN_L00_00269b70(v, r, g, b, s, 0.0f, scale_game_frames(rand() % 40 + 10));
        }
        m[0x20] = 4;
        break;
    }
}
#include "qcopy.h"
unsigned char *create_moby(int) __asm__("FUN_0020c4f8");
unsigned char *FUN_L00_002c8218(int a, u128 *b, u128 *c) {
    unsigned char *m = create_moby(0xe6);
    if (m) {
        char *q;
        q = *(char **)(m + 0x78);
        m[0x30] = 0xff;
        *(short *)(m + 0x32) = 0xff;
        m[0x31] = 1;
        m[0x20] = 0;
        qcopy(m + 0x10, b);
        qcopy(q, c);
        *(int *)(q + 0x30) = a;
        *(int *)(q + 0x34) = 0;
        *(short *)(q + 0x38) = 0;
        *(float *)(m + 0x2c) = *(float *)(*(char **)(m + 0x24) + 0x24) * 0.01f;
        if (hero.unk20A5 || hero.unk20AF)
            *(unsigned short *)(m + 0x34) |= 0x41;
    }
    return m;
}
typedef unsigned int u128_2c82f0 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2c82f0 q;
    float f[4];
} Vec4_2c82f0;
typedef struct {
    u8 p0[0x100];
    Vec4_2c82f0 v100;
    u8 p1[0x2FC - 0x110];
    void *w2FC;
    u8 p2[0x2080 - 0x300];
    char *w2080;
} S13F350_2c82f0;
extern S13F350_2c82f0 D_0013F350_002c82f0 __asm__("D_0013F350");
extern float D_0015ED70_002c82f0 __asm__("D_0015ED70");
extern char D_L00_00173E60_002c82f0[] __asm__("D_L00_00173E60") __attribute__((section(".data")));
void *FUN_L00_002603d0_002c82f0(void *) __asm__("FUN_L00_002603d0");
void FUN_001f9a10_002c82f0(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9a68_002c82f0(void *, void *, float) __asm__("FUN_001f9a68");
int FUN_001efa68_002c82f0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
void FUN_001f9bf8_002c82f0(void *, void *, float) __asm__("FUN_001f9bf8");
void FUN_L00_001ff660_002c82f0(void *, void *, void *) __asm__("FUN_L00_001ff660");
void FUN_L00_002c82f0(Vec4_2c82f0 *a, u8 *m, void *c) {
    char *q = *(char **)(m + 0x78);
    S13F350_2c82f0 *P;
    char *pos, *w;
    Vec4_2c82f0 t, u;
    qcopy(q, c);
    P = &D_0013F350_002c82f0;
    if (P->w2FC && FUN_L00_002603d0_002c82f0(P->w2FC)) {
        FUN_001f9a10_002c82f0(q, q, &P->v100);
    }
    pos = m + 0x10;
    qcopy(pos, a);
    {
        S13F350_2c82f0 *P2 = &D_0013F350_002c82f0;
        w = P2->w2080;
    }
    t.q = ((Vec4_2c82f0 *)(w + 0x10))->q;
    t.f[2] = a->f[2];
    if (FUN_001efa68_002c82f0(&t, a, 0, w, 0)) {
        char *g = D_L00_00173E60_002c82f0;
        float x;
        qcopy(pos, g);
        x = D_0015ED70_002c82f0;
        g += 0x20;
        FUN_001f9bf8_002c82f0(&u, g, x * 9.0f - x * 0.1f);
        FUN_001f9a10_002c82f0(pos, pos, &u);
        FUN_L00_001ff660_002c82f0(q, q, g);
        FUN_001f9a68_002c82f0(q, q, 0.6f);
    }
    m[0x20] = 2;
}

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002bffa8.s", FUN_L00_002bffa8);
#else
#include "rnc/math/fast_math.h"
typedef struct {
    OvlVec4 target_pos, velocity, previous_pos;
    s32 timer, age;
    struct Moby *target, *previous_target;
    float turn_velocity, speed;
    u8 pad48[0x14];
    float move_speed;
    struct Moby *owner;
    float angle;
} Vars_2bffa8;
extern CollisionHit collision_hit_2bffa8 __asm__("D_L00_00173E40") __attribute__((section(".data")));
extern int FUN_L00_0025a420(void *, int, int);
extern void FUN_L00_00260400(void *, int, void *, void *, void *, void *);
extern float FUN_00213508_2bffa8(void *, int, float) __asm__("FUN_00213508");
extern int FUN_001fa6d0_2c53c0(float) __asm__("FUN_001fa6d0");
extern void FUN_00257880_2c4380(void *, int, int, int) __asm__("FUN_00212f90");
extern int tick_countdown(s32 *) __asm__("FUN_001f9740");
extern int FUN_L00_002beab0(void *, int, int);
extern void FUN_L00_002beb90(void *);
extern void FUN_L00_002bebc8(void *);
extern void FUN_L00_002bec00(void *);
extern void FUN_L00_002bec48(void *);
extern struct Moby *FUN_L00_002bed90(void *, void *, void *, void *, int);
extern float FUN_L00_002bf050(void *, void *, void *, void *, float, float, float, float, float, float);
extern int FUN_L00_002bf8b0(void *, void *, void *);
extern int FUN_L00_002bf9f0(void *, void *, void *, int);
extern u8 FUN_L00_002bfec8(void *);
extern void FUN_L00_00258278_2bffa8(void *, void *, float, float, float, float) __asm__("FUN_L00_00258278");
extern u32 FUN_L00_00258ad0_2bffa8(void *, void *, int, int, float, float) __asm__("FUN_L00_00258ad0");
extern u32 FUN_L00_00259028(void *, void *, void *, void *);
extern int FUN_L00_0025b440(void *, void *);
extern void FUN_L00_0025e450_2bffa8(void *, void *, void *, int, int, int, int, int, int, int, int, float, float, float, float, float, float, float) __asm__("FUN_L00_0025e450");
extern void collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_0025a9f8_2bffa8(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void FUN_0028dbe8_2c4380(int, int, void *) __asm__("FUN_0022da68");
extern void FUN_L00_0024eb68_2c4380(void *) __asm__("FUN_0020c828");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int FUN_00213260_2bffa8(int) __asm__("FUN_00213260");
extern float FUN_001f9b48_2c1928(void *, void *) __asm__("FUN_001f9b48");
extern float FUN_001f9b80_2c1928(void *, void *) __asm__("FUN_001f9b80");
extern u8 *FUN_002141f8_2c26c8(struct Moby *) __asm__("FUN_002141f8");
extern int FUN_001efa68_002c82f0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void FUN_001f9c90_2bffa8(void *, void *, float) __asm__("FUN_001f9c90");
extern void FUN_L00_001ff600_2bffa8(void *, void *, float) __asm__("FUN_L00_001ff600");
extern void FUN_001e93b0_2bffa8(void *) __asm__("FUN_001e93b0");
extern char D_L00_001E9B48[], D_L00_001E9B70[];
extern s32 D_L00_00177F00[];
extern void *memset(void *, int, unsigned int);
extern f32 atan2_f(f32, f32) __asm__("FUN_001f9e90");
extern int collect_mobies_in_sphere_2bffa8(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
#define HF_2bffa8(o) (*(float *)((u8 *)&hero + (o)))
#define HI_2bffa8(o) (*(int *)((u8 *)&hero + (o)))
#define HB_2bffa8(o) (*((u8 *)&hero + (o)))
#define ANIM_2bffa8(a,t) FUN_00257880_2c4380(moby,(a),0,scale_game_frames(t))
#define OUTSIDE_2bffa8 (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f || 1021.0f < moby->pos.y || moby->pos.z < 2.0f || 1021.0f < moby->pos.z)
void FUN_L00_002bffa8(struct Moby *moby) {
    extern int level_game_mode __asm__("D_L00_0015F5C4") __attribute__((sda));
    Vars_2bffa8 *vars;
    OvlVec4 probe, start, end, delta, result;
    void *pos;
    float size, speed, distance, height, limit, angle, rate, old_z;
    u8 *data;
    int hit;
    u32 flags;
    struct Moby *target;
    if (!moby) return;
    vars = (Vars_2bffa8 *)moby->pvars;
    if (!vars) return;
    if (level_game_mode == 2) { moby->flags |= 0x41; return; }
    moby->flags &= 0xffbe;
    if (OUTSIDE_2bffa8) goto destroy;
    if (FUN_L00_0025a420(moby,0x800001,0)) moby->state = 11;
    size = convert_integer_to_float(item_text_variant[20]) + 1.0f;
    moby->scale += (moby->pclass->scale * size - moby->scale) * 0.05f;
    speed = convert_integer_to_float(item_text_variant[20]) * 0.5f + 1.0f;
    if (moby->seq == 0 || moby->seq == 6 || moby->seq == 7) moby->anim_speed = speed / size;
    else moby->anim_speed = 1.0f;
    if (moby->state != 11) {
        HI_2bffa8(0x1ec0) = FUN_001fa6d0_2c53c0(size * 183.296f) + 1;
        HI_2bffa8(0x1ef0) = 0;
        HF_2bffa8(0x1ec4) = size * 0.198f + 0.02f;
        if (moby->state == 0) {
            memset((u8 *)&hero + 0x1f10,0,0x40);
            {
            float scale_01;
            float scale_02;
            float scale_045 = frame_scale * 0.045f;
            float scale_066;
            float scale_2;
            float size_25 = size * 0.25f;
            float size_01 = size * 0.01f * frame_scale;
            float time_297 = frame_time_sq * 29.7f;
            float size_1 = size * 0.1f;
            scale_01 = frame_scale * 0.01f;
            scale_02 = frame_scale * 0.02f;
            scale_066 = frame_scale * 0.066f;
            scale_2 = frame_scale * 0.2f;
            HF_2bffa8(0x1ecc) = size_25;
            HF_2bffa8(0x1ec8) = size_25;
            vars->speed = scale_2;
            vars->turn_velocity = size_01;
            HF_2bffa8(0x1ee0) = 0.3f;
            HF_2bffa8(0x1ee4) = 2.7f;
            HF_2bffa8(0x1edc) = size_01;
            HI_2bffa8(0x1ed8) = 0;
            vars->move_speed = scale_066 * speed;
            HB_2bffa8(0x1f3c) = 2;
            HB_2bffa8(0x1f3d) = 3;
            HB_2bffa8(0x1f3e) = 4;
            HF_2bffa8(0x1f4c) = scale_045;
            HF_2bffa8(0x1ee8) = 3.1415927f;
            HF_2bffa8(0x1f2c) = time_297;
            HF_2bffa8(0x1f30) = size_1;
            HF_2bffa8(0x1f34) = 7.5f;
            HF_2bffa8(0x1f38) = 11.5f;
            HF_2bffa8(0x1f48) = scale_01;
            HF_2bffa8(0x1f00) = scale_01;
            HF_2bffa8(0x1f44) = scale_02;
            HF_2bffa8(0x1efc) = scale_02;
            HF_2bffa8(0x1f04) = scale_045;
            }
            vars->target = 0; moby->state = 14; vars->timer = 0;
            vars->move_speed = frame_scale * 0.1f * speed;
            FUN_L00_002beb90(vars); FUN_L00_002bec00(vars);
            if (moby->prev_seq != 5) ANIM_2bffa8(5,5);
        }
        if (moby->state == 13) {
            if (vars->owner && vars->owner->state != 254 && vars->owner->state != 253) {
                if (!OUTSIDE_2bffa8) return;
            }
            goto destroy;
        }
        vars->age = (s32)((u32)vars->age + 1);
        if (scale_game_frames(3600) < vars->age) moby->state = 11;
        tick_countdown(&vars->timer);
        qcopy(&vars->previous_pos,&moby->pos);
        if (FUN_L00_002beab0(moby,5,0)) {
            if (!collision_hit_2bffa8.moby) goto land;
            target = (struct Moby *)collision_hit_2bffa8.moby;
            if (collision_hit_2bffa8.unk1C < 0 && (target->flags & 0x1000) && target->pclass && target->pclass->type == 5) { moby->state = 11; goto collision_done; }
            if (!collision_hit_2bffa8.moby || collision_hit_2bffa8.unk1C < 0) {
land:
                old_z = moby->pos.z;
                qcopy(&moby->pos,(u8 *)&collision_hit_2bffa8 + 0x30);
                /* Retail subtracts the saved height from itself. */
                if (0.6f < absolute_float(old_z - old_z)) moby->pos.z = old_z + 0.01f;
                qcopy(&vars->velocity,(u8 *)&collision_hit_2bffa8 + 0x40);
                FUN_001f9c90_2bffa8(&vars->velocity,&vars->velocity,frame_scale * 0.05f);
                FUN_L00_001ff600_2bffa8(&vars->velocity,&vars->velocity,frame_scale * 0.05f);
                moby->state = 8;
                if (vars->target) { vars->previous_target = vars->target; vars->timer = (s32)((u32)vars->timer + 500); }
                if (moby->prev_seq != 5) ANIM_2bffa8(5,5);
            }
        }
collision_done:
        if (moby->pos.z < 0.1f) goto destroy;
        pos = &moby->pos;
        if (vars->timer > 4000) moby->state = 11;
    } else {
        pos = &moby->pos;
    }
    if (FUN_00213508_2bffa8(pos,0,0.5f) != 0.0f && collision_hit_2bffa8.moby)
        FUN_L00_00260400(moby,collision_hit_2bffa8.moby,pos,&moby->rot,pos,&moby->rot);
    switch (moby->state) {
    case 15: goto reset;
    case 0:
        FUN_001e93b0_2bffa8(D_L00_001E9B48); moby->state = 1; goto finish;
    case 8: {
        struct Hero *P = &hero;
        fast_vec_add(&probe,pos,&vars->velocity);
        vars->velocity.f[2] -= frame_time_sq * 9.0f;
        FUN_L00_002beb90(vars);
        flags = FUN_L00_00258ad0_2bffa8(moby,&vars->velocity,(*(int *)((u8 *)P + 0x1ec0)),0,size*0.7f,(*(float *)((u8 *)P + 0x1ef0)));
        if (flags & 2) { vars->velocity.f[2] = 0.0f; fast_vec_scale(&vars->velocity,&vars->velocity,0.5f); }
        FUN_L00_001ff600_2bffa8(&vars->velocity,&vars->velocity,frame_scale*0.05f);
        if (moby->anim_flags & 2) { if (flags & 2) moby->state = 2; else goto falling_anim; }
        goto turn;
    }
    case 1: case 9: case 10:
        if (moby->anim_flags & 2) moby->state = 2;
        break;
    case 2: case 3: case 4: case 5: {
        u8 state;
        float angle;
        struct Moby *target;
        target = vars->target;
        if (target && target->state != 254 && target->state != 253) {
            height = target->pos.z;
            data = FUN_002141f8_2c26c8(target);
            limit = speed;
            if (data) { distance = convert_integer_to_float(data[10]); height += *(float *)(data+0x10); limit = speed + distance * 0.125f; }
            if (FUN_001f9b48_2c1928(pos,&vars->target->pos) < limit && coll_sphere(speed,pos,1,moby)) { moby->state = 11; goto finish; }
            distance = FUN_001f9b80_2c1928(pos,&vars->target->pos);
            limit *= 0.5f;
            if (distance < limit && absolute_float(height - (moby->pos.z + speed * 0.1f)) < limit) {
                FUN_L00_002bec00(vars);
                FUN_L00_002bf8b0(moby,(u8 *)&hero+0x1f10,&vars->target->pos);
                FUN_L00_002bec48(vars);
                qcopy(&vars->target_pos,&vars->target->pos);
                vars->move_speed = frame_scale * 0.066f * speed;
                if (moby->prev_seq != 0) ANIM_2bffa8(0,2);
                moby->state = 6;
            }
        }
        qcopy(&probe,pos); probe.f[2] += size * 0.3f;
        hit = FUN_00213260_2bffa8(4);
        rate = frame_scale;
        if (!hit) { vars->target = FUN_L00_002bed90(moby,&probe,&moby->rot,vars->previous_target,1); rate = frame_scale; }
        vars->move_speed = rate * 0.1f * speed; moby->state = 4;
        if (!vars->target) {
            angle = random_float_between(-3.1415927f,3.1415927f);
            limit = size * 0.35f;
            if (!FUN_00213260_2bffa8(4)) vars->angle = FUN_L00_002bf050(moby,pos,0,&result,angle,1.847995638847351f,4.0f,limit,4.0f,convert_integer_to_float(HI_2bffa8(0x1ec0))*0.0009765625f);
            else result.f[0] = 5.0f;
        }
        target = vars->target;
        if (target && target != hero.moby) {
            if (!target || target->state == 254 || target->state == 253 || !target->pclass || target->pclass->type != 5 || !(target->flags & 0x1000)) {
                vars->target = 0; moby->state = 2;
                vars->move_speed = frame_scale * 0.0273f * speed; goto finish;
            }
        }
        state = moby->state;
        if (!target) {
            moby->state = 5;
            if (moby->prev_seq != 6) ANIM_2bffa8(6,10);
            vars->move_speed = frame_scale * 0.0273f * speed;
        } else {
            if (!FUN_00213260_2bffa8(4) && FUN_L00_002bf9f0(moby,vars,&vars->target->pos,1)) {
                qcopy(&vars->target_pos,&vars->target->pos);
                vars->move_speed = frame_scale * 0.066f * speed;
                if (moby->prev_seq != 0) ANIM_2bffa8(0,2);
                moby->state = 6; goto finish;
            }
            distance = FUN_001f9b80_2c1928(pos,&vars->target->pos);
            if (vars->target == hero.moby) {
                moby->state = 5;
                rate = frame_scale;
                if (moby->anim_flags & 2) {
                    rate = frame_scale;
                    if (moby->prev_seq != 6) { ANIM_2bffa8(6,0); rate = frame_scale; }
                }
                vars->move_speed = rate * 0.0273f * speed;
            }
            if (!(distance < 8.0f) && 20.0f < distance) {
                moby->state = 5; if (moby->prev_seq != 0) ANIM_2bffa8(0,10);
                limit = 0.0273f; rate = frame_scale;
            } else {
                moby->state = 4; if (moby->prev_seq != 7) ANIM_2bffa8(7,10);
                limit = 0.1f; rate = frame_scale;
            }
            vars->move_speed = rate * limit * speed;
            if (!FUN_00213260_2bffa8(4)) vars->angle = FUN_L00_002bf050(moby,pos,&vars->target->pos,&result,moby->rot.z,1.847995638847351f,4.0f,0.35f,4.0f,convert_integer_to_float(HI_2bffa8(0x1ec0))*0.0009765625f);
            else result.f[0] = 5.0f;
        }
        if (moby->state == 4 && state == 5) {
            moby->state = 12; if (moby->prev_seq != 1) ANIM_2bffa8(1,10); break;
        }
        vars->target_pos.f[1] = moby->pos.y + fast_sin(vars->angle) * 5.0f;
        vars->target_pos.f[0] = moby->pos.x + fast_cos(vars->angle) * 5.0f;
        vars->target_pos.f[2] = moby->pos.z + 1.0f;
        FUN_L00_002beb90(vars);
        FUN_L00_00258278_2bffa8(moby,&vars->speed,vars->angle,HF_2bffa8(0x1efc),HF_2bffa8(0x1f00),HF_2bffa8(0x1f04));
        fast_vec_scale(&delta,&vars->velocity,5.0f); delta.i[2] = 0;
        qcopy(&start,pos); start.f[2] += size * 0.15f;
        fast_vec_add(&end,&start,&delta);
        hit = FUN_001efa68_002c82f0(&start,&end,6,moby,0);
        target = (struct Moby *)collision_hit_2bffa8.moby;
        if (!hit || target == vars->target || (target && target->pclass && target->pclass->type == 5)) {
            old_z = moby->pos.z;
            flags = FUN_L00_00259028(moby,(u8 *)&hero+0x1ec0,vars,&vars->velocity);
            if (1.0f < absolute_float(moby->pos.z-old_z)) {
                moby->pos.z = old_z;
                if (FUN_L00_002beab0(moby,5,0)) {
                    qcopy(pos,(u8 *)&collision_hit_2bffa8+0x30);
                    if (1.0f < absolute_float(moby->pos.z-old_z)) moby->pos.z = old_z;
                }
            }
        } else {
            result.f[0] = 0.0f; flags = 2;
            vars->velocity.f[0] = moby->pos.x-vars->target_pos.f[0];
            vars->velocity.f[1] = moby->pos.y-vars->target_pos.f[1];
            vars->velocity.f[2] = 0.01f;
            fast_vec_normalize(&vars->velocity,&vars->velocity,0.1f);
        }
        FUN_L00_002bebc8(vars);
        if ((flags & 2) || result.f[0] < 4.0f) {
            if (!FUN_L00_002bf9f0(moby,vars,vars,0)) {
                if (vars->target) { vars->previous_target = vars->target; vars->timer = (s32)((u32)vars->timer + 500); }
                vars->velocity.f[0] = moby->pos.x-vars->target_pos.f[0];
                vars->velocity.f[2] = 0.01f;
                vars->velocity.f[1] = moby->pos.y-vars->target_pos.f[1];
                fast_vec_normalize(&vars->velocity,&vars->velocity,0.1f);
                moby->state = 8; if (moby->prev_seq != 5) ANIM_2bffa8(5,5); goto finish;
            }
            vars->move_speed = frame_scale * 0.066f * speed;
            if (moby->prev_seq != 0) ANIM_2bffa8(0,2);
            moby->state = 6;
        }
        break;
    }
    case 6: {
        u8 state;
        angle = atan2_f(vars->target_pos.f[0]-moby->pos.x,vars->target_pos.f[1]-moby->pos.y);
        FUN_L00_002beb90(vars);
        FUN_L00_00258278_2bffa8(moby,(u8 *)&hero+0x1ec0+0x14,angle,HF_2bffa8(0x1efc)+HF_2bffa8(0x1efc),HF_2bffa8(0x1f00)*0.5f,HF_2bffa8(0x1f04)+HF_2bffa8(0x1f04));
        FUN_L00_002bebc8(vars);
        if (fast_difference_between_rotations(moby->rot.z,angle) < 0.08726646f) { state = HB_2bffa8(0x1f3c); ANIM_2bffa8(state,3); moby->state = 7; }
        break;
    }
    case 7:
        old_z = moby->pos.z; FUN_L00_002bec00(vars);
        if (FUN_L00_0025b440(moby,(u8 *)&hero+0x1f10) == 4) {
            moby->state = 4; if (moby->prev_seq != 7) ANIM_2bffa8(7,3);
            vars->move_speed = frame_scale * 0.1f * speed;
        }
        FUN_L00_002bec48(vars); absolute_float(moby->pos.z-old_z); break;
    case 11:
        if (!OUTSIDE_2bffa8) {
            qcopy(&start,pos); end.q = 0; end.f[2] = end.f[3] = 1.0f;
            FUN_L00_0025e450_2bffa8(moby,&end,pos,5,2,4,-1,1,5,-1,item_text_variant[20],0.0f,0.0f,speed+speed,speed,9.0f,1.0f,15.0f);
            start.f[2] += size * 0.5f;
            /* The unused prepared start vector is present in retail. */
            hit = collect_mobies_in_sphere_2bffa8(speed,pos,21,moby,0);
            /* The query returns its integer result in v0. */
            delta.q = *(OvlQuad *)&moby->pos;
            FUN_L00_0025a9f8_2bffa8(moby,&delta,D_L00_00177F00,hit,0,0x10000,2,3,3.0f,1.0f,1.0f);
            if (item_text_variant[20]) FUN_0028dbe8_2c4380(4,0,moby);
            else FUN_0028dbe8_2c4380(0,0,moby);
        }
destroy:
        FUN_L00_0024eb68_2c4380(moby);
        return;
    case 12:
        if (moby->anim_flags & 2) {
            moby->state = 4; if (moby->prev_seq != 7) ANIM_2bffa8(7,10);
            vars->move_speed = frame_scale * 0.1f * speed;
        }
        goto finish;
    case 13:
        FUN_001e93b0_2bffa8(D_L00_001E9B70); goto finish;
    case 14:
        fast_vec_add(&start,pos,&vars->velocity);
        if (coll_sphere(size*0.5f,pos,1,moby)) {
            target = (struct Moby *)collision_hit_2bffa8.moby;
            if (target && target->oclass == 0xba) {
                fast_vec_sub(&start,(u8 *)&collision_hit_2bffa8+0x30,pos);
                distance = fast_vec_length(&start); start.i[2] = 0;
                fast_vec_normalize(&start,&start,distance*1.1f);
                fast_vec_add(pos,pos,&start); rate = frame_time_sq;
            }
        }
        rate = frame_time_sq;
        vars->velocity.f[2] -= rate * 9.0f;
        FUN_L00_002beb90(vars);
        flags = FUN_L00_00258ad0_2bffa8(moby,&vars->velocity,HI_2bffa8(0x1ec0),0,size*0.7f,HF_2bffa8(0x1ef0));
        if (flags & 2) { vars->velocity.f[2] = 0.0f; fast_vec_scale(&vars->velocity,&vars->velocity,0.5f); }
        FUN_001f9c90_2bffa8(&vars->velocity,&vars->velocity,frame_scale*0.05f);
        if (flags & 2) moby->state = 2;
        else {
falling_anim:
            if (moby->prev_seq != 5) ANIM_2bffa8(5,5);
        }
turn:
        angle = atan2_f(vars->velocity.f[0],vars->velocity.f[1]);
        FUN_L00_00258278_2bffa8(moby,&vars->turn_velocity,angle,HF_2bffa8(0x1edc),HF_2bffa8(0x1ee0),HF_2bffa8(0x1ee4));
        FUN_L00_002bebc8(vars); break;
    default:
reset: moby->state = 0; goto finish;
    }
    moby->state = FUN_L00_002bfec8(moby);
finish:
    if (OUTSIDE_2bffa8) FUN_L00_0024eb68_2c4380(moby);
    return;
}
#undef HF_2bffa8
#undef HI_2bffa8
#undef HB_2bffa8
#undef ANIM_2bffa8
#undef OUTSIDE_2bffa8
#endif
extern float D_L00_00173E68_2c1778 __asm__("D_L00_00173E68") __attribute__((section(".data")));
extern int D_L00_0015F5CC_2c1778[1] __asm__("D_L00_0015F5CC");
void FUN_00213508_2c1778(void *, int, float) __asm__("FUN_00213508");
float FUN_001fa580_2c1778(float, float) __asm__("FUN_001fa580");
float FUN_001f9b80_2c1778(void *, void *) __asm__("FUN_001f9b80");
float FUN_001f99c0_2c1778(float) __asm__("FUN_001f99c0");
void FUN_L00_00260860_2c1778(int, int) __asm__("FUN_L00_00260860");
float FUN_00213ed8_2c1778(void *, float, float) __asm__("FUN_00213ed8");
void FUN_0020c828_2c1778(void *) __asm__("FUN_0020c828");

extern const float D_L00_00161828 __attribute__((sda));
extern float D_L00_00161830 __attribute__((sda));
void FUN_L00_002c1778(void *mv) {
    unsigned char *m = mv;
    struct Hero *P;
    switch (m[0x20]) {
    case 0:
        FUN_00213508_2c1778(m + 0x10, 0, 0.5f);
        *(float *)(m + 0x40) = 1.5707964f;
        *(float *)(m + 0x44) = 3.1415927f;
        *(float *)(m + 0x18) = D_L00_00173E68_2c1778 + D_L00_00161828;
        m[0x20] = 1;
        break;
    case 1: {
        float r = FUN_001fa580_2c1778(*(float *)(m + 0x48), D_L00_00161830);
        int t = D_L00_0015F5CC_2c1778[0];
        *(float *)(m + 0x48) = r;
        if (t % 10 == 0) {
            P = &hero;
            if (P->health.hp) {
                if (FUN_001f9b80_2c1778(m + 0x10, &P->motion.pos) < 1.0f) {
                    if (FUN_001f99c0_2c1778(*(float *)(m + 0x18) - P->motion.pos.f[2]) < 2.0f) {
                        FUN_L00_00260860_2c1778(0x1A, 1);
                        m[0x20] = 2;
                    }
                }
            }
        }
    } break;
    case 2:
        FUN_00213ed8_2c1778(m + 0x2C, 0.0f, *(float *)(*(char **)(m + 0x24) + 0x24) * 0.02f);
        if (*(float *)(m + 0x2C) == 0.0f)
            FUN_0020c828_2c1778(m);
        break;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002c1ad0.s", FUN_L00_002c1ad0);
#else
typedef struct {
    u8 pad0[0x40];
    OvlVec4 grip;
    struct Moby *held;
    s32 timer;
} CarrierVars_2c1ad0;
typedef struct {
    u8 pad0[0x88];
    u16 deliveries;
    u16 unkA2;
    u32 levels;
} DeliveryStats_2c1ad0;
extern DeliveryStats_2c1ad0 D_00141680_2c1ad0 __asm__("D_00141680");
extern u8 D_0013E531;
extern f32 D_0015ED68;
extern f32 D_0015ED6C __attribute__((section(".sdata")));
extern f32 frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern s32 D_0015EEA4;
extern u32 D_0013CAE4 __attribute__((section(".data")));
extern CollisionHit coll_hit __asm__("D_L00_00173E40") __attribute__((section(".data")));
extern OvlVec4 D_L00_00166DC0_2c1ad0 __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern OvlVec4 D_L00_00166C80_2c1ad0[3] __asm__("D_L00_00166C80") __attribute__((section(".data")));
extern s16 D_L00_001B07B0_lui[]  __asm__("D_L00_001B07B0")__attribute__((section(".data")));
extern void FUN_L00_002aa008(void *, void *, void *);
extern struct Moby *FUN_L00_002a9ed0(void *, void *, void *);
extern void FUN_L00_0025f780(void *, void *, int);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
extern f32 square_root(f32) __asm__("FUN_001f9988");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 atan2_f(f32, f32) __asm__("FUN_001f9e90");
extern f32 FUN_001f9dc8(f32);
extern f32 FUN_001f9de0(f32);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 tick_countdown(s32 *) __asm__("FUN_001f9740");
extern s32 collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void setup_matrix_2c1ad0(void *, void *) __asm__("FUN_001fa2b8");
extern void matrix_from_basis_rows_2c1ad0(void *, void *) __asm__("FUN_001fa2d8");
extern void rotate_vector_2c1ad0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void adjust_direction_2c1ad0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern f32 vector_length_xy_2c1ad0(void *) __asm__("FUN_001f9b20");
extern f32 fast_subtract_rotations_2c1ad0(f32, f32) __asm__("FUN_001fa5c8");
extern void build_spherical_offset_2c1ad0(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern int hero_set_state_2c1ad0(int, int) __asm__("FUN_L00_002223f8");
extern int FUN_L00_00216de8_2c1ad0(int, int) __asm__("FUN_L00_00216de8");
extern int FUN_L00_00233db8_2c1ad0(int, int) __asm__("FUN_L00_00233db8");
extern int FUN_L00_00233e98_2c1ad0(int) __asm__("FUN_L00_00233e98");
#include "rnc/input/pad_state.h"
/* Level 00 carrier: updates its grip, tosses a held moby, then looks for another. */
void FUN_L00_002c1ad0(struct Moby *moby) {
    CarrierVars_2c1ad0 *vars;
    OvlVec4 saved, offset, rotatedOffset, local[4], pos, target, basis[3], matrix[4];
    OvlVec4 direction;
    union { OvlVec4 launch; OvlVec4 arc; } temporary;
    OvlVec4 probe;
    f32 angle, slope, distance, horizontal, vertical, radius, speed;
    OvlVec4 *targetPosition;


    if (moby == 0)
        return;
    vars = (CarrierVars_2c1ad0 *)moby->pvars;
    if (*(s32 *)((u8 *)vars + 0x5c) < 3) {
        ++*(s32 *)((u8 *)vars + 0x5c);
        return;
    }
    offset.q = 0;
    offset.f[1] = -0.09f;
    offset.f[2] = -0.02f;
    setup_matrix_2c1ad0(basis, D_L00_00166C80_2c1ad0);
    matrix_from_basis_rows_2c1ad0(matrix, basis);
    if (vars == 0)
        return;
    FUN_L00_0024f7c8((u8 *)moby, 0, &saved);
    build_moby_bone_transform(moby, 0, local);
    rotate_vector_2c1ad0(&rotatedOffset, &offset, local);
    add_vector_xyz(&vars->grip, &saved, &rotatedOffset);
    if (vars->held != 0)
        vars->held->collision_disable = 1;
    if (vars->held == 0 || vars->held->state == 0xFE || vars->held->state == 0xFD)
        vars->held = 0;
    if (hero.state.current == 1)
        hero_set_state_2c1ad0(0x1E, 1);
    if (tick_countdown((s32 *)((u8 *)vars + 0x58)) && !hero.unk20AC) {
        if ((hero.unk20A8 && hero.unk1BC == scale_game_frames(0x11)) ||
            ((D_0013CAE4 & hero.items[0].button_mask) &&
             hero.state.current == 0x1E && FUN_L00_00233e98_2c1ad0(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            FUN_L00_00216de8_2c1ad0(0x1A, 0);
            FUN_L00_00233db8_2c1ad0(-1, 1);
            moby->state = 3;
        }
    }
    if (vars->held != 0 && !hero.unk20AC) {
        f32 *out = (f32 *)vars->held->pvars;
        {
            f32 speed;
            temporary.launch.q = 0;
            if (item_text_variant[0x11]) {
                if (controller_state.held & 5) {
                    speed = 12.0f;
                    distance = D_0015ED6C;
                } else {
                    speed = 4.0f;
                    distance = frame_time;
                }
            } else {
                if (controller_state.held & 5) {
                    speed = 6.0f;
                    distance = D_0015ED6C;
                } else {
                    speed = 4.0f;
                    distance = frame_time;
                }
            }
            temporary.launch.f[0] = distance * speed;
            qcopy(&direction, &temporary.launch);
        }
        adjust_direction_2c1ad0(&direction, &direction, (u8 *)&hero + 0x640);
        angle = fast_add_rotations(-0.361968f,
                                   atan2_f(hero.unk670, hero.unk674));
        pos.f[0] = FUN_001f9dc8(angle) * 0.859039f;
        angle = fast_add_rotations(-0.361968f,
                                   atan2_f(hero.unk670, hero.unk674));
        pos.f[1] = FUN_001f9de0(angle) * 0.859039f;
        pos.f[2] = 0.0f;
        add_vector_xyz(&pos, &pos, &hero.motion.pos);
        pos.f[2] += 0.49082f;
        atan2_f(direction.f[0], direction.f[1]);
        targetPosition = &target;
        scale_vector_xyz(targetPosition, &direction, D_0015ED68 * 60.0f);
        add_vector_xyz(targetPosition, targetPosition, &pos);
        if (controller_state.held & 5) {
            s32 hit;
            f32 gravity;
            probe.q = 0;
            probe.f[2] = item_text_variant[0x11] ? 12.0f : 6.0f;
            hit = 0;
            temporary.arc.q = probe.q;
            rotate_vector_2c1ad0(&temporary.arc, &temporary.arc, matrix);
            {
                struct Moby *heroMoby = hero.moby;
                angle = atan2_f(temporary.arc.f[0], temporary.arc.f[1]);
                fast_subtract_rotations_2c1ad0(*(f32 *)((u8 *)heroMoby + 0x48), angle);
            }
            angle = atan2_f(temporary.arc.f[0], temporary.arc.f[1]);
            slope = atan2_f(vector_length_xy_2c1ad0(&temporary.arc), temporary.arc.f[2]);
            if (slope > 0.5934119f)
                slope = 0.5934119f;
            if (slope < -1.3962634f)
                slope = -1.3962634f;
            speed = 4.0f;
            if (item_text_variant[0x11]) {
                if (controller_state.held & 5) speed = 12.0f;
            } else {
                if (controller_state.held & 5) speed = 6.0f;
            }
            build_spherical_offset_2c1ad0(&temporary.arc, speed, angle, slope);
            distance = vector_length_xy_2c1ad0(&temporary.arc);
            if (distance == 0.0f)
                horizontal = temporary.arc.f[2] / 0.01f;
            else
                horizontal = temporary.arc.f[2] / distance;
            speed = 4.0f;
            if (item_text_variant[0x11]) {
                if (controller_state.held & 5) speed = 12.0f;
            } else {
                if (controller_state.held & 5) speed = 6.0f;
            }
            distance = square_root(frame_time_sq * 9.8f * speed * 0.5f);
            gravity = frame_time_sq * 9.8f;
            radius = distance + distance;
            radius *= distance;
            radius *= 1.0f - horizontal;
            radius /= gravity;
            vertical = horizontal * radius;
            if (radius > 0.0f) {
                temporary.arc.f[0] = FUN_001f9dc8(angle) * radius;
                temporary.arc.f[1] = FUN_001f9de0(angle) * radius;
                temporary.arc.f[2] = vertical;
                add_vector_xyz(&probe, &D_L00_00166DC0_2c1ad0, &temporary.arc);
                if (collision_line(&D_L00_00166DC0_2c1ad0, &probe, 2, moby, 0) &&
                    coll_hit.moby != (s32)hero.moby) {
                    targetPosition->q = coll_hit.point.q;
                    hit = 1;
                    angle = atan2_f(targetPosition->f[0] - pos.f[0], targetPosition->f[1] - pos.f[1]);
                }
                if (!hit) {
                    targetPosition->f[0] = FUN_001f9dc8(angle) * radius * 5.0f;
                    targetPosition->f[1] = FUN_001f9de0(angle) * radius * 5.0f;
                    targetPosition->f[2] = vertical * 5.0f;
                    add_vector_xyz(targetPosition, targetPosition, &D_L00_00166DC0_2c1ad0);
                }
            }
        }
        {
            f32 gravity = frame_time_sq_macro * 9.8f;
            speed = 4.0f;
            if (item_text_variant[0x11]) {
                if (controller_state.held & 5) speed = 12.0f;
        } else {
            if (controller_state.held & 5) speed = 6.0f;
        }
        FUN_L00_002c1928((f32 *)&pos, (OvlQuad *)targetPosition, out,
                          gravity, 1.0f, speed);
        }
    }
    switch (moby->state) {
    case 0:
        vars->held = 0;
        if (moby->anim_flags & 2) moby->state = 2;
        else moby->state = 1;
    case 1:
        if (moby->anim_flags & 2)
            moby->state = 2;
        break;
    case 3:
        if (vars->held != 0) {
            FUN_L00_002aa008(&pos, vars->held, vars->held->pvars);
            if (D_00141680_2c1ad0.deliveries < 0xffff)
                ++D_00141680_2c1ad0.deliveries;
            {
                s32 frames = scale_game_frames(D_0015EEA4);
                if (frames / 600 > D_00141680_2c1ad0.unkA2)
                    D_00141680_2c1ad0.unkA2 = scale_game_frames(D_0015EEA4) / 600;
            }
            D_00141680_2c1ad0.levels = (D_00141680_2c1ad0.levels | (1u << current_level_index)) | 0x80000000u;
            FUN_L00_0025f780(vars->held, D_L00_001B07B0_lui, 0x3f);
            vars->held = 0;
            vars->timer = scale_game_frames(10);
        }
        moby->state = 4;
        *(s32 *)((u8 *)vars + 0x58) = scale_game_frames(0x14);
        break;
    case 4:
        if (hero.state.current != 0x23 && !hero.unk20A8)
            moby->state = 2;
        break;
    case 5:
        moby->state = 6;
        break;
    case 6:
        if (vars->held != 0) {
            mark_moby_for_removal(vars->held);
            vars->held = 0;
        }
        break;
    }
    tick_countdown(&vars->timer);
    if (vars->held != 0)
        qcopy(&vars->held->pos, &vars->grip);
    else if (vars->timer == 0 && FUN_L00_00233e98_2c1ad0(-1)) {
        direction.q = 0;
        vars->held = FUN_L00_002a9ed0(moby, &vars->grip, &direction);
    }
}
#endif
/*
 * TODO(route): this body is DONE - it is byte-identical to retail under the
 * bundled GNU as (572/572 bytes; the only 23 differing words are relocation
 * sites: jal/lui/%lo with 0 addends). The current game route (cc1 -> Ps2EeAs)
 * reports SIZE 580/572 because Ps2EeAs 1.9.25.758 auto-inserts 2 NOPs before a
 * div.s that sits "too near a branch instruction / possible branch
 * destination" (its divbug workaround), and the else-block div.s here is right
 * after the $L7 label. Retail never has that padding: a byte scan of the whole
 * retail corpus found 0 `nop; nop; div` in 346 div words of SCUS_971.99 and
 * 26,539 div words across all 19 level text.bin - so the retail-era assembler
 * did not apply this workaround, and our vendored Ps2EeAs is from a different
 * (later) build.
 *
 * To make this function exact, whoever owns the route must first fix the
 * toolchain mismatch; this source needs no further change. Options, both
 * fail-closed (strip only NOPs that Ps2EeAs inserted relative to the GNU-as
 * reference, and only when the trimmed stream still explains every retail
 * word; validate with the whole-ELF gate):
 *   (a) in the game repo's padless-asm.py `finish` step (and the tools'
 *       overlay checker), drop the assembler-inserted div padding using the
 *       GNU-as reference object assembled from the same .s; or
 *   (b) assemble overlays with the bundled GNU as instead of Ps2EeAs, if a
 *       corpus check shows the overlays contain no Ps2EeAs-only output
 *       (short-loop / FPU hazard NOPs) - those ARE in retail and must stay.
 * Evidence and numbers: analysis/overlays/ps2eeas-div-padding.md (tools repo).
 * After the route is fixed: re-score this file, expect EXACT 572/572, then
 * promote normally. Do NOT add inline asm or a per-unit exception for this.
 */

typedef union {
    OvlQuad q;
    f32 f[4];
    s32 i[4];
} V2c2488;
extern s32 D_0013CAE0[] __asm__("D_0013CAE0") __attribute__((section(".data")));
extern u8 D_0013E52A[] __asm__("D_0013E52A");
extern f32 D_0015ED6C __asm__("D_0015ED6C");
extern f32 dist(void *, void *) __asm__("FUN_001f9b48");
extern void sub_2c(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_2c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_2c(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 atan(f32, f32) __asm__("FUN_001f9e90");
extern f32 adiff(f32, f32) __asm__("FUN_001fa688");
extern f32 dist2(void *, void *) __asm__("FUN_001f9b80");
extern f32 cos_2c(f32) __asm__("FUN_001f9dc8");
extern f32 sin_2c(f32) __asm__("FUN_001f9de0");
extern f32 scale_ticks(s32) __asm__("FUN_001fa6c0");
extern f32 fn_2c2488(void *, void *, f32, f32, s32) __asm__("FUN_L00_0025abf0");

void aim_throw_arc(V2c2488 *pos, V2c2488 *tgt, f32 *out, f32 speed, f32 unused, f32 range) __asm__("FUN_L00_002c2488");
void aim_throw_arc(V2c2488 *pos, V2c2488 *tgt, f32 *out, f32 speed, f32 unused, f32 range) {
    V2c2488 t;
    V2c2488 u;
    f32 ang;
    f32 s;
    f32 k;
    f32 v;
    struct Hero *P;
    if (range < dist(pos, tgt)) {
        sub_2c(&t, tgt, pos);
        scale_2c(&t, &t, range - 0.01f);
        add_2c(&t, &t, pos);
    } else {
        t.q = tgt->q;
    }
    ang = atan(t.f[0] - pos->f[0], t.f[1] - pos->f[1]);
    P = &hero;
    if (adiff(P->motion.rot.f[2], ang) > 0.87266463f || dist2(pos, &t) < 1.5f) {
        ang = P->motion.rot.f[2];
        u.f[0] = cos_2c(ang) * 1.5f;
        u.f[1] = sin_2c(P->motion.rot.f[2]) * 1.5f;
        u.i[2] = 0;
        add_2c(&u, &u, pos);
        t.f[0] = u.f[0];
        t.f[1] = u.f[1];
    }
    s = D_0015ED6C * 8.5f;
    out[0] = cos_2c(ang) * s;
    out[1] = sin_2c(ang) * s;
    *(s32 *)&out[2] = 0;
    out[2] = fn_2c2488(pos, &t, s, -speed, 0);
    if (D_0013CAE0[0] & 5) {
        k = scale_ticks(D_0013E52A[0]) * 2.5f + 8.5f;
        k = k / s;
    } else {
        k = 8.5f / s;
    }
    v = speed * k * 0.5f;
    if (v < out[2]) {
        out[2] = v;
    }
}
typedef struct {
    u8 pad0[0x40];
    OvlVec4 grip;
    struct Moby *held;
    s32 timer;
    s32 delay;
} CarrierVars_2c26c8;
typedef struct {
    u8 pad0[0x50];
    u16 deliveries; /* D_001416D0: saturates at 0xFFFF */
    u16 unkA2;
    u32 levels;
} DeliveryStats_2c26c8;
extern DeliveryStats_2c26c8 D_00141680_2c26c8 __asm__("D_00141680");
extern struct Moby *D_L00_001ABA00_2c26c8[] __asm__("D_L00_001ABA00") __attribute__((section(".data")));
extern u8 *FUN_002141f8_2c26c8(struct Moby *) __asm__("FUN_002141f8");
#include "rnc/input/pad_state.h"
extern struct Moby *FUN_L00_002ace70(struct Moby *, void *);
extern void FUN_L00_002acfd8(void *, struct Moby *, void *);
extern u8 D_0013E531;
extern f32 D_0015ED68;
extern f32 D_0015ED6C __attribute__((section(".sdata")));
extern f32 frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern s32 D_0015EEA4;
extern u32 D_0013CAE4 __attribute__((section(".data")));
extern CollisionHit coll_hit __asm__("D_L00_00173E40") __attribute__((section(".data")));
extern OvlVec4 D_L00_00166DC0_2c1ad0 __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern OvlVec4 D_L00_00166C80_2c1ad0[3] __asm__("D_L00_00166C80") __attribute__((section(".data")));
extern s16 D_L00_001B07B0_lui[]  __asm__("D_L00_001B07B0")__attribute__((section(".data")));
extern void FUN_L00_002aa008(void *, void *, void *);
extern struct Moby *FUN_L00_002a9ed0(void *, void *, void *);
extern void FUN_L00_0025f780(void *, void *, int);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
extern f32 square_root(f32) __asm__("FUN_001f9988");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 atan2_f(f32, f32) __asm__("FUN_001f9e90");
extern f32 FUN_001f9dc8(f32);
extern f32 FUN_001f9de0(f32);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 tick_countdown(s32 *) __asm__("FUN_001f9740");
extern s32 collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void setup_matrix_2c1ad0(void *, void *) __asm__("FUN_001fa2b8");
extern void matrix_from_basis_rows_2c1ad0(void *, void *) __asm__("FUN_001fa2d8");
extern void rotate_vector_2c1ad0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void adjust_direction_2c1ad0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern f32 vector_length_xy_2c1ad0(void *) __asm__("FUN_001f9b20");
extern f32 fast_subtract_rotations_2c1ad0(f32, f32) __asm__("FUN_001fa5c8");
extern void build_spherical_offset_2c1ad0(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern int hero_set_state_2c1ad0(int, int) __asm__("FUN_L00_002223f8");
extern int FUN_L00_00216de8_2c1ad0(int, int) __asm__("FUN_L00_00216de8");
extern int FUN_L00_00233db8_2c1ad0(int, int) __asm__("FUN_L00_00233db8");
extern int FUN_L00_00233e98_2c1ad0(int) __asm__("FUN_L00_00233e98");
/* Updates the grip, chooses a throw target, and handles the held moby states. */
void FUN_L00_002c26c8(struct Moby *moby) {
    CarrierVars_2c26c8 *vars;
    OvlVec4 local[4], saved, offset, rotatedOffset, pos, target, basis[3], matrix[4];
    OvlVec4 launch;
    union { OvlVec4 arc; } temporary;
    OvlVec4 direction;
    OvlVec4 probe;
    f32 angle, slope, distance, horizontal, vertical, radius, speed;
    f32 gravity, root;
    s32 hit = 0;

    if (moby == 0)
        return;
    vars = (CarrierVars_2c26c8 *)moby->pvars;
    offset.q = 0;
    offset.f[1] = -0.09f;
    offset.f[2] = -0.02f;
    setup_matrix_2c1ad0(basis, D_L00_00166C80_2c1ad0);
    matrix_from_basis_rows_2c1ad0(matrix, basis);
    if (vars == 0)
        return;
    if (vars->delay < 3) {
        ++vars->delay;
        return;
    }
    FUN_L00_0024f7c8((u8 *)moby, 0, &saved);
    build_moby_bone_transform(moby, 0, local);
    rotate_vector_2c1ad0(&rotatedOffset, &offset, local);
    add_vector_xyz(&vars->grip, &saved, &rotatedOffset);
    if (vars->held != 0)
        vars->held->collision_disable = 1;
    if (vars->held == 0 || vars->held->state == 0xFE || vars->held->state == 0xFD)
        vars->held = 0;
    if (hero.state.current == 1)
        hero_set_state_2c1ad0(0x1E, 1);
    if (tick_countdown(&vars->timer) && !hero.unk20AC) {
        if ((hero.unk20A8 && hero.unk1BC == scale_game_frames(0x11)) ||
            ((D_0013CAE4 & hero.items[0].button_mask) &&
             hero.state.current == 0x1E && FUN_L00_00233e98_2c1ad0(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            FUN_L00_00216de8_2c1ad0(0x1A, 0);
            FUN_L00_00233db8_2c1ad0(-1, 1);
            moby->state = 3;
        }
    }
    if (vars->held != 0) {
        f32 best = 15.0f;
        f32 *out = (f32 *)vars->held->pvars;
        launch.q = 0;
        launch.f[0] = 1.0f;
        adjust_direction_2c1ad0(&launch, &launch, hero.basis);
        pos.f[0] = FUN_001f9dc8(fast_add_rotations(-0.361968011f, atan2_f(hero.unk670, hero.unk674))) * 0.859039009f;
        pos.f[1] = FUN_001f9de0(fast_add_rotations(-0.361968011f, atan2_f(hero.unk670, hero.unk674))) * 0.859039009f;
        pos.f[2] = 0.0f;
        add_vector_xyz(&pos, &pos, &hero.motion.pos);
        pos.f[2] += 0.490819991f;
        angle = atan2_f(launch.f[0], launch.f[1]);
        {
            f32 speed = (controller_state.held & 5) ? scale_ticks(D_0013E52A[0]) * 2.5f + 8.5f : 8.5f;
            scale_vector_xyz(&target, &launch, speed);
        }
        add_vector_xyz(&target, &target, &pos);
        if ((controller_state.held & 5) && !hero.unk20AC) {
            temporary.arc.q = 0;
            temporary.arc.f[2] = 1.0f;
            rotate_vector_2c1ad0(&temporary.arc, &temporary.arc, matrix);
            hit = 0;
            fast_subtract_rotations_2c1ad0(hero.moby->rot.z, atan2_f(temporary.arc.f[0], temporary.arc.f[1]));
            angle = atan2_f(temporary.arc.f[0], temporary.arc.f[1]);
            slope = atan2_f(vector_length_xy_2c1ad0(&temporary.arc), temporary.arc.f[2]);
            if (0.593411922f < slope) slope = 0.593411922f;
            if (slope < -1.39626336f) slope = -1.39626336f;
            {
                f32 speed = (controller_state.held & 5) ? scale_ticks(D_0013E52A[0]) * 2.5f + 8.5f : 8.5f;
                build_spherical_offset_2c1ad0(&temporary.arc, speed, angle, slope);
            }
            qcopy(&direction, &D_L00_00166DC0_2c1ad0);
            {
                f32 distance = vector_length_xy_2c1ad0(&temporary.arc);
                f32 z = temporary.arc.f[2];
                if (distance != 0.0f) horizontal = z / distance;
                else horizontal = z / 0.01f;
            }
            gravity = frame_time_sq_macro * 11.0f;
            root = square_root(gravity * ((controller_state.held & 5) ? scale_ticks(D_0013E52A[0]) * 2.5f + 8.5f : 8.5f) * 0.5f);
            radius = (root + root) * root * (1.0f - horizontal) / (frame_time_sq_macro * 11.0f);
            vertical = horizontal * radius;
            if (0.0f < radius) {
                temporary.arc.f[0] = FUN_001f9dc8(angle) * radius;
                temporary.arc.f[1] = FUN_001f9de0(angle) * radius;
                temporary.arc.f[2] = vertical;
                add_vector_xyz(&probe, &D_L00_00166DC0_2c1ad0, &temporary.arc);
                if (collision_line(&D_L00_00166DC0_2c1ad0, &probe, 2, moby, 0) && coll_hit.moby != (s32)hero.moby) {
                    qcopy(&target, &coll_hit.point);
                    hit = 1;
                    angle = atan2_f(target.f[0] - pos.f[0], target.f[1] - pos.f[1]);
                }
                if (!hit) {
                    target.f[0] = FUN_001f9dc8(angle) * radius * 5.0f;
                    target.f[1] = FUN_001f9de0(angle) * radius * 5.0f;
                    target.f[2] = vertical * 5.0f;
                    add_vector_xyz(&target, &target, &D_L00_00166DC0_2c1ad0);
                }
            }
        } else {
            s32 entry = 0;
            struct Moby *candidate;
            hero.unkA50 = 0;
            while ((candidate = D_L00_001ABA00_2c26c8[entry]) != 0) {
                u8 *data = FUN_002141f8_2c26c8(candidate);
                if ((s8)candidate->state >= 0 && data && (candidate->flags & 0x1000)) {
                    f32 candidateAngle, elevation, difference;
                    qcopy(&temporary.arc, &candidate->pos);
                    temporary.arc.f[2] += *(f32 *)(data + 0x10);
                    distance = dist2(&pos, &temporary.arc);
                    candidateAngle = atan2_f(temporary.arc.f[0] - pos.f[0], temporary.arc.f[1] - pos.f[1]);
                    elevation = atan2_f(dist2(&pos, &temporary.arc), temporary.arc.f[2] - pos.f[2]);
                    difference = adiff(angle, candidateAngle);
                    if ((difference < 0.785398185f || (distance < 4.0f && difference < 1.57079637f)) && distance < best && elevation < 0.959931076f && temporary.arc.f[2] - pos.f[2] < 2.0f && !collision_line(&D_L00_00166DC0_2c1ad0, &temporary.arc, 6, moby, 0)) {
                        best = distance;
                        hero.unkA50 = (u8 *)candidate;
                        angle = candidateAngle;
                        qcopy(&target, &temporary.arc);
                    }
                }
                ++entry;
            }
        }
        gravity = frame_time_sq_macro * 11.0f;
        speed = (controller_state.held & 5) ? scale_ticks(D_0013E52A[0]) * 2.5f + 8.5f : 8.5f;
        aim_throw_arc((V2c2488 *)&pos, (V2c2488 *)&target, out, gravity, 0.00000999999975f, speed);
    }
    switch (moby->state) {
    case 0:
        vars->held = 0;
        if (!(moby->anim_flags & 2))
            moby->state = 1;
        else
            moby->state = 2;
    case 1:
        if (moby->anim_flags & 2)
            moby->state = 2;
        break;
    case 3:
        if (vars->held == 0) {
            vars->held = FUN_L00_002ace70(moby, &vars->grip);
        } else {
            FUN_L00_002acfd8(&pos, vars->held, vars->held->pvars);
            if (D_00141680_2c26c8.deliveries <= 0xfffe)
                ++D_00141680_2c26c8.deliveries;
            {
                s32 frames = scale_game_frames(D_0015EEA4);
                frames /= 600;
                if (D_00141680_2c26c8.unkA2 < frames)
                    D_00141680_2c26c8.unkA2 = scale_game_frames(D_0015EEA4) / 600;
            }
            D_00141680_2c26c8.levels = (D_00141680_2c26c8.levels | (1u << current_level_index)) | 0x80000000;
            vars->held = 0;
            moby->state = 4;
            vars->timer = scale_game_frames(0x14);
        }
        break;
    case 4:
        if (hero.state.current != 0x23 && !hero.unk20A8)
            moby->state = 2;
        break;
    case 5:
        moby->state = 6;
        break;
    case 6:
        /* Retail reloads held after destruction before the common tail. */
        if (vars->held != 0)
            mark_moby_for_removal(vars->held);
        break;
    }
    if (vars->held != 0)
        qcopy(&vars->held->pos, &vars->grip);
    else if (FUN_L00_00233e98_2c1ad0(-1) || hero.unk20A8)
        vars->held = FUN_L00_002ace70(moby, &vars->grip);
}
extern u8 D_0013E539;
extern s32 D_L00_0015FFD8_2c35c0 __asm__("D_L00_0015FFD8");
extern s16 D_L00_001B0830_lui[]  __asm__("D_L00_001B0830")__attribute__((section(".data")));
extern f32 FUN_002132a8_2c35c0(f32, f32) __asm__("FUN_002132a8");
extern void FUN_L00_0025f800(void *, void *);
extern void FUN_L00_0025d1b8(void *);
extern void FUN_L00_0025f780(void *, void *, s32);

u8 *FUN_L00_002c35c0(s32 value, u128 *position, u128 *other_position) {
    u8 *flags = item_text_variant;
    u8 *m;
    u8 *data;
    u8 *candidate;
    s32 i;
    s32 best_value;
    s32 current_value;
    f32 speed;

    m = create_moby(flags[0x19] ? 0x76c : 0xcb);
    if (m != 0) {
        m[0x30] = 0xff;
        *(u16 *)(m + 0x32) = 0xff;
        m[0x31] = 1;
        data = *(u8 **)(m + 0x78);
        m[0x20] = 0;
        qcopy(m + 0x10, position);
        qcopy(data + 0x10, other_position);
        speed = D_0015ED60_2c5be8 * -0.5f;
        *(s32 *)(data + 0x40) = value;
        *(u16 *)(data + 0x44) = 0;
        *(s32 *)(data + 0x54) = 0;
        *(f32 *)(m + 0x2c) = *(f32 *)(*(u8 **)(m + 0x24) + 0x24) * (speed + 1.0f);
        *(f32 *)(data + 0x48) = (scale_ticks(flags[0x19]) + 1.0f) * 4.0f;
        if (m[0x53] != 0)
            blend_moby_animation(m, 0, 0, scale_game_frames(0));
        *(s32 *)(m + 0x94) = 0;
        *(s32 *)(m + 0x98) = -1;
        *(f32 *)(m + 0x48) = FUN_002132a8_2c35c0(-3.14159274f, 3.14159274f);
        *(s32 *)(data + 0x58) = 0;
        *(s32 *)(data + 0x5c) = 0;
        while (D_L00_001B0830_lui[0] >= 6) {
            candidate = 0;
            best_value = 0;
            for (i = 1; i <= D_L00_001B0830_lui[0]; i++) {
                u8 *entry = (u8 *)(D_L00_0015FFD8_2c35c0 + (D_L00_001B0830_lui[i] << 8));
                if (entry != 0 &&
                    (*(s16 *)(entry + 0xa6) == 0xcb || *(s16 *)(entry + 0xa6) == 0x76c) &&
                    entry[0x20] < 5) {
                    current_value = *(s32 *)(*(u8 **)(entry + 0x78) + 0x54);
                    if (best_value <= current_value) {
                        candidate = entry;
                        best_value = current_value;
                    }
                }
            }
            if (candidate != 0) {
                FUN_L00_0025f800(candidate, D_L00_001B0830_lui);
                candidate[0x20] = 6;
            }
        }
        FUN_L00_00250df8(m);
        FUN_L00_0025d1b8(m);
        *(u16 *)data = 0;
        data[8] = 0;
        data[9] = 0;
        data[7] = 0x80;
        *(u16 *)(data + 0x0c) = 4;
        *(u16 *)(data + 0x0e) = 0xf;
        FUN_L00_0025f780(m, D_L00_001B0830_lui, 0x1f);
        if (hero.unk20A5 != 0 || hero.unk20AF != 0)
            *(u16 *)(m + 0x34) |= 0x41;
    }
    return m;
}
#include "qzero.h"

/* Tests a movement vector against the player and turns a colliding moby. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C4B90), where it is exact; names translated to the US level program. */


extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float square_root(float) __asm__("FUN_001f9988");
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
typedef struct { char pad[0x28]; float height; char pad2[0x14]; float vector[4]; } S173E40;
extern S173E40 D_L00_00173E40;
extern int FUN_001efa68(void *,void *,int,int,int);
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern unsigned char *FUN_L00_002603d0(int);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern char D_00173F60_alias[] __asm__("D_L00_00173E60");

void FUN_L00_002c3888(float *pos,char *moby,void *vec) {
 float a[4],b[4],c[4];
 char *state=*(char **)(moby+0x78);
 struct Hero *camera;
 char *player;
 qcopy(state+0x10,vec);
 { struct Hero *view = &hero;
   if(((int)view->ground_moby) && FUN_L00_002603d0(((int)view->ground_moby))) add_vector_xyz(state+0x10,state+0x10,&view->motion.displacement);
 }
 qcopy(moby+0x10,pos);
 camera = &hero;
 player=((char *)camera->moby);
 *(OvlQuad *)a=*(OvlQuad *)(player+0x10); a[2]=pos[2];
 subtract_vector_xyz(pos,pos,a);
 normalize_vector_xyz(pos,pos,vector_length_xyz(pos)+0.2f);
 add_vector_xyz(pos,pos,a);
 if(FUN_001efa68(a,pos,0,((int)camera->moby),0)) {
  float smoothing;
  qcopy(moby+0x10,D_00173F60_alias); qzero(state+0x10);
  moby[0x20]=4;
  *(short *)(state+0x44)=scale_game_frames_c(30);
  *(float *)(moby+0x18)=D_L00_00173E40.height;
  qcopy(b,D_L00_00173E40.vector);
  { float co,si;
  co=fast_cos(*(float *)(moby+0x48)); smoothing=0.1f; si=fast_sin(*(float *)(moby+0x48));
  c[0]=b[0]*co+b[1]*si; }
  { float co,si;
  co=fast_cos(*(float *)(moby+0x48)); si=fast_sin(*(float *)(moby+0x48));
  c[1]=b[1]*co-b[0]*si; c[2]=b[2]; }
  *(float *)(state+0x4C)=-fast_arc_tan(square_root(c[0]*c[0]+c[2]*c[2]),c[1]);
  *(float *)(state+0x50)=fast_arc_tan(c[2],c[0]);
  *(float *)(moby+0x40)=fast_add_rotations(*(float *)(moby+0x40),fast_subtract_rotations(*(float *)(state+0x4C),*(float *)(moby+0x40))*(frame_scale*smoothing));
  *(float *)(moby+0x44)=fast_add_rotations(*(float *)(moby+0x44),fast_subtract_rotations(*(float *)(state+0x50),*(float *)(moby+0x44))*(frame_scale*smoothing));
 } else moby[0x20]=2;
}

/* Vars of a thrown moby (FUN_L00_002c3888 uses the same block). */
typedef struct {
    u8 pad_00[0x10];
    Vec4 vel;                      /* 0x10: velocity per step */
    Vec4 landing;                  /* 0x20: predicted landing point */
    Vec4 normal;                   /* 0x30: surface normal there */
    struct Moby *holder;           /* 0x40: moby it was thrown from */
    s16 timer;                     /* 0x44 */
    s16 unk46;
} ThrowVars;

extern s32 D_L00_0015F5C4;                          /* nonzero: no throws */
extern f32 D_0015ED6C __attribute__((section(".sdata")));   /* frame-rate scale */
extern f32 frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern CollisionHit coll_hit __asm__("D_L00_00173E40") __attribute__((section(".data")));
/* coll_hit's normal as a whole quadword (CollisionHit names only x/y/z) */
extern Vec4 coll_hit_normal __asm__("D_L00_00173E80") __attribute__((section(".data")));
extern Vec4 D_L00_00166DC0 __attribute__((section(".data")));
extern f32 atan2_f(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 FUN_001f9dc8(f32);                        /* cos */
extern f32 FUN_001f9de0(f32);                        /* sin */
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern s32 tick_countdown(s32 *) __asm__("FUN_001f9740");
extern void probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");
extern void FUN_001f4600(void *, void *);
extern void FUN_L00_002c31c0(void);

/*
 * Predicts where a thrown moby lands: steps the velocity (with gravity) from
 * its start until collision_line hits ground (or a moby other than the hero
 * and the holder), for up to `timer` steps. Stores the landing point and
 * normal in vars and pulls the point 5% toward D_L00_00166DC0. mode 0 needs
 * a live hit moby; mode 1 starts from the hero's hand. Returns 1 on a hit.
 */
s32 FUN_L00_002c3af8(ThrowVars *v, struct Moby *m, s32 mode)
{
    Vec4 d, e, a, q, w;
    s32 t;
    s32 found = 0;
    s32 i;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4 != 0)
        return 0;
    qcopy(&a, &m->pos);
    if (m->state == 1) {
        d.f[0] = FUN_001f9dc8(fast_add_rotations(-0.36196801066398620605f, atan2_f(hero.unk670, hero.unk674))) * 0.85903900861740112305f;
        d.f[1] = FUN_001f9de0(fast_add_rotations(-0.36196801066398620605f, atan2_f(hero.unk670, hero.unk674))) * 0.85903900861740112305f;
        d.f[2] = 0.0f;
        add_vector_xyz(&d, &d, &hero.motion.pos);
        d.f[2] += 0.39081999659538269043f;
        qcopy(&e, &v->vel);
        if (hero.state.control_mode == 15)
            add_vector_xyz(&e, &e, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0((int)hero.ground_moby) && mode)
            add_vector_xyz(&e, &e, &hero.motion.displacement);
        t = scale_game_frames(300);
    } else {
        u8 *holder_vars = v->holder->pvars;

        qcopy(&d, &m->pos);
        qcopy(&e, &v->vel);
        {
            s32 timer = v->timer;
            s32 flag = *(s32 *)(holder_vars + 0x50);

            t = timer;
            if (flag)
                v->unk46 = 1;
        }
    }
    e.f[2] -= D_0015ED6C * 0.5f;
    q.q = ((Vec4 *)&hero.moby->pos)->q;
    q.f[2] = d.f[2];
    if (collision_line(&q, &d, 0x10, hero.moby, 0) && (FUN_001f0b58() || 0.0f < e.f[2])) {
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby != hero.moby && (struct Moby *)coll_hit.moby != v->holder)
                || t < scale_game_frames(300) - scale_game_frames(10)) {
                w.q = 0;
                w.f[2] = 0.5f;
                qcopy(&v->landing, &((struct Moby *)coll_hit.moby)->pos);
                found = 1;
                add_vector_xyz(&w, &w, &v->landing);
                probe_ground_height(&w, 0, 0.5f);
            }
        } else if (coll_hit.unk1C > 0) {
            qcopy(&v->landing, &coll_hit.point);
            found = 1;
        }
    }
    while (!tick_countdown(&t) && !found) {
        qcopy(&a, &d);
        for (i = 9; i >= 0; i--) {
            add_vector_xyz(&d, &d, &e);
            e.f[2] -= frame_time_sq_macro * 9.0f;
        }
        if (!collision_line(&a, &d, 0x10, m, 0))
            continue;
        if (!FUN_001f0b58() && 0.0f < e.f[2])
            continue;
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby == hero.moby || (struct Moby *)coll_hit.moby == v->holder)
                && !(t < scale_game_frames(300) - scale_game_frames(10)))
                continue;
            w.q = 0;
            w.f[2] = 0.5f;
            qcopy(&v->landing, &((struct Moby *)coll_hit.moby)->pos);
            add_vector_xyz(&w, &w, &v->landing);
            probe_ground_height(&w, 0, 0.5f);
            break;
        } else if (coll_hit.unk1C > 0) {
            qcopy(&v->landing, &coll_hit.point);
            break;
        }
    }
    if (t == 0)
        return 0;
    if (mode == 0) {
        if (coll_hit.moby == 0)
            return 0;
        if (FUN_L00_002603d0(coll_hit.moby) == 0)
            goto ret0;
    }
    if (mode != 2 || coll_hit.moby == 0 || !FUN_L00_002603d0(coll_hit.moby))
        FUN_001f4600(FUN_L00_002c31c0, m);
    v->normal.q = coll_hit_normal.q;
    subtract_vector_xyz(&a, &v->landing, &D_L00_00166DC0);
    scale_vector_xyz(&a, &a, 0.94999998807907104492f);
    add_vector_xyz(&v->landing, &a, &D_L00_00166DC0);
    return 1;
ret0:
    /* a second return 0 block, as in retail */
    return 0;
}
extern char *D_0015FFD8_2c4380 __asm__("D_L00_0015FFD8");
extern float D_0015ED60_2c4380 __asm__("D_0015ED60");
extern float D_0015ED70_2c4380 __asm__("D_0015ED70");
extern float D_0015ED6C_2c4380 __asm__("D_0015ED6C");
extern float FUN_00200240_2c4380(int) __asm__("FUN_001fa6c0");
extern int FUN_L00_0025a420(void *, int, int);
extern int FUN_L00_00258830(void *, void *, float, float, float, int);
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern float FUN_L00_00259698_2c4380(void *, int, float) __asm__("FUN_00213508");
extern void FUN_L00_00260400(void *, int, void *, void *, void *, void *);
extern void FUN_L00_002c40a0(S_2c40a0 *);
extern void FUN_L00_002c4328(void *);
extern void FUN_L00_0025d458(void *, void *);
extern void FUN_L00_0025d538(void *, void *);
extern void FUN_L00_0025f8e0(float, void *);
extern void FUN_L00_0025e450(float, void *, void *, int, float, float, int, int, int,
                              float, float, float, float, int, int, int, int, int);
extern void FUN_L00_00259858(float, void *, void *, int);
extern void collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_0024eb68_2c4380(void *) __asm__("FUN_0020c828");
extern void FUN_00257880_2c4380(void *, int, int, int) __asm__("FUN_00212f90");
extern float FUN_00257be8_2c4380(float, float) __asm__("FUN_002132a8");
extern void FUN_0028dbe8_2c4380(int, int, void *) __asm__("FUN_0022da68");
extern int FUN_001fef20_2c4380(int) __asm__("FUN_001f96f8");
extern int FUN_001fef68_2c4380(void *) __asm__("FUN_001f9740");
extern int FUN_001fef98_2c4380(void *) __asm__("FUN_001f9770");
extern int FUN_001fefc8_2c4380(void *) __asm__("FUN_L00_001fefc8");
extern float FUN_001ff3d8_2c4380(void *) __asm__("FUN_001f9af0");
extern float FUN_001ff408_2c4380(void *) __asm__("FUN_001f9b20");
extern float FUN_001ff1e0_2c4380(float) __asm__("FUN_001f9988");
extern void FUN_001ff2a8_2c4380(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001ff278_2c4380(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001ff300_2c4380(float, void *, void *) __asm__("FUN_001f9a68");
extern void FUN_001ff660_2c4380(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern float FUN_001ff8b0_2c4380(float, float) __asm__("FUN_001f9e90");
extern float FUN_001ff7e8_2c4380(float) __asm__("FUN_001f9dc8");
extern float FUN_001ff800_2c4380(float) __asm__("FUN_001f9de0");
extern float FUN_00200130_2c4380(float, float) __asm__("FUN_001fa5c8");
extern float FUN_002000e8_2c4380(float, float) __asm__("FUN_001fa580");
extern void FUN_L00_0025f800(void *, void *);

void FUN_L00_002c4380(char *moby) {
    int active = D_0013E533[6];
    char *vars = *(char **)(moby + 0x78);
    char *target;
    float v[4] __attribute__((aligned(16)));
    struct { float w[4]; float x[4]; } scratch __attribute__((aligned(16)));
    float rotated_normal[4] __attribute__((aligned(16)));
    float len, angle, cos_angle, sin_angle, rate;
    int i, hit, state;
    short *ids;
    float effect;
    if (*(short *)(moby + 0xa6) == 0xcb)
        active = 0;
    effect = FUN_00200240_2c4380(active);
    if (*(float *)(moby + 0x10) < 2.0f || 1021.0f < *(float *)(moby + 0x10) ||
        *(float *)(moby + 0x14) < 2.0f || 1021.0f < *(float *)(moby + 0x14) ||
        *(float *)(moby + 0x18) < 2.0f || 1021.0f < *(float *)(moby + 0x18)) {
        FUN_L00_0025f800(moby, D_L00_001B0830_lui);
        FUN_L00_0024eb68_2c4380(moby);
        return;
    }
    if ((hero.items[0].moby == 0 || *(short *)((char *)hero.items[0].moby + 0xa6) != 0x232) &&
        (u8)moby[0x20] == 1) {
        FUN_L00_0025f800(moby, D_L00_001B0830_lui);
        FUN_L00_0024eb68_2c4380(moby);
        return;
    }
    if (hero.vehicle_mode != 0 && (u8)moby[0x20] == 1) {
        FUN_L00_0025f800(moby, D_L00_001B0830_lui);
        FUN_L00_0024eb68_2c4380(moby);
        return;
    }
    ++*(int *)(vars + 0x54);
    if (hero.unk20A5 != 0 && (u8)moby[0x20] < 2)
        *(u16 *)(moby + 0x34) |= 0x41;
    else
        *(u16 *)(moby + 0x34) &= 0xffbe;
    if (*(short *)(*(char **)(vars + 0x40) + 0xa6) == 0x232 && *(short *)(vars + 0x46) == 0 &&
        FUN_001ff3d8_2c4380(vars + 0x10) > 0.0f) {
        if (hero.ground_moby == 0 || !FUN_L00_002603d0((int)hero.ground_moby))
            FUN_L00_002c3af8((ThrowVars *)vars, (struct Moby *)moby, 1);
        else if (!FUN_L00_002c3af8((ThrowVars *)vars, (struct Moby *)moby, 0))
            FUN_L00_002c3af8((ThrowVars *)vars, (struct Moby *)moby, 2);
    }
    switch ((u8)moby[0x20]) {
    case 0:
        moby[0x20] = 1;
        *(short *)(vars + 0x44) = 0;
        *(int *)(moby + 0x98) = -1;
        *(short *)(vars + 0x6c) = 0;
        vars[0x6e] = 0;
        break;
    case 1: {
        char *target;
        *(short *)(vars + 0x44) = 0;
        target = *(char **)(vars + 0x40);
        if (target && (((u8)target[0x20] & 0xf0) != 0xf0) && hero.items[0].moby &&
            *(short *)((char *)hero.items[0].moby + 0xa6) == 0x232)
            return;
        FUN_L00_0025f800(moby, D_L00_001B0830_lui);
        FUN_L00_0024eb68_2c4380(moby);
        return;
    }
    case 2:
        if (*(float *)(moby + 0x2c) < *(float *)(*(char **)(moby + 0x24) + 0x24))
            *(float *)(moby + 0x2c) += D_0015ED60_2c4380 * 0.02f;
        qcopy(v, moby + 0x10);
        *(float *)(vars + 0x18) -= D_0015ED70_2c4380 * 9.0f;
        for (i = 1; i <= D_L00_001B0830_lui[0]; ++i) {
            char *target;
            ids = D_L00_001B0830_lui + i;
            target = (char *)D_0015FFD8_2c4380 + *ids * 0x100;
            if (target != moby && (u8)target[0x20] > 1 && target &&
                (u8)target[0x20] != 0xfe && (u8)target[0x20] != 0xfd) {
                float len;
                FUN_001ff2a8_2c4380(scratch.w, moby + 0x10, target + 0x10);
                scratch.w[2] = 0.0f;
                len = FUN_001ff3d8_2c4380(scratch.w);
                if (len < 1.8f) {
                    FUN_001ff300_2c4380((1.8f - len) * 0.015f, scratch.w, scratch.w);
                    FUN_001ff278_2c4380(vars + 0x10, vars + 0x10, scratch.w);
                }
            }
        }
        if (hero.state.current == 0x1e)
            *(int *)((char *)hero.moby + 0x94) = 0;
        hit = FUN_L00_00258830(moby, vars + 0x10, 0.2f, 0.2f, 0.0f, 0);
        if (hit & 1) {
            qcopy(scratch.w, moby + 0x10);
            scratch.w[2] += 0.2f;
            if (coll_sphere(0.23f, scratch.w, 0, moby)) {
                if (*(float *)(vars + 0x18) < 0.0f) {
                    float len;
                    FUN_001ff2a8_2c4380(scratch.x, moby + 0x10, &hero.motion.pos);
                    scratch.x[2] = 0.0f;
                    len = FUN_001ff3d8_2c4380(scratch.x);
                    if (len < 1.8f) {
                        FUN_001ff300_2c4380((1.8f - len) * 0.015f, scratch.x, scratch.x);
                        FUN_001ff278_2c4380(vars + 0x10, vars + 0x10, scratch.x);
                    }
                }
                FUN_001ff660_2c4380(vars + 0x10, vars + 0x10, &coll_hit.normal_x);
                if (coll_hit.moby != 0 && coll_hit.unk1C <= 0)
                    FUN_001ff300_2c4380(1.05f, vars + 0x10, vars + 0x10);
                else
                    FUN_001ff300_2c4380(0.2f, vars + 0x10, vars + 0x10);
                rate = 0.01f;
                len = FUN_001ff3d8_2c4380(vars + 0x10);
                if (len < D_0015ED60_2c4380 * rate ||
                    (hero.ground_moby && FUN_L00_002603d0((int)hero.ground_moby) &&
                     FUN_001ff3d8_2c4380(vars + 0x10) < D_0015ED60_2c4380 * rate +
                           FUN_001ff3d8_2c4380(&hero.motion.displacement))) {
                    angle = FUN_001ff8b0_2c4380(coll_hit.normal_z,
                                                 FUN_001ff408_2c4380(&coll_hit.normal_x));
                    if (angle < 0.61086524f) {
                        ++*(short *)(vars + 0x44);
                        if (coll_hit.moby == 0 ||
                            coll_hit.unk1C > 0 ||
                            *(short *)(vars + 0x44) > FUN_001fef20_2c4380(240)) {
                            if (((u8 *)moby)[0x53] != 1)
                                FUN_00257880_2c4380(moby, 1, 0, FUN_001fef20_2c4380(10));
                            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
                            *(int *)(moby + 0x98) = 0;
                            qzero(vars + 0x10);
                            moby[0x20] = 3;
                            *(short *)(vars + 0x44) = FUN_001fef20_2c4380(600);
                            *(float *)(moby + 0x18) = coll_hit.point.f[2];
                            qcopy(scratch.x, &coll_hit.normal_x);
                            cos_angle = FUN_001ff7e8_2c4380(*(float *)(moby + 0x48));
                            sin_angle = FUN_001ff800_2c4380(*(float *)(moby + 0x48));
                            rotated_normal[0] = scratch.x[0] * cos_angle + scratch.x[1] * sin_angle;
                            cos_angle = FUN_001ff7e8_2c4380(*(float *)(moby + 0x48));
                            sin_angle = FUN_001ff800_2c4380(*(float *)(moby + 0x48));
                            rotated_normal[1] = scratch.x[1] * cos_angle - scratch.x[0] * sin_angle;
                            rotated_normal[2] = scratch.x[2];
                            *(float *)(vars + 0x4c) = -FUN_001ff8b0_2c4380(
                                FUN_001ff1e0_2c4380(rotated_normal[0] * rotated_normal[0] + rotated_normal[2] * rotated_normal[2]), rotated_normal[1]);
                            *(float *)(vars + 0x50) = FUN_001ff8b0_2c4380(rotated_normal[2], rotated_normal[0]);
                            rate = 0.1f;
                            *(float *)(moby + 0x40) = FUN_002000e8_2c4380(*(float *)(moby + 0x40),
                                FUN_00200130_2c4380(*(float *)(vars + 0x4c), *(float *)(moby + 0x40)) *
                                (D_0015ED60_2c4380 * rate));
                            *(float *)(moby + 0x44) = FUN_002000e8_2c4380(*(float *)(moby + 0x44),
                                FUN_00200130_2c4380(*(float *)(vars + 0x50), *(float *)(moby + 0x44)) *
                                (D_0015ED60_2c4380 * rate));
                        }
                    }
                }
                FUN_L00_002c4328(moby);
                if (FUN_001fefc8_2c4380(vars + 0x6f))
                    FUN_0028dbe8_2c4380(0, 0, moby), vars[0x6f] = FUN_001fef20_2c4380(20);
                else
                    FUN_L00_0025f8e0(D_0015ED6C_2c4380, vars + 0x10);
            }
        }
        if (hero.state.current == 0x1e) {
            target = hero.moby;
            *(int *)(target + 0x94) = *(int *)(*(char **)(target + 0x24) + 0x10);
        }
        break;
    case 3:
    case 4:
        hit = FUN_L00_0025a420(moby, 0x30001, 0);
        *(float *)(moby + 0x40) = FUN_002000e8_2c4380(*(float *)(moby + 0x40),
            FUN_00200130_2c4380(*(float *)(vars + 0x4c), *(float *)(moby + 0x40)) *
            (D_0015ED60_2c4380 * 0.1f));
        *(float *)(moby + 0x44) = FUN_002000e8_2c4380(*(float *)(moby + 0x44),
            FUN_00200130_2c4380(*(float *)(vars + 0x50), *(float *)(moby + 0x44)) *
            (D_0015ED60_2c4380 * 0.1f));
        FUN_L00_002c40a0((S_2c40a0 *)moby);
        FUN_001fef68_2c4380(vars + 0x5c);
        FUN_001fef98_2c4380(vars + 0x6c);
        if (hit) {
            target = (char *)hit;
            if (*(int *)(target + 0x20) != *(int *)(vars + 0x58) ||
                *(int *)(vars + 0x5c) <= 0) {
                *(int *)(vars + 0x58) = *(int *)(target + 0x20);
                *(int *)(vars + 0x5c) = FUN_001fef20_2c4380(30);
                /* Fakematch: the empty loop keeps the timer read below the cooldown store. */
                do {} while (0);
                if (*(short *)(vars + 0x6c) == 0) {
                    if (*(u16 *)(target + 0x28) == 0x101 &&
                        ((*(u64 *)(vars + 0x68) & 0xffffff00000000ULL) == 0)) {
                        *(short *)(vars + 0x6c) = FUN_001fef20_2c4380(100);
                        vars[0x6e] = 1;
                    } else
                        vars[0x6e] = 0;
                    FUN_0028dbe8_2c4380(0, 0, moby);
                    {
                        float len = *(float *)(target + 0x2c);
                        ((u8 *)vars)[7] = 0xfa;
                        *(float *)(vars + 0x48) -= len;
                    }
                    FUN_L00_0025d458(moby, vars);
                    if (*(float *)(vars + 0x48) < 0.0f) {
                        *(OvlQuad *)scratch.w = 0;
                        scratch.w[0] = D_0015ED60_2c4380 * 0.5f;
                        scratch.w[2] = D_0015ED60_2c4380 * 0.7f;
                        scratch.w[1] = scratch.w[0];
                        *(OvlQuad *)v = *(OvlQuad *)scratch.w;
                        FUN_L00_0025f8e0(scratch.w[0], v);
                        FUN_L00_0025e450(0, moby, v, 0, 0, 4.0f, 0, 0, 0,
                                         3.0f, 9.0f, 1.0f, 12.0f, 2, 1, 1, -1, active);
                        if (active) {
                            FUN_L00_00259858(effect + effect, scratch.w, moby, 0x810000);
                            ((u8 *)scratch.x)[8] = 2;
                            ((u8 *)scratch.x)[9] = 1;
                            *(u16 *)((u8 *)scratch.x + 10) = *(u16 *)(moby + 0xa6);
                            collect_mobies_in_sphere(effect * 3.0f, moby + 0x10, 0x10, moby, scratch.w);
                        }
                        FUN_L00_0025f800(moby, D_L00_001B0830_lui);
                        FUN_L00_0024eb68_2c4380(moby);
                        return;
                    }
                }
            }
        }
        ((u8 *)moby)[0xa4] = 0xff;
        FUN_L00_0025d538(moby, vars);
        if (moby[0x70] & 2) {
            if (((u8 *)moby)[0x53] != 2)
                FUN_00257880_2c4380(moby, 2, 0, FUN_001fef20_2c4380(10));
            *(float *)(moby + 0x58) = FUN_00257be8_2c4380(0.7f, 1.05f);
        }
        if ((u8)moby[0x20] != 4) {
            len = FUN_L00_00259698_2c4380(moby + 0x10, 0, 0.5f);
            if (len != 0.0f) {
                FUN_L00_002c4328(moby);
                if (len + 0.02f < *(float *)(moby + 0x18)) {
                    *(float *)(vars + 0x18) -= D_0015ED70_2c4380 * 9.8f;
                    FUN_001ff278_2c4380(moby + 0x10, moby + 0x10, vars + 0x10);
                    len = FUN_L00_00259698_2c4380(moby + 0x10, 0, 0.5f);
                    if (*(float *)(moby + 0x18) < len) {
                        *(float *)(moby + 0x18) = len;
                        qzero(vars + 0x10);
                        qcopy(v, &coll_hit.normal_x);
                        cos_angle = FUN_001ff7e8_2c4380(*(float *)(moby + 0x48));
                        sin_angle = FUN_001ff800_2c4380(*(float *)(moby + 0x48));
                        scratch.w[0] = v[0] * cos_angle + v[1] * sin_angle;
                        cos_angle = FUN_001ff7e8_2c4380(*(float *)(moby + 0x48));
                        sin_angle = FUN_001ff800_2c4380(*(float *)(moby + 0x48));
                        scratch.w[1] = v[1] * cos_angle - v[0] * sin_angle;
                        scratch.w[2] = v[2];
                        *(float *)(vars + 0x4c) = -FUN_001ff8b0_2c4380(
                            FUN_001ff1e0_2c4380(scratch.w[0] * scratch.w[0] + scratch.w[2] * scratch.w[2]), scratch.w[1]);
                        *(float *)(vars + 0x50) = FUN_001ff8b0_2c4380(scratch.w[2], scratch.w[0]);
                    }
                } else
                    qzero(vars + 0x10);
                if (coll_hit.moby)
                    FUN_L00_00260400(moby, coll_hit.moby, moby + 0x10,
                                     moby + 0x40, moby + 0x10, moby + 0x40);
            }
        } else {
            if (*(float *)(moby + 0x2c) < *(float *)(*(char **)(moby + 0x24) + 0x24))
                *(float *)(moby + 0x2c) += D_0015ED60_2c4380 * 0.02f;
            if (FUN_001fef98_2c4380(vars + 0x44)) {
                if (((u8 *)moby)[0x53])
                    FUN_00257880_2c4380(moby, 0, 0, FUN_001fef20_2c4380(60));
                FUN_L00_0025f800(moby, D_L00_001B0830_lui);
                moby[0x20] = 5;
            }
        }
        break;
    case 5:
        len = FUN_L00_00259698_2c4380(moby + 0x10, 0, 0.5f);
        if (len != 0.0f && coll_hit.moby)
            FUN_L00_00260400(moby, coll_hit.moby, moby + 0x10,
                             moby + 0x40, moby + 0x10, moby + 0x40);
        if (moby[0x70] & 2)
            *(int *)(moby + 0x58) = 0;
        if (FUN_001fef98_2c4380(vars + 0x44) || (moby[0x70] & 2))
            moby[0x20] = 6;
        break;
    case 6: {
        float len;
        len = FUN_L00_00259698_2c4380(moby + 0x10, 0, 0.5f);
        if (len != 0.0f && coll_hit.moby) {
            FUN_L00_00260400(moby, coll_hit.moby, moby + 0x10,
                             moby + 0x40, moby + 0x10, moby + 0x40);
        }
        len = D_0015ED60_2c4380;
        *(float *)(moby + 0x2c) -= *(float *)(moby + 0x2c) * (len * 0.02f);
        if (!(*(float *)(moby + 0x2c) <
            *(float *)(*(char **)(moby + 0x24) + 0x24) * 0.1f))
            return;
        FUN_L00_0024eb68_2c4380(moby);
        break;
    }
    case 7:
    default:
        moby[0x20] = 0;
    }
}
extern int D_L00_0015F5CC_2c53c0 __asm__("D_L00_0015F5CC") __attribute__((section(".sdata")));
extern float D_0015ED60_2c53c0 __asm__("D_0015ED60") __attribute__((section(".sdata")));
extern float D_0015ED6C_2c53c0 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern int FUN_001f96f8_2c53c0(int) __asm__("FUN_001f96f8");
extern float FUN_001f99c0_2c53c0(float) __asm__("FUN_001f99c0");
extern void FUN_001f9a10_2c53c0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a28_2c53c0(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f9af0_2c53c0(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9b48_2c53c0(void *, void *) __asm__("FUN_001f9b48");
extern float FUN_001f9b80_2c53c0(void *, void *) __asm__("FUN_001f9b80");
extern void FUN_001f9bf8_2c53c0(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_001f9dc8_2c53c0(float) __asm__("FUN_001f9dc8");
typedef struct {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) FxVec;
typedef struct {
    char pad[0x34];
    short sound_multiple;
    short sound_single;
    char pad2[0x14];
} FxAmmoRecord;
extern char ammo_sounds_2c53c0[] __asm__("D_L00_00179AC0");
extern FxVec D_L00_00173E70_2c53c0 __asm__("D_L00_00173E70");
extern FxVec D_L00_00173E60_2c53c0 __asm__("D_L00_00173E60");
extern float frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern int FUN_001f9740_2c53c0(int *) __asm__("FUN_001f9740");
extern int FUN_L00_001f0d60_2c53c0(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int FUN_001efa68_2c53c0(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void FUN_001f99f8_2c53c0(void *) __asm__("FUN_001f99f8");
extern float FUN_00213ed8_2c53c0(void *, float, float) __asm__("FUN_00213ed8");
extern int FUN_001fa6d0_2c53c0(float) __asm__("FUN_001fa6d0");
extern int FUN_L00_00233e40_2c53c0(int, int) __asm__("FUN_L00_00233e40");
extern void FUN_L00_00263db0_2c53c0(int, int, int) __asm__("FUN_L00_00263db0");
extern int FUN_001f99a8_2c53c0(int) __asm__("FUN_001f99a8");
extern int FUN_L00_0028dc90_2c53c0(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern float FUN_L00_00250430_2c53c0(void *, void *) __asm__("FUN_L00_00250430");
extern void FUN_0020c828_2c53c0(void *) __asm__("FUN_0020c828");
typedef struct {
    int amount;
    char pad4[2];
    short recovery_timer;
    int weapon;
    char padC[4];
    Vec4f velocity;
    int timer;
    float rise_speed;
    float resting_height;
} AmmoPickup_2c53c0;

extern int D_L00_00161838_macro __asm__("D_L00_00161838") __attribute__((section(".sdata")));
/* Update ammunition pickup physics, collection, animation and scale.
   Animation states share one stack value; index reloads follow calls. */
void FUN_L00_002c53c0(struct Moby *m) {
    AmmoPickup_2c53c0 *p;
    unsigned int state;
    FxVec position;
    FxVec predicted;
    float value;
    float magnitude;
    float current;
    float target;
    float step;
    float remaining;
    p = (AmmoPickup_2c53c0 *)m->pvars;
    FUN_001f9740_2c53c0(&p->timer);
    state = m->state;
    if (state == 1 || state == 4 || state == 3) {
        magnitude = FUN_001f9af0_2c53c0(&p->velocity);
        if (magnitude != 0.0f) {
            qcopy(&position, &m->pos);
            position.z += 0.5f;
            if (FUN_L00_001f0d60_2c53c0(0.4f, &position, 2, 0)) {
                qcopy(&m->pos, &D_L00_00173E70_2c53c0);
                m->pos.z -= 0.5f;
            }
        }
        p->velocity.z -= frame_time_sq_macro * 4.9f;
        FUN_001f9a10_2c53c0(&predicted, &m->pos, &p->velocity);
        p->velocity.z -= frame_time_sq_macro * 4.9f;
        qcopy(&position, &m->pos);
        position.z += 0.1f;
        if (FUN_001efa68_2c53c0(&position, &predicted, 2, 0, 0)) {
            FUN_001f99f8_2c53c0(&p->velocity);
            qcopy(&m->pos, &D_L00_00173E60_2c53c0);
        } else {
            qcopy(&m->pos, &predicted);
        }
    }
    switch (m->state) {
    case 0:
        m->state = 1;
        break;
    case 1: {
        struct Hero *player;
        float *value_ptr;
        float height_delta;
        float distance;
        int duration;
        int eligible;
        int idx;
        int sound_idx;
        int account_idx;
        int before;
        int after;
        int difference;
        int sound;
        int elapsed;
        int random;
        int max_ammo;
        value_ptr = &value;
        *value_ptr = (float)m->alpha;
        duration = FUN_001f96f8_2c53c0(30);
        FUN_00213ed8_2c53c0(value_ptr, 128.0f, (float)(128 / duration));
        m->alpha = FUN_001fa6d0_2c53c0(*value_ptr);
        if (p->timer > 0)
            return;
        if (FUN_001f9af0_2c53c0(&p->velocity) != 0.0f)
            return;
        player = &hero;
        eligible = 1;
        height_delta =
            FUN_001f99c0_2c53c0(m->pos.z - player->motion.pos.f[2]);
        if (player->reach_height < height_delta) {
            eligible = 0;
        } else {
            distance = FUN_001f9b80_2c53c0(&m->pos, &player->motion.pos);
            if (player->reach_distance < distance)
                eligible = 0;
        }
        idx = p->weapon;
        before = weapon_ammo_counts[idx];
        max_ammo = D_L00_001C40B0[idx].hE;
        if (before >= max_ammo)
            eligible = 0;
        if (m->pos.z < p->resting_height - 2.0f)
            eligible = 1;
        if (hero.health.hp == 0)
            eligible = 0;
        if (!eligible)
            goto pickup_failed;

        {
            FUN_L00_00233e40_2c53c0(idx, p->amount);
            sound_idx = p->weapon;
            after = weapon_ammo_counts[sound_idx];
            if (before < after) {
                difference = after - before;
                if (difference == 1) {
                    sound = ((FxAmmoRecord *)(ammo_sounds_2c53c0 + sound_idx * sizeof(FxAmmoRecord)))->sound_single;
                } else {
                    sound = ((FxAmmoRecord *)(ammo_sounds_2c53c0 + sound_idx * sizeof(FxAmmoRecord)))->sound_multiple;
                }
                FUN_L00_00263db0_2c53c0(sound, difference, -1);
            }
            account_idx = p->weapon;
            D_0013DE08[account_idx] += weapon_ammo_counts[account_idx] - before;
            m->state = 2;
            m->scale *= D_0015ED60_2c53c0 * 0.20000005f + 1.0f;
            FUN_001f99f8_2c53c0(&p->velocity);
            elapsed = D_L00_0015F5CC_2c53c0 - D_L00_00161838_macro;
            p->rise_speed = D_0015ED6C_2c53c0 * 8.0f;
            random = FUN_001f99a8_2c53c0(elapsed);
            if (FUN_001f96f8_2c53c0(10) < random) {
                FUN_L00_0028dc90_2c53c0(0, 0, (int)m, 0xD5);
                D_L00_00161838_macro = D_L00_0015F5CC_2c53c0;
            }
            return;
        }
    pickup_failed:
        if (m->alpha < 0xFF)
            ++m->alpha;
        if (FUN_L00_00250430_2c53c0(m, hero.moby) <= -0.2f) {
            m->state = 3;
            p->recovery_timer = FUN_001f96f8_2c53c0(60);
        }
        return;
    }
    case 2: {
        float limit;
        float distance;
        float factor;
        float class_scale;
        FUN_00213ed8_2c53c0(&p->rise_speed, 0.0f, frame_time_sq_macro * 24.0f);
        m->pos.z += p->rise_speed;
        limit = FUN_001f9af0_2c53c0(&p->velocity);
        FUN_00213ed8_2c53c0(&limit, D_0015ED6C_2c53c0 * 64.0f, frame_time_sq_macro * 24.0f);
        FUN_001f9a28_2c53c0(&p->velocity, &hero.motion.unkD0, &m->pos);
        if (m->scale == 0.0f || FUN_001f9af0_2c53c0(&p->velocity) < limit) {
            FUN_0020c828_2c53c0(m);
            return;
        }
        FUN_001f9bf8_2c53c0(&p->velocity, &p->velocity, limit);
        FUN_001f9a10_2c53c0(&m->pos, &m->pos, &p->velocity);
        distance = FUN_001f9b48_2c53c0(&m->pos, &hero.motion.unkD0);
        factor = (float)FUN_001f96f8_2c53c0(30);
        factor = factor * limit;
        factor = factor + 0.01f;
        distance = distance / factor;
        if (1.0f < distance) {
            distance = 1.0f;
        } else if (distance < 0.3f) {
            distance = 0.3f;
        }
        class_scale = m->pclass->scale;
        FUN_00213ed8_2c53c0(&m->scale, class_scale * distance, class_scale * 0.1f);
        return;
    }
    case 3: {
        float *value_ptr;
        float angle;
        float oscillation;
        float target;
        int phase;
        int random;
        int idx;
        phase = D_L00_0015F5CC_2c53c0 % 60;
        value = (float)m->alpha;
        angle = (float)phase / 60.0f;
        angle = angle * 6.28318f;
        angle = angle - 3.14159f;
        oscillation = FUN_001f9dc8_2c53c0(angle);
        oscillation = oscillation * 5.0f;
        oscillation = oscillation + 5.0f;
        target = (float)(FUN_001fa6d0_2c53c0(oscillation) + 10);
        value_ptr = &value;
        random = FUN_001f96f8_2c53c0(30);
        FUN_00213ed8_2c53c0(value_ptr, target, (float)(128 / random));
        m->alpha = FUN_001fa6d0_2c53c0(*value_ptr);
        if (FUN_L00_00250430_2c53c0(m, hero.moby) >= 0.0f) {
            m->state = 1;
            return;
        }
        idx = p->weapon;
        if (weapon_ammo_counts[idx] <
                D_L00_001C40B0[idx].hE) {
            m->state = 1;
        }
        return;
    }
    case 4:
        current = m->scale;
        target = m->pclass->scale;
        step = D_0015ED60_2c53c0 * 0.03f;
        current += (target - current) * step;
        m->scale = current;
        target = m->pclass->scale;
        remaining = (target - current) / target;
        if (remaining < 0.01f) {
            m->scale = target;
            m->state = 1;
            m->collision = m->pclass->collision;
        }
        break;
    }
}
#ifndef NON_MATCHING
#include "rnc/input/pad_state.h"
#include "rnc/math/vector.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"



#include "qcopy.h"

/* Swingshot (class 208) update: keeps the hook at the glove, shows the targets, fires the hook to the swing target
   or the grind target, swings the hero, and reels the hook back. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C74B8), where it is exact; names translated to the US level program. */


typedef struct Moby Moby;


typedef int OvlQuad_6158 __attribute__((mode(TI)));

typedef unsigned int Q_2C74B8 __attribute__((mode(TI)));

typedef union { struct { float x, y, z, w; } f; Q_2C74B8 q; } V_2C74B8 __attribute__((aligned(16)));

typedef struct { V_2C74B8 row[3]; } M3_2C74B8;

typedef struct HookData_2C74B8 { char pad0[4]; float speed; } HookData_2C74B8;

typedef struct Data_2C74B8 {
    char pad0[0x14];
    Moby *hook;
    float spin;
    float scale;
    float size;
} Data_2C74B8;

extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern int FUN_001efa68(void *,void *,int,int,int);
extern int FUN_L00_00205000();
extern int FUN_L00_002223f8();
extern int FUN_L00_002323b8();
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
unsigned char *FUN_L00_002c6f88(unsigned char *src, float f);
void FUN_L00_001edf60(int a, int b, void *pos, int kind, int mode, float x, float y, float z);
extern int FUN_L00_002223f8_2bffa8(int, int) __asm__("FUN_L00_002223f8");
extern Moby *FUN_L00_002c6f88_2bffa8(void *, float) __asm__("FUN_L00_002c6f88");
extern void FUN_L00_001edf60_2bffa8(int, int, float, void *, int, float, int, float, int) __asm__("FUN_L00_001edf60");
extern int FUN_001f96f8_2bffa8(int) __asm__("FUN_001f96f8");
extern void FUN_00212f90_2bffa8(void *, int, int, int) __asm__("FUN_00212f90");
extern void FUN_L00_0024f7c8_2bffa8(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int FUN_0022da68_2bffa8(int, int, void *) __asm__("FUN_0022da68");
extern float FUN_001f9b48_2bffa8(void *, void *) __asm__("FUN_001f9b48");
extern void FUN_001f4600_2bffa8(void (*)(char *), void *) __asm__("FUN_001f4600");
extern void FUN_001f9a28_2bffa8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9bf8_2bffa8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_001f9a10_2bffa8(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001f9af0_2bffa8(void *) __asm__("FUN_001f9af0");
extern int FUN_001efa68_2bffa8(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_L00_00205000_2bffa8(int) __asm__("FUN_L00_00205000");
extern void FUN_L00_002323b8_2bffa8(int, int, float) __asm__("FUN_L00_002323b8");
extern void FUN_L00_002c5dc8_2bffa8(char *) __asm__("FUN_L00_002c5dc8");

void FUN_L00_002c6158(Moby *m) {
    V_2C74B8 v0;
    V_2C74B8 v10;
    Data_2C74B8 *d;
    HookData_2C74B8 *hd;
    float speed;
    int n;

    if (m == 0) return;
    if (hero.state.current == 1) FUN_L00_002223f8_2bffa8(30, 1);
    d = (void *)m->pvars;
    if (d == 0) return;
    hd = 0;
    if (!d->hook) d->hook = FUN_L00_002c6f88_2bffa8(m, frame_time * 24.0f);
    if (d->hook) hd = (void *)d->hook->pvars;
    if ((hero.state.current == 1 || hero.state.current == 30) &&
        hero.state_timer >= 2) {
        FUN_L00_001edf60_2bffa8((int)m, 0xFF0000FF, 1.0f, 0, 0x24, 0.0f, -1, 90.0f, 4);
        if (hero.unk968 && hero.unk964)
            FUN_L00_001edf60_2bffa8((int)m, 0xFF00FF00, 0.5f, &hero.unk964->pos, 0x24, 0.0f, -1, 90.0f, 4);
        else if (hero.unk990)
            FUN_L00_001edf60_2bffa8((int)m, 0xFF00FF00, 0.5f, &hero.unk990->pos, 0x24, 0.0f, -1, 90.0f, 4);
    }
    switch (m->state) {
    case 0:
    case 1:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 1) FUN_00212f90_2bffa8(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        if (d->hook) {
            FUN_L00_0024f7c8_2bffa8(m, 0, &d->hook->pos);
            {
                Moby *h = d->hook;
                *(u128 *)&h->rot_row0 = *(u128 *)&m->rot_row0;
                *(u128 *)&h->rot_row1 = *(u128 *)&m->rot_row1;
                *(u128 *)&h->rot_row2 = *(u128 *)&m->rot_row2;
            }
        }
        if (hero.state.current == 0x24 ||
            (hero.state.control_mode == 0xF && hero.unk5B4)) {
            if (hd) {
                d->size = 0.280000001f;
                d->scale = 1.0f;
                d->spin = 0.0f;
                hd->speed = frame_time * 24.0f;
                if (m->prev_seq != 3) FUN_00212f90_2bffa8(m, 3, 0, FUN_001f96f8_2bffa8(10));
                FUN_0022da68_2bffa8(0, 0, m);
                m->state = 2;
            }
        }
        if (hero.state.current == 0x2C && hd) {
            speed = FUN_001f9b48_2bffa8(&hero.unk994->pos, &d->hook->pos);
            n = FUN_001f96f8_2bffa8(7);
            if (hero.items[0].unk20 < FUN_001f96f8_2bffa8(30)) n = FUN_001f96f8_2bffa8(5);
            hd->speed = speed / (float)n;
            d->size = 0.280000001f;
            d->scale = 1.0f;
            d->spin = 0.0f;
            if (m->prev_seq != 3) FUN_00212f90_2bffa8(m, 3, 0, FUN_001f96f8_2bffa8(10));
            FUN_0022da68_2bffa8(0, 0, m);
            m->state = 9;
        }
        break;
    case 2:
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4) FUN_00212f90_2bffa8(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f9a28_2bffa8(&v0, &hero.unk964->pos, &d->hook->pos);
        speed = hd->speed;
        if (hero.state.control_mode == 0xF) {
            speed = speed + hero.unk574;
        } else if (hero.state.prev_control_mode == 4 || hero.state.prev_control_mode == 2) {
            if (2.0f < hero.ground_distance) speed = frame_time * 48.0f;
        }
        qcopy(&v10, &hero.motion.pos);
        v10.f.z += 0.5f;
        if (FUN_001efa68_2bffa8(&v10, &hero.unk964->pos, 2, 0, 0)) {
            if (0.5f < hero.ground_distance) {
                FUN_L00_002223f8_2bffa8(6, 1);
            } else {
                FUN_L00_002223f8_2bffa8(0, 0);
                FUN_L00_002323b8_2bffa8(FUN_L00_00205000_2bffa8(0), 0, (float)FUN_001f96f8_2bffa8(14));
            }
            m->state = 6;
        } else if (speed < FUN_001f9b48_2bffa8(&d->hook->pos, &hero.unk964->pos)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
        } else {
            qcopy(&d->hook->pos, &hero.unk964->pos);
            m->state = 3;
        }
        break;
    case 3:
        FUN_0022da68_2bffa8(1, 0, m);
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        qcopy(&d->hook->pos, &hero.unk964->pos);
        m->state = 4;
        break;
    case 4:
        FUN_L00_0024f7c8_2bffa8(m, 0, &v0);
        qcopy(&v0, &hero.motion.pos);
        v0.f.z += 0.5f;
        if (FUN_001efa68_2bffa8(&v0, &hero.unk964->pos, 2, 0, 0)) {
            if (hero.air_frames.s) FUN_L00_002223f8_2bffa8(6, 1);
            else FUN_L00_002223f8_2bffa8(0, 1);
            m->state = 6;
            break;
        }
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4) FUN_00212f90_2bffa8(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        d->spin = d->spin + 0.69813168f;
        d->size = d->size * (frame_scale * -0.100000024f + 1.0f);
        if (d->size < 0.168000013f) {
            hero.unk5B4 = 0;
            if (d->hook && 3.5f < FUN_001f9b48_2bffa8(&d->hook->pos, &hero.motion.pos)) {
                if (!(controller_state.held & hero.items[0].button_mask)) {
                    hero.unk988 = 1;
                    FUN_L00_002223f8_2bffa8(0, 0);
                    FUN_L00_002323b8_2bffa8(FUN_L00_00205000_2bffa8(0), 0, (float)FUN_001f96f8_2bffa8(14));
                    m->state = 6;
                } else {
                    FUN_0022da68_2bffa8(2, 0, m);
                    if (hero.unk978) FUN_L00_002223f8_2bffa8(0x26, 1);
                    else FUN_L00_002223f8_2bffa8(0x25, 1);
                    m->state = 5;
                }
            } else {
                FUN_L00_002223f8_2bffa8(0, 1);
                m->state = 6;
            }
        }
        break;
    case 5:
        qcopy(&d->hook->pos, &hero.unk964->pos);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4) FUN_00212f90_2bffa8(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        d->size = d->size * (frame_scale * -0.100000024f + 1.0f);
        d->spin = d->spin + 0.69813168f;
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        if ((unsigned int)(hero.state.current - 0x25) >= 2) m->state = 6;
        break;
    case 6:
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        hd->speed = frame_time * 80.0f;
        m->state = 7;
        break;
    case 7:
        FUN_L00_0024f7c8_2bffa8(m, 0, &v10);
        FUN_001f9a28_2bffa8(&v0, &v10, &d->hook->pos);
        if (hd->speed < FUN_001f9b48_2bffa8(&d->hook->pos, &v10)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
            FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        } else {
            qcopy(&d->hook->pos, &v10);
            if (m->prev_seq != 1) FUN_00212f90_2bffa8(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        break;
    case 8:
        break;
    case 9:
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4) FUN_00212f90_2bffa8(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f9a28_2bffa8(&v0, &hero.unk994->pos, &d->hook->pos);
        if (hd->speed < FUN_001f9af0_2bffa8(&v0)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
        } else {
            qcopy(&d->hook->pos, &hero.unk994->pos);
            m->state = 11;
            FUN_0022da68_2bffa8(1, 0, m);
        }
        break;
    case 10:
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        m->state = 11;
        break;
    case 11:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4) FUN_00212f90_2bffa8(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        d->spin = d->spin + 0.69813168f;
        d->size = d->size * (frame_scale * -0.600000024f + 1.0f);
        if (d->size < 0.168000013f) {
            hero.unk99C = 1;
            m->state = 12;
        }
        break;
    case 12:
        if (hero.unk994)
            qcopy(&d->hook->pos, &hero.unk994->pos);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4) FUN_00212f90_2bffa8(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        d->size = d->size * (frame_scale * -0.600000024f + 1.0f);
        d->spin = d->spin + 0.69813168f;
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        if (hero.state.current != 0x2C) m->state = 13;
        break;
    case 13:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4) FUN_00212f90_2bffa8(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        hd->speed = frame_time * 80.0f;
        m->state = 14;
        break;
    case 14:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4) FUN_00212f90_2bffa8(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_L00_0024f7c8_2bffa8(m, 0, &v10);
        FUN_001f9a28_2bffa8(&v0, &v10, &d->hook->pos);
        if (hd->speed < FUN_001f9b48_2bffa8(&d->hook->pos, &v10)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
            FUN_001f4600_2bffa8(FUN_L00_002c5dc8_2bffa8, m);
        } else {
            qcopy(&d->hook->pos, &v10);
            if (m->prev_seq != 1) FUN_00212f90_2bffa8(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        if (hero.state.current == 0x2C && hd) {
            hd->speed = FUN_001f9b48_2bffa8(&hero.unk994->pos, &d->hook->pos) / 10.0f;
            d->scale = 1.0f;
            d->size = 0.0f;
            d->spin = 0.0f;
            if (m->prev_seq != 3) FUN_00212f90_2bffa8(m, 3, 0, FUN_001f96f8_2bffa8(10));
            m->state = 9;
        }
        break;
    case 15:
        break;
    }
}
#else
extern s32 D_001413E4_2c6158 __asm__("D_001413E4");
extern s32 D_0013F4E8_2c6158 __asm__("D_0013F4E8");
extern s32 D_0013FCB8_2c6158 __asm__("D_0013FCB8");
extern u8 *D_0013FCB4_2c6158 __asm__("D_0013FCB4");
extern u8 *D_0013FCE0_2c6158 __asm__("D_0013FCE0");
extern u8 *D_0013FCE4_2c6158 __asm__("D_0013FCE4");
extern s32 D_0013F904_2c6158 __asm__("D_0013F904");
extern s32 D_00140400_2c6158 __asm__("D_00140400");
extern s32 D_0013FCC8_2c6158 __asm__("D_0013FCC8");
extern s32 D_0013FCD8_2c6158 __asm__("D_0013FCD8");
extern s32 D_0013FCEC_2c6158 __asm__("D_0013FCEC");
extern u32 D_0013CAE0_2c6158 __asm__("D_0013CAE0") __attribute__((section(".data")));
extern float D_0015ED6C_2c6158 __asm__("D_0015ED6C");
extern float D_0015ED60_2c6158 __asm__("D_0015ED60");
extern float D_0013F8C4_2c6158 __asm__("D_0013F8C4");
extern float D_0013F62C_2c6158 __asm__("D_0013F62C");
extern void FUN_002223f8_2c6158(s32, s32) __asm__("FUN_L00_002223f8");
extern void FUN_001edf60_2c6158(void *, float, u32, void *, s32, float, s32, float, s32) __asm__("FUN_L00_001edf60");
extern s32 FUN_001fef20_2c6158(s32) __asm__("FUN_001f96f8");
extern void FUN_00257880_2c6158(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void FUN_0028dbe8_2c6158(s32, s32, void *) __asm__("FUN_0022da68");
extern void FUN_001f93e0_2c6158(void *, void *) __asm__("FUN_L00_001f93e0");
extern float FUN_001ff450_2c6158(void *, void *) __asm__("FUN_001f9b48");
extern void FUN_001ff2a8_2c6158(void *, void *, void *) __asm__("FUN_001f9a28");
extern s32 FUN_001efc70_2c6158(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void FUN_001ff500_2c6158(float, void *, void *) __asm__("FUN_001f9bf8");
extern void FUN_001ff278_2c6158(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001ff3d8_2c6158(void *) __asm__("FUN_001f9af0");
extern void *FUN_00205000_2c6158(s32) __asm__("FUN_L00_00205000");
extern void FUN_002323b8_2c6158(float, void *, s32) __asm__("FUN_L00_002323b8");

void FUN_L00_002c6158(u8 *m) {
    u8 *vars;
    u8 *child;
    u8 *motion;
    OvlVec4 a, b;
    float speed, distance;
    s32 frames;
    void *effect;

    if (m == 0) return;
    if (hero.state.current == 1) FUN_002223f8_2c6158(30, 1);
    vars = *(u8 **)(m + 0x78);
    if (vars == 0) return;
    motion = 0;
    if (*(u8 **)(vars + 0x14) == 0) {
        if ((*(u8 **)(vars + 0x14) = FUN_L00_002c6f88(m, D_0015ED6C_2c6158 * 24.0f)) == 0)
        goto child_ready;
    }
    motion = *(u8 **)(*(u8 **)(vars + 0x14) + 0x78);
child_ready:;

    if (hero.state.current == 1 || hero.state.current == 30) {
        if (hero.state_timer >= 2) {
            FUN_001edf60_2c6158(m, 1.0f, 0xff0000ff, 0, 36, 0.0f, -1, 90.0f, 4);
            if (hero.unk968 && ((u8 *)hero.unk964)) {
                FUN_001edf60_2c6158(m, 0.5f, 0xff00ff00, ((u8 *)hero.unk964) + 0x10, 36, 0.0f, -1, 90.0f, 4);
            } else if (((u8 *)hero.unk990)) {
                FUN_001edf60_2c6158(m, 0.5f, 0xff00ff00, ((u8 *)hero.unk990) + 0x10, 36, 0.0f, -1, 90.0f, 4);
            }
        }
    }
    switch (m[0x20]) {
    case 0:
    case 1:
        if (m[0x70] & 2) {
            if (m[0x53] != 1) blend_moby_animation(m, 1, 0, scale_game_frames(10));
            m[0x20] = 1;
        }
        if (*(u8 **)(vars + 0x14) != 0) {
            FUN_L00_0024f7c8(m, 0, *(u8 **)(vars + 0x14) + 0x10);
            child = *(u8 **)(vars + 0x14);
            *(u128 *)(child + 0xc0) = *(u128 *)(m + 0xc0);
            *(u128 *)(child + 0xd0) = *(u128 *)(m + 0xd0);
            *(u128 *)(child + 0xe0) = *(u128 *)(m + 0xe0);
        }
        if ((hero.state.current == 36 || (hero.state.control_mode == 15 && hero.unk5B4)) && motion) {
            *(float *)(vars + 0x18) = 0.0f;
            *(float *)(vars + 0x1c) = 1.0f;
            *(float *)(vars + 0x20) = 0.28f;
            *(float *)(motion + 4) = D_0015ED6C_2c6158 * 24.0f;
            if (m[0x53] != 3) blend_moby_animation(m, 3, 0, scale_game_frames(10));
            FUN_0028dbe8_2c6158(0, 0, m);
            m[0x20] = 2;
        }
        if (hero.state.current == 44) {
            if (motion == 0) return;
            distance = FUN_001ff450_2c6158(((u8 *)hero.unk994) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10);
            frames = scale_game_frames(7);
            if (hero.items[0].unk20 < scale_game_frames(30)) frames = scale_game_frames(5);
            *(float *)(motion + 4) = distance / (float)frames;
            *(float *)(vars + 0x18) = 0.0f;
            *(float *)(vars + 0x1c) = 1.0f;
            *(float *)(vars + 0x20) = 0.28f;
            if (m[0x53] != 3) blend_moby_animation(m, 3, 0, scale_game_frames(10));
            FUN_0028dbe8_2c6158(0, 0, m);
            m[0x20] = 9;
        }
        return;
    case 2:
        FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
        if ((m[0x70] & 2) && m[0x53] != 4) blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001ff2a8_2c6158(&a, ((u8 *)hero.unk964) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10);
        speed = *(float *)(motion + 4);
        if (hero.state.control_mode == 15) speed += hero.unk574;
        else if ((hero.state.prev_control_mode == 4 || hero.state.prev_control_mode == 2) && hero.ground_distance > 2.0f) speed = D_0015ED6C_2c6158 * 48.0f;
        qcopy(&b, &hero.motion.pos);
        b.f[2] += 0.5f;
        if (FUN_001efc70_2c6158(&b, ((u8 *)hero.unk964) + 0x10, 2, 0, 0)) {
            if (hero.ground_distance > 0.5f) FUN_002223f8_2c6158(6, 1);
            else {
                FUN_002223f8_2c6158(0, 0);
                effect = FUN_00205000_2c6158(0);
                frames = scale_game_frames(14);
                FUN_002323b8_2c6158((float)frames, effect, 0);
            }
            m[0x20] = 6;
            return;
        }
        distance = FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.unk964) + 0x10);
        if (speed < distance) {
            FUN_001ff500_2c6158(speed, &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            return;
        }
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.unk964) + 0x10);
        m[0x20] = 3;
        return;
    case 3:
        FUN_0028dbe8_2c6158(1, 0, m);
        FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.unk964) + 0x10);
        m[0x20] = 4;
        return;
    case 4: {
            float factor;
            /* Retail tests ordered comparisons; an unordered fade value returns here. */
            FUN_L00_0024f7c8(m, 0, &a);
            qcopy(&a, &hero.motion.pos);
            a.f[2] += 0.5f;
            if (FUN_001efc70_2c6158(&a, ((u8 *)hero.unk964) + 0x10, 2, 0, 0) != 0) {
                if (hero.air_frames.s != 0) FUN_002223f8_2c6158(6, 1);
                else FUN_002223f8_2c6158(0, 1);
                m[0x20] = 6;
                return;
            }
            if ((m[0x70] & 2) && m[0x53] != 4) blend_moby_animation(m, 4, 0, scale_game_frames(10));
            FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
            factor = *(float *)(vars + 0x20) * (D_0015ED60_2c6158 * -0.100000024f + 1.0f);
            *(float *)(vars + 0x18) += 0.6981317f;
            *(float *)(vars + 0x20) = factor;
            if (!(factor < 0.16800001f)) return;
            hero.unk5B4 = 0;
            if ((*(u8 **)(vars + 0x14)) != 0 && 3.5f < FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &hero.motion.pos)) {
                if ((D_0013CAE0_2c6158 & hero.items[0].button_mask) == 0) {
                    hero.unk988 = 1;
                    FUN_002223f8_2c6158(0, 0);
                    effect = FUN_00205000_2c6158(0);
                    frames = scale_game_frames(14);
                    FUN_002323b8_2c6158((float)frames, effect, 0);
                    m[0x20] = 6;
                } else {
                    FUN_0028dbe8_2c6158(2, 0, m);
                    if (hero.unk978 != 0) FUN_002223f8_2c6158(38, 1);
                    else FUN_002223f8_2c6158(37, 1);
                    m[0x20] = 5;
                }
            } else {
                FUN_002223f8_2c6158(0, 1);
                m[0x20] = 6;
                return;
            }
            return;
        }
    case 5: {
            float fade_scale;
            qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.unk964) + 0x10);
            fade_scale = D_0015ED60_2c6158;
            if ((m[0x70] & 2) && m[0x53] != 4) {
                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                fade_scale = D_0015ED60_2c6158;
            }
            *(float *)(vars + 0x18) += 0.6981317f;
            *(float *)(vars + 0x20) *= fade_scale * -0.100000024f + 1.0f;
            FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
            if ((u32)(hero.state.current - 37) < 2) return;
            m[0x20] = 6;
            return;
        }
    case 6:
        FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
        *(float *)(motion + 4) = D_0015ED6C_2c6158 * 80.0f;
        m[0x20] = 7;
        return;
    case 7:
        FUN_L00_0024f7c8(m, 0, &b);
        FUN_001ff2a8_2c6158(&a, &b, (*(u8 **)(vars + 0x14)) + 0x10);
        distance = FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (*(float *)(motion + 4) < distance) {
            FUN_001ff500_2c6158(*(float *)(motion + 4), &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
            return;
        }
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (m[0x53] != 1) blend_moby_animation(m, 1, 0, scale_game_frames(10));
        m[0x20] = 1;
        return;
    case 9: {
            float step;
            FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
            if ((m[0x70] & 2) && m[0x53] != 4) blend_moby_animation(m, 4, 0, scale_game_frames(10));
            FUN_001ff2a8_2c6158(&a, ((u8 *)hero.unk994) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10);
            distance = FUN_001ff3d8_2c6158(&a);
            step = *(float *)(motion + 4);
            if (step < distance) {
                FUN_001ff500_2c6158(step, &a, &a);
                FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
                return;
            }
            qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.unk994) + 0x10);
            m[0x20] = 11;
            FUN_0028dbe8_2c6158(1, 0, m);
            return;
        }
    case 10:
        FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
        m[0x20] = 11;
        return;
    case 11: {
            float factor;
            if ((m[0x70] & 2) && m[0x53] != 4) blend_moby_animation(m, 4, 0, scale_game_frames(10));
            FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
            factor = *(float *)(vars + 0x20) * (D_0015ED60_2c6158 * -0.6f + 1.0f);
            *(float *)(vars + 0x18) += 0.6981317f;
            *(float *)(vars + 0x20) = factor;
            if (!(factor < 0.16800001f)) return;
            hero.unk99C = 1;
            m[0x20] = 12;
            return;
        }
    case 12: {
            float fade_scale;
            if (((u8 *)hero.unk994)) qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.unk994) + 0x10);
            fade_scale = D_0015ED60_2c6158;
            if ((m[0x70] & 2) && m[0x53] != 4) {
                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                fade_scale = D_0015ED60_2c6158;
            }
            *(float *)(vars + 0x18) += 0.6981317f;
            *(float *)(vars + 0x20) *= fade_scale * -0.6f + 1.0f;
            FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
            if (hero.state.current != 44) m[0x20] = 13;
            return;
        }
    case 13:
        if ((m[0x70] & 2) && m[0x53] != 4) blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
        *(float *)(motion + 4) = D_0015ED6C_2c6158 * 80.0f;
        m[0x20] = 14;
        return;
    case 14:
        if ((m[0x70] & 2) && m[0x53] != 4) blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_L00_0024f7c8(m, 0, &b);
        FUN_001ff2a8_2c6158(&a, &b, (*(u8 **)(vars + 0x14)) + 0x10);
        distance = FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (*(float *)(motion + 4) < distance) {
            FUN_001ff500_2c6158(*(float *)(motion + 4), &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            FUN_001f93e0_2c6158(FUN_L00_002c5dc8, m);
        } else {
            qcopy((*(u8 **)(vars + 0x14)) + 0x10, &b);
            if (m[0x53] != 1) blend_moby_animation(m, 1, 0, scale_game_frames(10));
            m[0x20] = 1;
        }
        if (hero.state.current != 44 || motion == 0) return;
        distance = FUN_001ff450_2c6158(((u8 *)hero.unk994) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10);
        *(float *)(motion + 4) = distance / 10.0f;
        *(float *)(vars + 0x1c) = 1.0f;
        *(float *)(vars + 0x18) = 0.0f;
        *(float *)(vars + 0x20) = 0.0f;
        if (m[0x53] != 3) blend_moby_animation(m, 3, 0, scale_game_frames(10));
        m[0x20] = 9;
        return;
    case 15:
    default:
        return;
    }
}
#endif
/* Vars of the carrier moby that picks up and holds another moby. */
typedef struct {
    u8 pad0[0x40];
    OvlVec4 grip;           /* 0x40: where the held moby is carried */
    struct Moby *held;      /* 0x50 */
    s32 timer;              /* 0x54 */
} CarrierVars;

#include "rnc/input/pad_state.h"

typedef float Vec_2c7548[4] __attribute__((aligned(16)));
typedef float Mat_2c7548[4][4] __attribute__((aligned(16)));
typedef float Mat3_2c7548[3][4] __attribute__((aligned(16)));

extern char camera_2c7548[] __asm__("D_L00_00166C80");
extern char camera_pos_2c7548[] __asm__("D_L00_00166DC0");
extern CollisionHit collision_hit_2c7548 __asm__("D_L00_00173E40");
extern float frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern void setup_matrix_2c7548(void *, void *) __asm__("FUN_001fa2b8");
extern void matrix_from_basis_rows_2c7548(void *, void *) __asm__("FUN_001fa2d8");
extern void adjust_direction_2c7548(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float atan2_2c7548(float, float) __asm__("FUN_001f9e90");
extern float add_rotations_2c7548(float, float) __asm__("FUN_001fa580");
extern float subtract_rotations_2c7548(float, float) __asm__("FUN_001fa5c8");
extern float cos_2c7548(float) __asm__("FUN_001f9dc8");
extern float sin_2c7548(float) __asm__("FUN_001f9de0");
extern float sqrt_2c7548(float) __asm__("FUN_001f9988");
extern void add_vector_2c7548(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_2c7548(void *, void *, float) __asm__("FUN_001f9a68");
extern void rotate_vector_2c7548(void *, void *, void *) __asm__("FUN_001f9d20");
extern float vector_length_xy_2c7548(void *) __asm__("FUN_001f9b20");
extern void build_spherical_offset_2c7548(void *, float, float, float) __asm__("FUN_00214db0");
extern int collision_line_2c7548(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void FUN_L00_002c7320_2c7548(float *pos, float *tgt, float *out, float speed, float unused, float range) __asm__("FUN_L00_002c7320");

/* Moves the moby the carrier holds toward its throw target: ahead of the hero,
   or along the camera aim (and up to a collision hit) while pad bits 0x5 are held. */
void FUN_L00_002c7548(struct Moby *moby, CarrierVars *vars, OvlVec4 *grip)
{
    Vec_2c7548 v0;
    Mat3_2c7548 basis;
    Mat_2c7548 matrix;
    Vec_2c7548 direction, target, arc, saved, probe;
    struct Moby *held;
    float *out;
    float horizontal, radius, angle, vertical, slope, distance, root;
    int hit;
    float speed;
    float *pos = v0;

    *(u128 *)pos = grip->q;
    setup_matrix_2c7548(basis, camera_2c7548);
    matrix_from_basis_rows_2c7548(matrix, basis);
    held = vars->held;
    if (held == 0) return;
    out = (float *)held->pvars;
    *(u128 *)direction = 0;
    direction[0] = 1.0f;
    adjust_direction_2c7548(direction, direction, hero.basis);
    {
        struct Hero *h = &hero;
        pos[0] = cos_2c7548(add_rotations_2c7548(-0.361968011f, atan2_2c7548(h->unk670, h->unk674))) * 0.859039009f;
        pos[1] = sin_2c7548(add_rotations_2c7548(-0.361968011f, atan2_2c7548(h->unk670, h->unk674))) * 0.859039009f;
        pos[2] = 0.0f;
        add_vector_2c7548(pos, pos, &hero.motion.pos);
        pos[2] = pos[2] + 0.390819997f;
        atan2_2c7548(direction[0], direction[1]);
        speed = (controller_state.held & 5) ? 3.5f : 2.5f;
        scale_vector_2c7548(target, direction, speed);
        add_vector_2c7548(target, target, pos);
        if ((controller_state.held & 5) && h->unk20AC == 0) {
            *(u128 *)arc = 0;
            arc[2] = 1.0f;
            rotate_vector_2c7548(arc, arc, matrix);
            hit = 0;
            subtract_rotations_2c7548(h->moby->rot.z, atan2_2c7548(arc[0], arc[1]));
            angle = atan2_2c7548(arc[0], arc[1]);
            slope = atan2_2c7548(vector_length_xy_2c7548(arc), arc[2]);
            if (0.593411922f < slope) slope = 0.593411922f;
            if (slope < -1.39626336f) slope = -1.39626336f;
            build_spherical_offset_2c7548(arc, (controller_state.held & 5) ? 3.5f : 2.5f, angle, slope);
            qcopy(saved, camera_2c7548 + 0x140);
            distance = vector_length_xy_2c7548(arc);
            {
                float z = arc[2];
                if (distance != 0.0f) {
                    horizontal = z / distance;
                } else {
                    horizontal = z / 0.00999999978f;
                }
            }
            root = sqrt_2c7548(frame_time_sq_macro * 9.0f * ((controller_state.held & 5) ? 3.5f : 2.5f) * 0.5f);
            radius = (root + root) * root * (1.0f - horizontal) / (frame_time_sq_macro * 9.0f);
            vertical = horizontal * radius;
            if (0.0f < radius) {
                arc[0] = cos_2c7548(angle) * radius;
                arc[1] = sin_2c7548(angle) * radius;
                arc[2] = vertical;
                add_vector_2c7548(probe, camera_pos_2c7548, arc);
                if (collision_line_2c7548(camera_pos_2c7548, probe, 2, (int)moby, 0)
                    && collision_hit_2c7548.moby != (s32)hero.moby) {
                    qcopy(target, collision_hit_2c7548.point.f);
                    hit = 1;
                    angle = atan2_2c7548(target[0] - pos[0], target[1] - pos[1]);
                }
                if (hit == 0) {
                    horizontal = 5.0f;
                    target[0] = cos_2c7548(angle) * radius * horizontal;
                    target[1] = sin_2c7548(angle) * radius * horizontal;
                    target[2] = vertical * horizontal;
                    add_vector_2c7548(target, target, camera_pos_2c7548);
                }
            }
        } else {
            hero.unkA50 = 0;
        }
    }
    FUN_L00_002c7320_2c7548(pos, target, out, frame_time_sq_macro * 9.0f, 0.00000999999975f,
                             (controller_state.held & 5) ? 3.5f : 2.5f);
}
/* Counters kept at D_00141680 for the carrier's deliveries. */
typedef struct {
    u8 pad0[0xA0];
    u16 deliveries;         /* 0xA0: counts up to 0xFFFF */
    u16 unkA2;              /* 0xA2: raised to scale_game_frames(D_0015EEA4) / 600 */
    u32 levels;             /* 0xA4: bit per level index, plus 0x80000000 */
} DeliveryStats;

extern DeliveryStats D_00141680;
extern u32 D_0013CAE4 __attribute__((section(".data")));
extern s32 D_0015EEA4;
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern int FUN_L00_00216de8(int, int);
extern int use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern int get_ammo(int) __asm__("FUN_L00_00233e98");
extern void FUN_L00_002c7548(struct Moby *, CarrierVars *, OvlVec4 *);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

/* Carrier: keeps its grip point in front of it, hands the held moby over when the hero asks, and
   picks a new one up when get_ammo allows. */
void FUN_L00_002c7a58(struct Moby *moby) {
    OvlVec4 pos;
    OvlVec4 offset;
    OvlVec4 local;
    OvlVec4 mat[4];
    OvlVec4 grip;
    CarrierVars *vars = (CarrierVars *)moby->pvars;

    offset.q = 0;
    offset.f[0] = 0.01f;
    offset.f[1] = -0.14f;
    offset.f[2] = -0.05f;
    moby_point_position((u8 *)moby, 0, &pos);
    build_moby_bone_transform(moby, 0, mat);
    transform_vector(&local, &offset, mat);
    add_vector_xyz(&vars->grip, &pos, &local);
    if (vars->held != 0) {
        vars->held->collision_disable = 1;
    }
    if (vars->held == 0 || vars->held->state == 0xFE || vars->held->state == 0xFD) {
        vars->held = 0;
    }
    if (hero.state.current == HERO_STATE_LOOK) {
        hero_set_state(0x1E, 1);
    }
    if (tick_countdown(&vars->timer) && !hero.unk20AC) {
        if ((hero.unk20A8 && hero.unk1BC == scale_game_frames(0x11)) ||
            ((D_0013CAE4 & hero.items[0].button_mask) && hero.state.current == 0x1E && get_ammo(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            FUN_L00_00216de8(0x1A, 0);
            use_ammo(-1, 1);
            moby->state = 3;
        }
    }
    grip.q = vars->grip.q;
    FUN_L00_002c7548(moby, vars, &grip);
    switch (moby->state) {
    case 0:
        vars->held = 0;
        if (moby->anim_flags & 2) {
            moby->state = 2;
        } else {
            moby->state = 1;
        }
    case 1:
        if (moby->anim_flags & 2) {
            moby->state = 2;
        }
        break;
    case 2:
        break;
    case 3:
        if (vars->held != 0) {
            if (D_00141680.deliveries < 0xFFFF) {
                D_00141680.deliveries++;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > D_00141680.unkA2) {
                D_00141680.unkA2 = scale_game_frames(D_0015EEA4) / 600;
            }
            D_00141680.levels = D_00141680.levels | (1 << current_level_index) | 0x80000000;
            FUN_L00_002c82f0((void *)&vars->grip, (u8 *)vars->held, vars->held->pvars);
            vars->held = 0;
        } else {
            allocate_voice_for_target_entry(0, 0, moby);
        }
        moby->state = 4;
        vars->timer = scale_game_frames(0x14);
        break;
    case 4:
        if (hero.state.current != 0x23 && !hero.unk20A8) {
            moby->state = 2;
        }
        break;
    case 5:
        moby->state = 6;
        break;
    case 6:
        return;
    }
    if (vars->held != 0) {
        qcopy(&vars->held->pos, &vars->grip);
    } else if (get_ammo(-1)) {
        qzero(&grip);
        vars->held = (struct Moby *)FUN_L00_002c8218((int)moby, (void *)&vars->grip, (void *)&grip);
    }
}
