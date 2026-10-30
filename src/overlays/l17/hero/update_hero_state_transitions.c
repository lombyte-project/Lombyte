/* Level 17 hero state transitions: update_hero_state_transitions. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/hero.h"
#include "rnc/overlay/quad.h"
#include "rnc/input/pad_state.h"
#include "rnc/hero/hero.h"
#include "rnc/game/usage_stats.h"
#include "rnc/data_labels.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/l17/hero/hero_set_state.h"
#include "rnc/overlay/shared/weapons/wrench.h"
#include "rnc/overlay/shared/hero/update_hero_vehicle_attachments_and_effects.h"
#include "rnc/overlay/shared/hero/try_transition_hero_water_state.h"

/* Hero state machine: per-frame transition logic for the current hero state (level 17's build of the shared func_L00_0022A340, without states 90-97). */
/* Ported from rac1-decomp (src/overlays/l17_fleet/help_002260F0.c: func_L17_002260F0), where it is exact; names translated to the US level program. */

typedef struct {
    char  p0[0x254];
    float f254;
    float f258;
} S17B000;

typedef struct {
    char  p0[0xC];
    float fC;
    char  p1[8];
    float f18;
} S17C738;

typedef struct {
    char           p0[0x74];
    unsigned char  f74;
    char           p1[0x13];
    unsigned char *f88;
} Row13E650;

extern HeroTableRow2C       D_L17_0017C528[];
extern HeroAnimRow          D_L17_0017A3C0[];
extern HeroTableEntry70     l17_hero_idle_fidget_entries[] __asm__("D_L17_0017A190");
extern S17B000              D_L17_0017AF80;
extern S17C738              D_L17_0017C6B8;
extern float                D_L17_0015F638;
extern float D_L17_001676D8 NOT_SDA;
extern float                scale_time(float) __asm__("FUN_001f96b0");
extern float                adjust_hero_animation_speed_for_target_frame(float a,
                                                                         float b,
                                                                         float c,
                                                                         float d) __asm__("FUN_L00_00208f20");
extern float
predict_hero_trajectory_intercept_time(float a, float b, float c) __asm__("FUN_L00_00232b90");
extern float          gravity_z(float *v) __asm__("FUN_L00_00233a78");
extern float          predict_hero_trajectory_z(float *pos,
                                                float *vel,
                                                int   *steps,
                                                float  step) __asm__("FUN_L00_00233c30");
extern float          randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float          calculate_triangular_corrected_scalar(float *a,
                                                            float *b,
                                                            int   *out,
                                                            float  f) __asm__("FUN_L00_00259468");
extern float          FUN_L00_0025abf0(void *a, void *b, void *out, float speed, float g);
extern float          read_pad_float_history(int) __asm__("FUN_L00_00266e80");
extern float          approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float          fast_cos(float);
extern float          fast_sin(float);
extern float          vector_length_xy(void *);
extern int D_00141660 NOT_SDA;
int                   r1_repeat_timer __asm__("D_L17_0015F678") = 0;
extern int            D_L17_0015F6DC_d __asm__("D_L17_0015F6DC");
extern int            D_L17_0015F594 __attribute__((sda));
extern int D_L17_0016D080 NOT_SDA;
extern int                collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int                return_hero_animation_id_unchanged(int) __asm__("FUN_L00_002050b8");
extern int                probe_hero_ledge_edge(float *, float *) __asm__("FUN_L00_0020c758");

#include "rnc/overlay/path.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"
#include "rnc/audio/voice_pool.h"

extern int nearest_rail_point() __asm__("FUN_L00_0020cd08");
extern int hero_slot_item_id(int) __asm__("FUN_L00_0020d498");
extern int is_wrench_moby(void *) __asm__("FUN_L00_00211360");
extern int test_hero_animation_frame_window(float t) __asm__("FUN_L00_00211870");
extern int get_hero_high_jump_input_window_frames(void) __asm__("FUN_L00_00221d98");
extern int try_start_hero_weapon_fire_state(void) __asm__("FUN_L00_002276c0");
extern int try_enter_hero_water_state(void) __asm__("FUN_L00_002279b0");
extern int process_hero_equipped_item_input(void) __asm__("FUN_L00_00227fa0");
extern int try_start_hero_jump(int, int) __asm__("FUN_L00_00228b78");
extern int pick_idle_fidget(int *) __asm__("FUN_L00_00229010");
extern int hero_handle_jump_press(void) __asm__("FUN_L00_00229660");
extern int check_hero_forward_collision_height(void) __asm__("FUN_L00_00229910");
extern int try_start_hero_item_idle_animation(void) __asm__("FUN_L00_00229a98");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int
sample_path(int *tab, float *out, int *a, float *b, int c, float d) __asm__("FUN_L00_0025d808");
extern int FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);
extern int pad_pressed_within(int, int, int *) __asm__("FUN_L00_00266d60");
extern int check_pad_press_separation(int, int, int, int) __asm__("FUN_L00_00266e00");
extern int select_hero_help_value(int a) __asm__("FUN_L17_002020a8");
extern int process_hero_hit_response(int) __asm__("FUN_L17_0020dbe8");
extern int queue_animation_update(int, int, int, int, int, int, int) __asm__("FUN_001ff308");

#include "rnc/game/item_state.h"

extern void
emit_hero_animation_voice_and_ground_particle(int a0, int a1, int a2) __asm__("FUN_L00_00205f60");
extern void release_and_clear_hero_voice_slots(void) __asm__("FUN_L00_00206c08");
extern void emit_hero_surface_effect_burst(int, int, int) __asm__("FUN_L00_00209ca8");
extern void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
extern void hero_switch_item(int i) __asm__("FUN_L00_0020fea0");
extern void hero_respawn(void) __asm__("FUN_L00_00211250");
extern void stick_target(int, float) __asm__("FUN_L00_002118c8");
extern void update_hero_target_speed_from_stick(void) __asm__("FUN_L00_00211be8");
extern void allocate_hero_voice_for_entry(int, int) __asm__("FUN_L00_00216de8");
extern void update_hero_animation_for_movement_speed(void) __asm__("FUN_L00_00228c18");
extern void
remove_hero_cached_moby_and_clear_slot_three_moby_flags(void) __asm__("FUN_L00_00228fa8");
extern void update_hero_locomotion_state(void) __asm__("FUN_L00_002293a8");
extern void blend_hero_slot3_animation_pair(void) __asm__("FUN_L00_00231d08");
extern void set_hero_animation(int, int, float) __asm__("FUN_L00_002323b8");
extern void clear_hero_queued_animation(void) __asm__("FUN_L00_00232628");
extern void hero_set_attack_ticks(int id) __asm__("FUN_L00_00232640");
extern void configure_hero_point_effect_emission(float         a,
                                                 float         b,
                                                 short         c,
                                                 unsigned char d,
                                                 unsigned char e,
                                                 unsigned char f) __asm__("FUN_L00_00233288");
extern void gravity_set_z(float *dst, float *src, float z) __asm__("FUN_L00_00233b20");
extern void hud_keep_visible(int, int) __asm__("FUN_L00_00235e18");
extern void initialize_eight_slot_hud_selector(void) __asm__("FUN_L00_00237ae8");
extern void update_hud_item_selector_selection_and_visibility(void) __asm__("FUN_L00_00237ba0");
extern void draw_hud_item_selector_panel(void) __asm__("FUN_L00_002382a0");
extern void initialize_four_slot_hud_selector(void) __asm__("FUN_L00_00238b18");
extern void update_hud_radial_selector_selection_and_visibility(void) __asm__("FUN_L00_00238b80");
extern void draw_hud_radial_selector_panel(void) __asm__("FUN_L00_00238f88");
extern void initialize_bolt_counter_hud(void) __asm__("FUN_L00_00239cc8");
extern void update_bolt_counter_hud_animation_and_visibility(void) __asm__("FUN_L00_00239d00");
extern void draw_bolt_counter_hud_panel(void) __asm__("FUN_L00_00239df8");
extern void FUN_L00_0025f730(void *a, float x);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
void        show_health_hud(void) __asm__("FUN_L00_00234e00");
extern int  func_L00_00235790_r(void) __asm__("FUN_L00_00234e00");

void update_hero_state_transitions(void) __asm__("FUN_L17_002258a8");

