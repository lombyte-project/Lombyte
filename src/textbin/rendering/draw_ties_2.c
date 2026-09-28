/* Ported from rac1-decomp, the PAL decompilation (src/game/tiefunc.c, func_00236CA8). */
#include "sda.h"
extern void FUN_001f98d0(void *, void *, int);
extern int D_0018A2B0[];
extern void FlushCache(int);
extern char *D_001E1700[];
extern char D_001E3000[];
extern char D_001E4200[];
extern char D_001E2A00[];
extern char D_001E3E00[];
extern char D_001E3200[];
extern char D_001E4400[];
extern int D_00160F00 MACRO_ADDR;
extern int D_0015EE78 MACRO_ADDR;
extern int D_0015EE74 MACRO_ADDR;
extern int D_00160F68 MACRO_ADDR;
extern char D_00160F30[];
extern char D_00160F40[];
extern char D_001DF030[];
extern void func_001F21B8(void *, int); /* empty profiling marker */
extern void func_001F21B0(void *, int); /* empty profiling marker */
extern void FUN_00235be8(void);
extern void WriteDmaChannel(void *, int, int);
extern void func_00235640(void); /* DmaTieTextures */
extern int D_00160F4C MACRO_ADDR;
typedef struct {
    int unk00[6];
    int unk18;
} DrawCfg_236CA8;
/* DrawTies_2: DrawTies_1's two passes, the first with the odd ties' 0x8
   flag set (and the saved tie data swapped in around it), the second with
   the even ones'. Each flag loop reads the tie count into its own local
   (the loop's stores could alias it) and the flag is a `short` (so the
   `&= ~8` stays an int AND with -9, not andi 0xFFF7). */
void draw_ties_2(void) __asm__("FUN_00235990");

void draw_ties_2(void) {
    {
        int i;
        int n = D_00160F4C;
        for (i = 1; i < n; i += 2) {
            *(short *)(D_001E1700[i] + 0x24) |= 8;
        }
    }
    {
        int p = D_00160F00;
        D_00160F68 = p;
        D_0015EE74 = D_0015EE78;
        p += 0x10;
        D_00160F00 = p;
    }
    func_001F21B8(D_00160F30, 1);
    {
        DrawCfg_236CA8 *d = (DrawCfg_236CA8 *)D_0018A2B0;
        if (d->unk18 != 0) {
            FlushCache(0);
            FUN_00235be8();
            WriteDmaChannel(D_001E4400, 0x3600, 0x40);
        }
    }
    func_00235640();
    FUN_001f98d0(D_001E4200, D_001E3000, 0x200);
    FUN_001f98d0(D_001E3E00, D_001E2A00, 0x400);
    {
        int i;
        int n = D_00160F4C;
        for (i = 1; i < n; i += 2) {
            *(short *)(D_001E1700[i] + 0x24) &= ~8;
        }
    }
    {
        int i;
        int n = D_00160F4C;
        for (i = 0; i < n; i += 2) {
            *(short *)(D_001E1700[i] + 0x24) |= 8;
        }
    }
    {
        DrawCfg_236CA8 *d = (DrawCfg_236CA8 *)D_0018A2B0;
        int p = D_00160F00;
        D_00160F68 = p;
        D_0015EE74 = D_0015EE78;
        p += 0x10;
        D_00160F00 = p;
        if (d->unk18 != 0) {
            FlushCache(0);
            FUN_00235be8();
            WriteDmaChannel(D_001E3200, 0x3600, 0x40);
        }
    }
    func_001F21B8(D_00160F40, 5);
    func_00235640();
    {
        int i;
        int n = D_00160F4C;
        for (i = 0; i < n; i += 2) {
            *(short *)(D_001E1700[i] + 0x24) &= ~8;
        }
    }
    FUN_001f98d0((void *)D_00160F00, D_001DF030, 0x20);
    func_001F21B0(D_00160F40, 5);
}

extern __typeof__(draw_ties_2) func_00235990 __attribute__((alias("FUN_00235990")));
