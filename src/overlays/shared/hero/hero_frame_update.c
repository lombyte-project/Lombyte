/* Hero per-frame update: idle animation, voice requests, ground material effects, ledge probes, vehicle mode entry and exit, hit response and collision dimensions. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/hero.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/entities.h"
#include "qcopy.h"
#include "rnc/math/circle_offset.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/interpolation.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_hit_material.h"

/* Looks up the value of the current help record, or 0/0x54 in the special cases. */
/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: func_L01_002274A8), where it is exact; names translated to the US level program. */

extern int           hero_slot_item_id(int) __asm__("FUN_L00_0020d498");
extern unsigned char D_0013E533[];

int get_hero_idle_animation_index(int item_lookup_enabled) __asm__("FUN_L01_00226f10");

int get_hero_idle_animation_index(int item_lookup_enabled) {
    char *hero_state_base = (char *)D_0013E533 + 0xE1D;
    char *item_record;
    int   item_id;
    if (*(unsigned char *)(hero_state_base + 0x20A4) == 3)
        return 0;
    if (*(int *)(hero_state_base + 0x22A8) == 1)
        return 0x54;
    if (item_lookup_enabled == 0)
        return 0;
    if (*(unsigned char *)(hero_state_base + 0x20A8) == 0)
        return 0;
    if (*(unsigned char *)(hero_state_base + 0x20AA) == 0)
        return 0;
    if (*(short *)(hero_state_base + 0x22C8) != 0)
        return 0;
    item_id = hero_slot_item_id(0);
    item_record = D_L01_00179F40 - (-(item_id * 0x4C));
    return *(int *)(item_record + 0x24);
}
/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: func_L01_002283E0), where it is exact; names translated to the US level program. */

extern int request_grouped_voice(int voice_group_index,
                                 int voice_pair_index,
                                 int voice_index_offset,
                                 int voice_flags,
                                 int voice_moby) __asm__("FUN_L00_0028de68");

void request_hero_voice_if_enabled(int voice_group_index,
                                   int voice_pair_index,
                                   int call_enabled) __asm__("FUN_L01_00227e48");

void request_hero_voice_if_enabled(int voice_group_index, int voice_pair_index, int call_enabled) {
    if (call_enabled != 0) {
        char *hero_state_base = (char *)D_0013E533 + 0xE1D;
        char *voice_index_source = *(char **)(hero_state_base + 0x10E0);
        int   flag = 0;
        if (voice_index_source != 0) {
            flag = *(short *)(voice_index_source + 0xA6) == 0xAD;
        }
        request_grouped_voice(
            voice_group_index, voice_pair_index, flag, 0, *(int *)(hero_state_base + 0x2080));
    }
}

#include "rnc/mobys/moby.h"

extern int           level_frame_count __asm__("D_L01_0015F5CC");
extern unsigned char D_0013D4DC[];

#include "rnc/game/options.h"

extern float        D_L01_0017C04C[];
extern unsigned int fast_tween_color(float, unsigned int, unsigned int);
extern void         update_hero_easing_records(void) __asm__("FUN_L00_002054e8");
extern void         apply_hero_and_item_scale_sequence(void) __asm__("FUN_L00_00207a58");
extern void         update_hero_scalar_pulse(void) __asm__("FUN_L00_00207c48");
extern void         enqueue_hero_glow_draw_callback_once(void) __asm__("FUN_L00_00207d00");
extern void         update_hero_vehicle_moby(void) __asm__("FUN_L00_00208a10");
extern void         update_hero_moby_probed_height_bounds(void) __asm__("FUN_L00_00208b60");
extern void         hero_update(void) __asm__("FUN_L00_00211670");
extern void         allocate_hero_voice_for_entry(int, int) __asm__("FUN_L00_00216de8");
extern void         update_hero_delayed_voice_requests(void) __asm__("FUN_L00_00216f10");
extern void         advance_hero_animation(void) __asm__("FUN_L00_00232670");
extern void         reveal_map_around_position(void *) __asm__("FUN_L00_002484e0");
extern void         moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void         FUN_L00_00257024(void *);
extern void         update_hero_ground_material_effects(void) __asm__("FUN_L01_0022cd48");
extern void         update_hero_state_machine(void) __asm__("FUN_L01_00242930");

/* Per-frame hero update; in vehicle mode 3 it also pulses the hero moby's glow colour. */
void hero_frame_update(void) __asm__("FUN_L01_00228000");

void hero_frame_update(void) {
    struct Moby *hero_moby;
    int          period, red, green, blue, alpha, i;
    unsigned int color;
    float        pulse;

    advance_hero_animation();
    hero_update();
    update_hero_ground_material_effects();
    update_hero_state_machine();
    if (hero.vehicle_mode == 0)
        return;
    update_hero_delayed_voice_requests();
    update_hero_easing_records();
    if (hero.vehicle_mode == 0)
        return;
    update_hero_vehicle_moby();
    update_hero_scalar_pulse();
    apply_hero_and_item_scale_sequence();
    reveal_map_around_position(&hero.motion.pos);
    if (hero.vehicle_mode == 3) {
        if ((&big_head_cheat_enabled)[1] != 0)
            D_L01_0017C04C[0] = 1.9f;
        alpha = 0x80;
        period = scale_game_frames(0xAA);
        if (hero.glow_pulse_level == 2)
            period = scale_game_frames(0x32);
        else if (hero.glow_pulse_level == 1)
            period = scale_game_frames(0x5A);
        pulse = fast_sin(
            (((float)(level_frame_count % period) / convert_integer_to_float(period)) * 2.0f) *
                3.1415927f +
            -3.1415927f);
        red = truncate_float_to_s32(pulse * 20.0f) + 0x32;
        green = truncate_float_to_s32(pulse * 70.0f) + 0xB4;
        blue = truncate_float_to_s32(pulse * 10.0f);
        hero_moby = hero.moby;
        if (red > 0xFF)
            red = 0xFF;
        if (green > 0xFF)
            green = 0xFF;
        blue += 0x14;
        hero_moby->flags |= 0x10;
        if (hero.glow_pulse_level == 2) {
            unsigned int packed = (alpha << 24) | (blue << 16);
            packed |= (red / 2) << 8;
            color = packed | green;
        } else {
            unsigned int packed = (alpha << 24) | (blue << 16);
            packed |= green << 8;
            color = packed | red;
        }
        hero.moby->glow_color = fast_tween_color(0.1f, hero.moby->glow_color, color);
        for (i = 0; i < 3; i++) {
            moby_point_position(hero.moby, i, (char *)&hero + 0x1D50 + i * 16);
        }
        alpha = 0x30;
        {
            struct Moby *moby = hero.moby;
            blue = ((unsigned char *)&moby->glow_color)[2];
            green = ((unsigned char *)&moby->glow_color)[1];
            red = ((unsigned char *)&moby->glow_color)[0];
        }
        if (hero.vehicle_toggle_timer != 0)
            alpha = truncate_float_to_s32((convert_integer_to_float(hero.vehicle_toggle_timer) /
                                           convert_integer_to_float(0x12)) *
                                          (float)alpha);
        {
            unsigned int packed = (alpha << 24) | (blue << 16);
            packed |= green << 8;
            packed |= red;
            *(unsigned int *)((char *)&hero + 0x22E4) = packed;
        }
        enqueue_hero_glow_draw_callback_once();
        hero.glow_pulse_level = 0;
        if (hero.vehicle_mode == 3) {
            int pending = hero.pending_item[0];
            if (pending != 0 && pending != 0x1F) {
                if (hero.vehicle_toggle_timer == 0) {
                    allocate_hero_voice_for_entry(0x19, 0);
                    hero.vehicle_toggle_timer = 0x12;
                    hero.unk22DC = 1;
                }
            } else {
                hero.pending_item[0] = 0;
                if (hero.on_magnet_ground != 0 && hero.grounded_frames != 0 && D_0013D4DC[0] != 0 &&
                    hero.vehicle_toggle_timer == 0) {
                    allocate_hero_voice_for_entry(0x19, 0);
                    hero.vehicle_toggle_timer = 0x12;
                    hero.unk22DC = 1;
                }
            }
        }
    }
    update_hero_moby_probed_height_bounds();
    FUN_L00_00257024(hero.moby);
}

/* Per-frame update of the help/menu state: validates the cursor, then runs the update chain. */
/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: func_L01_00228E08), where it is exact; names translated to the US level program. */

extern void update_hero_rotation_targets_and_random_manipulators(void) __asm__("FUN_L00_002056a8");
extern void update_hero_glow_and_extra_manipulators(void) __asm__("FUN_L00_002059d8");
extern void update_hero_bolt_exclamation_hud(void) __asm__("FUN_L00_00205ea8");
extern void dispatch_hero_animation_timed_events(void) __asm__("FUN_L00_00206080");
extern void check_hero_item_reward_conditions(void) __asm__("FUN_L00_00206cf8");
extern void scan_hero_input_sequences(void) __asm__("FUN_L00_00206e00");
extern void update_hero_moby(void) __asm__("FUN_L00_00208820");
extern void update_hero_world_matrices(int) __asm__("FUN_L00_00209240");
extern void update_hero_collision_ease_target(void) __asm__("FUN_L00_0020a318");
extern void update_hero_pitch_and_yaw_easing_targets(void) __asm__("FUN_L00_0020a540");
extern void update_hero_idle_random_rotation_targets(void) __asm__("FUN_L00_0020a9e8");
extern void update_hero_target_tracking(void) __asm__("FUN_L00_0020ac98");
extern void update_hero_easing_targets_and_point_effects(void) __asm__("FUN_L00_0020b1d8");
extern void update_hero_equipped_items(void) __asm__("FUN_L00_00210928");
extern void update_hero_transformed_negative_z_angle_parameters(void) __asm__("FUN_L00_00214fe8");
extern void hero_frame_update(void) __asm__("FUN_L01_00228000");
void        update_hero_blink_and_gold_weapon_tint(void) __asm__("FUN_L01_00228e38");

