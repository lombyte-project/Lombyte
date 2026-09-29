/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00207F00). */
#define NOT_SDA __attribute__((section(".data")))
extern unsigned char D_0013D3BA NOT_SDA;
int FUN_00207650(void) {
    return D_0013D3BA != 0;
}

extern __typeof__(FUN_00207650) func_00207650 __attribute__((alias("FUN_00207650")));
