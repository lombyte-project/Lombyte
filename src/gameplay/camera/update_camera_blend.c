/* Ported from rac1-decomp, the PAL decompilation (src/game/camera.c, func_001ED658). */

#include "sda.h"
#include "qcopy.h"

extern char D_001871B0[];
extern char D_00187290[];
extern int D_0018C32C NOT_SDA;
extern int func_001ECAF8(void *, void *);
extern int FUN_001eccd8(void *, void *);

/* Picks the update path by the flag at D_001871B0+2 (func_001ECAF8 when
   clear, FUN_001eccd8 when set), each passed arg0 and a slot inside
   D_001871B0. On success, unless D_0018C32C is set, copies four 16-byte
   vectors from arg0 into D_00187290's block (retail's qcopy); the last
   destination is -0x210 from D_00187290, a different member reached by
   pointer arithmetic on the same char array. Either way it clears
   D_001871B0's leading halfword and its flag byte. */
void update_camera_blend(char *arg0) __asm__("FUN_001ed2b0");

void update_camera_blend(char *arg0) {
    char *base = D_001871B0;
    int result;

    if (*(unsigned char *)(base + 2) == 0) {
        result = func_001ECAF8(arg0, base + 0x10);
    } else {
        result = FUN_001eccd8(arg0, base + 0x70);
    }
    if (result != 0) {
        if (D_0018C32C == 0) {
            qcopy(D_00187290, arg0);
            qcopy(D_00187290 + 0x10, arg0 + 0x10);
            qcopy(D_00187290 + 0x20, arg0 + 0x20);
            qcopy(D_00187290 - 0x210, arg0 + 0x30);
        }
        *(short *)base = 0;
        base[2] = 0;
    }
}

extern __typeof__(update_camera_blend) func_001ED2B0 __attribute__((alias("FUN_001ed2b0")));
