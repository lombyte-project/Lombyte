/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00225E00). */
#include "qcopy.h"
extern void func_001FA2B8(void *, void *);
extern char D_001863D0[];
extern char D_001D5DD0[];
extern char D_001863D0_a[] __asm__("D_001863D0");
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
typedef struct {
    char pad00[0xC];
    int bone;           /* 0x0C */
    int cls;            /* 0x10 */
    char pad14[4];
    int still;          /* 0x18 */
    char pad1C[0x30];
} PauseClassRec;
extern PauseClassRec D_001863D0_r[] __asm__("D_001863D0");
extern void func_00212F90(void *, int, int, int);
extern void func_0020CB88(int, void *);
extern void FUN_0020cb10(int, int, void *);
/* Moby update with a per-class record: find the class in D_001863D0 (0x4C
   bytes each, 0x25 of them), refresh the matrix from the record's bone,
   and either register the moby (+0x18 set) or restart its idle sound;
   also re-sync the D_001D5DD0 animation group while its +1 flag is up.
   The two store groups are in the order that schedules as retail's. */
void FUN_00224b70(void *arg0) {
    PauseMoby *m = arg0;
    unsigned char *b = arg0;
    float mtx[16];
    int id = *(int *)(*m->cls + 0x44);
    int i;
    int still;
    int sync;
    unsigned char *s;

    if ((b[0x70] & 2) && b[0x53] != 1) {
        func_00212F90(m, 1, 0, 0);
    }
    for (i = 0; i < 0x25; i++) {
        if (D_001863D0_r[i].cls == m->oclass) {
            break;
        }
    }
    func_0020CCA8(id, D_001863D0_r[i].bone, mtx);
    qcopy(m->pos, &mtx[12]);
    FUN_0020def8(m);
    still = D_001863D0_r[i].still == 0;
    func_001FA2B8(m->mtx, mtx);
    if (!still) {
        func_00214128(m->mtx);
    }
    FUN_0020e098(m);
    sync = 0;
    s = (unsigned char *)D_001D5DD0;
    if (s[1] != 0) {
        sync = 1;
        func_0020CB88(id, s);
    }
    if (still) {
        func_001E9480(D_001863D0_a, D_001863D0, m->sound, 0, id);
        m->x68 = D_001863D0;
        m->x54 = 0;
        m->x6C = D_001863D0;
        *(int *)(b + 0x58) = 0;
        m->x50 = 0;
    }
    if (sync) {
        FUN_0020cb10(id, 0, s);
        *(int *)(s + 0x20) = 0;
        *(int *)(s + 0x24) = 0;
        *(int *)(s + 0x28) = 0;
    }
}

extern __typeof__(FUN_00224b70) func_00224B70 __attribute__((alias("FUN_00224b70")));