extern void update_hero_blink_and_gold_weapon_tint(void) __asm__("FUN_L01_00228e38");
extern void update_hero_ledge_probe(void) __asm__("FUN_L01_0022d090");
extern void noop_main_callback(void *) __asm__("FUN_001e93e8");
void        begin_draw_frame(void) __asm__("FUN_001f7978");
void        fade_to_black(s32 n) __asm__("FUN_001f4a58");

void update_hero_frame(void) __asm__("FUN_L01_00228870");

void update_hero_frame(void) {
    char *hero_state_base = (char *)D_0013E533 + 0xE1D;
    if (*(float *)(hero_state_base + 0x80) < 2.0f || *(float *)(hero_state_base + 0x84) < 2.0f ||
        *(float *)(hero_state_base + 0x80) > 1022.0f ||
        *(float *)(hero_state_base + 0x84) > 1022.0f) {
        char *q;
        fade_to_black(scale_game_frames(0x10));
        q = (char *)D_0013E533 + 0xE1D;
        q[0x20B1] = 1;
        return;
    }
    *(short *)(hero_state_base + 0x1EB2) = 0;
    *(short *)(hero_state_base + 0x1EB0) = 0;
    if (*(unsigned char *)(hero_state_base + 0x20A4) != 0) {
        hero_frame_update();
        return;
    }
    advance_hero_animation();
    hero_update();
    update_hero_ground_material_effects();
    update_hero_ledge_probe();
    update_hero_state_machine();
    if (*(unsigned char *)(hero_state_base + 0x20A4) != 0) {
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
    noop_main_callback(*(void **)(hero_state_base + 0x2080));
    if (*(int *)(hero_state_base + 0x208C) != 0x16 && *(int *)(hero_state_base + 0x2084) != 0x32) {
        reveal_map_around_position(hero_state_base + 0x80);
    }
    {
        char *q = (char *)D_0013E533 + 0xE1D;
        FUN_L00_00257024(*(void **)(q + 0x2080));
    }
    update_hero_blink_and_gold_weapon_tint();
    update_hero_moby_probed_height_bounds();
}

extern void copy_hero_color_to_item_mobys(void) __asm__("FUN_L00_002072c8");
extern void update_item_tint_from_hero(struct Moby *, struct Moby *) __asm__("FUN_L00_00207330");

/*
 * Blinks the hero's moby colour for the first 30 frames of states 0x80 and
 * 0x82 (and of state 0x76 for 20 frames in levels 15 and 17), then, when the
 * equipped item is a gold weapon, runs FUN_L00_00207330 on its moby.
 */
void update_hero_blink_and_gold_weapon_tint(void) {
    /* three scalars, not an array: an int[3] changes the stack layout */
    int r, g, b;
    int blink;

    blink = 0;
    if (hero.state.current == 0x80 || hero.state.current == 0x82)
        blink = hero.state_timer < scale_game_frames(30);
    if (current_level_index == 0xF || current_level_index == 0x11) {
        if (hero.state.current == 0x76 && hero.state_timer < scale_game_frames(20))
            blink = 1;
    }
    if (blink) {
        get_moby_color(hero.moby, &r, &g, &b);
        if (hero.state_timer % 4 < 3) {
            r = 0;
            g = 0;
            b = 0;
        } else {
            r = 0x90;
            g = 0x90;
            b = 0xF0;
        }
        set_moby_color(hero.moby, r, g, b);
        copy_hero_color_to_item_mobys();
    }
    if (hero.items[0].item_id >= 0 && item_text_variant[hero.items[0].item_id] != 0 &&
        hero.items[0].moby != 0)
        update_item_tint_from_hero(hero.items[0].moby, hero.moby);
}

extern int  func_L00_001F10E0_2cd48(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int  l01_hero_set_state(int, int) __asm__("FUN_L01_0023cf98");
extern void exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");

void update_hero_ground_material_effects(void) {
    struct Hero  *hero_ptr = &hero;
    int           saved_ground_type;
    int           x;
    unsigned char saved = hero_ptr->ground_sound_class;
    saved_ground_type = hero_ptr->ground_type;
    fast_mem_zero16(&hero_ptr->ground_type, 0x10);
    hero_ptr->ground_sound_class = saved;
    hero_ptr->ground_type = -1;
    hero_ptr->wading = 0;
    hero_ptr->unk308 = 0;
    if (saved_ground_type == -1)
        return;
    if (saved_ground_type == 2) {
        if (hero_ptr->frames_off_ground.s == 0 || hero_ptr->ground_distance < 0.3f) {
            hero_ptr->on_magnet_ground = 1;
            if (hero_ptr->vehicle_mode == 0) {
                char *item_slot_1_moby = (char *)hero_ptr->items[1].moby;
                if (item_slot_1_moby != 0 && MOBY(item_slot_1_moby)->oclass == 0xAD)
                    hero_ptr->unk308 = 1;
            }
        }
    }
    if (saved_ground_type == 0xE) {
        struct Hero *q = &hero;
        q->base_condition = 1;
        q->water_depth = 0.2f;
        q->water_surface_z = q->ground_z.f + 0.2f;
    }
    if (saved_ground_type == 0) {
        struct Hero *q = &hero;
        q->water_depth = q->water_surface_z - q->ground_z.f;
        if (q->water_depth < 0.85f) {
            if (q->water_depth > 0.25f)
                q->wading = 1;
        }
        q = &hero;
        q->base_condition = 1;
    }
    if (current_level_index == 0xD) {
        struct Hero *q = &hero;
        if (q->state.current != 0x7B) {
            if (func_L00_001F10E0_2cd48(q->capsule_radius + 0.03f, &q->motion.body_center, 2, 0)) {
                if (collision_hit_material() == 0xB) {
                    l01_hero_set_state(0x7B, 1);
                    return;
                }
            }
        }
    }
    if (saved_ground_type == 0xB) {
        struct Hero *q = &hero;
        q->selector_11 = 1;
    }
    if (saved_ground_type == 4) {
        struct Hero *q = &hero;
        q->selector_4 = 1;
    }
    if (saved_ground_type == 0xD) {
        struct Hero *q = &hero;
        q->selector_13 = 1;
    }
    if (saved_ground_type == 8) {
        struct Hero *q = &hero;
        q->on_slip_ground = 1;
    }
    if (saved_ground_type == 9) {
        struct Hero *q = &hero;
        q->unk12EE = 1;
    }
    if (saved_ground_type == 0xC) {
        struct Hero *q = &hero;
        q->on_slip_ground = 1;
    }
    {
        struct Hero *q = &hero;
        if (q->selector_4 != 0 && q->grounded_frames != 0) {
            x = q->state.control_mode;
            if (x != 0x10 && x != 0x14 && x != 7) {
                l01_hero_set_state(0x31, 1);
                return;
            }
        }
        {
            struct Hero *sink_check_hero = &hero;
            if (sink_check_hero->selector_13 != 0) {
                if (sink_check_hero->state.current == 0x7F)
                    return;
                if (absolute_float(sink_check_hero->water_surface_z -
                                   (sink_check_hero->motion.pos.f[2] + 0.25f)) < 1.0f) {
                    if (sink_check_hero->water_surface_z - sink_check_hero->motion.pos.f[2] >
                        0.0f) {
                        if (sink_check_hero->motion.displacement.f[2] < 0.0f) {
                            exit_hero_vehicle_mode_if_mode_three();
                            l01_hero_set_state(HERO_STATE_SINK, 1);
                        }
                    }
                }
            }
        }
    }
}

extern CollisionHit coll_hit __asm__("D_L01_001742C0") NOT_SDA;
extern int          collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void
predict_position_with_vertical_drop(void *, void *, void *, int, float) __asm__("FUN_L00_00259508");
extern char *FUN_L00_002603d0(char *);
extern s32   rig_probe(void *, s32, f32, s32, f32) __asm__("FUN_L00_001f19a0");

/* Probes for a reachable ledge ahead of the hero and records its edge. */
void update_hero_ledge_probe(void) {
    Vec4  probe, top, low, from, head, wall, to, edge;
    f32   grab_drop, drop, fwd, up, reach, step, top_margin, max_slope, edge_offset;
    f32   wall_yaw, launch_scale, yaw_to_ledge, five, one, limit, inc, wall_normal_offset, half;
    int   found, hit, i, j, material, allowed, moby_allowed, frames;
    u8   *moby_record;
    char *moby_data;

    if (hero.unk308 == 2)
        return;
    hero.ledge_found = 0;
    if (hero.ledge_probe_cooldown != 0)
        return;
    allowed = 0;
    if (hero.vehicle_mode == 0) {
        if ((hero.state.control_mode == 4 &&
             (hero.past_jump_apex != 0 || hero.motion.velocity.f[2] < frame_time * 3.5f)) ||
            hero.state.control_mode == 2 || hero.state.current == 8 || hero.state.current == 0x1A ||
            hero.state.current == 0x1B)
            allowed = 1;
        if (hero.state.current == 0xE && hero.anim_frame < 37.0f)
            allowed = 0;
    }
    if (!allowed)
        return;

    drop = -0.7f;
    grab_drop = -1.43f;
    fwd = 0.3f;
    up = 1.5f;
    reach = 1.15f;
    edge_offset = 0.45f;
    launch_scale = hero.fall_state_gravity;
    if (hero.state.control_mode == 4)
        launch_scale = hero.jump_gravity;
    frames = scale_game_frames(2);
    step = 0.25f;
    top_margin = 0.15f;
    predict_position_with_vertical_drop(
        &probe, &hero.motion.pos, &hero.motion.resolved_velocity, frames, launch_scale);
    found = 0;
    i = 0;
    probe.f[0] += fast_cos(hero.motion.rot.f[2]) * hero.capsule_radius;
    probe.f[1] += fast_sin(hero.motion.rot.f[2]) * hero.capsule_radius;
    while (1) {
    probe_next:
        probe.f[0] += fast_cos(hero.motion.rot.f[2]) * fwd * step;
        probe.f[1] += fast_sin(hero.motion.rot.f[2]) * fwd * step;
        qcopy(&top, &probe);
        top.f[2] += up + top_margin;
        qcopy(&low, &probe);
        low.f[2] += reach;
        if (collision_line(&top, &low, 4, hero.moby, 0)) {
            material = collision_hit_material();
            if (material != 9 && material != 12 && material != 0) {
                if (fast_arc_tan(coll_hit.normal_z, vector_length_xy(&coll_hit.normal_x)) <
                    0.34906584f)
                    found = 1;
            }
        }
        i++;
        if (found)
            break;
        if (i < 4)
            goto probe_next;
        return;
    }

    if (coll_hit.moby != 0) {
        moby_allowed = 0;
        moby_record = suck_get_aim_height(coll_hit.moby);
        moby_data = FUN_L00_002603d0((char *)coll_hit.moby);
        if (moby_record && (*(u16 *)(moby_record + 0x1E) & 1))
            moby_allowed = 1;
        else if (moby_data)
            moby_allowed = *(u32 *)(moby_data + 0x3C) & 1;
        if (!moby_allowed)
            return;
    }
    hero.ledge_moby = coll_hit.moby;
    if (hero.motion.pos.f[2] + up < coll_hit.point.f[2])
        return;
    qcopy(&top, &coll_hit.point);
    qcopy(&low, &coll_hit.point);
    low.f[2] += 0.05f;
    qcopy_nc(&from, &hero.motion.pos);
    from.f[2] = low.f[2];
    if (collision_line(&from, &low, 4, hero.moby, 0))
        return;
    qcopy(&head, &coll_hit.point);
    head.f[2] += 0.6f;
    if (rig_probe(&head, 4, edge_offset, (s32)hero.moby, 0.6f))
        hero.ledge_flags |= 1;
    else
        hero.ledge_flags &= ~1;
    yaw_to_ledge = fast_arc_tan(top.f[0] - hero.motion.pos.f[0], top.f[1] - hero.motion.pos.f[1]);
    wall_yaw = hero.ground_z.f;
    max_slope = top.f[2];
    hero.ledge_top_z = max_slope;
    max_slope -= wall_yaw;
    if (max_slope < 1.8f)
        return;

    five = 5.0f;
    one = 1.0f;
    hit = 0;
    for (j = 0; j < 5; j++) {
        qcopy(&wall, &hero.motion.pos);
        qcopy(&to, &top);
        circle_offset(&to, yaw_to_ledge, hero.capsule_radius);
        to.f[2] += drop * (one - (f32)j / five);
        wall.f[2] = to.f[2];
        if (collision_line(&wall, &to, 2, 0, 0)) {
            hit = 1;
            hero.ledge_normal_yaw = fast_arc_tan(coll_hit.normal_x, coll_hit.normal_y);
            break;
        }
    }
    if (!hit)
        return;
    if (fast_difference_between_rotations(hero.motion.rot.f[2],
                                          fast_add_rotations(hero.ledge_normal_yaw, 3.14159274f)) <
        1.13446403f) {
        limit = 2.4f;
        inc = 0.03f;
        wall_normal_offset = 0.0f;
        half = 0.5f;
        do {
            qcopy(&to, &top);
            to.f[2] = top.f[2];
            to.f[0] += fast_cos(hero.ledge_normal_yaw) * wall_normal_offset;
            to.f[1] += fast_sin(hero.ledge_normal_yaw) * wall_normal_offset;
            qcopy(&wall, &to);
            to.f[2] += half;
            wall.f[2] -= half;
            if (!collision_line(&to, &wall, 4, hero.moby, 0)) {
                qcopy(&edge, &to);
                edge.f[2] = top.f[2];
                wall_normal_offset = inc * half;
                wall_normal_offset = -wall_normal_offset;
                edge.f[0] += fast_cos(hero.ledge_normal_yaw) * wall_normal_offset;
                edge.f[1] += fast_sin(hero.ledge_normal_yaw) * wall_normal_offset;
                qcopy(&hero.ledge_hang_pos, &edge);
                hero.ledge_hang_pos.f[0] += fast_cos(hero.ledge_normal_yaw) * edge_offset;
                hero.ledge_hang_pos.f[1] += fast_sin(hero.ledge_normal_yaw) * edge_offset;
                hero.ledge_found = 1;
                hero.ledge_hang_pos.f[2] += grab_drop;
                return;
            }
            wall_normal_offset += inc;
        } while (wall_normal_offset < limit);
    }
}

/* line query from -> to; fills the collision hit record, nonzero on a hit */
extern int
collision_line(void *from, void *to, int mask, void *ignore, int) __asm__("FUN_001efa68");

/*
 * Ledge probe, FUN_L00_0020c758 without its hero.vehicle_mode == 1 variant:
 * steps forward from `pos` along the hero's yaw looking for a flat top
 * (normal within 20 degrees of up, not material 9 or 12) at least 1.8 above
 * the floor and not above pos + up, then for the wall under it. Returns 1 when the hero (facing `*dir`) faces that wall within
 * 65 degrees and the ledge edge is found within 2.4 units along it.
 * Constants live in locals because retail loads each one once.
 */
int test_hero_ledge_ahead(Vec4 *ledge_probe_origin, f32 *facing_yaw) __asm__("FUN_L01_0022d838");

int test_hero_ledge_ahead(Vec4 *ledge_probe_origin, f32 *facing_yaw) {
    Vec4 probe, top, low, wall, edge;
    f32  drop, fwd, up, reach;
    f32  yaw_to_ledge, wall_yaw, wall_normal_offset, step, top_margin, max_slope, one, five, half,
        inc, limit;
    int found, hit, i, j, material;

    if (hero.unk308 == 2)
        return 0;

    drop = -0.7f;
    fwd = 0.3f;
    up = 1.5f;
    reach = 1.15f;
    wall_yaw = 0.0f;
    found = 0;
    qcopy(&probe, ledge_probe_origin);
    step = 0.25f;
    i = 0;
    top_margin = 0.15f;
    max_slope = 0.34906584f; /* 20 degrees */
    probe.f[0] += fast_cos(hero.motion.rot.f[2]) * hero.capsule_radius;
    probe.f[1] += fast_sin(hero.motion.rot.f[2]) * hero.capsule_radius;
    do {
        probe.f[0] += fast_cos(hero.motion.rot.f[2]) * fwd * step;
        probe.f[1] += fast_sin(hero.motion.rot.f[2]) * fwd * step;
        qcopy(&top, &probe);
        top.f[2] += up + top_margin;
        qcopy(&low, &probe);
        low.f[2] += reach;
        if (collision_line(&top, &low, 4, hero.moby, 0)) {
            material = collision_hit_material();
            if (material != 9 && material != 12) {
                if (fast_arc_tan(coll_hit.normal_z, vector_length_xy(&coll_hit.normal_x)) <
                    max_slope)
                    found = 1;
            }
        }
        i++;
        if (found)
            break;
    } while (i < 4);
    if (!found)
        return 0;
    if (ledge_probe_origin->f[2] + up < coll_hit.point.f[2])
        return 0;

    qcopy(&top, &coll_hit.point);
    yaw_to_ledge =
        fast_arc_tan(top.f[0] - ledge_probe_origin->f[0], top.f[1] - ledge_probe_origin->f[1]);
    if (top.f[2] - hero.ground_z.f < 1.8f)
        return 0;

    /* the wall below the top, tested at five heights */
    five = 5.0f;
    one = 1.0f;
    hit = 0;
    for (j = 0; j < 5; j++) {
        qcopy(&low, ledge_probe_origin);
        qcopy(&wall, &top);
        circle_offset(&wall, yaw_to_ledge, hero.capsule_radius);
        wall.f[2] += drop * (one - j / five);
        low.f[2] = wall.f[2];
        if (collision_line(&low, &wall, 2, 0, 0)) {
            hit = 1;
            wall_yaw = fast_arc_tan(coll_hit.normal_x, coll_hit.normal_y);
            break;
        }
    }
    if (!hit)
        return 0;

    if (fast_difference_between_rotations(*facing_yaw, fast_add_rotations(wall_yaw, 3.14159274f)) <
        1.13446403f) {
        /* walk along the wall normal until the top ends */
        limit = 2.4f;
        inc = 0.07f;
        wall_normal_offset = 0.0f;
        half = 0.5f;
        do {
            qcopy(&wall, &top);
            wall.f[2] = top.f[2];
            wall.f[0] += fast_cos(wall_yaw) * wall_normal_offset;
            wall.f[1] += fast_sin(wall_yaw) * wall_normal_offset;
            qcopy(&low, &wall);
            wall.f[2] += half;
            low.f[2] -= half;
            if (collision_line(&wall, &low, 4, hero.moby, 0)) {
                wall_normal_offset += inc;
                continue;
            }
            /* edge found: its point is computed but unused, as in retail */
            qcopy(&edge, &wall);
            wall_normal_offset = inc * half;
            wall_normal_offset = -wall_normal_offset;
            edge.f[2] = top.f[2];
            edge.f[0] += fast_cos(wall_yaw) * wall_normal_offset;
            edge.f[1] += fast_sin(wall_yaw) * wall_normal_offset;
            if (0.52359879f < fast_difference_between_rotations(
                                  fast_add_rotations(wall_yaw, 3.14159274f), *facing_yaw))
                return 0;
            return 1;
        } while (wall_normal_offset < limit);
    }
    return 0;
}
/* Starts a level scene: resets the player state, takes the new moby and sets up its flags. */
/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: func_L01_00231960), where it is exact; names translated to the US level program. */

extern void reset_hero_easing_records(void) __asm__("FUN_L00_00205538");
extern void release_and_clear_hero_voice_slots(void) __asm__("FUN_L00_00206c08");
extern void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
extern void hero_update_items() __asm__("FUN_L00_00210748");
extern void clear_hero_queued_animation(void) __asm__("FUN_L00_00232628");
extern void FUN_L00_00232fe8(void);
extern void clear_moby_transform_history(char *o) __asm__("FUN_L00_00262840");
extern void update_hero_collision_dimensions(void) __asm__("FUN_L01_00231ae0");

void enter_hero_vehicle_mode_and_set_moby(int vehicle_mode,
                                          int initial_state,
                                          int new_moby) __asm__("FUN_L01_00231348");

void enter_hero_vehicle_mode_and_set_moby(int vehicle_mode, int initial_state, int new_moby) {
    char *hero_state_base;
    cancel_hero_item_animation();
    hero_state_base = (char *)D_0013E533 + 0xE1D;
    *(char *)(hero_state_base + 0x20A6) = 1;
    hero_update_items();
    *(int *)(hero_state_base + 0x2080) = *(int *)(hero_state_base + 0xA88);
    clear_moby_transform_history(hero_state_base + 0x1670);
    *(int *)(*(char **)(hero_state_base + 0x2080) + 0x98) = -1;
    FUN_L00_00232fe8();
    release_and_clear_hero_voice_slots();
    reset_hero_easing_records();
    *(char *)(hero_state_base + 0x20A4) = vehicle_mode;
    *(int *)(hero_state_base + 0xA84) = new_moby;
    *(int *)(hero_state_base + 0x2080) = new_moby;
    update_hero_collision_dimensions();
    *(float *)(hero_state_base + 0x220) = *(float *)(hero_state_base + 0x228);
    *(float *)(hero_state_base + 0x224) = *(float *)(hero_state_base + 0x22C);
    *(unsigned short *)(*(char **)(hero_state_base + 0x2080) + 0x34) |= 6;
    *(float *)(hero_state_base + 0xA90) = 1.0f;
    *(float *)(hero_state_base + 0xA94) = 1.0f;
    clear_hero_queued_animation();
    FUN_L00_00257024(*(void **)(hero_state_base + 0x2080));
    apply_hero_and_item_scale_sequence();
    *(short *)(hero_state_base + 0x22CC) = initial_state;
    if (vehicle_mode == 1) {
        *(unsigned short *)(hero_state_base + 0x22B0) =
            *(unsigned short *)(hero_state_base + 0x22A8);
        *(int *)(hero_state_base + 0x22A8) = *(int *)(hero_state_base + 0x22AC);
    }
    l01_hero_set_state(initial_state, 1);
}

/* resets the hero state and default camera fields */
/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: func_L01_00231A68), where it is exact; names translated to the US level program. */

