#include "types.h"
#include "rnc/rendering/draw_config.h"
#include "rnc/data_labels.h"

extern u8 D_0015FEE0[];
extern u8 D_0015FEF0[];
extern s32 D_0015FF38;
extern void noop_core_callback() __asm__("func_001F21B0");
extern s32 noop_core_callback_secondary() __asm__("func_001F21B8");
extern s32 dma_moby_textures() __asm__("FUN_0020cdf0");
extern s32 configure_graphics_setup_dma_chain() __asm__("FUN_0020d060");
extern void process_moby_anim_data() __asm__("FUN_0020d1a8");
extern void FUN_002116b8();

void draw_mobys_clean_up(void) __asm__("FUN_0020d3b0");

void draw_mobys_clean_up(void) {
    noop_core_callback_secondary(D_0015FEE0, 3);
    dma_moby_textures();
    noop_core_callback(D_0015FEE0, 5);
    if (draw_config.moby.enabled != 0) {
        process_moby_anim_data();
        if (D_0015FF38 != 0) {
            FUN_002116b8();
        }
    }
    noop_core_callback(D_0015FEF0, 3);
    if (draw_config.moby.enabled != 0) {
        if (D_0015FF40 != 0) {
            configure_graphics_setup_dma_chain();
        }
    }
}
