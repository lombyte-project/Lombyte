#ifndef LOMBYTE_RNC_RENDERING_DEPTH_TEST_PACKETS_H
#define LOMBYTE_RNC_RENDERING_DEPTH_TEST_PACKETS_H

#include "types.h"
#include "rnc/sdk/libgraph.h"

/* GS register addresses the packets write through A+D. */
#define GS_REG_TEST_1   0x47
#define GS_REG_TEXFLUSH 0x3F

/* GIF tag (low word) fields. */
#define GIF_TAG_NLOOP(n) ((u64)(n))
#define GIF_TAG_EOP      ((u64)1 << 15)
#define GIF_TAG_NREG(n)  ((u64)(n) << 60)

/* TEST_1 fields the packets set: alpha test on with ATST NEVER and AFAIL
   FB_ONLY, depth test on, ZTST in bits 17-18. */
#define GS_TEST_ATE          0x1
#define GS_TEST_AFAIL_FB     0x1000
#define GS_TEST_ZTE          0x10000
#define GS_TEST_ZTST(method) ((u64)(method) << 17)
#define GS_ZTST_NEVER        0
#define GS_ZTST_ALWAYS       1
#define GS_ZTST_GEQUAL       2
#define GS_ZTST_GREATER      3

/* A GS packet of two A+D writes: TEST_1, then TEXFLUSH. emit_selected_
   graphics_dma_packets sends one by reference; entry i tests depth with
   ZTST i (NEVER, ALWAYS, GEQUAL, GREATER). */
struct GsDepthTestPacket {
    u64 tag[2]; /* GIF tag: PACKED, NREG 1, REGS A+D */
    u64 test;
    u64 test_reg;
    u64 texflush;
    u64 texflush_reg;
};

extern struct GsDepthTestPacket gs_depth_test_packets[4] __asm__("D_001D7D90");

#endif /* LOMBYTE_RNC_RENDERING_DEPTH_TEST_PACKETS_H */
