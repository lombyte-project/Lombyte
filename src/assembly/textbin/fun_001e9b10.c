#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001e9b10/FUN_001e9b10.s", FUN_001e9b10);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef union {
    u128 q;
    f32 f[4];
    s32 i[4];
} Vec4;

typedef struct Shrub {
    f32 v[3];
    f32 scale;          /* 0x0C */
    struct ShrubData *data; /* 0x10 */
    f32 x14;            /* 0x14 */
    s16 x18;            /* 0x18 */
    u8 cls;             /* 0x1A */
    u8 x1B;             /* 0x1B */
    u16 x1C;            /* 0x1C */
    u16 x1E;            /* 0x1E */
} Shrub;

typedef struct Tie {
    f32 v[3];
    f32 scale;          /* 0x0C */
    f32 radius;         /* 0x10 */
    u8 pad14[3];
    u8 x17;             /* 0x17 */
    s16 x18;            /* 0x18 */
    u8 cls;             /* 0x1A */
    u8 x1B;             /* 0x1B */
    u16 x1C;            /* 0x1C */
    u16 x1E;            /* 0x1E */
} Tie;

typedef struct FRow {
    f32 v[3];
    f32 w;
} FRow;

typedef struct IRow {
    f32 v[3];
    s32 w;
} IRow;

typedef struct ShrubData {
    FRow m[4];
    u8 pad40[0x100];
    u8 x140[0x80];
} ShrubData;

typedef struct TieData {
    IRow m[3];
    FRow m3;
} TieData;

typedef struct ShrubClass {
    u8 pad00[0x26];
    u16 count;          /* 0x26 */
    Shrub *first;       /* 0x28 */
    u8 pad2C[4];
    f32 x30[3];         /* 0x30 */
    f32 scale;          /* 0x3C */
    f32 x40;            /* 0x40 */
} ShrubClass;

typedef struct TieClass {
    u8 pad00[0xC];
    f32 scale;          /* 0x0C */
    u8 pad10[6];
    u16 count;          /* 0x16 */
    Tie *first;         /* 0x18 */
    f32 *x1C;           /* 0x1C */
    f32 x20;            /* 0x20 */
} TieClass;

typedef struct LevelHeader {
    s32 x0;
    s32 x4;
    u8 pad08[0x2C];
    s32 x34;
    u8 pad38[4];
    s32 x3C;
} LevelHeader;

typedef struct Ent100 {
    u8 pad00[0x20];
    u8 x20;
    u8 pad21[0x15];
    s16 x36;
    u8 pad38[0xC8];
} Ent100;

typedef struct Ent40 {
    u8 pad00[0x36];
    u16 x36;
    u8 pad38[2];
    s16 x3A;
    u8 pad3C[4];
} Ent40;

typedef struct Display {
    u8 pad00[0x23C];
    s32 x23C;
    s32 x240;
    s32 x244;
} Display;

typedef struct Buf194100 {
    s32 x0;
    u8 *x4;
    u8 *x8;
    s32 xC;
    s32 x10;
    s32 x14;
} Buf194100;

typedef struct S18C318 {
    u8 pad00[0x1C];
    s32 x1C;
} S18C318;

typedef struct S160AB0 {
    s32 x0;
    u8 *x4;
} S160AB0;

extern u8 D_00100AE0[];
extern u8 D_0013F350[];
extern s32 D_0015ED80;
extern f32 D_0015ED60;
extern f32 D_0015F43C;
extern u8 D_0015F484;
extern u8 D_0015F485;
extern u8 D_0015F486;
extern f32 D_0015F488;
extern f32 D_0015F48C;
extern f32 D_0015F490;
extern f32 D_0015F494;
extern s32 D_0015F60C;
extern Ent100 *D_0015FF18;
extern Ent100 *D_0015FF1C;
extern Ent100 *D_0015FF20;
extern u8 *D_0015FF28;
extern s32 D_0015FF30;
extern u8 *D_001600AC;
extern s32 D_001600B0;
extern s32 D_001600B4;
extern s32 D_001600B8;
extern s32 D_001600BC;
extern u8 D_0016034C;
extern s32 D_001603D0;
extern Tie *D_001603D4;
extern Tie *D_001603D8;
extern TieData *D_001603DC;
extern u8 *D_001603E0;
extern f32 D_001603E4;
extern S160AB0 D_00160AB0;
extern Ent40 *D_00160E8C __attribute__((sda));
extern s32 D_00160E90;
extern f32 D_00160EC0;
extern Shrub *D_00160F50;
extern Shrub *D_00160F54;
extern ShrubData *D_00160F58;
extern s32 D_00160F5C;
extern f32 D_00160F70;
extern u8 D_00186F40[];
extern S18C318 D_0018C318;
extern Display D_0018CD00;
extern u8 *D_001940D8;
extern Buf194100 D_00194100;
extern u8 D_00194180[];
extern u8 D_001941C0[];
extern u8 D_0019BDC0[];
extern u8 D_0019C1C0[];
extern u8 D_0019C3C0[];
extern u8 D_001AAA40[];
extern u8 D_001B76E0[];
extern u8 D_001C76E0[];
extern u8 D_001CD780[];
extern TieClass *D_001D7F30[];
extern u8 D_001D80B0[];
extern ShrubClass *D_001E1700[];
extern u8 D_001E1A00[];
extern char D_001E76D8[];

