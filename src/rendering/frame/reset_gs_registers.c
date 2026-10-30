#include "types.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/view.h"
#include "rnc/rendering/draw_environment.h"

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void        reset_gs_registers(void) __asm__("FUN_001f3868");

void reset_gs_registers(void) {
    struct DmaTag *initial_packet_tag;

    render_packet_cursor.tag->tag = 0x30000000 | RESET_GS_REGS_QWC;
    render_packet_cursor.tag->addr = (u32)reset_gs_regs_fixed;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000000 | RESET_GS_REGS_QWC;
    initial_packet_tag = render_packet_cursor.tag;
    render_packet_cursor.tag = initial_packet_tag + 1;
    initial_packet_tag[1].tag = 0x3000000B;
    render_packet_cursor.tag->addr = (u32)&draw_environment;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x5000000B;
    render_packet_cursor.tag++;
    vu1_add_g_sregister(0x3D,
                        view_context.fog_r | ((u64)view_context.fog_g << 8) |
                            ((u64)view_context.fog_b << 16));
}

extern __typeof__(reset_gs_registers) func_001F3868 __attribute__((alias("FUN_001f3868")));

u64 reset_gs_regs_fixed[RESET_GS_REGS_QWC][2] = {
    {0x1000000000008012ULL, 0x000000000000000EULL}, /* GIF tag: NLOOP 0x12, EOP, one A+D register */
    {0x0000008000000044ULL,               0x42ULL}, /* ALPHA_1 */
    {0x0000008000000044ULL,               0x43ULL}, /* ALPHA_2 */
    {0x0000000000000000ULL,               0x08ULL}, /* CLAMP_1 */
    {0x0000000000000000ULL,               0x09ULL}, /* CLAMP_2 */
    {0x0000000000000001ULL,               0x46ULL}, /* COLCLAMP */
    {0x0000000000000000ULL,               0x44ULL}, /* DIMX */
    {0x0000000000000000ULL,               0x45ULL}, /* DTHE */
    {0x0000000000000000ULL,               0x4AULL}, /* FBA_1 */
    {0x0000000000000000ULL,               0x4BULL}, /* FBA_2 */
    {0x0000000000000000ULL,               0x49ULL}, /* PABE */
    {0x0000000000000000ULL,               0x1BULL}, /* PRMODE */
    {0x0000000000000001ULL,               0x1AULL}, /* PRMODECONT */
    {0x0000000000000000ULL,               0x22ULL}, /* SCANMSK */
    {0x000000000005360BULL,               0x47ULL}, /* TEST_1 */
    {0x000000000005360BULL,               0x48ULL}, /* TEST_2 */
    {0x0000008000000000ULL,               0x3BULL}, /* TEXA */
    {0x0000000000000001ULL,               0x1CULL}, /* TEXCLUT */
    {0x0000000000000001ULL,               0x3FULL}, /* TEXFLUSH */
};