extern int cutscene_mode __asm__("D_L01_0015F5C4");

void exit_hero_vehicle_mode_and_restore_moby(void) __asm__("FUN_L01_00231450");

void exit_hero_vehicle_mode_and_restore_moby(void) {
    char *hero_state_base;
    char *previous_active_moby;
    char *moby_to_remove;
    char *restored_moby;
    reset_hero_easing_records();
    hero_state_base = (char *)D_0013E533 + 0xE1D;
    previous_active_moby = *(char **)(hero_state_base + 0xA84);
    hero_state_base[0x20A4] = 0;
    if (previous_active_moby != 0) {
        MOBY(previous_active_moby)->flags &= 0xFFF9;
    }
    *(int *)(hero_state_base + 0xA84) = 0;
    moby_to_remove = *(char **)(hero_state_base + 0xA8C);
    if (moby_to_remove != 0) {
        mark_moby_for_removal(moby_to_remove);
        *(int *)(hero_state_base + 0xA8C) = 0;
    }
    release_and_clear_hero_voice_slots();
    restored_moby = *(char **)(hero_state_base + 0xA88);
    *(char **)(hero_state_base + 0x2080) = restored_moby;
    qcopy(restored_moby + 0x10, hero_state_base + 0x80);
    MOBY(restored_moby)->collision_disable = 0;
    *(float *)(hero_state_base + 0xA94) = 1.0f;
    clear_hero_queued_animation();
    if (*(int *)(hero_state_base + 0x2084) == 0x64) {
        if (cutscene_mode == 2)
            return;
        if (cutscene_mode == 6)
            return;
    }
    l01_hero_set_state(HERO_STATE_IDLE, 1);
}

