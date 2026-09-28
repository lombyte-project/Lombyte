/* Ported from rac1-decomp, the PAL decompilation (src/game/camera.c, func_001ECC48). */
#include "sda.h"
#include "qcopy.h"
extern char D_0013F350[];
extern void FUN_001f9bf8(void *dst, void *src, float len);    /* dst = normalize(src) * len */
extern void func_001EC530(float *out, void *p0, void *p1, void *dir0, void *dir1,
                           void *axis);
extern char D_001871B0[];
extern short D_001871B0_h __asm__("D_001871B0") NOT_SDA;
extern void func_002144D8(void *, void *);
extern int FUN_001f96f8(int);
extern float func_001FA6C0(int);
/* Camera mode update from arg (its +0x30 vector is copied). In mode 1
   the sub-mode at +3 picks what is captured from it: 0 the +0x50/+0x60
   pair, 2 the +0xC0/+0xD0 pair and func_001EC710's angles, otherwise
   D_0013F350's axes and func_001EC530's yaw/pitch/dist into +0x70. Other
   modes restore through func_001EC7F0/func_001EC868. Then the mode
   becomes 3, +2 takes the sub-mode, and either the +0x10 blend resets or
   the +0x70 timer advances. */
void start_camera_blend(void *arg) __asm__("FUN_001ec8a0");

void start_camera_blend(void *arg) {
    char *arg0 = arg;
    char *r = D_001871B0;
    char local0[16];
    char local1[16];
    char local2[16];
    unsigned char sub;

    if (*(short *)r == 1) {
        sub = r[3];
        if (sub == 0) {
            qcopy(r + 0x50, arg0 + 0x30);
            func_002144D8(r + 0x60, arg0);
        } else if (sub == 2) {
            qcopy(r + 0xC0, arg0 + 0x30);
            func_002144D8(r + 0xD0, arg0);
            func_001EC710();
        } else {
            char *g = D_0013F350;

            FUN_001f9bf8(local0, *(char **)(g + 0x2080) + 0xC0, 1.0f);
            FUN_001f9bf8(local1, *(char **)(g + 0x2080) + 0xD0, 1.0f);
            FUN_001f9bf8(local2, *(char **)(g + 0x2080) + 0xE0, 1.0f);
            func_001EC530((float *)(r + 0x70), arg0 + 0x30, *(char **)(r - 0xF0) + 0x30,
                          local0, local1, local2);
            func_002144D8(r + 0xB0, arg0);
            qcopy(r + 0xD0, r + 0xB0);
        }
    } else {
        sub = r[3];
        if (sub == 2) {
            func_001EC7F0();
            func_001EC710();
        } else if (sub == 1) {
            func_001EC7F0();
            qcopy(r + 0xB0, r + 0xD0);
        } else if (sub == 0) {
            func_001EC868();
        }
    }
    D_001871B0_h = 3;
    *(unsigned char *)(r + 2) = r[3];
    if (*(unsigned char *)(r + 2) == 0) {
        char *a = r + 0x10;

        *(float *)(a + 0x10) = *(float *)(a + 0x14);
        *(int *)(a + 0xC) = 0;
        qcopy(r + 0x40, r + 0x50);
        *(float *)(a + 0x0) = 0;
        *(float *)(a + 0x4) = *(float *)(a + 0x8);
        qcopy(r + 0x30, r + 0x60);
    } else {
        char *b = r + 0x70;
        int n = ++*(int *)(b + 0x14);

        *(int *)(b + 0xC) = FUN_001f96f8(n);
        *(float *)(b + 0x10) = 1.0f / func_001FA6C0(*(int *)(b + 0xC));
    }
}
extern float func_001FA6C0(int arg0);
extern void FUN_001f9bf8(void *dst, void *src, float len);

extern __typeof__(start_camera_blend) func_001EC8A0 __attribute__((alias("FUN_001ec8a0")));
