/* Hero: item action input, stick target magnitude, movement integration and hop velocity. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/interpolation.h"
#include "rnc/overlay/shared/input/pad_pressed_within.h"
#include "rnc/overlay/l02/hero/hero_set_state.h"

extern float D_L02_0017C3B8[];
extern void  stick_target(int, float) __asm__("FUN_L00_002118c8");

void update_hero_stick_target_magnitude(void) __asm__("FUN_L02_00220b80");

void update_hero_stick_target_magnitude(void) {
    char *x;
    stick_target(0, 1.0f);
    x = D_0013E533 + 0xE1D;
    if (*(int *)(x + 0x2084) == 0x3F) {
        float two = frame_time + frame_time;
        float frame_scaled_value = frame_time * 3.5f * *(float *)(x + 0x190);
        *(float *)(x + 0x190) = frame_scaled_value;
        if (frame_scaled_value < two) {
            *(float *)(x + 0x190) = two;
        }
    } else if (0.0f < *(float *)(x + 0x190)) {
        if (*(float *)(x + 0x190) < D_L02_0017C3B8[3]) {
            *(float *)(x + 0x190) = D_L02_0017C3B8[2] * frame_time;
        } else {
            *(float *)(x + 0x190) = D_L02_0017C3B8[6] * frame_time;
        }
    }
}

#include "rnc/game/usage_stats.h"
#include "eetypes.h"
#include "qcopy.h"
/*
 * Hero movement step (FUN_L01_00233de0 twin; control modes 0xD and 0xE skip the speed cap): clamps the velocity, moves the hero by it (plus the
 * one-frame impulse at unk920 and the push at unkF0), runs collision, then
 * derives this frame's movement vectors and speeds:
 *   motion.unk100  position change this frame
 *   motion.unk110  movement direction scaled by the velocity along it
 *   motion.unk120  same, without the gravity-up component
 *   motion.unk130  same, gravity-up part only
 *   motion.unk140  displacement caused by the push
 *   motion.unk160  speed, unk164 horizontal speed, unk168 forward speed
 *   motion.unk16C  slope: vertical / horizontal change, clamped to +-0.5
 */

/* The FUN_L00_00233xxx helpers work in the hero's gravity frame (hero byte
   0x20B3: 0 = world z up, otherwise the frame of the hero's moby). */
extern void update_hero_attachment(s32, void *, void *) __asm__("FUN_L00_00233f80");
extern f32  gravity_length_xy(void *) __asm__("FUN_L00_002339d0");
extern f32  gravity_z(void *) __asm__("FUN_L00_00233a78");
extern void gravity_set_z(void *, void *, f32) __asm__("FUN_L00_00233b20");
extern void gravity_flatten(void *, void *) __asm__("FUN_L00_002338d0");
extern void
set_gravity_relative_vector_length_xy(float *, float *, float) __asm__("FUN_L00_00233ba0");
extern f32
forward_component(void *) __asm__("FUN_L00_00213350"); /* dot with (cos, sin) of the hero yaw */

extern void resolve_hero_collisions_with_history(void) __asm__("FUN_L00_002137a8");
extern void update_hero_ground_contact(void) __asm__("FUN_L02_002216a0");
extern void correct_hero_ground_penetration(void) __asm__("FUN_L00_00212ff0");

void integrate_hero_movement(void) __asm__("FUN_L02_002226d8");

