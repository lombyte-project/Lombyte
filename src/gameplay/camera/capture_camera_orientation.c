/* Ported from rac1-decomp (src/game/camera.c, func_001ECAB8). */
#include "qcopy.h"
#include "rnc/gameplay/hero.h"
extern void FUN_001f9bf8(void *dst, void *src, float len); /* dst = normalize(src) * len */
extern void compute_camera_angles(float *out, void *p0, void *p1, void *dir0, void *dir1,
                                  void *axis) __asm__("func_001EC530");
extern char D_001871B0[];
/* Builds three unit vectors from D_0013F350's +0x2080 pointer table
   (+0xC0/+0xD0/+0xE0 offsets, re-read at each call as retail does),
   stashes two of them into D_001871B0's record (+0x90, +0xA0), calls
   func_001EC530 to compute the camera's yaw/pitch/dist into +0x70,
   then copies +0xD0 back over +0xB0 (retail's qcopy, see common.h). */
void capture_camera_orientation(void) __asm__("FUN_001ec710");

void capture_camera_orientation(void) {
    struct Hero *g = &hero;
    char *r = D_001871B0;
    char local0[16];
    char local1[16];
    char local2[16];

    FUN_001f9bf8(local0, (char *)g->moby + 0xC0, 1.0f);
    FUN_001f9bf8(local1, (char *)g->moby + 0xD0, 1.0f);
    FUN_001f9bf8(local2, (char *)g->moby + 0xE0, 1.0f);

    qcopy(r + 0x90, local0);
    qcopy(r + 0xA0, local2);

    compute_camera_angles((float *)(r + 0x70), r + 0xC0, (char *)&g->motion.pos, local0, local1, local2);

    qcopy(r + 0xB0, r + 0xD0);
}
extern void FUN_001f9bf8(void *dst, void *src, float len);

extern __typeof__(capture_camera_orientation) func_001EC710 __attribute__((alias("FUN_001ec710")));
