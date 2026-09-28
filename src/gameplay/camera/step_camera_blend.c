/* Ported from rac1-decomp, the PAL decompilation (src/game/camera.c, func_001ECEA0). */
#include "sda.h"
#include "qcopy.h"
extern void FUN_001f9a10(void *, void *, void *);
extern char D_0013F490[];
extern char D_00187080[];
extern char D_0018C318[];
extern char D_00187290[];
extern float D_0015ED60 MACRO_ADDR;
extern float FUN_002133d0(float, float, float);
extern void func_002144D8(void *, void *);
extern void FUN_001fa400(void *, void *, void *, float);
extern void FUN_001fa4f8(void *, void *);
extern void func_001FA2B8(void *, void *);
/* Camera blend step toward to: while the position (cam[3]) or rotation
   (cam[0]) blend hasn't reached 1, move cam+0x40 from cam+0x30 toward
   to's position by the eased factor (FUN_002133d0), copy it to
   D_00187080 unless the D_0018C318 flag is set, slerp the rotation
   (FUN_001fa400) into cam+0x50 and load it as the view matrix, then
   advance both blends by their rates times D_0015ED60, capped at 1.
   Returns 1 once both are complete. */
int step_camera_blend(void *arg0, void *arg1) __asm__("FUN_001ecaf8");

int step_camera_blend(void *arg0, void *arg1) {
    float *to = arg0;
    float *cam = arg1;
    float m[4];
    float q[16];
    char *st;
    float t;
    float *rot;

    if (cam[3] == 1.0f && cam[0] == 1.0f) {
        return 1;
    }
    t = FUN_002133d0(0.0f, 1.0f, cam[3]);
    FUN_001f9a10(cam + 12, D_0013F490, cam + 12);
    st = D_0018C318;
    cam[16] = cam[12] + (to[12] - cam[12]) * t;
    cam[17] = cam[13] + (to[13] - cam[13]) * t;
    cam[18] = cam[14] + (to[14] - cam[14]) * t;
    if (*(int *)(st + 0x14) == 0) {
        qcopy(D_00187080, cam + 16);
    }
    func_002144D8(m, to);
    rot = cam + 20;
    t = FUN_002133d0(0.0f, 1.0f, cam[0]);
    FUN_001fa400(rot, cam + 8, m, t);
    FUN_001fa4f8(rot, q);
    if (*(int *)(st + 0x14) == 0) {
        func_001FA2B8(D_00187290, q);
    }
    cam[3] += cam[4] * D_0015ED60;
    if (1.0f < cam[3]) {
        cam[3] = 1.0f;
    }
    cam[0] += cam[1] * D_0015ED60;
    if (1.0f < cam[0]) {
        cam[0] = 1.0f;
    }
    return 0;
}
extern void FUN_001f9a10(void *dst, void *a, void *b);

extern __typeof__(step_camera_blend) func_001ECAF8 __attribute__((alias("FUN_001ecaf8")));
