/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00226380). */
#include "qcopy.h"
extern void func_001FA2B8(void *, void *);
extern int FUN_0020d580(void *);
extern void FUN_0020def8(void *);
extern void func_0020CCA8(int, int, void *);
extern void func_00214128(void *);
extern void FUN_0020e098(void *);
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
/* func_00225FB8's sibling: refresh the matrix from bone 0x1E (class 0x197) or 0x1D, copy its translation row to the position and re-register. FUN_0020d580 takes the moby, and the bone test is written == 0x197 so the movn picks 0x1D as retail does. */
void FUN_002250f0(PauseMoby *m) {
    float mtx[16];
    int id = *(int *)(*m->cls + 0x44);

    FUN_0020d580(m);
    FUN_0020def8(m);
    func_0020CCA8(id, m->oclass == 0x197 ? 0x1E : 0x1D, mtx);
    qcopy(m->pos, &mtx[12]);
    func_001FA2B8(m->mtx, mtx);
    func_00214128(m->mtx);
    FUN_0020e098(m);
}

extern __typeof__(FUN_002250f0) func_002250F0 __attribute__((alias("FUN_002250f0")));
