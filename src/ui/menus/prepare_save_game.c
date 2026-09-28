/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00227C78). */

#include "sda.h"

extern int D_0015ED84 MACRO_ADDR;
extern char D_0013D290[];
extern void func_0020ABB0(char *out);
extern void prepare_save_game(int, int);
extern char D_0015EE98[] MACRO_ADDR;
extern char D_00141EC0[];
extern void sceCdReadClock(void *);
extern void sceScfGetLocalTimefromRTC(void *);
extern void FUN_00208770(void);
extern void func_00207B08(void *);

void prepare_save_game(int arg0, int arg1) __asm__("FUN_002269c0");

void prepare_save_game(int arg0, int arg1) {
    char *b = D_0013D290;
    int saving = 1;
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    FUN_00208770();
    func_00207B08(D_00141EC0 + (D_0015ED84 << 11));
    func_0020ABB0((char *)arg0);
    *(int *)(b + 0xF4) = saving;
    *(int *)(b + 0xEC) = arg0;
    *(int *)(b + 0x14) = arg1;
    *(int *)(b + 0xC0) = 0;
    if (*(int *)(b + 0xDC) < 0) {
        *(int *)(b + 0xE0) = 0;
        *(int *)(b + 0xDC) = 0x13;
    }
}

extern __typeof__(prepare_save_game) func_002269C0 __attribute__((alias("FUN_002269c0")));
