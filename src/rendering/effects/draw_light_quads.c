/* Ported from rac1-decomp (src/game/draw.c, func_001F4C30). */

#include "sda.h"
#include "qcopy.h"

extern int D_0015F474 MACRO_ADDR;

typedef float FVec4[4] __attribute__((aligned(16)));

typedef struct {
    FVec4 v;
} FRow;

typedef struct {
    FRow pos;
    FRow dir;
} LightRec;

extern FRow D_0018CAA0[4];
extern LightRec D_0018E340_l[] __asm__("D_0018E340");
extern int get_effect_texture(int) __asm__("FUN_001f44b8");
extern void FUN_001f9bf8(void *dst, void *src, float len);
extern void func_001F7D30(void *, int, int);

/* Builds a 4-row matrix per light in D_0018E340 (count D_0015F474):
   each row starts as the light's position and is pushed along the
   corner D_0018CAA0[k], projected off the normalised light direction,
   scaled by the position's w; func_001F7D30 draws it. The packet header,
   colours and UVs are filled on the stack but never sent. Both copies
   are qcopy (retail's lq/sq stay inside the loop), and the inner loop
   needs its own counter, not the one the setup loop used. */
void draw_light_quads(void) __asm__("FUN_001f4880");

void draw_light_quads(void) {
    FRow m[4];
    int colors[4];
    float uv[4][2];
    unsigned long pkt[4];
    FRow dir;
    int i;
    int j;

    pkt[1] = get_effect_texture(0);
    pkt[2] = 0xFF9000000260;
    pkt[0] = 5;
    pkt[3] = 0x8000000044;
    for (j = 0; j < 4; j++) {
        uv[j][0] = D_0018CAA0[j].v[2];
        uv[j][1] = D_0018CAA0[j].v[3];
        colors[j] = 0x40808080;
    }
    for (i = 0; i < D_0015F474; i++) {
        float s;
        int k;

        qcopy(&dir, &D_0018E340_l[i].dir);
        FUN_001f9bf8(&dir, &dir, 1.0f);
        s = D_0018E340_l[i].pos.v[3];
        for (k = 0; k < 4; k++) {
            float cx = D_0018CAA0[k].v[0];
            float vx = dir.v[0];
            float cy = D_0018CAA0[k].v[1];
            float d = cx * vx + cy * dir.v[1];

            qcopy(&m[k], &D_0018E340_l[i].pos);
            m[k].v[0] += (cx - vx * d) * s;
            m[k].v[1] += (cy - dir.v[1] * d) * s;
            m[k].v[2] -= dir.v[2] * d * s;
        }
        func_001F7D30(m, 0, 0);
    }
}

extern __typeof__(draw_light_quads) func_001F4880 __attribute__((alias("FUN_001f4880")));
