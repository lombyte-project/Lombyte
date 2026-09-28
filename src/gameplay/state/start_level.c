/* Ported from rac1-decomp, the PAL decompilation (src/game/bmain.c, func_001E9808). */
#include "sda.h"
extern int D_0015ED80 MACRO_ADDR;
extern int D_0015EED8 MACRO_ADDR;
extern int D_0015F604 MACRO_ADDR;
extern int D_0015EE88 MACRO_ADDR;
typedef struct {
    int b;
    int a;
} LevelLoad;
typedef struct {
    char pad0[0x1A78];
    LevelLoad normal[4];
    LevelLoad alt[4];
} Globals137C80;
extern Globals137C80 D_00137B80;
extern char D_0013E550[];
extern char D_0013D290[];
extern char D_001940C0[];
extern void FlushCache(int);
extern void func_0022DCD0(void);
extern void func_00215EE8(void);
extern int FUN_001f96f8(int);
extern void func_001F4A58(int);
extern void func_0012EE08(int);
extern void func_002093D8(void);
extern void func_0023A3B8(int, int, int, int, int);
extern void sceCdSync(int);
extern void sceGsSyncV(int);
extern void FUN_00120558(int, int);
extern int FUN_0012f1c8(void);
extern void sceGsSyncVCallback(void (*)(void));
extern void func_00200B10(int, int, int, int, int, int);
/* Start level `level`: pick its two load parameters from the level
   table in D_00137B80 (the alternate set while D_0015ED80 is set), stop
   sound and wait for the loader (func_002093D8) to go idle, then load it
   (func_0023A3B8) and reset the display state. */
void start_level(int level) __asm__("FUN_001e9488");

void start_level(int level) {
    int a;
    int b;
    int t;

    if (level < 0) {
        return;
    }
    if (D_0015ED80 != 0) {
        b = D_00137B80.alt[level].b;
        a = D_00137B80.alt[level].a;
    } else {
        b = D_00137B80.normal[level].b;
        a = D_00137B80.normal[level].a;
    }
    D_0015EED8 = 2;
    D_0013E550[0x6B] |= 8;
    FlushCache(0);
    func_0022DCD0();
    func_00215EE8();
    func_001F4A58(FUN_001f96f8(12));
    D_0015F604 = 1;
    FlushCache(0);
    func_0022DCD0();
    func_00215EE8();
    func_0012EE08(0);
    for (;;) {
        char *ld = D_0013D290;
        if (*(int *)(ld + 0xD4) < 3 && *(int *)(ld + 0xDC) < 0) {
            break;
        }
        func_002093D8();
    }
    {
        char *g = D_001940C0;
        t = *(int *)(g + 0x1C);
    }
    func_0023A3B8(b, a, t + 0x100000, t + 0x400000, 0);
    sceCdSync(0);
    sceGsSyncV(0);
    FUN_00120558(0, 0);
    sceGsSyncVCallback(FUN_0012f1c8);
    func_00200B10(0x1000000, D_0015EE88, 0x1B, 6, 6, 1);
    D_0015EED8 = 0;
    func_001F4A58(4);
    D_0015F604 = 0;
    D_0013E550[0x6B] |= 0x10;
}

extern __typeof__(start_level) func_001E9488 __attribute__((alias("FUN_001e9488")));
