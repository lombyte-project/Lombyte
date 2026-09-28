/* Ported from rac1-decomp, the PAL decompilation (src/game/sound.c, func_0022F090). */
/* 8 bytes of post-endlabel nop padding in retail -- see func_001F6668. */
void FUN_0022dd78(int arg0, long arg1) {
    int *p = (int *)(int)arg1;
    if (p != 0) {
        *p = arg0;
    }
}

extern __typeof__(FUN_0022dd78) func_0022DD78 __attribute__((alias("FUN_0022dd78")));
