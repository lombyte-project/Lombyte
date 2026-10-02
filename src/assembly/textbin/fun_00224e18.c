#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00224e18/FUN_00224e18.s", FUN_00224e18);
#else
/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_002260A8). */
#include "qcopy.h"
extern void func_001FA2B8(void *, void *);
extern char D_001863D0[];
extern char D_001863D0_a[] __asm__("D_001863D0");
extern char D_001D5E50[];
extern char D_001D5E10[];
extern int func_0020D580(void *);
extern void func_0020DEF8(void *);
extern void func_0020CCA8(int, int, void *);
extern void func_00214128(void *);
extern void func_0020E098(void *);
extern void func_001E9480(void *, void *, int, int, int);
extern void func_0020CB88(int, void *);
extern void func_0020CB10(int, int, void *);
typedef struct {
    char pad00[0x10];
    float pos[4];       /* 0x10 */
    char pad20[4];
    int sound;          /* 0x24 */
    char pad28[0x28];
    int x50;            /* 0x50 */
    int x54;            /* 0x54 */
    char pad58[0x10];
    char *x68;          /* 0x68 */
    char *x6C;          /* 0x6C */
    char pad70[8];
    char **cls;         /* 0x78 */
    char pad7C[0x2A];
    short oclass;       /* 0xA6 */
    char padA8[0x18];
    float mtx[16];      /* 0xC0 */
} PauseMoby;

void FUN_00224e18(void *arg0) {
    PauseMoby *m = arg0;
    char **cls = m->cls;
    float mtx[16];
    int id = *(int *)(*cls + 0x44);
    int self;
    int syncA;
    int syncB;
    char *p;
    char *q;
    int snd;

    func_0020D580(m);
    func_0020DEF8(m);
    self = m == *(PauseMoby **)(*cls + 0x5C);
    func_0020CCA8(id, self ? 3 : 2, mtx);
    qcopy(m->pos, &mtx[12]);
    func_001FA2B8(m->mtx, mtx);
    func_00214128(m->mtx);
    func_0020E098(m);
    syncA = 0;
    syncB = 0;
    if ((unsigned char)D_001D5E50[1] != 0) {
        syncA = 1;
        func_0020CB88(id, D_001D5E50);
    }
    if ((unsigned char)D_001D5E10[1] != 0) {
        syncB = 1;
        func_0020CB88(id, D_001D5E10);
    }
    if (!self) {
        p = D_001863D0;
        q = D_001863D0_a;
        snd = m->sound;
    } else {
        q = D_001863D0_a;
        p = D_001863D0;
        snd = m->sound;
    }
    func_001E9480(q, p, snd, 0, id);
    m->x68 = p;
    m->x6C = p;
    if (syncA) {
        func_0020CB10(id, 0x17, D_001D5E50);
        *(int *)(D_001D5E50 + 0x20) = 0;
        *(int *)(D_001D5E50 + 0x24) = 0;
        *(int *)(D_001D5E50 + 0x28) = 0;
    }
    if (syncB) {
        func_0020CB10(id, 0x16, D_001D5E10);
        *(int *)(D_001D5E10 + 0x20) = 0;
        *(int *)(D_001D5E10 + 0x24) = 0;
        *(int *)(D_001D5E10 + 0x28) = 0;
    }
    m->x54 = 0;
    m->x50 = 0;
}
#endif /* NON_MATCHING */
