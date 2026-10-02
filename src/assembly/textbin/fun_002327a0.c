#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002327a0/FUN_002327a0.s", FUN_002327a0);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef float FVec4[4] __attribute__((aligned(16)));

typedef struct {
    u128 q;
} QWord;

typedef struct {
    s16 vert;
    s16 pad;
} QuadCorner;

typedef struct {
    QuadCorner c[4];
} Quad;

typedef struct {
    u8 pad0[0x10];
    float x;
    float y;
    u8 pad18[0x8E];
    s16 oClass;
} Moby;

typedef struct {
    u8 pad0[0x140];
    float x;
    float y;
} Player;

extern s32 D_0013E050[];
extern s32 D_0015F604;
extern s32 D_001604A4 __attribute__((sda));
extern s32 D_001604A8 __attribute__((sda));
extern s32 D_001604AC __attribute__((sda));
extern s32 D_00160520[2] __attribute__((sda));
extern s32 D_00160530[2] __attribute__((sda));
extern s32 D_00160540[2] __attribute__((sda));
extern QWord *D_00160550[2] __attribute__((sda));
extern QWord *D_00160560[2] __attribute__((sda));
extern Quad *D_00160570[2] __attribute__((sda));
extern Player D_00186F40;
extern FVec4 D_00187080;
extern QWord D_001DC4E0[];
extern float D_001DCB40[][2];
extern float D_001DCE70[][2];

extern unsigned long func_001F44B8(int);
extern void func_001F7D30(void *, int, int);
extern int func_001F96F8(int);
extern void func_001F9740(s32 *);
extern float func_001F9988(float);
extern float AbsoluteFloat(float) __asm__("func_001F99C0");
extern void func_001F9A28(void *, void *, void *);
extern void func_001F9A68(void *, void *, float);
extern float func_001F9AB0(void *, void *);
extern void func_001F9BF8(void *, void *, float);
extern void func_001F9D20(void *, void *, void *);
extern float func_001FA6C0(int);
extern void func_0020CCA8(Moby *, int, void *);

void FUN_002327a0(Moby *moby) {
    QWord m[4];
    int colors[4];
    float uv[4][2];
    unsigned long pkt[4];
    FVec4 mtx[4];
    FVec4 nrm;
    FVec4 refl;
    FVec4 dir;
    Quad *quads;
    QWord *verts;
    QWord *normals;
    int nquads;
    int nverts;
    int idx;
    int color;
    int flag;
    int tex;
    float t;
    float s;
    float x;
    float y;
    int i;
    int j;

    idx = moby->oClass - 0x212;
    verts = D_00160560[idx];
    normals = D_00160550[idx];
    quads = D_00160570[idx];
    nquads = D_00160540[idx];
    nverts = D_00160530[idx];
    if (D_0015F604 == 6 && D_0013E050[0] == 4) {
        pkt[1] = func_001F44B8(1);
    } else {
        pkt[1] = func_001F44B8(0x15);
    }
    flag = 0;
    color = D_00160520[idx];
    pkt[2] = 0xFF9000000260;
    pkt[3] = 0x8000000044;
    pkt[0] = 0;
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    func_0020CCA8(moby, 0, mtx);
    if (D_0015F604 != 0 ||
        (AbsoluteFloat(D_00186F40.x - moby->x) < 16.0f && AbsoluteFloat(D_00186F40.y - moby->y) < 16.0f)) {
        flag = 1;
    }
    if (D_0015F604 == 6 && D_0013E050[0] == 4) {
        flag = 0;
    }
    if (flag != 0 || D_001604A4 == 1) {
        D_001604A8 = 1;
        func_001F9740(&D_001604AC);
        t = func_001FA6C0(D_001604AC) / func_001FA6C0(func_001F96F8(0x3C));
        for (i = 0; i < nverts; i++) {
            func_001F9D20(&D_001DC4E0[i], &verts[i], mtx);
            func_001F9A28(dir, &D_001DC4E0[i], D_00187080);
            func_001F9BF8(dir, dir, 1.0f);
            func_001F9D20(nrm, &normals[i], mtx);
            func_001F9BF8(nrm, nrm, 0.1f);
            func_001F9A68(refl, nrm, func_001F9AB0(nrm, dir) * 2.0f);
            func_001F9A28(refl, dir, refl);
            func_001F9BF8(refl, refl, 1.0f);
            refl[2] += 1.0f;
            s = func_001F9988(refl[2] * 2.0f) * 2.0f;
            if (D_001604A4 == 1 || D_001604AC == 0) {
                D_001DCB40[i][0] = refl[0] / s + 0.5f;
                D_001DCB40[i][1] = refl[1] / s + 0.5f;
            } else {
                x = refl[0] / s + 0.5f;
                D_001DCB40[i][0] = x + (D_001DCE70[i][0] - x) * t;
                y = refl[1] / s + 0.5f;
                D_001DCB40[i][1] = y + (D_001DCE70[i][1] - y) * t;
            }
        }
        if (D_001604A4 == 1) {
            D_001604A4 = 2;
        }
    } else {
        if (D_001604A8 == 1) {
            D_001604A8 = 0;
            for (i = 0; i < nverts; i++) {
                D_001DCE70[i][0] = D_001DCB40[i][0];
                D_001DCE70[i][1] = D_001DCB40[i][1];
                func_001F9D20(&D_001DC4E0[i], &verts[i], mtx);
            }
        } else {
            for (i = 0; i < nverts; i++) {
                func_001F9D20(&D_001DC4E0[i], &verts[i], mtx);
            }
        }
        D_001604AC = func_001F96F8(0x3C);
    }
    for (i = 0; i < nquads; i++) {
        for (j = 0; j < 4; j++) {
            int k = quads[i].c[j].vert;

            qcopy(&m[j], &D_001DC4E0[k]);
            uv[j][0] = D_001DCB40[k][0];
            uv[j][1] = D_001DCB40[k][1];
        }
        func_001F7D30(m, 0, 0);
    }
}
#endif /* NON_MATCHING */
