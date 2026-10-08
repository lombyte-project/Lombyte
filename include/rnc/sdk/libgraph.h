#ifndef LOMBYTE_RNC_SDK_LIBGRAPH_H
#define LOMBYTE_RNC_SDK_LIBGRAPH_H

#include "types.h"

/* GIF tag (128 bits): NLOOP data loops, EOP ends the packet, FLG selects
   PACKED/REGLIST/IMAGE, NREG registers listed in REGS0..15 (0xE = A+D). */
struct GifTag {
    u64 NLOOP : 15;
    u64 EOP : 1;
    u64 pad16 : 16;
    u64 id : 14;
    u64 PRE : 1;
    u64 PRIM : 11;
    u64 FLG : 2;
    u64 NREG : 4;
    u64 REGS0 : 4;
    u64 REGS1 : 4;
    u64 REGS2 : 4;
    u64 REGS3 : 4;
    u64 REGS4 : 4;
    u64 REGS5 : 4;
    u64 REGS6 : 4;
    u64 REGS7 : 4;
    u64 REGS8 : 4;
    u64 REGS9 : 4;
    u64 REGS10 : 4;
    u64 REGS11 : 4;
    u64 REGS12 : 4;
    u64 REGS13 : 4;
    u64 REGS14 : 4;
    u64 REGS15 : 4;
};

/* GS DISPFB register. */
struct GsDispfb {
    u64 FBP : 9;
    u64 FBW : 6;
    u64 PSM : 5;
    u64 p0 : 12;
    u64 DBX : 11;
    u64 DBY : 11;
    u64 p1 : 10;
};

/* GS FRAME register. */
struct GsFrame {
    u64 FBP : 9;
    u64 p0 : 7;
    u64 FBW : 6;
    u64 p1 : 2;
    u64 PSM : 6;
    u64 p2 : 2;
    u64 FBMSK : 32;
};

/* Display registers set by sceGsSetDefDispEnv and written to the GS by
   sceGsPutDispEnv (PMODE, SMODE2, DISPFB, DISPLAY, BGCOLOR). */
struct sceGsDispEnv {
    u64 pmode;
    u64 smode2;
    u64 dispfb;
    u64 display;
    u64 bgcolor;
};

/* Drawing context 1 registers as A+D pairs (value, register address),
   filled by sceGsSetDefDrawEnv. */
struct sceGsDrawEnv1 {
    struct GsFrame frame1;
    u64 frame1addr;
    u64 zbuf1;
    u64 zbuf1addr;
    u64 xyoffset1;
    u64 xyoffset1addr;
    u64 scissor1;
    u64 scissor1addr;
    u64 prmodecont;
    u64 prmodecontaddr;
    u64 colclamp;
    u64 colclampaddr;
    u64 dthe;
    u64 dtheaddr;
    u64 test1;
    u64 test1addr;
};

/* Image upload packet built by sceGsSetDefLoadImage and sent by
   sceGsExecLoadImage: a GIF tag (q[0..1]), BITBLTBUF, TRXPOS, TRXREG and
   TRXDIR with their register addresses (q[2..9]), then the IMAGE GIF tag
   of the pixel data (q[10..11]). */
typedef struct {
    u64 q[12];
} sceGsLoadImage __attribute__((aligned(16)));

#endif /* LOMBYTE_RNC_SDK_LIBGRAPH_H */
