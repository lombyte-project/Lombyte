/* Level 16 moby class spinning_sprite_effect (0x1d6, named rocket in the class record): a spinning moby with four glowing sprites. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"
/* updates the moby: initialises in state 0, then runs its motion in state 1 */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CA848), where it is exact; names translated to the US level program. */

extern float D_0015ED6C;
void         initialize_spinning_sprite_effect(struct Moby *moby) __asm__("FUN_L16_002c9568");

extern void initialize_spinning_sprite_effect(struct Moby *) __asm__("FUN_L16_002c9568");
extern void FUN_L16_002c9650_u(char *) __asm__("FUN_L16_002c9650");
f32         probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");

void update_spinning_sprite_effect(struct Moby *moby) __asm__("FUN_L16_002c9480");

void update_spinning_sprite_effect(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_SPIN_AND_DRAW_SPRITES = 1
    };
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE: {
        float ground_height;
        float vertical_distance_to_ground;
        initialize_spinning_sprite_effect((char *)moby);
        moby->scale = moby->scale * 0.9f;
        ground_height = probe_ground_height(&moby->pos, 0, 0.5f);
        vertical_distance_to_ground = absolute_float(ground_height - moby->pos.z);
        if (vertical_distance_to_ground < 1.0f) {
            moby->pos.z = ground_height + 0.9f;
        }
        moby->state = 1;
        break;
    }
    case MOBY_STATE_SPIN_AND_DRAW_SPRITES:
        moby->rot.z = fast_add_rotations(moby->rot.z, D_0015ED6C * 4.1887903f);
        FUN_L16_002c9650_u((char *)moby);
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CA930), where it is exact; names translated to the US level program. */

s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

void initialize_spinning_sprite_effect(struct Moby *moby) {
    char  *data = (char *)moby->pvars;
    float *random_value_cursor = (float *)data;
    int   *frame_count_cursor;
    int    i;
    int    unscaled_frame_count;
    for (i = 3; i >= 0; i--, random_value_cursor++) {
        *random_value_cursor = (float)random_integer_below(0xFF);
    }
    frame_count_cursor = (int *)(data + 0x20);
    *(float *)(data + 0x10) = -1.0f;
    *(float *)(data + 0x14) = -2.25f;
    *(float *)(data + 0x18) = 1.25f;
    *(float *)(data + 0x1C) = 2.5f;
    unscaled_frame_count = 0x3F;
    for (i = 3; i >= 0; i--, frame_count_cursor++) {
        *frame_count_cursor = scale_game_frames(unscaled_frame_count);
        unscaled_frame_count += 0x40;
    }
    *(float *)(data + 0x30) = 3.5f;
    *(float *)(data + 0x34) = 4.2f;
    *(float *)(data + 0x3C) = 3.5f;
    *(float *)(data + 0x38) = 4.2f;
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CAA18), where it is exact; names translated to the US level program. */

typedef struct {
    float a[4];
    float b[4];
    int   c[4];
    float d[4];
} L16SpriteData;

extern char           D_L16_001671C0[];
extern unsigned char *FUN_L00_00272f68(void *, int, unsigned char, int, int, int, int, float);
extern int            fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

void draw_spinning_sprite_effect_sprites(struct Moby *moby) __asm__("FUN_L16_002c9650");

void draw_spinning_sprite_effect_sprites(struct Moby *moby) {
    L16SpriteData *sprite_data = (L16SpriteData *)moby->pvars;
    float          d[4] __attribute__((aligned(16)));
    float          particle_position_step[4] __attribute__((aligned(16)));
    int            i, col;
    float          elapsed_cycle_fraction;

    moby->pos.z += 0.1f;
    fast_vec_sub(d, D_L16_001671C0, &moby->pos);
    fast_vec_normalize(d, d, -0.3f);
    fast_vec_normalize(particle_position_step, d, 0.1f);
    fast_vec_add(d, d, &moby->pos);
    moby->pos.z -= 0.1f;
    for (i = 0; i < 4; i++) {
        float unwrapped_sprite_value = sprite_data->a[i] + sprite_data->b[i];
        sprite_data->a[i] = unwrapped_sprite_value;
        if (255.0f <= unwrapped_sprite_value) {
            sprite_data->a[i] = unwrapped_sprite_value - 255.0f;
        } else if (unwrapped_sprite_value <= 0.0f) {
            sprite_data->a[i] = unwrapped_sprite_value + 255.0f;
        }
        if (fast_dec_timer(&sprite_data->c[i])) {
            sprite_data->c[i] = scale_game_frames(0xFF);
        }
        elapsed_cycle_fraction =
            convert_integer_to_float(scale_game_frames(0xFF) - sprite_data->c[i]);
        elapsed_cycle_fraction = elapsed_cycle_fraction / (float)scale_game_frames(0xFF);
        col =
            fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - elapsed_cycle_fraction));
        FUN_L00_00272f68(d,
                         col,
                         truncate_float_to_s32(sprite_data->a[i]) & 0xFF,
                         0x35,
                         1,
                         2,
                         0,
                         sprite_data->d[i]);
        fast_vec_add(d, d, particle_position_step);
    }
}
