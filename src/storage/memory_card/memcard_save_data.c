/* Ported from rac1-decomp (src/game/memcard.c, func_0020BFC8). */
#include "sda.h"
extern char D_0013D290[];
extern int D_0015ED84 MACRO_ADDR;
extern int D_001A04C0[];
extern int D_001A07C0[];
extern int memcard_prepare_data(void *dst, int i, int *table) __asm__("func_0020AD78");
extern void sceCdReadClock(void *);
extern void sceScfGetLocalTimefromRTC(void *);
extern void FUN_00208770(void);
extern void func_00207B08(void *);
extern char D_0015EE98[] MACRO_ADDR;
extern char D_00141EC0[];
extern unsigned char D_0013DD58[];
extern int D_0015ED98 MACRO_ADDR;
extern int D_0015EE24 MACRO_ADDR;
extern int D_0015EE20 MACRO_ADDR;
extern int D_0015EEB4 MACRO_ADDR;
extern char D_0014EED0[];
extern char D_001506D0[];
/* memcard_Save(slot, flags): starts the save (sceCdReadClock/sceScfGetLocalTimefromRTC
   on D_0015EE98, FUN_00208770, func_00207B08 on the D_00141EC0 page for
   D_0015ED84). With no valid card slot (D_0013D290+0xC8, its record's
   +0x14) it returns whether `slot` is 0. Otherwise it ORs `slot` into the
   pending mask (+0xFC); unless nothing is pending, a save is running
   (+0xDC >= 3) or a result is waiting (+0xE4 >= 0), it stamps +0xD0 with
   D_0015ED84 (temporarily switching D_0015ED84 to `flags` and setting that
   D_0013DD58 byte), fills entry e[+0x14] (four globals and the 8-byte
   name), serialises both tables (func_0020AD78), restores the byte and
   D_0015ED84, and sets result 0xF. Returns whether the result is 0xF.
   D_0015EE98 is MACRO_ADDR (retail rebuilds its address at each use), the
   name field is reached from a `q + 0x30` base, and each block reads
   D_0013D290 through its own `char *` local (retail keeps only the %hi). */
int memcard_save_data(int slot, int flags) __asm__("FUN_0020b178");

int memcard_save_data(int slot, int flags) {
    unsigned char saved;

    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    FUN_00208770();
    func_00207B08(D_00141EC0 + (D_0015ED84 << 11));
    {
        char *p = D_0013D290;
        int active = *(int *)(p + 0xC0);
        int mask;

        if (active == -1 || *(int *)(p + active * 0xB8 + 0x14) < 0) {
            return slot == 0;
        }
        mask = *(int *)(p + 0xF4) | slot;
        *(int *)(p + 0xF4) = mask;
        if (mask == 0) {
            goto done;
        }
        if (slot == 0) {
            D_0015EEB4 |= 0x200;
        }
        if (*(int *)(p + 0xD4) >= 3) {
            goto done;
        }
        if (*(int *)(p + 0xDC) >= 0) {
            goto done;
        }
        *(int *)(p + 0xC8) = D_0015ED84;
        saved = 0;
        if (flags >= 0) {
            D_0015ED84 = flags;
            saved = D_0013DD58[flags];
            if (saved == 0) {
                D_0013DD58[flags] = 1;
            }
        }
    }
    {
        char *q = D_0013D290;
        char *names = q + 0x30;

        *(int *)(q + *(int *)(q + 0x14) * 0x1C + 0x24) = D_0015ED98;
        *(int *)(q + *(int *)(q + 0x14) * 0x1C + 0x20) = D_0015ED84;
        *(int *)(q + *(int *)(q + 0x14) * 0x1C + 0x2C) = D_0015EE24;
        memcpy(names + *(int *)(q + 0x14) * 0x1C, D_0015EE98, 8);
        *(int *)(q + *(int *)(q + 0x14) * 0x1C + 0x28) = D_0015EE20;
        memcard_prepare_data(D_0014EED0, 0, D_001A04C0);
        memcard_prepare_data(D_001506D0, *(int *)(q + 0xC8), D_001A07C0);
        if (flags >= 0) {
            D_0013DD58[D_0015ED84] = saved;
            D_0015ED84 = *(int *)(q + 0xC8);
        }
        if (*(int *)(q + 0xDC) < 0) {
            *(int *)(q + 0xDC) = 0xF;
            *(int *)(q + 0xE0) = *(int *)(q + 0xC0);
        }
    }
done: {
    char *r = D_0013D290;
    return *(int *)(r + 0xDC) == 0xF;
}
}

extern __typeof__(memcard_save_data) func_0020B178 __attribute__((alias("FUN_0020b178")));
