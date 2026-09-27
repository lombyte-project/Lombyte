/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002073E8). */
#define NOT_SDA __attribute__((section(".data")))
extern unsigned char D_0013D3A7 NOT_SDA;
int FUN_00206bb8(void) {
    return D_0013D3A7 != 0;
}

extern __typeof__(FUN_00206bb8) func_00206BB8 __attribute__((alias("FUN_00206bb8")));
