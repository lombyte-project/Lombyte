#include "types.h"
#include "qcopy.h"
#include "rnc/rendering/dma_tag.h"

typedef struct {
    u8 pad0[0x190];
    u8 v190[0x10];
    u8 v1A0[0x10];
    u8 pad1B0[0x60];
    f32 f210;
    u8 pad214[0x14];
    f32 f228;
    f32 f22C;
} Camera;

extern u16 D_0010E800[];
extern u8 D_0010E810[];
extern s32 D_0015F620;
extern f32 D_0015F348 __attribute__((sda));
extern u8 D_00187080[];
extern Camera D_0018CD00;
extern void FUN_001f9ff8(f32 (*)[4], f32);
extern void FUN_001f9a68(f32 *, u8 *, f32);
extern void FUN_001fa378(void *, u8 *, f32 (*)[4]);
extern void vu1_add_data_ref(u8 *, s32) __asm__("func_00233830");
extern void vu1_gs_regs_font(void) __asm__("func_00233C90");

void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void font_queue_vu_state(void) {
    f32 m[4][4];
    struct DmaTag *base;
    u8 *p;

    FUN_001f9ff8(m, 1024.0f);
    FUN_001f9a68(m[3], D_00187080, -1024.0f);
    m[3][3] = 1.0f;
    if (D_0015F620 != 7) {
        vu1_add_data_ref(D_0010E810, D_0010E800[0]);
        D_0015F620 = 7;
    }
    render_packet_cursor.tag->tag = 0x10000000;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0x11000000;
    render_packet_cursor.tag->vif1 = 0x1000404;
    base = render_packet_cursor.tag;
    base[1].tag = 0;
    base[1].addr = 0;
    base[1].vif0 = 0;
    base[1].vif1 = 0x6C0C43A4;
    p = (u8 *)(base + 2);
    FUN_001fa378(p, D_00187080 - 0x100, m);
    *(f32 *)(p + 0x38) += D_0015F348;
    p = (u8 *)(base + 6);
    FUN_001fa378(p, D_00187080 - 0x80, m);
    *(f32 *)(p + 0x38) += D_0015F348;
    base[10].tag = 0x8000;
    base[10].addr = 0x303EC000;
    base[10].vif0 = 0x412;
    *(f32 *)&base[10].vif1 = D_0018CD00.f210;
    p = (u8 *)(base + 11);
    qcopy(p, D_0018CD00.v190);
    p = (u8 *)(base + 12);
    qcopy(p, D_0018CD00.v1A0);
    *(f32 *)&base[13].tag = D_0018CD00.f22C;
    *(f32 *)&base[13].addr = D_0018CD00.f228;
    base[13].vif0 = 0;
    base[13].vif1 = 0;
    base[14].tag = 0x3000000;
    base[14].addr = 0x20001D2;
    base[14].vif0 = 0x15000000;
    base[14].vif1 = 0;
    p = (u8 *)(base + 15);
    render_packet_cursor.tag->tag |= (((u8 *)p - (u8 *)render_packet_cursor.tag) >> 4) - 1;
    render_packet_cursor.tag = (struct DmaTag *)p;
    vu1_gs_regs_font();
}

extern __typeof__(font_queue_vu_state) func_001F76A0 __attribute__((alias("FUN_001f76a0")));
/* Recovered original symbol name. */
extern __typeof__(font_queue_vu_state) FontQueueVUState __attribute__((alias("FUN_001f76a0")));
