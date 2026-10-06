/* Ported from rac1-decomp (src/game/pause.c, func_00227C78). */

#include "sda.h"

extern int D_0015ED84 MACRO_ADDR;
extern char D_0013D290[];
extern void memcard_make_whole_save(char *out) __asm__("func_0020ABB0");
extern void prepare_save_game(int, int) __asm__("FUN_002269c0");
extern char D_0015EE98[] MACRO_ADDR;
extern char D_00141EC0[];
extern void sceCdReadClock(void *);
extern void sceScfGetLocalTimefromRTC(void *);
extern void FUN_00208770(void);
extern void func_00207B08(void *);

void prepare_save_game(int save_data, int slot) __asm__("FUN_002269c0");

void prepare_save_game(int save_data, int slot) {
    char *b = D_0013D290;
    int saving = 1;
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    FUN_00208770();
    func_00207B08(D_00141EC0 + (D_0015ED84 << 11));
    memcard_make_whole_save((char *)save_data);
    *(int *)(b + 0xF4) = saving;
    *(int *)(b + 0xEC) = save_data;
    *(int *)(b + 0x14) = slot;
    *(int *)(b + 0xC0) = 0;
    if (*(int *)(b + 0xDC) < 0) {
        *(int *)(b + 0xE0) = 0;
        *(int *)(b + 0xDC) = 0x13;
    }
}

extern __typeof__(prepare_save_game) func_002269C0 __attribute__((alias("FUN_002269c0")));