extern s32 D_0013DF88[] __asm__("D_0013DF88");
extern s32 D_0015EEA8 __asm__("D_0015EEA8");

void release_and_clear_hero_voice_slots(void) __asm__("FUN_L00_00206c08");
void allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
void hero_knockback(void *, s32, f32, f32) __asm__("FUN_L00_00210c80");

#define G hero

extern OvlMobyEntry40 l01_moby_hit_response_entries[] __asm__("D_L01_00178580");
s32 process_hero_hit_response(s32 apply_state_and_motion_response) __asm__("FUN_L01_00231580");

s32 process_hero_hit_response(s32 apply_state_and_motion_response) {
    u8             *hero_moby;
    OvlMobyEntry40 *hero_moby_entry;
    u8             *mob;
    s32             flag;
    s32             id;
    f32             horizontal_scale, vertical_amount, cached_frame_time;
    OvlVec4         motion_delta;

    if (G.state.control_mode == 0x14) {
        return 0;
    }
    if (G.state.control_mode == 7) {
        return 0;
    }
    if (G.state.current == 0x32) {
        return 0;
    }
    if (G.invulnerable_frames != 0) {
        return 0;
    }
    if (cutscene_mode != 0) {
        return 0;
    }
    G.damage_source_moby = 0;
    hero_moby = (u8 *)G.moby;
    if (hero_moby[0xA4] == 0xFF) {
        return 0;
    }
    hero_moby_entry = &l01_moby_hit_response_entries[hero_moby[0xA4]];
    if (hero_moby_entry->owner != hero_moby) {
        return 0;
    }
    if ((hero_moby_entry->unk24 ^ 1) & 1) {
        return 0;
    }
    D_0013DF88[current_level_index]++;
    G.damage_source_moby = (struct Moby *)hero_moby_entry->target;
    D_0015EEA8++;
    if (apply_state_and_motion_response == 0) {
        return 1;
    }
    release_and_clear_hero_voice_slots();
    flag = 0;
    if (hero_moby_entry->flags & 1) {
        qcopy(&motion_delta, &hero_moby_entry->pos);
        if (*(f32 *)((u8 *)hero_moby_entry + 0x1C) == 5627.9248046875f) {
            flag = 1;
        }
    } else if (hero_moby_entry->target != 0) {
        fast_vec_sub(&motion_delta, (u8 *)&G + 0x80, hero_moby_entry->target + 0x10);
    } else {
        motion_delta.f[0] = fast_cos(fast_add_rotations(G.motion.rot.f[2], 3.1415927f));
        motion_delta.f[1] = fast_sin(fast_add_rotations(G.motion.rot.f[2], 3.1415927f));
        motion_delta.i[2] = 0;
    }
    switch (G.vehicle_mode) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        mob = (u8 *)G.damage_source_moby;
        if (mob != 0 && (*(s16 *)(mob + 0xA6) == 0x4EB || *(s16 *)(mob + 0xA6) == 0x558)) {
            id = 0x80;
        call_ret:
            l01_hero_set_state(id, 1);
            return 1;
        }
        if (G.state.control_mode == 0x16) {
            l01_hero_set_state(0x6D, 1);
            G.motion.resolved_velocity_z.f[2] = frame_time * 7.0f;
            return 1;
        }
        if (G.state.control_mode == 0x12) {
            mob = (u8 *)G.damage_source_moby;
            if (mob != 0 && *(s16 *)(mob + 0xA6) == 0x28F) {
                id = 0x82;
                goto call_ret;
            }
            l01_hero_set_state(0x75, 1);
        } else if (G.state.control_mode == 0x11) {
            mob = (u8 *)G.damage_source_moby;
            if (mob != 0 && *(s16 *)(mob + 0xA6) == 0x28F) {
                id = 0x82;
                goto call_ret;
            }
            if (current_level_index == 0xF || current_level_index == 0x11) {
                mob = (u8 *)G.damage_source_moby;
                if (mob != 0 && (*(s16 *)(mob + 0xA6) == 0x28F || *(s16 *)(mob + 0xA6) == 0x7B ||
                                 *(s16 *)(mob + 0xA6) == 0x29D)) {
                    allocate_hero_voice_for_entry(0x1C, 0);
                }
            }
            l01_hero_set_state(0x76, 1);
        } else if (G.state.control_mode == 3) {
            mob = (u8 *)G.damage_source_moby;
            if (mob != 0) {
                id = 0x82;
                if (*(s16 *)(mob + 0xA6) == 0x28F) {
                    goto call_ret;
                }
            }
            l01_hero_set_state(HERO_STATE_HURT, 1);
        } else {
            l01_hero_set_state(HERO_STATE_HURT, 1);
        }
        cached_frame_time = frame_time;
        horizontal_scale = cached_frame_time * 5.7f;
        vertical_amount = cached_frame_time * 2.4f;
        if (G.on_magnet_ground != 0) {
            horizontal_scale = 0.0f;
            vertical_amount = cached_frame_time * 1.7f;
        }
        hero_knockback(&motion_delta, flag, horizontal_scale, vertical_amount);
        if (flag != 0 && hero_moby_entry->unk28 == 4) {
            motion_delta.f[2] += motion_delta.f[2];
        }
        break;
    case 3:
        l01_hero_set_state(0x56, 1);
        hero_knockback(&motion_delta, flag, frame_time * 5.0f, frame_time * 2.4f);
        break;
    }
    fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &motion_delta);
    fast_vec_add(&hero.motion.displacement, &hero.motion.displacement, &motion_delta);
    return 1;
}
/* Adapted from the exact update_hero_smoothing (FUN_L17_0020e1c0, src/overlays/l17/hero/process_hero_hit_response.c)
   and update_hero_collision_capsule_dimensions (FUN_L00_00211380, src/overlays/shared/hero/hero_switch_item.c);
   L01 has only the mode-0 and mode-3 cases and no b12E2 branch. */

