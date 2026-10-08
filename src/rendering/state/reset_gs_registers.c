#include "types.h"
#include "rnc/rendering/dma_tag.h"
struct GifTag {
    u32 w0;
    u32 addr;
    u32 w2;
    u32 w3;
};
struct Disp {
    u8 pad0[0x230];
    s32 a;
    s32 b;
    s32 c;
};
extern u8 D_0013CFC0[];
extern u8 D_0013CF10[];
extern struct Disp D_0018CD00;
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void reset_gs_registers(void) __asm__("FUN_001f3868");

void reset_gs_registers(void) {
    struct GifTag *p;

    render_packet_cursor.gif->w0 = 0x30000013;
    render_packet_cursor.gif->addr = (u32)D_0013CFC0;
    render_packet_cursor.gif->w2 = 0;
    render_packet_cursor.gif->w3 = 0x50000013;
    p = render_packet_cursor.gif;
    render_packet_cursor.gif = p + 1;
    p[1].w0 = 0x3000000B;
    render_packet_cursor.gif->addr = (u32)D_0013CF10;
    render_packet_cursor.gif->w2 = 0;
    render_packet_cursor.gif->w3 = 0x5000000B;
    render_packet_cursor.gif++;
    vu1_add_g_sregister(0x3D, D_0018CD00.a | ((u64)D_0018CD00.b << 8) | ((u64)D_0018CD00.c << 16));
}

extern __typeof__(reset_gs_registers) func_001F3868 __attribute__((alias("FUN_001f3868")));
