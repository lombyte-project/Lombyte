/* Shared moby class red_plant (also other plants and leaves): rustles when the hero moves through it. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

void red_plant_update(unsigned char *p) __asm__("FUN_L00_002e3ca8");

void red_plant_update(unsigned char *moby) {
    char *hero_position = (char *)&hero.motion.pos;
    if (fast_vec_dist(hero_position, moby + 0x10) < 1.0f) {
        if (fast_vec_length(hero_position + 0x80) > frame_time + frame_time && moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(6));
        }
    }
    if (moby[0x70] & 2) {
        if (moby[0x53] == 1) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(6));
        }
    }
}
