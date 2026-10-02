#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022eaa8/FUN_0022eaa8.s", FUN_0022eaa8);
#else
#include "types.h"
#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"

typedef union { u128 q; f32 f[4]; } Vec4;
typedef struct { Vec4 v[4]; } Mat4;

typedef struct {
    u8 pad00[0xC];
    u8 count;          /* 0x0C */
    u8 pad0D[0x24 - 0x0D];
    f32 scale;         /* 0x24 */
    u8 pad28[0x48 - 0x28];
    s32 frames[1];     /* 0x48 */
} Model;

typedef struct {
    u8 pad00[0x10];
    Vec4 pos;          /* 0x10 */
    Model *model;      /* 0x20 */
} ObjHead;

typedef struct {
    u8 pad00[0x10];
    u8 pos[0x14];      /* 0x10 */
    Model *model;      /* 0x24 */
    u8 pad28[4];
    f32 fade;          /* 0x2C */
    u8 pad30[2];
    s16 unk32;         /* 0x32 */
    u16 flags;         /* 0x34 */
    u8 pad36[0x50 - 0x36];
    u8 unk50;          /* 0x50 */
    u8 unk51;
    u8 unk52;
    u8 unk53;
    f32 blend;         /* 0x54 */
    u8 pad58[0x68 - 0x58];
    u8 *unk68;         /* 0x68 */
    u8 *unk6C;         /* 0x6C */
    u8 pad70;
    u8 unk71;
    u8 unk72;
    u8 pad73[5];
    u8 *verts;         /* 0x78 */
} Obj;

typedef struct {
    u8 pad00[0x34];
    s32 time;          /* 0x34 */
    s32 frame;         /* 0x38 */
    s32 unk3C;         /* 0x3C */
    s16 end;           /* 0x40 */
    u8 pad42[2];
    s16 count;         /* 0x44 */
    u8 pad46[0x58 - 0x46];
    s32 unk58;         /* 0x58 */
    s32 unk5C;         /* 0x5C */
    s32 words[0x46];   /* 0x60 */
    Obj *objs[1];      /* 0x178 */
} Transfer;

typedef struct {
    u8 pad00[0x50];
    s32 idx;           /* 0x50 */
    s32 len;           /* 0x54 */
    s32 mode;          /* 0x58 */
    s32 state;         /* 0x5C */
    u8 pad60[0xC0 - 0x60];
    Vec4 trailA[32];   /* 0xC0 */
    Vec4 trailB[32];   /* 0x2C0 */
} Scene;

typedef struct {
    u8 pad00[0xB0];
    f32 unkB0;
} Display;

typedef struct {
    s32 off;
    s32 flag;
} Entry;

typedef struct {
    u8 pad00[4];
    s32 base;          /* 0x04 */
    u8 pad08[0x50 - 0x08];
    s32 tbl[5];        /* 0x50 */
} Bank;

typedef struct {
    u8 pad00[4];
    s32 unk4;
    s32 unk8;
    u8 pad0C[8];
    Bank *bank;        /* 0x14 */
} Level;

typedef struct {
    u32 w0;
    u32 w1;
} Word2;

extern u8 D_0013DD40[];
extern u8 D_0013DD43[];
extern Scene D_0013E030;
extern u8 D_0013E5C0[];
extern s32 D_0015ED5C;
extern s32 D_0015ED84;
extern s16 D_0015EE48 __attribute__((sda));
#define D_0015EE4A (*(s16 *)0x0015EE4A)
extern f32 D_0015F43C MACRO_ADDR;
extern s32 D_0015F618 MACRO_ADDR;
extern f32 D_00160404 MACRO_ADDR;
extern u8 D_00160460[] MACRO_ADDR;
extern s32 D_001604E0 __attribute__((sda));
extern s32 D_001604F0 __attribute__((sda));
extern s32 D_00160500 __attribute__((sda));
extern s32 D_00160F0C;
extern Transfer D_0018CB20;
extern Transfer D_0018CB20_b[] __asm__("D_0018CB20");
extern Transfer D_0018CB20_c[] __asm__("D_0018CB20");
extern Obj *D_0018CC98[];
extern Display D_0018CD00;
extern Level D_001940C0;
extern Word2 D_001D9A30[];
extern u128 D_001D9AE0[];
extern f32 D_001D9B30[];
extern f32 D_001D9B48[];

