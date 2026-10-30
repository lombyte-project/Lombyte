/* Level 05 hero state setter: hero_set_state, which leaves the current hero state and enters the new one. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/hero/hero_handle_jump_press.h"
#include "rnc/overlay/shared/hero/hero_idle_sequence.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/hero/hero_frame_update.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/overlay/shared/hero/process_hero_primary_item_input.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/hero_tables.h"

/* Ported from rac1-decomp (src/overlays/l05_rilgar/help_0024D680.c: func_L05_0024D680), where it is exact; names translated to the US level program. */

extern char           D_L05_0017C380_e[] __asm__("D_L05_0017C380");
extern HeroTableRow2C D_L05_0017C088[];
extern char          *moby_pool_4f60 __asm__("D_L05_0015FFD8");
extern char          *hero_slot_moby(int) __asm__("FUN_L00_0020d460");
extern float          level05_hero_animation_blend_curves[][25] __asm__("D_L05_0017C250");
extern float          get_hero_yaw_history(int back) __asm__("FUN_L00_0020d658");
extern float          gravity_length_xy(float *v) __asm__("FUN_L00_002339d0");
extern float          vector_length_xy(void *);
extern int            D_L05_001612D8;
extern int            D_L05_0015F5C4;
extern int            hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");
extern int            is_wrench_moby(char *) __asm__("FUN_L00_00211360");
extern int            stick_direction_vs_facing(void) __asm__("FUN_L00_00221df8");
extern int queue_animation_update(int, int, int, int, int, int, int) __asm__("FUN_001ff308");
extern s32 D_L05_0015F688 __attribute__((sda));
int        D_L05_0015F6E8[4] __attribute__((section(".sdata"))) = {8, 0, 11, 16};

struct HeroVelocityKey D_L05_0015F6F8[1] __attribute__((section(".sdata"))) = {
    {48.0f, 0.0f, 1000}
};

