typedef struct { float f[4]; } __attribute__((aligned(16))) V_28efc8;

extern float D_L00_001BD990_28efc8[8] __asm__("D_001D9890") __attribute__((section(".data")));
extern V_28efc8 D_L00_001BD9B0_28efc8[8] __asm__("D_001D98B0") __attribute__((section(".data")));
extern V_28efc8 D_L00_001BDA30_28efc8[2] __asm__("D_001D9930") __attribute__((section(".data")));
extern V_28efc8 D_L00_001BDA50_28efc8[2] __asm__("D_001D9950") __attribute__((section(".data")));
extern V_28efc8 D_L00_001BDA70_28efc8[10] __asm__("D_001D9970") __attribute__((section(".data")));
#include "rnc/rendering/level_render_state.h"
#include "sda.h"
#include "types.h"

typedef struct {
    V_28efc8 corner[4];
    unsigned int color[4];
    struct { float u, v; } uv[4];
    long unk70, tex, unk80, unk88;
} Q_28efc8;
extern int D_L00_00160580_28efc8[1] __asm__("D_001604C0") __attribute__((sda));
void FUN_001f9a68_28efc8(float, void *, void *) __asm__("FUN_001f9a68");
void FUN_001f9a10_28efc8(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9cf8_28efc8(void *, void *, void *) __asm__("FUN_001f9cf8");
long FUN_001f44b8_28efc8(int) __asm__("FUN_001f44b8");
int FUN_00213260_28efc8(int) __asm__("FUN_00213260");
float FUN_001fa6c0_28efc8(int) __asm__("func_001FA6C0");
void FUN_L00_001fd228_28efc8(void *, void *, int) __asm__("func_001F7D30");
void FUN_0022e1b0(unsigned char *m) {
    Q_28efc8 quad;
    V_28efc8 v;
    V_28efc8 *tbl;
    int i, j, k;
    tbl = D_L00_001BD9B0_28efc8;
    if (level_render_state.content_variant == 1) tbl = D_L00_001BDA30_28efc8;
    else if (level_render_state.content_variant == 2) tbl = D_L00_001BDA50_28efc8;
    quad.tex = FUN_001f44b8_28efc8(5);
    quad.unk80 = 0xFF9000000260L;
    quad.unk88 = 0x8000000048L;
    quad.unk70 = 0;
    for (j = 0; j < 4; j++) {
        quad.uv[j].u = ((float (*)[2])D_L00_001BD990_28efc8)[j][0];
        quad.uv[j].v = ((float (*)[2])D_L00_001BD990_28efc8)[j][1];
    }
    FUN_001f9a68_28efc8(0.0009765625f, &v, m);
    for (i = 0; i < D_L00_00160580_28efc8[level_render_state.content_variant]; i++) {
        int c = m[0xBC];
        unsigned int col;
        float s;
        if (*(short *)(m + 0xB2)) c += FUN_00213260_28efc8(*(short *)(m + 0xB2));
        s = FUN_001fa6c0_28efc8(c) * (tbl[i].f[3] / 40.0f);
        col = (c << 24) | 0x2058B0;
        if (*(short *)(m + 0xA6) == 0x215) col = (c << 24) | 0x308000;
        for (k = 0; k < 4; k++) {
            quad.color[k] = col;
            FUN_001f9a68_28efc8(s, &quad.corner[k], &D_L00_001BDA70_28efc8[k]);
            FUN_001f9a10_28efc8(&quad.corner[k], &quad.corner[k], &tbl[i]);
            FUN_001f9cf8_28efc8(&quad.corner[k], &quad.corner[k], (char *)level_render_state.player + 0xC0);
            FUN_001f9a10_28efc8(&quad.corner[k], &quad.corner[k], &v);
        }
        FUN_L00_001fd228_28efc8(&quad, 0, 0);
    }
}

extern __typeof__(FUN_0022e1b0) func_0022E1B0 __attribute__((alias("FUN_0022e1b0")));

float D_L00_001BD990_28efc8[8] = {0, 0, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f};

V_28efc8 D_L00_001BD9B0_28efc8[8] = {{{-3.3f, 1.85f, 0.13f, 0.7f}}, {{-3.3f, 1.42f, 0.42f, 0.7f}}, {{-3.3f, 1.42f, -0.15f, 0.7f}}, {{-3.3f, 0.8f, 1.6f, 0.7f}}, {{-3.3f, -1.85f, 0.13f, 0.7f}}, {{-3.3f, -1.42f, 0.42f, 0.7f}}, {{-3.3f, -1.42f, -0.15f, 0.7f}}, {{-3.3f, -0.8f, 1.6f, 0.7f}}};

V_28efc8 D_L00_001BDA30_28efc8[2] = {{{-4.75f, 1.75f, 0.5f, 1.2f}}, {{-4.75f, -1.75f, 0.5f, 1.2f}}};

V_28efc8 D_L00_001BDA50_28efc8[2] = {{{-5.7f, 0.9f, -0.4f, 1.15f}}, {{-5.7f, -0.9f, -0.4f, 1.15f}}};

V_28efc8 D_L00_001BDA70_28efc8[10] = {{{0.0f, -1.0f, 1.0f, 0.0f}}, {{0.0f, 1.0f, 1.0f, 0.0f}}, {{0.0f, -1.0f, -1.0f, 0.0f}}, {{0.0f, 1.0f, -1.0f, 0.0f}}, {{-3.5f, 1.5f, 1.2f, 0.0f}}, {{-3.5f, -1.5f, 1.2f, 0.0f}}, {{-3.5f, 1.8f, 2.6f, 0.0f}}, {{-3.5f, -1.8f, 2.6f, 0.0f}}, {{-3.5f, 1.2f, 1.2f, 0.0f}}, {{-3.5f, -1.2f, 1.2f, 0.0f}}};
