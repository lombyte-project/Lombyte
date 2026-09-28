/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00222B98). */
extern void func_001F4280(int);
extern void func_001F4398(void);
extern void *func_001FDD10(int);
extern void FUN_00233980(int, long);
typedef struct {
    short s[12];
} TextBox;
extern char D_001602E8[];
extern int D_001D5CC4[];
extern void func_001F75F0(TextBox *, long, char *, int);
/* Draws the two-line prompt box (text 0x4FB3 for D_001D5CC4[0] in 0..2,
   0x4FB5 for 3, else D_001602E8) sized from arg0's +0x20/+0x24. The box
   is an aggregate initializer: this compiler clears it with a memset
   libcall, fills a temporary and copies that into the local with
   ldl/ldr/sdl/sdr pairs, exactly retail's sequence. */
int draw_prompt_box(char *arg0) __asm__("FUN_00221b50");

int draw_prompt_box(char *arg0) {
    char *text;
    int v;

    FUN_00233980(0x42, 0x44);
    FUN_00233980(0x47, 0x2004B);
    func_001F4280(0);
    text = D_001602E8;
    v = D_001D5CC4[0];
    switch (v) {
    case 0:
    case 1:
    case 2:
        text = func_001FDD10(0x4FB3);
        break;
    case 3:
        text = func_001FDD10(0x4FB5);
        break;
    }
    {
        TextBox c = { { 1, *(int *)(arg0 + 0x24) + 1, 1, *(int *)(arg0 + 0x20) + 1,
                        *(int *)(arg0 + 0x20) >> 1, 5, 0, 0, 0x10, 5 } };

        func_001F75F0(&c, 0x80000000L, text, -1);
        c.s[5] = (*(int *)(arg0 + 0x24) - c.s[7]) >> 1;
        c.s[9] ^= 4;
        func_001F75F0(&c, 0x80000000L, text, -1);
        c.s[0]--;
        c.s[1]--;
        c.s[2]--;
        c.s[3]--;
        c.s[4]--;
        c.s[5]--;
        func_001F75F0(&c, 0x80FFA888L, text, -1);
    }
    func_001F4398();
    return 2;
}

extern __typeof__(draw_prompt_box) func_00221B50 __attribute__((alias("FUN_00221b50")));