void integrate_hero_movement(void) {
    Vec4 old_pos;
    Vec4 movement_vector_scratch;
    Vec4 velocity_projection_source;
    f32  limit;
    f32  velocity_measure;
    f32  saved_platform_delta_w;

    qcopy(&old_pos, &hero.motion.pos);
    update_hero_attachment(hero.ledge_moby, &hero.motion.platform_delta, &hero.motion.rot);

    /* cap the speed; states 0x22 and 0x14 cap the horizontal part and the fall speed */
    if (hero.state.current == 0x22 || hero.state.current == HERO_STATE_WRENCH_SLAM) {
        if (hero.collision_off_frames == 0) {
            velocity_measure = gravity_length_xy(&hero.motion.velocity);
            limit = hero.capsule_radius - 0.02f;
            if (limit < velocity_measure) {
                fast_vec_normalize(&hero.motion.velocity, &hero.motion.velocity, limit);
            }
        }
        velocity_measure = gravity_z(&hero.motion.velocity);
        limit = -hero.ground_distance;
        if (velocity_measure < limit) {
            if (0.0f < limit) {
                limit = 0.0f;
            }
            gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, limit);
        }
    } else if (hero.state.control_mode != 0xD) {
        if (hero.state.control_mode != 0xE && hero.collision_off_frames == 0) {
            velocity_measure = fast_vec_length(&hero.motion.velocity);
            limit = hero.capsule_radius - 0.02f;
            if (limit < velocity_measure) {
                fast_vec_normalize(&hero.motion.velocity, &hero.motion.velocity, limit);
            }
        }
    }

    fast_vec_add(&hero.motion.pos, &hero.motion.pos, &hero.motion.velocity);
    fast_vec_add(&hero.motion.pos, &hero.motion.pos, &hero.move_impulse);
    clear_u64_value(&hero.move_impulse);
    hero.wall_hit = 0;
    hero.coll_hit_moby = 0;

    if (fast_vec_length(&hero.motion.platform_delta) <= 0.0001f) {
        resolve_hero_collisions_with_history();
        update_hero_ground_contact();
        fast_vec_sub(&hero.motion.displacement, &hero.motion.pos, &old_pos);
        correct_hero_ground_penetration();
    } else {
        update_hero_ground_contact();
    }

    /* movement direction, scaled by the velocity along it (never negative) */
    fast_vec_sub(&hero.motion.resolved_velocity, &hero.motion.pos, &old_pos);
    qcopy(&hero.motion.resolved_velocity_xy, &hero.motion.resolved_velocity);
    qcopy(&hero.motion.resolved_velocity_z, &hero.motion.resolved_velocity);
    fast_vec_normalize(&hero.motion.resolved_velocity, &hero.motion.resolved_velocity, 1.0f);
    velocity_measure = fast_vec_dot(&hero.motion.resolved_velocity, &hero.motion.velocity);
    if (velocity_measure < 0.0f) {
        velocity_measure = 0.0f;
    }
    fast_vec_normalize(&hero.motion.resolved_velocity, &hero.motion.velocity, velocity_measure);

    qcopy(&movement_vector_scratch, &hero.motion.velocity);
    gravity_flatten(&movement_vector_scratch, &movement_vector_scratch);
    gravity_flatten(&hero.motion.resolved_velocity_xy, &hero.motion.resolved_velocity_xy);
    fast_vec_normalize(&hero.motion.resolved_velocity_xy, &hero.motion.resolved_velocity_xy, 1.0f);
    velocity_measure = fast_vec_dot(&hero.motion.resolved_velocity_xy, &movement_vector_scratch);
    if (velocity_measure < 0.0f) {
        velocity_measure = 0.0f;
    }
    fast_vec_normalize(
        &hero.motion.resolved_velocity_xy, &movement_vector_scratch, velocity_measure);

    qcopy(&velocity_projection_source, &hero.motion.velocity);
    set_gravity_relative_vector_length_xy(
        (float *)&velocity_projection_source, (float *)&velocity_projection_source, 0.0f);
    set_gravity_relative_vector_length_xy(
        (float *)&hero.motion.resolved_velocity_z, (float *)&hero.motion.resolved_velocity_z, 0.0f);
    fast_vec_normalize(&hero.motion.resolved_velocity_z, &hero.motion.resolved_velocity_z, 1.0f);
    velocity_measure = fast_vec_dot(&hero.motion.resolved_velocity_z, &velocity_projection_source);
    if (velocity_measure < 0.0f) {
        velocity_measure = 0.0f;
    }
    fast_vec_normalize(
        &hero.motion.resolved_velocity_z, &velocity_projection_source, velocity_measure);

    hero.motion.speed_xy = fast_vec_length(&hero.motion.resolved_velocity);
    hero.motion.flat_speed = vector_length_xy(&hero.motion.resolved_velocity);
    /* read through the address, as retail reloads the base register here */
    movement_vector_scratch.q = (&hero.motion.resolved_velocity)->q;
    hero.motion.forward_speed = forward_component(&movement_vector_scratch);
    if (hero.motion.forward_speed < 0.0f) {
        hero.motion.forward_speed = 0.0f;
    }

    /* apply the push, keeping its w */
    qcopy(&movement_vector_scratch, &hero.motion.pos);
    if (fast_vec_length(&hero.motion.platform_delta) > 0.0001f) {
        fast_vec_add(&hero.motion.pos, &hero.motion.pos, &hero.motion.platform_delta);
        saved_platform_delta_w = hero.motion.platform_delta.f[3];
        clear_u64_value(&hero.motion.platform_delta);
        hero.motion.platform_delta.f[3] = saved_platform_delta_w;
        resolve_hero_collisions_with_history();
        update_hero_ground_contact();
        fast_vec_sub(&hero.motion.displacement, &hero.motion.pos, &old_pos);
        correct_hero_ground_penetration();
    }
    fast_vec_sub(&hero.motion.platform_carry, &hero.motion.pos, &movement_vector_scratch);
    {
        /* push w moves to unk140.w; a block-local keeps retail's order */
        f32 push_w = hero.motion.platform_delta.f[3];

        hero.motion.platform_delta.f[3] = 0.0f;
        hero.motion.platform_carry.f[3] = push_w;
    }

    fast_vec_sub(&hero.motion.displacement, &hero.motion.pos, &old_pos);
    hero.motion.move_slope = 0.0f;
    if (hero.motion.flat_speed > 0.004f) {
        hero.motion.move_slope = hero.motion.displacement.f[2] / hero.motion.flat_speed;
        if (hero.motion.move_slope > 0.5f) {
            hero.motion.move_slope = 0.5f;
        } else if (hero.motion.move_slope < -0.5f) {
            hero.motion.move_slope = -0.5f;
        }
    }

    /* cap the step at 52 units per frame-rate unit */
    if (hero.motion.speed_xy > frame_time * 52.0f) {
        fast_vec_scale(&hero.motion.displacement,
                       &hero.motion.displacement,
                       frame_time * 52.0f / hero.motion.speed_xy);
        hero.motion.speed_xy = frame_time * 52.0f;
    }
}

