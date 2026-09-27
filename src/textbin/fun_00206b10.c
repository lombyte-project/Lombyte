/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00207340). */
typedef struct {
    char _pad0[0x12E4];
    unsigned char unk12E4;
    char _pad12E5[0x208C - 0x12E5];
    int unk208C;
} Menu13F450;
extern Menu13F450 D_0013F350;
/* The struct's address in a local keeps one base register for both
   field reads. */
int FUN_00206b10(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 < 0x100) {
        Menu13F450 *s = &D_0013F350;
        return s->unk208C == 17 || s->unk208C == 18 || s->unk12E4 == 1;
    }
    return (arg1 >= 95.0f) ? 1 : 0;
}

extern __typeof__(FUN_00206b10) func_00206B10 __attribute__((alias("FUN_00206b10")));
