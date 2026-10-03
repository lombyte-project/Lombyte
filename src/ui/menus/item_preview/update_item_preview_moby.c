/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_0021EFA0). */
#include "sda.h"
#include "qcopy.h"
extern int FUN_00225530(int);
extern char *D_001D5BF4 NOT_SDA;
extern char D_001863D0[];
extern char D_00186F40[];
extern void *func_00225490(int);
extern void FUN_0021e1f8(char *);
/* Keeps the item preview moby in step with the highlighted entry: drop it
   when the entry's class (D_001863D0 record +0x3A) is -1, spawn it in
   front of the camera focus when there is none yet, or respawn it with
   the old one's position and orientation when the class changed. */
int update_item_preview_moby(char *arg0) __asm__("FUN_0021df98");

int update_item_preview_moby(char *arg0) {
    char *q = *(char **)(D_001D5BF4 + 0x40);
    int item = *(short *)(*(int *)(q + 0x3C) * 10 + *(char **)(q + 0x48) + 6);
    short cur;
    char *rec;
    short want;

    cur = *(char **)(arg0 + 0x44) != 0 ? *(short *)(*(char **)(arg0 + 0x44) + 0xA6) : -1;
    rec = D_001863D0 + item * 0x4C;
    want = *(short *)(rec + 0x3A);
    if (want != -1 && cur == -1) {
        char *o = func_00225490(want);

        if (o != 0) {
            char *t = D_00186F40;

            *(char **)(arg0 + 0x44) = o;
            *(short *)(o + 0x34) = 0;
            *(float *)(o + 0x10) = *(float *)(t + 0x140) + 6.0f;
            *(float *)(o + 0x14) = *(float *)(t + 0x144);
            *(float *)(o + 0x18) = *(float *)(t + 0x148) - 0.3f;
            *(float *)(o + 0x48) = 3.1415927f;
            *(void **)(o + 0x74) = (void *)FUN_0021e1f8;
            **(void ***)(o + 0x78) = arg0;
        }
    } else if (want == -1) {
        *(int *)(arg0 + 0x44) = FUN_00225530(*(int *)(arg0 + 0x44));
    } else if (cur != want) {
        char *n = func_00225490(want);

        if (n != 0) {
            char *old;

            *(short *)(n + 0x34) = 0;
            old = *(char **)(arg0 + 0x44);
            qcopy(n + 0x10, old + 0x10);
            qcopy(n + 0x40, old + 0x40);
            *(int *)(n + 0x74) = *(int *)(old + 0x74);
            **(void ***)(n + 0x78) = arg0;
        }
        FUN_00225530(*(int *)(arg0 + 0x44));
        *(char **)(arg0 + 0x44) = n;
    }
    return 0;
}

extern __typeof__(update_item_preview_moby) func_0021DF98 __attribute__((alias("FUN_0021df98")));
