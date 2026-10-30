/* Level 01 moby class pulsing_scroll_layers (0x4c9): queues the draw of three scrolling layers whose glow pulses with a sine. */
#include "types.h"
#include "rnc/math/fast_math.h"

typedef struct {
    u8  pad0[0x5C];
    f32 f5c;
} Ent;

typedef struct {
    u8  pad0[0x30];
    u8  b30;
    u8  b31;
    s16 h32;
} FlagMoby;

extern Ent l01_sine_modulated_scroll_layers[] __asm__("D_L01_00202D80");
extern s32 level_frame_count __asm__("D_L01_0015F5CC");
void       mine_blend_color(void) __asm__("FUN_L00_002371e0");
void       update_and_draw_scrolling_overlay_layers(s32, void *) __asm__("FUN_L01_002b96e0");
void       add_draw_callback(void *fn, void *arg) __asm__("FUN_001f4600");

void draw_pulsing_scroll_layers(void) __asm__("FUN_L01_00309bf8");

void draw_pulsing_scroll_layers(void) {
    f32 normalized_sine_value;
    s32 i;

    mine_blend_color();
    update_and_draw_scrolling_overlay_layers(3, l01_sine_modulated_scroll_layers);
    normalized_sine_value =
        fast_sin((convert_integer_to_float(level_frame_count & 0x3F) - 32.0f) * 0.09817477f);
    normalized_sine_value = normalized_sine_value * 0.5f + 0.5f;
    for (i = 2; i >= 0; i--) {
        l01_sine_modulated_scroll_layers[i].f5c = normalized_sine_value;
    }
}

/* Sets both flag fields to 0xFF and queues draw_pulsing_scroll_layers for the moby. */
void update_pulsing_scroll_layers(FlagMoby *moby) __asm__("FUN_L01_00309c98");

void update_pulsing_scroll_layers(FlagMoby *moby) {
    int flag_init_value = 0xFF;
    moby->h32 = flag_init_value;
    moby->b30 = flag_init_value;
    add_draw_callback(draw_pulsing_scroll_layers, moby);
}
