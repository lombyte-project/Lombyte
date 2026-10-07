#include "types.h"
typedef unsigned int u128_25a478 __attribute__((mode(TI), aligned(16)));
typedef union { u128_25a478 q; f32 f[4]; } V_25a478;
typedef struct { u8 pad0[0xA6]; s16 hA6; } T_25a478;
typedef struct {
    u8 pad0[0x10];
    union { V_25a478 pos; struct { f32 x, y, z, f1C; } s; } u;
    T_25a478 *tgt;
    u8 pad24[4];
    u8 state;
    u8 sub;
    u16 kind;
    f32 f2C;
} P_25a478;
typedef struct {
    u8 pad0[8];
    u8 b8;
    u8 pad9[0x14 - 9];
    s16 h14;
    s16 h16;
    u8 pad18[4];
    s16 h1C;
    u8 pad1e[0x2E - 0x1E];
    u8 b2E;
} Q_25a478;
typedef struct { u8 pad0[0x10]; f32 x, y; } O_25a478;
typedef struct { u8 pad0[0x80]; f32 x, y; } G_25a478;
extern G_25a478 G_25a478v __asm__("D_0013F350");
extern u8 D_0013E533[];
extern f32 D_0015FFC8[2] __attribute__((sda));
extern f32 D_001CC0C8[][8];
extern u8 D_001CC080[][4];
extern Q_25a478 *func_002141F8(void *);
extern Q_25a478 *ReadStateField(void *);
extern s32 func_001F9770(s16 *);
extern s32 func_001F96F8(s32);
extern f32 func_001F9E90(f32, f32);
extern f32 func_001F9DC8(f32);
extern f32 func_001F9DE0(f32);
extern f32 FUN_001f9af0(void *);
extern void FUN_001f9a68(void *, void *, f32);
extern void FUN_001f9bf8(void *, void *, f32);
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_001f9c48(void *, void *, f32);

s32 FUN_00213928(O_25a478 *obj, P_25a478 *p, Q_25a478 *q, u32 flags, s32 *out, f32 *fp, s32 *kp, s32 idx) {
    V_25a478 v;
    V_25a478 t;
    s32 r;
    s32 kind;
    s32 res;
    Q_25a478 *o;

    *out = 0;
    if (kp) *kp = -1;
    if (p && p->tgt && (p->tgt->hA6 == 0x131 || p->tgt->hA6 == 0xA8) && q) {
        p->f2C = D_0015FFC8[q->b8];
    }
    if (!obj) {
        if (q) {
            q->h1C = 0;
            q->h14 = -1;
        }
        q->h16 = -1;
        *out = 0;
        return 0;
    }
    if (!q) {
        q = func_002141F8(obj);
        if (!q) {
            *out = 0;
            return 0;
        }
    }
    if (idx == 4) idx = q->b8;
    if (!p) {
        if (q) {
            func_001F9770(&q->h1C);
            q->h16 = -1;
        }
        *out = 1;
        if (fp) *fp = 0;
        return 0xB;
    }
    if (p->u.s.f1C == 5627.9248f) {
        V_25a478 *pv = &p->u.pos;
        FUN_001f9a68(pv, pv, D_001CC0C8[idx][p->state]);
        kind = p->kind;
        p->u.s.f1C = 5627.9248f;
        if (kind == 0x47) {
            v.f[0] = func_001F9DC8(func_001F9E90(obj->x - G_25a478v.x, obj->y - G_25a478v.y));
            v.f[1] = func_001F9DE0(func_001F9E90(obj->x - G_25a478v.x, obj->y - G_25a478v.y));
            v.f[2] = 1.0f;
            FUN_001f9bf8(&v, &v, FUN_001f9af0(pv) * 0.7f);
            FUN_001f9a68(pv, pv, 0.3f);
            FUN_001f9a10(&t, pv, &v);
            pv->q = t.q;
            FUN_001f9c48(pv, pv, 1.0f);
        }
    } else {
        if (!p->tgt) { p->state = 8; kind = 0x47; }
        else { kind = p->tgt->hA6; p->state = 8; }
    }
    switch (p->state) {
    case 0:
        if (p->kind == 1 || p->kind == 2) { *out = 2; r = 0; break; }
        goto dflt;
    case 1:
        if (p->sub < 2) { *out = 2; r = 0xB; break; }
        if (p->sub == 2) { *out = 2; r = 5; break; }
        *out = 2; r = 9;
        break;
    case 2:
        if (p->sub < 2) { *out = 2; r = 8; break; }
        *out = 2; r = 0xD;
        break;
    case 3:
        if (p->sub < 2) { *out = 2; r = 0xA; break; }
        if (p->sub < 3) { *out = 2; r = 0xF; break; }
        *out = 2; r = 0x10; q->h14 = -1; q->h1C = 0;
        break;
    case 4:
        *out = 2; r = 0xE;
        break;
    case 5:
        if (p->sub < 2) { *out = 3; r = 4; break; }
        *out = 4; r = 7;
        break;
    case 7:
        if (p->sub < 2) { *out = 2; r = 0xC; break; }
        *out = 2; r = 0x11;
        break;
    default:
    dflt:
        *out = 2; r = 1;
        break;
    }
    if (kp) *kp = kind;
    res = D_001CC080[r][q->b8];
    if (q->h14 != kind) q->h1C = 0;
    if (func_001F9770(&q->h1C)) {
        switch (r) {
        case 0: q->h1C = func_001F96F8(0x25); break;
        case 1: q->h1C = func_001F96F8(0xF); break;
        case 4: q->h1C = func_001F96F8(0x3C); break;
        case 7:
            if (D_0013E533[0]) q->h1C = func_001F96F8(0x2D);
            else q->h1C = func_001F96F8(0x3C);
            break;
        case 11: q->h1C = func_001F96F8(0x3C); break;
        default: q->h1C = func_001F96F8(0x1E); break;
        }
    } else {
        switch (r) {
        case 0:
        case 1:
            if (!(flags & 0x10)) res = 0xB;
            if (!(flags & 0x20)) *out = 1;
            goto z;
        case 4:
            if (!(flags & 1)) res = 0xB;
            if (!(flags & 2)) *out = 1;
            goto z;
        case 7:
            if (!(flags & 4)) res = 0xB;
            if (!(flags & 8)) *out = 1;
        z:
            p->f2C = 0;
            break;
        case 11:
            res = 0xC;
            break;
        default:
            p->f2C = 0;
            res = 0xD;
            break;
        }
    }
    q->h14 = kind;
    q->h16 = kind;
    if (fp) *fp = p->f2C;
    if (r == 4) {
        o = ReadStateField(obj);
        if (o) o->b2E = 1;
    } else {
        Q_25a478 *o2 = ReadStateField(obj);
        if (o2) o2->b2E = 0;
    }
    return res;
}

extern __typeof__(FUN_00213928) func_00213928 __attribute__((alias("FUN_00213928")));
