#include "types.h"

struct M2c_D_0018A2B0 {
    u8 pad_0[0x28];
    s32 unk28;
};

extern u8 D_0015FEE0[];
extern u8 D_0015FEF0[];
extern s32 D_0015FF38;
extern s32 D_0015FF40;
extern struct M2c_D_0018A2B0 D_0018A2B0;
extern void func_001F21B0();
extern s32 func_001F21B8();
extern s32 dma_moby_textures() __asm__("FUN_0020cdf0");
extern s32 FUN_0020d060();
extern void process_moby_anim_data() __asm__("FUN_0020d1a8");
extern void FUN_002116b8();

void draw_mobys_clean_up(void) __asm__("FUN_0020d3b0");

void draw_mobys_clean_up(void) {
    func_001F21B8(D_0015FEE0, 3);
    dma_moby_textures();
    func_001F21B0(D_0015FEE0, 5);
    if (D_0018A2B0.unk28 != 0) {
        process_moby_anim_data();
        if (D_0015FF38 != 0) {
            FUN_002116b8();
        }
    }
    func_001F21B0(D_0015FEF0, 3);
    if (D_0018A2B0.unk28 != 0) {
        if (D_0015FF40 != 0) {
            FUN_0020d060();
        }
    }
}
