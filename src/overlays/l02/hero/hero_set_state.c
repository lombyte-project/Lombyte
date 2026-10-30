/* Level 02 hero state setter: hero_set_state, with the per-frame state transitions (update_hero_state_transitions) and enter_hero_idle_or_stop. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/hero.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/usage_stats.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/hero/hero_handle_jump_press.h"
#include "rnc/overlay/shared/hero/hero_idle_sequence.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/hero/process_hero_item_action_input.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/hero_tables.h"

/* Ported from rac1-decomp (src/overlays/l02_aridia/help_0022BE40.c: func_L02_0022BE40), where it is exact; names translated to the US level program. */

extern HeroTableRow2C D_L02_0017C228[];
extern char          *hero_slot_moby(int) __asm__("FUN_L00_0020d460");
/* D_0015ED60 reached through the word below it: retail loads it with lui here, not $gp. */
extern float D_0015ED5C_lui[2] __asm__("D_0015ED5C");
extern float hero_animation_blend_curve_samples[][25] __asm__("D_L02_0017C3F0");
extern float get_hero_yaw_history(int back) __asm__("FUN_L00_0020d658");
extern float gravity_length_xy(float *v) __asm__("FUN_L00_002339d0");
extern float vector_length_xy(void *);
extern int   hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");
extern int   is_wrench_moby(char *) __asm__("FUN_L00_00211360");
extern int   stick_direction_vs_facing(void) __asm__("FUN_L00_00221df8");
extern int   get_hero_idle_animation_index(int a) __asm__("FUN_L01_00226f10");

struct HeroVelocityKey D_L02_0015F6E8[1] __attribute__((section(".sdata"))) = {
    {48.0f, 0.0f, 1000}
};

int D_L02_0015F6D8[4] __attribute__((section(".sdata"))) = {8, 0, 11, 16};

extern unsigned char l02_run_jump_animation_ids_by_frame[] __asm__("D_L02_0017C3D8");
extern void          hero_change_health(int n) __asm__("FUN_L00_002050c0");
extern void          clear_hero_idle_rotation_targets(void) __asm__("FUN_L00_0020a500");
extern void          exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");
extern void create_missing_hero_item_slot_animations(int a, int b) __asm__("FUN_L00_0020e360");
extern void scale_hero_motion(float) __asm__("FUN_L00_00213de8");
extern void update_hero_target_heading(float a, float b, float c) __asm__("FUN_L00_00214ed8");
extern void spawn_hero_radial_particle_burst(void) __asm__("FUN_L00_00217450");
extern void hero_queue_sequence(int bank, int seq) __asm__("FUN_L00_002325e0");
extern void FUN_L00_0025f730(void *a, float x);
extern void create_moby_transform_history_entry(char *o, int a, int b) __asm__("FUN_L00_00262528");
extern void clear_moby_transform_history(char *o) __asm__("FUN_L00_00262840");
extern void request_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L01_002405a0");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

/* Sets hero.state.current to a: moves state, control_mode and state_timer into the prev_* fields, zeroes state_step, then sets up the new state; returns 0 when the change is refused. */
extern int  hero_set_state(int, int) __asm__("FUN_L02_0022b728");
extern char level02_hero_transform_trail_jump_velocity_keys[] __asm__("D_L02_0017C520");

