/* Ported from rac1-decomp (src/game/pause.c, func_00222640). */
#include "sda.h"
extern int D_0013D4C0 NOT_SDA;
extern unsigned char D_0013E520[];
typedef struct {
    unsigned short a; /* +0 */
    short b;          /* +2 */
    short c;          /* +4 */
    short id;         /* +6 */
    short idx;        /* +8 */
} Item0A;
extern Item0A D_001CF120[];
extern Item0A D_001D60E0[];
extern int D_001D6178[];
extern char D_001863D0[];
extern char D_001D0D00[];
/* Builds the pause-menu item list from the 15 entries of D_001CF120
   that D_0013D4C0 enables. The flag needs its own `f = b != 0`:
   `(b != 0) << 2` folds into a branch. */
int build_pause_item_list(void) __asm__("FUN_002215f8");

int build_pause_item_list(void) {
    int i;
    int n = 0;
    char *m;

    for (i = 0; i < 15; i++) {
        int id = D_001CF120[i].id;
        if (((unsigned char *)&D_0013D4C0)[id] != 0) {
            Item0A *out = &D_001D60E0[n];
            char *rec = D_001863D0 + id * 0x4C;
            unsigned char b = D_0013E520[id];
            int f = b != 0;
            out->a = *(unsigned short *)(rec + 0x38);
            out->b = f << 2;
            out->c = 0;
            out->id = id;
            out->idx = i;
            D_001D6178[n] = b ? *(short *)(rec + 0x42) : *(short *)(rec + 0x40);
            n++;
        }
    }
    m = D_001D0D00;
    *(int *)(m + 0x40) = n;
    return 0;
}

extern __typeof__(build_pause_item_list) func_002215F8 __attribute__((alias("FUN_002215f8")));