extern f32 D_0013F570[] __asm__("D_0013F570");
void       vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");

void update_hero_collision_dimensions(void) {
    switch (hero.vehicle_mode) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        hero.capsule_top_target = 0.8f;
        hero.capsule_bottom_target = 0.7f;
        hero.capsule_radius_target = 0.45f;
        break;
    case 3:
        hero.capsule_top_target = 0.8f;
        hero.capsule_bottom_target = 0.6f;
        hero.capsule_radius_target = 0.45f;
        break;
    }
    if (hero.state.control_mode == 4) {
        if (hero.state_timer > hero.takeoff_frame && hero.past_jump_apex == 0)
            hero.capsule_bottom_target = hero.unk434;
    } else if (hero.state.current == HERO_STATE_FALL) {
        hero.capsule_bottom_target = 0.5f;
    } else if (hero.state.current == HERO_STATE_CROUCH) {
        hero.capsule_top_target = 0.35000002f;
    } else if ((u32)(hero.state.control_mode - 0x11) < 2) {
        hero.capsule_top_target = 0.0f;
        hero.capsule_bottom_target = 0.0f;
    } else if (hero.state.current == HERO_STATE_SINK) {
        hero.capsule_radius_target = 0.8f;
    }
    if (!hero.wall_hit || hero.state.prev_control_mode == 0x12 || hero.state.control_mode == 0x11 ||
        hero.base_condition ||
        vector_distance_xy(hero.wall_contact_point.f, hero.motion.pos.f) >
            hero.capsule_radius * 0.5f) {
        f32         *smoothed_values = D_0013F570;
        struct Hero *hero_base = (struct Hero *)((u8 *)smoothed_values - 0x220);
        approach_value(hero_base->capsule_top_target, frame_scale * 0.02f, smoothed_values);
        approach_value(hero_base->capsule_bottom_target, frame_scale * 0.02f, smoothed_values + 1);
        vendor_spring(smoothed_values + 5,
                      smoothed_values + 6,
                      hero_base->capsule_radius_target,
                      frame_scale_sq * 0.02f,
                      frame_scale_sq * 0.3f,
                      frame_time * 4.0f);
    }
}

extern char  D_0013E533_c[] __asm__("D_0013E533");
extern float D_L01_0017C238[];
extern void  FUN_L00_002118c8(int, float);

void hero_speed_from_table(void) __asm__("FUN_L01_00232290");

void hero_speed_from_table(void) {
    char *x;
    FUN_L00_002118c8(0, 1.0f);
    x = D_0013E533_c + 0xE1D;
    if (0.0f < *(float *)(x + 0x190)) {
        if (*(float *)(x + 0x190) < D_L01_0017C238[3]) {
            *(float *)(x + 0x190) = D_L01_0017C238[2] * frame_time;
        } else {
            *(float *)(x + 0x190) = D_L01_0017C238[6] * frame_time;
        }
    }
    {
        char *y = D_0013E533_c + 0xE1D;
        if (*(int *)(y + 0x2084) == 0x73) {
            float scaled_value = *(float *)(y + 0x190) * 0.8f;
            float minimum_value = frame_time * 2.5f;
            *(float *)(y + 0x190) = scaled_value;
            if (scaled_value < minimum_value) {
                *(float *)(y + 0x190) = minimum_value;
            }
        }
    }
}
/* Checks the hero's movement against walls ahead and slides the direction along the wall. */
/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: func_L01_00232F90), where it is exact; names translated to the US level program. */

extern char  D_L01_001742C0_lui[] __asm__("D_L01_001742C0") NOT_SDA;
extern float gravity_length_xy(float *v) __asm__("FUN_L00_002339d0");
extern void  sceVu0Normalize(void *, void *) __asm__("FUN_00125180");

extern int D_L01_001742DC NOT_SDA;
void                      clip_hero_horizontal_velocity(int flag) __asm__("FUN_L01_00232978");

void clip_hero_horizontal_velocity(int flag) {
    float        slope_probe_start[4];
    float        slope_probe_end[4];
    float        material_probe_start[4];
    float        material_probe_end[4];
    struct Hero *hero_ptr = &hero;
    float        probe_height_offset, slope_probe_distance, hit_normal_elevation;
    int          k, x;
    struct Hero *material_probe_hero, *slope_probe_hero, *slide_hero;
    char        *slope_hit, *slide_hit;
    float       *slide_normal;

    if (hero_ptr->unk308 != 0) {
        return;
    }
    if (gravity_length_xy((float *)(&hero_ptr->motion.velocity)) < frame_time * 0.1f) {
        return;
    }
    probe_height_offset = 0.37f;
    slope_probe_distance = 0.5f;
    x = hero_ptr->state.control_mode;
    if (x >= 0x11 && x <= 0x12) {
        probe_height_offset = 0.0f;
        slope_probe_distance = 0.7f;
    }
    qcopy(slope_probe_start, &hero_ptr->motion.pos);
    slope_probe_start[2] += probe_height_offset;
    qcopy(material_probe_start, &hero_ptr->motion.pos);
    material_probe_start[2] += probe_height_offset * 0.3f;
    if (flag != 0 || vector_length_xy(&hero_ptr->motion.velocity) < 0.01f) {
        slope_probe_end[0] = fast_cos(hero_ptr->motion.rot.f[2]) * slope_probe_distance;
        slope_probe_end[1] = fast_sin(hero_ptr->motion.rot.f[2]) * slope_probe_distance;
        slope_probe_end[2] = 0.0f;
        material_probe_end[0] = fast_cos(hero_ptr->motion.rot.f[2]) * slope_probe_distance * 1.4f;
        material_probe_end[1] = fast_sin(hero_ptr->motion.rot.f[2]) * slope_probe_distance * 1.4f;
        material_probe_end[2] = 0.0f;
    } else {
        qcopy(slope_probe_end, &hero_ptr->motion.velocity);
        slope_probe_end[2] = 0.0f;
        fast_vec_scale(slope_probe_end,
                       slope_probe_end,
                       slope_probe_distance / vector_length_xy(slope_probe_end));
        qcopy(material_probe_end, slope_probe_end);
        fast_vec_scale(material_probe_end, material_probe_end, 1.4f);
    }
    fast_vec_add(slope_probe_end, slope_probe_end, slope_probe_start);
    k = -1;
    fast_vec_add(material_probe_end, material_probe_end, material_probe_start);
    material_probe_hero = &hero;
    if (collision_line(
            material_probe_start, material_probe_end, 4, (int)material_probe_hero->moby, 0) != 0) {
        if (D_L01_001742DC > 0) {
            k = collision_hit_material();
        }
    }
    hit_normal_elevation = 0.0f;
    slope_probe_hero = &hero;
    if (collision_line(slope_probe_start, slope_probe_end, 4, (int)slope_probe_hero->moby, 0) !=
        0) {
        slope_hit = D_L01_001742C0_lui;
        if (*(int *)(slope_hit + 0x1C) > 0) {
            hit_normal_elevation =
                fast_arc_tan(*(float *)(slope_hit + 0x48), vector_length_xy(slope_hit + 0x40));
        }
    }
    if (hit_normal_elevation >= 0.87266463f || k == 8 || k == 12) {
        slide_hero = &hero;
        slide_hit = D_L01_001742C0_lui;
        slide_normal = (float *)(slide_hit + 0x40);
        *(int *)(slide_hit + 0x48) = 0;
        slide_hero->unk240 = *(int *)(slide_hit + 0x18);
        sceVu0Normalize(slide_normal, slide_normal);
        {
            float c = -slide_hero->motion.velocity.f[0] * *(float *)(slide_hit + 0x40) -
                      slide_hero->motion.velocity.f[1] * *(float *)(slide_hit + 0x44);
            if (c > 0.0f) {
                fast_vec_scale(slide_normal, slide_normal, c);
                fast_vec_add(
                    &slide_hero->motion.velocity, &slide_hero->motion.velocity, slide_normal);
            }
        }
    }
}