typedef union {
    u128 q;
    f32  f[4];
} HopVec;

extern void build_scaled_hero_move_impulse(f32, f32) __asm__("FUN_L00_002132b8");
extern void speed_step(f32, f32) __asm__("FUN_L00_00212088");
extern void turn_to(int, float, float, float) __asm__("FUN_L00_00211e30");
extern f32
spring_angle(float *, float *, int, float, float, float, float) __asm__("FUN_L00_0025bc98");
extern void rotate_hero_about_fixed_local_pivot(float, float, float) __asm__("FUN_L00_002091d8");
extern void add_hero_forward_offset(void *, void *, f32) __asm__("FUN_L00_00233708");
extern void air_accel(f32) __asm__("FUN_L01_00234358");

#define ED64 frame_scale_sq
#define ED6C frame_time
#define ED70 frame_time_sq

extern f32 hero_low_speed_threshold_basis[] __asm__("D_L02_0017C3D0");
void       hero_update_hop_velocity(void) __asm__("FUN_L02_00223450");

void hero_update_hop_velocity(void) {
    HopVec velocity_addend;
    f32    x;
    f32    speed;
    f32    a;
    f32    speed_reduction_step;
    f32    velocity_change_step;
    s32    state;

    state = hero.state.current;
    if (state == 10) {
        build_scaled_hero_move_impulse(0.7f, ED6C * 0.0f);
        qcopy(&hero.motion.momentum, &hero.move_impulse);
        hero.ground_speed = hero.motion.flat_speed;
    } else if (state == 16) {
        if (scale_game_frames(0x36) < hero.state_timer) {
            approach_value(ED70 * 25.0f, ED70 * 7.0f, &hero.jump_gravity);
        }
        if (hero.state_timer < scale_game_frames(0x37)) {
            hero.target_speed = hero.long_jump_speed;
        } else {
            hero.target_speed = ED6C * 3.5f;
        }
        speed_step(ED70 * 44.0f, ED70 * 45.0f);
        set_gravity_relative_vector_length_xy(&hero.motion.velocity, &hero.motion.velocity, 0.0f);
        add_hero_forward_offset(&hero.motion.velocity, &hero.motion.velocity, hero.ground_speed);
    } else if (state == 0x1C) {
        if (scale_game_frames(0x28) < hero.state_timer) {
            turn_to(0, ED64 * 0.04f, ED64 * 0.2f, ED6C * 15.009831f);
            air_accel(ED70 * 20.0f);
        }
        if (13.0f <= hero.anim_frame && hero.anim_frame <= 30.0f) {
            if (27.0f < hero.anim_frame) {
                approach_value(0.0f, ED70 * 8.0f, &hero.ledge_jump_speed);
            } else {
                approach_value(ED6C * 2.0f, ED70 * 8.0f, &hero.ledge_jump_speed);
            }
        }
        hero.motion.velocity.f[0] = fast_cos(hero.motion.rot.f[2]) * hero.ledge_jump_speed;
        hero.motion.velocity.f[1] = fast_sin(hero.motion.rot.f[2]) * hero.ledge_jump_speed;
    } else if (state == 11 || state == 12) {
        hero.motion.move_yaw = hero.takeoff_yaw;
        turn_to(0, ED64 * 0.04f, ED64 * 0.2f, ED6C * 6.981317f);
        if (hero.unk450 != 3) {
            hero.ground_speed = hero.air_target_speed;
        } else {
            hero.ground_speed = 0.0f;
        }
        if (hero.state.current == HERO_STATE_HIGH_JUMP) {
            if (hero.past_jump_apex != 0 && hero.frames_to_land < scale_game_frames(0xC)) {
                approach_value(0.0f, ED70 * 8.0f, &hero.high_jump_drift_speed);
            } else {
                approach_value(ED6C * 4.2f, ED70 * 8.0f, &hero.high_jump_drift_speed);
            }
        }
        hero.motion.velocity.f[0] = fast_cos(hero.high_jump_drift_yaw) * hero.high_jump_drift_speed;
        hero.motion.velocity.f[1] = fast_sin(hero.high_jump_drift_yaw) * hero.high_jump_drift_speed;
        a = hero.air_target_speed *
            fast_sin(fast_difference_between_rotations(hero.takeoff_yaw, hero.high_jump_drift_yaw));
        hero.motion.velocity.f[0] += fast_cos(hero.takeoff_yaw) * a;
        hero.motion.velocity.f[1] += fast_sin(hero.takeoff_yaw) * a;
        build_scaled_hero_move_impulse(0.7f, ED6C * 0.0f);
        qcopy(&hero.motion.momentum, &hero.move_impulse);
    } else if (state == 0x11) {
        hero.motion.move_yaw = hero.unk470.f[3];
        qcopy(&hero.motion.move_dir, &hero.unk470);
        if (scale_game_frames(9) < hero.state_timer) {
            turn_to(-1, ED64 * 0.042f, ED64 * 0.2f, ED6C * 11.868238f);
        }
        if (scale_game_frames(9) <= hero.state_timer) {
            if (hero.unk308 == 0) {
                hero.motion.velocity.f[0] = fast_cos(hero.motion.move_yaw) * (ED6C * 4.9f);
                hero.motion.velocity.f[1] = fast_sin(hero.motion.move_yaw) * (ED6C * 4.9f);
            } else {
                set_gravity_relative_vector_length_xy(
                    &hero.motion.velocity, &hero.motion.velocity, 0.0f);
                fast_vec_normalize(&velocity_addend, &hero.motion.move_dir, ED6C * 4.9f);
                fast_vec_add(&hero.motion.velocity, &hero.motion.velocity, &velocity_addend);
            }
        }
        hero.ground_speed = hero.motion.flat_speed;
    } else {
        if (state == 0x2D) {
            x = hero.motion.rot.f[2];
            hero.motion.yaw_error = spring_angle(&x,
                                                 &hero.motion.yaw_rate,
                                                 0,
                                                 hero.motion.move_yaw,
                                                 ED64 * 0.018f,
                                                 ED64 * 0.2f,
                                                 ED6C * 7.330383f);
            a = fast_subtract_rotations(x, hero.motion.rot.f[2]);
            rotate_hero_about_fixed_local_pivot(0.0f, 0.0f, a);
        } else {
            turn_to(0, ED64 * 0.04f, ED64 * 0.2f, hero.air_turn_rate);
        }
        build_scaled_hero_move_impulse(0.7f, ED6C * 0.0f);
        qcopy(&hero.motion.momentum, &hero.move_impulse);
        velocity_change_step = ED70 * 20.0f;
        if (hero.target_speed < hero_low_speed_threshold_basis[0] * ED6C * 0.2f) {
            if (hero.state.control_mode == 2) {
                speed_reduction_step = hero.stickless_decel;
                speed = vector_length_xy(&hero.motion.velocity);
                approach_value(0.0f, speed_reduction_step, &speed);
                set_vector_length_xy(hero.motion.velocity.f, hero.motion.velocity.f, speed);
                return;
            }
            if (hero.on_flagged_ground_moby != 0) {
                velocity_change_step = ED70 * 6.0f;
            } else {
                velocity_change_step = ED70 * 2.0f;
            }
        }
        air_accel(velocity_change_step);
    }
}

