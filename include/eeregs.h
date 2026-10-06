#ifndef LOMBYTE_EEREGS_H
#define LOMBYTE_EEREGS_H

#include "types.h"

/* EE register addresses used by Sony's eeregs/libdma conventions. */
#define REG_DMAC_CTRL ((vu32 *)0x1000e000)
#define REG_DMAC_STAT ((vu32 *)0x1000e010)

#define D3_CHCR ((vu32 *)0x1000b000)
#define D3_MADR ((vu32 *)0x1000b010)
#define D3_QWC  ((vu32 *)0x1000b020)

#define D4_CHCR ((vu32 *)0x1000b400)
#define D4_MADR ((vu32 *)0x1000b410)
#define D4_QWC  ((vu32 *)0x1000b420)
#define D4_TADR ((vu32 *)0x1000b430)

#define GS_CSR ((vu64 *)0x12001000)

#define VU0_MICRO_ADDR ((u32)0x11000000)
#define VU0_MEM_ADDR   ((u32)0x11004000)
#define VU1_MICRO_ADDR ((u32)0x11008000)
#define VU1_MEM_ADDR   ((u32)0x1100c000)

#endif /* LOMBYTE_EEREGS_H */