extern void FillTransferWords(void *, s32, s32);
extern void ReadGlobalTableEntry(void);
extern void func_0012DC80(void);
extern void func_0012E308(s32, s32, s32, s32, s32, s32, s32, void *);
extern void func_0012EB00(void);
extern void func_001E9428(void);
extern void func_001E9430(void);
extern void func_001F2D98(void);
extern s32 func_001F96F8(s32);
extern u32 func_001F98D0(void *, void *, s32);
extern void func_001F9A10(void *, void *, void *);
extern void func_001F9A68(void *, void *, f32);
extern f32 func_001FA6C0(s32);
extern void func_002049F0(s32) __asm__("FUN_002049f0");
extern void func_0020C828(Obj *);
extern void func_0020C880(Obj *, s32);
extern void func_0020CCA8(Obj *, s32, void *);
extern void func_0020DEF8(Obj *);
extern void func_0022DE10(void);
extern s32 rand(void);

void FUN_0022eaa8(void) {
    Vec4 a;
    Vec4 b;
    Mat4 m1;
    Mat4 m2;
    Transfer *t;
    Scene *s;
    u16 two;
    Obj *obj;
    Obj *o;
    s32 r;
    Model *mdl;
    Bank *bank;
    u8 *base;
    s32 *tp;
    Entry *e;
    s32 i;
    s32 i2;
    s32 i3;
    s32 add;
    u8 *fb;
    f32 g;
    s32 j;
    s32 k;
    s32 n;
    s32 q;
    s32 hp;
    s32 h;
    s32 frame;
    u8 *v;
    s32 lo;
    s32 hi;
    f32 f;
    f32 scale;

    func_001E9430();
    D_0015F43C -= 0.25f;
    D_0018CB20.frame++;
    D_0018CB20.time++;
    if (D_0015F43C < 0.0f) {
        D_0015F43C = 0.0f;
    }
    if (D_0018CB20.time == 1) {
        if (D_0015ED84 != 0 && (D_0015ED84 != 1 || D_0013DD43[0] != 0)) {
            ReadGlobalTableEntry();
            func_0012E308(D_0015ED5C, D_0013E030.mode, 0x400, 0, 0, 0, 0, D_0013E5C0);
            func_0012EB00();
            func_0012DC80();
        }
    }
    if (D_0018CB20.time >= D_0018CB20.end) {
        for (i2 = 0; i2 < D_0018CB20.count; i2++) {
            o = D_0018CB20.objs[i2];
            if (o != 0) {
                o->model->count--;
                o->model->frames[o->model->count] = 0;
                func_0020C828(o);
            }
        }
        if (D_0015ED84 != 0 && (D_0015ED84 != 1 || D_0013DD43[0] != 0) && D_0015EE48 < 3) {
            D_0013E030.state = 0;
        }
        if (D_0013E030.state < 2) {
            if (D_0013E030.state == 0) {
                r = (rand() >> 16) % 3 + 1;
                D_0013E030.mode = (D_0013E030.mode + r) & 3;
            } else {
                D_0013E030.mode = 4;
            }
            D_0013E030.idx = 0;
            D_0013E030.len = 0;
            D_0013E030.state++;
            qcopy(&D_001604F0, &D_001604E0);
            FillTransferWords(&D_0018CB20, 0, 0x1C0);
            D_0018CB20.unk58 = D_001940C0.unk4 + D_00160F0C;
            D_0018CB20.unk5C = D_001940C0.unk8 + D_00160F0C;
            bank = D_001940C0.bank;
            tp = &bank->tbl[0];
            tp += D_0013E030.mode;
            base = (u8 *)bank + bank->base;
            e = (Entry *)(base + *tp);
            for (i3 = 0; i3 < 0x46 && e->flag != 0; i3++, e++) {
                add = 0x800;
                D_0018CB20.words[i3] = (s32)(*tp + base) + (e->off + add);
            }
            func_002049F0(0);
        } else {
            if (D_0015EE4A != 0) {
                D_0015EE4A = 0;
            }
            D_0015F618 = 1;
            return;
        }
    } else if (D_0018CB20.frame >= 0x60) {
        func_002049F0(++D_0018CB20.unk3C);
    }
    func_0022DE10();
    if (D_0018CD00.unkB0 < D_001D9B48[D_0013E030.mode]) {
        D_0018CD00.unkB0 = D_001D9B48[D_0013E030.mode];
    }
    func_001F2D98();
    scale = 1.0f;
    if (D_0013E030.mode == 4) {
        scale = (f32)(D_0018CB20.end - D_0018CB20.time) / (f32)D_0018CB20.end;
        func_001F9A68(&a, &D_00160500, scale);
        func_001F9A10(&D_001604F0, &D_001604F0, &a);
    }
    func_001F9A68(D_00160460, &D_001D9AE0[D_0013E030.mode],
                  (f32)(D_0018CB20.time - func_001F96F8(0x78)) * 20.0f * scale);
    D_00160404 = D_001D9B30[D_0013E030.mode];
    for (i = 0; i < D_0018CB20.count; i++) {
        obj = D_0018CC98[i];
        for (j = 0; j < 2; j++) {
            frame = D_0018CB20.frame;
            lo = frame >> 1;
            hi = lo + 1;
            mdl = obj->model;
            fb = (u8 *)mdl->frames[mdl->count - 1] + 0x1C;
            h = *(s32 *)(fb + lo * 4) + 0x10;
            hp = *(s32 *)(fb + hi * 4) + 0x10;
            f = func_001FA6C0(frame & 1) * 0.5f + (f32)j * 0.25f;
            obj->blend = f;
            v = obj->verts;
            func_001F9A68(&a, v + lo * 16, 1.0f - f);
            func_001F9A68(&b, v + hi * 16, obj->blend);
            two = 2;
            func_001F9A10(obj->pos, &a, &b);
            obj->unk52 = 2;
            obj->unk53 = two;
            obj->unk50 = 0;
            obj->unk51 = 0;
            func_0020C880(obj, two);
            func_001F98D0(obj->unk68 + 0x10, (void *)h, 0x20);
            func_001F98D0(obj->unk6C + 0x10, (void *)hp, 0x20);
            obj->unk32 = 0x1FF;
            obj->unk72 = 0xFF;
            obj->unk71 = 0xFF;
            func_0020DEF8(obj);
            obj->unk52 = obj->model->count - 1;
            obj->unk53 = obj->model->count - 1;
            func_0020CCA8(obj, 1, &m1);
            func_0020CCA8(obj, 2, &m2);
            D_0013E030.idx = (D_0013E030.idx + 1) & 0x1F;
            if (D_0013E030.len < 0x20) {
                D_0013E030.len++;
            }
            q = D_0013E030.idx;
            qcopy(&D_0013E030.trailA[q], &m1.v[3]);
            qcopy(&D_0013E030.trailB[q], &m2.v[3]);
            if (D_0013E030.mode == 4) {
                if (D_0015ED84 == 0 || (D_0015ED84 == 1 && D_0013DD40[3] == 0)) {
                    obj->flags |= 1;
                    D_0013E030.len = 0;
                }
                if (D_0018CB20.time > D_0018CB20.end - 0x38) {
                    obj->fade = obj->model->scale * ((f32)(D_0018CB20.end - D_0018CB20.time) * 0.017857144f);
                    for (n = 0; n < 3; n++) {
                        D_001D9A30[n].w0 = (D_001D9A30[n].w0 & 0xFFFFFF)
                            | ((D_0018CB20_b[0].end - D_0018CB20_c[0].time) << 24);
                    }
                } else {
                    for (n = 0; n < 3; n++) {
                        D_001D9A30[n].w0 = (D_001D9A30[n].w0 & 0xFFFFFF) | 0x38000000;
                    }
                }
            }
        }
    }
    func_001E9428();
}
#endif /* NON_MATCHING */
