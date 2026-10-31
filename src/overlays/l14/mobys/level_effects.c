/* Level 14 level effects moby class (0x58a): queues the draw of the level's textured effect list each frame. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/rendering/effects/draw_textured_effect_list.h"

/* Draws the level's textured effect list (the draw callback the moby below queues). */
/* One 0x20-byte entry of draw_textured_effect_list (FUN_L12_002bc210): a bounding sphere it culls with, the
 * three data pointers and the count it passes to the two draw helpers. */
typedef struct TexturedEffectEntry {
    f32   bsphere[4]; /* 0x00: centre and radius */
    void *data10;     /* 0x10: second argument of both helpers */
    void *data14;     /* 0x14: fourth argument of FUN_L01_0021fa98 */
    void *data18;     /* 0x18: third argument of FUN_L12_002667fc */
    s32   count;      /* 0x1C: first argument of both helpers */
} TexturedEffectEntry;

/* The draw below passes 0x13 entries. */
extern TexturedEffectEntry level14_textured_effect_list[19] __asm__("D_L14_001F5F80");
extern s32                 get_effect_texture() __asm__("FUN_001f44b8");

void draw_level14_textured_effect_list(void) __asm__("FUN_L14_00307a28");

void draw_level14_textured_effect_list(void) {
    draw_textured_effect_list(
        level14_textured_effect_list, 0x13, get_effect_texture(0x2e), get_effect_texture(0x2f));
}

void add_draw_callback(int arg0, int arg1) __asm__("FUN_001f4600");

void level_effects_update(struct Moby *moby) __asm__("FUN_L14_00307a80");

/* Moby class 1418 */
void level_effects_update(struct Moby *moby) {
    enum {
        DRAW_CALLBACK_INITIALIZE = 0,
        DRAW_CALLBACK_ENQUEUE = 1
    };
    switch (moby->state) {
    case DRAW_CALLBACK_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case DRAW_CALLBACK_ENQUEUE:
        add_draw_callback((int)draw_level14_textured_effect_list, (int)moby);
        break;
    }
}
