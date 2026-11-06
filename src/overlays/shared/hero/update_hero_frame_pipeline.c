/* Hero: the per-frame update pipeline. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/overlay/shared/hero/advance_hero_animation.h"
#include "rnc/overlay/shared/ui/map/reveal_map_around_position.h"
#include "rnc/rendering/frame/begin_draw_frame.h"
#include "rnc/overlay/shared/hero/hero_frame_update.h"
#include "rnc/overlay/shared/hero/hero_idle_sequence.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"

/* Per-frame driver: validates the cursor, then runs each update pass in order. */
/* Ported from rac1-decomp (src/overlays/shared/help_00200890.c: func_L03_00200890), where it is exact; names translated to the US level program. */

extern void FUN_L00_00257024(void *);
extern void update_hero_state_machine(void) __asm__("FUN_L03_0021c668");
extern void noop_main_callback(void *) __asm__("FUN_001e93e8");

void update_hero_frame_pipeline(void) __asm__("FUN_L03_002002f8");

void update_hero_frame_pipeline(void) {
    struct Hero *hero_state = (struct Hero *)((char *)D_0013E533 + 0xE1D);
    if (hero_state->motion.pos.f[0] < 2.0f || hero_state->motion.pos.f[1] < 2.0f ||
        hero_state->motion.pos.f[0] > 1022.0f || hero_state->motion.pos.f[1] > 1022.0f) {
        char *q;
        fade_to_black(scale_game_frames(0x10));
        q = (char *)D_0013E533 + 0xE1D;
        q[0x20B1] = 1;
        return;
    }
    hero_state->glow_draw_queued = 0;
    hero_state->unk1EB0 = 0;
    if (hero_state->vehicle_mode != HERO_VEHICLE_MODE_ON_FOOT) {
        hero_frame_update();
        return;
    }
    advance_hero_animation();
    hero_update();
    update_hero_ground_material_effects();
    update_hero_ledge_probe();
    update_hero_offset_rail_candidate();
    update_hero_state_machine();
    if (hero_state->vehicle_mode != HERO_VEHICLE_MODE_ON_FOOT) {
        return;
    }
    update_hero_delayed_voice_requests();
    update_hero_easing_targets_and_point_effects();
    update_hero_collision_ease_target();
    update_hero_pitch_and_yaw_easing_targets();
    update_hero_target_tracking();
    update_hero_idle_random_rotation_targets();
    update_hero_rotation_targets_and_random_manipulators();
    update_hero_glow_and_extra_manipulators();
    update_hero_transformed_negative_z_angle_parameters();
    update_hero_easing_records();
    update_hero_moby();
    update_hero_world_matrices(0);
    update_hero_equipped_items();
    begin_draw_frame();
    update_hero_scalar_pulse();
    apply_hero_and_item_scale_sequence();
    check_hero_item_reward_conditions();
    update_hero_bolt_exclamation_hud();
    dispatch_hero_animation_timed_events();
    scan_hero_input_sequences();
    noop_main_callback(((void *)hero_state->moby));
    if (hero_state->state.control_mode != 0x16 && hero_state->state.current != 0x32) {
        reveal_map_around_position(&hero_state->motion.pos);
    }
    {
        struct Hero *q = (struct Hero *)((char *)D_0013E533 + 0xE1D);
        FUN_L00_00257024(((void *)q->moby));
    }
    update_hero_blink_and_gold_weapon_tint();
    update_hero_moby_probed_height_bounds();
}
