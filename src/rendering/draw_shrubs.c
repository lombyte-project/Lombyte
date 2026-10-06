#include "types.h"

extern volatile s32 D_0015EE74;
extern s32 D_0015EE78;
extern u8 D_001603B0[];
extern u8 D_001603C0[];
extern s32 D_001603F0;
extern s32 D_00160F00;
struct ShrubDrawState {
    s32 unk0;
    s32 pad[2];
};
extern struct ShrubDrawState D_0018A2D0;
extern u8 D_001D8EB0[];
extern void func_00118A80(s32);
extern void WriteDmaChannel(u32, u32, u32);
extern void func_001F21B0(void *, s32);
extern void func_001F21B8(void *, s32);
extern void dma_shrub_textures(void) __asm__("func_002288F0");
extern void func_00228BE8(void);

void draw_shrubs(void) __asm__("FUN_00228b38");

void draw_shrubs(void) {
    s32 packet;

    packet = D_00160F00;
    D_001603F0 = packet;
    D_0015EE74 = D_0015EE78;
    packet += 0x10;
    D_00160F00 = packet;
    func_001F21B8(D_001603B0, 1);
    if (D_0018A2D0.unk0 != 0) {
        func_00118A80(0);
        func_00228BE8();
        WriteDmaChannel(D_001D8EB0, 0x3200, 0x40);
    }
    func_001F21B8(D_001603C0, 7);
    dma_shrub_textures();
    func_001F21B0(D_001603C0, 7);
}

extern __typeof__(draw_shrubs) func_00228B38 __attribute__((alias("FUN_00228b38")));
