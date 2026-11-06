/* Level 14 hero state changes: hero_set_state. */
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
#include "rnc/overlay/shared/hero/hero_update_hop_movement.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/hero/hero_tables.h"
#include "rnc/overlay/shared/hero/jump_velocity_keys.h"

/* Ported from rac1-decomp (src/overlays/l14_oltanis/help_002306C8.c: func_L14_002306C8), where it is exact; names translated to the US level program. */

extern HeroTableRow2C D_L14_0017C328[];
extern char          *hero_slot_moby(int) __asm__("FUN_L00_0020d460");
extern float          level14_hero_animation_blend_curves[][25] __asm__("D_L14_0017C4F0");
extern float          get_hero_yaw_history(int back) __asm__("FUN_L00_0020d658");
extern float          gravity_length_xy(float *v) __asm__("FUN_L00_002339d0");
extern float          vector_length_xy(void *);
extern int            D_L14_0015F5C4;
extern int            hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");
extern int            is_wrench_moby(char *) __asm__("FUN_L00_00211360");
extern int            stick_direction_vs_facing(void) __asm__("FUN_L00_00221df8");
extern int
sample_path(int *tab, float *out, int *a, float *b, int c, float d) __asm__("FUN_L00_0025d808");

struct HeroVelocityKey D_L14_0015F6E8[1] __attribute__((section(".sdata"))) = {
    {48.0f, 0.0f, 1000}
};

extern short D_L14_0015F688 __attribute__((sda));
int          D_L14_0015F6D8[4] __attribute__((section(".sdata"))) = {8, 0, 11, 16};

extern unsigned char l14_run_jump_animation_ids_by_frame[] __asm__("D_L14_0017C4D8");
extern void          hero_change_health(int n) __asm__("FUN_L00_002050c0");
extern void          clear_hero_idle_rotation_targets(void) __asm__("FUN_L00_0020a500");
extern void          exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");
extern void create_missing_hero_item_slot_animations(int a, int b) __asm__("FUN_L00_0020e360");
extern void scale_hero_motion(float) __asm__("FUN_L00_00213de8");
extern void update_hero_target_heading(float a, float b, float c) __asm__("FUN_L00_00214ed8");
extern void hero_queue_sequence(int bank, int seq) __asm__("FUN_L00_002325e0");
extern void FUN_L00_0025f730(void *a, float x);
extern void create_moby_transform_history_entry(char *o, int a, int b) __asm__("FUN_L00_00262528");
extern void clear_moby_transform_history(void *o) __asm__("FUN_L00_00262840");

/* Sets hero.state.current to a: moves state, control_mode and state_timer into the prev_* fields, zeroes state_step, then sets up the new state; returns 0 when the change is refused. */
extern int             hero_set_state(int, int) __asm__("FUN_L14_0022ff58");
struct HeroVelocityKey level14_transform_trail_jump_velocity_keys
    [HERO_TRANSFORM_TRAIL_JUMP_VELOCITY_KEYS_COUNT] __asm__("D_L14_0017C620") =
        HERO_TRANSFORM_TRAIL_JUMP_VELOCITY_KEYS;

