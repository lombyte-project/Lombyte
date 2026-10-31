/* Level 16 grindrail_button. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

extern float fast_add_rotations(float, float);
extern float frame_time, D_0015EE70;
extern int   FUN_001fa6e0_u(float, int, int) __asm__("FUN_001fa6e0");

float D_L16_00161DF0[4] __attribute__((section(".sdata"))) = {0.0f, -1.0f, 1.0f, 0.0f};

extern void func_L16_002E6B48_6908(void *) __asm__("FUN_L16_002e56e0");

void grindrail_button_update(struct Moby *m) __asm__("FUN_L16_002e54a0");

/* Moby class 1442 */
void grindrail_button_update(struct Moby *button_moby) {
    enum {
        GRINDRAIL_BUTTON_INITIALIZE_GLOW_PHASE = 0,
        GRINDRAIL_BUTTON_CHECK_ACTIVATION = 1
    };
    char *button_data = (char *)button_moby->pvars;
    char *hero_position;
    char *hero_instance;
    float button_offset_from_hero[4];
    float glow_blend_factor;
    switch (button_moby->state) {
    case GRINDRAIL_BUTTON_INITIALIZE_GLOW_PHASE:
        button_moby->state = 1;
        *(float *)button_data = random_angle_radians();
        break;
    case GRINDRAIL_BUTTON_CHECK_ACTIVATION:
        if (fast_dec_timer((int *)(button_data + 4))) {
            hero_position = (char *)&hero.motion.pos;
            if (vector_distance_xy(&button_moby->pos, hero_position) < 4.0f) {
                hero_instance = hero_position - 0x80;
                if (*(int *)(hero_instance + 0x2084) == 0x28 &&
                    *(int *)(hero_instance + 0x5A8) != -1) {
                    transform_vector_by_basis(
                        button_offset_from_hero, &D_L16_00161DF0, &button_moby->rot_row0);
                    fast_vec_add(
                        button_offset_from_hero, button_offset_from_hero, &button_moby->pos);
                    fast_vec_sub(button_offset_from_hero, button_offset_from_hero, hero_position);
                    transform_vector_by_basis(
                        button_offset_from_hero, button_offset_from_hero, hero_position - 0x40);
                    if (absolute_float(button_offset_from_hero[1]) < 0.1f) {
                        if ((button_offset_from_hero[0] > 0.0f &&
                             *(int *)(hero_instance + 0x5A8) == 3) ||
                            (button_offset_from_hero[0] < 0.0f &&
                             *(int *)(hero_instance + 0x5A8) == 2)) {
                            func_L16_002E6B48_6908(button_moby);
                            allocate_voice_for_target_entry(1, 0, button_moby);
                        }
                    }
                }
            }
        }
        if (find_moby_hit(button_moby, 0x330000, 0)) {
            func_L16_002E6B48_6908(button_moby);
            allocate_voice_for_target_entry(0, 0, button_moby);
            button_moby->hit_slot = 0xFF;
        }
        break;
    }
    *(float *)button_data = fast_add_rotations(*(float *)button_data, frame_time * 12.566371f);
    glow_blend_factor = (fast_sin(*(float *)button_data) + 1.0f) * 0.5f;
    if (button_moby->command) {
        button_moby->glow_color = FUN_001fa6e0_u(glow_blend_factor, 0x80208020, 0x80202020);
    } else {
        button_moby->glow_color = FUN_001fa6e0_u(glow_blend_factor, 0x80202080, 0x80202020);
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E6908), where it is exact; names translated to the US level program. */

typedef struct {
    float value;
} Level16VendorTurnData;

typedef struct {
    char                   pad0[0x20];
    unsigned char          state;
    char                   pad21[0x57];
    Level16VendorTurnData *data;
    char                   pad7C[0x40];
    unsigned char          active;
} Level16VendorTurnMoby;

/* Start the vendor moby's turn and mark it active. */
/* Ported from rac1-decomp (src/overlays/l16/vendor_002A50F0.c: func_L16_002E6B48), where it is exact; names translated to the US level program. */

void activate_grindrail_button(Level16VendorTurnMoby *moby) __asm__("FUN_L16_002e56e0");

void activate_grindrail_button(Level16VendorTurnMoby *moby) {
    moby->data->value = -3.1415925f;
    moby->active = 1;
    moby->state = 2;
}