extern unsigned char l05_run_jump_animation_by_run_frame[] __asm__("D_L05_0017C238");
extern void         *D_L05_001612D0;
extern void          hero_change_health(int n) __asm__("FUN_L00_002050c0");
extern void          clear_hero_idle_rotation_targets(void) __asm__("FUN_L00_0020a500");
extern void          exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");
extern void create_missing_hero_item_slot_animations(int a, int b) __asm__("FUN_L00_0020e360");
extern void scale_hero_motion(float) __asm__("FUN_L00_00213de8");
extern void update_hero_target_heading(float a, float b, float c) __asm__("FUN_L00_00214ed8");
extern void spawn_hero_radial_particle_burst(void) __asm__("FUN_L00_00217450");
extern void hero_queue_sequence(int bank, int seq) __asm__("FUN_L00_002325e0");
extern void gravity_flatten(float *dst, float *src) __asm__("FUN_L00_002338d0");
extern void FUN_L00_0025f730(void *a, float x);
extern void create_moby_transform_history_entry(char *o, int a, int b) __asm__("FUN_L00_00262528");
extern void clear_moby_transform_history(char *o) __asm__("FUN_L00_00262840");
extern void pool_splash(void *, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern void initialize_trick_combo_hud(void) __asm__("FUN_L05_00262ae8");
extern void update_trick_combo_hud(void) __asm__("FUN_L05_00262b58");
extern void draw_trick_combo_hud(void) __asm__("FUN_L05_00262f50");
extern void hud_release_held_element(void) __asm__("FUN_L05_00264298");
extern void initialize_hud_element_defaults(void) __asm__("FUN_L05_002661a0");
extern void FUN_L05_002661e8(void);
extern void draw_hud_progress_and_rank(void) __asm__("FUN_L05_00266320");
extern void fill_transfer_words(void *, int, int) __asm__("func_001F97E8");
extern void draw_hud_timer(void) __asm__("FUN_L05_00266710");

/* Sets hero.state.current to a: moves state, control_mode and state_timer into the prev_* fields, zeroes state_step, then sets up the new state; returns 0 when the change is refused. */
extern int  hero_set_state(int, int) __asm__("FUN_L05_0024cee8");
extern char level05_hero_water_surface_jump_velocity_keys[] __asm__("D_L05_0017C3C0");

int hero_set_state(int a, int b) {
    int   old2088;
    char *moby;
    int   old20A7;
    {
        struct Hero *p = &hero;
        if (p->state.control_mode == 0x14 && (a != 0x64 || b != 2))
            return 0;
    }
    if ((D_L05_0015F5C4 == 2 || D_L05_0015F5C4 == 6) && b != 2)
        return 0;
    {
        struct Hero *p = &hero;
        if (p->health.hp == 0 && (p->state.control_mode == 2 || p->state.control_mode == 7) &&
            a != 6 && a != 0x3D && a != 0x47 && a != 0x57 && a != 0x62 && a != 0x77 && a != 0x7B &&
            a != 0x7C && a != 0x7F && a != 0x6A && a != 0x80 && a != 0x82)
            return 0;
    }
    {
        struct Hero *p = &hero;
        old2088 = p->state.step;
        moby = ((char *)p->moby);
        old20A7 = p->item_select_blocked;
        p->state.prev2 = p->state.prev;
        p->state.prev2_control_mode = p->state.prev_control_mode;
        p->state.prev_timer = p->state_timer;
        p->state.prev = p->state.current;
        p->state.prev_control_mode = p->state.control_mode;
        p->state.current = a;
        p->state.step = 0;
        p->unk20A5 = 0;
        p->item_use_locked = 0;
        if (p->vehicle_mode == 0) {
            MOBY(moby)->collision_disable = 0;
            p->tilt_disabled = 0;
            p->item_select_blocked = 0;
            p->unk20B2 = 0;
            p->unk20AF = 0;
            p->glow_pulse_level = 0;
            clear_moby_transform_history(&p->trail);
        }
        if (p->vehicle_mode == 3) {
            int s = p->state.current;
            if (!(s >= 0x53 && s <= 0x59) && !(s >= 0x65 && s <= 0x67) && s != 0 && s != 1) {
                exit_hero_vehicle_mode_if_mode_three();
            }
        }
    }
    {
        struct Hero *p = &hero;
        create_missing_hero_item_slot_animations(p->state.prev, p->state.current);
        p->rand_timer.range = 0;
    }
    switch (a) {
    case HERO_STATE_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0;
        p->unk2284 = 0;
        p->unk1010 = random_int_between(scale_game_frames(0x32), scale_game_frames(0x64));
        clear_hero_idle_rotation_targets();
        if (p->vehicle_mode != 0) {
            request_hero_idle_state_for_vehicle_mode();
            return 1;
        }
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (p->on_slip_ground != 0 && p->frames_off_ground.s == 0) {
            hero_set_state(HERO_STATE_SLIP, 1);
            return 0;
        }
        {
            struct Hero *q = &hero;
            q->rand_timer.range = 0x68;
        }
        if (b) {
            if (get_hero_idle_animation_index(0) == 0x54) {
                set_hero_animation(
                    get_hero_idle_animation_index(0), 0, (float)scale_game_frames(0x12));
            } else {
                set_hero_animation(get_hero_idle_animation_index(0), 0, -1.0f);
            }
        }
        break;
    }
    case HERO_STATE_AT_TARGET_POSE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0;
        p->unk2284 = 0;
        clear_u64_value(&p->motion.momentum);
        p->rand_timer.range = 0x68;
        if (b) {
            if (get_hero_idle_animation_index(0) == 0x54) {
                set_hero_animation(
                    get_hero_idle_animation_index(0), 0, (float)scale_game_frames(0x12));
            } else {
                set_hero_animation(get_hero_idle_animation_index(0), 0, -1.0f);
            }
        }
        break;
    }
    case HERO_STATE_FIDGET_ALT: {
        int t = scale_game_frames(9);
        set_hero_animation(get_hero_idle_animation_index(0), 0, (float)t);
        {
            struct Hero *p = &hero;
            if (*(void **)&p->unk1614 != 0) {
                blend_moby_animation(*(void **)&p->unk1614, 0, 0, t);
            }
        }
        break;
    }
    case HERO_STATE_DYING: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x14;
        p->unk2284 = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (b)
            set_hero_animation(0x45, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_DYING_SPEED_LIMITED: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x14;
        p->unk2284 = 0;
        p->health.hp = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        FUN_L00_0025f730(&p->motion.momentum, frame_time * 3.5f);
        if (b)
            set_hero_animation(0x7C, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_SINK: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x14;
        p->unk2284 = 0;
        p->health.hp = 0;
        p->unk694 = p->motion.displacement.f[2] * 0.2f;
        p->unk69C = p->water_surface_z - 0.37f;
        if (p->unk694 < frame_time * -7.0f)
            p->unk694 = frame_time * -7.0f;
        p->item_select_blocked = 1;
        p->unk6A0 = 0;
        p->unk690 = p->motion.pos.f[2] - p->unk69C;
        if (b)
            set_hero_animation(0x74, 0, (float)scale_game_frames(0xB));
        break;
    }
    case HERO_STATE_SWIM: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x11;
        p->unk2284 = 0xB;
        p->item_select_blocked = 1;
        *(int *)&p->drown_sink_speed = 0;
        if (p->state.prev_control_mode == 0x12) {
            allocate_hero_voice_for_entry(3, 0);
            p->state.step = 1;
        } else {
            p->unk8F4 = scale_game_frames(0x2D);
        }
        {
            struct Hero *q = &hero;
            int          n;
            q->rand_timer.range = 0x68;
            q->unk8E0 = 0;
            n = scale_game_frames(0xC);
            if (b)
                set_hero_animation(0x3B, 0, (float)n);
        }
        break;
    }
    case HERO_STATE_SWIM_WITH_CROUCH_INPUT: {
        struct Hero *p = &hero;
        int          n;
        p->state.control_mode = 0x11;
        p->unk2284 = 0xB;
        p->item_select_blocked = 1;
        p->unk8F4 = scale_game_frames(0x3C);
        if (p->state.prev_control_mode == 0x12) {
            allocate_hero_voice_for_entry(3, 0);
            p->state.step = 1;
        }
        p->unk8E0 = 0;
        p->rand_timer.range = 0x68;
        n = scale_game_frames(0xB);
        if (p->state.prev == 0x33)
            n = scale_game_frames(0xF);
        if (b)
            set_hero_animation(0x3D, 0, (float)n);
        break;
    }
    case HERO_STATE_SWIM_IDLE: {
        struct Hero *p = &hero;
        int          n;
        int          t;
        p->state.control_mode = 0x11;
        p->unk2284 = 0xB;
        p->item_select_blocked = 1;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        p->rand_timer.range = 0x68;
        n = scale_game_frames(0x23);
        t = -2;
        if (p->state.prev != 0x35)
            t = n;
        if (b)
            set_hero_animation(0x3C, 0, (float)t);
        break;
    }
    case HERO_STATE_WATER_SURFACE_ENTRY: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x12;
        p->item_select_blocked = 1;
        p->unk2284 = 0;
        p->splash_effect_timer = 0;
        if (p->state.prev_control_mode == 0x11) {
            emit_hero_surface_effect_burst(3, 0xA, 0);
            if (D_L05_001612D0 != 0) {
                pool_splash(D_L05_001612D0,
                            D_L05_001612D8,
                            0,
                            p->motion.pos.f[0],
                            p->motion.pos.f[1],
                            0.4f,
                            0.3f);
            }
        } else if (p->motion.displacement.f[2] < frame_time * -0.5f) {
            int n = truncate_float_to_s32(absolute_float(p->motion.displacement.f[2]) * 300.0f);
            int v = n;
            if (n > 0x28)
                v = 0x28;
            emit_hero_surface_effect_burst(3, v, 1);
            if (D_L05_001612D0 != 0) {
                pool_splash(D_L05_001612D0,
                            D_L05_001612D8,
                            0,
                            p->motion.pos.f[0],
                            p->motion.pos.f[1],
                            0.5f,
                            -0.4f);
            }
            p->splash_effect_timer = scale_game_frames(0x4B);
        }
        {
            struct Hero *q = &hero;
            q->unk698 = 0;
            if (q->state.prev_control_mode == 0x11) {
                float d = (float)q->underwater_air;
                if (d < 2500.0f) {
                    help_queue_add(8, scale_game_frames(0x1E), 0);
                } else if (d < 7500.0f) {
                    help_queue_add(7, scale_game_frames(0x1B), 0);
                }
                {
                    struct Hero *r = &hero;
                    if (r->state.prev_control_mode == 0x11) {
                        float e = (float)r->underwater_air;
                        if (e < 2500.0f) {
                            help_queue_add(8, scale_game_frames(0x1E), 0);
                        } else if (e < 7500.0f) {
                            help_queue_add(7, scale_game_frames(0x28), 0);
                        }
                    }
                }
            }
        }
        {
            struct Hero *s = &hero;
            s->unk694 = s->motion.displacement.f[2] * 0.37f;
            if (s->unk694 < frame_time * -7.0f)
                s->unk694 = frame_time * -7.0f;
            s->unk69C = s->water_surface_z;
            s->unk6A0 = 0;
            s->unk690 = s->motion.pos.f[2] - s->unk69C - -0.12f;
            qcopy(&s->motion.momentum, &s->motion.resolved_velocity);
            s->motion.momentum.f[2] = 0.0f;
            FUN_L00_0025f730(&s->motion.momentum, frame_time * 5.5f);
            s->rand_timer.range = 0x68;
            if (b)
                set_hero_animation(0x3A, 0, (float)scale_game_frames(0x16));
        }
        break;
    }
    case HERO_STATE_WATER_SURFACE_HURT:
    case HERO_STATE_HURT_WITH_DISPLACEMENT_VELOCITY: {
        struct Hero *p = &hero;
        char        *r;
        p->state.control_mode = 7;
        p->unk2284 = 0;
        scale_hero_motion(0.5f);
        p->item_select_blocked = 1;
        qcopy(&p->motion.velocity, &p->motion.displacement);
        r = find_moby_hit(((void *)p->moby), 1, 0);
        if (r != 0) {
            hero_change_health(truncate_float_to_s32(*(float *)(r + 0x2C)));
        } else {
            hero_change_health(1);
        }
        {
            int          t = scale_game_frames(0x4D);
            struct Hero *q = &hero;
            q->invulnerable_frames = t;
            if (b) {
                if (q->state.current == 0x75) {
                    set_hero_animation(0x70, 3, -3.0f);
                } else {
                    set_hero_animation(0x6F, 3, -3.0f);
                }
                {
                    struct Hero *w = &hero;
                    *(float *)(((char *)w->moby) + 0x54) =
                        level05_hero_animation_blend_curves[w->anim_blend_curve]
                                                           [(w->anim_blend_step)++];
                }
            }
        }
        break;
    }
    case HERO_STATE_WATER_SURFACE_MOVE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x12;
        p->item_select_blocked = 1;
        p->unk2284 = 0;
        *(int *)&p->drown_sink_speed = 0;
        p->unk698 = scale_game_frames(0x2D);
        p->rand_timer.range = 0x68;
        if (b)
            set_hero_animation(0x65, 0, (float)scale_game_frames(0x11));
        break;
    }
    case HERO_STATE_DROWN: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x14;
        p->item_select_blocked = 1;
        p->unk2284 = 0;
        p->health.hp = 0;
        if (p->jump_button_frames > scale_game_frames(0x32)) {
            struct UsageStat *h = usage_stats.stat;
            if (h[12].count <= 0xFFFE) {
                h[12].count = h[12].count + 1;
            }
        }
        {
            struct UsageStat *h = usage_stats.stat;
            struct Hero      *q;
            if (scale_game_frames(D_0015EEA4) / 600 > h[12].unk2) {
                h[12].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            h[12].level_mask = h[12].level_mask | (1 << current_level_index) | 0x80000000;
            q = &hero;
            if (q->swim_cross_tap_count >= 3) {
                if (h[29].count <= 0xFFFE) {
                    h[29].count = h[29].count + 1;
                }
            }
        }
        {
            struct UsageStat *h = usage_stats.stat;
            char             *v;
            if (scale_game_frames(D_0015EEA4) / 600 > h[29].unk2) {
                h[29].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            h[29].level_mask = h[29].level_mask | (1 << current_level_index) | 0x80000000;
            v = (char *)&hero.motion.momentum;
            qcopy(v, v - 0x40);
        }
        if (b)
            set_hero_animation(0x5F, 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_DYING_ZERO_HEALTH: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x14;
        p->item_select_blocked = 1;
        p->unk2284 = 0;
        p->health.hp = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (b)
            set_hero_animation(0x80, 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_HOVERBOARD_RIDE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x16;
        p->unk2284 = 8;
        p->item_select_blocked = 1;
        p->unk20B2 = 1;
        p->board_jump_frames = 0;
        p->hoverboard_airborne = 0;
        if (p->state.prev == 0x6D || p->state.prev == 0x6F || p->state.prev == 0x6E) {
            struct Hero *r = &hero;
            if (((void *)r->race_path) != 0) {
                int n =
                    select_point_index_by_distance(&r->motion.pos, ((void *)r->race_path), 0.0f);
                if (n < r->race_path_node)
                    r->race_path_node = n;
            }
            {
                struct Hero *s = &hero;
                s->hoverboard_airborne = 0;
                s->board_jump_frames = 0;
                s->boost_pad_timer = 0;
                s->unk8B8.i = 0;
                s->race_ammo = 0;
                s->off_course_reset_pending = 0;
            }
        }
        {
            struct Hero *q = &hero;
            if (q->state.prev_control_mode != 0x16) {
                char *m;
                if (current_level_index == 5) {
                    q->motion.pos.f[0] = 247.0f;
                    q->motion.pos.f[1] = 287.0f;
                    q->motion.pos.f[2] = 76.0f;
                    q->motion.rot.f[2] = -2.1f;
                }
                m = (char *)q->hoverboard_moby;
                fill_transfer_words(&q->smoothed_spin_stick, 0, 0x210);
                q->hoverboard_moby = (u8 *)m;
                q->unk890 = (u8 *)moby_pool_4f60 + (*(int *)(*(char **)(m + 0x78) + 0x28) << 8);
                q->board_gravity = frame_time_sq * 22.0f;
                q->unk87C = 1.2217305f;
                q->trick_hud_handle = -1;
                if (alternate_item_available[0] != 0) {
                    hud_release_held_element();
                    q->trick_hud_handle =
                        queue_animation_update(0x10,
                                               0xFFFF,
                                               (int)initialize_trick_combo_hud,
                                               (int)update_trick_combo_hud,
                                               (int)draw_trick_combo_hud,
                                               (int)&q->boost_frames,
                                               (int)((float)scale_game_frames(0x11) * 60.0f));
                }
                q->board_air_yaw = q->motion.rot.f[2];
                queue_animation_update(5,
                                       0xFFFF,
                                       (int)initialize_hud_element_defaults,
                                       (int)FUN_L05_002661e8,
                                       (int)draw_hud_progress_and_rank,
                                       0,
                                       0);
                queue_animation_update(7,
                                       0xFFFF,
                                       (int)initialize_hud_element_defaults,
                                       (int)FUN_L05_002661e8,
                                       (int)draw_hud_timer,
                                       0,
                                       0);
            } else {
                q->board_vertical_speed = -fast_vec_dot(&q->gravity_dir, &q->motion.displacement);
            }
        }
        if (b) {
            struct Hero *t = &hero;
            if (t->race_ammo != 0) {
                set_hero_animation(0x7D, 0, (float)scale_game_frames(5));
            } else {
                set_hero_animation(0x52, 0, (float)scale_game_frames(5));
            }
        }
        break;
    }
    case HERO_STATE_HOVERBOARD_ANIMATION_VARIANT: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x16;
        p->unk2284 = 8;
        p->item_select_blocked = 1;
        p->unk870 = 0;
        if (b)
            set_hero_animation(
                select_hero_hoverboard_animation_variant(), 5, (float)scale_game_frames(8));
        break;
    }
    case HERO_STATE_HOVERBOARD_RESPAWN_HEIGHT_OR_ANIMATION_WAIT: {
        struct Hero *p = &hero;
        p->item_select_blocked = 1;
        p->unk2284 = 0;
        release_and_clear_hero_voice_slots();
        p->boost_pad_timer = 0;
        fast_vec_scale(&p->motion.velocity, &p->motion.resolved_velocity, 0.7f);
        p->motion.velocity.f[2] = frame_time * 10.0f;
        set_hero_animation(0x61, 3, (float)scale_game_frames(4));
        break;
    }
    case HERO_STATE_HOVERBOARD_RESPAWN_POSITION_OR_TIMEOUT_WAIT: {
        struct Hero *p = &hero;
        int          n;
        int          v;
        p->item_select_blocked = 1;
        p->unk2284 = 0;
        release_and_clear_hero_voice_slots();
        p->boost_pad_timer = 0;
        p->board_respawn_delay = scale_game_frames(0x46);
        p->water_surface_z = p->board_floor_z;
        n = truncate_float_to_s32(absolute_float(p->motion.displacement.f[2]) * 300.0f);
        v = n;
        if (n > 0x28)
            v = 0x28;
        emit_hero_surface_effect_burst(3, v, 1);
        allocate_hero_voice_for_entry(3, 0);
        p->splash_effect_timer = scale_game_frames(0x4B);
        p->unk694 = p->motion.displacement.f[2] * 0.3f;
        if (p->unk694 < frame_time * -5.7f)
            p->unk694 = frame_time * -5.7f;
        p->unk69C = p->board_floor_z;
        p->unk6A0 = 0;
        p->unk690 = p->motion.pos.f[2] - p->unk69C - -0.12f;
        if (0.2f < p->unk690) {
            p->unk690 = 0.2f;
        } else if (p->unk690 < -0.2f) {
            p->unk690 = -0.2f;
        }
        {
            char *hero_velocity_bytes = (char *)&hero.motion.velocity;
            char *w;
            clear_u64_value(hero_velocity_bytes);
            w = hero_velocity_bytes + 0x70;
            qcopy(w, hero_velocity_bytes + 0x30);
            gravity_flatten((float *)w, (float *)w);
            FUN_L00_0025f730(w, frame_time * 9.5f);
            *(int *)(hero_velocity_bytes + 0xF18) = 0x68;
            if (b)
                set_hero_animation(0x3A, 0, (float)scale_game_frames(0x11));
        }
        break;
    }
    case HERO_STATE_HOVERBOARD_RESPAWN_ANIMATION_WAIT: {
        struct Hero *p = &hero;
        p->item_select_blocked = 1;
        p->unk2284 = 0;
        release_and_clear_hero_voice_slots();
        p->boost_pad_timer = 0;
        p->rand_timer.range = 0x68;
        if (b)
            set_hero_animation(0x62, 4, (float)scale_game_frames(4));
        break;
    }
    case 0x38: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x13;
        p->unk2284 = 0;
        if (b)
            set_hero_animation(0x46, 0, (float)scale_game_frames(8));
        break;
    }
    case 0x39: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x13;
        p->unk2284 = 0;
        if (b)
            set_hero_animation(0x47, 0, (float)scale_game_frames(8));
        break;
    }
    case HERO_STATE_ANIMATION_WAIT_TO_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x13;
        p->unk2284 = 0;
        if (b)
            set_hero_animation(0x48, 0, (float)scale_game_frames(8));
        break;
    }
    case HERO_STATE_LOOK: {
        struct Hero *p = &hero;
        int          t;
        p->state.control_mode = 0;
        p->unk2284 = 4;
        *(int *)&D_L05_0015F688 = scale_game_frames(0x3C);
        if (p->unk20AA == 0) {
            t = get_hero_idle_animation_index(1);
            if (*(unsigned char *)(((char *)p->moby) + 0x53) != t && b) {
                set_hero_animation(t, 0, (float)scale_game_frames(0x12));
            }
        }
        break;
    }
    case HERO_STATE_ITEM_BUTTON_RELEASE_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = 8;
        p->unk2284 = 0;
        qcopy(&p->motion.momentum, &p->motion.displacement);
        set_hero_animation(get_hero_idle_animation_index(0), 0, (float)scale_game_frames(0xA));
        p->unk1D0 = scale_game_frames(7);
        break;
    }
    case HERO_STATE_WAIT_FOR_ITEM_RELEASE_THEN_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = 8;
        p->unk2284 = 0;
        qcopy(&p->motion.momentum, &p->motion.displacement);
        set_hero_animation(get_hero_idle_animation_index(0), 0, (float)scale_game_frames(5));
        p->unk1D0 = scale_game_frames(7);
        break;
    }
    case HERO_STATE_ITEM_BUTTON_HOLD_ANIMATION_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = 1;
        p->unk2284 = 0;
        p->ground_speed = vector_length_xy(&p->motion.displacement);
        p->state.step = 0;
        p->unk3B8 = 0;
        p->run_start_blending = 0;
        set_hero_animation(0x2E, 0, (float)scale_game_frames(5));
        break;
    }
    case HERO_STATE_RUN:
    case HERO_STATE_DIRECTION_LIMITED_GROUND_MOVE:
    case HERO_STATE_SHALLOW_WATER_RUN: {
        struct Hero *p = &hero;
        p->state.control_mode = 1;
        p->unk2284 = 0;
        if (p->state.prev_control_mode != 4 && p->state.prev2_control_mode != 4) {
            p->ground_speed = vector_length_xy(&p->motion.resolved_velocity);
        }
        {
            struct Hero *q = &hero;
            if (frame_time * 7.0f < q->ground_speed)
                q->ground_speed = frame_time * 7.0f;
            q->unk3BE = 0;
            stick_target(1.0f, 0);
            if (1.5707964f <
                fast_difference_between_rotations(q->motion.rot.f[2], q->motion.move_yaw)) {
                if (q->state.prev_control_mode == 7 || q->state.prev == 3 || q->state.prev2 == 3) {
                    struct Hero *r = &hero;
                    r->ground_speed = 0.0f;
                }
            }
        }
        {
            struct Hero *s = &hero;
            s->rand_timer.range = 0x68;
            if (s->on_slip_ground != 0 && s->frames_off_ground.s == 0) {
                hero_set_state(HERO_STATE_SLIP, 1);
                return 0;
            }
        }
        {
            struct Hero *t = &hero;
            if (t->wading != 0 || t->state.current == 0x73) {
                t->state.current = 0x73;
                set_hero_animation(0x60, 0, (float)scale_game_frames(8));
            } else {
                int n;
                t->state.step = 0;
                t->unk3B8 = 0;
                t->run_start_blending = 0;
                n = scale_game_frames(8);
                if (t->health.hp == 1)
                    n = scale_game_frames(0xC);
                if (t->state.prev2 == 0x23)
                    n = scale_game_frames(0xF);
                if (t->state.prev2 == 6 || t->state.prev2 == 0x2D)
                    n = scale_game_frames(0x12);
                {
                    struct Hero *u = &hero;
                    if (u->state.prev2_control_mode == 4 &&
                        scale_game_frames(5) < u->rail_land_frames)
                        n = scale_game_frames(0xD);
                }
                if (b) {
                    struct Hero *v = &hero;
                    set_hero_animation(v->state.step + 3, D_L05_0015F6E8[v->state.step], (float)n);
                }
            }
        }
        break;
    }
    case HERO_STATE_APPROACH_TARGET_POSE: {
        struct Hero *p = &hero;
        p->state.control_mode = 1;
        p->unk2284 = 0;
        p->ground_speed = vector_length_xy(&p->motion.resolved_velocity);
        p->item_use_locked = 1;
        if (p->vehicle_mode == 0)
            cancel_hero_item_animation();
        if (b) {
            if (p->vehicle_mode == 3) {
                set_hero_animation(1, 0, (float)scale_game_frames(8));
            } else if (p->vehicle_mode == 1) {
                set_hero_animation(2, 0, (float)scale_game_frames(8));
            } else if (p->vehicle_mode == 0) {
                set_hero_animation(
                    p->state.step + 3, D_L05_0015F6E8[p->state.step], (float)scale_game_frames(8));
            }
        }
        break;
    }
    case HERO_STATE_VOICE_SLOT_CLEANUP_IDLE_EXIT: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x10;
        p->unk2284 = 3;
        p->item_select_blocked = 1;
        fill_transfer_words((char *)p + 0x9D0, 0, 0x20);
        qcopy(&p->motion.momentum, &p->motion.displacement);
        if (b)
            set_hero_animation(0x64, 0, (float)scale_game_frames(0xF));
        break;
    }
    case HERO_STATE_STOP: {
        struct Hero *p = &hero;
        p->state.control_mode = 1;
        p->unk2284 = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (p->on_slip_ground != 0 && p->frames_off_ground.s == 0) {
            hero_set_state(HERO_STATE_SLIP, 1);
            return 0;
        }
        if (try_start_hero_item_idle_animation())
            return 0;
        if (get_hero_idle_animation_index(0) == 0x54) {
            if (hero_set_state(HERO_STATE_IDLE, 0)) {
                set_hero_animation(
                    get_hero_idle_animation_index(0), 0, (float)scale_game_frames(0x12));
            }
            return 0;
        }
        if (b) {
            struct Hero *q = &hero;
            float        d = q->anim_frame;
            int          id = 6;
            if (return_hero_animation_id_unchanged(MOBY(moby)->seq) == 4 && 3.0f < d && d < 16.0f)
                id = 5;
            set_hero_animation(id, 4, (float)scale_game_frames(9));
        }
        break;
    }
    case HERO_STATE_BRAKE_AT_TARGET_POSITION: {
        struct Hero *p = &hero;
        char        *v = (char *)&p->motion.resolved_velocity;
        p->state.control_mode = 1;
        p->unk2284 = 0;
        p->ground_speed = gravity_length_xy((float *)v);
        qcopy(&p->motion.momentum, v);
        if (b) {
            int   id = 6;
            float d = p->anim_frame;
            if (return_hero_animation_id_unchanged(MOBY(moby)->seq) == 4 && 3.0f < d && d < 16.0f)
                id = 5;
            set_hero_animation(id, 4, (float)scale_game_frames(9));
        }
        break;
    }
    case HERO_STATE_CROUCH: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xC;
        p->unk2284 = 0;
        p->ground_speed = vector_length_xy(&p->motion.resolved_velocity);
        if (frame_time * 7.0f < p->ground_speed)
            p->ground_speed = frame_time * 7.0f;
        if (p->on_slip_ground != 0 && p->frames_off_ground.s == 0) {
            hero_set_state(HERO_STATE_SLIP, 1);
            return 0;
        }
        {
            struct Hero *q = &hero;
            q->rand_timer.range = 0x68;
            qcopy(&q->motion.momentum, &q->motion.resolved_velocity);
            FUN_L00_0025f730(&q->motion.momentum, frame_time * 7.0f);
            if (frame_time * 3.5f < q->motion.forward_speed)
                allocate_hero_voice_for_entry(0xC, 0);
            if (b) {
                if (*(unsigned char *)(((char *)q->moby) + 0x52) ==
                    get_hero_idle_animation_index(0)) {
                    set_hero_animation(0xD, 2, (float)scale_game_frames(9));
                } else {
                    set_hero_animation(0xD, 2, (float)scale_game_frames(0xE));
                }
                hero_queue_sequence(3, 0x12);
            }
        }
        break;
    }
    case HERO_STATE_GRAPPLE_TARGET_ENTRY: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xD;
        p->unk2284 = 7;
        {
            char *h = ((char *)&item_usage);
            if (*(unsigned short *)(h + 0x60) <= 0xFFFE) {
                *(unsigned short *)(h + 0x60) = *(unsigned short *)(h + 0x60) + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > *(unsigned short *)(h + 0x62)) {
                *(unsigned short *)(h + 0x62) = scale_game_frames(D_0015EEA4) / 600;
            }
            *(unsigned int *)(h + 0x64) =
                *(unsigned int *)(h + 0x64) | (1 << current_level_index) | 0x80000000;
        }
        p->motion.move_yaw =
            fast_arc_tan(*(float *)(((char *)p->grapple_target) + 0x10) - p->motion.pos.f[0],
                         *(float *)(((char *)p->grapple_target) + 0x14) - p->motion.pos.f[1]);
        if (b) {
            set_hero_animation(0x2F, 2, (float)scale_game_frames(5));
            hero_queue_sequence(0xA, 0xE);
        }
        break;
    }
    case HERO_STATE_APPROACH_TARGET_MOBY: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xD;
        p->unk2284 = 7;
        *(int *)&p->grapple_pull_speed = 0;
        p->grapple_pull_ending = 0;
        p->item_use_locked = 1;
        if (b)
            set_hero_animation(0x30, 1, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_CONSTRAIN_TO_TARGET_RADIUS: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xD;
        p->unk2284 = 7;
        p->item_use_locked = 1;
        *(int *)&p->grapple_pull_speed = 0;
        p->grapple_pull_ending = 0;
        p->tether_radius = fast_vec_dist(&p->motion.pos, ((char *)p->grapple_target) + 0x10);
        if (b)
            set_hero_animation(0x30, 1, (float)scale_game_frames(5));
        break;
    }
    case HERO_STATE_GRAPPLE_SWING_ENTRY: {
        struct Hero *p = &hero;
        char        *d;
        p->state.control_mode = 0xE;
        p->unk2284 = 7;
        {
            char *h = ((char *)&item_usage);
            if (*(unsigned short *)(h + 0x60) <= 0xFFFE) {
                *(unsigned short *)(h + 0x60) = *(unsigned short *)(h + 0x60) + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > *(unsigned short *)(h + 0x62)) {
                *(unsigned short *)(h + 0x62) = scale_game_frames(D_0015EEA4) / 600;
            }
            *(unsigned int *)(h + 0x64) =
                *(unsigned int *)(h + 0x64) | (1 << current_level_index) | 0x80000000;
        }
        p->swing_entry_push = 1;
        p->swing_length_vel = 0.0f;
        p->item_use_locked = 1;
        *(char **)&p->swing_target_moby = ((char *)p->swing_target);
        p->unk99C = 0;
        p->swing_anim_dir = 0;
        *(int *)&p->swing_push_speed = 0;
        *(int *)&p->swing_yaw_vel = 0;
        d = *(char **)(((char *)p->swing_target_moby) + 0x78);
        p->unk9A0 = *(float *)(d + 4);
        if (*(float *)(d + 0x10) == p->swing_length_vel) {
            *(float *)(d + 0x14) = *(float *)(d + 0xC);
        } else {
            *(float *)(d + 0x14) = *(float *)(d + 0x10);
        }
        {
            struct Hero *q = &hero;
            q->swing_rope_spring_k = frame_scale_sq * 0.025f;
            q->unk9C8 = frame_scale_sq * 0.33f;
            q->swing_rope_max_speed = frame_time * 11.0f;
            clear_u64_value(&q->motion.rot_spring_vel);
            q->grapple_lockout_timer = scale_game_frames(0x28);
            if (*(float *)(d + 0x24) != 0.0f)
                q->swing_rope_spring_k = *(float *)(d + 0x24);
            if (*(float *)(d + 0x28) != 0.0f)
                q->unk9C8 = *(float *)(d + 0x28);
            if (*(float *)(d + 0x2C) != 0.0f)
                q->swing_rope_max_speed = *(float *)(d + 0x2C);
            q->swing_yaw = fast_arc_tan(
                *(float *)(((char *)q->swing_target_moby) + 0x10) - q->motion.pos.f[0],
                *(float *)(((char *)q->swing_target_moby) + 0x14) - q->motion.pos.f[1]);
            q->unk9A4 = fast_vec_dist(&q->motion.pos, ((char *)q->swing_target_moby) + 0x10);
            q->swing_gravity = frame_time_sq * 27.0f;
            if (q->state.prev_control_mode == 4) {
                q->swing_gravity = q->jump_gravity;
            } else if (q->state.prev_control_mode == 2) {
                q->swing_gravity = q->fall_state_gravity;
            }
            if (b) {
                set_hero_animation(0x34, 0, (float)scale_game_frames(0xB));
                {
                    struct Hero *r = &hero;
                    r->anim_speed = 0.7f;
                }
            }
        }
        break;
    }
    case HERO_STATE_ITEM_RELEASE_FALL: {
        struct Hero *p = &hero;
        p->state.control_mode = 2;
        p->unk2284 = 7;
        clear_u64_value(&p->motion.rot_spring_vel);
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (b)
            set_hero_animation(0xB, 0, (float)scale_game_frames(0xF));
        break;
    }
    case HERO_STATE_RISE_THEN_FALL_WITH_AIRBORNE_HIT_QUERY: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xB;
        p->rise_fall_target_frame = 39.0f;
        p->unk2284 = 0x50;
        *(int *)&p->past_apex = 0;
        p->rise_fall_land_frames = 0;
        p->rise_fall_gravity = frame_time_sq * 100.0f;
        if (b)
            set_hero_animation(0x2A, 0, (float)scale_game_frames(6));
        break;
    }
    case HERO_STATE_LEDGE_HANG_ENTRY: {
        struct Hero *p = &hero;
        p->state.control_mode = 3;
        p->unk2284 = 0xD;
        p->unk4EC = frame_time_sq * 24.0f;
        p->unk4F0 = p->motion.pos.f[2];
        if (p->state.prev_control_mode == 4) {
            p->unk4EC = p->jump_gravity;
            p->unk4F0 = p->smoothed_ground_z;
        }
        p->item_select_blocked = 1;
        if (b) {
            if (frame_time * 3.5f < p->motion.flat_speed) {
                set_hero_animation(0x20, 0, (float)scale_game_frames(9));
            } else {
                set_hero_animation(0x20, 1, (float)(scale_game_frames(9) + 2));
            }
        }
        break;
    }
    case HERO_STATE_LEDGE_HANG: {
        struct Hero *p = &hero;
        p->state.control_mode = 3;
        p->unk2284 = 0xD;
        p->item_select_blocked = 1;
        p->rand_timer.range = 0x68;
        if (b)
            set_hero_animation(0x21, 0, (float)scale_game_frames(6));
        break;
    }
    case HERO_STATE_LEDGE_SHIMMY_POSITIVE_YAW: {
        struct Hero *p = &hero;
        p->state.control_mode = 3;
        p->unk2284 = 0xD;
        p->item_select_blocked = 1;
        if (b)
            set_hero_animation(0x24, 0, (float)scale_game_frames(6));
        break;
    }
    case HERO_STATE_LEDGE_SHIMMY_NEGATIVE_YAW: {
        struct Hero *p = &hero;
        p->state.control_mode = 3;
        p->unk2284 = 0xD;
        p->item_select_blocked = 1;
        if (b)
            set_hero_animation(0x25, 0, (float)scale_game_frames(6));
        break;
    }
    case HERO_STATE_SLIP: {
        struct Hero *p = &hero;
        p->state.control_mode = 2;
        p->unk2284 = 0;
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (b)
            set_hero_animation(0xB, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_FALL:
    case HERO_STATE_FATAL_FALL: {
        struct Hero *p = &hero;
        p->state.control_mode = 2;
        p->unk2284 = 0;
        p->fall_state_gravity = frame_time_sq * 24.0f;
        p->stickless_decel = frame_time_sq * 7.0f;
        if (p->state.current == 0x77) {
            if (p->vehicle_mode == 0) {
                p->health.unk22B2 = allocate_hero_voice_for_entry(0x17, 0x20);
            }
            p->pivot_rot_x_target = randf_sym(frame_time * 3.1415927f, frame_time * 4.3633232f);
            p->fatal_fall_tumble_target_y =
                randf_sym(frame_time * 3.1415927f, frame_time * 4.3633232f);
        }
        {
            struct Hero *q = &hero;
            q->rand_timer.range = 0x68;
            q->state.step = 0;
            qcopy(&q->motion.velocity, &q->motion.resolved_velocity);
            if (b) {
                if (q->vehicle_mode == 3) {
                    set_hero_animation(6, 0, (float)scale_game_frames(8));
                    return 1;
                }
                if (1.75f < q->ground_distance) {
                    set_hero_animation(0xB, 0, (float)scale_game_frames(0xC));
                    q->state.step = 1;
                } else {
                    set_hero_animation(0xA, 0, (float)scale_game_frames(0xA));
                }
            }
        }
        break;
    }
    case HERO_STATE_DELAYED_JUMP_OR_R1_REPEAT_EXIT_WAIT: {
        struct Hero *p = &hero;
        int          animation_timing_parameter = -2;
        p->state.control_mode = 1;
        p->unk22CA = 1;
        p->rand_timer.range = 0x68;
        p->unk2284 = 0;
        if (frame_time * 4.5f < p->motion.speed_xy) {
            scale_hero_motion(frame_time * 4.5f / p->motion.speed_xy);
        }
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (b) {
            set_hero_animation(0x13, 0, (float)animation_timing_parameter);
        }
        break;
    }
    case HERO_STATE_LAND: {
        struct Hero *p = &hero;
        int          animation_timing_setting = -2;
        p->state.control_mode = 5;
        p->rand_timer.range = 0x68;
        p->unk2284 = 0;
        if (frame_time * 4.5f < p->motion.speed_xy) {
            scale_hero_motion(frame_time * 4.5f / p->motion.speed_xy);
        }
        qcopy(&p->motion.velocity, &p->motion.displacement);
        if (b) {
            set_hero_animation(0x13, 0, (float)animation_timing_setting);
        }
        break;
    }
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
    case HERO_STATE_GRIND_JUMP:
    case HERO_STATE_GRIND_RAIL_TRANSFER:
    case HERO_STATE_JUMP_WITH_HEALTH_DAMAGE:
    case HERO_STATE_VEHICLE_MODE_1_JUMP:
    case HERO_STATE_ALT_LEDGE_JUMP:
    case HERO_STATE_VEHICLE_MODE_2_JUMP:
    case HERO_STATE_JUMP_FROM_RESPAWN_WAIT: {
        struct Hero *p = &hero;
        if (p->jump_lock_timer != 0) {
            p->state.current = p->state.prev;
            p->state.step = old2088;
            p->state_timer = p->state.prev_timer;
            return 0;
        }
        p->state.control_mode = 4;
        if (a >= 0x29 && a <= 0x2A) {
            p->unk2284 = 3;
        } else {
            p->unk2284 = 0x50;
        }
        {
            char *v = (char *)&hero.motion.velocity;
            char *r;
            qcopy(v, v + 0x30);
            r = v - 0xE0;
            if (*(int *)(r + 0x2084) != 0x12) {
                if (*(unsigned char *)(r + 0x12E4) != 0 &&
                    *(float *)(r + 0x88) < *(float *)(r + 0x2F0) + 0.5f) {
                    emit_hero_surface_effect_burst(3, 0x10, 0);
                }
            }
        }
        {
            struct Hero *u = &hero;
            if (u->state.current == 0x69)
                allocate_hero_voice_for_entry(0xA, 0);
            u->smoothed_ground_z = u->motion.pos.f[2];
            u->jump_gravity = frame_time_sq * 29.7f;
            u->unk3E8 = -1;
            u->air_speed_step_up = frame_time_sq * 20.0f;
            u->jump_decel = frame_time_sq * 11.0f;
            u->unk4A8 = u->on_ice_ground;
            u->past_jump_apex = 0;
            u->rail_land_frames = 0;
            u->unk428 = 0.0f;
            u->unk42C = 0.0f;
            u->velocity_keys_active = 0;
            u->fall_start_distance = 2.5f;
            u->jump_blocked_ahead = 0;
            u->unk4AC = 0;
            u->can_chain_high_jump = 0;
            u->can_double_jump = 0;
            u->jump_max_frames = scale_game_frames(0x50);
            u->fall_delay_frames = scale_game_frames(0x46);
            u->unk4A6 = 0;
            u->jump_anim_retime_scale = 0.5f;
            u->air_stick_speed = frame_time * 5.7f;
            u->air_turn_rate = frame_time * 15.0098314f;
            u->fall_gravity = 0.0f;
            u->unk22B4 = 0;
            if (u->state.current == 7) {
                u->unk430 = 1.47f;
                u->takeoff_frame = scale_game_frames(5);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 30.0f;
                u->jump_land_anim_frame = 12.0f;
                u->unk48C = 2.62f;
                u->jump_height_start = 1.47f;
                u->unk498 = scale_game_frames(0xF);
                u->unk4AC = 1;
                u->can_chain_high_jump = 1;
                u->can_double_jump = 1;
                u->fall_gravity = u->jump_gravity * 1.17f;
            } else if (u->state.current == 0x12) {
                u->unk430 = 0.1f;
                u->takeoff_frame = scale_game_frames(2);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 28.0f;
                u->jump_land_anim_frame = 14.0f;
                u->unk48C = 0.2f;
                u->jump_height_start = 0.1f;
                u->unk498 = scale_game_frames(0xA);
                if (u->water_depth < 1.25f) {
                    u->takeoff_frame = scale_game_frames(0xF);
                    u->jump_height_start = 1.95f;
                    u->unk48C = 1.98f;
                    u->unk430 = 1.95f;
                } else {
                    gravity_set_z(
                        (float *)(&u->motion.velocity), (float *)(&u->motion.velocity), 0.0f);
                    u->velocity_keys_active = 1;
                    u->unk3D0 = scale_game_frames(2);
                    u->velocity_keys_end_frame = scale_game_frames(0x23) + u->unk3D0;
                    u->keyed_move_target_frame = 0.0f;
                    u->unk3DC = 18.0f;
                    u->keyed_anim_target_frame = scale_game_frames(0x28);
                    u->unk3E4 = level05_hero_water_surface_jump_velocity_keys;
                }
                {
                    struct Hero *w = &hero;
                    w->item_select_blocked = 1;
                }
            } else if (u->state.current == 0x3C) {
                u->unk430 = 5.5f;
                u->takeoff_frame = scale_game_frames(5);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 26.0f;
                u->jump_land_anim_frame = 13.0f;
                u->unk48C = 5.55f;
                u->jump_height_start = 5.5f;
                u->unk498 = scale_game_frames(2);
                u->jump_max_frames = scale_game_frames(0x78);
                hero_change_health(1);
                if (current_level_index == 0xA) {
                    hero_set_state(0x7C, 1);
                    return 0;
                }
                if (current_level_index != 6) {
                    if (u->state.prev == 0x3C || u->state.prev2 == 0x3C || u->health.hp == 0) {
                        struct Hero *w = &hero;
                        w->sink_jump_count = w->sink_jump_count + 1;
                        if (w->sink_jump_count >= 2 || w->health.hp == 0) {
                            hero_set_state(0x7C, 1);
                            return 0;
                        }
                    } else {
                        u->sink_jump_count = 0;
                    }
                }
                {
                    struct Hero *w = &hero;
                    w->fall_delay_frames = scale_game_frames(0x96);
                    w->jump_gravity = frame_time_sq * 15.0f;
                    w->unk49C = scale_game_frames(5);
                    w->unk49E = scale_game_frames(4);
                }
            } else if (u->state.current == 0x69) {
                float v;
                u->state.control_mode = 0x19;
                if (u->motion.pos.f[2] < u->sink_surface_z) {
                    v = u->sink_surface_z - u->motion.pos.f[2];
                } else {
                    v = u->fall_gravity;
                }
                {
                    struct Hero *w = &hero;
                    w->unk430 = v + 1.7f;
                    w->takeoff_frame = scale_game_frames(5);
                    w->unk434 = 0.6f;
                    w->apex_anim_frame = 18.0f;
                    w->target_anim_frame = 30.0f;
                    w->jump_land_anim_frame = 12.0f;
                    w->unk48C = v + 1.75f;
                    w->jump_height_start = v + 1.7f;
                    w->unk498 = scale_game_frames(0x13);
                }
            } else if (u->state.current == 9) {
                u->unk430 = 1.0f;
                u->takeoff_frame = scale_game_frames(5);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 29.0f;
                u->jump_land_anim_frame = 11.0f;
                u->unk48C = 2.7f;
                u->jump_height_start = 1.0f;
                u->unk498 = scale_game_frames(0xF);
                u->air_stick_speed = frame_time * 5.7f;
                u->unk4AC = 1;
                u->can_chain_high_jump = 1;
                u->can_double_jump = 1;
            } else if (u->state.current == 0xA) {
                u->unk430 = 1.9f;
                u->takeoff_frame = scale_game_frames(5);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 29.0f;
                u->jump_land_anim_frame = 10.0f;
                u->unk48C = 1.95f;
                u->jump_height_start = 1.9f;
                u->unk498 = scale_game_frames(0xE);
                u->can_chain_high_jump = 1;
                u->jump_gravity = frame_time_sq * 11.0f;
                u->jump_max_frames = scale_game_frames(0x78);
                u->fall_delay_frames = scale_game_frames(0x5A);
                u->unk4A6 = scale_game_frames(0x46);
            } else if (u->state.current == 0x10) {
                char *x = &u->trail.pos[0];
                u->unk430 = 1.5f;
                u->takeoff_frame = scale_game_frames(5);
                u->unk434 = 0.8f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 30.0f;
                u->jump_land_anim_frame = 11.0f;
                u->unk48C = 1.55f;
                u->jump_height_start = 1.5f;
                u->unk498 = scale_game_frames(1);
                u->unk430 = 0.4f;
                u->jump_height_start = 0.4f;
                u->unk48C = 0.9f;
                initialize_moby_transform_history(((int)u->moby), x);
                create_moby_transform_history_entry(x, 0x28, 2);
                create_moby_transform_history_entry(x, 0x14, 4);
                create_moby_transform_history_entry(x, 0xA, 6);
                u->can_chain_high_jump = 1;
                u->long_jump_speed = frame_time * 11.5f;
                if (1.4f < u->ground_distance)
                    u->long_jump_speed = frame_time * 5.7f;
                u->jump_gravity = frame_time_sq * 8.5f;
                u->air_speed_step_up = frame_time_sq * 44.0f;
                u->jump_decel = frame_time_sq * 50.0f;
            } else if (u->state.current == 0x1C) {
                u->unk430 = 2.0f;
                u->takeoff_frame = scale_game_frames(0xA);
                u->unk434 = 0.85f;
                u->apex_anim_frame = 20.0f;
                u->target_anim_frame = 29.0f;
                u->jump_land_anim_frame = 19.0f;
                u->unk48C = 2.05f;
                u->jump_height_start = 2.0f;
                u->unk498 = scale_game_frames(1);
                u->jump_anim_retime_scale = 1.0f;
                u->jump_gravity = frame_time_sq * 25.0f;
                *(int *)&u->ledge_jump_speed = 0;
                u->item_select_blocked = 1;
                u->can_chain_high_jump = 1;
            } else if (u->state.current == 0x11) {
                u->unk430 = 3.3f;
                u->takeoff_frame = scale_game_frames(9);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 12.0f;
                u->target_anim_frame = 20.0f;
                u->jump_land_anim_frame = 10.0f;
                u->unk48C = 3.35f;
                u->jump_height_start = 3.3f;
                u->unk498 = scale_game_frames(0xC);
                u->fall_delay_frames = scale_game_frames(0x50);
                u->jump_gravity = frame_time_sq * 29.0f;
                qcopy(&u->unk470, &u->unk460);
                u->unk22B4 = 1;
                u->item_select_blocked = 1;
                u->unk4AC = 1;
            } else if (u->state.current == 0xD) {
                char *x = &u->trail.pos[0];
                u->unk430 = 0.1f;
                u->takeoff_frame = scale_game_frames(9);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 26.0f;
                u->target_anim_frame = 38.0f;
                u->jump_land_anim_frame = 18.0f;
                u->unk48C = 0.2f;
                u->jump_height_start = 0.1f;
                u->unk498 = scale_game_frames(0xE);
                u->fall_start_distance = 4.5f;
                u->jump_max_frames = scale_game_frames(0x78);
                u->air_stick_speed = frame_time * 1.9f;
                initialize_moby_transform_history(((int)u->moby), x);
                create_moby_transform_history_entry(x, 0x28, 3);
                create_moby_transform_history_entry(x, 0x14, 5);
                u->velocity_keys_active = 1;
                u->unk3D0 = scale_game_frames(9);
                u->velocity_keys_end_frame = scale_game_frames(0x23) + u->unk3D0;
                u->unk3DC = 26.0f;
                u->keyed_move_target_frame = 0.0f;
                u->keyed_anim_target_frame = scale_game_frames(0x2D);
                u->unk3E4 = D_L05_0017C380_e;
                u->can_chain_high_jump = 1;
            } else if (u->state.current == 0xF) {
                u->unk430 = 1.9f;
                u->takeoff_frame = scale_game_frames(9);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 50.0f;
                u->target_anim_frame = 64.0f;
                u->jump_land_anim_frame = 28.0f;
                u->unk48C = 1.95f;
                u->jump_height_start = 1.9f;
                u->unk498 = scale_game_frames(5);
                u->fall_start_distance = 4.5f;
                u->jump_max_frames = scale_game_frames(0x82);
                u->air_stick_speed = frame_time * 3.3f;
                u->unk4A6 = scale_game_frames(0x55);
                u->ledge_probe_cooldown = scale_game_frames(0x46);
                u->velocity_keys_active = 1;
                if (pal_mode != 0) {
                    u->unk3D0 = scale_game_frames(0x22);
                } else {
                    u->unk3D0 = scale_game_frames(0x23);
                }
                {
                    struct Hero *w = &hero;
                    w->velocity_keys_end_frame = scale_game_frames(0x24) + w->unk3D0;
                    w->keyed_move_target_frame = 29.0f;
                    w->unk3DC = 50.0f;
                    w->keyed_anim_target_frame = scale_game_frames(0x56);
                    w->unk3E4 = D_L05_0015F6F8;
                    w->can_chain_high_jump = 1;
                }
            } else if (u->state.current == 0xE) {
                float v = 0.7f;
                if (u->ground_distance < 2.0f)
                    v = (2.5f - u->ground_distance) * 0.5f + 0.7f;
                u->unk434 = 0.6f;
                u->apex_anim_frame = 32.0f;
                u->target_anim_frame = 43.0f;
                u->jump_land_anim_frame = 19.0f;
                u->unk48C = v + 0.05f;
                u->unk430 = v;
                u->jump_height_start = v;
                u->takeoff_frame = -1;
                u->unk498 = scale_game_frames(0xE);
                u->jump_anim_retime_scale = 1.0f;
                u->air_stick_speed = frame_time * 3.5f;
                u->unk4A6 = scale_game_frames(0x1E);
            } else if (u->state.current == 0xB) {
                u->unk430 = 3.2f;
                u->takeoff_frame = scale_game_frames(5);
                u->unk434 = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 31.0f;
                u->jump_land_anim_frame = 24.0f;
                u->unk48C = 3.25f;
                u->jump_height_start = 3.2f;
                u->unk498 = scale_game_frames(0xE);
                u->fall_start_distance = 4.5f;
                u->fall_delay_frames = scale_game_frames(0x32);
                u->jump_anim_retime_scale = 1.0f;
                u->takeoff_yaw = get_hero_yaw_history(scale_game_frames(0xA));
                u->unk450 = stick_direction_vs_facing();
                u->high_jump_drift_yaw = u->motion.move_yaw;
                if (frame_time * 1.5f < u->motion.flat_speed) {
                    u->air_target_speed = u->motion.flat_speed;
                } else {
                    u->air_target_speed = 0.0f;
                }
                {
                    struct Hero *w = &hero;
                    w->high_jump_drift_speed = 0;
                    w->unk4AC = 1;
                    w->can_chain_high_jump = 1;
                }
            }
        }
        {
            struct Hero *q = &hero;
            if (q->fall_start_distance == 0.0f)
                q->fall_start_distance = q->unk430 * 1.5f;
            if (b) {
                int s = q->state.current;
                if (s == 7) {
                    set_hero_animation(7, 0, (float)scale_game_frames(5));
                } else if (s == 0x12) {
                    set_hero_animation(0x72, 1, -1.0f);
                } else if (s == 0x3C) {
                    set_hero_animation(0x43, 0, (float)scale_game_frames(4));
                } else if (s == 0x69) {
                    set_hero_animation(7, 0, (float)scale_game_frames(5));
                } else if (s == 9) {
                    int id = 8;
                    int n = truncate_float_to_s32(q->anim_frame);
                    if (return_hero_animation_id_unchanged(MOBY(moby)->seq) == 4)
                        id = l05_run_jump_animation_by_run_frame[n];
                    set_hero_animation(id, 0, (float)scale_game_frames(5));
                } else if (s == 0xA) {
                    set_hero_animation(0x12, 0, (float)scale_game_frames(5));
                } else if (s == 0x10) {
                    if (q->state.prev_control_mode == 4 ||
                        (q->state.prev == 4 && scale_game_frames(7) > q->state_timer)) {
                        set_hero_animation(0x26, 0, (float)scale_game_frames(8));
                        q->takeoff_frame += scale_game_frames(3);
                    } else {
                        set_hero_animation(0x26, 0, (float)scale_game_frames(5));
                    }
                } else if (s == 0x1C) {
                    set_hero_animation(0x22, 0, (float)scale_game_frames(4));
                } else if (s == 0x11) {
                    set_hero_animation(0x23, 2, (float)scale_game_frames(7));
                } else if (s == 0xD) {
                    set_hero_animation(0x11, 0, (float)scale_game_frames(5));
                } else if (s == 0xE) {
                    set_hero_animation(0x16, 3, (float)scale_game_frames(7));
                    q->anim_speed = frame_scale * 0.8f;
                } else if (s == 0xF) {
                    set_hero_animation(0x15, 0, (float)scale_game_frames(5));
                } else if (s == 0xB) {
                    int id;
                    if (q->unk450 == 2)
                        q->unk450 = 0;
                    id = q->unk450 + 0x1C;
                    if (opt_mirror_anim != 0) {
                        if (id == 0x1C) {
                            id = 0x1D;
                        } else if (id == 0x1D) {
                            id = 0x1C;
                        }
                    }
                    {
                        struct Hero *w = &hero;
                        if (w->state.prev_control_mode == 4) {
                            set_hero_animation(id, 1, (float)scale_game_frames(4));
                            w->takeoff_frame = scale_game_frames(1);
                        } else {
                            set_hero_animation(id, 0, (float)scale_game_frames(5));
                        }
                    }
                }
            }
        }
        break;
    }
    case HERO_STATE_WRENCH_COMBO:
    case HERO_STATE_WRENCH_SLAM:
    case HERO_STATE_CROUCH_SWING:
    case HERO_STATE_HELD_ITEM_ATTACK:
    case HERO_STATE_ANIMATION_RECOVERY_WITH_JUMP_CANCEL:
    case HERO_STATE_VEHICLE_MODE_1_ATTACK: {
        struct Hero *p = &hero;
        p->state.control_mode = 6;
        p->speed_scale = 1.0f;
        p->unk2284 = 0;
        p->heading_locked = 0;
        *(int *)&p->melee_target = 0;
        p->unkA64 = 0;
        switch (p->state.current) {
        case HERO_STATE_WRENCH_SLAM: {
            struct UsageStat *h = usage_stats.stat;
            struct Hero      *q;
            if (h[0].count <= 0xFFFE) {
                h[0].count = h[0].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > h[0].unk2) {
                h[0].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            h[0].level_mask = h[0].level_mask | (1 << current_level_index) | 0x80000000;
            q = &hero;
            q->unk2284 = 0x50;
            q->attack_record = 4;
            q->saved_item[0] = 0;
            update_hero_target_heading(8.0f, 1.0471976f, -1.0f);
            *(int *)&q->slam_descending = 0;
            q->items[0].timer = 0xA;
            if (b) {
                char *m;
                int   n = scale_game_frames(0xB);
                if (q->state.prev == 0xB && scale_game_frames(0x2A) > q->state.prev_timer)
                    n = scale_game_frames(0x11);
                set_hero_animation(0x2B, 4, (float)n);
                m = hero_slot_moby(0);
                if (is_wrench_moby(m))
                    blend_moby_animation(m, 0xA, 4, n + 2);
            }
            break;
        }
        case HERO_STATE_CROUCH_SWING: {
            struct UsageStat *h = usage_stats.stat;
            char             *item_usage_stats;
            struct Hero      *q;
            if (h[1].count <= 0xFFFE) {
                h[1].count = h[1].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > h[1].unk2) {
                h[1].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            h[1].level_mask = h[1].level_mask | (1 << current_level_index) | 0x80000000;
            item_usage_stats = item_usage;
            if (*(unsigned short *)(item_usage_stats + 0x128) <= 0xFFFE) {
                *(unsigned short *)(item_usage_stats + 0x128) =
                    *(unsigned short *)(item_usage_stats + 0x128) + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 >
                *(unsigned short *)(item_usage_stats + 0x12A)) {
                *(unsigned short *)(item_usage_stats + 0x12A) = scale_game_frames(D_0015EEA4) / 600;
            }
            *(unsigned int *)(item_usage_stats + 0x12C) =
                *(unsigned int *)(item_usage_stats + 0x12C) | (1 << current_level_index) |
                0x80000000;
            q = &hero;
            q->attack_record = 3;
            q->saved_item[0] = 0;
            update_hero_target_heading(14.0f, 0.5235988f, -1.0f);
            q->unkA7C = q->unkA7C == 0;
            if (b) {
                int n = scale_game_frames(0xA);
                if (*(unsigned char *)(((char *)q->moby) + 0x53) == 0x1A)
                    n = scale_game_frames(3);
                if (q->state.prev == 1)
                    n = scale_game_frames(0x11);
                set_hero_animation(0x1A, 0, (float)n);
                hero_queue_sequence(6, 0x15);
            }
            break;
        }
        case HERO_STATE_HELD_ITEM_ATTACK: {
            struct Hero *q = &hero;
            if (((void *)q->items[0].moby) == 0)
                break;
            if (b) {
                set_hero_animation(0x67, 4, -1.0f);
                blend_moby_animation(((void *)q->items[0].moby), 3, 4, -1);
            }
            update_hero_target_heading(11.0f, 0.87266463f, -1.0f);
            break;
        }
        case HERO_STATE_WRENCH_COMBO: {
            char           *h = ((char *)&item_usage);
            struct Hero    *q;
            HeroTableRow2C *e;
            int             k;
            if (*(unsigned short *)(h + 0x40) <= 0xFFFE) {
                *(unsigned short *)(h + 0x40) = *(unsigned short *)(h + 0x40) + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > *(unsigned short *)(h + 0x42)) {
                *(unsigned short *)(h + 0x42) = scale_game_frames(D_0015EEA4) / 600;
            }
            q = &hero;
            *(unsigned int *)(h + 0x44) =
                *(unsigned int *)(h + 0x44) | (1 << current_level_index) | 0x80000000;
            e = &D_L05_0017C088[q->attack_record];
            q->saved_item[0] = 0;
            k = 0;
            if (q->state.prev_control_mode == 6 && e->unk0 == 0 && q->anim_frame < (float)e->unk8) {
                k = (e->kind + 1) % 3;
            }
            {
                struct Hero *r = &hero;
                r->attack_record = k;
            }
            update_hero_target_heading(5.0f, 0.7853982f, 0.7853982f);
            if (b) {
                char *m;
                int   j = 0;
                int   n;
                if (k == 0) {
                    n = scale_game_frames(5);
                } else if (k == 1) {
                    j = 2;
                    n = scale_game_frames(7);
                } else {
                    j = 4;
                    n = scale_game_frames(7);
                }
                {
                    struct Hero *r = &hero;
                    if (r->heading_locked != 0 && k < 2) {
                        n += truncate_float_to_s32(fast_difference_between_rotations(
                                                       r->attack_target_yaw, r->motion.rot.f[2]) *
                                                   1.5f);
                    }
                }
                j = j / 2;
                set_hero_animation(k + 0x17, j, (float)n);
                m = hero_slot_moby(0);
                if (is_wrench_moby(m))
                    blend_moby_animation(m, k + 3, j, n);
            }
            break;
        }
        case HERO_STATE_ANIMATION_RECOVERY_WITH_JUMP_CANCEL: {
            struct Hero *q;
            if (b)
                set_hero_animation(0x2C, 7, (float)scale_game_frames(9));
            q = &hero;
            if (q->items[0].item_id == 0xA && ((char *)q->throw_target) != 0) {
                q->heading_locked = 1;
                q->attack_target_yaw =
                    fast_arc_tan(*(float *)(((char *)q->throw_target) + 0x10) - q->motion.pos.f[0],
                                 *(float *)(((char *)q->throw_target) + 0x14) - q->motion.pos.f[1]);
            }
            break;
        }
        }
        break;
    }
    case HERO_STATE_WAIT_FOR_ANIMATION_THEN_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xA;
        p->unk2284 = 0;
        p->ground_speed = frame_time * 9.0f;
        if (b) {
            char           *m;
            HeroTableRow2C *e = &D_L05_0017C088[p->attack_record];
            int             k = e->kind;
            int             n;
            if (e->unk0 == 1)
                k = 2;
            n = scale_game_frames(5);
            set_hero_animation(k + 0x27, 5, (float)n);
            m = hero_slot_moby(0);
            if (is_wrench_moby(m))
                blend_moby_animation(m, k + 7, 5, n);
        }
        break;
    }
    case HERO_STATE_WALL_BONK: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xA;
        p->unk2284 = 0;
        p->pushback_yaw = fast_add_rotations(p->motion.rot.f[2], 3.1415927f);
        p->ground_speed = frame_time * 9.0f;
        if (b)
            set_hero_animation(0x29, 5, (float)scale_game_frames(8));
        break;
    }
    case HERO_STATE_HURT: {
        struct Hero *p = &hero;
        char        *r;
        p->state.control_mode = 7;
        p->unk2284 = 0;
        scale_hero_motion(0.5f);
        p->unk1EE = scale_game_frames(0x2D);
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        FUN_L00_0025f730(&p->motion.velocity, frame_time * 7.0f);
        r = find_moby_hit(((void *)p->moby), 1, 0);
        if (r != 0) {
            hero_change_health(truncate_float_to_s32(*(float *)(r + 0x2C)));
        } else {
            hero_change_health(1);
        }
        {
            struct Hero *q = &hero;
            q->invulnerable_frames = scale_game_frames(0x4D);
            if (b) {
                set_hero_animation(0x10, 3, -3.0f);
                *(float *)(((char *)q->moby) + 0x54) =
                    level05_hero_animation_blend_curves[q->anim_blend_curve]
                                                       [(q->anim_blend_step)++];
            }
        }
        break;
    }
    case HERO_STATE_WAIT_FOR_JUMP_OR_RESPAWN: {
        struct Hero *p = &hero;
        if (p->state.prev == 0x69 ||
            (p->state.prev2 == 0x69 && p->state.prev_control_mode != 0 && p->state.prev != 7)) {
            p->sink_jump_count = p->sink_jump_count + 1;
        } else {
            struct Hero *q = &hero;
            q->sink_jump_count = 0;
        }
        {
            struct Hero *r = &hero;
            r->state.control_mode = 0x19;
            r->unk2284 = 0;
            allocate_hero_voice_for_entry(9, 0);
            r->rand_timer.range = 0x68;
            r->target_speed = 0.0f;
            r->ground_speed = 0.0f;
            if (b)
                set_hero_animation(0x71, 0, (float)scale_game_frames(8));
        }
        break;
    }
    case HERO_STATE_HEIGHT_OR_TIMEOUT_RESPAWN_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x19;
        p->unk2284 = 0;
        if (current_level_index == 0xF)
            spawn_hero_radial_particle_burst();
        p->collision_off_frames = 0x2710;
        allocate_hero_voice_for_entry(9, 0);
        p->rand_timer.range = 0x68;
        if (b)
            set_hero_animation(0x71, 0, (float)scale_game_frames(8));
        break;
    }
    case HERO_STATE_DEATH_DESCENT: {
        if (current_level_index == 6) {
            int c0;
            int c1;
            int c2;
            hero_set_state(HERO_STATE_DYING, 1);
            {
                struct Hero *q = &hero;
                q->color_fade_active = 1;
                get_moby_color(((void *)q->moby), &c0, &c1, &c2);
                q->tint_color = (c2 << 16) | ((c1 << 8) | 0x80000000) | c0;
            }
            return 0;
        }
        {
            struct Hero *p = &hero;
            p->state.control_mode = 0x14;
            p->unk2284 = 0;
            p->health.hp = 0;
            p->collision_off_frames = 0x2710;
            p->sink_surface_z = p->ground_z.f;
            allocate_hero_voice_for_entry(9, 0);
            if (b)
                set_hero_animation(0x71, 0, (float)scale_game_frames(8));
        }
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0;
        p->unk2284 = 0;
        if (b)
            set_hero_animation(0, 0, (float)scale_game_frames(0xE));
        break;
    }
    case HERO_STATE_DELAYED_CIRCLE_RELEASE_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = 0;
        p->unk2284 = 0;
        if (b) {
            set_hero_animation(4, 0, (float)scale_game_frames(7));
            hero_queue_sequence(0xE, 0x22);
        }
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_SLIP: {
        struct Hero *p = &hero;
        p->state.control_mode = 2;
        p->unk2284 = 0;
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (b)
            set_hero_animation(6, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_FALL: {
        struct Hero *p = &hero;
        p->state.control_mode = 2;
        p->unk2284 = 0;
        p->fall_state_gravity = frame_time_sq * 24.0f;
        p->stickless_decel = frame_time_sq * 7.0f;
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (b)
            set_hero_animation(6, 0, (float)scale_game_frames(7));
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_RUN: {
        struct Hero *p = &hero;
        p->state.control_mode = 1;
        p->unk2284 = 0;
        {
            char *v = (char *)&p->motion.resolved_velocity;
            char *g;
            float r = vector_length_xy(v);
            g = v - 0x110;
            *(float *)(g + 0x194) = r;
            if (frame_time * 1.5f < r)
                *(float *)(g + 0x194) = frame_time * 1.5f;
            *(short *)(g + 0x3B8) = 0;
            if (b)
                set_hero_animation(1, 0, (float)scale_game_frames(8));
        }
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_DYING: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x14;
        p->unk2284 = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (b)
            set_hero_animation(3, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_HURT: {
        struct Hero *p = &hero;
        p->state.control_mode = 7;
        p->unk2284 = 0;
        scale_hero_motion(0.5f);
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        FUN_L00_0025f730(&p->motion.velocity, frame_time * 5.0f);
        hero_change_health(1);
        p->invulnerable_frames = scale_game_frames(0x4D);
        if (b) {
            set_hero_animation(2, 3, -3.0f);
            *(float *)(((char *)p->moby) + 0x54) =
                level05_hero_animation_blend_curves[p->anim_blend_curve][(p->anim_blend_step)++];
        }
        break;
    }
    case HERO_STATE_GRIND_COLLISION_RECOVERY: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xF;
        p->unk2284 = 0;
        if (p->rail_off_frames == 0)
            hero_change_health(1);
        p->invulnerable_frames = scale_game_frames(0x4D);
        p->unk5CC = frame_time * 2.5f;
        if (frame_time * 2.5f < p->grind_base_speed)
            p->grind_base_speed = frame_time * 2.5f;
        p->unk5AC.i = 0;
        clear_u64_value(&p->motion.velocity);
        clear_u64_value(&p->motion.displacement);
        p->motion.velocity.f[2] = frame_time * 10.0f;
        if (b)
            set_hero_animation(0x10, 2, -3.0f);
        break;
    }
    case HERO_STATE_BLEND_TO_IDLE_ANIMATION: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->item_use_locked = 1;
        p->unk2284 = 0;
        set_hero_animation(get_hero_idle_animation_index(0), 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_LOOK_RELEASE_WAIT: {
        struct Hero *p = &hero;
        p->unk2284 = 0x51;
        p->state.control_mode = 0;
        if (return_hero_animation_id_unchanged(*(unsigned char *)(((char *)p->moby) + 0x53)) !=
            get_hero_idle_animation_index(1)) {
            set_hero_animation(get_hero_idle_animation_index(1), 0, (float)scale_game_frames(5));
        }
        break;
    }
    case HERO_STATE_IDLE_ANIMATION_ENTRY: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->unk2284 = 0;
        set_hero_animation(get_hero_idle_animation_index(0), 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_IDLE_ANIMATION_AFTER_ITEM_CANCELLATION: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->item_use_locked = 1;
        p->unk2284 = 0;
        cancel_hero_item_animation();
        set_hero_animation(get_hero_idle_animation_index(0), 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_IDLE_ANIMATION_LONG_BLEND_ENTRY: {
        struct Hero *p = &hero;
        p->state.control_mode = 8;
        p->unk2284 = 0;
        set_hero_animation(get_hero_idle_animation_index(0), 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_OPTIONAL_IDLE_ANIMATION: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->unk2284 = 0;
        p->item_select_blocked = 1;
        p->tilt_disabled = 1;
        p->item_use_locked = 1;
        if (b)
            set_hero_animation(get_hero_idle_animation_index(0), 0, -1.0f);
        break;
    }
    case HERO_STATE_CLEAR_VELOCITY: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->unk2284 = 0;
        p->item_select_blocked = 1;
        p->item_use_locked = 1;
        clear_u64_value(&p->motion.velocity);
        if (b)
            set_hero_animation(0xB, 0, -1.0f);
        break;
    }
    case HERO_STATE_CANCEL_ITEM_WITH_OPTIONAL_IDLE_ANIMATION:
    case HERO_STATE_IDLE_ANIMATION_WITH_DYING_OVERRIDE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x18;
        p->item_use_locked = 1;
        cancel_hero_item_animation();
        if (b)
            set_hero_animation(get_hero_idle_animation_index(0), 0, -1.0f);
        break;
    }
    }
    if (old20A7 != 0) {
        struct Hero *t = &hero;
        if (t->item_select_blocked == 0) {
            t->restore_item[0] = 1;
        }
    }
    {
        struct Hero *t = &hero;
        if (t->unk308 == 0 && t->items[1].item_id == 0x1C && t->saved_item[1] != 0) {
            t->restore_item[1] = 1;
        }
    }
    {
        struct Hero *t = &hero;
        if (t->state.control_mode != 0xF && t->items[1].item_id == 0x1D && t->saved_item[1] != 0) {
            t->restore_item[1] = 1;
        }
    }
    if (hero_is_in_special_state() == 0) {
        struct Hero *t = &hero;
        if (t->items[1].item_id == 0 && t->saved_item[1] != 0) {
            t->restore_item[1] = 1;
        }
    }
    {
        struct Hero *t = &hero;
        if (t->state.control_mode == 0x14) {
            cancel_hero_item_animation();
            t->item_use_locked = 1;
        }
        if (t->state.control_mode != t->state.prev_control_mode) {
            release_and_clear_hero_voice_slots();
        }
        t->state_timer = 0;
        t->step_frames = 0;
        if (t->state.control_mode != t->state.prev_control_mode) {
            t->control_mode_timer = 0;
        }
        t->anim_seq_frames = 0;
    }
    return 1;
}