typedef union {
    OvlQuad q;
    f32     f[4];
} V;

extern u8 D_L01_001742C0[] NOT_SDA;
extern u8 D_L01_001742E0[] NOT_SDA;

void hero_get_collision_ray_endpoints(void *, void *, s32, f32, f32) __asm__("FUN_L00_002126b8");
s32  FUN_001efa68_c(void *, void *, s32, s32, s32) __asm__("FUN_001efa68");
s32  coll_sound_class(void) __asm__("FUN_L00_001f3608");
f32  resolve_camera_surface_height(void *, void *) __asm__("FUN_002135f0");
s32  is_moby_in_class_range_500_to_540(s32) __asm__("FUN_L00_0025e3b8");
void FUN_00125180_c(void *, void *) __asm__("FUN_00125180");
f32  compute_gravity_relative_vector_inclination(void *) __asm__("FUN_L00_00233d30");
void compute_rotated_vector_angles(void *, void *) __asm__("FUN_L00_00216078");
void transform_hero_polar_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_002335c8");

extern CollisionHit HH __asm__("D_L01_001742C0") NOT_SDA;
#define HI(o) (*(s32 *)(D_L01_001742C0 + (o)))
#define HF(o) (*(f32 *)(D_L01_001742C0 + (o)))

void update_hero_ground_contact_and_gravity(void) __asm__("FUN_L01_00232dc0");

void update_hero_ground_contact_and_gravity(void) {
    V collision_ray_end, ground_ray_start, ground_delta_or_angles, side_ray_start,
        unused_side_probe_point, side_hit_angles, normalized_side_hit_normal;
    f32 f;
    s32 collision_sound_class;
    s32 i;
    f32 side_probe_angle;
    u8 *hit_moby_record;

    s32 k;
    f32 ground_ray_start_offset;

    hero.ground_normal.f[2] = 1.0f;
    hero.air_frames.u++;
    hero.frames_off_ground.u++;
    hero.ground_normal.i[0] = 0;
    hero.ground_normal.i[1] = 0;
    hero.ground_slope.i = 0;
    hero.ground_z.i = 0;
    clear_u64_value((u8 *)&hero.ground_point);
    hero.on_flagged_ground_moby = 0;
    hero.ground_distance = 32.0f;
    hero.ground_moby = 0;
    if (hero.in_water_frames != 0) {
        hero.in_water_frames++;
    }
    k = 0;
    if (hero.gravity_mode)
        k = 1;
    ground_ray_start_offset = hero.capsule_radius * 2.2f;
    if (hero.state.control_mode == 2) {
        ground_ray_start_offset = hero.capsule_radius * 2.9f;
    }
    hero_get_collision_ray_endpoints(
        &ground_ray_start, &collision_ray_end, k, ground_ray_start_offset, -48.0f);
    if (FUN_001efa68_c(&ground_ray_start, &collision_ray_end, 2, (s32)hero.moby, 0) == 0) {
        goto tail;
    }
    collision_sound_class = coll_sound_class();
    if (collision_sound_class == -1) {
        collision_sound_class = 3;
    }
    hero.ground_sound_class = collision_sound_class;
    hero.ground_type = collision_hit_material();
    if (hero.ground_type == 0) {
        hero.water_surface_z =
            resolve_camera_surface_height(D_L01_001742E0, (u8 *)&hero.water_surface_normal);
        if (hero.motion.pos.f[2] < hero.water_surface_z && hero.in_water_frames <= 0) {
            hero.in_water_frames = 1;
        }
        if (FUN_001efa68_c(&ground_ray_start, &collision_ray_end, 0x24, (s32)hero.moby, 0) == 0) {
            goto d1;
        }
        collision_sound_class = coll_sound_class();
        if (current_level_index == 1 || current_level_index == 0x12) {
            collision_sound_class = 3;
        }
        hero.ground_sound_class = collision_sound_class;
    }
    if (hero.ground_type == 0xD) {
        hero.water_surface_z = HH.point.f[2];
        ground_ray_start.f[2] = HH.point.f[2] - 0.01f;
        if (FUN_001efa68_c(&ground_ray_start, &collision_ray_end, 4, (s32)hero.moby, 0) == 0) {
            goto d1;
        }
    }
    if (hero.ground_type == 0xB) {
        hero.sink_surface_z = HH.point.f[2];
        goto d1;
    }
    if (HH.moby != 0) {
        if (is_moby_in_class_range_500_to_540(HH.moby)) {
            hero.on_flagged_ground_moby = 1;
        }
        hit_moby_record = suck_get_aim_height(HH.moby);
        if (hit_moby_record != 0 && (*(u16 *)(hit_moby_record + 0x1E) & 8)) {
            hero.on_flagged_ground_moby = 1;
        }
    }
    if (HH.unk1C > 0) {
        qcopy((u8 *)&hero.ground_point, &HH.point);
        hero.ground_z.f = HH.point.f[2];
        hero.ground_distance = fast_vec_dist((u8 *)&hero.motion.pos, (u8 *)&hero.ground_point);
        hero.ground_moby = (struct Moby *)HH.moby;
        FUN_00125180_c((u8 *)&hero.ground_normal, (u8 *)&HH.normal_x);
        hero.ground_slope.f = compute_gravity_relative_vector_inclination((u8 *)&HH.normal_x);
        hero.downhill_yaw = fast_arc_tan(HH.normal_x, HH.normal_y);
        fast_vec_sub(&ground_delta_or_angles, (u8 *)&hero.ground_point, (u8 *)&hero.motion.pos);
        if (0.0f < fast_vec_dot(&ground_delta_or_angles, (u8 *)&hero.ground_normal)) {
            hero.ground_distance = -hero.ground_distance;
        }
        if (hero.ground_distance < 0.02f) {
            hero.frames_off_ground.s = 0;
            if (hero.ground_slope.f <= 0.87266463f || hero.gravity_mode == 1 ||
                hero.state.control_mode == 0x16) {
                hero.air_frames.s = 0;
                hero.last_landed_moby = HH.moby;
            }
        }
    }
tail:
    if (hero.air_frames.s != 0) {
        hero.grounded_frames = 0;
    } else {
        hero.grounded_frames++;
        qcopy((u8 *)&hero.last_ground_point, (u8 *)&hero.ground_point);
    }
    if (hero.motion.pos.f[2] > hero.water_surface_z) {
        hero.in_water_frames = 0;
    }
d1:
    clear_u64_value((u8 *)&hero.gravity_dir);
    switch (hero.gravity_mode) {
    case HERO_GRAVITY_WORLD_DOWN:
        hero.gravity_dir.f[2] = -1.0f;
        break;
    case HERO_GRAVITY_OPPOSITE_GROUND_NORMAL:
        qcopy((u8 *)&hero.gravity_dir, (u8 *)&hero.ground_normal);
        fast_vec_scale((u8 *)&hero.gravity_dir, (u8 *)&hero.gravity_dir, -1.0f);
        break;
    case HERO_GRAVITY_TOWARD_REFERENCE_POINT:
        fast_vec_sub(
            (u8 *)&hero.gravity_dir, (u8 *)&hero.motion.gravity_point, (u8 *)&hero.motion.pos);
        fast_vec_normalize((u8 *)&hero.gravity_dir, (u8 *)&hero.gravity_dir, 1.0f);
        break;
    }
    hero.ground_pitch.i = 0;
    hero.unk2E8.i = 0;
    hero.unk2D0.i[0] = 0;
    hero.unk2D0.i[1] = 0;
    hero.unk2C8.i[0] = 0;
    hero.unk2C8.i[1] = 0;
    hero.unk2C0.i[0] = 0;
    hero.unk2C0.i[1] = 0;
    if (hero.state.control_mode == 0xF || hero.state.control_mode == 0x15 ||
        hero.state.control_mode == 6 || hero.state.control_mode == 4 ||
        hero.state.control_mode == 5 || hero.state.control_mode == 3) {
        return;
    }
    if (!(hero.ground_distance < 0.25f)) {
        return;
    }
    compute_rotated_vector_angles((u8 *)&hero.ground_normal, &ground_delta_or_angles);
    if (-0.78539819f < ground_delta_or_angles.f[1] && ground_delta_or_angles.f[1] < 0.78539819f) {
        hero.ground_pitch.f = ground_delta_or_angles.f[1];
    }
    if (-0.78539819f < ground_delta_or_angles.f[0] && ground_delta_or_angles.f[0] < 0.78539819f) {
        hero.unk2E8.f = ground_delta_or_angles.f[0];
    }
    for (i = 0; i < 2; i++) {
        side_probe_angle = 1.5707964f;
        if (i == 1) {
            side_probe_angle = -side_probe_angle;
        }
        transform_hero_polar_offset_to_world(&side_ray_start, 0.2f, side_probe_angle, 0.45f);
        transform_hero_polar_offset_to_world(
            &unused_side_probe_point, 0.2f, side_probe_angle, -0.45f);
        if (FUN_001efa68_c(&side_ray_start, &collision_ray_end, 0x22, (s32)hero.moby, 0)) {
            f = HH.point.f[2] - hero.ground_z.f;
            hero.unk2C0.f[i] = f;
            if (0.4f < absolute_float(f)) {
                hero.unk2C0.f[i] = 0.0f;
            }
            FUN_00125180_c(&normalized_side_hit_normal, (u8 *)&HH.normal_x);
            compute_rotated_vector_angles(&normalized_side_hit_normal, &side_hit_angles);
            if (-0.78539819f < side_hit_angles.f[1] && side_hit_angles.f[1] < 0.78539819f) {
                hero.unk2C8.f[i] = side_hit_angles.f[1];
            }
            if (-0.78539819f < side_hit_angles.f[0] && side_hit_angles.f[0] < 0.78539819f) {
                hero.unk2D0.f[i] = side_hit_angles.f[0];
            }
        } else {
            hero.unk2C0.i[i] = 0;
            hero.unk2C8.f[i] = hero.ground_pitch.f;
            hero.unk2D0.f[i] = hero.unk2E8.f;
        }
    }
}

