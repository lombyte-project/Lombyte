/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00226250). */
#include "qcopy.h"
extern void func_001FA2B8(void *, void *);
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
extern void func_00212F90(void *, int, int, int);
/* Moby 0x259 update: while flag 0x02 is set, keep its animation on 1 (or,
   for class 0x25F in state 6, run the 6 animation until its timer +0x20
   runs out and then blend back to 1), then refresh its matrix from bone 5
   as func_00225FB8 does. */
void FUN_00224fc0(void *arg0) {
    PauseMoby *m = arg0;
    unsigned char *b = (unsigned char *)m;
    float mtx[16];
    int id = *(int *)(*m->cls + 0x44);

    if (b[0x70] & 2) {
        if (m->oclass == 0x25F) {
            if (b[0x52] == 6) {
                if (--b[0x20] == 0) {
                    if (b[0x53] != 1) {
                        func_00212F90(m, 1, 0, 10);
                    }
                } else if (b[0x53] != 6) {
                    func_00212F90(m, 6, 0, 0);
                }
            } else if (b[0x53] != 1) {
                func_00212F90(m, 1, 0, 0);
            }
        } else if (b[0x53] != 1) {
            func_00212F90(m, 1, 0, 0);
        }
    }
    func_0020CCA8(id, 5, mtx);
    qcopy(m->pos, &mtx[12]);
    func_001FA2B8(m->mtx, mtx);
    func_00214128(m->mtx);
    FUN_0020e098(m);
}

extern __typeof__(FUN_00224fc0) func_00224FC0 __attribute__((alias("FUN_00224fc0")));
