/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00222FA8). */
extern void func_001F4280(int);
extern void func_001F4398(void);
extern void func_001F68E8_c(int, int, long, void *, int) __asm__("func_001F6530");
extern void func_001F68E8_c(int, int, long, void *, int)
    __asm__("func_001F6530");
extern int func_001FF960(int, int);
typedef struct { char c[2]; } Glyph2;
extern char D_001602F0[];
extern char D_001602F8[];
extern int func_001FFA10(int);
extern void func_00200600(float, float, int, int, int, float, float, float);
/* Draws the glyph "\x10" (arg0->0x38 set) or "\x11" and a gauge sprite
   below it, rotated by pi in the second case. The glyph string is a
   2-byte char struct copied onto the stack (retail's lb/lb/sb/sb). */
int FUN_00221f58(char *arg0) {
    Glyph2 buf;

    func_001F4280(0);
    if (*(int *)(arg0 + 0x38) != 0) {
        buf = *(Glyph2 *)D_001602F0;
        func_001F68E8_c(4, *(int *)(arg0 + 0x24) / 2 - 8, 0x80FFA888L, &buf, -1);
        func_00200600(640.0f, (float)(*(int *)(arg0 + 0x24) << 3), 0x20, 0x10,
                        func_001FFA10(func_001FF960(0xE99E, 6)), 128.0f,
                        256.0f, 0.0f);
    } else {
        buf = *(Glyph2 *)D_001602F8;
        func_001F68E8_c(*(int *)(arg0 + 0x20) - 0x18,
                        *(int *)(arg0 + 0x24) / 2 - 8, 0x80FFA888L, &buf, -1);
        func_00200600(192.0f, (float)(*(int *)(arg0 + 0x24) << 3), 0x20, 0x10,
                        func_001FFA10(func_001FF960(0xE99E, 6)), 128.0f,
                        256.0f, 3.14159274f);
    }
    func_001F4398();
    return 2;
}

extern __typeof__(FUN_00221f58) func_00221F58 __attribute__((alias("FUN_00221f58")));