int hero_set_state(int requested_state, int animation_update_mode) {
    int   saved_state_step;
    char *moby;
    int   old20A7;
    float grind_direction_or_jump_reference_position[4];
    {
        struct Hero *p = &hero;
        if (p->state.control_mode == HERO_MODE_DYING &&
            (requested_state != HERO_STATE_IDLE_ANIMATION_WITH_DYING_OVERRIDE ||
             animation_update_mode != HERO_ANIMATION_ENTRY_OVERRIDE_GUARDS))
            return 0;
    }
    if ((D_L14_0015F5C4 == 2 || D_L14_0015F5C4 == 6) &&
        animation_update_mode != HERO_ANIMATION_ENTRY_OVERRIDE_GUARDS)
        return 0;
    {
        struct Hero *p = &hero;
        if (p->health.hp == 0 &&
            (p->state.control_mode == HERO_MODE_FALL || p->state.control_mode == HERO_MODE_HURT) &&
            requested_state != HERO_STATE_FALL && requested_state != HERO_STATE_DYING &&
            requested_state != HERO_STATE_VEHICLE_MODE_1_DYING &&
            requested_state != HERO_STATE_VEHICLE_MODE_3_DYING &&
            requested_state != HERO_STATE_VEHICLE_MODE_2_DYING &&
            requested_state != HERO_STATE_FATAL_FALL &&
            requested_state != HERO_STATE_HEIGHT_OR_TIMEOUT_RESPAWN_WAIT &&
            requested_state != HERO_STATE_DEATH_DESCENT && requested_state != HERO_STATE_SINK &&
            requested_state != HERO_STATE_DROWN &&
            requested_state != HERO_STATE_DYING_SPEED_LIMITED &&
            requested_state != HERO_STATE_DYING_ZERO_HEALTH)
            return 0;
    }
    {
        struct Hero *p = &hero;
        saved_state_step = p->state.step;
        moby = ((char *)p->moby);
        old20A7 = p->item_select_blocked;
        p->state.prev2 = p->state.prev;
        p->state.prev2_control_mode = p->state.prev_control_mode;
        p->state.prev_timer = p->state_timer;
        p->state.prev = p->state.current;
        p->state.prev_control_mode = p->state.control_mode;
        p->state.current = requested_state;
        p->state.step = 0;
        p->hidden = 0;
        p->item_use_locked = 0;
        if (p->vehicle_mode == HERO_VEHICLE_MODE_ON_FOOT) {
            MOBY(moby)->collision_disable = 0;
            p->tilt_disabled = 0;
            p->item_select_blocked = 0;
            p->on_hoverboard = 0;
            p->hand_item_hidden = 0;
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
    switch (requested_state) {
    case HERO_STATE_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_IDLE;
        p->camera_mode = 0;
        p->head_look_timer = random_int_between(scale_game_frames(0x32), scale_game_frames(0x64));
        clear_hero_idle_rotation_targets();
        if (p->vehicle_mode != HERO_VEHICLE_MODE_ON_FOOT) {
            request_hero_idle_state_for_vehicle_mode();
            return 1;
        }
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (p->on_ice_ground != 0 && vector_length_xy(&p->motion.momentum) > frame_time * 0.5f) {
            hero_set_state(0x2F, 1);
            return 0;
        }
        {
            struct Hero *r = &hero;
            if (r->on_slip_ground != 0 && r->frames_off_ground.s == 0) {
                hero_set_state(HERO_STATE_SLIP, 1);
                return 0;
            }
        }
        {
            struct Hero *q = &hero;
            q->rand_timer.range = 0x68;
        }
        if (animation_update_mode) {
            if (hero_get_gadget_anim_id(0) == 0x54) {
                set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(0x12));
            } else {
                set_hero_animation(hero_get_gadget_anim_id(0), 0, -1.0f);
            }
        }
        break;
    }
    case HERO_STATE_AT_TARGET_POSE: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_IDLE;
        p->camera_mode = 0;
        clear_u64_value(&p->motion.momentum);
        p->rand_timer.range = 0x68;
        if (animation_update_mode) {
            if (hero_get_gadget_anim_id(0) == 0x54) {
                set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(0x12));
            } else {
                set_hero_animation(hero_get_gadget_anim_id(0), 0, -1.0f);
            }
        }
        break;
    }
    case HERO_STATE_FIDGET_ALT: {
        int t = scale_game_frames(9);
        set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)t);
        {
            struct Hero *p = &hero;
            if (*(void **)&p->fidget_prop_moby != 0) {
                blend_moby_animation(*(void **)&p->fidget_prop_moby, 0, 0, t);
            }
        }
        break;
    }
    case HERO_STATE_DYING: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_DYING;
        p->camera_mode = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (animation_update_mode)
            set_hero_animation(0x45, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_DYING_SPEED_LIMITED: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_DYING;
        p->camera_mode = 0;
        p->health.hp = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        FUN_L00_0025f730(&p->motion.momentum, frame_time * 3.5f);
        if (animation_update_mode)
            set_hero_animation(0x7C, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_SINK: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_DYING;
        p->camera_mode = 0;
        p->health.hp = 0;
        p->swim_bob_velocity = p->motion.displacement.f[2] * 0.2f;
        p->swim_water_level = p->water_surface_z - 0.37f;
        if (p->swim_bob_velocity < frame_time * -7.0f)
            p->swim_bob_velocity = frame_time * -7.0f;
        p->item_select_blocked = 1;
        p->unk6A0 = 0;
        p->swim_bob_offset = p->motion.pos.f[2] - p->swim_water_level;
        if (animation_update_mode)
            set_hero_animation(0x74, 0, (float)scale_game_frames(0xB));
        break;
    }
    case HERO_STATE_LOOK: {
        struct Hero *p = &hero;
        int          t;
        p->state.control_mode = HERO_MODE_IDLE;
        p->camera_mode = 4;
        *(int *)&D_L14_0015F688 = scale_game_frames(0x3C);
        if (p->weapon_up == 0) {
            t = hero_get_gadget_anim_id(1);
            if (p->moby->prev_seq != t && animation_update_mode) {
                set_hero_animation(t, 0, (float)scale_game_frames(0x12));
            }
        }
        break;
    }
    case HERO_STATE_ITEM_BUTTON_RELEASE_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_ITEM_RELEASE_WAIT;
        p->camera_mode = 0;
        qcopy(&p->motion.momentum, &p->motion.displacement);
        set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(0xA));
        p->idle_return_delay = scale_game_frames(7);
        break;
    }
    case HERO_STATE_WAIT_FOR_ITEM_RELEASE_THEN_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_ITEM_RELEASE_WAIT;
        p->camera_mode = 0;
        qcopy(&p->motion.momentum, &p->motion.displacement);
        set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(5));
        p->idle_return_delay = scale_game_frames(7);
        break;
    }
    case HERO_STATE_ITEM_BUTTON_HOLD_ANIMATION_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_RUN;
        p->camera_mode = 0;
        p->ground_speed = vector_length_xy(&p->motion.displacement);
        p->state.step = 0;
        p->move_along_stick = 0;
        p->run_start_blending = 0;
        set_hero_animation(0x2E, 0, (float)scale_game_frames(5));
        break;
    }
    case HERO_STATE_RUN:
    case HERO_STATE_RUN_CLEAR_VELOCITY_Z:
    case HERO_STATE_DIRECTION_LIMITED_GROUND_MOVE:
    case HERO_STATE_SHALLOW_WATER_RUN: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_RUN;
        p->camera_mode = 0;
        if (p->state.prev_control_mode != HERO_MODE_JUMP &&
            p->state.prev2_control_mode != HERO_MODE_JUMP) {
            p->ground_speed = vector_length_xy(&p->motion.resolved_velocity);
        }
        {
            struct Hero *q = &hero;
            if (frame_time * 7.0f < q->ground_speed)
                q->ground_speed = frame_time * 7.0f;
            q->sharp_turn = 0;
            stick_target(1.0f, 0);
            if (1.5707964f <
                fast_difference_between_rotations(q->motion.rot.f[2], q->motion.move_yaw)) {
                if (q->state.prev_control_mode == HERO_MODE_HURT || q->state.prev == 3 ||
                    q->state.prev2 == 3) {
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
            if (t->on_ice_ground != 0 || t->state.current == 0x2F) {
                if (frame_time * 7.7f < t->ground_speed)
                    t->ground_speed = frame_time * 7.7f;
                t->motion.velocity.f[2] = 0.0f;
                FUN_L00_0025f730(&t->motion.velocity, frame_time * 7.7f);
                t->prev_ground_pitch.i = 0;
                t->state.current = 0x2F;
                set_hero_animation(0x37, 0, (float)scale_game_frames(8));
            } else if (t->magnetic_floor == 1 || t->state.current == 0x3F) {
                t->state.current = 0x3F;
                set_hero_animation(0x5B, 0, (float)scale_game_frames(8));
            } else {
                int n;
                t->state.step = 0;
                t->move_along_stick = 0;
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
                    if (u->state.prev2_control_mode == HERO_MODE_JUMP &&
                        scale_game_frames(5) < u->rail_land_frames)
                        n = scale_game_frames(0xD);
                }
                if (animation_update_mode) {
                    struct Hero *v = &hero;
                    set_hero_animation(v->state.step + 3, D_L14_0015F6D8[v->state.step], (float)n);
                }
            }
        }
        break;
    }
    case HERO_STATE_ANIMATION_FRAME_EXIT_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_RUN;
        p->camera_mode = 0;
        if (animation_update_mode) {
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
        p->state.control_mode = HERO_MODE_RUN;
        p->camera_mode = 0;
        p->ground_speed = vector_length_xy(&p->motion.resolved_velocity);
        p->item_use_locked = 1;
        if (p->vehicle_mode == HERO_VEHICLE_MODE_ON_FOOT)
            cancel_hero_item_animation();
        if (animation_update_mode) {
            if (p->vehicle_mode == 3) {
                set_hero_animation(1, 0, (float)scale_game_frames(8));
            } else if (p->vehicle_mode == HERO_VEHICLE_MODE_CLANK) {
                set_hero_animation(2, 0, (float)scale_game_frames(8));
            } else if (p->vehicle_mode == HERO_VEHICLE_MODE_ON_FOOT) {
                set_hero_animation(
                    p->state.step + 3, D_L14_0015F6D8[p->state.step], (float)scale_game_frames(8));
            }
        }
        break;
    }
    case HERO_STATE_STOP: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_RUN;
        p->camera_mode = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (p->on_ice_ground != 0) {
            hero_set_state(0x2F, 1);
            return 0;
        }
        if (p->on_slip_ground != 0 && p->frames_off_ground.s == 0) {
            hero_set_state(HERO_STATE_SLIP, 1);
            return 0;
        }
        if (try_start_hero_item_idle_animation())
            return 0;
        if (hero_get_gadget_anim_id(0) == 0x54) {
            if (hero_set_state(HERO_STATE_IDLE, 0)) {
                set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(0x12));
            }
            return 0;
        }
        if (animation_update_mode) {
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
        p->state.control_mode = HERO_MODE_RUN;
        p->camera_mode = 0;
        p->ground_speed = gravity_length_xy((float *)v);
        qcopy(&p->motion.momentum, v);
        if (animation_update_mode) {
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
        p->state.control_mode = HERO_MODE_CROUCH;
        p->camera_mode = 0;
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
            if (animation_update_mode) {
                if (q->moby->seq == hero_get_gadget_anim_id(0)) {
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
        p->state.control_mode = HERO_MODE_GRAPPLE;
        p->camera_mode = 7;
        {
            struct UsageStat *h = (struct UsageStat *)item_usage;
            if (h[12].count <= 0xFFFE) {
                h[12].count = h[12].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > h[12].latest_time_bucket) {
                h[12].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
            h[12].level_mask = h[12].level_mask | (1 << current_level_index) | 0x80000000;
        }
        p->motion.move_yaw = fast_arc_tan(p->grapple_target->pos.x - p->motion.pos.f[0],
                                          p->grapple_target->pos.y - p->motion.pos.f[1]);
        if (animation_update_mode) {
            set_hero_animation(0x2F, 2, (float)scale_game_frames(5));
            hero_queue_sequence(0xA, 0xE);
        }
        break;
    }
    case HERO_STATE_APPROACH_TARGET_MOBY: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_GRAPPLE;
        p->camera_mode = 7;
        *(int *)&p->grapple_pull_speed = 0;
        p->grapple_pull_ending = 0;
        p->item_use_locked = 1;
        if (animation_update_mode)
            set_hero_animation(0x30, 1, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_CONSTRAIN_TO_TARGET_RADIUS: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_GRAPPLE;
        p->camera_mode = 7;
        p->item_use_locked = 1;
        *(int *)&p->grapple_pull_speed = 0;
        p->grapple_pull_ending = 0;
        p->tether_radius = fast_vec_dist(&p->motion.pos, &p->grapple_target->pos);
        if (animation_update_mode)
            set_hero_animation(0x30, 1, (float)scale_game_frames(5));
        break;
    }
    case HERO_STATE_GRAPPLE_SWING_ENTRY: {
        struct Hero *p = &hero;
        char        *d;
        p->state.control_mode = HERO_MODE_GRAPPLE_SWING;
        p->camera_mode = 7;
        {
            struct UsageStat *h = (struct UsageStat *)item_usage;
            if (h[12].count <= 0xFFFE) {
                h[12].count = h[12].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > h[12].latest_time_bucket) {
                h[12].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
            h[12].level_mask = h[12].level_mask | (1 << current_level_index) | 0x80000000;
        }
        p->swing_entry_push = 1;
        p->swing_length_vel = 0.0f;
        p->item_use_locked = 1;
        *(char **)&p->swing_target_moby = ((char *)p->swing_target);
        p->swing_hooked = 0;
        p->swing_anim_dir = 0;
        *(int *)&p->swing_push_speed = 0;
        *(int *)&p->swing_yaw_vel = 0;
        d = *(char **)(((char *)p->swing_target_moby) + 0x78);
        p->swing_rope_target_length = *(float *)(d + 4);
        if (*(float *)(d + 0x10) == p->swing_length_vel) {
            *(float *)(d + 0x14) = *(float *)(d + 0xC);
        } else {
            *(float *)(d + 0x14) = *(float *)(d + 0x10);
        }
        {
            struct Hero *q = &hero;
            q->swing_rope_spring_k = frame_scale_sq * 0.025f;
            q->swing_rope_damping = frame_scale_sq * 0.33f;
            q->swing_rope_max_speed = frame_time * 11.0f;
            clear_u64_value(&q->motion.rot_spring_vel);
            q->grapple_lockout_timer = scale_game_frames(0x28);
            if (*(float *)(d + 0x24) != 0.0f)
                q->swing_rope_spring_k = *(float *)(d + 0x24);
            if (*(float *)(d + 0x28) != 0.0f)
                q->swing_rope_damping = *(float *)(d + 0x28);
            if (*(float *)(d + 0x2C) != 0.0f)
                q->swing_rope_max_speed = *(float *)(d + 0x2C);
            q->swing_yaw = fast_arc_tan(q->swing_target_moby->pos.x - q->motion.pos.f[0],
                                        q->swing_target_moby->pos.y - q->motion.pos.f[1]);
            q->swing_rope_length = fast_vec_dist(&q->motion.pos, &q->swing_target_moby->pos);
            q->swing_gravity = frame_time_sq * 27.0f;
            if (q->state.prev_control_mode == HERO_MODE_JUMP) {
                q->swing_gravity = q->jump_gravity;
            } else if (q->state.prev_control_mode == HERO_MODE_FALL) {
                q->swing_gravity = q->fall_state_gravity;
            }
            if (animation_update_mode) {
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
        p->state.control_mode = HERO_MODE_FALL;
        p->camera_mode = 7;
        clear_u64_value(&p->motion.rot_spring_vel);
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (animation_update_mode)
            set_hero_animation(0xB, 0, (float)scale_game_frames(0xF));
        break;
    }
    case HERO_STATE_RISE_THEN_FALL_WITH_AIRBORNE_HIT_QUERY: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xB;
        p->rise_fall_target_frame = 39.0f;
        p->camera_mode = 0x50;
        *(int *)&p->past_apex = 0;
        p->rise_fall_land_frames = 0;
        p->rise_fall_gravity = frame_time_sq * 100.0f;
        if (animation_update_mode)
            set_hero_animation(0x2A, 0, (float)scale_game_frames(6));
        break;
    }
    case HERO_STATE_GRIND: {
        struct Hero *p = &hero;
        float        pos[4];
        int          seg;
        float        t;
        p->state.control_mode = HERO_MODE_GRIND;
        p->camera_mode = 3;
        p->rand_timer.range = 0x68;
        p->rail_off_frames = 0;
        p->grind_direction = 1;
        p->grind_yaw_offset = -1.5707964f;
        p->grind_anim_variant = 0;
        if (p->state.prev_control_mode != HERO_MODE_GRIND) {
            p->grind_pose = -1;
            p->grind_speed_boost.i = 0;
            p->grind_base_speed = 0.0f;
        }
        p->rail_swing_target = 0;
        p->rail_jump_armed = 0;
        p->rail_jump_trigger = 0;
        p->rail_jump_queued = 0;
        seg = p->rail_segment;
        t = p->rail_seg_frac;
        sample_path(*(int **)&p->rail_spline, pos, &seg, &t, p->rail_spline_loops, 1.0f);
        fast_vec_sub(grind_direction_or_jump_reference_position, pos, &p->motion.pos);
        if (0.001f < fast_vec_length(grind_direction_or_jump_reference_position)) {
            fast_vec_normalize(grind_direction_or_jump_reference_position,
                               grind_direction_or_jump_reference_position,
                               1.0f);
            if (p->state.prev_control_mode != HERO_MODE_GRIND) {
                p->grind_base_speed = fast_vec_dot(&p->motion.displacement,
                                                   grind_direction_or_jump_reference_position);
            }
        }
        {
            struct Hero *q = &hero;
            if (((char *)q->grind_look_moby) != 0) {
                float ang = fast_arc_tan(q->grind_look_moby->pos.x - q->motion.pos.f[0],
                                         q->grind_look_moby->pos.y - q->motion.pos.f[1]);
                if (1.5707964f <
                    fast_difference_between_rotations(
                        ang,
                        fast_add_rotations(
                            q->grind_yaw_offset,
                            fast_arc_tan(grind_direction_or_jump_reference_position[0],
                                         grind_direction_or_jump_reference_position[1])))) {
                    q->grind_yaw_offset = 1.5707964f;
                    q->grind_anim_variant = 1;
                }
            }
        }
        if (animation_update_mode) {
            struct Hero *r = &hero;
            set_hero_animation(r->grind_anim_variant + 0x31, 0, (float)scale_game_frames(6));
        }
        break;
    }
    case HERO_STATE_LEDGE_HANG_ENTRY: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_LEDGE;
        p->camera_mode = 0xD;
        p->ledge_gravity = frame_time_sq * 24.0f;
        p->smoothed_air_z = p->motion.pos.f[2];
        if (p->state.prev_control_mode == HERO_MODE_JUMP) {
            p->ledge_gravity = p->jump_gravity;
            p->smoothed_air_z = p->smoothed_ground_z;
        }
        p->item_select_blocked = 1;
        if (animation_update_mode) {
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
        p->state.control_mode = HERO_MODE_LEDGE;
        p->camera_mode = 0xD;
        p->item_select_blocked = 1;
        p->rand_timer.range = 0x68;
        if (animation_update_mode)
            set_hero_animation(0x21, 0, (float)scale_game_frames(6));
        break;
    }
    case HERO_STATE_LEDGE_SHIMMY_POSITIVE_YAW: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_LEDGE;
        p->camera_mode = 0xD;
        p->item_select_blocked = 1;
        if (animation_update_mode)
            set_hero_animation(0x24, 0, (float)scale_game_frames(6));
        break;
    }
    case HERO_STATE_LEDGE_SHIMMY_NEGATIVE_YAW: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_LEDGE;
        p->camera_mode = 0xD;
        p->item_select_blocked = 1;
        if (animation_update_mode)
            set_hero_animation(0x25, 0, (float)scale_game_frames(6));
        break;
    }
    case HERO_STATE_SLIP: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_FALL;
        p->camera_mode = 0;
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (animation_update_mode)
            set_hero_animation(0xB, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_FALL:
    case HERO_STATE_FATAL_FALL: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_FALL;
        p->camera_mode = 0;
        p->fall_state_gravity = frame_time_sq * 24.0f;
        p->stickless_decel = frame_time_sq * 7.0f;
        if (p->state.current == 0x77) {
            if (p->vehicle_mode == HERO_VEHICLE_MODE_ON_FOOT) {
                p->health.fall_voice_slot = allocate_hero_voice_for_entry(0x17, 0x20);
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
            if (animation_update_mode) {
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
        int          animation_timing_preset_selector = -2;
        p->state.control_mode = HERO_MODE_RUN;
        p->hover_latch = 1;
        p->rand_timer.range = 0x68;
        p->camera_mode = 0;
        if (frame_time * 4.5f < p->motion.speed_xy) {
            scale_hero_motion(frame_time * 4.5f / p->motion.speed_xy);
        }
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (animation_update_mode) {
            set_hero_animation(0x13, 0, (float)animation_timing_preset_selector);
        }
        break;
    }
    case HERO_STATE_LAND: {
        struct Hero *p = &hero;
        int          animation_timing_selector = -2;
        p->state.control_mode = HERO_MODE_LAND;
        p->rand_timer.range = 0x68;
        p->camera_mode = 0;
        if (frame_time * 4.5f < p->motion.speed_xy) {
            scale_hero_motion(frame_time * 4.5f / p->motion.speed_xy);
        }
        qcopy(&p->motion.velocity, &p->motion.displacement);
        if (animation_update_mode) {
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
            p->state.step = saved_state_step;
            p->state_timer = p->state.prev_timer;
            return 0;
        }
        p->state.control_mode = HERO_MODE_JUMP;
        if (requested_state >= HERO_STATE_GRIND_JUMP &&
            requested_state <= HERO_STATE_GRIND_RAIL_TRANSFER) {
            p->camera_mode = 3;
        } else {
            p->camera_mode = 0x50;
        }
        {
            char        *v = (char *)&hero.motion.velocity;
            struct Hero *hero_base;
            qcopy(v, v + 0x30);
            hero_base = (struct Hero *)(v - 0xE0);
            if (hero_base->state.current != 0x12) {
                if (hero_base->base_condition != 0 &&
                    hero_base->motion.pos.f[2] < hero_base->water_surface_z + 0.5f) {
                    emit_hero_surface_effect_burst(3, 0x10, 0);
                }
            }
        }
        {
            struct Hero *u = &hero;
            u->smoothed_ground_z = u->motion.pos.f[2];
            u->jump_gravity = frame_time_sq * 29.7f;
            u->velocity_key_index = -1;
            u->air_speed_step_up = frame_time_sq * 20.0f;
            u->jump_decel = frame_time_sq * 11.0f;
            u->took_off_from_ice = u->on_ice_ground;
            u->past_jump_apex = 0;
            u->rail_land_frames = 0;
            u->pending_jump_impulse = 0.0f;
            u->jump_impulse_total = 0.0f;
            u->velocity_keys_active = 0;
            u->fall_start_distance = 2.5f;
            u->jump_blocked_ahead = 0;
            u->can_use_item_in_jump = 0;
            u->can_chain_high_jump = 0;
            u->can_double_jump = 0;
            u->jump_max_frames = scale_game_frames(0x50);
            u->fall_delay_frames = scale_game_frames(0x46);
            u->pack_glide_delay = 0;
            u->jump_anim_retime_scale = 0.5f;
            u->air_stick_speed = frame_time * 5.7f;
            u->air_turn_rate = frame_time * 15.0098314f;
            u->fall_gravity = 0.0f;
            u->wall_jumped = 0;
            if (u->state.current == 7) {
                u->jump_height = 1.47f;
                u->takeoff_frame = scale_game_frames(5);
                u->air_capsule_bottom = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 30.0f;
                u->jump_land_anim_frame = 12.0f;
                u->jump_height_target = 2.62f;
                u->jump_height_start = 1.47f;
                u->jump_ramp_frames = scale_game_frames(0xF);
                u->can_use_item_in_jump = 1;
                u->can_chain_high_jump = 1;
                u->can_double_jump = 1;
                u->fall_gravity = u->jump_gravity * 1.17f;
            } else if (u->state.current == 0x29) {
                u->jump_height = 2.4f;
                u->takeoff_frame = scale_game_frames(5);
                u->air_capsule_bottom = 0.8f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 28.0f;
                u->jump_land_anim_frame = 14.0f;
                u->jump_height_target = 2.45f;
                u->jump_height_start = 2.4f;
                u->jump_ramp_frames = scale_game_frames(0x11);
                u->state.control_mode = HERO_MODE_GRIND;
                u->rail_gap_jump = 0;
                u->jump_gravity = frame_time_sq * 27.0f;
                if (u->selector_5 != 0) {
                    float k = 4.7f;
                    *(u128 *)grind_direction_or_jump_reference_position = 0;
                    u->jump_height = u->jump_height + k;
                    u->jump_height_start = u->jump_height_start + k;
                    u->jump_height_target = u->jump_height_target + k;
                    u->jump_gravity = frame_time_sq * 14.0f;
                    grind_direction_or_jump_reference_position[0] = 331.0f;
                    grind_direction_or_jump_reference_position[1] = 104.0f;
                    grind_direction_or_jump_reference_position[2] = 145.0f;
                    if (fast_vec_dist(&u->motion.pos, grind_direction_or_jump_reference_position) <
                        10.0f) {
                        k = 0.7f;
                        u->jump_height = u->jump_height + k;
                        u->jump_height_start = u->jump_height_start + k;
                        u->jump_height_target = u->jump_height_target + k;
                    }
                }
            } else if (u->state.current == 0x2A) {
                u->jump_height = 2.5f;
                u->takeoff_frame = scale_game_frames(2);
                u->air_capsule_bottom = 0.8f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 31.0f;
                u->jump_land_anim_frame = 14.0f;
                u->jump_height_target = 2.55f;
                u->jump_height_start = 2.5f;
                u->jump_ramp_frames = scale_game_frames(0xA);
                u->state.control_mode = HERO_MODE_GRIND;
                u->rail_gap_jump = 0;
                u->jump_gravity = frame_time_sq * 24.0f;
            } else if (u->state.current == 9) {
                u->jump_height = 1.0f;
                u->takeoff_frame = scale_game_frames(5);
                u->air_capsule_bottom = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 29.0f;
                u->jump_land_anim_frame = 11.0f;
                u->jump_height_target = 2.7f;
                u->jump_height_start = 1.0f;
                u->jump_ramp_frames = scale_game_frames(0xF);
                u->air_stick_speed = frame_time * 5.7f;
                u->can_use_item_in_jump = 1;
                u->can_chain_high_jump = 1;
                u->can_double_jump = 1;
            } else if (u->state.current == 0xA) {
                u->jump_height = 1.9f;
                u->takeoff_frame = scale_game_frames(5);
                u->air_capsule_bottom = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 29.0f;
                u->jump_land_anim_frame = 10.0f;
                u->jump_height_target = 1.95f;
                u->jump_height_start = 1.9f;
                u->jump_ramp_frames = scale_game_frames(0xE);
                u->can_chain_high_jump = 1;
                u->jump_gravity = frame_time_sq * 11.0f;
                u->jump_max_frames = scale_game_frames(0x78);
                u->fall_delay_frames = scale_game_frames(0x5A);
                u->pack_glide_delay = scale_game_frames(0x46);
            } else if (u->state.current == 0x10) {
                char *x = (char *)&u->trail;
                u->jump_height = 1.5f;
                u->takeoff_frame = scale_game_frames(5);
                u->air_capsule_bottom = 0.8f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 30.0f;
                u->jump_land_anim_frame = 11.0f;
                u->jump_height_target = 1.55f;
                u->jump_height_start = 1.5f;
                u->jump_ramp_frames = scale_game_frames(1);
                u->jump_height = 0.4f;
                u->jump_height_start = 0.4f;
                u->jump_height_target = 0.9f;
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
                u->jump_height = 2.0f;
                u->takeoff_frame = scale_game_frames(0xA);
                u->air_capsule_bottom = 0.85f;
                u->apex_anim_frame = 20.0f;
                u->target_anim_frame = 29.0f;
                u->jump_land_anim_frame = 19.0f;
                u->jump_height_target = 2.05f;
                u->jump_height_start = 2.0f;
                u->jump_ramp_frames = scale_game_frames(1);
                u->jump_anim_retime_scale = 1.0f;
                u->jump_gravity = frame_time_sq * 25.0f;
                u->ledge_jump_speed = 0;
                u->item_select_blocked = 1;
                u->can_chain_high_jump = 1;
            } else if (u->state.current == 0x11) {
                u->jump_height = 3.3f;
                u->takeoff_frame = scale_game_frames(9);
                u->air_capsule_bottom = 0.6f;
                u->apex_anim_frame = 12.0f;
                u->target_anim_frame = 20.0f;
                u->jump_land_anim_frame = 10.0f;
                u->jump_height_target = 3.35f;
                u->jump_height_start = 3.3f;
                u->jump_ramp_frames = scale_game_frames(0xC);
                u->fall_delay_frames = scale_game_frames(0x50);
                u->jump_gravity = frame_time_sq * 29.0f;
                qcopy(&u->wall_jump_normal, &u->wall_hit_normal);
                u->wall_jumped = 1;
                u->item_select_blocked = 1;
                u->can_use_item_in_jump = 1;
            } else if (u->state.current == 0xD) {
                char *x = (char *)&u->trail;
                u->jump_height = 0.1f;
                u->takeoff_frame = scale_game_frames(9);
                u->air_capsule_bottom = 0.6f;
                u->apex_anim_frame = 26.0f;
                u->target_anim_frame = 38.0f;
                u->jump_land_anim_frame = 18.0f;
                u->jump_height_target = 0.2f;
                u->jump_height_start = 0.1f;
                u->jump_ramp_frames = scale_game_frames(0xE);
                u->fall_start_distance = 4.5f;
                u->jump_max_frames = scale_game_frames(0x78);
                u->air_stick_speed = frame_time * 1.9f;
                initialize_moby_transform_history(*(int *)&u->moby, x);
                create_moby_transform_history_entry(x, 0x28, 3);
                create_moby_transform_history_entry(x, 0x14, 5);
                u->velocity_keys_active = 1;
                u->velocity_keys_start_frame = scale_game_frames(9);
                u->velocity_keys_end_frame = scale_game_frames(0x23) + u->velocity_keys_start_frame;
                u->unk3DC = 26.0f;
                u->keyed_move_target_frame = 0.0f;
                u->keyed_anim_target_frame = scale_game_frames(0x2D);
                u->velocity_keys = level14_transform_trail_jump_velocity_keys;
                u->can_chain_high_jump = 1;
            } else if (u->state.current == 0xF) {
                u->jump_height = 1.9f;
                u->takeoff_frame = scale_game_frames(9);
                u->air_capsule_bottom = 0.6f;
                u->apex_anim_frame = 50.0f;
                u->target_anim_frame = 64.0f;
                u->jump_land_anim_frame = 28.0f;
                u->jump_height_target = 1.95f;
                u->jump_height_start = 1.9f;
                u->jump_ramp_frames = scale_game_frames(5);
                u->fall_start_distance = 4.5f;
                u->jump_max_frames = scale_game_frames(0x82);
                u->air_stick_speed = frame_time * 3.3f;
                u->pack_glide_delay = scale_game_frames(0x55);
                u->ledge_probe_cooldown = scale_game_frames(0x46);
                u->velocity_keys_active = 1;
                if (pal_mode != 0) {
                    u->velocity_keys_start_frame = scale_game_frames(0x22);
                } else {
                    u->velocity_keys_start_frame = scale_game_frames(0x23);
                }
                {
                    struct Hero *w = &hero;
                    w->velocity_keys_end_frame =
                        scale_game_frames(0x24) + w->velocity_keys_start_frame;
                    w->keyed_move_target_frame = 29.0f;
                    w->unk3DC = 50.0f;
                    w->keyed_anim_target_frame = scale_game_frames(0x56);
                    w->velocity_keys = D_L14_0015F6E8;
                    w->can_chain_high_jump = 1;
                }
            } else if (u->state.current == 0xE) {
                float v = 0.7f;
                if (u->ground_distance < 2.0f)
                    v = (2.5f - u->ground_distance) * 0.5f + 0.7f;
                u->air_capsule_bottom = 0.6f;
                u->apex_anim_frame = 32.0f;
                u->target_anim_frame = 43.0f;
                u->jump_land_anim_frame = 19.0f;
                u->jump_height_target = v + 0.05f;
                u->jump_height = v;
                u->jump_height_start = v;
                u->takeoff_frame = -1;
                u->jump_ramp_frames = scale_game_frames(0xE);
                u->jump_anim_retime_scale = 1.0f;
                u->air_stick_speed = frame_time * 3.5f;
                u->pack_glide_delay = scale_game_frames(0x1E);
            } else if (u->state.current == 0xB) {
                u->jump_height = 3.2f;
                u->takeoff_frame = scale_game_frames(5);
                u->air_capsule_bottom = 0.6f;
                u->apex_anim_frame = 18.0f;
                u->target_anim_frame = 31.0f;
                u->jump_land_anim_frame = 24.0f;
                u->jump_height_target = 3.25f;
                u->jump_height_start = 3.2f;
                u->jump_ramp_frames = scale_game_frames(0xE);
                u->fall_start_distance = 4.5f;
                u->fall_delay_frames = scale_game_frames(0x32);
                u->jump_anim_retime_scale = 1.0f;
                u->takeoff_yaw = get_hero_yaw_history(scale_game_frames(0xA));
                u->jump_stick_direction = stick_direction_vs_facing();
                u->high_jump_drift_yaw = u->motion.move_yaw;
                if (frame_time * 1.5f < u->motion.flat_speed) {
                    u->air_target_speed = u->motion.flat_speed;
                } else {
                    u->air_target_speed = 0.0f;
                }
                {
                    struct Hero *w = &hero;
                    w->high_jump_drift_speed = 0;
                    w->can_use_item_in_jump = 1;
                    w->can_chain_high_jump = 1;
                }
            }
        }
        {
            struct Hero *q = &hero;
            if (q->fall_start_distance == 0.0f)
                q->fall_start_distance = q->jump_height * 1.5f;
            if (animation_update_mode) {
                int s = q->state.current;
                if (s == 7) {
                    set_hero_animation(7, 0, (float)scale_game_frames(5));
                } else if (s == 0x29) {
                    set_hero_animation(0x50, 4, (float)scale_game_frames(5));
                } else if (s == 0x2A) {
                    int id = 0x1E;
                    if (1.5707964f <
                        fast_difference_between_rotations(
                            q->motion.rot.f[2],
                            fast_arc_tan(q->rail_point.f[0] - q->prev_rail_point.f[0],
                                         q->rail_point.f[1] - q->prev_rail_point.f[1])))
                        id = 0x1F;
                    set_hero_animation(id, 0, (float)scale_game_frames(5));
                } else if (s == 9) {
                    int id = 8;
                    int n = truncate_float_to_s32(q->anim_frame);
                    if (return_hero_animation_id_unchanged(MOBY(moby)->seq) == 4)
                        id = l14_run_jump_animation_ids_by_frame[n];
                    set_hero_animation(id, 0, (float)scale_game_frames(5));
                } else if (s == 0xA) {
                    set_hero_animation(0x12, 0, (float)scale_game_frames(5));
                } else if (s == 0x10) {
                    if (q->state.prev_control_mode == HERO_MODE_JUMP ||
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
                    if (q->jump_stick_direction == 2)
                        q->jump_stick_direction = 0;
                    id = q->jump_stick_direction + 0x1C;
                    if (opt_mirror_anim != 0) {
                        if (id == 0x1C) {
                            id = 0x1D;
                        } else if (id == 0x1D) {
                            id = 0x1C;
                        }
                    }
                    {
                        struct Hero *w = &hero;
                        if (w->state.prev_control_mode == HERO_MODE_JUMP) {
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
        p->state.control_mode = HERO_MODE_ATTACK;
        p->speed_scale = 1.0f;
        p->camera_mode = 0;
        p->heading_locked = 0;
        *(int *)&p->melee_target = 0;
        p->melee_hit = 0;
        switch (p->state.current) {
        case HERO_STATE_WRENCH_SLAM: {
            struct UsageStat *h = usage_stats.stat;
            struct Hero      *q;
            if (h[0].count <= 0xFFFE) {
                h[0].count = h[0].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > h[0].latest_time_bucket) {
                h[0].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
            h[0].level_mask = h[0].level_mask | (1 << current_level_index) | 0x80000000;
            q = &hero;
            q->camera_mode = 0x50;
            q->attack_record = 4;
            q->saved_item[0] = 0;
            update_hero_target_heading(8.0f, 1.0471976f, -1.0f);
            *(int *)&q->slam_descending = 0;
            q->items[0].timer = 0xA;
            if (animation_update_mode) {
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
            struct UsageStat *item_usage_stats_base;
            struct Hero      *q;
            if (h[1].count <= 0xFFFE) {
                h[1].count = h[1].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > h[1].latest_time_bucket) {
                h[1].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
            h[1].level_mask = h[1].level_mask | (1 << current_level_index) | 0x80000000;
            item_usage_stats_base = (struct UsageStat *)item_usage;
            if (item_usage_stats_base[37].count <= 0xFFFE) {
                item_usage_stats_base[37].count = item_usage_stats_base[37].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 >
                item_usage_stats_base[37].latest_time_bucket) {
                item_usage_stats_base[37].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
            item_usage_stats_base[37].level_mask =
                item_usage_stats_base[37].level_mask | (1 << current_level_index) | 0x80000000;
            q = &hero;
            q->attack_record = 3;
            q->saved_item[0] = 0;
            update_hero_target_heading(14.0f, 0.5235988f, -1.0f);
            q->crouch_swing_toggle = q->crouch_swing_toggle == 0;
            if (animation_update_mode) {
                int n = scale_game_frames(0xA);
                if (q->moby->prev_seq == 0x1A)
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
            if (animation_update_mode) {
                set_hero_animation(0x67, 4, -1.0f);
                blend_moby_animation(((void *)q->items[0].moby), 3, 4, -1);
            }
            update_hero_target_heading(11.0f, 0.87266463f, -1.0f);
            break;
        }
        case HERO_STATE_WRENCH_COMBO: {
            struct UsageStat *h = (struct UsageStat *)item_usage;
            struct Hero      *q;
            HeroTableRow2C   *e;
            int               k;
            if (h[8].count <= 0xFFFE) {
                h[8].count = h[8].count + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > h[8].latest_time_bucket) {
                h[8].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
            q = &hero;
            h[8].level_mask = h[8].level_mask | (1 << current_level_index) | 0x80000000;
            e = &D_L14_0017C328[q->attack_record];
            q->saved_item[0] = 0;
            k = 0;
            if (q->state.prev_control_mode == HERO_MODE_ATTACK && e->combo_ends == 0 &&
                q->anim_frame < (float)e->chain_before_frame) {
                k = (e->kind + 1) % 3;
            }
            {
                struct Hero *r = &hero;
                r->attack_record = k;
            }
            update_hero_target_heading(5.0f, 0.7853982f, 0.7853982f);
            if (animation_update_mode) {
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
            if (animation_update_mode)
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
        p->state.control_mode = HERO_MODE_ATTACK;
        p->melee_hit = 0;
        if (p->state.prev == 0x70) {
            int v = 5;
            if (D_L14_0017C328[p->attack_record].kind == 0)
                v = 6;
            p->attack_record = v;
        } else {
            p->attack_record = 5;
        }
        {
            struct Hero *q = &hero;
            int          e = D_L14_0017C328[q->attack_record].kind;
            if (animation_update_mode) {
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
    case HERO_STATE_GRIND_ATTACK: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_GRIND;
        p->melee_hit = 0;
        if (p->state.prev == 0x2B) {
            p->attack_record = p->attack_record == 0;
        } else {
            p->attack_record = 0;
        }
        if (animation_update_mode) {
            struct Hero *q = &hero;
            char        *m;
            int          n = scale_game_frames(4);
            set_hero_animation(q->attack_record + 0x4E, 0, (float)n);
            m = hero_slot_moby(0);
            if (is_wrench_moby(m))
                blend_moby_animation(m, q->attack_record + 0xC, 0, n);
        }
        break;
    }
    case HERO_STATE_WAIT_FOR_ANIMATION_THEN_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0xA;
        p->camera_mode = 0;
        p->ground_speed = frame_time * 9.0f;
        if (animation_update_mode) {
            char           *m;
            HeroTableRow2C *e = &D_L14_0017C328[p->attack_record];
            int             k = e->kind;
            int             n;
            if (e->combo_ends == 1)
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
        p->camera_mode = 0;
        p->pushback_yaw = fast_add_rotations(p->motion.rot.f[2], 3.1415927f);
        p->ground_speed = frame_time * 9.0f;
        if (animation_update_mode)
            set_hero_animation(0x29, 5, (float)scale_game_frames(8));
        break;
    }
    case HERO_STATE_HURT: {
        struct Hero *p = &hero;
        char        *r;
        p->state.control_mode = HERO_MODE_HURT;
        p->camera_mode = 0;
        scale_hero_motion(0.5f);
        p->hit_flash_timer = scale_game_frames(0x2D);
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
            if (animation_update_mode) {
                set_hero_animation(0x10, 3, -3.0f);
                q->moby->anim_time = level14_hero_animation_blend_curves[q->anim_blend_curve]
                                                                        [(q->anim_blend_step)++];
            }
        }
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_IDLE: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_IDLE;
        p->camera_mode = 0;
        if (animation_update_mode)
            set_hero_animation(0, 0, (float)scale_game_frames(0xE));
        break;
    }
    case HERO_STATE_DELAYED_CIRCLE_RELEASE_WAIT: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_IDLE;
        p->camera_mode = 0;
        if (animation_update_mode) {
            set_hero_animation(4, 0, (float)scale_game_frames(7));
            hero_queue_sequence(0xE, 0x22);
        }
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_SLIP: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_FALL;
        p->camera_mode = 0;
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (animation_update_mode)
            set_hero_animation(6, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_FALL: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_FALL;
        p->camera_mode = 0;
        p->fall_state_gravity = frame_time_sq * 24.0f;
        p->stickless_decel = frame_time_sq * 7.0f;
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        if (animation_update_mode)
            set_hero_animation(6, 0, (float)scale_game_frames(7));
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_RUN: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_RUN;
        p->camera_mode = 0;
        {
            char        *v = (char *)&p->motion.resolved_velocity;
            struct Hero *hero_base;
            float        planar_speed = vector_length_xy(v);
            hero_base = (struct Hero *)(v - 0x110);
            hero_base->ground_speed = planar_speed;
            if (frame_time * 1.5f < planar_speed)
                hero_base->ground_speed = frame_time * 1.5f;
            hero_base->move_along_stick = 0;
            if (animation_update_mode)
                set_hero_animation(1, 0, (float)scale_game_frames(8));
        }
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_DYING: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_DYING;
        p->camera_mode = 0;
        qcopy(&p->motion.momentum, &p->motion.resolved_velocity);
        if (animation_update_mode)
            set_hero_animation(3, 0, (float)scale_game_frames(0xC));
        break;
    }
    case HERO_STATE_VEHICLE_MODE_3_HURT: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_HURT;
        p->camera_mode = 0;
        scale_hero_motion(0.5f);
        qcopy(&p->motion.velocity, &p->motion.resolved_velocity);
        FUN_L00_0025f730(&p->motion.velocity, frame_time * 5.0f);
        hero_change_health(1);
        p->invulnerable_frames = scale_game_frames(0x4D);
        if (animation_update_mode) {
            set_hero_animation(2, 3, -3.0f);
            p->moby->anim_time =
                level14_hero_animation_blend_curves[p->anim_blend_curve][(p->anim_blend_step)++];
        }
        break;
    }
    case HERO_STATE_GRIND_COLLISION_RECOVERY: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_GRIND;
        p->camera_mode = 0;
        if (p->rail_off_frames == 0)
            hero_change_health(1);
        p->invulnerable_frames = scale_game_frames(0x4D);
        p->grind_recovery_speed = frame_time * 2.5f;
        if (frame_time * 2.5f < p->grind_base_speed)
            p->grind_base_speed = frame_time * 2.5f;
        p->grind_speed_boost.i = 0;
        clear_u64_value(&p->motion.velocity);
        clear_u64_value(&p->motion.displacement);
        p->motion.velocity.f[2] = frame_time * 10.0f;
        if (animation_update_mode)
            set_hero_animation(0x10, 2, -3.0f);
        break;
    }
    case HERO_STATE_BLEND_TO_IDLE_ANIMATION: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->item_use_locked = 1;
        p->camera_mode = 0;
        set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_LOOK_RELEASE_WAIT: {
        struct Hero *p = &hero;
        p->camera_mode = 0x51;
        p->state.control_mode = HERO_MODE_IDLE;
        if (return_hero_animation_id_unchanged(p->moby->prev_seq) != hero_get_gadget_anim_id(1)) {
            set_hero_animation(hero_get_gadget_anim_id(1), 0, (float)scale_game_frames(5));
        }
        break;
    }
    case HERO_STATE_IDLE_ANIMATION_ENTRY: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->camera_mode = 0;
        set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_IDLE_ANIMATION_AFTER_ITEM_CANCELLATION: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->item_use_locked = 1;
        p->camera_mode = 0;
        cancel_hero_item_animation();
        set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_IDLE_ANIMATION_LONG_BLEND_ENTRY: {
        struct Hero *p = &hero;
        p->state.control_mode = HERO_MODE_ITEM_RELEASE_WAIT;
        p->camera_mode = 0;
        set_hero_animation(hero_get_gadget_anim_id(0), 0, (float)scale_game_frames(0xA));
        break;
    }
    case HERO_STATE_OPTIONAL_IDLE_ANIMATION: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->camera_mode = 0;
        p->item_select_blocked = 1;
        p->tilt_disabled = 1;
        p->item_use_locked = 1;
        if (animation_update_mode)
            set_hero_animation(hero_get_gadget_anim_id(0), 0, -1.0f);
        break;
    }
    case HERO_STATE_CLEAR_VELOCITY: {
        struct Hero *p = &hero;
        p->state.control_mode = 9;
        p->camera_mode = 0;
        p->item_select_blocked = 1;
        p->item_use_locked = 1;
        clear_u64_value(&p->motion.velocity);
        if (animation_update_mode)
            set_hero_animation(0xB, 0, -1.0f);
        break;
    }
    case HERO_STATE_CANCEL_ITEM_WITH_OPTIONAL_IDLE_ANIMATION:
    case HERO_STATE_IDLE_ANIMATION_WITH_DYING_OVERRIDE: {
        struct Hero *p = &hero;
        p->state.control_mode = 0x18;
        p->item_use_locked = 1;
        cancel_hero_item_animation();
        if (animation_update_mode)
            set_hero_animation(hero_get_gadget_anim_id(0), 0, -1.0f);
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
        if (t->magnetic_floor == 0 && t->items[1].item_id == 0x1C && t->saved_item[1] != 0) {
            t->restore_item[1] = 1;
        }
    }
    {
        struct Hero *t = &hero;
        if (t->state.control_mode != HERO_MODE_GRIND && t->items[1].item_id == 0x1D &&
            t->saved_item[1] != 0) {
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
        if (t->state.control_mode == HERO_MODE_DYING) {
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

/* This level's water surface jump uses D_L14_0015F6E8 instead; the copy stays unread. */
struct HeroVelocityKey D_L14_0017C660[HERO_WATER_SURFACE_JUMP_VELOCITY_KEYS_COUNT] =
    HERO_WATER_SURFACE_JUMP_VELOCITY_KEYS;
