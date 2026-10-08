#include "types.h"
#include "rnc/rendering/draw_config.h"
#include "rnc/globals.h"
#include "rnc/rendering/dma_tag.h"

struct Locals {
    u8 pad0[0x30];
    s32 v30;
    u8 pad34[8];
    f32 v3C;
};

extern u8 D_00160E70[];
extern u8 D_00160E80[];
extern s32 D_00160EBC;
extern u8 D_00187080[];
extern u8 D_001E1300[];
extern void FlushCache(s32);
extern void WriteDmaChannel(u32, u32, u32);
extern void func_001F21B0(void *, s32);
extern void func_001F21B8(void *, s32);
extern void FUN_001f9a68(void *, void *, f32);
extern void FUN_001f9fc8(void *);
extern void FUN_001fa378(void *, void *, void *);
extern void dma_tfrag_textures(void) __asm__("func_002331C0");
extern void func_00233FB0(void);
void write_vif_unpack_packet(s32 addr, void *src, s32 qwc) __asm__("FUN_00233888");
/* Draws the level terrain (tfrags). Each tfrag has a 0x40-byte header:
 *   0x10  where its data starts
 *   0x16, 0x18, 0x1a  where its data lists start (shared, lower detail, full detail)
 *   0x1e  vertex colours: one RGBA colour per vertex, 0x80 means full brightness
 *   0x3d  number of triangles at full detail
 * Texture coordinates are 16-bit numbers that the hardware adds to 2048.0, so
 * negative values come out at half size. The strips list how to join the
 * vertices into triangles and when to switch to the next texture. */
void draw_tfrag(void) __asm__("FUN_002333a8");

void draw_tfrag(void) {
    struct Locals L;
    s32 packet;
    u8 *p;

    packet = render_packet_cursor.addr;
    D_00160EBC = packet;
    gs_texture_allocation_cursor = gs_texture_allocation_start;
    packet += 0x10;
    render_packet_cursor.addr = packet;
    func_001F21B8(D_00160E70, 1);
    FUN_001f9fc8(&L);
    p = D_00187080;
    FUN_001f9a68(&L.v30, p, -1024.0f);
    L.v3C = 1.0f;
    FUN_001fa378(&L, p - 0x100, &L);
    write_vif_unpack_packet(5, &L, 4);
    write_vif_unpack_packet(0x14D, &L, 4);
    if (draw_config.tfrag.enabled != 0) {
        FlushCache(0);
        func_00233FB0();
    }
    func_001F21B8(D_00160E80, 2);
    dma_tfrag_textures();
    if (draw_config.tfrag.enabled != 0) {
        WriteDmaChannel(D_001E1300, 0x3000, 0x40);
    }
    func_001F21B0(D_00160E80, 2);
}

extern __typeof__(draw_tfrag) func_002333A8 __attribute__((alias("FUN_002333a8")));
