/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00207200). */
typedef struct {
    char _pad0[0x12E4];
    unsigned char unk12E4;
    char _pad12E5[0x208C - 0x12E5];
    int unk208C;
} Menu13F450;
extern Menu13F450 D_0013F350;
/* Menu hit test: arg3 is the third float ($f14). b (state 16) is
   computed before a, as retail evaluates it; the second if's own
   `arg1 >= 0xC8` is retail's second test of $a1. */
int FUN_002069d0(void *arg0, int arg1, float unused1, float unused2, float arg3) {
    Menu13F450 *s = &D_0013F350;
    int b = s->unk208C == 16;
    int a = s->unk208C == 17 || s->unk208C == 18 || s->unk12E4 == 1;

    if (arg1 < 0xC8 && arg3 >= 39.5f && arg3 <= 42.5f && !a) {
        return 1;
    }
    if (arg1 >= 0xC8 && arg3 >= 87.0f && !b) {
        return 1;
    }
    return 0;
}

extern __typeof__(FUN_002069d0) func_002069D0 __attribute__((alias("FUN_002069d0")));
