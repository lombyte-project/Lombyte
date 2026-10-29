/* Ported from rac1-decomp (src/game/mobyutil.c, func_00215048). */
/* A goto into the first `return 0` gives retail's backward beqz; the two
   nops before it are short-loop padding (tools/ps2eeas_nops.py). */
int suck_get_aim_height(char *moby) __asm__("FUN_002141f8");
int suck_get_aim_height(char *moby) {
    if (moby == 0) {
    ret0:
        return 0;
    }
    if ((*(unsigned short *)(moby + 0x34) & 0x20) == 0) {
        goto ret0;
    }
    return **(int **)(moby + 0x78);
}

extern __typeof__(suck_get_aim_height) func_002141F8 __attribute__((alias("FUN_002141f8")));