extern void DebugPrint(char *fmt, ...);
extern void FillTransferWords(void *, s32, s32);
extern void PackDmaTag(s32 arg0, u64 arg1, u64 arg2);
extern void func_001F2588(void);
extern void func_001F37E8(void);
extern void func_001F61E8(void);
extern void func_001F9810(void *, s32);
extern void func_001F9838(void *, void *, s32);
extern f32 func_001F99C8(f32, f32);
extern void func_001F9A10(void *, void *, void *);
extern void func_001F9A80(void *, void *, f32);
extern f32 func_001F9AF0(void *);
extern void func_001F9CF8(void *, void *, void *);
extern f32 func_001FA6C0(s32);
extern s32 func_001FA6D0(f32);
extern void func_00214970(s32);
extern void func_00217020(void);
extern void func_0022A5E0(void *);
extern void func_002334D8(void *);
extern void func_00234F98(void *);
extern void func_00237370(void *);




u8 *FUN_001e9b10(LevelHeader *hdr) {
    u8 *p;
    u8 *c;
    u8 *lc;
    u8 *sc;
    u8 *tc;
    s32 n;
    s32 i;
    s32 i2;
    s32 i0;
    s32 i1;
    s32 i3;
    s32 i5;
    s32 i6;
    s32 j;
    s32 k;
    s32 last;
    s32 lastTie;
    u8 *p2;
    u16 *sp;
    u16 *spa;
    u16 *spb;
    Shrub *sh;
    ShrubData *sd;
    Tie *tie;
    TieData *td;
    s32 cls;
    s32 size0;
    s32 size1;
    s32 size2;
    s32 size3;
    s32 size4;
    f32 s;
    f32 a;
    f32 b;
    s32 x;
    s32 y;
    s32 r;
    s32 g;
    s32 bl;
    u8 *col;
    Ent100 *e;
    Shrub *o;

    p = D_001940D8;
    FillTransferWords(D_0013F350, 0, 0x2310);
    FillTransferWords(D_00186F40, 0, 0x3A0);
    FillTransferWords(D_001AAA40, 0, 0x180);
    if (D_0015ED80 != 0) {
        if (D_0015ED60 == 1.0f) {
            func_00214970(1);
        }
    } else if (D_0015ED60 != 1.0f) {
        func_00214970(0);
    }
    D_0016034C = *(u8 *)&D_0015ED80;
    D_00194100.x4 = D_001941C0;
    D_00194100.x8 = D_00194180;
    D_00194100.xC = 0;
    D_00194100.x10 = 0;
    D_00194100.x14 = 0;
    func_001F9810(D_001B76E0, 0x10000);
    func_001F9810(D_001C76E0, 0x60);

    c = (u8 *)hdr + hdr->x0;
    D_0018CD00.x23C = *(s32 *)c;
    c += 4;
    D_0018CD00.x240 = *(s32 *)c;
    c += 4;
    D_0018CD00.x244 = *(s32 *)c;
    c += 4;
    D_0015F484 = *c;
    c += 4;
    D_0015F485 = *c;
    c += 4;
    D_0015F486 = *c;
    c += 4;
    D_0015F488 = *(f32 *)c;
    D_0015F48C = *(f32 *)(c + 4);
    D_0015F490 = *(f32 *)(c + 8);
    D_0015F494 = *(f32 *)(c + 12);
    func_001F2588();
    D_00160EC0 = 512000.0f;
    D_00160F70 = 720.0f;
    D_001603E4 = 500.0f;
    D_0015FF30 = 500;
    D_001600BC = 0x1F4000;
    PackDmaTag(D_0018CD00.x23C, D_0018CD00.x240, D_0018CD00.x244);
    func_002334D8(D_00100AE0);
    func_001F9810(D_0019C1C0, 0x100);
    func_001F9810(D_0019C3C0, 0x180);
    func_001F9810(D_0019BDC0, 0x400);

    lc = (u8 *)hdr + hdr->x4;
    n = *(s32 *)lc;
    lc += 0x10;
    if (n >= 12) {
        DebugPrint(D_001E76D8);
        n = 12;
    }
    if (n != 0) {
        func_001F9838(D_0019BDC0, lc, n << 6);
    }

    for (i0 = 0; i0 < D_00160E90; i0++) {
        D_00160E8C[i0].x36 = 0xFFFF;
    }
    i1 = 0;
    while (i1 < D_00160E90) {
        sp = (u16 *)0x70003000;
        for (j = 0; j < 0x3FF && i1 < D_00160E90; j++, i1++) {
            *sp++ = i1;
        }
        *sp = 0xFFFF;
        func_00234F98((void *)0x70003000);
    }

    sc = (u8 *)hdr + hdr->x34;
    D_00160F5C = *(s32 *)sc;
    sc += 0x10;
    D_00160F50 = (Shrub *)p;
    size0 = D_00160F5C << 5;
    p += size0;
    if (D_00160F5C != 0) {
        FillTransferWords(D_00160F50, 0, size0);
    }
    size1 = D_00160F5C * 0x1C0;
    p = (u8 *)(((u32)p + 0x3F) & ~0x3F);
    D_00160F58 = (ShrubData *)p;
    p += size1;
    if (D_00160F5C != 0) {
        FillTransferWords(D_00160F58, 0, size1);
    }
    last = -1;
    D_00160F54 = &D_00160F50[D_00160F5C];
    for (i2 = 0; i2 < D_00160F5C; i2++) {
        sh = &D_00160F50[i2];
        sd = &D_00160F58[i2];
        sh->cls = D_001E1A00[*(s32 *)sc];
        if (sh->cls != last) {
            D_001E1700[sh->cls]->first = sh;
            D_001E1700[sh->cls]->count = 0;
            last = sh->cls;
        }
        D_001E1700[sh->cls]->count++;
        sh->data = sd;
        sh->x14 = *(s32 *)(sc + 4);
        sh->x1E = 0xFFFF;
        sh->x1B = 0;
        sh->x1C = 0;
        k = *(s32 *)(sc + 0xC);
        qcopy(&sd->m[0], sc + 0x10);
        qcopy(&sd->m[1], sc + 0x20);
        qcopy(&sd->m[2], sc + 0x30);
        qcopy(&sd->m[3], sc + 0x40);
        sh->x18 = k;
        sc += 0x50;
        sd->m[3].w = D_001E1700[sh->cls]->x40;
        func_001F9CF8(sh, D_001E1700[sh->cls]->x30, sd);
        s = func_001F9AF0(&sd->m[0]);
        s = func_001F99C8(s, func_001F9AF0(&sd->m[1]));
        s = func_001F99C8(s, func_001F9AF0(&sd->m[2]));
        sh->scale = D_001E1700[sh->cls]->scale * s;
        func_001F9A80(sh, sh, sd->m[3].w);
        func_001F9A10(sh, sh, &sd->m[3]);
        D_00160F58[i2].m[0].w = 1.0f / func_001F9AF0(&D_00160F58[i2].m[0]);
        D_00160F58[i2].m[1].w = 1.0f / func_001F9AF0(&D_00160F58[i2].m[1]);
        D_00160F58[i2].m[2].w = 1.0f / func_001F9AF0(&D_00160F58[i2].m[2]);
        func_001F9838(sd->x140, sc, 0x80);
        sc += 0x80;
        sh->x1C = *(u16 *)sc;
        sc += 0x10;
    }
    spa = (u16 *)0x70000000;
    for (i3 = 0; i3 < D_00160F5C; i3++) {
        *spa++ = i3;
    }
    *spa = 0xFFFF;
    func_00237370((void *)0x70000000);

    tc = (u8 *)hdr + hdr->x3C;
    D_001603D0 = *(s32 *)tc;
    tc += 0x10;
    D_001603D4 = (Tie *)p;
    size2 = D_001603D0 << 5;
    p += size2;
    if (D_001603D0 != 0) {
        FillTransferWords(D_001603D4, 0, size2);
    }
    p = (u8 *)(((u32)p + 0x3F) & ~0x3F);
    D_001603DC = (TieData *)p;
    size3 = D_001603D0 << 6;
    p += size3;
    if (D_001603D0 != 0) {
        FillTransferWords(D_001603DC, 0, size3);
    }
    size4 = D_001603D0 * 0x60;
    D_001603E0 = p;
    p += size4;
    p = (u8 *)(((u32)p + 0x3F) & ~0x3F);
    if (D_001603D0 != 0) {
        FillTransferWords(D_001603E0, 0, size4);
    }
    lastTie = -1;
    D_001603D8 = &D_001603D4[D_001603D0];
    D_00160AB0.x4 = D_001603E0;
    p2 = p + 0x4000;
    for (i = 0; i < D_001603D0; i++) {
        tie = &D_001603D4[i];
        cls = D_001D80B0[*(s32 *)tc];
        td = &D_001603DC[i];
        tie->cls = cls;
        if (cls != lastTie) {
            lastTie = cls;
            D_001D7F30[cls]->first = tie;
            D_001D7F30[cls]->count = 0;
        }
        D_001D7F30[cls]->count++;
        tie->x18 = i;
        tie->radius = *(f32 *)(tc + 4);
        if (tie->radius < 16.0f) {
            tie->radius = 16.0f;
        }
        tie->x1B = 0;
        tie->x1E = 0xFFFF;
        if (D_001D7F30[cls]->x1C != 0) {
            f32 rad;

            tie->x17 = func_001FA6D0(*D_001D7F30[cls]->x1C);
            rad = func_001FA6C0(tie->x17) + 24.0f;
            if (tie->radius < rad) {
                tie->radius = rad;
            }
        }
        qcopy(&td->m[0], tc + 0x10);
        qcopy(&td->m[1], tc + 0x20);
        qcopy(&td->m[2], tc + 0x30);
        qcopy(&td->m3, tc + 0x40);
        tc += 0x50;
        td->m3.w = D_001D7F30[cls]->x20;
        a = (func_001F9AF0(&td->m[0]) + func_001F9AF0(&td->m[1])) * 0.5f;
        b = func_001F9AF0(&td->m[2]);
        x = func_001FA6D0(a * 4096.0f);
        y = func_001FA6D0(b * 4096.0f);
        if (x > 0x10000) {
            x = 0x10000;
        }
        if (y > 0x10000) {
            y = 0x10000;
        }
        td->m[2].w = x | (y << 16);
        r = *(s32 *)tc;
        tc += 4;
        g = *(s32 *)tc;
        tc += 4;
        bl = *(s32 *)tc;
        tc += 8;
        g = (g << 8) | 0x80000000;
        td->m[0].w = (bl << 16) | g | r;
        tie->x1C = *(u16 *)tc;
        tc += 0x10;
        func_001F9CF8(tie, D_001D7F30[cls], td);
        s = func_001F9AF0(&td->m[0]);
        s = func_001F99C8(s, func_001F9AF0(&td->m[1]));
        s = func_001F99C8(s, func_001F9AF0(&td->m[2]));
        tie->scale = D_001D7F30[cls]->scale * s;
        func_001F9A80(tie, tie, td->m3.w);
        func_001F9A10(tie, tie, &td->m3);
    }
    spb = (u16 *)0x70000000;
    for (i5 = 0; i5 < D_001603D0; i5++) {
        *spb++ = i5;
    }
    *spb = 0xFFFF;
    func_0022A5E0((void *)0x70000000);

    for (i6 = 0; i6 < D_001603D0; i6++) {
        s32 cr;
        s32 cg;
        s32 cb;
        u8 *cp;
        s32 k2;

        cp = D_001603E0 + i6 * 0x60;
        cr = 0;
        cg = 0;
        cb = 0;
        for (k2 = 0; k2 < 24; k2++) {
            cr += cp[0];
            cg += cp[1];
            cb += cp[2];
            cp += 4;
        }
        cr /= 24;
        cg /= 24;
        cb /= 24;
        *(s32 *)((u8 *)&D_001603DC[i6] + 0x1C) = (cb << 16) | (cg << 8) | cr;
    }

    D_0015FF18 = (Ent100 *)p;
    FillTransferWords(p, 0, 0x4000);
    p = p2;
    D_0015FF1C = D_0015FF18;
    *((u8 *)D_0015FF18 + 0x20) = 0xFF;
    D_0015FF28 = p;
    p += 0x2000;
    D_001600AC = p;
    D_0015FF20 = &D_0015FF18[0x3F];
    p += 0x20000;
    D_001600B4 = -1;
    D_001600B0 = 0;
    D_001600B8 = 0;
    func_001F9810(D_001CD780, 0x200);
    for (e = D_0015FF18; e != D_0015FF20; e++) {
        e->x36 = 0x7F80;
    }
    for (o = D_00160F50; o != D_00160F54; o++) {
        o->x18 = 0x7F80;
    }
    for (i0 = 0; i0 < D_00160E90; i0++) {
        D_00160E8C[i0].x3A = 0x7F80;
    }
    D_0018C318.x1C = 0;
    func_001F37E8();
    func_00217020();
    func_001F61E8();
    D_0015F60C = 0;
    D_0015F43C = 1.0f;
    return p;
}
#endif /* NON_MATCHING */
