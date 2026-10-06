#include "types.h"

struct sceGsDrawEnv1 {
    unsigned long frame1;
    unsigned long frame1addr;
    unsigned long zbuf1;
    unsigned long zbuf1addr;
    unsigned long xyoffset1;
    unsigned long xyoffset1addr;
    unsigned long scissor1;
    unsigned long scissor1addr;
    unsigned long prmodecont;
    unsigned long prmodecontaddr;
    unsigned long colclamp;
    unsigned long colclampaddr;
    unsigned long dthe;
    unsigned long dtheaddr;
    unsigned long test1;
    unsigned long test1addr;
};

#define M2C_SYNC() __asm__ __volatile__("sync")

extern s16 sceGszbufaddr(s16 psm, s16 w, s16 h);

s32 sceGsSetDefDrawEnv(struct sceGsDrawEnv1 *draw, s16 psm, s16 w, s16 h, s16 ztest, s16 zpsm) {
    s16 fbw;
    s16 zbp;
    long ofx;
    long ofy;

    fbw = ((w + 63) >> 6) & 0x3F;
    draw->frame1addr = 0x4C;
    draw->frame1 = ((unsigned long)fbw << 16) | ((unsigned long)(psm & 0xF) << 24);
    draw->zbuf1addr = 0x4E;
    if (ztest == 0) {
        zbp = sceGszbufaddr(psm, w, h);
        draw->zbuf1 =
            (unsigned long)zbp | ((unsigned long)(zpsm & 0xF) << 24) | ((unsigned long)1 << 32);
    } else {
        zbp = sceGszbufaddr(psm, w, h);
        draw->zbuf1 = (unsigned long)zbp | ((unsigned long)(zpsm & 0xF) << 24);
    }
    draw->xyoffset1addr = 0x18;
    ofx = 2048L - (w >> 1);
    ofy = 2048L - (h >> 1);
    draw->xyoffset1 = ((unsigned long)(ofx << 4)) | (((unsigned long)(ofy << 4)) << 32);
    draw->scissor1addr = 0x40;
    draw->scissor1 = ((unsigned long)(w - 1) << 16) | ((unsigned long)(h - 1) << 48);
    draw->prmodecontaddr = 0x1A;
    draw->prmodecont |= 1;
    draw->colclampaddr = 0x46;
    draw->colclamp |= 1;
    draw->dtheaddr = 0x45;
    if (psm & 2) {
        draw->dthe |= 1;
    } else {
        draw->dthe &= ~1ULL;
    }
    draw->test1addr = 0x47;
    if (ztest != 0) {
        draw->test1 = ((unsigned long)(ztest & 3) << 17) | 0x10000;
    } else {
        draw->test1 = 0x30000;
    }
    M2C_SYNC();
    return 8;
}