void update_hero_state_transitions(void) {
    unsigned char *moby;
    int            prevSub;
    int            prevState;
    int            prevAnim;
    float          query_position[4] __attribute__((aligned(16)));
    float          spatial_query_vector[4] __attribute__((aligned(16)));
    int            record_index;
    int            respawn_jump_press_age_frames;
    int            path_seg;
    float          path_t;
    int            new_path;
    int            new_seg;
    int            new_t;
    int            new_loop;
    int            grind_jump_apex_steps;
    int            grind_recovery_apex_steps;
    float          ledge_hang_probe_yaw;
    float          ledge_positive_side_probe_yaw;
    float          ledge_negative_side_probe_yaw;
    float          ledge_shimmy_probe_yaw;
    int            jump_apex_steps;
    int            airborne_jump_press_age_frames;
    int            predicted_apex_steps;
    float          alt_ledge_hang_probe_yaw;
    float          alt_ledge_positive_side_probe_yaw;
    float          alt_ledge_negative_side_probe_yaw;
    float          alt_ledge_shimmy_probe_yaw;

    moby = ((unsigned char *)hero.moby);
    prevSub = hero.state.control_mode;
    prevState = hero.state.step;
    prevAnim = moby[0x53];
    if (process_hero_hit_response(1) == 0) {
        if (hero.health.hp <= 0 && hero.state.control_mode != 0x14 &&
            (hero.state.current == HERO_STATE_IDLE || hero.state.current == HERO_STATE_RUN)) {
            hero_set_state(HERO_STATE_DYING, 1);
        } else if (process_hero_equipped_item_input() == 0 && try_enter_hero_water_state() == 0 &&
                   try_transition_hero_water_state() == 0 &&
                   try_start_hero_weapon_fire_state() == 0) {
            if ((controller_state.pressed & 0x10) || (controller_state.pressed_unmasked & 0x10)) {
                if (hero.items[0].switch_lock != 2 && D_L17_0015F594 == 0 &&
                    (hero.item_use_locked == 0 || hero.vehicle_mode == 1 || hero.unk1B2 != 0) &&
                    (hero.item_select_blocked == 0 || hero.vehicle_mode == 1 || hero.unk1B2 != 0)) {
                    switch (hero.vehicle_mode) {
                    case HERO_VEHICLE_MODE_ON_FOOT:
                    case 3: {
                        int id;

                        if (hero.quick_swap_tap_window != 0) {
                            hero.item_selector_timer = scale_game_frames(0x1B);
                        } else {
                            hero.item_selector_timer = scale_game_frames(0xF);
                        }
                        queue_animation_update(
                            3,
                            0,
                            (int)initialize_eight_slot_hud_selector,
                            (int)update_hud_item_selector_selection_and_visibility,
                            (int)draw_hud_item_selector_panel,
                            0,
                            0);
                        id = func_L00_00235790_r();
                        hud_keep_visible(id, scale_game_frames(0xB4));
                        id = queue_animation_update(
                            2,
                            0x754E,
                            (int)initialize_bolt_counter_hud,
                            (int)update_bolt_counter_hud_animation_and_visibility,
                            (int)draw_bolt_counter_hud_panel,
                            (int)&current_bolt_count,
                            0x98967F);
                        hud_keep_visible(id, scale_game_frames(0xB4));
                    } break;
                    case HERO_VEHICLE_MODE_CLANK:
                        queue_animation_update(
                            3,
                            0,
                            (int)initialize_four_slot_hud_selector,
                            (int)update_hud_radial_selector_selection_and_visibility,
                            (int)draw_hud_radial_selector_panel,
                            0,
                            0);
                        break;
                    }
                }
            }
            fast_dec_timer_s16(&hero.quick_swap_tap_timer);
            fast_dec_timer_s16(&hero.quick_swap_tap_window);
            if (controller_state.pressed & 0x10) {
                hero.quick_swap_tap_timer = scale_game_frames(0x14);
            }
            if ((hero.quick_swap_tap_timer != 0 || hero.quick_swap_tap_window != 0) &&
                (controller_state.raw_released & 0x10)) {
                if (hero.quick_swap_tap_window == 0) {
                    hero.quick_swap_tap_window = scale_game_frames(0x14);
                } else {
                    int base = D_0015ED8C;
                    int next = base;
                    int equipped_item_id = hero.items[0].item_id;
                    if (equipped_item_id == 8) {
                        next = D_00141660;
                    }
                    if (next != 0 && equipped_item_id != base) {
                        if (usage_stats.stat[31].count < 0xFFFF) {
                            usage_stats.stat[31].count++;
                        }
                        if (scale_game_frames(D_0015EEA4) / 600 > usage_stats.stat[31].unk2) {
                            usage_stats.stat[31].unk2 = scale_game_frames(D_0015EEA4) / 600;
                        }
                        usage_stats.stat[31].level_mask = usage_stats.stat[31].level_mask |
                                                          (1 << current_level_index) | 0x80000000;
                        hero.pending_item[0] = next;
                        hero.quick_swap_tap_timer = 0;
                        hero.quick_swap_tap_window = 0;
                    }
                }
            }
            if (D_L17_0015F638 > hero.motion.pos.f[2] && hero.ground_distance > 2.0f &&
                hero.vehicle_mode != 2 && hero.state.current != 0x77) {
                hero_set_state(0x77, 1);
            } else {
                switch (hero.state.current) {
                case HERO_STATE_IDLE:
                    if (controller_state.held & 5) {
                        hero_set_state(HERO_STATE_LOOK, 1);
                        break;
                    }
                    clear_hero_queued_animation();
                    if (hero.unk22CA != 0) {
                        if (hero.unk308 == 0 || item_available[0x1C] == 0) {
                            hero_set_state(0x81, 1);
                            break;
                        }
                    }
                    {
                        int            flag;
                        unsigned char *m;
                        int            idle_current_animation_id;

                        flag = 1;
                        if (hero_slot_item_id(0) == 0x1B) {
                            if (((unsigned char *)hero.moby)[0x53] != select_hero_help_value(0)) {
                                set_hero_animation(select_hero_help_value(0), 0, -2.0f);
                            }
                            flag = 0;
                        }
                        if (hero.on_ice_ground != 0) {
                            flag = 0;
                        }
                        if (hero.items[3].state == 2) {
                            m = ((unsigned char *)hero.items[3].moby);
                            if (m != 0 && (m[0x70] & 2) && m[0x53] != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(7));
                            }
                            m = ((unsigned char *)hero.items[3].moby2);
                            if (m != 0 && (m[0x70] & 2) && m[0x53] != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(7));
                            }
                        }
                        if (0.61086524f < D_L17_0017AF80.f258 ||
                            D_L17_0017AF80.f254 > 0.43633232f) {
                            m = ((unsigned char *)hero.items[3].moby);
                            if (m != 0 && m[0x53] != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(0xC));
                            }
                            m = ((unsigned char *)hero.items[3].moby2);
                            if (m != 0 && m[0x53] != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(0xC));
                            }
                        }
                        idle_current_animation_id = return_hero_animation_id_unchanged(moby[0x53]);
                        if (idle_current_animation_id == select_hero_help_value(0) && flag) {
                            if (pick_idle_fidget(&record_index)) {
                                HeroTableEntry70 *rec;

                                hero.fidget_index = record_index;
                                rec = &l17_hero_idle_fidget_entries[record_index];
                                hero.fidget_cooldown = scale_game_frames(0x78);
                                rec->duration_frames = scale_time(rec->duration) * 60.0f;
                                if (rec->use_state_40 != 0) {
                                    hero_set_state(HERO_STATE_FIDGET, 1);
                                } else {
                                    set_hero_animation(rec->unk48, 0, scale_game_frames(0xC));
                                }
                            }
                        } else if (hero.anim_flags & 2) {
                            int keep = 0;
                            if (hero.item_anim_active != 0 && hero.unk20AA != 0) {
                                if (((unsigned char *)hero.moby)[0x53] == hero.item_hold_anim ||
                                    hero.held_item_anim_blend[0] != 0) {
                                    keep = 1;
                                }
                            }
                            if (!keep) {
                                hero.item_anim_active = 0;
                                if (select_hero_help_value(0) == 0x54) {
                                    set_hero_animation(
                                        select_hero_help_value(0), 0, scale_game_frames(0x12));
                                } else {
                                    set_hero_animation(select_hero_help_value(0), 0, -2.0f);
                                }
                            }
                        }
                    }
                    if (scale_game_frames(0xA) < hero.state_timer) {
                        if (((unsigned char *)hero.items[3].moby2) != 0 &&
                            ((unsigned char *)hero.items[3].moby2)[0x53] == 1) {
                            if (fast_dec_timer(&hero.slot3_anim_timer)) {
                                if (0.55850536f > D_L17_0017AF80.f258 &&
                                    D_L17_0017AF80.f254 < 0.34906584f && hero.items[3].state == 2) {
                                    hero.slot3_anim_timer = random_int_between(
                                        scale_game_frames(0x6E), scale_game_frames(0x10E));
                                    blend_hero_slot3_animation_pair();
                                }
                            }
                        }
                    }
                    if (hero.item_anim_active != 0 && hero.anim_changing == 0 &&
                        hero.unk20AA != 0 && hero.unk22C8 == 0) {
                        int id = hero_slot_item_id(0);
                        if (id != hero.item_anim_item_id) {
                            cancel_hero_item_animation();
                        } else {
                            int anim = D_L17_0017A3C0[id].anim;
                            if (((unsigned char *)hero.moby)[0x53] != anim) {
                                if (hero.held_item_anim_blend[0] != 0) {
                                    hero.unkD14 = 1;
                                }
                                hero.item_hold_anim = anim;
                                set_hero_animation(hero.item_hold_anim, 0, scale_game_frames(0xB));
                            }
                        }
                    }
                    stick_target(0, 1.0f);
                    if (hero.on_slip_ground != 0 && hero.frames_off_ground.s == 0) {
                        hero_set_state(HERO_STATE_SLIP, 1);
                        break;
                    }
                    if (hero.air_frames.s > scale_game_frames(4)) {
                        if (hero.ground_distance > 0.4f) {
                            if (hero.state.prev != 8) {
                                hero_set_state(HERO_STATE_FALL, 1);
                                break;
                            }
                            if (scale_game_frames(0xF) > hero.state_timer) {
                                break;
                            }
                            hero_set_state(HERO_STATE_FALL, 1);
                            break;
                        }
                    }
                    if (hero.move_lock_timer != 0) {
                        break;
                    }
                    {
                        int ok = 1;
                        if (hero.state.prev == 1 || hero.state.prev == 0x1E) {
                            if (scale_game_frames(5) > hero.state_timer) {
                                ok = 0;
                            }
                        }
                        if (ok) {
                            if (hero.unk308 != 1) {
                                int t = scale_game_frames(7);
                                if (hero.jump_buffer_lockout != 0) {
                                    t = 1;
                                }
                                if (try_start_hero_jump(t, 0)) {
                                    break;
                                }
                            }
                        }
                    }
                    if (hero.unk308 == 1) {
                        if (pad_pressed_within(0x40, scale_game_frames(7), 0)) {
                            hero_set_state(0x71, 1);
                            break;
                        }
                    }
                    if ((controller_state.held & 0xA) && hero.grounded_frames != 0) {
                        if (((unsigned char *)hero.moby)[0x53] != 0x1A ||
                            !(hero.anim_frame < 108.0f)) {
                            hero_set_state(HERO_STATE_CROUCH, 1);
                            break;
                        }
                    }
                    if (hero.grind_rail != 0) {
                        hero_set_state(HERO_STATE_GRIND, 1);
                        break;
                    }
                    if (hero.on_ice_ground != 0 && hero.ground_slope.f > 0.08726646f) {
                        hero_set_state(0x2F, 1);
                        break;
                    }
                    {
                        int ok = 1;
                        if (hero.state.prev == 1 || hero.state.prev == 0x1E) {
                            if (scale_game_frames(0x18) > hero.state_timer && hero.unk308 == 0) {
                                stick_target(0, 1.0f);
                                if (fast_difference_between_rotations(
                                        hero.motion.move_yaw, D_L17_001676D8) > 1.2217305f) {
                                    ok = 0;
                                }
                            }
                        }
                        if (ok && 0.22f < hero.stick_magnitude) {
                            if (current_level_index == 0xC) {
                                int slot = hero.ground_sound_class;
                                emit_hero_animation_voice_and_ground_particle(slot, 0, 0);
                                emit_hero_animation_voice_and_ground_particle(slot, 1, 0);
                            }
                            hero_set_state(HERO_STATE_RUN, 1);
                        }
                    }
                    break;
                case HERO_STATE_FIDGET:
                    if (controller_state.held & 5) {
                        hero_set_state(HERO_STATE_LOOK, 1);
                        break;
                    }
                    if (hero.air_frames.s != 0) {
                        hero_set_state(HERO_STATE_FIDGET_ALT, 1);
                        break;
                    }
                    if (controller_state.held & 0x40) {
                        hero_set_state(HERO_STATE_FIDGET_ALT, 1);
                        break;
                    }
                    if (controller_state.held & 0xA) {
                        hero_set_state(HERO_STATE_FIDGET_ALT, 1);
                        break;
                    }
                    update_hero_target_speed_from_stick();
                    if (hero.target_speed > 0.0f) {
                        hero_set_state(HERO_STATE_FIDGET_ALT, 1);
                        break;
                    }
                    if (hero.anim_flags & 2) {
                        remove_hero_cached_moby_and_clear_slot_three_moby_flags();
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_FIDGET_ALT:
                    if (scale_game_frames(8) < hero.state_timer) {
                        remove_hero_cached_moby_and_clear_slot_three_moby_flags();
                        if (hero.air_frames.s != 0) {
                            hero_set_state(HERO_STATE_FALL, 1);
                            break;
                        }
                        if (try_start_hero_jump(scale_game_frames(7), 0)) {
                            break;
                        }
                        if (controller_state.held & 0xA) {
                            hero_set_state(HERO_STATE_CROUCH, 1);
                            break;
                        }
                        update_hero_target_speed_from_stick();
                        if (hero.target_speed > 0.0f) {
                            hero_set_state(HERO_STATE_RUN, 1);
                            break;
                        }
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_DYING:
                case HERO_STATE_VEHICLE_MODE_1_DYING:
                case HERO_STATE_VEHICLE_MODE_3_DYING:
                case HERO_STATE_VEHICLE_MODE_2_DYING:
                case HERO_STATE_DROWN:
                case HERO_STATE_DYING_SPEED_LIMITED:
                case HERO_STATE_DYING_ZERO_HEALTH:
                    if (hero.anim_flags & 2) {
                        hero_respawn();
                        return;
                    }
                    break;
                case HERO_STATE_SINK:
                    if (hero.state_timer > scale_game_frames(0xDC)) {
                        hero_respawn();
                        return;
                    }
                    break;
                case HERO_STATE_SWIM:
                case HERO_STATE_SWIM_WITH_CROUCH_INPUT:
                    if (hero.state.current == 0x33) {
                        hero.anim_speed = hero.ground_speed * 15.0f;
                        if (hero.anim_speed < 0.25f) {
                            hero.anim_speed = 0.25f;
                        }
                        if (hero.anim_speed > 0.75f) {
                            hero.anim_speed = 0.75f;
                        }
                    }
                    if (hero.state.current == 0x33) {
                        float lim = frame_time * 2.0f;
                        if (!(controller_state.held & 0xC0) && hero.anim_frame > 9.0f &&
                            hero.anim_frame < 19.0f && hero.state.step != 1 &&
                            hero.stick_magnitude < 0.1f && hero.ground_speed < lim) {
                            hero_set_state(0x34, 1);
                            break;
                        }
                    }
                    if (item_available[4] != 0 && (controller_state.held & 0xA)) {
                        if (hero.state.current == 0x33 &&
                            scale_game_frames(0x14) < hero.state_timer) {
                            hero_set_state(0x35, 1);
                            break;
                        }
                    } else if (hero.state.current == 0x35 && hero.stick_magnitude > 0.5f) {
                        hero_set_state(0x33, 1);
                        break;
                    }
                    if (hero.state.current == 0x35 && !(controller_state.held & 0xC0) &&
                        hero.state.step != 1 && hero.stick_magnitude < 0.7f &&
                        !(controller_state.held & 0xA)) {
                        hero_set_state(0x34, 1);
                    }
                    break;
                case HERO_STATE_SWIM_IDLE:
                    if (hero.anim_flags & 2) {
                        if (((unsigned char *)hero.moby)[0x53] != 0x3C) {
                            set_hero_animation(0x3C, 0, scale_game_frames(0xB));
                        }
                    }
                    if (0.2f < hero.stick_magnitude || (controller_state.held & 0xC0) ||
                        (item_available[4] != 0 && (controller_state.held & 0xA))) {
                        if (item_available[4] != 0 && (controller_state.held & 0xA)) {
                            hero_set_state(0x35, 1);
                            break;
                        }
                        hero_set_state(0x33, 1);
                    }
                    break;
                case HERO_STATE_WATER_SURFACE_ENTRY:
                    if (hero.jump_lockout_timer == 0) {
                        if (pad_pressed_within(0x40, scale_game_frames(8), 0)) {
                            if (scale_game_frames(0xA) < hero.state_timer) {
                                hero_set_state(0x12, 1);
                                break;
                            }
                        }
                    }
                    hero.item_select_blocked = 1;
                    if (hero.unk1DE == 0) {
                        if (pad_pressed_within(0x80, scale_game_frames(7), 0) ||
                            (item_available[4] != 0 &&
                             pad_pressed_within(0xA, scale_game_frames(7), 0))) {
                            if (item_available[4] != 0 && (controller_state.held & 0xA)) {
                                hero_set_state(0x35, 1);
                                break;
                            }
                            hero_set_state(0x33, 1);
                            break;
                        }
                    }
                    if (hero.wading != 0 && hero.water_depth < 0.4f) {
                        if (hero_set_state(HERO_STATE_IDLE, 0)) {
                            set_hero_animation(
                                select_hero_help_value(0), 0, scale_game_frames(0x12));
                        }
                    }
                    if (0.2f < hero.stick_magnitude) {
                        if (scale_game_frames(0xA) < hero.state_timer) {
                            hero_set_state(0x36, 1);
                        }
                    }
                    break;
                case HERO_STATE_WATER_SURFACE_MOVE:
                    if (hero.jump_lockout_timer == 0) {
                        if (pad_pressed_within(0x40, scale_game_frames(8), 0)) {
                            hero_set_state(0x12, 1);
                            break;
                        }
                    }
                    if (hero.unk1DE == 0) {
                        if (pad_pressed_within(0x80, scale_game_frames(7), 0)) {
                            if (item_available[4] != 0 && (controller_state.held & 0xA)) {
                                hero_set_state(0x35, 1);
                                break;
                            }
                            hero_set_state(0x33, 1);
                            break;
                        }
                    }
                    if (hero.wading != 0 && hero.water_depth < 0.7f) {
                        hero_set_state(0x73, 0);
                        set_hero_animation(0x60, 0, scale_game_frames(0x11));
                        hero.motion.velocity.f[2] = 0.0f;
                        hero.motion.displacement.f[2] = 0.0f;
                    }
                    if (0.1f > hero.stick_magnitude && hero.ground_speed == 0.0f &&
                        hero.unk698 == 0) {
                        hero_set_state(0x37, 1);
                    }
                    break;
                case HERO_STATE_HURT_WITH_DISPLACEMENT_VELOCITY:
                    if (hero.health.hp <= 0) {
                        if (current_level_index == 0xF || current_level_index == 0x11) {
                            hero_set_state(0x82, 1);
                            break;
                        }
                        hero_set_state(0x6A, 1);
                        set_hero_animation(0x5F, 0x28, scale_game_frames(0x12));
                        break;
                    }
                    if (hero.anim_flags & 2) {
                        hero_set_state(0x34, 1);
                        break;
                    }
                    if (scale_game_frames(0x32) < hero.state_timer) {
                        hero_set_state(0x34, 0);
                    }
                    break;
                case HERO_STATE_WATER_SURFACE_HURT:
                    if (hero.health.hp <= 0) {
                        hero_set_state(0x6A, 1);
                        set_hero_animation(0x5F, 0x25, scale_game_frames(0x12));
                        break;
                    }
                    if (scale_game_frames(0x32) < hero.state_timer) {
                        hero_set_state(0x37, 0);
                    }
                    break;
                case 56:
                case 57:
                    if (hero.items[0].item_id != 0x16) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_ANIMATION_WAIT_TO_IDLE:
                    if (hero.anim_flags & 2) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_LOOK:
                    if (hero.vehicle_mode == 0) {
                        if (hero.unk20AA == 0 || hero.unk20A5 != 0) {
                            if (controller_state.held & 0xA) {
                                if (((unsigned char *)hero.moby)[0x53] != 0xD) {
                                    set_hero_animation(0xD, 0, scale_game_frames(8));
                                }
                            } else {
                                int a = select_hero_help_value(1);
                                if (((unsigned char *)hero.moby)[0x53] != a) {
                                    set_hero_animation(a, 0, scale_game_frames(8));
                                }
                            }
                        }
                    }
                    {
                        unsigned char *m = ((unsigned char *)hero.items[0].moby);
                        int            ok = 1;
                        if (m != 0 && is_wrench_moby(m) && m[0x20] != 0) {
                            ok = 0;
                        }
                        if (controller_state.held & 5) {
                            break;
                        }
                        if (!ok) {
                            break;
                        }
                    }
                    if (hero.vehicle_mode != 0) {
                        enter_hero_idle_state_for_current_vehicle_mode();
                        break;
                    }
                    if (controller_state.held & 0xA) {
                        hero_set_state(HERO_STATE_CROUCH, 1);
                        break;
                    }
                    hero_set_state(HERO_STATE_IDLE, 0);
                    {
                        int a = select_hero_help_value(1);
                        if (((unsigned char *)hero.moby)[0x53] != a) {
                            set_hero_animation(a, 0, scale_game_frames(8));
                        }
                    }
                    break;
                case HERO_STATE_ITEM_BUTTON_RELEASE_WAIT:
                    if (controller_state.held & hero.items[0].button_mask) {
                        break;
                    }
                    if (hero.unk1D0 != 0) {
                        break;
                    }
                    hero_set_state(HERO_STATE_IDLE, 1);
                    break;
                case HERO_STATE_WAIT_FOR_ITEM_RELEASE_THEN_IDLE:
                    if (controller_state.held & hero.items[0].button_mask) {
                        break;
                    }
                    if (hero.unk1D0 != 0) {
                        break;
                    }
                    if (((unsigned char *)hero.moby)[0x53] == 0x36) {
                        hero_set_state(HERO_STATE_IDLE, 0);
                        break;
                    }
                    hero_set_state(HERO_STATE_IDLE, 1);
                    break;
                case HERO_STATE_ITEM_BUTTON_HOLD_ANIMATION_WAIT: {
                    int mask = hero.items[0].button_mask;
                    if (((unsigned char *)hero.moby)[0x52] == 0x2E && hero.anim_frame > 28.0f) {
                        if (controller_state.held & mask) {
                            hero_set_state(0x17, 1);
                            break;
                        }
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                } break;
                case HERO_STATE_WRENCH_COMBO:
                case HERO_STATE_WRENCH_SLAM:
                case HERO_STATE_CROUCH_SWING:
                    if (hero.state.current == HERO_STATE_WRENCH_SLAM) {
                        int slam_trajectory_intercept_frames =
                            truncate_float_to_s32(predict_hero_trajectory_intercept_time(
                                60.0f, frame_time_sq * 130.0f, -1.0f));
                        int n = 0x1A;
                        if (hero.slam_descending == 0.0f) {
                            hero.anim_speed = 0.33f;
                        } else if (hero.slam_descending != 0.0f && hero.air_frames.s != 0 &&
                                   slam_trajectory_intercept_frames != 0 &&
                                   (float)n - hero.anim_frame > 1.0f) {
                            adjust_hero_animation_speed_for_target_frame(
                                n, slam_trajectory_intercept_frames, 0.5f, -1.0f);
                            if (hero.ground_distance < 0.7f && hero.anim_speed < 0.75f) {
                                hero.anim_speed = 0.75f;
                            }
                        } else if (hero.air_frames.s == 0 && hero.anim_frame < (float)n) {
                            hero.anim_speed = 2.0f;
                        } else {
                            hero.anim_speed = 1.0f;
                        }
                    }
                    {
                        HeroTableRow2C *rec = &D_L17_0017C528[hero.attack_record];
                        int             t = (truncate_float_to_s32(hero.anim_frame) - rec->fC) * 2;
                        if ((float)rec->f14 < hero.anim_frame && hero.anim_changing == 0 &&
                            !(hero.state.current == HERO_STATE_CROUCH_SWING && hero.unk308 != 0)) {
                            if (pad_pressed_within(0x40, t, 0)) {
                                if (controller_state.held & 0xA) {
                                    if (hero_handle_jump_press()) {
                                        break;
                                    }
                                } else if (hero.jump_lock_timer == 0) {
                                    hero_set_state(HERO_STATE_JUMP, 1);
                                    break;
                                }
                            }
                        }
                        if (hero.anim_changing == 0 && (float)rec->f18 < hero.anim_frame) {
                            hero_set_state(HERO_STATE_IDLE, 0);
                            break;
                        }
                    }
                    if (hero.state.current == HERO_STATE_CROUCH_SWING) {
                        if (hero.items[0].item_id != 8) {
                            hero_set_state(HERO_STATE_IDLE, 1);
                            break;
                        }
                        if (scale_game_frames(0x64) < hero.state_timer &&
                            hero.queued_anim_frame < 0x1A) {
                            hero_set_attack_ticks(2);
                            hero.queued_anim_frame = 0x1A;
                        }
                    }
                    break;
                case HERO_STATE_WAIT_FOR_ANIMATION_THEN_IDLE:
                case HERO_STATE_WALL_BONK:
                    if (hero.anim_flags & 2) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_RUN:
                case HERO_STATE_RUN_CLEAR_VELOCITY_Z:
                case HERO_STATE_SHALLOW_WATER_RUN:
                case 126:
                    if (hero.run_start_blending != 0 &&
                        (MOBY(moby)->anim_time > 0.9f || moby[0x52] != 0xFF)) {
                        hero.run_start_blending = 0;
                    }
                    if (hero.on_slip_ground != 0 && hero.frames_off_ground.s == 0) {
                        hero_set_state(HERO_STATE_SLIP, 1);
                        break;
                    }
                    if (!(hero.air_frames.s < scale_game_frames(5)) &&
                        !(hero.state.prev_control_mode == 0x12 && hero.wading != 0) &&
                        hero.ground_distance > 1.35f) {
                        if (check_hero_forward_collision_height() == 0) {
                            hero_set_state(HERO_STATE_FALL, 1);
                            break;
                        }
                    }
                    {
                        int t = scale_game_frames(9);
                        if (hero.jump_buffer_lockout != 0) {
                            t = 1;
                        }
                        if (pad_pressed_within(0x40, t, 0)) {
                            if (!(hero.air_frames.s > scale_game_frames(4)) ||
                                hero.still_frames != 0) {
                                stick_target(0, 1.0f);
                                if (try_start_hero_jump(t, 1)) {
                                    break;
                                }
                                if (hero.state.step <= 0) {
                                    hero_set_state(HERO_STATE_JUMP, 1);
                                    break;
                                }
                                if (hero.target_speed > D_L17_0017C6B8.fC &&
                                    fast_difference_between_rotations(
                                        hero.motion.move_yaw, hero.motion.rot.f[2]) < 1.0471976f &&
                                    D_L17_0017C6B8.f18 * frame_time * 0.85f <
                                        hero.motion.flat_speed) {
                                    hero_set_state(HERO_STATE_RUN_JUMP, 1);
                                    break;
                                }
                                hero_set_state(HERO_STATE_JUMP, 1);
                                break;
                            }
                        }
                    }
                    if (hero.air_frames.s != 0) {
                        break;
                    }
                    if (hero.on_ice_ground == 0 || (frame_time * 2.0f > hero.motion.flat_speed &&
                                                    hero.ground_slope.f < 0.08726646f)) {
                        if (controller_state.held & 5) {
                            hero_set_state(HERO_STATE_LOOK, 1);
                            break;
                        }
                    }
                    if (controller_state.held & 0xA) {
                        hero_set_state(HERO_STATE_CROUCH, 1);
                        break;
                    }
                    if (hero.on_ice_ground != 0 && hero.state.current == 0x2F) {
                        float len = vector_length_xy(hero.motion.displacement.f);
                        if (hero.stick_magnitude < 0.17f &&
                            (((unsigned char *)hero.moby)[0x53] == 0x37 ||
                             (((unsigned char *)hero.moby)[0x53] == select_hero_help_value(0) &&
                              hero.ground_slope.f > 0.08726646f)) &&
                            scale_game_frames(8) < hero.stick_idle_frames &&
                            hero.anim_changing == 0) {
                            set_hero_animation(0x6D, 0, scale_game_frames(0xE));
                        } else if (0.3f < hero.stick_magnitude &&
                                   ((unsigned char *)hero.moby)[0x53] != 0x37 &&
                                   hero.anim_changing == 0) {
                            set_hero_animation(0x37, 0, scale_game_frames(0xE));
                        } else if (((unsigned char *)hero.moby)[0x53] == 0x6D &&
                                   hero.anim_changing == 0 && hero.stick_magnitude < 0.17f &&
                                   hero.ground_slope.f < 0.08726646f && len < frame_time * 1.7f) {
                            set_hero_animation(
                                select_hero_help_value(0), 0, scale_game_frames(0x14));
                        }
                    }
                    if (hero.state.current == HERO_STATE_RUN || hero.state.current == 0x7E) {
                        if (hero.on_ice_ground != 0) {
                            hero_set_state(0x2F, 1);
                        }
                    } else if (hero.state.current == 0x2F) {
                        if (hero.on_ice_ground == 0) {
                            hero_set_state(HERO_STATE_RUN, 1);
                        }
                    }
                    if (hero.state.current == HERO_STATE_RUN) {
                        if (hero.wading != 0) {
                            hero_set_state(0x73, 1);
                        }
                    } else if (hero.state.current == 0x73) {
                        if (hero.wading == 0) {
                            hero_set_state(HERO_STATE_RUN, 1);
                        }
                    }
                    if (hero.state.current == HERO_STATE_RUN && hero.unk308 != 0) {
                        hero_set_state(0x3F, 1);
                    }
                    if (hero.grind_rail != 0) {
                        hero_set_state(HERO_STATE_GRIND, 1);
                        break;
                    }
                    if (hero.state_timer > scale_game_frames(0xF) ||
                        (hero.state.prev_control_mode != 0 && hero.state.prev_control_mode != 1)) {
                        if ((controller_state.pressed & 0x10000) && hero.unk20AA == 0) {
                            hero.unk3BE = 1;
                        }
                    }
                    if (hero.state.current == HERO_STATE_RUN) {
                        if (hero.stick_magnitude < 0.17f) {
                            if (frame_time * 2.7f < hero.motion.speed_xy && hero.unk20AA == 0) {
                                if (hero.motion.speed_xy < frame_time * 4.4f ||
                                    read_pad_float_history(4) < 0.8f) {
                                    if (hero_set_state(HERO_STATE_STOP, 0) == 0) {
                                        break;
                                    }
                                    if (select_hero_help_value(0) == 0x54) {
                                        set_hero_animation(
                                            select_hero_help_value(0), 0, scale_game_frames(0x12));
                                    } else {
                                        set_hero_animation(0x14, 7, scale_game_frames(9));
                                    }
                                    break;
                                }
                                hero_set_state(HERO_STATE_STOP, 1);
                                break;
                            }
                            if (!(hero.state_timer > scale_game_frames(0x19))) {
                                break;
                            }
                            if (try_start_hero_item_idle_animation()) {
                                break;
                            }
                            if (hero_set_state(HERO_STATE_IDLE, 0) == 0) {
                                break;
                            }
                            if (select_hero_help_value(0) == 0x54) {
                                set_hero_animation(
                                    select_hero_help_value(0), 0, scale_game_frames(0x12));
                            } else {
                                set_hero_animation(select_hero_help_value(0), 0, -1.0f);
                            }
                            break;
                        }
                        update_hero_animation_for_movement_speed();
                        break;
                    } else if (hero.state.current == 0x2F) {
                        if (controller_state.no_direction != 0) {
                            if (vector_length_xy(hero.motion.velocity.f) < frame_time * 0.05f &&
                                hero.ground_slope.f < 0.08726646f) {
                                if (hero_set_state(HERO_STATE_IDLE, 0)) {
                                    set_hero_animation(
                                        select_hero_help_value(0), 0, scale_game_frames(0x11));
                                }
                            }
                        }
                        if (moby[0x53] == 0x37) {
                            hero.anim_speed = (1.0f - hero.unk3B0) * 2.0f;
                            if (hero.anim_speed > 1.7f) {
                                hero.anim_speed = 1.7f;
                            }
                            if (hero.anim_speed < 0.75f) {
                                hero.anim_speed = 0.75f;
                            }
                        } else if (moby[0x53] == 0x6D) {
                            hero.anim_speed = hero.motion.speed_xy * 10.0f;
                            if (hero.anim_speed > 1.1f) {
                                hero.anim_speed = 1.1f;
                            }
                            if (hero.anim_speed < 0.3f) {
                                hero.anim_speed = 0.3f;
                            }
                        }
                    } else if (hero.state.current == 0x73) {
                        if (controller_state.no_direction != 0) {
                            if (vector_length_xy(hero.motion.velocity.f) < frame_time * 3.2f) {
                                if (hero_set_state(HERO_STATE_IDLE, 0)) {
                                    set_hero_animation(
                                        select_hero_help_value(0), 0, scale_game_frames(0x11));
                                }
                            }
                        }
                        hero.anim_speed = hero.motion.speed_xy * 35.0f;
                        if (hero.anim_speed < 0.6f) {
                            hero.anim_speed = 0.6f;
                        }
                        if (hero.anim_speed > 1.05f) {
                            hero.anim_speed = 1.05f;
                        }
                    }
                    break;
                case HERO_STATE_APPROACH_TARGET_POSE:
                    if (vector_distance_xy(hero.motion.pos.f, hero.target_pose.f) < 0.2f) {
                        if (frame_time * 2.7f > hero.motion.speed_xy) {
                            if (fast_difference_between_rotations(
                                    hero.motion.rot.f[2], hero.target_pose.f[3]) < 0.034906585f) {
                                hero_set_state(0x67, 1);
                                break;
                            }
                        }
                        if (hero.vehicle_mode != 0 && frame_time * 2.7f <= hero.motion.speed_xy) {
                            hero_set_state(0x66, 1);
                        }
                        break;
                    }
                    update_hero_animation_for_movement_speed();
                    break;
                case HERO_STATE_BRAKE_AT_TARGET_POSITION:
                    if (hero.anim_flags & 2) {
                        hero_set_state(0x65, 1);
                    }
                    break;
                case HERO_STATE_AT_TARGET_POSE:
                    if ((hero.anim_flags & 2) || hero.unk1650 != 0) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    if (0.2f < vector_distance_xy(hero.motion.pos.f, hero.target_pose.f)) {
                        hero_set_state(0x65, 1);
                    }
                    break;
                case HERO_STATE_WAIT_FOR_JUMP_OR_RESPAWN:
                    if (hero.sink_jump_count < 2) {
                        if (scale_game_frames(0xF) +
                                hero.sink_jump_count * scale_game_frames(0x19) <
                            hero.state_timer) {
                            if (pad_pressed_within(
                                    0x40, scale_game_frames(7), &respawn_jump_press_age_frames)) {
                                hero_set_state(0x69, 1);
                            }
                        }
                    }
                    if (hero.sink_surface_z - 1.2f > hero.motion.pos.f[2] ||
                        hero.state_timer > scale_game_frames(0xAA)) {
                        hero_respawn();
                        return;
                    }
                    break;
                case HERO_STATE_HEIGHT_OR_TIMEOUT_RESPAWN_WAIT:
                    if (hero.sink_surface_z - 1.2f > hero.motion.pos.f[2] ||
                        hero.state_timer > scale_game_frames(0x96)) {
                        hero_respawn();
                        return;
                    }
                    break;
                case HERO_STATE_DEATH_DESCENT:
                    if (hero.sink_surface_z - 1.2f > hero.motion.pos.f[2] ||
                        hero.state_timer > scale_game_frames(0x64)) {
                        hero_respawn();
                        return;
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_1_DEATH_DESCENT:
                    if (hero.sink_surface_z - 0.8f > hero.motion.pos.f[2] ||
                        hero.state_timer > scale_game_frames(0x96)) {
                        hero_respawn();
                        return;
                    }
                    break;
                case HERO_STATE_DIRECTION_LIMITED_GROUND_MOVE:
                    hero.anim_speed = hero.ground_speed * 27.0f;
                    if (hero.anim_speed < 0.5f) {
                        hero.anim_speed = 0.5f;
                    }
                    if (controller_state.held & 0xA) {
                        hero_set_state(HERO_STATE_CROUCH, 1);
                        break;
                    }
                    if (pad_pressed_within(0x40, scale_game_frames(7), 0)) {
                        hero_set_state(0x71, 1);
                        break;
                    }
                    if (!(hero.air_frames.s < scale_game_frames(5)) &&
                        hero.ground_distance > 0.8f) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        break;
                    }
                    hero.unk1F8 = scale_game_frames(0x28);
                    if (absolute_float(hero.motion.rot.f[1]) > 0.7853982f ||
                        absolute_float(hero.motion.rot.f[0]) > 0.7853982f) {
                        hero.ledge_probe_cooldown = scale_game_frames(0x28);
                    }
                    if ((hero.grounded_frames != 0 && hero.on_magnet_ground == 0) ||
                        (hero.air_frames.s > scale_game_frames(7) && hero.ground_distance > 1.0f)) {
                        hero_set_state(HERO_STATE_RUN, 1);
                        break;
                    }
                    if (((unsigned char *)hero.moby)[0x53] != 0x5B) {
                        set_hero_animation(0x5B, 0, scale_game_frames(8));
                    }
                    if (controller_state.no_direction != 0 &&
                        !(hero.state_timer < scale_game_frames(0x1E))) {
                        if (hero_set_state(HERO_STATE_IDLE, 0)) {
                            clear_u64_value(hero.motion.momentum.f);
                            set_hero_animation(
                                select_hero_help_value(0), 0, scale_game_frames(0xF));
                        }
                    }
                    break;
                case HERO_STATE_ATTACK_RECORD_FRAME_EXIT_WAIT:
                    if (hero.anim_frame > (float)D_L17_0017C528[hero.attack_record].f18) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_ANIMATION_FRAME_EXIT_WAIT:
                    if (25.0f < hero.anim_frame) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_VOICE_SLOT_CLEANUP_IDLE_EXIT:
                    if (hero.selector_4 != 0) {
                        break;
                    }
                    if (hero.unk1E0 != 0) {
                        break;
                    }
                    if (hero.voice_slot2 != -1) {
                        Row13E650 *voice_slot_record =
                            (Row13E650 *)((u8 *)&voice_pool + hero.voice_slot2 * 0x70);
                        if (voice_slot_record->f88 == ((unsigned char *)hero.moby) &&
                            voice_slot_record->f74 != 0) {
                            release_voice_slot(hero.voice_slot2);
                        }
                    }
                    hero.voice_slot2 = -1;
                    if (hero_set_state(HERO_STATE_IDLE, 0) == 0) {
                        break;
                    }
                    set_hero_animation(select_hero_help_value(0), 0, scale_game_frames(0x11));
                    qcopy(hero.motion.momentum.f, hero.motion.platform_carry.f);
                    break;
                case HERO_STATE_STOP:
                    if (controller_state.held & 5) {
                        hero_set_state(HERO_STATE_LOOK, 1);
                        break;
                    }
                    if (hero.move_lock_timer == 0) {
                        if (controller_state.pressed & 0x40) {
                            hero_set_state(HERO_STATE_JUMP, 1);
                            break;
                        }
                        if (controller_state.held & 0xA) {
                            hero_set_state(HERO_STATE_CROUCH, 1);
                            break;
                        }
                    }
                    if (hero.air_frames.s > scale_game_frames(8) && hero.ground_distance > 0.3f) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        break;
                    }
                    if (hero.anim_flags & 2) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                        break;
                    }
                    if (hero.grind_rail != 0) {
                        hero_set_state(HERO_STATE_GRIND, 1);
                        break;
                    }
                    if (!(hero.stick_magnitude > 0.2f)) {
                        break;
                    }
                    if (select_hero_help_value(0) != 0x54) {
                        if (hero.ground_speed == 0.0f) {
                            hero_set_state(HERO_STATE_IDLE, 0);
                            break;
                        }
                    } else if (hero.anim_frame > 10.0f) {
                        if (hero_set_state(HERO_STATE_IDLE, 0)) {
                            set_hero_animation(
                                select_hero_help_value(0), 0, scale_game_frames(0x11));
                        }
                    }
                    if (0.5f < hero.stick_magnitude) {
                        hero_set_state(HERO_STATE_RUN, 1);
                    }
                    break;
                case HERO_STATE_CROUCH:
                    if (controller_state.held & 5) {
                        hero_set_state(HERO_STATE_LOOK, 1);
                        break;
                    }
                    if (hero.state_timer > scale_game_frames(0xB) &&
                        !(controller_state.held & 0xA)) {
                        hero_set_state(HERO_STATE_IDLE, 0);
                        set_hero_animation(select_hero_help_value(0), 0, -2.0f);
                        break;
                    }
                    if (hero.on_slip_ground != 0 && hero.frames_off_ground.s == 0) {
                        hero_set_state(HERO_STATE_SLIP, 1);
                        break;
                    }
                    if (hero.unk308 == 1) {
                        if (pad_pressed_within(0x40, scale_game_frames(7), 0)) {
                            hero_set_state(0x71, 1);
                            break;
                        }
                    } else if (hero_handle_jump_press()) {
                        break;
                    }
                    if (pad_pressed_within(0x80, scale_game_frames(0xF), 0)) {
                        if (hero.items[0].item_id == 8) {
                            hero_set_state(HERO_STATE_CROUCH_SWING, 1);
                        }
                    }
                    if (hero.air_frames.s > scale_game_frames(8)) {
                        if (hero.state.prev != 8) {
                            hero_set_state(HERO_STATE_FALL, 1);
                            break;
                        }
                        if (scale_game_frames(0xF) > hero.state_timer) {
                            break;
                        }
                        hero_set_state(HERO_STATE_FALL, 1);
                    }
                    break;
                case HERO_STATE_GRAPPLE_TARGET_ENTRY:
                    if (hero.items[0].item_id != 0xC) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_APPROACH_TARGET_MOBY:
                    adjust_hero_animation_speed_for_target_frame(
                        10.0f, hero.target_arrival_frames, 0.5f, -1.0f);
                    if ((float)scale_game_frames(0x16) > hero.target_arrival_frames &&
                        (float)scale_game_frames(0x1E) > hero.target_arrival_frames) {
                        set_hero_animation(0xA, 0, scale_game_frames(0x10));
                    }
                    {
                        int go = 0;
                        if (!(controller_state.held & hero.items[0].button_mask)) {
                            if ((float)scale_game_frames(0x19) < hero.target_arrival_frames) {
                                hero.unk988 = 1;
                            }
                            go = 1;
                        }
                        if (hero.state.step == 1 && frame_time * 4.0f > hero.grapple_pull_speed) {
                            go = 1;
                        }
                        if (!go) {
                            break;
                        }
                    }
                    hero.ground_speed = hero.grapple_pull_speed;
                    if (frame_time * 7.0f < hero.ground_speed) {
                        hero.ground_speed = frame_time * 7.0f;
                    }
                    if (hero.ground_distance < 0.3f) {
                        hero_set_state(HERO_STATE_IDLE, 0);
                        set_hero_animation(select_hero_help_value(0), 0, scale_game_frames(0x1E));
                        break;
                    }
                    if (((unsigned char *)hero.moby)[0x53] == 0xA) {
                        hero_set_state(HERO_STATE_FALL, 0);
                    } else {
                        hero_set_state(HERO_STATE_FALL, 0);
                        set_hero_animation(0xA, 0, scale_game_frames(0x11));
                    }
                    hero.unk2284 = 0;
                    hero.stickless_decel = frame_time_sq * 42.0f;
                    hero.fall_state_gravity = frame_time_sq * 29.0f;
                    break;
                case HERO_STATE_CONSTRAIN_TO_TARGET_RADIUS:
                    if (controller_state.held & hero.items[0].button_mask) {
                        break;
                    }
                    hero.ground_speed = hero.grapple_pull_speed;
                    if (frame_time * 7.0f < hero.ground_speed) {
                        hero.ground_speed = frame_time * 7.0f;
                    }
                    if (hero.ground_distance < 0.4f) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                        break;
                    }
                    hero_set_state(HERO_STATE_FALL, 1);
                    hero.unk2284 = 0;
                    break;
                case HERO_STATE_GRAPPLE_SWING_ENTRY:
                    if (controller_state.held & hero.items[0].button_mask) {
                        break;
                    }
                    if (scale_game_frames(0xA) < hero.state_timer) {
                        if (scale_game_frames(0x28) > hero.state_timer) {
                            hero.unk988 = 1;
                        }
                        hero_set_state(0x2D, 1);
                        hero.stickless_decel = frame_time_sq * 7.0f;
                        hero.fall_state_gravity = frame_time_sq * 27.0f;
                    }
                    break;
                case HERO_STATE_RISE_THEN_FALL_WITH_AIRBORNE_HIT_QUERY:
                    if (hero.state_timer < scale_game_frames(0xC) &&
                        (hero.state.prev2 != 0x10 ||
                         (hero.state.prev != 8 && hero.state.prev != 6)) &&
                        hero.state.prev != 0xF && hero.state.prev != 0xA &&
                        hero.state.prev != 0x10 && hero.state.prev != 0x12 &&
                        hero.state.prev != 0x3C && (controller_state.pressed & 0x40)) {
                        hero_set_state(HERO_STATE_PACK_LONG_JUMP, 1);
                    }
                    {
                        int rise_fall_trajectory_intercept_frames =
                            truncate_float_to_s32(predict_hero_trajectory_intercept_time(
                                60.0f, hero.rise_fall_gravity, -1.0f));
                        hero.predicted_land_frames = rise_fall_trajectory_intercept_frames;
                        if (hero.past_apex != 0.0f && hero.frames_off_ground.s != 0 &&
                            rise_fall_trajectory_intercept_frames != 0) {
                            if (rise_fall_trajectory_intercept_frames == 2) {
                                scale_game_frames(0xA);
                            }
                            adjust_hero_animation_speed_for_target_frame(
                                hero.rise_fall_target_frame,
                                hero.predicted_land_frames,
                                0.5f,
                                -1.0f);
                        } else {
                            hero.anim_speed = 1.0f;
                        }
                    }
                    if (hero.anim_flags & 2) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_GRIND:
                case HERO_STATE_GRIND_JUMP:
                case HERO_STATE_GRIND_RAIL_TRANSFER:
                case HERO_STATE_GRIND_ATTACK:
                case HERO_STATE_GRIND_COLLISION_RECOVERY:
                    if (hero.state.current == 0x42) {
                        hero.unk5BE = 0;
                    } else {
                        int   grab;
                        float len;

                        grab = 0;
                        if (current_level_index == 0x10 && hero.wall_hit != 0) {
                            unsigned char *m = ((unsigned char *)hero.coll_hit_moby);
                            if (m != 0 && *(short *)(m + 0xA6) == 0x101) {
                                grab = 1;
                                m[0xBC] = 1;
                            }
                        }
                        len = vector_length_xy(hero.motion.displacement.f);
                        if (len < vector_length_xy(hero.motion.velocity.f) * 0.5f || grab ||
                            hero.unk5BE != 0) {
                            int ok = 1;

                            if (current_level_index != 0x10) {
                                qcopy(query_position, hero.rail_point.f);
                                query_position[2] += 2.0f;
                                spatial_query_vector[0] = fast_cos(hero.rail_dir_yaw) * 5.0f;
                                spatial_query_vector[1] = fast_sin(hero.rail_dir_yaw) * 5.0f;
                                spatial_query_vector[2] = 0.0f;
                                fast_vec_add(
                                    spatial_query_vector, spatial_query_vector, query_position);
                                if (collision_line(query_position, spatial_query_vector, 2, 0, 0)) {
                                    ok = 0;
                                }
                            }
                            hero.unk5BE = 0;
                            if (hero.health.hp >= 2 && ok) {
                                hero_set_state(0x42, 1);
                                if (grab) {
                                    if (hero.ground_distance > 1.7f) {
                                        hero.motion.velocity.f[2] = frame_time * 8.0f;
                                    } else if (hero.ground_distance > 1.0f) {
                                        hero.motion.velocity.f[2] = frame_time * 10.0f;
                                    } else {
                                        hero.motion.velocity.f[2] = frame_time * 13.0f;
                                    }
                                }
                                break;
                            }
                            {
                                float ang;

                                ang = fast_add_rotations(fast_arc_tan(hero.motion.velocity.f[0],
                                                                      hero.motion.velocity.f[1]),
                                                         3.1415927f);
                                ang = fast_add_rotations(ang, randf_sym(0.5235988f, 0.61086524f));
                                hero.motion.resolved_velocity.f[0] =
                                    fast_cos(ang) * hero.unk574 * 0.7f;
                                hero.motion.resolved_velocity.f[1] =
                                    fast_sin(ang) * hero.unk574 * 0.7f;
                                hero.motion.resolved_velocity.f[2] = frame_time * 4.5f;
                            }
                            if (scale_game_frames(0xF) < hero.rail_off_frames &&
                                hero.health.hp < 2) {
                                hero.health.hp = 2;
                            }
                            hero_set_state(HERO_STATE_HURT, 1);
                            hero.unk1C8 = scale_game_frames(0x46);
                            break;
                        }
                    }
                    if (hero.state.current == HERO_STATE_GRIND) {
                        int go = 1;

                        if (current_level_index == 8) {
                            *(OvlQuad *)query_position = 0;
                            query_position[0] = 342.0f;
                            query_position[1] = 111.1f;
                            query_position[2] = 42.0f;
                            if (fast_vec_dist(hero.motion.pos.f, query_position) < 7.0f) {
                                go = 0;
                                hero.move_lock_timer = scale_game_frames(0x64);
                            }
                        }
                        if (go) {
                            if (pad_pressed_within(0x40, scale_game_frames(7), 0)) {
                                if (hero.unk5B8 != 0) {
                                    hero.rail_jump_queued = 1;
                                    break;
                                }
                                hero_set_state(0x29, 1);
                                break;
                            }
                        }
                        if (scale_game_frames(5) < hero.rail_off_frames) {
                            if (hero.ground_distance > 8.0f) {
                                hero.move_lock_timer = scale_game_frames(0x46);
                            }
                            hero_set_state(HERO_STATE_FALL, 1);
                            hero.unk1C8 = scale_game_frames(8);
                            break;
                        }
                        if (pad_pressed_within(0x80, scale_game_frames(7), 0)) {
                            hero_set_state(0x2B, 1);
                            break;
                        }
                        if (hero.grind_pose == -1) {
                            int a = hero.grind_anim_variant + 0x31;
                            if (((unsigned char *)hero.moby)[0x53] != a) {
                                set_hero_animation(a, 0, scale_game_frames(0xC));
                            }
                        } else {
                            int a = hero.grind_pose + 0x4A;
                            if (((unsigned char *)hero.moby)[0x53] != a) {
                                set_hero_animation(a, 0, scale_game_frames(0xC));
                            }
                        }
                        if (hero.unk5BC != 0) {
                            if (hero.rail_jump_queued != 0) {
                                float t;

                                hero_set_state(0x29, 1);
                                hero.rail_gap_jump = 1;
                                t = FUN_L00_0025abf0(hero.motion.pos.f,
                                                     hero.jump_target_pos.f,
                                                     0,
                                                     hero.motion.flat_speed,
                                                     -hero.jump_gravity);
                                *(OvlQuad *)spatial_query_vector = 0;
                                *(OvlQuad *)query_position = 0;
                                spatial_query_vector[2] = t;
                                t = calculate_triangular_corrected_scalar(
                                    query_position, spatial_query_vector, 0, hero.jump_gravity);
                                hero.jump_height_start = t;
                                hero.jump_gravity = hero.rail_jump_gravity;
                                hero.unk430 = t;
                                hero.unk48C = t + 0.05f;
                            }
                        }
                        hero.unk5B8 = 0;
                        hero.unk5BC = 0;
                    } else if (hero.state.current == 0x2B) {
                        int n;

                        if (pad_pressed_within(0x40, scale_game_frames(5), 0) &&
                            hero.anim_frame >= (float)D_L17_0017C528[hero.attack_record].f14) {
                            hero_set_state(0x29, 1);
                            break;
                        }
                        n = 0xE;
                        if (hero.attack_record == 0) {
                            n = 0x17;
                        }
                        if (pad_pressed_within(0x80, scale_game_frames(7), 0)) {
                            if ((float)n < hero.anim_frame) {
                                hero_set_state(0x2B, 1);
                            }
                            break;
                        }
                        if (hero.anim_frame > (float)D_L17_0017C528[hero.attack_record].f18) {
                            hero_set_state(HERO_STATE_GRIND, 1);
                            break;
                        }
                        if (scale_game_frames(5) < hero.rail_off_frames) {
                            hero_set_state(HERO_STATE_FALL, 1);
                            hero.unk1C8 = scale_game_frames(8);
                        }
                    } else if (hero.state.current == 0x29 || hero.state.current == 0x2A) {
                        if ((scale_game_frames(5) < hero.rail_off_frames ||
                             hero.rail_gap_jump != 0) &&
                            hero.grind_rail != 0 &&
                            hero.motion.pos.f[2] - hero.rail_point.f[2] < 0.35f) {
                            hero_set_state(HERO_STATE_GRIND, 1);
                            break;
                        }
                        if (hero.state.current == 0x29 &&
                            hero.state_timer < scale_game_frames(0xA) && hero.past_jump_apex == 0 &&
                            hero.unk12EE == 0 && hero.stick_magnitude > 0.5f &&
                            hero.rail_switching == 0) {
                            stick_target(0, 1.0f);
                            if (fast_difference_between_rotations(
                                    fast_difference_between_rotations(hero.motion.move_yaw,
                                                                      hero.rail_dir_yaw),
                                    1.5707964f) < 1.2217305f) {
                                float ang;
                                float d;
                                float lim;

                                ang = fast_add_rotations(hero.rail_dir_yaw, 1.5707964f);
                                if (1.5707964f <
                                    fast_difference_between_rotations(hero.motion.move_yaw, ang)) {
                                    ang = fast_add_rotations(hero.rail_dir_yaw, -1.5707964f);
                                }
                                lim = 4.5f;
                                for (d = 2.0f; d <= lim; d += 0.75f) {
                                    float t = hero.motion.speed_xy * (float)scale_game_frames(0);

                                    qcopy(query_position, hero.motion.pos.f);
                                    path_seg = hero.rail_segment;
                                    path_t = hero.rail_seg_frac;
                                    /* Step a copy of the hero position along its current path. */
                                    if (sample_path((int *)hero.rail_spline,
                                                    query_position,
                                                    &path_seg,
                                                    &path_t,
                                                    hero.rail_spline_loops,
                                                    t)) {
                                        fast_vec_scale(spatial_query_vector,
                                                       hero.motion.displacement.f,
                                                       scale_game_frames(0x30));
                                        fast_vec_add(query_position,
                                                     hero.motion.pos.f,
                                                     spatial_query_vector);
                                    }
                                    query_position[0] += fast_cos(ang) * d;
                                    query_position[1] += fast_sin(ang) * d;
                                    /* Look for another path (not this one) near the probe. */
                                    if (nearest_rail_point(query_position,
                                                           &new_path,
                                                           spatial_query_vector,
                                                           &new_seg,
                                                           &new_t,
                                                           &new_loop,
                                                           hero.rail_spline,
                                                           0)) {
                                        hero.transfer_from_spline = hero.rail_spline;
                                        hero.prev_rail_segment = hero.rail_segment;
                                        hero.prev_rail_seg_frac = hero.rail_seg_frac;
                                        hero.unk5A0 = hero.rail_spline_loops;
                                        qcopy(hero.prev_rail_point.f, hero.rail_point.f);
                                        hero.rail_spline = new_path;
                                        hero.rail_spline_loops = new_loop;
                                        FUN_L00_0025df68((void *)hero.rail_spline,
                                                         hero.motion.pos.f,
                                                         hero.rail_point.f,
                                                         &hero.rail_segment,
                                                         &hero.rail_seg_frac,
                                                         hero.rail_spline_loops,
                                                         999.0f,
                                                         2.0f,
                                                         0.0f);
                                        hero.rail_transfer_frames = 0;
                                        hero.rail_switching = 1;
                                        hero_set_state(0x2A, 1);
                                        break;
                                    }
                                }
                            }
                        }
                        if (hero.state_timer > hero.takeoff_frame) {
                            if (gravity_z(hero.motion.velocity.f) < 0.001f) {
                                hero.past_jump_apex = 1;
                            }
                        }
                        if (hero.state_timer >= hero.takeoff_frame + scale_game_frames(1) &&
                            hero.state_timer < hero.takeoff_frame + scale_game_frames(9) &&
                            hero.past_jump_apex == 0 && hero.anim_changing == 0) {
                            predict_hero_trajectory_z(hero.motion.pos.f,
                                                      hero.motion.velocity.f,
                                                      &grind_jump_apex_steps,
                                                      hero.jump_gravity);
                            adjust_hero_animation_speed_for_target_frame(hero.apex_anim_frame,
                                                                         grind_jump_apex_steps,
                                                                         0.5f,
                                                                         frame_scale_sq * 0.25f);
                            if (hero.rail_switching != 0) {
                                if (hero.anim_speed > 2.0f) {
                                    hero.anim_speed = 2.0f;
                                }
                                if (hero.anim_speed < 0.5f) {
                                    hero.anim_speed = 0.5f;
                                }
                            }
                        } else if (hero.past_jump_apex != 0 && hero.rail_land_frames == 0) {
                            hero.frames_to_land = scale_game_frames(0x1E);
                            if (hero.rail_off_frames == 0) {
                                hero.frames_to_land =
                                    truncate_float_to_s32(predict_hero_trajectory_intercept_time(
                                        60.0f, hero.jump_gravity, hero.rail_point.f[2]));
                            }
                            if (hero.frames_to_land != 0) {
                                adjust_hero_animation_speed_for_target_frame(hero.target_anim_frame,
                                                                             hero.frames_to_land,
                                                                             0.5f,
                                                                             frame_scale_sq *
                                                                                 0.25f);
                            }
                            if (hero.rail_switching != 0) {
                                if (hero.anim_speed > 2.0f) {
                                    hero.anim_speed = 2.0f;
                                }
                                if (hero.anim_speed < 0.5f) {
                                    hero.anim_speed = 0.5f;
                                }
                            }
                            if (hero.rail_off_frames == 0 && hero.ground_distance < 0.5f) {
                                if (hero.motion.pos.f[2] + hero.motion.velocity.f[2] <
                                    hero.rail_point.f[2]) {
                                    hero.motion.velocity.f[2] =
                                        hero.rail_point.f[2] - hero.motion.pos.f[2];
                                    hero.rail_land_frames = scale_game_frames(1);
                                    hero.anim_speed = 1.0f;
                                }
                            }
                            if (-0.5f > hero.motion.pos.f[2] - hero.rail_point.f[2]) {
                                hero_set_state(HERO_STATE_FALL, 1);
                                hero.move_lock_timer = scale_game_frames(0x46);
                                hero.unk1C8 = scale_game_frames(8);
                                break;
                            }
                        }
                        if (hero.rail_land_frames != 0) {
                            hero.rail_land_frames = hero.rail_land_frames + 1;
                            if (scale_game_frames(2) < hero.rail_land_frames) {
                                int a;

                                hero.state.current = 0x28;
                                a = hero.grind_anim_variant + 0x31;
                                set_hero_animation(a, 0, scale_game_frames(7));
                                break;
                            }
                        }
                        if (hero.rail_off_frames != 0 && hero.air_frames.s == 0 &&
                            hero.past_jump_apex != 0) {
                            if (hero_set_state(HERO_STATE_IDLE, 0)) {
                                set_hero_animation(0xC, 4, scale_game_frames(6));
                            }
                        }
                    } else if (hero.state.current == 0x42) {
                        if (hero.motion.velocity.f[2] > 0.0f) {
                            predict_hero_trajectory_z(hero.motion.pos.f,
                                                      hero.motion.velocity.f,
                                                      &grind_recovery_apex_steps,
                                                      frame_time_sq * 23.0f);
                            adjust_hero_animation_speed_for_target_frame(
                                9.0f, grind_recovery_apex_steps, 0.5f, -1.0f);
                        }
                        if (hero.anim_flags & 2) {
                            hero_set_state(HERO_STATE_GRIND, 1);
                            break;
                        }
                        if (hero.motion.velocity.f[2] < 0.0f) {
                            if (absolute_float(hero.motion.pos.f[2] - hero.rail_point.f[2]) <
                                0.3f) {
                                hero_set_state(HERO_STATE_GRIND, 1);
                            }
                        }
                    }
                    break;
                case HERO_STATE_FOLLOW_PATH:
                    if (((unsigned char *)hero.moby)[0x53] == 0x73) {
                        if (test_hero_animation_frame_window(11.0f)) {
                            allocate_hero_voice_for_entry(0xD, 0);
                            emit_randomized_sparkle_pairs(hero.grab_rail_point.f, 4);
                        }
                        if (hero.anim_flags & 2) {
                            set_hero_animation(0x66, 0, scale_game_frames(0x1E));
                        }
                    }
                    if (scale_game_frames(5) < hero.follow_path_end_frames) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        hero.rail_grab_cooldown = scale_game_frames(8);
                    }
                    break;
                case HERO_STATE_LEDGE_HANG_ENTRY:
                    if (13.5f < hero.anim_frame) {
                        hero_set_state(0x19, 0);
                    }
                    break;
                case HERO_STATE_LEDGE_HANG:
                    if ((hero.anim_flags & 2) && ((unsigned char *)hero.moby)[0x53] == 0x20) {
                        set_hero_animation(0x21, 0, scale_game_frames(6));
                    }
                    stick_target(0, 1.0f);
                    qcopy(query_position, hero.motion.pos.f);
                    ledge_hang_probe_yaw = hero.motion.rot.f[2];
                    if (probe_hero_ledge_edge(query_position, &ledge_hang_probe_yaw) == 0) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        hero.ledge_probe_cooldown = scale_game_frames(0xA);
                        break;
                    }
                    if ((hero.stick_magnitude > 0.5f &&
                         pad_pressed_within(0x40, scale_game_frames(9), 0) &&
                         2.3561945f < fast_difference_between_rotations(hero.motion.move_yaw,
                                                                        hero.motion.rot.f[2])) ||
                        (controller_state.pressed & 0xA)) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        hero.ledge_probe_cooldown = scale_game_frames(0xA);
                        break;
                    }
                    if (pad_pressed_within(0x40, scale_game_frames(0x11), 0)) {
                        hero_set_state(0x1C, 1);
                        break;
                    }
                    if (0.5f < hero.stick_magnitude &&
                        fast_difference_between_rotations(
                            hero.motion.move_yaw,
                            fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) < 0.7853982f) {
                        qcopy(spatial_query_vector, hero.motion.pos.f);
                        spatial_query_vector[0] +=
                            fast_cos(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
                        spatial_query_vector[1] +=
                            fast_sin(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
                        ledge_positive_side_probe_yaw = hero.motion.rot.f[2];
                        if (probe_hero_ledge_edge(spatial_query_vector,
                                                  &ledge_positive_side_probe_yaw)) {
                            hero_set_state(0x1A, 1);
                        }
                        break;
                    }
                    if (0.5f < hero.stick_magnitude &&
                        fast_difference_between_rotations(
                            hero.motion.move_yaw,
                            fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) < 0.7853982f) {
                        qcopy(spatial_query_vector, hero.motion.pos.f);
                        spatial_query_vector[0] +=
                            fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
                        spatial_query_vector[1] +=
                            fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
                        ledge_negative_side_probe_yaw = hero.motion.rot.f[2];
                        if (probe_hero_ledge_edge(spatial_query_vector,
                                                  &ledge_negative_side_probe_yaw)) {
                            hero_set_state(0x1B, 1);
                        }
                    }
                    break;
                case HERO_STATE_LEDGE_SHIMMY_POSITIVE_YAW:
                case HERO_STATE_LEDGE_SHIMMY_NEGATIVE_YAW: {
                    float ang;

                    qcopy(query_position, hero.motion.pos.f);
                    ang = 1.5707964f;
                    if (hero.state.current == 0x1B) {
                        ang = -1.5707964f;
                    }
                    query_position[0] +=
                        fast_cos(fast_add_rotations(hero.motion.rot.f[2], ang)) * 0.3f;
                    query_position[1] +=
                        fast_sin(fast_add_rotations(hero.motion.rot.f[2], ang)) * 0.3f;
                    ledge_shimmy_probe_yaw = hero.motion.rot.f[2];
                    stick_target(0, 1.0f);
                    if (hero.stick_magnitude < 0.5f ||
                        0.7853982f < fast_difference_between_rotations(
                                         hero.motion.move_yaw,
                                         fast_add_rotations(hero.motion.rot.f[2], ang)) ||
                        probe_hero_ledge_edge(query_position, &ledge_shimmy_probe_yaw) == 0) {
                        if (hero.anim_flags & 2) {
                            hero_set_state(0x19, 1);
                            break;
                        }
                    }
                }
                    if (0.5f < hero.stick_magnitude &&
                        pad_pressed_within(0x40, scale_game_frames(9), 0) &&
                        2.3561945f < fast_difference_between_rotations(hero.motion.move_yaw,
                                                                       hero.motion.rot.f[2])) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        hero.ledge_probe_cooldown = scale_game_frames(0x28);
                        break;
                    }
                    if (pad_pressed_within(0x40, scale_game_frames(9), 0)) {
                        hero_set_state(0x1C, 1);
                    }
                    break;
                case HERO_STATE_SLIP:
                    if (scale_game_frames(0x12C) < hero.state_timer) {
                        hero_respawn();
                    }
                    if (hero.on_slip_ground != 0) {
                        break;
                    }
                    if (hero.air_frames.s != 0) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        break;
                    }
                    hero_set_state(HERO_STATE_IDLE, 1);
                    break;
                case HERO_STATE_FALL:
                case HERO_STATE_ITEM_RELEASE_FALL:
                    if (hero.state.step == 0) {
                        if (!(scale_game_frames(0x12) > hero.state_timer) ||
                            hero.ground_distance > 1.75f) {
                            set_hero_animation(0xB, 0, scale_game_frames(0x10));
                            hero.state.step = 1;
                            if (hero.state.current == 0x2D) {
                                hero.unk2284 = 7;
                            } else {
                                hero.unk2284 = 0;
                            }
                        }
                    }
                    if (hero.air_frames.s == 0) {
                        int first;
                        int one;

                        emit_hero_animation_voice_and_ground_particle(
                            hero.ground_sound_class, 0, 1);
                        emit_hero_animation_voice_and_ground_particle(
                            hero.ground_sound_class, 1, 1);
                        if (frame_time * -9.0f > hero.motion.displacement.f[2]) {
                            hero.motion.displacement.f[2] = frame_time * -9.0f;
                        }
                        if (frame_time * -9.0f > hero.motion.velocity.f[2]) {
                            hero.motion.velocity.f[2] = frame_time * -9.0f;
                        }
                        if (hero.health.hp <= 0) {
                            hero_set_state(HERO_STATE_DYING, 1);
                            break;
                        }
                        first = hero.ground_moby == 0;
                        one = 1;
                        if (!(scale_game_frames(0x5A) > hero.state_timer)) {
                            if (hero_set_state(HERO_STATE_IDLE, 0)) {
                                set_hero_animation(0xC, 4, scale_game_frames(6));
                            }
                            if (first) {
                                hero.move_lock_timer = scale_game_frames(0x16);
                            }
                            configure_hero_point_effect_emission(0.022f, 0.004f, 0xA, 4, 0, 1);
                            break;
                        }
                        if (hero.state.step == one) {
                            if (hero_set_state(HERO_STATE_IDLE, 0)) {
                                set_hero_animation(0xC, 9, -1.0f);
                            }
                            gravity_set_z(hero.motion.momentum.f, hero.motion.momentum.f, 0.0f);
                            FUN_L00_0025f730(hero.motion.momentum.f, frame_time * 7.0f);
                            hero.move_lock_timer = scale_game_frames(7);
                            if (hero.state.prev == 0x2D) {
                                hero.move_lock_timer = scale_game_frames(0xA);
                            }
                            if (first) {
                                if (scale_game_frames(0x32) < hero.state_timer) {
                                    hero.move_lock_timer = scale_game_frames(0x12);
                                }
                            }
                            configure_hero_point_effect_emission(0.019f, 0.002f, 0xA, 2, 0, 1);
                            break;
                        }
                        if (hero.stick_magnitude > 0.5f && hero.move_lock_timer == 0) {
                            hero_set_state(HERO_STATE_RUN, 1);
                            configure_hero_point_effect_emission(0.016f, 0.002f, 7, 2, 0, 1);
                            break;
                        }
                        if (frame_time * 3.0f < hero.motion.flat_speed) {
                            if (hero_set_state(HERO_STATE_STOP, 0) == 0) {
                                break;
                            }
                            set_hero_animation(6, 0, scale_game_frames(0xC));
                            FUN_L00_0025f730(hero.motion.momentum.f, frame_time * 7.7f);
                            break;
                        }
                        hero_set_state(HERO_STATE_IDLE, 1);
                        configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 0, 1);
                        break;
                    }
                    if (hero.health.hp <= 0) {
                        break;
                    }
                    if ((controller_state.held & 0x40) && item_available[2] != 0 &&
                        hero_slot_item_id(3) != 4 && hero.pack_disabled == 0 &&
                        hero.move_lock_timer == 0 && hero.ground_distance > 0.6f) {
                        hero_set_state(HERO_STATE_LAND, 1);
                        break;
                    }
                    if (hero.still_frames > scale_game_frames(0x14)) {
                        int t = scale_game_frames(6);
                        if (hero.jump_buffer_lockout != 0) {
                            t = 1;
                        }
                        if (try_start_hero_jump(t, 0)) {
                            break;
                        }
                    }
                    if (hero_slot_item_id(3) == 3 && hero.pack_disabled == 0) {
                        if (check_pad_press_separation(
                                0x40, 0xA, scale_game_frames(9), scale_game_frames(8))) {
                            if ((controller_state.held & 0xA) &&
                                (controller_state.pressed & 0x40)) {
                                if (hero.past_jump_apex == 0 || hero.ground_distance > 1.5f) {
                                    hero_set_state(HERO_STATE_PACK_LONG_JUMP, 1);
                                    break;
                                }
                            }
                        }
                    }
                    if (hero.ledge_found != 0) {
                        hero_set_state(0x18, 1);
                        break;
                    }
                    if (hero.grind_rail != 0) {
                        hero_set_state(HERO_STATE_GRIND, 1);
                        break;
                    }
                    if (hero.grab_rail_found != 0) {
                        hero_set_state(0x74, 1);
                    }
                    break;
                case HERO_STATE_FATAL_FALL:
                    if ((current_level_index == 3 && 5.0f > hero.motion.pos.f[2]) ||
                        (current_level_index == 6 && 50.0f > hero.motion.pos.f[2]) ||
                        (current_level_index == 0x10 &&
                         (77.0f > hero.motion.pos.f[2] ||
                          (hero.state_timer > 0x14 && hero.wall_hit != 0 &&
                           hero.motion.displacement.f[2] >= 0.0f))) ||
                        hero.state_timer > scale_game_frames(0x78)) {
                    respawn:
                        hero_respawn();
                        return;
                    }
                    break;
                case HERO_STATE_DELAYED_JUMP_OR_R1_REPEAT_EXIT_WAIT:
                    if (hero.on_slip_ground != 0 && hero.ground_distance < 0.5f) {
                        hero_set_state(HERO_STATE_SLIP, 1);
                        break;
                    }
                    {
                        int both = 0;

                        fast_dec_timer(&r1_repeat_timer);
                        if (r1_repeat_timer != 0) {
                            both = 1;
                            if (!((controller_state.held & 8) && (controller_state.pressed & 8))) {
                                both = 0;
                            }
                        }
                        if (controller_state.pressed & 8) {
                            r1_repeat_timer = scale_game_frames(0x1E);
                        }
                        if (!(hero.state_timer > scale_game_frames(0x14))) {
                            break;
                        }
                        if (!(controller_state.pressed & 0x40) && !both) {
                            break;
                        }
                    }
                    hero.unk22CA = 0;
                    hero.move_lock_timer = scale_game_frames(0x19);
                    if (hero.air_frames.s != 0) {
                        if (hero_set_state(HERO_STATE_IDLE, 0) == 0) {
                            break;
                        }
                        set_hero_animation(7, 0x17, scale_game_frames(9));
                        configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 0, 1);
                        break;
                    }
                    if (hero_set_state(HERO_STATE_IDLE, 0) == 0) {
                        break;
                    }
                    set_hero_animation(select_hero_help_value(0), 0, -2.0f);
                    configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 0, 1);
                    break;
                case HERO_STATE_LAND:
                    if (hero.wall_jump_window != 0) {
                        if (scale_game_frames(9) > hero.state_timer) {
                            int hit = 0;
                            if (hero.state.prev == 7 || hero.state.prev == 9 ||
                                hero.state.prev == 0xE || hero.state.prev == 0x11) {
                                hit = 1;
                            }
                            if (hit) {
                                hero_set_state(0x11, 1);
                            }
                            break;
                        }
                    }
                    if (hero.state_timer > scale_game_frames(0x1E) &&
                        !(controller_state.held & 0x40) &&
                        !(controller_state.held_unmasked & 0x40)) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        break;
                    }
                    if (hero.ledge_found != 0) {
                        hero_set_state(0x18, 1);
                        break;
                    }
                    if (hero.air_frames.s != 0 &&
                        !(hero.motion.pos.f[2] - hero.ground_z.f < 0.4f)) {
                        break;
                    }
                    if (scale_game_frames(0xA) > hero.state_timer) {
                        hero.jump_buffer_lockout = scale_game_frames(0xC) - hero.state_timer;
                    }
                    stick_target(0, 1.0f);
                    if (hero.state.prev == 0x10 && scale_game_frames(4) > hero.state_timer) {
                        if (hero_set_state(HERO_STATE_STOP, 0) == 0) {
                            break;
                        }
                        set_hero_animation(5, 0, -1.0f);
                        hero.move_lock_timer = scale_game_frames(0x22);
                        break;
                    }
                    if (controller_state.held & 0xA) {
                        hero_set_state(HERO_STATE_CROUCH, 1);
                        configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 0, 1);
                        break;
                    }
                    if (hero.target_speed > D_L17_0017C6B8.fC &&
                        fast_difference_between_rotations(hero.motion.move_yaw,
                                                          hero.motion.rot.f[2]) < 1.5707964f) {
                        if (hero.on_ice_ground != 0 || hero.wading != 0) {
                            hero_set_state(HERO_STATE_RUN, 1);
                            break;
                        }
                        hero_set_state(HERO_STATE_RUN, 0);
                        if (hero.state.current == HERO_STATE_RUN) {
                            hero.state.step = 1;
                            set_hero_animation(4, 2, scale_game_frames(8));
                            hero.run_start_blending = 1;
                        }
                        configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 1, 0);
                        break;
                    }
                    if (D_L17_0017C6B8.fC < hero.target_speed) {
                        if (hero.on_ice_ground != 0 || hero.wading != 0) {
                            hero_set_state(HERO_STATE_RUN, 1);
                            break;
                        }
                        hero_set_state(HERO_STATE_RUN, 0);
                        if (hero.state.current == HERO_STATE_RUN) {
                            hero.state.step = 1;
                            set_hero_animation(4, D_L17_0015F6DC_d, scale_game_frames(0xC));
                        }
                        configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 1, 0);
                        break;
                    }
                    if (hero.air_frames.s == 0) {
                        if (hero_set_state(HERO_STATE_IDLE, 0) == 0) {
                            break;
                        }
                        set_hero_animation(select_hero_help_value(0), 0, -2.0f);
                        configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 0, 1);
                        break;
                    }
                    if (hero_set_state(HERO_STATE_IDLE, 0) == 0) {
                        break;
                    }
                    set_hero_animation(7, 0x17, scale_game_frames(9));
                    configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 0, 1);
                    break;
                case HERO_STATE_JUMP:
                case HERO_STATE_RUN_JUMP:
                case HERO_STATE_PACK_JUMP:
                case HERO_STATE_HIGH_JUMP:
                case HERO_STATE_STORED_HEADING_JUMP:
                case HERO_STATE_JUMP_WITH_TRANSFORM_TRAIL:
                case HERO_STATE_DOUBLE_JUMP:
                case HERO_STATE_JUMP_WITH_VELOCITY_KEY_PROFILE:
                case HERO_STATE_PACK_LONG_JUMP:
                case HERO_STATE_REPEATABLE_JUMP:
                case HERO_STATE_WATER_SURFACE_JUMP:
                case HERO_STATE_LEDGE_JUMP:
                case HERO_STATE_JUMP_WITH_HEALTH_DAMAGE:
                case HERO_STATE_JUMP_FROM_RESPAWN_WAIT:
                    if (hero.can_chain_high_jump != 0) {
                        int t = get_hero_high_jump_input_window_frames();
                        if (t > 0 && (hero.state_timer < t || hero.rail_land_frames != 0) &&
                            hero.grounded_frames != 0) {
                            if (hero.state.current != HERO_STATE_HIGH_JUMP ||
                                pad_pressed_within(0x40, scale_game_frames(7), 0)) {
                                int ok = 1;
                                if (hero.state.current == 0x1C || hero.rail_land_frames != 0) {
                                    if (pad_pressed_within(0x40, scale_game_frames(7), 0) == 0) {
                                        ok = 0;
                                    }
                                }
                                if (hero.state.current == HERO_STATE_HIGH_JUMP) {
                                    if (hero.rail_land_frames == 0) {
                                        ok = 0;
                                    }
                                }
                                if (ok) {
                                    if (pad_pressed_within(0x1F000, scale_game_frames(5), 0)) {
                                        if (controller_state.held & 0xA) {
                                            hero_set_state(HERO_STATE_HIGH_JUMP, 1);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (hero_slot_item_id(3) == 3 && hero.pack_disabled == 0 &&
                        hero.state.current != HERO_STATE_PACK_LONG_JUMP &&
                        hero.state.current != 0x11 && hero.state.current != 0x12 &&
                        hero.state.current != 0x3C && hero.state.current != 0xF &&
                        hero.state.current != HERO_STATE_PACK_JUMP && hero.state.current != 0x69 &&
                        (hero.state.current == 0xE || scale_game_frames(9) < hero.state_timer) &&
                        (controller_state.held & 0xA)) {
                        if (pad_pressed_within(0x40, scale_game_frames(8), 0)) {
                            hero_set_state(HERO_STATE_PACK_LONG_JUMP, 1);
                            break;
                        }
                    }
                    if (scale_game_frames(7) > hero.state_timer && pad_pressed_within(0xA, 2, 0) &&
                        hero.state.current != HERO_STATE_PACK_LONG_JUMP &&
                        hero.state.current != HERO_STATE_PACK_JUMP && hero.state.current != 0x12 &&
                        hero.state.current != 0x3C && hero.state.current != 0x11 &&
                        hero.state.current != 0xE && hero.state.current != 0x69) {
                        if (hero_slot_item_id(3) == 2 && item_available[2] != 0 &&
                            hero.pack_disabled == 0 &&
                            frame_time * 0.9f < hero.motion.forward_speed) {
                            hero_set_state(HERO_STATE_PACK_JUMP, 1);
                        } else if (hero_slot_item_id(3) == 3 && hero.pack_disabled == 0 &&
                                   frame_time * 0.9f < hero.motion.forward_speed &&
                                   hero.long_jump_lockout == 0) {
                            hero_set_state(HERO_STATE_PACK_LONG_JUMP, 1);
                        }
                    }
                    if (hero.takeoff_frame + scale_game_frames(8) < hero.state_timer &&
                        (hero.past_jump_apex == 0 ||
                         scale_game_frames(0xA) < hero.frames_to_land) &&
                        hero.stick_magnitude < 0.3f &&
                        (hero.state.current == HERO_STATE_JUMP ||
                         hero.state.current == HERO_STATE_RUN_JUMP || hero.state.current == 0xD ||
                         hero.state.current == 0xF || hero.state.current == 0xE)) {
                        if (pad_pressed_within(0xA, scale_game_frames(9), 0)) {
                            if (hero_slot_item_id(3) != 3) {
                                break;
                            }
                            if (hero.pack_disabled != 0) {
                                break;
                            }
                            hero_set_state(0x22, 1);
                            break;
                        }
                    }
                    if (hero.grind_rail != 0) {
                        hero_set_state(HERO_STATE_GRIND, 1);
                        break;
                    }
                    if (hero.grab_rail_found != 0) {
                        hero_set_state(0x74, 1);
                        break;
                    }
                    if (hero.jump_max_frames < hero.state_timer) {
                        if (hero.rail_land_frames == 0) {
                            hero_set_state(HERO_STATE_FALL, 1);
                            break;
                        }
                    } else if (hero.rail_land_frames == 0 &&
                               (hero.state.current == HERO_STATE_PACK_JUMP ||
                                hero.state.current == HERO_STATE_PACK_LONG_JUMP) &&
                               hero.wall_probe_distance < 0.9f && hero.wall_hit != 0) {
                        if (fast_vec_length(hero.motion.velocity.f) > frame_time * 7.0f) {
                            float len = fast_vec_length(hero.motion.displacement.f);
                            if (len < fast_vec_length(hero.motion.velocity.f) * 0.5f &&
                                hero.ledge_found == 0 && hero.ground_distance < 2.0f) {
                                hero_set_state(HERO_STATE_WALL_BONK, 1);
                                break;
                            }
                        }
                    }
                    {
                        int isNear;

                        isNear = 0;
                        if (hero.state.current == 0xE && hero.anim_frame < 35.0f) {
                            isNear = 1;
                        }
                        if (!(hero.state_timer > hero.takeoff_frame)) {
                            break;
                        }
                        if (hero.state_timer == hero.takeoff_frame + 1) {
                            if (hero.velocity_keys_active != 0) {
                                float d = hero.unk3D0 - hero.state_timer;
                                if (0.0f < d) {
                                    adjust_hero_animation_speed_for_target_frame(
                                        hero.keyed_move_target_frame,
                                        d,
                                        hero.jump_anim_retime_scale,
                                        -1.0f);
                                }
                                break;
                            }
                            if (hero.state.current == 0xE) {
                                break;
                            }
                            predict_hero_trajectory_z(hero.motion.pos.f,
                                                      hero.motion.velocity.f,
                                                      &jump_apex_steps,
                                                      hero.jump_gravity);
                            adjust_hero_animation_speed_for_target_frame(
                                hero.apex_anim_frame,
                                jump_apex_steps,
                                hero.jump_anim_retime_scale,
                                -1.0f);
                            if (1.5f < hero.anim_speed) {
                                hero.anim_speed = 1.5f;
                            }
                            break;
                        }
                        if (hero.velocity_keys_active != 0 && !(hero.state_timer < hero.unk3D0) &&
                            hero.state_timer < hero.velocity_keys_end_frame) {
                            float d = hero.keyed_anim_target_frame - hero.unk3D0;
                            if (0.0f < d) {
                                adjust_hero_animation_speed_for_target_frame(
                                    hero.apex_anim_frame, d, hero.jump_anim_retime_scale, -1.0f);
                            }
                            break;
                        }
                        if (hero.in_water_frames == 1) {
                            allocate_hero_voice_for_entry(0x11, 0);
                            emit_hero_surface_effect_burst(3, 0x18, 1);
                        }
                        if (hero.rail_land_frames != 0) {
                            hero.rail_land_frames = hero.rail_land_frames + 1;
                        } else if (hero.air_frames.s == 0 && hero.past_jump_apex != 0 && !isNear) {
                            int fr;

                            hero.rail_land_frames = scale_game_frames(1);
                            configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 0, 1);
                            fr = truncate_float_to_s32(hero.jump_land_anim_frame) + 2;
                            if (hero.target_anim_frame - 1.5f > hero.anim_frame) {
                                set_hero_animation(return_hero_animation_id_unchanged(moby[0x53]),
                                                   fr,
                                                   scale_game_frames(7));
                            }
                        }
                        if (hero.rail_land_frames != 0) {
                            hero.anim_speed = 1.0f;
                        } else if (hero.past_jump_apex != 0) {
                            if (moby[0x51] < truncate_float_to_s32(hero.jump_land_anim_frame)) {
                                if (hero.state.current == 0xE && hero.anim_frame < 37.0f) {
                                    approach_value(&hero.anim_speed, 1.0f, frame_scale_sq * 0.15f);
                                } else {
                                    int jump_trajectory_intercept_frames = truncate_float_to_s32(
                                        predict_hero_trajectory_intercept_time(
                                            60.0f, hero.jump_gravity, -1.0f));
                                    hero.frames_to_land = jump_trajectory_intercept_frames;
                                    adjust_hero_animation_speed_for_target_frame(
                                        hero.target_anim_frame,
                                        jump_trajectory_intercept_frames,
                                        hero.jump_anim_retime_scale,
                                        frame_scale_sq * 0.2f);
                                }
                                if (!isNear) {
                                    int fr = truncate_float_to_s32(hero.jump_land_anim_frame) + 2;
                                    if (hero.anim_speed > 1.8f) {
                                        set_hero_animation(
                                            return_hero_animation_id_unchanged(moby[0x53]),
                                            fr,
                                            hero.frames_to_land + 3);
                                    }
                                }
                                if (0.2f > hero.anim_speed) {
                                    hero.anim_speed = 0.2f;
                                }
                                if (hero.anim_speed > 2.7f) {
                                    hero.anim_speed = 2.7f;
                                }
                            }
                            if (hero.state.current == HERO_STATE_RUN_JUMP &&
                                hero.target_anim_frame - hero.anim_frame > 6.0f &&
                                (return_hero_animation_id_unchanged(moby[0x52]) == 8 ||
                                 return_hero_animation_id_unchanged(moby[0x52]) == 9) &&
                                moby[0x52] == moby[0x53] &&
                                D_L17_0017C6B8.f18 * frame_time * 0.1f > hero.target_speed &&
                                frame_time * 2.5f > hero.motion.flat_speed) {
                                float sp = (float)scale_game_frames(9) / hero.anim_speed;
                                if ((float)scale_game_frames(0xA) < sp) {
                                    sp = scale_game_frames(0xA);
                                }
                                set_hero_animation(
                                    7, (truncate_float_to_s32(hero.anim_frame) + 4) / 2, sp);
                                hero.state.current = 7;
                            }
                            if (truncate_float_to_s32(hero.anim_frame) ==
                                truncate_float_to_s32(hero.target_anim_frame)) {
                                hero.anim_speed = 0.0f;
                            }
                        }
                    }
                    if (hero.rail_land_frames != 0) {
                        if (hero.state.current == HERO_STATE_PACK_JUMP) {
                            if (hero_set_state(HERO_STATE_STOP, 0) == 0) {
                                break;
                            }
                            set_hero_animation(5, 0, -2.0f);
                            fast_vec_scale(hero.motion.momentum.f, hero.motion.momentum.f, 0.8f);
                            break;
                        }
                        if (hero.on_slip_ground == 0 && hero.move_lock_timer == 0) {
                            if (pad_pressed_within(0x40, scale_game_frames(8), 0)) {
                                if (hero.air_frames.s < scale_game_frames(4)) {
                                    if (hero.target_speed > D_L17_0017C6B8.fC &&
                                        fast_difference_between_rotations(hero.motion.move_yaw,
                                                                          hero.motion.rot.f[2]) <
                                            1.0471976f &&
                                        D_L17_0017C6B8.f18 * frame_time * 0.85f <
                                            hero.motion.flat_speed) {
                                        hero_set_state(HERO_STATE_RUN_JUMP, 1);
                                        break;
                                    }
                                    hero_set_state(HERO_STATE_JUMP, 1);
                                    break;
                                }
                            }
                        }
                    }
                    if (hero.rail_land_frames > scale_game_frames(1) &&
                        hero.state.current != HERO_STATE_PACK_JUMP) {
                        update_hero_locomotion_state();
                    } else if (hero.past_jump_apex != 0 &&
                               !(hero.fall_delay_frames > hero.state_timer) &&
                               hero.ground_distance > hero.fall_start_distance) {
                        hero_set_state(HERO_STATE_FALL, 1);
                    }
                    if (hero.state.current == HERO_STATE_PACK_LONG_JUMP &&
                        scale_game_frames(0x3C) < hero.state_timer && hero.ground_distance > 1.5f) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        break;
                    }
                    {
                        int hit = 0;
                        if (hero.state.current == HERO_STATE_JUMP ||
                            hero.state.current == HERO_STATE_RUN_JUMP ||
                            hero.state.current == 0x11) {
                            hit = 1;
                        }
                        if (hero.wall_jump_window != 0 && hero.rail_land_frames == 0 && hit) {
                            if (hero.state.current != 0x11 ||
                                scale_game_frames(0x19) < hero.state_timer) {
                                int a = scale_game_frames(7);
                                if (hero.state.current == 0x11) {
                                    a = scale_game_frames(7);
                                }
                                if (pad_pressed_within(0x40, a, &airborne_jump_press_age_frames)) {
                                    if (airborne_jump_press_age_frames < hero.state_timer) {
                                        hero_set_state(0x11, 1);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if (hero.state.current == HERO_STATE_PACK_JUMP ||
                        hero.state.current == HERO_STATE_PACK_LONG_JUMP) {
                        if (scale_game_frames(0x19) < hero.state_timer &&
                            hero.on_flagged_ground_moby != 0) {
                            hero.unk4A6 = scale_game_frames(0x14);
                        }
                    }
                    {
                        int delay;
                        int n;

                        delay = scale_game_frames(0x14);
                        if (hero.state.current == HERO_STATE_PACK_LONG_JUMP) {
                            delay = scale_game_frames(8);
                        }
                        if (hero.state.current == 0xE && hero.ground_distance > 4.0f) {
                            if (hero.past_jump_apex == 0 && hero.anim_speed < 1.3f) {
                                approach_value(&hero.anim_speed, 1.3f, frame_scale * 0.1f);
                            }
                            delay = scale_game_frames(0x11);
                            hero.unk4A6 = delay;
                        }
                        n = hero.state_timer - 4;
                        if (n < 2) {
                            n = 2;
                        }
                        if (scale_game_frames(0x1E) < n) {
                            n = scale_game_frames(0x1E);
                        }
                        if (hero.rail_land_frames == 0 && hero.state.current != 0x12 &&
                            hero.state.current != 0x1C && hero.state.current != 0x11 &&
                            hero.state.current != HERO_STATE_HIGH_JUMP &&
                            hero.state.current != 0x3C && hero.state.current != 0x69) {
                            if (pad_pressed_within(0x40, n, 0) || hero.state.current == 0xE ||
                                hero.state.current == 0xF || hero.state.current == 0xD) {
                                if (hero.state.current != 0xE && hero.item_anim_active == 0 &&
                                    (hero.on_slip_ground == 0 || !(hero.ground_distance < 0.4f)) &&
                                    hero.can_double_jump != 0 &&
                                    scale_game_frames(0xE) < hero.state_timer &&
                                    (hero.past_jump_apex == 0 ||
                                     (hero.ground_distance > 0.7f &&
                                      absolute_float(gravity_z(hero.motion.velocity.f)) <
                                          frame_time * 5.8f))) {
                                    hero_set_state(0xE, 1);
                                } else if (item_available[2] != 0 && hero_slot_item_id(3) != 4 &&
                                           hero.pack_disabled == 0 &&
                                           (controller_state.held & 0x40) &&
                                           delay < hero.state_timer &&
                                           hero.unk4A6 < hero.state_timer) {
                                    float lim = 0.6f;
                                    if (hero.state.current == HERO_STATE_RUN_JUMP ||
                                        hero.state.current == HERO_STATE_JUMP) {
                                        lim = 1.0f;
                                    }
                                    if (hero.state.current == 0xE) {
                                        lim = 1.5f;
                                    }
                                    if (lim < hero.ground_distance) {
                                        hero_set_state(HERO_STATE_LAND, 1);
                                    }
                                }
                            }
                        }
                    }
                    if (hero.ledge_found != 0 && hero.state.current != HERO_STATE_HIGH_JUMP) {
                        hero_set_state(0x18, 1);
                    }
                    break;
                case HERO_STATE_HURT:
                    if (12.5f < hero.anim_frame && hero.air_frames.s != 0 &&
                        hero.ground_distance > 1.0f) {
                        hero_set_state(HERO_STATE_FALL, 1);
                        break;
                    }
                    if (current_level_index == 0x10 && 0.0f > hero.motion.velocity.f[2] &&
                        hero.health.hp == 0 && hero.state.prev_control_mode == 0xF) {
                        hero_set_state(0x77, 1);
                        break;
                    }
                    if (hero.state_timer > scale_game_frames(0x1B)) {
                        if (hero.health.hp <= 0) {
                            if (hero.selector_1 != 0) {
                                hero_set_state(0x7C, 1);
                                break;
                            }
                            hero_set_state(HERO_STATE_DYING, 1);
                            break;
                        }
                        if (hero.state.prev == 0x81) {
                            hero_set_state(0x81, 1);
                            break;
                        }
                        hero_set_state(HERO_STATE_IDLE, 0);
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_1_IDLE:
                    if (hero.unk22FC > scale_game_frames(0x39) - scale_game_frames(0xF)) {
                        set_hero_animation(0x12, 0, scale_game_frames(0xA));
                        break;
                    }
                    if (hero.unk22FC == 0 && (hero.anim_flags & 2)) {
                        if (((unsigned char *)hero.moby)[0x53] != 0) {
                            set_hero_animation(0, 0, scale_game_frames(0xC));
                        } else if (random_float_between(0.0f, 1.0f) < 0.5f) {
                            set_hero_animation(1, 0, scale_game_frames(8));
                        }
                    }
                    if (hero.on_slip_ground != 0) {
                        hero_set_state(0x52, 1);
                        break;
                    }
                    if (controller_state.held & 5) {
                        hero_set_state(HERO_STATE_LOOK, 1);
                        break;
                    }
                    if (hero.air_frames.s != 0) {
                        hero_set_state(0x45, 1);
                        break;
                    }
                    if (pad_pressed_within(0x40, scale_game_frames(8), 0)) {
                        hero_set_state(0x49, 1);
                        break;
                    }
                    if (pad_pressed_within(0x80, scale_game_frames(9), 0)) {
                        hero_set_state(0x51, 1);
                        break;
                    }
                    stick_target(0, 1.0f);
                    if (hero.stick_magnitude > 0.2f && hero.target_speed > 0.0f) {
                        hero_set_state(0x44, 1);
                        break;
                    }
                    if (hero.unk22FC == 0 && ((unsigned char *)hero.moby)[0x53] == 0x12) {
                        set_hero_animation(0, 0, scale_game_frames(0xC));
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_1_ATTACK:
                    if (hero.on_slip_ground != 0) {
                        hero_set_state(0x52, 1);
                        break;
                    }
                    {
                        HeroTableRow2C *rec = &D_L17_0017C528[hero.attack_record];
                        int             t;

                        if (hero.anim_changing == 0 && (float)rec->f18 < hero.anim_frame) {
                            hero_set_state(0x43, 0);
                        }
                        if (hero.anim_changing == 0 && (float)rec->f10 <= hero.anim_frame) {
                            int n = truncate_float_to_s32(hero.anim_frame) - rec->fC;

                            if (n < 1) {
                                n = 1;
                            }
                            if (scale_game_frames(0xF) < n) {
                                n = scale_game_frames(0xF);
                            }
                            if (pad_pressed_within(0x80, n, 0)) {
                                hero_set_state(0x51, 1);
                            }
                        }
                        t = (truncate_float_to_s32(hero.anim_frame) - rec->fC) * 2;
                        if ((float)rec->f14 < hero.anim_frame && hero.anim_changing == 0) {
                            if (pad_pressed_within(0x40, t, 0)) {
                                hero_set_state(0x49, 1);
                            }
                        }
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_1_JUMP:
                case HERO_STATE_ALT_LEDGE_JUMP:
                    if (!(hero.state_timer > hero.takeoff_frame)) {
                        break;
                    }
                    if (hero.state_timer == hero.takeoff_frame + scale_game_frames(1)) {
                        predict_hero_trajectory_z(hero.motion.pos.f,
                                                  hero.motion.velocity.f,
                                                  &predicted_apex_steps,
                                                  hero.jump_gravity);
                        adjust_hero_animation_speed_for_target_frame(
                            hero.apex_anim_frame, predicted_apex_steps, 0.5f, -1.0f);
                        break;
                    }
                    if (hero.velocity_keys_active != 0 && !(hero.state_timer < hero.unk3D0) &&
                        hero.state_timer < hero.velocity_keys_end_frame) {
                        float d = hero.keyed_anim_target_frame - hero.unk3D0;
                        if (0.0f < d) {
                            adjust_hero_animation_speed_for_target_frame(
                                hero.apex_anim_frame, d, 0.5f, -1.0f);
                        }
                        break;
                    }
                    if (hero.rail_land_frames != 0) {
                        hero.rail_land_frames = hero.rail_land_frames + 1;
                    } else if (hero.air_frames.s == 0 && hero.past_jump_apex != 0) {
                        int fr;

                        hero.rail_land_frames = scale_game_frames(1);
                        fr = truncate_float_to_s32(hero.target_anim_frame) + 3;
                        if (hero.anim_frame < hero.target_anim_frame) {
                            set_hero_animation(moby[0x53], fr, scale_game_frames(6));
                        }
                    }
                    if (hero.rail_land_frames != 0) {
                        hero.anim_speed = 1.0f;
                    } else if (hero.past_jump_apex != 0) {
                        if (moby[0x51] < truncate_float_to_s32(hero.target_anim_frame)) {
                            int alt_jump_trajectory_intercept_frames =
                                truncate_float_to_s32(predict_hero_trajectory_intercept_time(
                                    60.0f, hero.jump_gravity, -1.0f));
                            int fr;

                            hero.frames_to_land = alt_jump_trajectory_intercept_frames;
                            adjust_hero_animation_speed_for_target_frame(
                                hero.target_anim_frame,
                                alt_jump_trajectory_intercept_frames,
                                0.5f,
                                -1.0f);
                            fr = truncate_float_to_s32(hero.target_anim_frame) + 1;
                            if (hero.anim_speed > 1.8f) {
                                set_hero_animation(moby[0x53], fr, hero.frames_to_land + 2);
                            }
                            if (hero.anim_speed < 0.2f) {
                                hero.anim_speed = 0.2f;
                            }
                            if (hero.anim_speed > 2.7f) {
                                hero.anim_speed = 2.7f;
                            }
                        }
                        if (truncate_float_to_s32(hero.anim_frame) ==
                            truncate_float_to_s32(hero.target_anim_frame)) {
                            hero.anim_speed = 0.0f;
                        }
                    }
                    if (hero.rail_land_frames != 0 && hero.move_lock_timer == 0) {
                        if (pad_pressed_within(0x40, scale_game_frames(4), 0)) {
                            if (hero.air_frames.s < scale_game_frames(4)) {
                                stick_target(0, 1.0f);
                                hero_set_state(0x49, 1);
                                break;
                            }
                        }
                    }
                    if (scale_game_frames(1) < hero.rail_land_frames) {
                        hero.unk2284 = 0;
                        stick_target(0, 1.0f);
                        if (hero.target_speed > D_L17_0017C6B8.fC && hero.stick_magnitude > 0.5f &&
                            hero.on_slip_ground == 0 && hero.move_lock_timer == 0 &&
                            fast_difference_between_rotations(hero.motion.move_yaw,
                                                              hero.motion.rot.f[2]) < 1.5707964f) {
                            hero_set_state(0x44, 1);
                        } else if (hero.rail_land_frames > scale_game_frames(0xC)) {
                            hero_set_state(0x43, 1);
                        }
                    }
                    if (2.0f < hero.ground_distance && hero.past_jump_apex != 0 &&
                        scale_game_frames(0x28) < hero.state_timer) {
                        hero_set_state(0x45, 1);
                        break;
                    }
                    if (hero.past_jump_apex != 0 && hero.ground_distance < 0.2f) {
                        if (pad_pressed_within(0x80, scale_game_frames(5), 0)) {
                            hero_set_state(0x51, 1);
                            break;
                        }
                    }
                    if (hero.ledge_found != 0) {
                        hero_set_state(0x4A, 1);
                        break;
                    }
                    if (hero.state.current != 0x4C) {
                        if (pad_pressed_within(0x40, scale_game_frames(5), 0)) {
                            if (hero.ground_distance > 0.55f) {
                                hero_set_state(0x4F, 1);
                            }
                        }
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_1_LAND:
                    if (hero.state_timer > scale_game_frames(0x1E) &&
                        !(controller_state.held & 0x40)) {
                        hero_set_state(0x45, 1);
                        break;
                    }
                    if (hero.ledge_found != 0) {
                        hero_set_state(0x4A, 1);
                        break;
                    }
                    if (hero.air_frames.s != 0 &&
                        !(hero.motion.pos.f[2] - hero.ground_z.f < 0.0f)) {
                        break;
                    }
                    if (scale_game_frames(0xA) > hero.state_timer) {
                        hero.jump_buffer_lockout = scale_game_frames(0xC) - hero.state_timer;
                    }
                    if (hero.stick_magnitude > 0.3f) {
                        hero_set_state(0x44, 1);
                        break;
                    }
                    if (hero.air_frames.s == 0) {
                        hero_set_state(0x43, 1);
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_1_FALL:
                    if (current_level_index == 6 && 115.0f > hero.motion.pos.f[2]) {
                        goto respawn;
                    }
                    if (hero.air_frames.s == 0) {
                        if (frame_time * -9.0f > hero.motion.displacement.f[2]) {
                            hero.motion.displacement.f[2] = frame_time * -9.0f;
                        }
                        if (frame_time * -9.0f > hero.motion.velocity.f[2]) {
                            hero.motion.velocity.f[2] = frame_time * -9.0f;
                        }
                        if (hero.health.hp <= 0) {
                            hero_set_state(0x47, 1);
                            break;
                        }
                        if (hero.stick_magnitude > 0.5f && hero.move_lock_timer == 0) {
                            hero_set_state(0x44, 1);
                            break;
                        }
                        hero_set_state(0x43, 1);
                        break;
                    }
                    if (hero.health.hp <= 0) {
                        break;
                    }
                    if (hero.ledge_found != 0) {
                        hero_set_state(0x4A, 1);
                        break;
                    }
                    if (pad_pressed_within(0x40, scale_game_frames(5), 0)) {
                        if (hero.ground_distance > 0.6f) {
                            hero_set_state(0x4F, 1);
                        }
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_1_SLIP:
                    if (scale_game_frames(0x12C) < hero.state_timer) {
                        hero_respawn();
                    }
                    if (hero.on_slip_ground != 0) {
                        break;
                    }
                    if (hero.air_frames.s != 0) {
                        hero_set_state(0x45, 1);
                        break;
                    }
                    hero_set_state(0x43, 1);
                    break;
                case HERO_STATE_VEHICLE_MODE_1_RUN:
                case HERO_STATE_VEHICLE_MODE_1_SPEED_SCALED_RUN:
                    if (hero.on_slip_ground != 0) {
                        hero_set_state(0x52, 1);
                        break;
                    }
                    if (hero.state.current == 0x44) {
                        if (hero.anim_changing == 0 &&
                            frame_time * 3.0f * 0.5f > hero.motion.speed_xy &&
                            ((unsigned char *)hero.moby)[0x53] != 2) {
                            unsigned char  *m = ((unsigned char *)hero.moby);
                            unsigned char **anims = *(unsigned char ***)(m + 0x24);
                            int fr = (m[0x51] * anims[0x14][0x10] / anims[0x15][0x10] + 1) %
                                     anims[0x14][0x10];

                            set_hero_animation(2, fr, scale_game_frames(8));
                        } else if (hero.anim_changing == 0 &&
                                   frame_time * 3.0f * 0.6f < hero.motion.speed_xy &&
                                   ((unsigned char *)hero.moby)[0x53] != 3) {
                            unsigned char  *m = ((unsigned char *)hero.moby);
                            unsigned char **anims = *(unsigned char ***)(m + 0x24);
                            int fr = (m[0x51] * anims[0x15][0x10] / anims[0x14][0x10] + 7) %
                                     anims[0x15][0x10];

                            set_hero_animation(3, fr, scale_game_frames(8));
                        }
                        if (((unsigned char *)hero.moby)[0x53] == 2) {
                            hero.anim_speed = hero.motion.speed_xy * 90.0f;
                        } else if (((unsigned char *)hero.moby)[0x53] == 3) {
                            hero.anim_speed = hero.motion.speed_xy * 20.0f;
                        }
                    } else {
                        hero.anim_speed = hero.motion.speed_xy * 196.0f;
                    }
                    if (pad_pressed_within(0x40, scale_game_frames(8), 0)) {
                        hero_set_state(0x49, 1);
                        break;
                    }
                    if (pad_pressed_within(0x80, scale_game_frames(9), 0)) {
                        hero_set_state(0x51, 1);
                        break;
                    }
                    if (!(hero.air_frames.s < scale_game_frames(5))) {
                        if (hero.ground_distance > 0.8f || hero.ground_slope.f > 0.87266463f) {
                            hero_set_state(0x45, 1);
                            break;
                        }
                    }
                    if (hero.air_frames.s == 0 && hero.stick_magnitude < 0.17f) {
                        hero_set_state(0x43, 1);
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_1_HURT:
                    if (!(hero.state_timer < scale_game_frames(0x20)) && hero.air_frames.s != 0 &&
                        hero.ground_distance > 1.0f) {
                        hero_set_state(0x45, 1);
                        break;
                    }
                    if (hero.state_timer > scale_game_frames(0x23)) {
                        if (hero.health.hp <= 0) {
                            hero_set_state(0x47, 1);
                            break;
                        }
                        hero_set_state(0x43, 0);
                    }
                    break;
                case HERO_STATE_ALT_LEDGE_HANG_ENTRY:
                    if (hero.anim_flags & 2) {
                        hero_set_state(0x4B, 1);
                    }
                    break;
                case HERO_STATE_ALT_LEDGE_HANG:
                    stick_target(0, 1.0f);
                    qcopy(query_position, hero.motion.pos.f);
                    alt_ledge_hang_probe_yaw = hero.motion.rot.f[2];
                    if (probe_hero_ledge_edge(query_position, &alt_ledge_hang_probe_yaw) == 0) {
                        hero_set_state(0x45, 1);
                        hero.ledge_probe_cooldown = scale_game_frames(0xA);
                        break;
                    }
                    if ((hero.stick_magnitude > 0.5f &&
                         pad_pressed_within(0x40, scale_game_frames(9), 0) &&
                         2.3561945f < fast_difference_between_rotations(hero.motion.move_yaw,
                                                                        hero.motion.rot.f[2])) ||
                        (controller_state.held & 0xA)) {
                        hero_set_state(0x45, 1);
                        hero.ledge_probe_cooldown = scale_game_frames(0xA);
                        break;
                    }
                    if (pad_pressed_within(0x40, scale_game_frames(9), 0)) {
                        hero_set_state(0x4C, 1);
                        break;
                    }
                    if (0.5f < hero.stick_magnitude &&
                        fast_difference_between_rotations(
                            hero.motion.move_yaw,
                            fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) < 0.7853982f) {
                        qcopy(spatial_query_vector, hero.motion.pos.f);
                        spatial_query_vector[0] +=
                            fast_cos(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
                        spatial_query_vector[1] +=
                            fast_sin(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
                        alt_ledge_positive_side_probe_yaw = hero.motion.rot.f[2];
                        if (probe_hero_ledge_edge(spatial_query_vector,
                                                  &alt_ledge_positive_side_probe_yaw)) {
                            hero_set_state(0x4D, 1);
                        }
                        break;
                    }
                    if (0.5f < hero.stick_magnitude &&
                        fast_difference_between_rotations(
                            hero.motion.move_yaw,
                            fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) < 0.7853982f) {
                        qcopy(spatial_query_vector, hero.motion.pos.f);
                        spatial_query_vector[0] +=
                            fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
                        spatial_query_vector[1] +=
                            fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
                        alt_ledge_negative_side_probe_yaw = hero.motion.rot.f[2];
                        if (probe_hero_ledge_edge(spatial_query_vector,
                                                  &alt_ledge_negative_side_probe_yaw)) {
                            hero_set_state(0x4E, 1);
                        }
                    }
                    break;
                case HERO_STATE_ALT_LEDGE_SHIMMY_YAW_PLUS_90:
                case HERO_STATE_ALT_LEDGE_SHIMMY_YAW_MINUS_90: {
                    float ang;

                    qcopy(query_position, hero.motion.pos.f);
                    ang = 1.5707964f;
                    if (hero.state.current == 0x4E) {
                        ang = -1.5707964f;
                    }
                    query_position[0] +=
                        fast_cos(fast_add_rotations(hero.motion.rot.f[2], ang)) * 0.3f;
                    query_position[1] +=
                        fast_sin(fast_add_rotations(hero.motion.rot.f[2], ang)) * 0.3f;
                    alt_ledge_shimmy_probe_yaw = hero.motion.rot.f[2];
                    stick_target(0, 1.0f);
                    if (hero.stick_magnitude < 0.5f ||
                        0.7853982f < fast_difference_between_rotations(
                                         hero.motion.move_yaw,
                                         fast_add_rotations(hero.motion.rot.f[2], ang)) ||
                        probe_hero_ledge_edge(query_position, &alt_ledge_shimmy_probe_yaw) == 0) {
                        if (hero.anim_flags & 2) {
                            hero_set_state(0x4B, 1);
                            break;
                        }
                    }
                }
                    if (0.5f < hero.stick_magnitude &&
                        pad_pressed_within(0x40, scale_game_frames(9), 0) &&
                        2.3561945f < fast_difference_between_rotations(hero.motion.move_yaw,
                                                                       hero.motion.rot.f[2])) {
                        hero_set_state(0x45, 1);
                        hero.ledge_probe_cooldown = scale_game_frames(0x28);
                        break;
                    }
                    if (pad_pressed_within(0x40, scale_game_frames(9), 0)) {
                        hero_set_state(0x4C, 1);
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_3_IDLE:
                    if (hero.anim_flags & 2) {
                        if (((unsigned char *)hero.moby)[0x53] != 0) {
                            set_hero_animation(0, 0, scale_game_frames(0xC));
                        } else if (random_float_between(0.0f, 1.0f) < 0.5f) {
                            set_hero_animation(5, 0, scale_game_frames(8));
                        }
                    }
                    if (controller_state.held & 5) {
                        hero_set_state(HERO_STATE_LOOK, 1);
                        break;
                    }
                    if (hero.air_frames.s != 0) {
                        hero_set_state(0x55, 1);
                        break;
                    }
                    if (controller_state.pressed & 0x20) {
                        hero_set_state(0x59, 1);
                        break;
                    }
                    if (pad_pressed_within(0x80, scale_game_frames(7), 0)) {
                        if (hero.vehicle_toggle_timer == 0) {
                            allocate_hero_voice_for_entry(0x19, 0);
                            hero.vehicle_toggle_timer = 0x12;
                            hero.unk22DC = 1;
                        }
                        hero_switch_item(0);
                        break;
                    }
                    stick_target(0, 1.0f);
                    if (hero.stick_magnitude > 0.22f && hero.target_speed > 0.0f) {
                        hero_set_state(0x54, 1);
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_3_SLIP:
                    if (scale_game_frames(0x12C) < hero.state_timer) {
                        hero_respawn();
                    }
                    if (hero.on_slip_ground != 0) {
                        break;
                    }
                    if (hero.air_frames.s != 0) {
                        hero_set_state(0x55, 1);
                        break;
                    }
                    hero_set_state(0x53, 1);
                    break;
                case HERO_STATE_DELAYED_CIRCLE_RELEASE_WAIT:
                    if (hero.state_timer > scale_game_frames(0x2D) &&
                        !(controller_state.held & 0x20)) {
                        hero_set_state(0x53, 0);
                        set_hero_animation(0, 0, scale_game_frames(0x14));
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_3_FALL:
                    if (hero.air_frames.s != 0) {
                        break;
                    }
                    if (frame_time * -9.0f > hero.motion.displacement.f[2]) {
                        hero.motion.displacement.f[2] = frame_time * -9.0f;
                    }
                    if (frame_time * -9.0f > hero.motion.velocity.f[2]) {
                        hero.motion.velocity.f[2] = frame_time * -9.0f;
                    }
                    if (hero.health.hp <= 0) {
                        hero_set_state(0x57, 1);
                        break;
                    }
                    if (hero.stick_magnitude > 0.5f && scale_game_frames(0x28) > hero.state_timer &&
                        hero.move_lock_timer == 0) {
                        hero_set_state(0x54, 1);
                        break;
                    }
                    hero_set_state(0x53, 0);
                    set_hero_animation(7, 0, -1.0f);
                    hero.anim_speed = 0.35f;
                    hero.move_lock_timer = scale_game_frames(0x14);
                    break;
                case HERO_STATE_VEHICLE_MODE_3_RUN:
                    hero.anim_speed = hero.motion.speed_xy * 54.0f;
                    if (!(hero.air_frames.s < scale_game_frames(5))) {
                        if (hero.ground_distance > 0.8f || hero.ground_slope.f > 0.87266463f) {
                            hero_set_state(0x55, 1);
                            break;
                        }
                    }
                    if (pad_pressed_within(0x80, scale_game_frames(7), 0)) {
                        if (hero.vehicle_toggle_timer == 0) {
                            allocate_hero_voice_for_entry(0x19, 0);
                            hero.vehicle_toggle_timer = 0x12;
                            hero.unk22DC = 1;
                        }
                        hero_switch_item(0);
                        break;
                    }
                    if (controller_state.pressed & 0x20) {
                        hero_set_state(0x59, 1);
                        break;
                    }
                    if (hero.air_frames.s == 0 && hero.stick_magnitude < 0.17f) {
                        hero_set_state(0x53, 1);
                    }
                    break;
                case HERO_STATE_VEHICLE_MODE_3_HURT:
                    if (!(hero.state_timer < scale_game_frames(0x20)) && hero.air_frames.s != 0 &&
                        hero.ground_distance > 1.0f) {
                        hero_set_state(0x55, 1);
                        break;
                    }
                    if (hero.anim_flags & 2) {
                        hero_set_state(0x53, 1);
                        break;
                    }
                    if (scale_game_frames(0x2D) < hero.state_timer) {
                        if (hero.health.hp <= 0) {
                            hero_set_state(0x57, 1);
                            break;
                        }
                        hero_set_state(0x53, 0);
                    }
                    break;
                case HERO_STATE_LOOK_RELEASE_WAIT:
                    if (hero.vehicle_mode == 0) {
                        if (hero.unk20AA == 0 || hero.unk20A5 != 0) {
                            if (controller_state.held & 0xA) {
                                if (((unsigned char *)hero.moby)[0x53] != 0xD) {
                                    set_hero_animation(0xD, 0, scale_game_frames(8));
                                }
                            } else {
                                int a = select_hero_help_value(1);
                                if (((unsigned char *)hero.moby)[0x53] != a) {
                                    set_hero_animation(a, 0, scale_game_frames(8));
                                }
                            }
                        }
                    }
                    if (controller_state.held & 5) {
                        break;
                    }
                    if (hero.state.current == 0x1E) {
                        int a;

                        hero_set_state(HERO_STATE_IDLE, 0);
                        a = select_hero_help_value(1);
                        if (((unsigned char *)hero.moby)[0x53] != a) {
                            set_hero_animation(a, 0, scale_game_frames(8));
                        }
                    }
                    D_L17_0016D080 = 0;
                    break;
                case HERO_STATE_HELD_ITEM_ATTACK:
                    if (((unsigned char *)hero.items[0].moby) != 0 && hero.anim_changing == 0 &&
                        hero.anim_frame > 20.0f) {
                        hero_set_state(HERO_STATE_IDLE, 0);
                    }
                    break;
                case HERO_STATE_ANIMATION_RECOVERY_WITH_JUMP_CANCEL:
                    if (19.0f < hero.anim_frame &&
                        pad_pressed_within(0x40, scale_game_frames(0xB), 0)) {
                        if (controller_state.held & 0xA) {
                            hero_handle_jump_press();
                            break;
                        }
                        hero_set_state(HERO_STATE_JUMP, 1);
                        break;
                    }
                    if (21.0f <= hero.anim_frame) {
                        hero_set_state(HERO_STATE_IDLE, 0);
                    }
                    break;
                }
            }
        }
    }
    if (prevSub != hero.state.control_mode) {
        release_and_clear_hero_voice_slots();
    }
    if ((hero.state.prev == 0x40 && hero.state.current != HERO_STATE_FIDGET_ALT) ||
        (hero.state.prev == 0x41 && hero.unk1614 != 0)) {
        remove_hero_cached_moby_and_clear_slot_three_moby_flags();
    }
    if (prevState != hero.state.step) {
        hero.step_frames = 0;
    }
    if (prevAnim != moby[0x53]) {
        hero.anim_seq_frames = 0;
    }
}
