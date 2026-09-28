/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00225FB8). */
#include "qcopy.h"
extern void func_001FA2B8(void *, void *);
extern char D_001863D0[];
extern char D_001863D0_a[] __asm__("D_001863D0");
extern int FUN_0020d580(void *);
extern void FUN_0020def8(void *);
extern void func_0020CCA8(int, int, void *);
extern void func_00214128(void *);
extern void FUN_0020e098(void *);
extern void func_001E9480(void *, void *, int, int, int);
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
/* Moby update: refresh its matrix from bone 4 of its class (+0x44),
   copying the translation row to the position, re-register it, start
   its idle sound (6 for class 0x1B1, else 0) on the D_001863D0 table
   and reset the sound fields. */
void FUN_00224d28(PauseMoby *m) {
    float mtx[16];
    int id = *(int *)(*m->cls + 0x44);

    FUN_0020d580(m);
    FUN_0020def8(m);
    func_0020CCA8(id, 4, mtx);
    qcopy(m->pos, &mtx[12]);
    func_001FA2B8(m->mtx, mtx);
    func_00214128(m->mtx);
    FUN_0020e098(m);
    if (m->oclass == 0x1B1) {
        func_001E9480(D_001863D0_a, D_001863D0, m->sound, 6, id);
    } else {
        func_001E9480(D_001863D0_a, D_001863D0, m->sound, 0, id);
    }
    m->x50 = 0;
    m->x68 = D_001863D0;
    m->x54 = 0;
    m->x6C = D_001863D0;
}

extern __typeof__(FUN_00224d28) func_00224D28 __attribute__((alias("FUN_00224d28")));
