/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_0021FF80). */

extern int D_001A00F0[];
extern unsigned char D_0014BEC0[];
extern void func_001F4280(int);
extern void func_001F4398(void);
extern void func_0020D330(int, int);
extern char D_00186F40[];
extern void *func_001FE540_id(int) __asm__("func_001FDD10");
extern const float D_001602A4_f __asm__("D_001602A4") __attribute__((sda));
extern short D_001602A4_s __asm__("D_001602A4");
extern char D_001602A8[];
extern int D_001E0888[];
extern int D_0013E500[];
extern int sprintf(char *, const char *, ...);
extern void func_001F6CF8_c(int, int, long, char *, int) __asm__("func_001F6940");
int setup_menu_display_text(char *arg0) __asm__("FUN_0021ef78");

int setup_menu_display_text(char *arg0) {
    char buf[0x100];
    char *t = D_00186F40;
    char *o = *(char **)(arg0 + 0x44);
    int count;

    *(float *)(o + 0x10) = *(float *)(t + 0x140) + 8.0f;
    *(float *)(o + 0x14) = *(float *)(t + 0x144) + D_001602A4_f;
    *(float *)(o + 0x18) = *(float *)(t + 0x148) - 0.1f;
    if (*(int *)(arg0 + 0x44) != 0) {
        func_0020D330(*(int *)(arg0 + 0x44), 1);
    }
    func_001F4280(0);
    count = 0;
    {
        int k;
        for (k = 0; k < 4; k++) {
            if (D_0014BEC0[D_001A00F0[0x89] * 4 + k] != 0) {
                count = count + 1;
            }
        }
    }
    sprintf(buf, D_001602A8, func_001FE540_id(0x4F4F), count,
                  func_001FE540_id(0x4F53), D_001E0888[D_001A00F0[0x89]]);
    func_001F6CF8_c(*(int *)(arg0 + 0x20) - 0x10, (D_0013E500[1] >> 1) - 8,
                    0x80000000L, buf, -1);
    func_001F6CF8_c(*(int *)(arg0 + 0x20) - 0x11, (D_0013E500[1] >> 1) - 9,
                    0x80FFA888L, buf, -1);
    func_001F4398();
    if (0) {
        (void)D_001602A4_s;
    }
    return 8;
}

extern __typeof__(setup_menu_display_text) func_0021EF78 __attribute__((alias("FUN_0021ef78")));