typedef unsigned int u128_233940 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_233940 q;
    f32         f[4];
} V_233940;

extern void        f2347c0_233940(void) __asm__("FUN_L00_002347c0");
extern void        f233810_233940(void *, void *, f32) __asm__("FUN_L00_00233810");
extern void        f2334d0_233940(void *, void *, f32) __asm__("FUN_L00_002334d0");
extern s32         coll_sphere(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern s32         coll3_233940(void *, f32) __asm__("FUN_L00_001f3170");
extern u128_233940 D_L01_001742F0[] NOT_SDA;
s32 resolve_hero_position_with_collision_queries(s32 mode) __asm__("FUN_L01_00233940");

s32 resolve_hero_position_with_collision_queries(s32 mode) {
    V_233940     old;
    V_233940     collision_query_offset;
    V_233940     resolved_position_delta;
    s32          mask;
    s32          i;
    u128_233940 *collision_result_position;
    f32          query_height_or_displacement_length;
    if (hero.collision_off_frames != 0) {
        return 1;
    }
    qcopy(&old, &hero.motion.pos.q);
    clear_u64_value(&collision_query_offset);
    if (hero.path_constrain_pending != 0) {
        f2347c0_233940();
        hero.path_constrain_pending = 0;
    }
    if (hero.state.control_mode != 0x11) {
        if (hero.gravity_mode != 0 || hero.unk1F8 != 0) {
            if (hero.gravity_mode == 1 || hero.unk1F8 != 0) {
                f233810_233940(&collision_query_offset, &collision_query_offset, 0.6f);
            } else {
                f2334d0_233940(
                    &collision_query_offset, &collision_query_offset, -hero.capsule_bottom);
            }
        } else {
            collision_query_offset.f[2] = hero.capsule_bottom;
        }
    }
    fast_vec_add(&hero.motion.pos.q, &hero.motion.pos.q, &collision_query_offset);
    mask = 0x24;
    if (hero.state.current == HERO_STATE_SINK) {
        mask = 0xD24;
    }
    for (i = 0; i < 8; i++) {
        if (hero.gravity_mode != 0) {
            if (!coll_sphere(&hero.motion.pos.q, mask, ((s32)hero.moby), frame_scale * 0.4f))
                break;
        } else if (hero.state.control_mode == 0x11) {
            if (!coll_sphere(&hero.motion.pos.q, mask, ((s32)hero.moby), 0.6f))
                break;
        } else {
            s32 collision_query_result;
            query_height_or_displacement_length = hero.capsule_top - hero.capsule_bottom;
            if (query_height_or_displacement_length < 0.05f) {
                query_height_or_displacement_length = 0.05f;
            }
            collision_query_result = rig_probe(&hero.motion.pos.q,
                                               mask,
                                               hero.capsule_radius,
                                               ((s32)hero.moby),
                                               query_height_or_displacement_length);
            if (!(collision_query_result | coll3_233940(&hero.motion.pos.q, hero.capsule_radius)))
                break;
        }
        collision_result_position = D_L01_001742F0;
        qcopy(&hero.motion.pos.q, collision_result_position);
        qcopy(&hero.wall_contact_normal.q, collision_result_position + 1);
        qcopy(&hero.wall_contact_point.q, collision_result_position - 1);
        hero.wall_hit = 1;
        hero.coll_hit_moby = ((u8 **)collision_result_position)[-6];
    }
    fast_vec_sub(&hero.motion.pos.q, &hero.motion.pos.q, &collision_query_offset);
    fast_vec_sub(&resolved_position_delta, &hero.motion.pos.q, &old);
    query_height_or_displacement_length = fast_vec_length(&resolved_position_delta);
    if (hero.capsule_radius * 1.5f < query_height_or_displacement_length) {
        if (mode == 0xF) {
            if (512.0f < resolved_position_delta.f[0])
                resolved_position_delta.f[0] = 512.0f;
            else if (resolved_position_delta.f[0] < -512.0f)
                resolved_position_delta.f[0] = -512.0f;
            if (512.0f < resolved_position_delta.f[1])
                resolved_position_delta.f[1] = 512.0f;
            else if (resolved_position_delta.f[1] < -512.0f)
                resolved_position_delta.f[1] = -512.0f;
            if (512.0f < resolved_position_delta.f[2])
                resolved_position_delta.f[2] = 512.0f;
            else if (resolved_position_delta.f[2] < -512.0f)
                resolved_position_delta.f[2] = -512.0f;
            fast_vec_normalize(
                &resolved_position_delta, &resolved_position_delta, hero.capsule_radius);
            fast_vec_add(&hero.motion.pos.q, &old, &resolved_position_delta);
        }
        return -1;
    }
    return 1;
}

#include "eetypes.h"
#include "rnc/input/pad_state.h"

typedef struct {
    s32 w0;
    u8  pad4[0x70 - 4];
} Entry70;

extern u8 item_available[] __asm__("D_0013D4C0");

extern void update_hero_tilt(void) __asm__("FUN_L00_002165b8");
extern void exit_hero_vehicle_mode_and_restore_moby(void) __asm__("FUN_L01_00231450");
extern s32  pad_pressed_within(s32, s32, s32) __asm__("FUN_L00_00266d60");
extern f32  gravity_z(void *) __asm__("FUN_L00_00233a78");
extern void update_hero_planar_distance_spring_motion(void) __asm__("FUN_L00_00221b68");
extern void transform_hero_local_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_00233660");
extern void hero_respawn(void) __asm__("FUN_L00_00211250");

extern Entry70 l01_countdown_timer_records[] __asm__("D_L01_00179D70");
void           update_hero_timers_and_vehicle_transitions(void) __asm__("FUN_L01_0023c710");

void update_hero_timers_and_vehicle_transitions(void) {
    Vec4         sum;
    Vec4         sample_delta;
    struct Moby *vehicle_moby;
    s32          i;
    s32          sample_index_base;
    s32          sample_count;

    if (hero.tilt_disabled == 0) {
        update_hero_tilt();
    }
    hero.state_timer++;
    hero.control_mode_timer++;
    hero.step_frames++;
    hero.anim_seq_frames++;
    fast_dec_timer(&hero.move_lock_timer);
    fast_dec_timer(&hero.invulnerable_frames);
    fast_dec_timer_s16(&hero.unk1B2);
    fast_dec_timer(&hero.long_jump_lockout);
    fast_dec_timer_s16(&hero.jump_lockout_timer);
    fast_dec_timer_s16(&hero.unk1DE);
    fast_dec_timer(&hero.wall_jump_window);
    fast_dec_timer_s16(&hero.unk1F0);
    fast_dec_timer_s16(&hero.ledge_probe_cooldown);
    fast_dec_timer_s16(&hero.unk1C8);
    fast_dec_timer_s16(&hero.rail_catch_grace);
    fast_dec_timer_s16(&hero.rail_grab_cooldown);
    fast_dec_timer_s16(&hero.grapple_lockout_timer);
    fast_dec_timer(&hero.collision_off_frames);
    fast_dec_timer(&hero.unk1D0);
    fast_dec_timer(&hero.jump_buffer_lockout);
    fast_dec_timer_s16(&hero.fidget_cooldown);
    fast_dec_timer_s16(&hero.unk1E0);
    fast_dec_timer_s16(&hero.unk1EE);
    fast_dec_timer_s16(&hero.unk1EA);
    fast_dec_timer_s16(&hero.unk1F4);
    fast_dec_timer_s16(&hero.jump_lock_timer);
    fast_dec_timer_s16(&hero.unk1DC);
    fast_dec_timer_s16(&hero.unk1F6);
    fast_dec_timer_s16(&hero.unk1DA);
    fast_dec_timer_s16(&hero.unk1F8);
    if (!(controller_state.held & 5)) {
        hero.unk1EC = 0;
    }
    if (hero.item_anim_active != 0) {
        hero.item_anim_frames++;
    } else {
        hero.item_anim_frames = 0;
    }
    if (controller_state.no_direction != 0) {
        hero.stick_held_frames = 0;
        hero.stick_idle_frames++;
    } else {
        hero.stick_idle_frames = 0;
        hero.stick_held_frames++;
    }
    if (hero.vehicle_toggle_timer != 0 && fast_dec_timer_s16(&hero.vehicle_toggle_timer) != 0 &&
        hero.health.hp != 0 && cutscene_mode == 0) {
        if (hero.vehicle_mode == 3) {
            exit_hero_vehicle_mode_and_restore_moby();
            if (pad_pressed_within(0x80, scale_game_frames(0x12), 0)) {
                hero.force_slot0_item8 = 1;
            }
        } else {
            vehicle_moby = create_moby(0x27A);
            hero.vehicle_mode3_moby = vehicle_moby;
            if (vehicle_moby != 0) {
                qcopy(&vehicle_moby->pos, &hero.motion.pos);
                qcopy(&vehicle_moby->rot, &hero.motion.rot);
                vehicle_moby->draw_dist = 0x40;
                ((struct Moby *)hero.vehicle_mode3_moby)->visible = 1;
                ((struct Moby *)hero.vehicle_mode3_moby)->glow_color = 0;
                ((struct Moby *)hero.vehicle_mode3_moby)->spawn_frame = hero.moby->spawn_frame;
                update_entity(hero.vehicle_mode3_moby);
                enter_hero_vehicle_mode_and_set_moby(3, 0x53, hero.vehicle_mode3_moby);
            }
        }
    }
    for (i = 0; i < 5; i++) {
        fast_dec_timer(&l01_countdown_timer_records[i].w0);
    }
    hero.current_path_dist = 9999.0f;
    if (hero.air_frames.s != 0 && gravity_z(&hero.motion.velocity) < 0.0f) {
        if (hero.pos_history_count != 0x20)
            goto skip;
        clear_u64_value(&sum);
        sample_count = 8;
        for (i = 0; i < sample_count; i++) {
            sample_index_base = hero.pos_history_index - i;
            {
                s32 sample_index = (sample_index_base + 0x1F) % 32;
                s32 subtrahend_sample_index = (sample_index_base + 0x1E) % 32;
                fast_vec_sub(&sample_delta,
                             &hero.pos_history[sample_index],
                             &hero.pos_history[subtrahend_sample_index]);
            }
            fast_vec_add(&sum, &sum, &sample_delta);
        }
        fast_vec_scale(&sum, &sum, 1.0f / (f32)sample_count);
        if (fast_vec_length(&sum) < frame_time) {
            hero.still_frames++;
            goto skip;
        }
    }
    hero.still_frames = 0;
skip:
    update_hero_planar_distance_spring_motion();
    transform_hero_local_offset_to_world(&hero.motion.body_center, 0.0f, 0.0f, 0.7f);
    if (item_available[0x22]) {
        hero.reach_distance = 12.0f;
        hero.reach_height = 4.5f;
    } else {
        hero.reach_distance = 3.0f;
        hero.reach_height = 1.75f;
    }
    {
        f32 z = hero.ground_z.f;
        if (hero.motion.pos.f[2] - z < 4.0f) {
            qcopy(&hero.motion.near_ground_point, &hero.motion.pos);
            hero.motion.near_ground_point.f[2] = z + 0.5f;
        } else {
            qcopy(&hero.motion.near_ground_point, &hero.motion.body_center);
        }
    }
    if (hero.motion.pos.f[2] < 1.0f) {
        hero_respawn();
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: func_L01_00240CE8), where it is exact; names translated to the US level program. */

void request_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L01_002405a0");

void request_hero_idle_state_for_vehicle_mode(void) {
    switch ((unsigned char)D_0013E533[0x2EC1]) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        l01_hero_set_state(HERO_STATE_IDLE, 1);
        break;
    case 3:
        l01_hero_set_state(0x53, 1);
        break;
    }
}

extern s32 pad_held __asm__("D_0013CAE0") NOT_SDA;
void       set_hero_animation(s32, s32, f32) __asm__("FUN_L00_002323b8");
void       stick_target(s32, f32) __asm__("FUN_L00_002118c8");
void       hero_queue_sequence(s32, s32) __asm__("FUN_L00_002325e0");
void       FUN_L00_0025f730_242198(void *, f32) __asm__("FUN_L00_0025f730");
s32        get_hero_idle_animation_index(s32) __asm__("FUN_L01_00226f10");
void       select_hero_locomotion_state(void) __asm__("FUN_L01_00242198");

void select_hero_locomotion_state(void) {
    struct Hero *hero_ptr, *run_hero, *stop_idle_hero;
    s32          run_frames_or_idle_selector;
    hero_ptr = &hero;
    hero_ptr->unk2284 = 0;
    stick_target(0, 1.0f);
    if (pad_held & 0xA) {
        l01_hero_set_state(HERO_STATE_CROUCH, 0);
        set_hero_animation(0xD, 2, -2.0f);
        hero_queue_sequence(3, 0x12);
        return;
    }
    run_hero = &hero;
    if (0.3f < run_hero->stick_magnitude && run_hero->move_lock_timer == 0) {
        if (run_hero->on_ice_ground != 0 || run_hero->wading != 0) {
            l01_hero_set_state(HERO_STATE_RUN, 1);
            return;
        }
        if (l01_hero_set_state(HERO_STATE_RUN, 0)) {
            run_hero->state.step = 1;
            run_frames_or_idle_selector = scale_game_frames(8);
            if (scale_game_frames(5) < run_hero->rail_land_frames)
                run_frames_or_idle_selector = scale_game_frames(14);
            set_hero_animation(4, D_L01_0015F6DC, (f32)run_frames_or_idle_selector);
            run_hero->run_start_blending = 1;
        }
        return;
    }
    stop_idle_hero = &hero;
    if ((frame_time * 3.0f < stop_idle_hero->motion.forward_speed &&
         scale_game_frames(3) < stop_idle_hero->rail_land_frames) ||
        stop_idle_hero->state.current == 0x10) {
        if (l01_hero_set_state(HERO_STATE_STOP, 0)) {
            set_hero_animation(5, 0, -1.0f);
            FUN_L00_0025f730_242198(&hero.motion.momentum, frame_time * 5.0f);
        }
        return;
    }
    if (scale_game_frames(12) < stop_idle_hero->rail_land_frames ||
        (stop_idle_hero->anim_flags & 2)) {
        if (l01_hero_set_state(HERO_STATE_IDLE, 0)) {
            if (get_hero_idle_animation_index(0) == 0x54) {
                run_frames_or_idle_selector = get_hero_idle_animation_index(0);
                set_hero_animation(run_frames_or_idle_selector, 0, (f32)scale_game_frames(15));
            } else if (stop_idle_hero->anim_flags & 2) {
                set_hero_animation(get_hero_idle_animation_index(0), 0, -2.0f);
            }
        }
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: func_L01_002490E0), where it is exact; names translated to the US level program. */

void set_hero_moby_flag_bit_zero(void) __asm__("FUN_L01_002488e8");

void set_hero_moby_flag_bit_zero(void) {
    *(unsigned short *)((char *)(*(char **)(D_0013E533 + 0x2E9D)) + 0x34) |= 1;
}

void clear_hero_moby_flag_bit_zero(void) __asm__("FUN_L01_00248900");

void clear_hero_moby_flag_bit_zero(void) {
    *(unsigned short *)((char *)(*(char **)(D_0013E533 + 0x2E9D)) + 0x34) &= ~1;
}

int D_L02_0017445C NOT_SDA = 0;
int D_L03_00173F5C NOT_SDA = 0;
int D_L05_0017425C NOT_SDA = 0;
int D_L08_0017465C NOT_SDA = 0;
int D_L11_0017485C NOT_SDA = 0;
int D_L12_001742DC NOT_SDA = 0;
int D_L14_0017455C NOT_SDA = 0;
int D_L16_0017425C NOT_SDA = 0;
