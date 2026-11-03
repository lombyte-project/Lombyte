#include "types.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"

/* A GIF tag and two A+D register writes: the 3 quadwords the DMA tag below sends. */
extern u64 normal_gs_regs_dma_payload[3][2] __asm__("D_001DE3C0");
void       vu1_gs_regs_normal(void) __asm__("FUN_00233bc8");

void vu1_gs_regs_normal(void) {
    *render_packet_cursor.words = 0x30000003;
    *(s32 *)((u32)render_packet_cursor.words + 4) = (s32)normal_gs_regs_dma_payload;
    *(s32 *)((u32)render_packet_cursor.words + 8) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 12) = 0x50000003;
    render_packet_cursor.words += 4;
}

extern __typeof__(vu1_gs_regs_normal) func_00233BC8 __attribute__((alias("FUN_00233bc8")));

u64 normal_gs_regs_dma_payload[3][2] = {
    {0x2000000000008001ULL, 0xEEULL}, /* GIF tag: NLOOP 1, EOP, two A+D registers */
    {0x000000000005360BULL, 0x47ULL}, /* TEST_1 */
    {0x0000000000000044ULL, 0x42ULL}, /* ALPHA_1 */
};