#include "sda.h"
#include "rnc/mobys/moby.h"
#include "rnc/input/pad_state.h"
#include "rnc/overlay/hero_tables.h"

/* pvars of the gadget moby in slot 0 for item 15 */
struct HeldGadgetVars {
    u8  pad_0[0xC];
    s32 voice; /* 0xC: allocate_voice_for_target_entry result */
};

extern int D_00141660 NOT_SDA;
extern HeroTableRow2C D_L02_0017C228[];

#include "rnc/overlay/shared/hero/process_hero_item_action_input_globals.h"

extern s32 hero_button_8_double_press_countdown_gp __asm__("D_L02_0015F674") __attribute__((sda));

extern s32  hero_slot_item_id(s32) __asm__("FUN_L00_0020d498");
extern void start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern s32  can_select_hero_item_31(void) __asm__("FUN_L00_0020fe30");
extern s32  is_wrench_moby(struct Moby *) __asm__("FUN_L00_00211360");
extern s32  allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
extern void launch_hero_wrench(void) __asm__("FUN_L00_00217658");
extern void select_hero_grapple_swing_target(void) __asm__("FUN_L00_00221ee0");
extern void select_hero_grapple_target(void) __asm__("FUN_L00_00222158");
extern s32  get_ammo(s32) __asm__("FUN_L00_00233e98");
extern s32  voice_is_playing(struct Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern void open_vendor_menu(s32) __asm__("FUN_L00_0029a6b0");

/*
 * Gadget button handling for the item in hero slot 0 (FUN_L00_00227fa0 twin).
 * Reads the slot's button mask and, per item id, starts the matching hero
 * state (hero_set_state) or gadget action once the buttons were pressed within
 * a frame window (pad_pressed_within). Afterwards, when the state did not change,
 * it handles the 0x81 state on button 8 (D_L02_0015F674 counts the window down)
 * or clears hero.unk22CA. Returns nonzero when the hero state changed.
 */
s32 process_hero_item_action_input(void) __asm__("FUN_L02_0022ee00");

s32 process_hero_item_action_input(void) {
    enum {
        HERO_ITEM_WRENCH = 8,
        HERO_ITEM_BUFFERED_OR_TIMED_HOLD_ACTIVATE = 10,
        HERO_ITEM_BUFFERED_OR_HELD_ACTIVATE = 12,
        HERO_ITEM_HOLD_TO_FIRE = 15,
        HERO_ITEM_AMMO_GATED_BUFFERED_OR_TIMED_HOLD_ACTIVATE = 17,
        HERO_ITEM_BUFFERED_ACTIVATE_WITH_VOICE = 18,
        HERO_ITEM_BUFFERED_OR_TIMED_HOLD_ACTIVATE_WITH_VOICE = 20,
        HERO_ITEM_PRESS_TO_ACTIVATE = 21,
        HERO_ITEM_AMMO_GATED_BUFFERED_OR_TIMED_HOLD_ACTIVATE_WITH_VOICE = 24,
        HERO_ITEM_BUFFERED_OR_TIMED_HOLD_ACTIVATE_WITH_EMPTY_AMMO_VOICE = 25,
        HERO_ITEM_CLASS_GATED_BUFFERED_ACTIVATE = 31,
        HERO_ITEM_BUFFERED_ACTIVATE = 32
    };
    s32                    out0;
    s32                    out1;
    s32                    out2;
    s32                    saved;
    u32                    buttons;
    s32                    frames;
    s32                    window;
    struct Moby           *moby;
    HeroTableRow2C        *row;
    struct HeldGadgetVars *vars;

    if (hero.item_use_locked != 0 && (hero.items[0].item_id != 8 || hero.unk308 != 1)) {
        return 0;
    }
    if (hero.vehicle_mode != 0) {
        return 0;
    }
    if (hero.items[0].moby == NULL) {
        return 0;
    }
    if (hero.items[0].state != 2) {
        return 0;
    }
    saved = hero.state_timer;
    buttons = hero.items[0].button_mask;

    switch (hero.items[0].item_id) {
    case HERO_ITEM_WRENCH:
        if (hero.state.control_mode == 0 || hero.state.control_mode == 1 ||
            (hero.state.control_mode == 4 && hero.rail_land_frames != 0)) {
            if (hero.on_slip_ground == 0 &&
                pad_pressed_within(buttons, scale_game_frames(7), &out0) != 0) {
                moby = hero.items[0].moby;
                if (moby != NULL && is_wrench_moby(moby) != 0 && moby->state == 0) {
                    if ((controller_state.held & 0xA) ||
                        ((hero.state.current == HERO_STATE_LOOK || hero.state.current == 0x1E) &&
                         scale_game_frames(0x14) < hero.state_timer)) {
                        if ((hero.state.current == HERO_STATE_LOOK || hero.state.current == 0x1E) &&
                            scale_game_frames(0x14) < hero.state_timer) {
                            launch_hero_wrench();
                        } else {
                            hero_set_state(HERO_STATE_CROUCH_SWING, 1);
                        }
                    } else if (hero.unk308 == 1) {
                        hero_set_state(0x70, 1);
                    } else {
                        hero_set_state(HERO_STATE_WRENCH_COMBO, 1);
                    }
                }
            }
        } else if ((hero.state.current == HERO_STATE_JUMP ||
                    hero.state.current == HERO_STATE_RUN_JUMP ||
                    hero.state.current == HERO_STATE_LAND ||
                    hero.state.current == HERO_STATE_FALL ||
                    (hero.state.current == 0xE && hero.anim_frame > 32.0f) ||
                    (hero.state.current == HERO_STATE_HIGH_JUMP &&
                         (hero.unk450 != 3 && hero.anim_frame > 18.0f) ||
                     (hero.state.current == HERO_STATE_HIGH_JUMP && hero.unk450 == 3 &&
                      hero.anim_frame > 20.0f)) ||
                    hero.state.current == 0x2D || hero.state.current == 0x11) &&
                   scale_game_frames(0xA) < hero.state_timer) {
            window = scale_game_frames(0xA);
            if (hero.state.current == HERO_STATE_HIGH_JUMP || hero.state.current == 0xE) {
                window = scale_game_frames(0x12);
            }
            if (pad_pressed_within(buttons, window, NULL) != 0) {
                if ((hero.past_jump_apex == 0 ? 0.1f < hero.ground_distance
                                              : 0.57f < hero.ground_distance) ||
                    (hero.state.current == HERO_STATE_LAND && 0.57f < hero.ground_distance)) {
                    hero_set_state(HERO_STATE_WRENCH_SLAM, 1);
                } else if (hero.on_slip_ground == 0) {
                    hero_set_state(HERO_STATE_WRENCH_COMBO, 1);
                }
            }
        } else if (hero.state.control_mode == 6) {
            row = &D_L02_0017C228[hero.attack_record];
            if (hero.state.current == HERO_STATE_CROUCH_SWING &&
                scale_game_frames(0x1E) < hero.state_timer && hero.anim_changing == 0 &&
                (f32)row->f10 <= hero.anim_frame && (controller_state.held & 0xA) &&
                pad_pressed_within(buttons, scale_game_frames(0xF), NULL) != 0) {
                hero_set_state(HERO_STATE_CROUCH_SWING, 1);
            } else if (hero.anim_changing == 0 && (f32)row->f10 <= hero.anim_frame) {
                frames = (truncate_float_to_s32(hero.anim_frame) - row->fC) * 2;
                if (frames < 1) {
                    frames = 1;
                }
                if (scale_game_frames(0xF) < frames) {
                    frames = scale_game_frames(0xF);
                }
                if (hero.on_slip_ground == 0 && pad_pressed_within(buttons, frames, NULL) != 0) {
                    if (controller_state.held & 0xA) {
                        hero_set_state(HERO_STATE_CROUCH_SWING, 1);
                    } else if (hero.unk308 == 1) {
                        hero_set_state(0x70, 1);
                    } else {
                        hero_set_state(HERO_STATE_WRENCH_COMBO, 1);
                    }
                }
            }
        }
        break;

    case HERO_ITEM_BUFFERED_OR_HELD_ACTIVATE:
        select_hero_grapple_swing_target();
        select_hero_grapple_target();
        window = scale_game_frames(5);
        if (hero.air_frames.s != 0) {
            window = scale_game_frames(8);
        }
        if (hero.state.control_mode == 0 || hero.state.control_mode == 1 ||
            hero.state.control_mode == 4 || hero.state.control_mode == 2 ||
            hero.state.control_mode == 0xF || hero.state.control_mode == 5 ||
            hero.state.current == 0x1E) {
            if (pad_pressed_within(buttons, window, NULL) != 0 ||
                ((controller_state.held & buttons) &&
                 hero.items[0].ready_frames > scale_game_frames(0xF))) {
                if (hero.swing_target_in_sight != 0) {
                    if (hero.grapple_target_in_sight != 0) {
                        if (hero.swing_target_dist < hero.unk984) {
                            hero.grapple_target_in_sight = 0;
                        } else {
                            hero.swing_target_in_sight = 0;
                        }
                    }
                }
                if (hero.swing_target_in_sight != 0 && hero.grapple_lockout_timer == 0 &&
                    hero.state.current != 0x2C) {
                    hero_set_state(0x2C, 1);
                } else if (hero.grapple_target_in_sight != 0 && hero.grapple_lockout_timer == 0) {
                    hero_set_state(0x24, 1);
                }
            }
        }
        break;

    case HERO_ITEM_CLASS_GATED_BUFFERED_ACTIVATE:
        if (hero.vehicle_mode == 0 && can_select_hero_item_31() != 0) {
            if (pad_pressed_within(buttons, scale_game_frames(7), NULL) != 0 &&
                D_00141660 == 0x1F && hero.items[0].moby->oclass == 0x1E3 &&
                hero.vehicle_toggle_timer == 0) {
                allocate_hero_voice_for_entry(0x18, 0);
                hero.vehicle_toggle_timer = 0x12;
                hero.unk22DC = 1;
            }
        }
        break;

    case HERO_ITEM_BUFFERED_ACTIVATE:
        if ((hero.state.control_mode == 0 || hero.state.control_mode == 1 ||
             hero.state.control_mode == 2 || hero.state.control_mode == 4 ||
             hero.state.control_mode == 5) &&
            pad_pressed_within(buttons, scale_game_frames(8), NULL) != 0) {
            open_vendor_menu(0);
        }
        break;

    case HERO_ITEM_HOLD_TO_FIRE:
        if (hero.state.control_mode == 0 || hero.state.control_mode == 1 ||
            hero.state.control_mode == 0xC || hero.state.control_mode == 2 ||
            hero.state.control_mode == 4 || hero.state.control_mode == 5 ||
            hero.state.control_mode == 0xF || hero.state.current == 0x1E) {
            if (hero.items[0].moby != NULL) {
                vars = (struct HeldGadgetVars *)hero.items[0].moby->pvars;
                if ((((controller_state.held & buttons) &&
                      scale_game_frames(5) < hero.items[0].ready_frames) ||
                     (controller_state.pressed & buttons)) &&
                    get_ammo(-1) != 0) {
                    if (hero.item_anim_active == 0) {
                        start_equipped_item_animation();
                    }
                } else if ((controller_state.held & buttons) && get_ammo(-1) == 0 &&
                           voice_is_playing(hero.items[0].moby, vars->voice) == 0) {
                    vars->voice = allocate_voice_for_target_entry(0, 0, hero.items[0].moby);
                }
            }
        }
        break;

    case HERO_ITEM_BUFFERED_OR_TIMED_HOLD_ACTIVATE:
    case HERO_ITEM_AMMO_GATED_BUFFERED_OR_TIMED_HOLD_ACTIVATE:
    case HERO_ITEM_BUFFERED_OR_TIMED_HOLD_ACTIVATE_WITH_VOICE:
    case HERO_ITEM_AMMO_GATED_BUFFERED_OR_TIMED_HOLD_ACTIVATE_WITH_VOICE:
    case HERO_ITEM_BUFFERED_OR_TIMED_HOLD_ACTIVATE_WITH_EMPTY_AMMO_VOICE:
        if ((hero.state.control_mode == 0 && hero.state.current != 0x1E) ||
            hero.state.control_mode == 1 || hero.state.control_mode == 0xC ||
            hero.state.control_mode == 5 || hero.state.control_mode == 0xF ||
            (hero.state.control_mode == 4 && hero.unk4AC != 0) ||
            (hero.state.current == 0x23 && hero.anim_frame > 25.0f)) {
            if (hero.item_anim_active == 0) {
                s32 hold;

                window = scale_game_frames(7);
                hold = hero.items[0].ready_frames;
                if (hold < window) {
                    window = hold;
                }
                if (pad_pressed_within(buttons, window, &out1) != 0 ||
                    ((controller_state.held & 0x20) && scale_game_frames(8) < hold &&
                     hold < scale_game_frames(0x11))) {
                    if (get_ammo(-1) != 0) {
                        if (hero.state.control_mode == 0xC || hero.state.control_mode == 0 ||
                            hero.state.current == HERO_STATE_STOP ||
                            (hero.state.control_mode == 1 && hero.stick_magnitude < 0.7f)) {
                            hero_set_state(0x23, 1);
                        } else {
                            start_equipped_item_animation();
                        }
                    } else if (hero.items[0].moby != NULL) {
                        allocate_voice_for_target_entry(0, 0, hero.items[0].moby);
                    }
                }
            }
        }
        break;

    case HERO_ITEM_BUFFERED_ACTIVATE_WITH_VOICE:
        if (hero.state.control_mode == 0 || hero.state.control_mode == 1 ||
            (hero.state.control_mode == 4 && hero.rail_land_frames != 0)) {
            if (hero.state.current != HERO_STATE_LOOK) {
                s32 limit;
                s32 hold;

                limit = scale_game_frames(0xA);
                hold = hero.items[0].ready_frames;
                if (pad_pressed_within(buttons, hold < limit ? hold : limit, &out2) != 0 &&
                    hero.items[0].moby != NULL) {
                    hero_set_state(0x20, 1);
                    if (hero.items[0].moby != NULL) {
                        allocate_voice_for_target_entry(0, 0, hero.items[0].moby);
                    }
                }
            }
        }
        break;

    case HERO_ITEM_PRESS_TO_ACTIVATE:
        if (hero.items[0].moby != NULL && (controller_state.pressed & buttons)) {
            start_equipped_item_animation();
        }
        break;
    }

    if (hero.state_timer == saved) {
        if (hero_slot_item_id(3) == 3 && hero.state.current != 0x81 &&
            (hero.unk308 == 0 || D_0013D4DC[0] == 0) && hero.pack_disabled == 0) {
            fast_dec_timer(&hero_button_8_double_press_countdown_gp);
            if (hero_button_8_double_press_countdown_gp != 0) {
                if (hero.state.control_mode == 0 || hero.state.control_mode == 1 ||
                    hero.state.control_mode == 0xC ||
                    ((hero.state.control_mode == 2 || hero.state.control_mode == 4) &&
                     hero.grounded_frames != 0)) {
                    if ((controller_state.held & 8) && (controller_state.pressed & 8)) {
                        hero_set_state(0x81, 1);
                    }
                }
            }
            if ((controller_state.pressed & 8) && hero.state.current != 0x81) {
                hero_button_8_double_press_countdown_gp = scale_game_frames(0x1E);
            }
        } else if (hero.state.current != HERO_STATE_WRENCH_COMBO && hero.state.current != 0x81 &&
                   hero.state.current != HERO_STATE_CROUCH_SWING &&
                   hero.state.current != HERO_STATE_HURT) {
            hero.unk22CA = 0;
        }
    }
    return hero.state_timer < saved;
}

s32 D_L06_0015F674 = 0;
s32 D_L08_0015F674 = 0;
s32 D_L11_0015F674 = 0;
s32 D_L12_0015F674 = 0;
s32 D_L14_0015F674 = 0;
s32 D_L15_0015F674 = 0;
s32 D_L18_0015F674 = 0;

s32 hero_button_8_double_press_countdown_gp __asm__("D_L02_0015F674") = 0;
