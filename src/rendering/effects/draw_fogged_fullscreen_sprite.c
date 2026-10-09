#include "types.h"
#include "rnc/rendering/fs_aa_buffer.h"
#include "rnc/globals.h"
struct Fog {
    u8 pad0[4];
    u32 color;
    u64 enable;
};
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void draw_rect_overlay(s32, s32, s32, s32, u64) __asm__("func_001F52A0");
void draw_fogged_fullscreen_sprite(struct Fog *fog) __asm__("FUN_001f5138");

void draw_fogged_fullscreen_sprite(struct Fog *fog) {
    if (fog->enable != 0) {
        vu1_add_g_sregister(0x42, fog->enable & 0xFF000000FFULL);
    }
    if (fog->color & 0xFF000000) {
        vu1_add_g_sregister(0x4E, (depth_buffer_address >> 13) | 0x1000000 | ((u64)0x8000 << 17));
        draw_rect_overlay(0, fs_aa_buffer.display_height, 0, fs_aa_buffer.display_width, fog->color);
        vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
    }
    if (fog->enable != 0) {
        vu1_add_g_sregister(0x42, ((u64)0x8000 << 24) | 0x44);
    }
}

extern __typeof__(draw_fogged_fullscreen_sprite) func_001F5138
    __attribute__((alias("FUN_001f5138")));
