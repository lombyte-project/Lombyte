#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002196b8/FUN_002196b8.s", FUN_002196b8);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef union { u128 q; f32 f[4]; } Vec4;

typedef struct {
    u8 pad0[0x50];
    int x;      /* 0x50 */
    int y;      /* 0x54 */
    int w;      /* 0x58 */
    int h;      /* 0x5C */
} Frame;

typedef struct {
    u8 pad0[0x78];
    Frame *frame;   /* 0x78 */
} Slot;

typedef struct Panel Panel;
struct Panel {
    u8 pad0[4];
    int (*draw)(Panel *);   /* 0x04 */
    u8 pad8[8];
    int flags;              /* 0x10 */
    u8 pad14[4];
    int x;                  /* 0x18 */
    int y;                  /* 0x1C */
    int w;                  /* 0x20 */
    int h;                  /* 0x24 */
};

typedef struct {
    u8 pad0[4];
    u8 *panels;             /* 0x04 */
    u8 pad8[0xD0];
    int xD8;                /* 0xD8 */
} Game;

extern long D_0015EED0;
extern void *D_0015FF18[];
extern int D_001601B0 __attribute__((sda));
extern int D_001CE2C0[];
extern Game D_001D5BF0;
extern Slot *D_001D5D90[];

extern void FUN_00233980(int, long);
extern void func_0020D278(void);
extern void func_0020D1F0(void);
extern void func_0020D218(void);
extern void func_0020D330(void *, int);
extern void func_00218D10(void);
extern void func_001F2260(void);
extern void func_00237A78(Vec4 *, Vec4 *, int *, int *, int *, int *);
extern void func_00200E08(int, int, int, int, u64, int);
extern void func_001F7888(int, int, int, f32);
extern void func_00200F90(int, int, int, int, u64, u32, int);
extern void func_001F7978(void);
extern void func_001F5450(int, int, int, int, int, int, int, int, long, long);
extern void func_0020D248(void);
extern void func_0020D3B0(void);
extern void func_001F4280(int);
extern void func_00223E28(Slot *);
extern void func_001F4398(void);

void FUN_002196b8(void)
{
    Vec4 mat[4];
    Vec4 unused[5];
    Vec4 a;
    Vec4 b;
    int w;
    int h;
    int x;
    int y;
    Panel **list;
    Slot *slot;
    Panel *p;
    Frame *f;
    Frame *fr;
    int i;
    int j;
    int pass;
    int k;
    int n;
    int flags;
    int fw;
    int fh;
    int tw;
    int th;
    int sw;
    int sh;
    int r;
    int u;
    int v;
    int uw;
    Slot **slots;
    int vh;
    int fx;
    int fy;

    FUN_00233980(0x47, 0x5360B);
    func_0020D278();
    func_0020D1F0();
    func_0020D218();
    func_0020D330(D_0015FF18[0], 4);
    func_00218D10();
    func_001F2260();
    for (i = 0; i < 14; i++) {
        if (D_001CE2C0[i] != 0 && D_001D5D90[i] != 0 && (i != 6 || D_001D5BF0.xD8 != 0)) {
            func_0020D330(D_001D5D90[i], 1);
        }
    }

    list = D_001D5BF0.panels != 0 ? (Panel **)(D_001D5BF0.panels + 0x44) : 0;
    for (j = 0; j < 14; j++) {
        slot = D_001D5D90[j];
        if (slot == 0 || D_001CE2C0[j] == 0 || (j == 6 && D_001D5BF0.xD8 == 0)) {
            continue;
        }
        f = slot->frame;
        qcopy(&mat[0], (Vec4 *)f + 0);
        qcopy(&mat[1], (Vec4 *)f + 1);
        qcopy(&mat[2], (Vec4 *)f + 2);
        qcopy(&mat[3], (Vec4 *)f + 3);
        a.q = mat[0].q;
        b.q = mat[3].q;
        func_00237A78(&a, &b, &w, &h, &x, &y);
        x++;
        y++;
        if (list != 0 && list[j] != 0) {
            list[j]->w = w;
            list[j]->h = h;
            list[j]->x = x;
            list[j]->y = y;
        }
        f->x = x;
        f->y = y;
        f->w = w;
        f->h = h;
        func_00200E08(x + 1, y + 1, x + w - 1, y + h - 1, D_001601B0, 0);
    }

    for (pass = 0; pass < 2; pass++) {
        for (k = 0; k < 14; k++) {
            slots = D_001D5D90;
            slot = slots[k];
            if (slot == 0 || list == 0) {
                continue;
            }
            p = list[k];
            if (p == 0) {
                continue;
            }
            flags = p->flags;
            if (flags & 4) {
                continue;
            }
            if (D_001CE2C0[k] == 0 || p->draw == 0) {
                continue;
            }
            if (k == 6 && D_001D5BF0.xD8 == 0) {
                continue;
            }
            if (pass == 0 && !(flags & 2)) {
                continue;
            }
            if (pass == 1 && (flags & 2)) {
                continue;
            }
            if (flags & 1) {
                p->draw(p);
                continue;
            }
            fr = slot->frame;
            fx = fr->x;
            fy = fr->y;
            fw = fr->w;
            fh = fr->h;
            tw = 7;
            while ((1 << tw) < fw) {
                tw++;
            }
            th = 7;
            while ((1 << th) < fh) {
                th++;
            }
            while (tw + th >= 18) {
                th--;
            }
            func_001F7888(tw, th, pass != 0, 1.0f);
            sh = 1 << th;
            sw = 1 << tw;
            func_00200F90(0, 0, sw, sh, D_001601B0, 0, 0);
            r = list[k]->draw(list[k]);
            func_001F7978();
            if (r & 1) {
                continue;
            }
            u = 0;
            uw = sw;
            v = 0;
            vh = sh;
            if (r & 2) {
                uw = fw;
                vh = fh;
            } else if (r & 8) {
                u = (uw - fw) / 2;
                v = (vh - fh) / 2;
                uw -= u;
                vh -= v;
                if (sw < uw) {
                    uw = sw;
                }
                if (sh < vh) {
                    vh = sh;
                }
                if (u < 0) {
                    u = 0;
                }
                if (v < 0) {
                    v = 0;
                }
            } else if (r & 4) {
                if (fw < fh) {
                    u = uw / 2 - fw * uw / (fh * 2);
                    uw -= u;
                } else {
                    v = vh / 2 - fh * vh / (fw * 2);
                    vh -= v;
                }
            } else if (!(r & 0x10)) {
                continue;
            }
            FUN_00233980(0x42, 0x8000000064L);
            FUN_00233980(0x47, 0x43);
            func_001F5450(fx, fy, fw, fh, u, v, uw - u, vh - v, 0x80808080L, D_0015EED0);
        }
        if (pass == 0) {
            func_0020D248();
            func_0020D3B0();
        }
    }

    func_001F4280(0);
    for (n = 0; n < 14; n++) {
        if (D_001CE2C0[n] != 0 && (n != 6 || D_001D5BF0.xD8 != 0)) {
            func_00223E28(slots[n]);
        }
    }
    func_001F4398();
}
#endif /* NON_MATCHING */
