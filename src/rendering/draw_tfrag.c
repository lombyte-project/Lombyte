#include "types.h"
#include "rnc/assembly_textbin_fun_002333a8_types.h"

struct Locals {
    u8 pad0[0x30];
    s32 v30;
    u8 pad34[8];
    f32 v3C;
};

extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern u8 D_00160E70[];
extern u8 D_00160E80[];
extern s32 D_00160EBC;
extern s32 D_00160F00;
extern u8 D_00187080[];
extern struct M2c_D_0018A2B0 D_0018A2B0;
extern u8 D_001E1300[];
extern void FlushCache(s32);
extern void WriteDmaChannel(u32, u32, u32);
extern void func_001F21B0(void *, s32);
extern void func_001F21B8(void *, s32);
extern void FUN_001f9a68(void *, void *, f32);
extern void FUN_001f9fc8(void *);
extern void FUN_001fa378(void *, void *, void *);
extern void func_002331C0(void);
extern void func_00233FB0(void);
void write_vif_unpack_packet(s32 addr, void *src, s32 qwc) __asm__("FUN_00233888");
void draw_tfrag(void) __asm__("FUN_002333a8");

void draw_tfrag(void) {
    struct Locals L;
    s32 packet;
    u8 *p;

    packet = D_00160F00;
    D_00160EBC = packet;
    D_0015EE74 = D_0015EE78;
    packet += 0x10;
    D_00160F00 = packet;
    func_001F21B8(D_00160E70, 1);
    FUN_001f9fc8(&L);
    p = D_00187080;
    FUN_001f9a68(&L.v30, p, -1024.0f);
    L.v3C = 1.0f;
    FUN_001fa378(&L, p - 0x100, &L);
    write_vif_unpack_packet(5, &L, 4);
    write_vif_unpack_packet(0x14D, &L, 4);
    if (D_0018A2B0.unk10 != 0) {
        FlushCache(0);
        func_00233FB0();
    }
    func_001F21B8(D_00160E80, 2);
    func_002331C0();
    if (D_0018A2B0.unk10 != 0) {
        WriteDmaChannel(D_001E1300, 0x3000, 0x40);
    }
    func_001F21B0(D_00160E80, 2);
}

extern __typeof__(draw_tfrag) func_002333A8 __attribute__((alias("FUN_002333a8")));