int hero_set_state(int a, int b) {
    int   old2088;
    char *moby;
    int   old20A7;
    {
        struct Hero *p = &hero;
        if (p->state.control_mode == 0x14 && (a != 0x64 || b != 2))
            return 0;
    }
    if ((level_game_mode == 2 || level_game_mode == 6) && b != 2)
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
    case HERO_STATE_LOOK: {
        struct Hero *p = &hero;
        int          t;
        p->state.control_mode = 0;
        p->unk2284 = 4;
        *(int *)&D_L02_0015F688 = scale_game_frames(0x3C);
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
            if (t->unk308 == 1 || t->state.current == 0x3F) {
                t->state.current = 0x3F;
                set_hero_animation(0x5B, 0, (float)scale_game_frames(8));
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
                    set_hero_animation(v->state.step + 3, D_L02_0015F6D8[v->state.step], (float)n);
                }
            }
        }
        break;
    }
    case HERO_STATE_ANIMATION_FRAME_EXIT_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = 1;
        p->unk2284 = 0;
        if (b) {
            if (p->state.prev == 4) {
                set_hero_animation(0x5E, 2, (float)scale_game_frames(5));
            } else {
                set_hero_animation(0x5E, 1, (float)scale_game_frames(8));
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
                    p->state.step + 3, D_L02_0015F6D8[p->state.step], (float)scale_game_frames(8));
            }
        }
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
        int          animation_timing_selector = -2;
        p->state.control_mode = 1;
        p->unk22CA = 1;
        p->rand_timer.range = 0x68;
        p->unk2284 = 0;
        if (frame_time * 4.5f < p->motion.speed_xy) {
            scale_hero_motion(frame_time * 4.5f / p->motion.speed_xy);
        }
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (b) {
            set_hero_animation(0x13, 0, (float)animation_timing_selector);
        }
        break;
    }
    case HERO_STATE_LAND: {
        struct Hero *p = &hero;
        int          animation_timing_selector = -2;
        p->state.control_mode = 5;
        p->rand_timer.range = 0x68;
        p->unk2284 = 0;
        if (frame_time * 4.5f < p->motion.speed_xy) {
            scale_hero_motion(frame_time * 4.5f / p->motion.speed_xy);
        }
        qcopy(&p->motion.velocity, &p->motion.displacement);
        if (b) {
            set_hero_animation(0x13, 0, (float)animation_timing_selector);
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
                char *x = (char *)&u->trail;
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
                initialize_moby_transform_history(*(int *)&u->moby, x);
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
                u->ledge_jump_speed = 0;
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
                char *x = (char *)&u->trail;
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
                initialize_moby_transform_history(*(int *)&u->moby, x);
                create_moby_transform_history_entry(x, 0x28, 3);
                create_moby_transform_history_entry(x, 0x14, 5);
                u->velocity_keys_active = 1;
                u->unk3D0 = scale_game_frames(9);
                u->velocity_keys_end_frame = scale_game_frames(0x23) + u->unk3D0;
                u->unk3DC = 26.0f;
                u->keyed_move_target_frame = 0.0f;
                u->keyed_anim_target_frame = scale_game_frames(0x2D);
                *(char **)&u->unk3E4 = level02_hero_transform_trail_jump_velocity_keys;
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
                    w->unk3E4 = D_L02_0015F6E8;
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
                } else if (s == 0x69) {
                    set_hero_animation(7, 0, (float)scale_game_frames(5));
                } else if (s == 9) {
                    int id = 8;
                    int n = truncate_float_to_s32(q->anim_frame);
                    if (return_hero_animation_id_unchanged(MOBY(moby)->seq) == 4)
                        id = l02_run_jump_animation_ids_by_frame[n];
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
                    q->anim_speed = D_0015ED5C_lui[1] * 0.8f;
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
            char             *item_usage_stats_base;
            struct Hero      *q;
            if (h[1].count <= 0xFFFE) {
                h[1].count = h[1].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > h[1].unk2) {
                h[1].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            h[1].level_mask = h[1].level_mask | (1 << current_level_index) | 0x80000000;
            item_usage_stats_base = item_usage;
            if (*(unsigned short *)(item_usage_stats_base + 0x128) <= 0xFFFE) {
                *(unsigned short *)(item_usage_stats_base + 0x128) =
                    *(unsigned short *)(item_usage_stats_base + 0x128) + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 >
                *(unsigned short *)(item_usage_stats_base + 0x12A)) {
                *(unsigned short *)(item_usage_stats_base + 0x12A) =
                    scale_game_frames(D_0015EEA4) / 600;
            }
            *(unsigned int *)(item_usage_stats_base + 0x12C) =
                *(unsigned int *)(item_usage_stats_base + 0x12C) | (1 << current_level_index) |
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
            e = &D_L02_0017C228[q->attack_record];
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
    case HERO_STATE_ATTACK_RECORD_FRAME_EXIT_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = 6;
        p->unkA64 = 0;
        if (p->state.prev == 0x70) {
            int v = 5;
            if (D_L02_0017C228[p->attack_record].kind == 0)
                v = 6;
            p->attack_record = v;
        } else {
            p->attack_record = 5;
        }
        {
            struct Hero *q = &hero;
            int          e = D_L02_0017C228[q->attack_record].kind;
            if (b) {
                char *m;
                int   z = e == 0;
                int   n = scale_game_frames(5);
                set_hero_animation(e + 0x5C, z, (float)n);
                m = hero_slot_moby(0);
                if (is_wrench_moby(m))
                    blend_moby_animation(m, e + 0xC, z, n);
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
            HeroTableRow2C *e = &D_L02_0017C228[p->attack_record];
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
                    hero_animation_blend_curve_samples[q->anim_blend_curve][(q->anim_blend_step)++];
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
                hero_animation_blend_curve_samples[p->anim_blend_curve][(p->anim_blend_step)++];
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

/* Hero state machine: per-frame transition logic for the current hero state (level 02 build). */
/* Ported from rac1-decomp (src/overlays/l02_aridia/help_0022BE40.c: func_L02_002310F0), where it is exact; names translated to the US level program. */

typedef struct {
    int  a;
    int  b;
    int  c;
    char pad[0x20];
} T2C_22BE40_u;

typedef struct {
    char  p0[0x254];
    float f254;
    float f258;
} S17AD00;

typedef struct {
    char  p0[0xC];
    float fC;
    char  p1[8];
    float f18;
} S17C438;

typedef struct {
    char           p0[0x80] __attribute__((aligned(16)));
    float          f80[4];
    float          f90[4];
    char           pA0[0x40];
    float          fE0[4];
    char           pF0[0x10];
    float          f100[4];
    char           p110[0x30];
    float          f140[4];
    float          f150[4];
    float          f160;
    float          f164;
    float          f168;
    char           p16C[0x14];
    float          f180;
    char           p184[0xC];
    float          f190;
    float          f194;
    int            f198;
    char           p19C[0x4];
    int            f1A0;
    int            f1A4;
    char           p1A8[0x8];
    short          f1B0;
    short          f1B2;
    int            f1B4;
    int            f1B8;
    char           p1BC[0x8];
    int            f1C4;
    char           p1C8[0x8];
    int            f1D0;
    int            f1D4;
    short          f1D8;
    char           p1DA[0x4];
    short          f1DE;
    short          f1E0;
    short          f1E2;
    short          f1E4;
    char           p1E6[0x2];
    short          f1E8;
    char           p1EA[0x8];
    short          f1F2;
    char           p1F4[0x4];
    short          f1F8;
    char           p1FA[0x4E];
    float          f248;
    char           p24C[0xB];
    unsigned char  f257;
    char           p258[0x80];
    float          f2D8;
    float          f2DC;
    float          f2E0;
    char           p2E4[0x10];
    float          f2F4;
    int            f2F8;
    int            f2FC;
    int            f300;
    char           p304[0x4];
    short          f308;
    short          f30A;
    short          f30C;
    short          f30E;
    char           p310[0xAC];
    short          f3BC;
    short          f3BE;
    char           p3C0[0x10];
    int            f3D0;
    int            f3D4;
    float          f3D8;
    char           p3DC[0x4];
    int            f3E0;
    char           p3E4[0x34];
    int            f418;
    short          f41C;
    short          f41E;
    int            f420;
    char           p424[0x1C];
    float          f440;
    float          f444;
    float          f448;
    int            f44C;
    char           p450[0x40];
    float          f490;
    float          f494;
    char           p498[0x2];
    short          f49A;
    char           p49C[0x4];
    float          f4A0;
    char           p4A4[0x2];
    short          f4A6;
    char           p4A8[0x2];
    short          f4AA;
    char           p4AC[0x2];
    unsigned char  f4AE;
    unsigned char  f4AF;
    float          f4B0;
    char           p4B4[0x4];
    float          f4B8;
    int            f4BC;
    float          f4C0;
    char           p4C4[0x24];
    int            f4E8;
    char           p4EC[0xF4];
    float          f5E0[4];
    char           p5F0[0xC];
    int            f5FC;
    int            f600;
    char           p604[0x94];
    int            f698;
    char           p69C[0x2A4];
    float          f940;
    float          f944;
    char           p948[0x18];
    float          f960;
    char           p964[0x8];
    float          f96C;
    char           p970[0x18];
    int            f988;
    char           p98C[0xD4];
    int            fA60;
    char           pA64[0xC];
    float          fA70;
    char           pA74[0x1C];
    float          fA90;
    char           pA94[0x4];
    int            fA98;
    int            fA9C;
    char           pAA0[0x8];
    float          fAA8;
    char           pAAC[0x8];
    int            fAB4;
    char           pAB8[0x250];
    int            fD08;
    char           pD0C[0x8];
    int            fD14;
    char           pD18[0x378];
    unsigned char *f1090;
    char           p1094[0xC];
    int            f10A0;
    char           p10A4[0x8];
    unsigned char  f10AC;
    char           p10AD[0xB];
    int            f10B8;
    char           p10BC[0xC4];
    unsigned char *f1180;
    unsigned char *f1184;
    char           p1188[0x1C];
    int            f11A4;
    char           p11A8[0x13A];
    unsigned char  f12E2;
    unsigned char  f12E3;
    char           p12E4[0x3];
    unsigned char  f12E7;
    char           p12E8[0x2];
    unsigned char  f12EA;
    char           p12EB[0x2];
    unsigned char  f12ED;
    char           p12EE[0x326];
    int            f1614;
    char           p1618[0x18];
    int            f1630;
    char           p1634[0x2];
    short          f1636;
    char           p1638[0x8];
    float          f1640[4];
    int            f1650;
    char           p1654[0xC];
    int            f1660;
    char           p1664[0xA1C];
    unsigned char *f2080;
    int            f2084;
    int            f2088;
    int            f208C;
    int            f2090;
    int            f2094;
    char           p2098[0x4];
    int            f209C;
    char           p20A0[0x4];
    unsigned char  f20A4;
    unsigned char  f20A5;
    char           p20A6[0x1];
    unsigned char  f20A7;
    unsigned char  f20A8;
    unsigned char  f20A9;
    unsigned char  f20AA;
    char           p20AB[0x1];
    unsigned char  f20AC;
    char           p20AD[0xB];
    int            f20B8;
    char           p20BC[0x164];
    int            f2220;
    char           p2224[0x4C];
    int            f2270;
    char           p2274[0x10];
    int            f2284;
    char           p2288[0xC];
    int            f2294;
    int            f2298;
    float          f229C;
    char           p22A0[0x4];
    float          f22A4;
    int            f22A8;
    char           p22AC[0x18];
    int            f22C4;
    short          f22C8;
    unsigned char  f22CA;
    char           p22CB[0x7];
    short          f22D2;
    short          f22D4;
    short          f22D6;
    short          f22D8;
    char           p22DA[0x2];
    short          f22DC;
    short          f22DE;
} Hero;

extern HeroAnimRow      D_L02_0017A0C0[];
extern HeroTableEntry70 l02_idle_fidget_records[] __asm__("D_L02_00179E90");
extern S17AD00          D_L02_0017AC80;
extern S17C438          D_L02_0017C3B8;

#include "rnc/overlay/l02/hero/hero_set_state_globals.h"

extern float scale_time(float) __asm__("FUN_001f96b0");
extern float adjust_hero_animation_speed_for_target_frame(float a,
                                                          float b,
                                                          float c,
                                                          float d) __asm__("FUN_L00_00208f20");
extern float
predict_hero_trajectory_intercept_time(float a, float b, float c) __asm__("FUN_L00_00232b90");
extern float          predict_hero_trajectory_z(float *pos,
                                                float *vel,
                                                int   *steps,
                                                float  step) __asm__("FUN_L00_00233c30");
extern float          read_pad_float_history(int) __asm__("FUN_L00_00266e80");
extern float          approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float          fast_cos(float);
extern float          fast_sin(float);
extern int D_00141660 NOT_SDA;
int                   r1_repeat_timer __asm__("D_L02_0015F678") = 0;
extern int            D_L02_0015F6DC_d __asm__("D_L02_0015F6DC");
extern int D_L02_0016CD80 NOT_SDA;
extern int                get_hero_high_jump_input_window_frames(void) __asm__("FUN_L00_00221d98");
extern int                try_start_hero_weapon_fire_state(void) __asm__("FUN_L00_002276c0");
extern int                try_start_hero_jump(int, int) __asm__("FUN_L00_00228b78");
extern int                pick_idle_fidget(int *) __asm__("FUN_L00_00229010");
extern int                hero_handle_jump_press(void) __asm__("FUN_L00_00229660");
extern int                check_hero_forward_collision_height(void) __asm__("FUN_L00_00229910");
extern int                pad_pressed_within(int, int, int *) __asm__("FUN_L00_00266d60");
extern int  check_pad_press_separation(int, int, int, int) __asm__("FUN_L00_00266e00");
extern int  test_hero_ledge_ahead(float *, float *) __asm__("FUN_L01_0022d838");
extern int  process_hero_hurt_response(int) __asm__("FUN_L02_0021ff70");
extern int  queue_animation_update(int, int, int, int, int, int, int) __asm__("FUN_001ff308");
extern void hero_switch_item(int i) __asm__("FUN_L00_0020fea0");
extern void update_hero_animation_for_movement_speed(void) __asm__("FUN_L00_00228c18");
extern void
remove_hero_cached_moby_and_clear_slot_three_moby_flags(void) __asm__("FUN_L00_00228fa8");
extern void blend_hero_slot3_animation_pair(void) __asm__("FUN_L00_00231d08");
extern void clear_hero_queued_animation(void) __asm__("FUN_L00_00232628");
extern void hero_set_attack_ticks(int id) __asm__("FUN_L00_00232640");
extern void configure_hero_point_effect_emission(float         a,
                                                 float         b,
                                                 short         c,
                                                 unsigned char d,
                                                 unsigned char e,
                                                 unsigned char f) __asm__("FUN_L00_00233288");
extern void hud_keep_visible(int, int) __asm__("FUN_L00_00235e18");
extern void initialize_eight_slot_hud_selector(void) __asm__("FUN_L00_00237ae8");
extern void update_hud_item_selector_selection_and_visibility(void) __asm__("FUN_L00_00237ba0");
extern void draw_hud_item_selector_panel(void) __asm__("FUN_L00_002382a0");
extern void initialize_bolt_counter_hud(void) __asm__("FUN_L00_00239cc8");
extern void update_bolt_counter_hud_animation_and_visibility(void) __asm__("FUN_L00_00239d00");
extern void draw_bolt_counter_hud_panel(void) __asm__("FUN_L00_00239df8");
extern void request_hero_voice_if_enabled(int voice_group_index,
                                          int voice_pair_index,
                                          int call_enabled) __asm__("FUN_L01_00227e48");
extern void select_hero_locomotion_state(void) __asm__("FUN_L01_00242198");
extern HeroTableRow2C D_L02_0017C2A8_rec[] __asm__("D_L02_0017C228");
extern int            func_L00_00235790_r(void) __asm__("FUN_L00_00234e00");

extern f32 frame_scale_gp __asm__("D_0015ED60") __attribute__((sda));
void       update_hero_state_transitions(void) __asm__("FUN_L02_00230988");

void update_hero_state_transitions(void) {
    unsigned char *moby;
    int            prevSub;
    int            prevState;
    int            prevAnim;
    float          ledge_probe_position[4] __attribute__((aligned(16)));
    float          lateral_ledge_probe_position[4] __attribute__((aligned(16)));
    int            record_index;
    int            respawn_jump_press_age_frames;
    float          current_position_probe_yaw;
    float          positive_lateral_probe_yaw;
    float          negative_lateral_probe_yaw;
    float          lateral_step_probe_yaw;
    int            predicted_trajectory_steps;
    int            jump_press_age_frames;

    moby = ((unsigned char *)hero.moby);
    prevSub = hero.state.control_mode;
    prevState = hero.state.step;
    prevAnim = moby[0x53];
    if (process_hero_hurt_response(1) == 0) {
        if (hero.health.hp <= 0 && hero.state.control_mode != 0x14 &&
            (hero.state.current == HERO_STATE_IDLE || hero.state.current == HERO_STATE_RUN)) {
            hero_set_state(HERO_STATE_DYING, 1);
        } else if (process_hero_item_action_input() == 0 &&
                   try_start_hero_weapon_fire_state() == 0) {
            if ((controller_state.pressed & 0x10) || (controller_state.pressed_unmasked & 0x10)) {
                if (hero.items[0].switch_lock != 2 && item_selector_hud_inhibit == 0 &&
                    (hero.item_use_locked == 0 || hero.vehicle_mode == 1 || hero.unk1B2 != 0) &&
                    (hero.item_select_blocked == 0 || hero.vehicle_mode == 1 || hero.unk1B2 != 0) &&
                    (hero.vehicle_mode == 0 || hero.vehicle_mode == 3)) {
                    int id;

                    if (hero.quick_swap_tap_window != 0) {
                        hero.item_selector_timer = scale_game_frames(0x1B);
                    } else {
                        hero.item_selector_timer = scale_game_frames(0xF);
                    }
                    queue_animation_update(3,
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
            if (fatal_fall_height_threshold > hero.motion.pos.f[2] && hero.ground_distance > 2.0f &&
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
                        if (hero.unk308 == 0 || D_0013D4DC[0] == 0) {
                            hero_set_state(0x81, 1);
                            break;
                        }
                    }
                    {
                        int            flag;
                        unsigned char *m;
                        int            idle_hero_animation_id;

                        flag = 1;
                        if (hero_slot_item_id(0) == 0x1B) {
                            if (((unsigned char *)hero.moby)[0x53] !=
                                get_hero_idle_animation_index(0)) {
                                set_hero_animation(get_hero_idle_animation_index(0), 0, -2.0f);
                            }
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
                        if (0.61086524f < D_L02_0017AC80.f258 ||
                            D_L02_0017AC80.f254 > 0.43633232f) {
                            m = ((unsigned char *)hero.items[3].moby);
                            if (m != 0 && m[0x53] != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(0xC));
                            }
                            m = ((unsigned char *)hero.items[3].moby2);
                            if (m != 0 && m[0x53] != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(0xC));
                            }
                        }
                        idle_hero_animation_id = return_hero_animation_id_unchanged(moby[0x53]);
                        if (idle_hero_animation_id == get_hero_idle_animation_index(0) && flag) {
                            if (pick_idle_fidget(&record_index)) {
                                HeroTableEntry70 *rec;

                                hero.fidget_index = record_index;
                                rec = &l02_idle_fidget_records[record_index];
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
                                if (get_hero_idle_animation_index(0) == 0x54) {
                                    set_hero_animation(get_hero_idle_animation_index(0),
                                                       0,
                                                       scale_game_frames(0x12));
                                } else {
                                    set_hero_animation(get_hero_idle_animation_index(0), 0, -2.0f);
                                }
                            }
                        }
                    }
                    if (scale_game_frames(0xA) < hero.state_timer) {
                        if (((unsigned char *)hero.items[3].moby2) != 0 &&
                            ((unsigned char *)hero.items[3].moby2)[0x53] == 1) {
                            if (fast_dec_timer(&hero.slot3_anim_timer)) {
                                if (0.55850536f > D_L02_0017AC80.f258 &&
                                    D_L02_0017AC80.f254 < 0.34906584f && hero.items[3].state == 2) {
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
                            int anim = D_L02_0017A0C0[id].anim;
                            if (((unsigned char *)hero.moby)[0x53] != anim) {
                                if (hero.held_item_anim_blend[0] != 0) {
                                    hero.unkD14 = 1;
                                }
                                hero.item_hold_anim = anim;
                                set_hero_animation(hero.item_hold_anim, 0, scale_game_frames(0xB));
                            }
                        }
                    }
                    stick_target(1.0f, 0);
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
                    {
                        int ok = 1;
                        if (hero.state.prev == 1 || hero.state.prev == 0x1E) {
                            if (scale_game_frames(0x18) > hero.state_timer && hero.unk308 == 0) {
                                stick_target(1.0f, 0);
                                if (fast_difference_between_rotations(
                                        hero.motion.move_yaw, D_L02_001673D8) > 1.2217305f) {
                                    ok = 0;
                                }
                            }
                        }
                        if (ok && 0.22f < hero.stick_magnitude) {
                            if (current_level_index == 0xC) {
                                int slot = hero.ground_sound_class;
                                request_hero_voice_if_enabled(slot, 0, 0);
                                request_hero_voice_if_enabled(slot, 1, 0);
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
                    update_hero_stick_target_magnitude();
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
                        update_hero_stick_target_magnitude();
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
                case HERO_STATE_LOOK:
                    if (hero.vehicle_mode == 0) {
                        if (hero.unk20AA == 0 || hero.unk20A5 != 0) {
                            if (controller_state.held & 0xA) {
                                if (((unsigned char *)hero.moby)[0x53] != 0xD) {
                                    set_hero_animation(0xD, 0, scale_game_frames(8));
                                }
                            } else {
                                int a = get_hero_idle_animation_index(1);
                                if (((unsigned char *)hero.moby)[0x53] != a) {
                                    set_hero_animation(a, 0, scale_game_frames(8));
                                }
                            }
                        }
                    }
                    {
                        unsigned char *m = ((unsigned char *)hero.items[0].moby);
                        int            ok = 1;
                        if (m != 0 && is_wrench_moby((char *)m) && m[0x20] != 0) {
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
                        request_hero_idle_state_for_vehicle_mode();
                        break;
                    }
                    if (controller_state.held & 0xA) {
                        hero_set_state(HERO_STATE_CROUCH, 1);
                        break;
                    }
                    hero_set_state(HERO_STATE_IDLE, 0);
                    {
                        int a = get_hero_idle_animation_index(1);
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
                        int slam_intercept_frames =
                            truncate_float_to_s32(predict_hero_trajectory_intercept_time(
                                60.0f, frame_time_sq * 130.0f, -1.0f));
                        int n = 0x1A;
                        if (hero.slam_descending == 0.0f) {
                            hero.anim_speed = 0.33f;
                        } else if (hero.slam_descending != 0.0f && hero.air_frames.s != 0 &&
                                   slam_intercept_frames != 0 &&
                                   (float)n - hero.anim_frame > 1.0f) {
                            adjust_hero_animation_speed_for_target_frame(
                                n, slam_intercept_frames, 0.5f, -1.0f);
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
                        HeroTableRow2C *rec = &D_L02_0017C2A8_rec[hero.attack_record];
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
                                stick_target(1.0f, 0);
                                if (try_start_hero_jump(t, 1)) {
                                    break;
                                }
                                if (hero.state.step <= 0) {
                                    hero_set_state(HERO_STATE_JUMP, 1);
                                    break;
                                }
                                if (hero.target_speed > D_L02_0017C3B8.fC &&
                                    fast_difference_between_rotations(
                                        hero.motion.move_yaw, hero.motion.rot.f[2]) < 1.0471976f &&
                                    D_L02_0017C3B8.f18 * frame_time * 0.85f <
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
                    if (hero.state.current == HERO_STATE_RUN && hero.unk308 != 0) {
                        hero_set_state(0x3F, 1);
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
                                    if (get_hero_idle_animation_index(0) == 0x54) {
                                        set_hero_animation(get_hero_idle_animation_index(0),
                                                           0,
                                                           scale_game_frames(0x12));
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
                            if (get_hero_idle_animation_index(0) == 0x54) {
                                set_hero_animation(
                                    get_hero_idle_animation_index(0), 0, scale_game_frames(0x12));
                            } else {
                                set_hero_animation(get_hero_idle_animation_index(0), 0, -1.0f);
                            }
                            break;
                        }
                        update_hero_animation_for_movement_speed();
                        break;
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
                                get_hero_idle_animation_index(0), 0, scale_game_frames(0xF));
                        }
                    }
                    break;
                case HERO_STATE_ATTACK_RECORD_FRAME_EXIT_WAIT:
                    if (hero.anim_frame > (float)D_L02_0017C2A8_rec[hero.attack_record].f18) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
                    break;
                case HERO_STATE_ANIMATION_FRAME_EXIT_WAIT:
                    if (25.0f < hero.anim_frame) {
                        hero_set_state(HERO_STATE_IDLE, 1);
                    }
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
                    if (!(hero.stick_magnitude > 0.2f)) {
                        break;
                    }
                    if (get_hero_idle_animation_index(0) != 0x54) {
                        if (hero.ground_speed == 0.0f) {
                            hero_set_state(HERO_STATE_IDLE, 0);
                            break;
                        }
                    } else if (hero.anim_frame > 10.0f) {
                        if (hero_set_state(HERO_STATE_IDLE, 0)) {
                            set_hero_animation(
                                get_hero_idle_animation_index(0), 0, scale_game_frames(0x11));
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
                        set_hero_animation(get_hero_idle_animation_index(0), 0, -2.0f);
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
                        set_hero_animation(
                            get_hero_idle_animation_index(0), 0, scale_game_frames(0x1E));
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
                        int rise_fall_intercept_frames =
                            truncate_float_to_s32(predict_hero_trajectory_intercept_time(
                                60.0f, hero.rise_fall_gravity, -1.0f));
                        hero.predicted_land_frames = rise_fall_intercept_frames;
                        if (hero.past_apex != 0.0f && hero.frames_off_ground.s != 0 &&
                            rise_fall_intercept_frames != 0) {
                            if (rise_fall_intercept_frames == 2) {
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
                case HERO_STATE_LEDGE_HANG_ENTRY:
                    if (13.5f < hero.anim_frame) {
                        hero_set_state(0x19, 0);
                    }
                    break;
                case HERO_STATE_LEDGE_HANG:
                    if ((hero.anim_flags & 2) && ((unsigned char *)hero.moby)[0x53] == 0x20) {
                        set_hero_animation(0x21, 0, scale_game_frames(6));
                    }
                    stick_target(1.0f, 0);
                    qcopy(ledge_probe_position, hero.motion.pos.f);
                    current_position_probe_yaw = hero.motion.rot.f[2];
                    if (test_hero_ledge_ahead(ledge_probe_position, &current_position_probe_yaw) ==
                        0) {
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
                        qcopy(lateral_ledge_probe_position, hero.motion.pos.f);
                        lateral_ledge_probe_position[0] +=
                            fast_cos(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
                        lateral_ledge_probe_position[1] +=
                            fast_sin(fast_add_rotations(hero.motion.rot.f[2], 1.5707964f)) * 0.3f;
                        positive_lateral_probe_yaw = hero.motion.rot.f[2];
                        if (test_hero_ledge_ahead(lateral_ledge_probe_position,
                                                  &positive_lateral_probe_yaw)) {
                            hero_set_state(0x1A, 1);
                        }
                        break;
                    }
                    if (0.5f < hero.stick_magnitude &&
                        fast_difference_between_rotations(
                            hero.motion.move_yaw,
                            fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) < 0.7853982f) {
                        qcopy(lateral_ledge_probe_position, hero.motion.pos.f);
                        lateral_ledge_probe_position[0] +=
                            fast_cos(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
                        lateral_ledge_probe_position[1] +=
                            fast_sin(fast_add_rotations(hero.motion.rot.f[2], -1.5707964f)) * 0.3f;
                        negative_lateral_probe_yaw = hero.motion.rot.f[2];
                        if (test_hero_ledge_ahead(lateral_ledge_probe_position,
                                                  &negative_lateral_probe_yaw)) {
                            hero_set_state(0x1B, 1);
                        }
                    }
                    break;
                case HERO_STATE_LEDGE_SHIMMY_POSITIVE_YAW:
                case HERO_STATE_LEDGE_SHIMMY_NEGATIVE_YAW: {
                    float lateral_probe_yaw_offset;

                    qcopy(ledge_probe_position, hero.motion.pos.f);
                    lateral_probe_yaw_offset = 1.5707964f;
                    if (hero.state.current == 0x1B) {
                        lateral_probe_yaw_offset = -1.5707964f;
                    }
                    ledge_probe_position[0] +=
                        fast_cos(
                            fast_add_rotations(hero.motion.rot.f[2], lateral_probe_yaw_offset)) *
                        0.3f;
                    ledge_probe_position[1] +=
                        fast_sin(
                            fast_add_rotations(hero.motion.rot.f[2], lateral_probe_yaw_offset)) *
                        0.3f;
                    lateral_step_probe_yaw = hero.motion.rot.f[2];
                    stick_target(1.0f, 0);
                    if (hero.stick_magnitude < 0.5f ||
                        0.7853982f < fast_difference_between_rotations(
                                         hero.motion.move_yaw,
                                         fast_add_rotations(hero.motion.rot.f[2],
                                                            lateral_probe_yaw_offset)) ||
                        test_hero_ledge_ahead(ledge_probe_position, &lateral_step_probe_yaw) == 0) {
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

                        request_hero_voice_if_enabled(hero.ground_sound_class, 0, 1);
                        request_hero_voice_if_enabled(hero.ground_sound_class, 1, 1);
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
                        first = ((int)hero.ground_moby) == 0;
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
                    if ((controller_state.held & 0x40) && D_0013D4DC[-0x1A] != 0 &&
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
                    set_hero_animation(get_hero_idle_animation_index(0), 0, -2.0f);
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
                    stick_target(1.0f, 0);
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
                    if (hero.target_speed > D_L02_0017C3B8.fC &&
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
                    if (D_L02_0017C3B8.fC < hero.target_speed) {
                        if (hero.on_ice_ground != 0 || hero.wading != 0) {
                            hero_set_state(HERO_STATE_RUN, 1);
                            break;
                        }
                        hero_set_state(HERO_STATE_RUN, 0);
                        if (hero.state.current == HERO_STATE_RUN) {
                            hero.state.step = 1;
                            set_hero_animation(4, D_L02_0015F6DC_d, scale_game_frames(0xC));
                        }
                        configure_hero_point_effect_emission(0.016f, 0.002f, 5, 2, 1, 0);
                        break;
                    }
                    if (hero.air_frames.s == 0) {
                        if (hero_set_state(HERO_STATE_IDLE, 0) == 0) {
                            break;
                        }
                        set_hero_animation(get_hero_idle_animation_index(0), 0, -2.0f);
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
                        if (hero_slot_item_id(3) == 2 && D_0013D4DC[-0x1A] != 0 &&
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
                            float displacement_length = fast_vec_length(hero.motion.displacement.f);
                            if (displacement_length <
                                    fast_vec_length(hero.motion.velocity.f) * 0.5f &&
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
                                                      &predicted_trajectory_steps,
                                                      hero.jump_gravity);
                            adjust_hero_animation_speed_for_target_frame(
                                hero.apex_anim_frame,
                                predicted_trajectory_steps,
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
                                    int jump_intercept_frames = truncate_float_to_s32(
                                        predict_hero_trajectory_intercept_time(
                                            60.0f, hero.jump_gravity, -1.0f));
                                    hero.frames_to_land = jump_intercept_frames;
                                    adjust_hero_animation_speed_for_target_frame(
                                        hero.target_anim_frame,
                                        jump_intercept_frames,
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
                                D_L02_0017C3B8.f18 * frame_time * 0.1f > hero.target_speed &&
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
                                    if (hero.target_speed > D_L02_0017C3B8.fC &&
                                        fast_difference_between_rotations(hero.motion.move_yaw,
                                                                          hero.motion.rot.f[2]) <
                                            1.0471976f &&
                                        D_L02_0017C3B8.f18 * frame_time * 0.85f <
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
                        select_hero_locomotion_state();
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
                                if (pad_pressed_within(0x40, a, &jump_press_age_frames)) {
                                    if (jump_press_age_frames < hero.state_timer) {
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
                                approach_value(&hero.anim_speed, 1.3f, frame_scale_gp * 0.1f);
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
                                } else if (D_0013D4DC[-0x1A] != 0 && hero_slot_item_id(3) != 4 &&
                                           hero.pack_disabled == 0 &&
                                           (controller_state.held & 0x40) &&
                                           delay < hero.state_timer &&
                                           hero.unk4A6 < hero.state_timer) {
                                    float minimum_ground_clearance = 0.6f;
                                    if (hero.state.current == HERO_STATE_RUN_JUMP ||
                                        hero.state.current == HERO_STATE_JUMP) {
                                        minimum_ground_clearance = 1.0f;
                                    }
                                    if (hero.state.current == 0xE) {
                                        minimum_ground_clearance = 1.5f;
                                    }
                                    if (minimum_ground_clearance < hero.ground_distance) {
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
                    if (hero.state_timer > scale_game_frames(0x1B)) {
                        if (hero.health.hp <= 0) {
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
                    stick_target(1.0f, 0);
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
                                int a = get_hero_idle_animation_index(1);
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
                        a = get_hero_idle_animation_index(1);
                        if (((unsigned char *)hero.moby)[0x53] != a) {
                            set_hero_animation(a, 0, scale_game_frames(8));
                        }
                    }
                    D_L02_0016CD80 = 0;
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

void enter_hero_idle_or_stop(void) __asm__("FUN_L02_00237478");

void enter_hero_idle_or_stop(void) {
    if (hero.motion.speed_xy < frame_time * 2.7f) {
        hero_set_state(HERO_STATE_IDLE, 1);
    } else {
        hero_set_state(HERO_STATE_STOP, 1);
    }
}
