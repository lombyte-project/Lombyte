#include "types.h"

extern char D_00186310[];
extern char D_00186F40[];
extern char D_001D5BF0[];
extern char D_001D5DD0[];
extern char D_001D5E10[];
extern char D_001D5E50[];
extern int D_0015FF4C;
extern int D_00224B60[];
extern int func_001E9410();
extern int FUN_00224fc0();
extern int FillTransferWords();
extern int func_00225490();
extern int FUN_00225ac0();
extern int FUN_00226718();
extern s32 select_next_stream_buffer() __asm__("FUN_00225c18");

int FUN_002240c8(char *arg0) {
    char *m;
    int *p;
    int *v;
    char *q;
    int i, j;

    FUN_00225ac0(1);
    FUN_00226718();
    {
        unsigned char *g = D_001D5BF0;

        D_0015FF4C = -1;
        *(int *)(g + 0x11C) = -1;
        *(int *)(g + 0x120) = -1;
        *(int *)(g + 0xA0) = select_next_stream_buffer(1);
        *(int *)(g + 0xA4) = select_next_stream_buffer(1);
        g[0xC8] = 0xFF;
        g[0xC9] = 0xFF;
        g[0xCA] = 0;
        p = (int *)(g + 0xB0);
        for (i = 2; i >= 0; i--) {
            *p = select_next_stream_buffer(0);
            p++;
        }
    }
    {
        char *g = D_001D5BF0;

        D_001D5DD0[1] = 0;
        *(int *)(g + 0x1C) = -1;
        D_001D5E10[1] = 0;
        D_001D5E50[1] = 0;
    }
    m = func_00225490(0);
    q = arg0 + 0xBB;
    for (j = 23; j >= 0; j--) {
        *q = 0;
        q--;
    }
    if (m != 0) {
        char *cam = D_00186F40;
        char *g;

        *(char **)(arg0 + 0x44) = m;
        *(short *)(m + 0x34) = 0;
        *(float *)(m + 0x10) = *(float *)(cam + 0x140) + 4.0f;
        *(float *)(m + 0x14) = *(float *)(cam + 0x144);
        *(float *)(m + 0x18) = *(float *)(cam + 0x148) - 0.6f;
        *(float *)(m + 0x48) = 3.1415927f;
        *(void **)(m + 0x74) = D_00224B60;
        v = *(int **)(m + 0x78);
        v[0] = (int)arg0;
        v[1] = 0;
        v[2] = 0;
        g = D_001D5BF0;
        *(int *)(g + 0xC0) = -1;
        FillTransferWords(D_00186310, 0, 0x40);
        func_001E9410(m);
    }
    m = func_00225490(0x259);
    if (m != 0) {
        **(int **)(m + 0x78) = (int)arg0;
        *(void **)(m + 0x74) = FUN_00224fc0;
        *(short *)(m + 0x34) = 4;
    }
    *(char **)(arg0 + 0x48) = m;
    return 0;
}

extern __typeof__(FUN_002240c8) func_002240C8 __attribute__((alias("FUN_002240c8")));
