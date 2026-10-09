#include "types.h"
#include "rnc/rendering/dma_tag.h"

#include "rnc/rendering/fs_aa_packets.h"
extern s32 D_0015F438;
extern void put_draw_buffer_large(void) __asm__("func_001FB2D0");
extern void append_gif_transfer_packet(void) __asm__("func_001FB368");
extern void emit_rgba_draw_packet(s32, s32, s32, s32) __asm__("func_001F5210");
extern void put_draw_buffer_small(void) __asm__("func_001FB3D0");
extern void vu1_init_chain(void) __asm__("func_002335D0");
extern void swap_render_buffer_chain(void) __asm__("func_00233630");
extern void vu1_send_chain(void) __asm__("func_002336A0");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern s32 sceGsSyncV(s32);

#define VP (*(u8 *volatile *)&render_packet_cursor.bytes)

void fade_to_black(s32 n) __asm__("FUN_001f4a58");

void fade_to_black(s32 n) {
    s32 i;

    vu1_sync_chain(1);
    sceGsSyncV(0);
    D_0015F438 += 1;
    vu1_init_chain();
    for (i = n - 1; i >= 0; i--) {
        put_draw_buffer_large();
        append_gif_transfer_packet();
        emit_rgba_draw_packet(0, 0, 0, 0x80);
        put_draw_buffer_small();
        vu1_add_g_sregister(1, (u64)(0x80 - (i * 0x80) / (i + 1)) << 24);
        *(u32 *)(VP + 0) = 0x30000014;
        *(u32 *)(VP + 4) = (u32)second_clear_packet;
        *(u32 *)(VP + 8) = 0;
        *(u32 *)(VP + 12) = 0x50000014;
        render_packet_cursor.bytes = VP + 0x10;
        vu1_sync_chain(1);
        sceGsSyncV(0);
        D_0015F438 += 1;
        vu1_send_chain();
        swap_render_buffer_chain();
    }
    vu1_sync_chain(1);
    sceGsSyncV(0);
    D_0015F438 += 1;
    vu1_init_chain();
    put_draw_buffer_large();
    append_gif_transfer_packet();
}

extern __typeof__(VP) func_001F4A58 __attribute__((alias("FUN_001f4a58")));
