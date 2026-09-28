/* Ported from rac1-decomp, the PAL decompilation (src/game/space.c, func_0022F128). */

#include "sda.h"
#include "qcopy.h"

extern void FUN_001f2d98(void);
extern char D_00187080[];
extern char D_0018CB20_c[] __asm__("D_0018CB20");
extern float D_0018CDB0 NOT_SDA;
extern unsigned char D_0015EDB4_m[4] __asm__("D_0015EDB4") MACRO_ADDR;
extern void sceVu0UnitMatrix(float *);
extern void SceVu0RotMatrixX(float *, float *, float);
extern void SceVu0RotMatrixY(float *, float *, float);
extern void sceVu0RotMatrixZ(float *, float *, float);
extern void func_001F9AD8(void *, void *, void *);
unsigned char build_object_rotation_matrix(void) __asm__("FUN_0022de10");

unsigned char build_object_rotation_matrix(void) {
    char *t = D_0018CB20_c;
    char *key = *(char **)(t + 0x54) + *(int *)(t + 0x38) * 32;
    unsigned char flag = key[0xC];
    float *ang = (float *)(key + 0x10);
    char *pos;
    char *cam;
    float m[16];

    D_0018CDB0 = ang[3];
    FUN_001f2d98();
    pos = D_00187080;
    qcopy(pos, key);
    sceVu0UnitMatrix(m);
    SceVu0RotMatrixX(m, m, *(float *)(key + 0x10));
    SceVu0RotMatrixY(m, m, ang[1]);
    sceVu0RotMatrixZ(m, m, ang[2]);
    cam = pos - 0x140;
    *(float *)(cam + 0x350) = -m[8];
    *(float *)(cam + 0x360) = -m[0];
    *(float *)(cam + 0x370) = m[4];
    *(float *)(cam + 0x354) = -m[9];
    *(float *)(cam + 0x364) = -m[1];
    *(float *)(cam + 0x374) = m[5];
    *(float *)(cam + 0x358) = -m[10];
    *(float *)(cam + 0x368) = -m[2];
    *(float *)(cam + 0x378) = m[6];
    if (D_0015EDB4_m[0] != 0) {
        func_001F9AD8(pos + 0x220, pos + 0x230, pos + 0x210);
    }
    return flag;
}

extern __typeof__(build_object_rotation_matrix) func_0022DE10 __attribute__((alias("FUN_0022de10")));
