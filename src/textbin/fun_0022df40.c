/* Ported from rac1-decomp, the PAL decompilation (src/game/space.c, func_0022F258). */
#define MACRO_ADDR __attribute__((section(".sdata")))
extern void FUN_001f98d0(void *, void *, int);
extern int FUN_001f44b8(int);
extern void FUN_001f9a68(void *, void *, float);
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_001f9cf8(void *, void *, void *);
typedef struct {
    float v[4][4];     /* 0x00 */
    int rgba[4];       /* 0x40 */
    char uv[0x20];     /* 0x50 */
    long gs[4];        /* 0x70 */
} Quad_FBE0;
extern char D_001D97B0[];
extern void func_001F7D30(void *, int, int);
extern int D_0015F604 MACRO_ADDR;
extern int FUN_001f96f8(int);
extern void FUN_001f9a80(void *, void *, float);
extern int FUN_001fa820(void *, int *, float);
extern float func_00213508(void *, int, float);
typedef struct {
    char pad[0x20];
    int mode;
    short level;
    short set;
} FlareCfg;
extern FlareCfg D_0013E030;
extern float D_001D97D0[][4][4];
/* Draws a light flare quad at arg0's position (+0x00; the matrix is at
   +0xC0) when FUN_001fa820 finds it on screen, with the alpha it
   returns. In D_0015F604 mode 6 with flare set 3, the flare fades as
   D_0013E030's level passes FUN_001f96f8(150), and its depth comes from
   func_00213508. The corners are D_001D97D0[set]. */
void FUN_0022df40(char *arg0) {
    float a[4];
    Quad_FBE0 q;
    float b[4];
    int alpha;
    float z;
    int i;

    alpha = 0;
    FUN_001f9a80(a, arg0, 1.0f / 1024.0f);
    if (FUN_001fa820(a, &alpha, 32.0f) < 0) {
        return;
    }
    if (D_0015F604 == 6 && D_0013E030.mode == 3
        && D_0013E030.level > FUN_001f96f8(150)) {
        alpha -= (D_0013E030.level - FUN_001f96f8(150)) * 4;
        if (alpha <= 0) {
            return;
        }
    }
    q.gs[1] = FUN_001f44b8(0);
    q.gs[2] = 0xFF9000000260;
    q.gs[3] = 0x8000000044;
    q.gs[0] = 0;
    FUN_001f98d0(q.uv, D_001D97B0, 0x20);
    FUN_001f9a68(b, arg0, 1.0f / 1024.0f);
    z = *(float *)(arg0 + 0x18) + 0.1f;
    if (D_0015F604 == 6) {
        z = func_00213508(arg0 + 0x10, 0, 0.5f) + 0.1f;
    }
    for (i = 0; i < 4; i++) {
        float *v;

        q.rgba[i] = ((alpha >> 1) << 24) | 0x808080;
        v = q.v[i];
        FUN_001f9cf8(v, D_001D97D0[D_0013E030.set][i], arg0 + 0xC0);
        FUN_001f9a10(v, v, arg0 + 0x10);
        v[2] = z;
    }
    func_001F7D30(&q, 0, 0);
}

extern __typeof__(FUN_0022df40) func_0022DF40 __attribute__((alias("FUN_0022df40")));
