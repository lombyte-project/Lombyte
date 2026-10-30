/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/game/options.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_check_damage.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/overlay/shared/hero/set_second_hero_easing_parameters.h"
#include "rnc/overlay/shared/math/rotations/split_float_fraction.h"
#include "rnc/overlay/shared/rendering/commands/get_moby_color.h"
#include "rnc/overlay/shared/hero/advance_hero_animation.h"
#include "rnc/overlay/shared/ui/map/reveal_map_around_position.h"
#include "rnc/rendering/frame/begin_draw_frame.h"

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00204130), where it is exact; names translated to the US level program. */

extern char D_L00_00179410[];
extern void dismiss_help(void) __asm__("FUN_001fdc08");

/* Once (flag 0x38): starts a 0x3C-frame timer, dismisses help, sets the first word to 8 if 0. */
void jet_camera_mode(void) __asm__("FUN_L00_00203b18");

void jet_camera_mode(void) {
    char *jet_camera_state = D_L00_00179410;
    if (*(short *)(jet_camera_state + 0x38) == 0) {
        *(short *)(jet_camera_state + 0x38) = 1;
        *(int *)(jet_camera_state + 0x3C) = scale_game_frames(0x3C);
        dismiss_help();
        if (*(int *)jet_camera_state == 0)
            *(int *)jet_camera_state = 8;
    }
}

extern char jet_camera_mode_setup_latch[] __asm__("D_L00_00179448");
/* Clears the short at D_L00_00179448. */
void clear_jet_camera_mode_latch(void) __asm__("FUN_L00_00203b78");

void clear_jet_camera_mode_latch(void) {
    *(short *)jet_camera_mode_setup_latch = 0;
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00205598), where it is exact; names translated to the US level program. */

extern void clear_hero_queued_animation(void) __asm__("FUN_L00_00232628");

/* Resets hero fields 0x220..0x234, 0x22CB, 0x2084 and 0xA94, then calls 00232628 and 002223f8. */
extern unsigned char D_0014161B[];
void                 reset_hero_to_idle(void) __asm__("FUN_L00_00204f80");

void reset_hero_to_idle(void) {
    struct Hero *hero_state = &hero;
    hero_state->capsule_top = 0.8f;
    hero_state->capsule_radius = 0.45f;
    hero_state->capsule_top_target = 0.8f;
    hero_state->capsule_bottom = 0.7f;
    hero_state->capsule_bottom_target = 0.7f;
    hero_state->vent_updraft = 0;
    hero_state->state.current = 0;
    *(int *)(D_0014161B + 0x4D) = 0;
    hero_state->anim_frame_rate = 1.0f;
    clear_hero_queued_animation();
    hero_set_state(HERO_STATE_IDLE, 1);
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00205618), where it is exact; names translated to the US level program. */

#include "rnc/overlay/item_defs.h"

int hero_idle_sequence(int a) __asm__("FUN_L00_00205000");

int hero_idle_sequence(int allow_item_idle_sequence) {
    struct Hero *vehicle_mode_owner = &hero;
    int          vehicle_mode = vehicle_mode_owner->vehicle_mode;
    if (vehicle_mode == 2 || (vehicle_mode < 3 ? vehicle_mode == 1 : vehicle_mode == 3)) {
        return 0;
    }
    {
        struct Hero *idle_sequence_state = &hero;
        if (idle_sequence_state->health.hp == 1) {
            return 0x54;
        }
        if (idle_sequence_state->on_ice_ground != 0) {
            return 0x6E;
        }
        if (allow_item_idle_sequence == 0) {
            return 0;
        }
        if (idle_sequence_state->item_anim_active == 0) {
            return 0;
        }
        if (idle_sequence_state->unk20AA == 0) {
            return 0;
        }
        if (idle_sequence_state->unk22C8 != 0) {
            return 0;
        }
    }
    {
        int i = hero_slot_item_id(0);
        return item_defs[i].hero_animation;
    }
}

/* Returns its argument unchanged. */
s32 return_hero_animation_id_unchanged(s32 input_value) __asm__("FUN_L00_002050b8");

s32 return_hero_animation_id_unchanged(s32 input_value) {
    return input_value;
}

/* Lowers hero.health.hp by one (not below 0) and calls 00234e00. */
void hero_change_health(s32 n) __asm__("FUN_L00_002050c0");

void hero_change_health(s32 health_decrement) {
    if (health_decrement) {
        struct Hero *hero_health_owner = &hero;
        int          health_after_subtraction;
        if (health_decrement >= 2)
            health_decrement = 1;
        health_after_subtraction = hero_health_owner->health.hp - health_decrement;
        hero_health_owner->health.hp = health_after_subtraction;
        if (health_after_subtraction < 0)
            hero_health_owner->health.hp = 0;
        show_health_hud();
    }
}

extern s32 D_001413D0[];
extern s32 hero_slot_three_secondary_moby[] __asm__("D_001404D4");
extern s32 D_001403E0[];

/* Returns D_001413D0[0] for a = 0, 2, 3, 4, D_001404D4[0] for 1, D_001403E0[0] for 5, else 0. */
int get_hero_easing_moby_by_kind(int ease_kind) __asm__("FUN_L00_00205110");

int get_hero_easing_moby_by_kind(int ease_kind) {
    if (ease_kind == 0 || ease_kind == 2 || ease_kind == 3 || ease_kind == 4) {
        return D_001413D0[0];
    }
    if (ease_kind == 1) {
        return hero_slot_three_secondary_moby[0];
    }
    if (ease_kind == 5) {
        return D_001403E0[0];
    }
    return 0;
}

#include "qcopy.h"

extern u8 D_001413F4 NOT_SDA;
void                 detach_manipulator(s32, void *) __asm__("FUN_0020cb88");
void                 attach_manipulator(s32, s32, void *) __asm__("FUN_0020cb10");
/* spring_angle (eases an angle toward a target) with its int argument last. EABI
   passes ints and floats in separate registers, so the call is the same; this order
   gives retail's register allocation. */
f32  spring_angle(f32 *, f32, f32 *, f32, f32, f32, s32) __asm__("FUN_L00_0025bc98");
void vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
void build_quaternion_from_axis_rotations(void *, void *) __asm__("FUN_L00_00259f50");

/* Eases one record's rotation and position toward their targets and rebuilds its matrix;
   once targets are zero and it has settled, detaches it from its moby. */
void update_hero_easing_record(struct HeroEase *ease_record) __asm__("FUN_L00_00205168");

void update_hero_easing_record(struct HeroEase *ease_record) {
    s32 moby = get_hero_easing_moby_by_kind(ease_record->kind);
    if (moby == 0)
        return;
    if (D_001413F4 == 0) {
        if (ease_record->kind == 4 || ease_record->kind == 2 || ease_record->kind == 3)
            return;
    } else if (ease_record->kind == 0) {
        return;
    }
    if (ease_record->rot_target.f[0] == 0.0f && ease_record->rot_target.f[1] == 0.0f &&
        ease_record->rot_target.f[2] == 0.0f && ease_record->pos_target.f[0] == 0.0f &&
        ease_record->pos_target.f[1] == 0.0f && ease_record->pos_target.f[2] == 0.0f &&
        absolute_float(ease_record->pos.f[0]) < 0.003f &&
        absolute_float(ease_record->pos.f[1]) < 0.003f &&
        absolute_float(ease_record->pos.f[2]) < 0.003f && ease_record->scale == 1.0f &&
        absolute_float(ease_record->rot.f[0]) < 0.005f &&
        absolute_float(ease_record->rot.f[1]) < 0.005f &&
        absolute_float(ease_record->rot.f[2]) < 0.005f) {
        if (ease_record->attached)
            detach_manipulator(moby, ease_record);
    } else {
        Vec4 *rot;
        Vec4 *pos;
        /* retail reads this flag at its literal address, with no relocation */
        if (*(u8 *)0x15EDB5) {
            ease_record->rot_target.f[0] = -ease_record->rot_target.f[0];
            ease_record->rot_target.f[2] = -ease_record->rot_target.f[2];
            ease_record->pos.f[1] = -ease_record->pos.f[1];
        }
        rot = &ease_record->rot;
        pos = &ease_record->pos;
        spring_angle(&rot->f[0],
                     ease_record->rot_target.f[0],
                     &ease_record->rot_vel.f[0],
                     ease_record->accel * frame_scale_sq,
                     ease_record->max_speed * frame_scale_sq,
                     0.0f,
                     2);
        spring_angle(&rot->f[1],
                     ease_record->rot_target.f[1],
                     &ease_record->rot_vel.f[1],
                     ease_record->accel * frame_scale_sq,
                     ease_record->max_speed * frame_scale_sq,
                     0.0f,
                     2);
        spring_angle(&rot->f[2],
                     ease_record->rot_target.f[2],
                     &ease_record->rot_vel.f[2],
                     ease_record->accel * frame_scale_sq,
                     ease_record->max_speed * frame_scale_sq,
                     0.0f,
                     2);
        vendor_spring(&pos->f[0],
                      &ease_record->pos_vel.f[0],
                      ease_record->pos_target.f[0],
                      ease_record->accel,
                      ease_record->max_speed,
                      0.0f);
        vendor_spring(&pos->f[1],
                      &ease_record->pos_vel.f[1],
                      ease_record->pos_target.f[1],
                      ease_record->accel,
                      ease_record->max_speed,
                      0.0f);
        vendor_spring(&pos->f[2],
                      &ease_record->pos_vel.f[2],
                      ease_record->pos_target.f[2],
                      ease_record->accel,
                      ease_record->max_speed,
                      0.0f);
        if (!ease_record->attached)
            attach_manipulator(moby, ease_record->attach_joint, ease_record);
        build_quaternion_from_axis_rotations(&ease_record->quat, rot);
        qcopy(&ease_record->translation, pos);
        ease_record->scale3.f[0] = ease_record->scale;
        ease_record->scale3.f[1] = ease_record->scale;
        ease_record->scale3.f[2] = ease_record->scale;
    }
    clear_u64_value(&ease_record->pos_target);
    clear_u64_value(&ease_record->rot_target);
    ease_record->scale = 1.0f;
}

extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
/* Runs FUN_L00_00205168 on each of the 31 hero eases (0x1550 bytes). */
void update_hero_easing_records(void) __asm__("FUN_L00_002054e8");

void update_hero_easing_records(void) {
    struct HeroEase *ease_record = hero_eases;
    struct HeroEase *end = (struct HeroEase *)((u8 *)ease_record + 0x1550);
    do {
        update_hero_easing_record(ease_record);
        ease_record++;
    } while ((s32)ease_record < (s32)end);
}

extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
s32                    get_hero_easing_moby_by_kind(s32) __asm__("FUN_L00_00205110");

void reset_hero_easing_records(void) __asm__("FUN_L00_00205538");

void reset_hero_easing_records(void) {
    s32   i;
    char *manipulator_records_base = (char *)hero_eases;
    char *record_reset_destination = manipulator_records_base + 0x90;
    char *record_reset_cursor = manipulator_records_base + 0x60;
    for (i = 0; i < 31; i++) {
        struct HeroEase *manipulator_record = &hero_eases[i];
        if (manipulator_record->attached != 0) {
            s32 manipulator_moby = get_hero_easing_moby_by_kind(manipulator_record->kind);
            if (manipulator_moby != 0)
                detach_manipulator(manipulator_moby, manipulator_record);
        }
        clear_u64_value(&hero_eases[i].rot);
        clear_u64_value(record_reset_cursor);
        record_reset_cursor += sizeof(struct HeroEase);
        clear_u64_value(record_reset_destination);
        record_reset_destination += sizeof(struct HeroEase);
        manipulator_record->scale = 1.0f;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00205C18), where it is exact; names translated to the US level program. */

extern int D_L00_0015F5CC;

void update_hero_random_timer(void) __asm__("FUN_L00_00205600");

void update_hero_random_timer(void) {
    struct Hero *hero_random_timer_owner = &hero;
    if (hero_random_timer_owner->rand_timer.fired == 0) {
        if (hero_random_timer_owner->rand_timer.range == 0) {
            hero_random_timer_owner->rand_timer.deadline = D_L00_0015F5CC + scale_game_frames(30);
        } else if (D_L00_0015F5CC > hero_random_timer_owner->rand_timer.deadline) {
            int base_delay_frames, first_random_delay_frames;
            hero_random_timer_owner->rand_timer.fired = 1;
            base_delay_frames = scale_game_frames(20);
            first_random_delay_frames =
                random_integer_below(hero_random_timer_owner->rand_timer.range);
            hero_random_timer_owner->rand_timer.deadline =
                D_L00_0015F5CC + base_delay_frames + first_random_delay_frames +
                random_integer_below(hero_random_timer_owner->rand_timer.range);
        }
    }
}

typedef u32            Quad_56a8 __attribute__((mode(TI), aligned(16)));
extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");

extern Quad_56a8 l00_hero_random_manipulator_initial_quaternions[] __asm__("D_L00_0017C040");
extern Quad_56a8 l00_hero_random_manipulator_initial_scales[] __asm__("D_L00_0017C0B0");
extern Quad_56a8 l00_hero_random_manipulator_initial_translations[] __asm__("D_L00_0017C120");
extern s32       hero_random_manipulator_attachment_indices[] __asm__("D_L00_0017C1C0");
extern f32       hero_random_manipulator_blend_by_phase[] __asm__("D_L00_0017C190");
void update_hero_rotation_targets_and_random_manipulators(void) __asm__("FUN_L00_002056a8");

void update_hero_rotation_targets_and_random_manipulators(void) {
    struct HeroEase *e;

    {
        struct HeroEase *e = hero_eases;
        e[12].rot_target.f[2] = e[3].rot_target.f[2] * 0.25f;
        if (e[12].rot_target.f[2] > 0.14835298f)
            e[12].rot_target.f[2] = 0.14835298f;
        else if (e[12].rot_target.f[2] < -0.14835298f)
            e[12].rot_target.f[2] = -0.14835298f;
    }
    e = hero_eases;
    e[12].rot_target.f[1] = e[3].rot_target.f[1] / 2.8f;
    if (e[12].rot_target.f[1] > 0.29670596f)
        e[12].rot_target.f[1] = 0.29670596f;
    if (e[12].rot_target.f[1] < -0.17453292f)
        e[12].rot_target.f[1] = -0.17453292f;
    if ((u32)(*((u8 *)hero.moby + 0x53) - 1) < 2) {
        if (e[12].rot_target.f[2] > 0.034906585f)
            e[12].rot_target.f[2] = 0.034906585f;
        if (e[12].rot_target.f[2] < -0.034906585f)
            e[12].rot_target.f[2] = -0.034906585f;
        if (e[12].rot_target.f[1] > 0.12217305f)
            e[12].rot_target.f[1] = 0.12217305f;
        if (e[12].rot_target.f[1] < -0.12217305f)
            e[12].rot_target.f[1] = -0.12217305f;
    }
    update_hero_random_timer();
    {
        if (hero.rand_timer.fired == 0)
            return;
        hero.rand_timer.fired++;
        if (hero.rand_timer.fired > 11)
            hero.rand_timer.fired = 0;
        if (hero.rand_timer.fired == 0) {
            s32 i;
            for (i = 0; i < 7; i++) {
                if (hero.manipulators[i].attached != 0)
                    detach_manipulator((s32)hero.moby, &hero.manipulators[i]);
            }
        } else {
            s32 i;
            s32 j;
            for (i = 0; i < 7; i++) {
                if (hero.manipulators[i].attached == 0) {
                    attach_manipulator((s32)hero.moby,
                                       hero_random_manipulator_attachment_indices[i],
                                       &hero.manipulators[i]);
                    hero.manipulators[i].absolute = 1;
                    qcopy(&hero.manipulators[i].quat,
                          &l00_hero_random_manipulator_initial_quaternions[i]);
                    qcopy(&hero.manipulators[i].scale3,
                          &l00_hero_random_manipulator_initial_scales[i]);
                    qcopy(&hero.manipulators[i].translation,
                          &l00_hero_random_manipulator_initial_translations[i]);
                }
            }
            for (j = 0; j < 7; j++)
                hero.manipulators[j].blend =
                    hero_random_manipulator_blend_by_phase[hero.rand_timer.fired];
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00205FF0). */
#include "rnc/mobys/moby.h"
#include "qcopy.h"

/* hero + 0xEF0: four more manipulators after manipulators[7], then a random-delay timer */
struct HeroExtraManipulators {
    u8                     pad[0xEF0];
    struct HeroManipulator m[4];
    u8                     pad2[0xC];
    s16                    delay;
    s16                    timer;
};
#define HERO_EXTRA   ((struct HeroExtraManipulators *)&hero)
#define EXTRA_MANIPS (HERO_EXTRA->m)

extern u8  D_0015EDB3_macro __asm__("D_0015EDB3") MACRO_ADDR;
extern f32 D_0015EE18 __asm__("D_0015EE18");

#include "rnc/overlay/shared/hero/hero_idle_sequence_globals.h"

extern s32 hero_extra_manipulator_attachment_indices_gp __asm__("D_L00_0015F6F8")
    __attribute__((sda));
extern u128 hero_extra_manipulator_initial_quaternions[] __asm__("D_L00_0017C1E0");
extern u128 hero_extra_manipulator_initial_scales[] __asm__("D_L00_0017C220");
extern u128 hero_extra_manipulator_initial_translations[] __asm__("D_L00_0017C260");
extern f32  hero_extra_manipulator_blend_by_phase[] __asm__("D_L00_0017C2A0");
extern f32  approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");

/* Pulses the glow colour of the hero's moby (or of the item slot 3 moby2),
   then attaches the four extra manipulators while their timer runs. */
void update_hero_glow_and_extra_manipulators(void) __asm__("FUN_L00_002059d8");

void update_hero_glow_and_extra_manipulators(void) {
    struct Hero *player, *glow_owner, *manip_owner;
    struct Moby *moby;
    s32 alpha, blue_swing, green_swing, red, blue, green, red_pulse, green_pulse, blue_pulse,
        period, min_delay, delay;
    f32 period_f, phase, wave, wave_24;

    if (D_0015EDB3_macro != 0)
        approach_value(&D_0015EE18, 1.8f, 0.05f);
    else
        approach_value(&D_0015EE18, 1.0f, 0.05f);

    player = &hero;
    if ((u8)(player->vehicle_mode - 1) < 2)
        moby = player->moby;
    else {
        if (player->pack_disabled != 0)
            return;
        moby = player->items[3].moby2;
    }
    if (moby == 0)
        return;
    glow_owner = &hero;
    moby->flags |= 0x10;
    alpha = 0x80;
    green = 0x88;
    red = 0x38;
    if (glow_owner->health.hp == 1)
        red = green;
    period = scale_game_frames(0x6E);
    period_f = convert_integer_to_float(period);
    phase = (f32)(D_L00_0015F5CC % period) / period_f;
    phase = phase + phase;
    phase = phase * 3.1415927f;
    wave = fast_sin(phase + -3.1415927f);
    wave_24 = wave * 24.0f;
    red_pulse = truncate_float_to_s32(wave_24) + 0xC;
    red = red + red_pulse;
    green_swing = truncate_float_to_s32(wave * 48.0f);
    green_pulse = green_swing + 0x18;
    blue_swing = truncate_float_to_s32(wave_24);
    green = green_swing + 0xA0;
    blue_pulse = blue_swing + 0xC;
    blue = blue_swing + 0x4C;
    if (glow_owner->unk1EE != 0) {
        f32 fade = 1.0f;
        s32 fade_in = scale_game_frames(5);
        s32 fade_out = scale_game_frames(20);
        s32 remaining = glow_owner->unk1EE;

        if (scale_game_frames(45) - fade_in < remaining) {
            fade = convert_integer_to_float(scale_game_frames(45) - glow_owner->unk1EE);
            fade = fade / convert_integer_to_float(fade_in);
        }
        if (glow_owner->unk1EE < fade_out) {
            fade = convert_integer_to_float(glow_owner->unk1EE);
            fade = fade / convert_integer_to_float(fade_out);
        }
        red = red - truncate_float_to_s32((f32)red_pulse * fade);
        green = green - truncate_float_to_s32((f32)green_pulse * fade);
        blue = blue - truncate_float_to_s32((f32)blue_pulse * fade);
        red = red + truncate_float_to_s32(fade * 112.0f);
        green = green - truncate_float_to_s32(fade * 72.0f);
    }
    moby->glow_color = (alpha << 24) | (blue << 16) | (green << 8) | red;
    manip_owner = &hero;
    if (manip_owner->vehicle_mode != 0)
        return;
    if (fast_dec_timer_s16(&HERO_EXTRA->delay) && moby->prev_seq == 1) {
        min_delay = scale_game_frames(50);
        delay = random_int_between(min_delay, scale_game_frames(200));
        HERO_EXTRA->delay = delay;
        HERO_EXTRA->timer = 1;
    }
    if (HERO_EXTRA->timer == 0)
        return;
    HERO_EXTRA->timer++;
    if (HERO_EXTRA->timer > 21)
        HERO_EXTRA->timer = 0;
    if (HERO_EXTRA->timer == 0) {
        s32 i;
        for (i = 0; i < 4; i++) {
            if (EXTRA_MANIPS[i].attached != 0)
                detach_manipulator((s32)moby, &EXTRA_MANIPS[i]);
        }
    } else {
        s32 i;
        s32 j;
        for (i = 0; i < 4; i++) {
            if (EXTRA_MANIPS[i].attached == 0) {
                attach_manipulator((s32)moby,
                                   ((s32 *)&hero_extra_manipulator_attachment_indices_gp)[i],
                                   &EXTRA_MANIPS[i]);
                EXTRA_MANIPS[i].absolute = 1;
                qcopy(&EXTRA_MANIPS[i].quat, &hero_extra_manipulator_initial_quaternions[i]);
                qcopy(&EXTRA_MANIPS[i].scale3, &hero_extra_manipulator_initial_scales[i]);
                qcopy(&EXTRA_MANIPS[i].translation,
                      &hero_extra_manipulator_initial_translations[i]);
                fast_vec_scale(
                    &EXTRA_MANIPS[i].translation, &EXTRA_MANIPS[i].translation, moby->scale);
            }
        }
        for (j = 0; j < 4; j++)
            EXTRA_MANIPS[j].blend = hero_extra_manipulator_blend_by_phase[HERO_EXTRA->timer];
    }
}
#undef EXTRA_MANIPS
#undef HERO_EXTRA

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_002064C0), where it is exact; names translated to the US level program. */

extern unsigned char D_0013D4D5[];

#include "rnc/data_labels.h"

extern void reset_numeric_hud_element_32x32_base(void) __asm__("FUN_L00_00235dc0");
extern void update_hud_panel_visibility(void) __asm__("FUN_L00_0023a218");
extern void draw_hud_bolt_exclamation_panel(void) __asm__("FUN_L00_0023a2d0");
s32         queue_animation_update(s32 chan, s32 id, s32 fn, s32 d, s32 e, s32 c, s32 b) __asm__("FUN_001ff308");

void update_hero_bolt_exclamation_hud(void) __asm__("FUN_L00_00205ea8");

void update_hero_bolt_exclamation_hud(void) {
    struct Hero *hero_state;
    if (D_0013D4D5[6] == 0) {
        *(int *)(((unsigned char *)&D_0013E533) + 0x2E65) = 0;
        return;
    }
    hero_state = &hero;
    if (hero_state->detector_target_dist < 20.0f && hero_state->metal_detector_target != 0) {
        hero_state->nearby_moby_close = 1;
    } else {
        struct Hero *animation_context_clear_owner = &hero;
        animation_context_clear_owner->nearby_moby_close = 0;
    }
    {
        struct Hero *animation_context_owner = &hero;
        animation_context_owner->detector_target_dist = 100000.0f;
        animation_context_owner->metal_detector_target = 0;
        if (animation_context_owner->nearby_moby_close != 0) {
            queue_animation_update(7,
                                   0x753A,
                                   (int)reset_numeric_hud_element_32x32_base,
                                   (int)update_hud_panel_visibility,
                                   (int)draw_hud_bolt_exclamation_panel,
                                   (int)(&animation_context_owner->nearby_moby_close),
                                   1);
        }
    }
}

extern void request_grouped_voice(int, char *, int, int, int) __asm__("FUN_L00_0028de68");
extern void spawn_texture_indexed_rotated_particle(float *, int, float) __asm__("FUN_L00_00274cf8");

void emit_hero_animation_voice_and_ground_particle(
    int   record_group_index,
    char *joint_index_offset,
    int   enable_record_dispatch) __asm__("FUN_L00_00205f60");

void emit_hero_animation_voice_and_ground_particle(int   record_group_index,
                                                   char *joint_index_offset,

                                                   int enable_record_dispatch) {
    float ground_adjusted_position[4];
    if (enable_record_dispatch) {
        struct Hero *g = &hero;
        int          item_moby_is_type_ad = 0;
        if (((char *)g->items[1].moby)) {
            item_moby_is_type_ad = *(s16 *)(((char *)g->items[1].moby) + 0xA6) == 0xAD;
        }
        request_grouped_voice(
            record_group_index, joint_index_offset, item_moby_is_type_ad, 0, ((int)g->moby));
    }
    if (current_level_index == 12) {
        struct Hero *g = &hero;
        if (g->ground_sound_class == 0 && ((int)g->ground_moby) == 0) {
            float k = 0.15f;
            moby_point_position(
                ((int)g->moby), joint_index_offset + 0x16, ground_adjusted_position);
            ground_adjusted_position[0] += fast_cos(g->motion.rot.f[2]) * k;
            ground_adjusted_position[1] += fast_sin(g->motion.rot.f[2]) * k;
            ground_adjusted_position[2] =
                probe_ground_height(ground_adjusted_position, 0, 0.5f) + 0.04f;
            spawn_texture_indexed_rotated_particle(
                ground_adjusted_position, joint_index_offset == 0, g->motion.rot.f[2]);
        }
    }
}

extern int test_hero_animation_frame_window(float) __asm__("FUN_L00_00211870");
extern void
emit_hero_animation_voice_and_ground_particle(int, char *, int) __asm__("FUN_L00_00205f60");

void dispatch_hero_animation_timed_events(void) __asm__("FUN_L00_00206080");

void dispatch_hero_animation_timed_events(void) {
    struct Hero *hero_state = &hero;
    int          k;
    if (hero_state->state.current == 2 && hero_state->state_timer < scale_game_frames(0xF)) {
        return;
    }
    k = hero_state->ground_sound_class;
    if (hero_state->state.current == 2 && hero_state->state_timer == scale_game_frames(0x16)) {
        emit_hero_animation_voice_and_ground_particle(k, 0, 1);
    }
    switch (((unsigned char *)hero.moby)[0x53]) {
    case 3:
        if (hero.anim_changing) {
            return;
        }
        if (test_hero_animation_frame_window(49.5f)) {
            emit_hero_animation_voice_and_ground_particle(k, 0, 1);
        }
        if (test_hero_animation_frame_window(17.0f)) {
            emit_hero_animation_voice_and_ground_particle(k, (char *)1, 1);
        }
        break;
    case 4:
        if (hero.anim_changing) {
            return;
        }
        if (test_hero_animation_frame_window(12.5f)) {
            emit_hero_animation_voice_and_ground_particle(k, 0, 1);
        }
        if (test_hero_animation_frame_window(1.0f)) {
            emit_hero_animation_voice_and_ground_particle(k, (char *)1, 1);
        }
        break;
    }
}

#include "qcopy.h"

typedef float M4_2061f0[16] __attribute__((aligned(16)));
void          matrix_to_euler(void *, void *) __asm__("FUN_00214598");
void          blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
void          advance_moby_animation(void *) __asm__("FUN_0020d580");
void          update_entity(void *) __asm__("FUN_L00_00250df8");
void          update_hero_bone_attachment(void) __asm__("FUN_L00_002061f0");

void update_hero_bone_attachment(void) {
    M4_2061f0    bone_transform;
    struct Hero *hero_state = &hero;
    char        *destination_moby = ((char *)hero_state->unkA88);
    struct Moby *hero_moby;
    destination_moby[0x7F] = 0;
    hero_moby = ((char *)hero_state->moby);
    if ((hero_moby->flags ^ 1) & 1) {
        build_moby_bone_transform(hero_moby, 5, bone_transform);
        qcopy(destination_moby + 0x10, &bone_transform[12]);
        copy_matrix3x4(destination_moby + 0xC0, bone_transform);
        matrix_to_euler(bone_transform, destination_moby + 0x40);
        MOBY(destination_moby)->flags &= 0xFFBE;
        blend_moby_animation(destination_moby, 0x81, 0, scale_game_frames(10));
        advance_moby_animation(destination_moby);
        update_entity(destination_moby);
    } else {
        MOBY(destination_moby)->flags |= 0x41;
    }
}

#include "rnc/overlay/quad.h"

extern char               D_0013F350_002062b0[] __asm__("D_0013F350");
extern f32                D_0015EE18_macro __asm__("D_0015EE18") MACRO_ADDR;
extern volatile f32       frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern f32                frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern f32                D_0015ED60_gp __asm__("D_0015ED60_gp") __attribute__((sda));
extern f32                D_0015EE18_gp __attribute__((sda));
extern u8                 D_0015EDB3_macro __asm__("D_0015EDB3") MACRO_ADDR;
extern s32 D_L00_0015F5CC MACRO_ADDR;
extern f32                D_L00_0017B85C_lui __asm__("D_L00_0017B85C") NOT_SDA;
extern void               FUN_L00_00211670(void);
extern void apply_hero_selected_flags_and_state_transitions(void) __asm__("FUN_L00_0020b960");
extern void check_ledge_grab(void) __asm__("FUN_L00_0020bed0");
extern void update_hero_motion_response_parameters(void) __asm__("FUN_L00_00215b68");
extern void FUN_L00_00229b70(void);
extern void update_hero_delayed_voice_requests(void) __asm__("FUN_L00_00216f10");
extern void update_hero_easing_records(void) __asm__("FUN_L00_002054e8");
void        update_hero_vehicle_moby(void) __asm__("FUN_L00_00208a10");

extern void update_hero_vehicle_moby(void) __asm__("FUN_L00_00208a10");
extern void update_hero_scalar_pulse(void) __asm__("FUN_L00_00207c48");
extern void apply_hero_and_item_scale_sequence(void) __asm__("FUN_L00_00207a58");
extern void update_hero_glow_and_extra_manipulators(void) __asm__("FUN_L00_002059d8");
extern void enqueue_hero_glow_draw_callback_once(void) __asm__("FUN_L00_00207d00");
extern void update_hero_moby_probed_height_bounds(void) __asm__("FUN_L00_00208b60");
extern void FUN_L00_00257024(int);
extern void update_entity(void *) __asm__("FUN_L00_00250df8");
extern void FUN_L00_0024f7c8(void *, s32, void *);
extern void blend_moby_animation_2062b0(void *, s32, s32, s32) __asm__("FUN_L00_00257880");
extern int  FUN_L00_0028dbe8(void *, s32, void *);
extern void allocate_hero_voice_for_entry(s32, s32) __asm__("FUN_L00_00216de8");
extern s32  FUN_L00_00200298(f32, s32, s32);
extern f32  FUN_L00_001ff800(f32);
extern f32  FUN_L00_002000e8(f32, f32);
extern s32  FUN_L00_00200250(f32);
extern f32  FUN_L00_00200240(s32);
extern s32  FUN_L00_001fef20(s32);
extern s32  FUN_L00_001fef98(void *);
extern void FUN_L00_0025b868(f32, f32, void *);

void update_hero_in_vehicle_mode(void) __asm__("FUN_L00_002062b0");

void update_hero_in_vehicle_mode(void) {
    enum {
        GREEN_GLOW_PALETTE = 1,
        YELLOW_GLOW_PALETTE = 2,
        RED_GLOW_PALETTE = 3,
        BLUE_GLOW_PALETTE = 4
    };
    char *g;
    char *moby;
    /* The two transform paths reuse the retail 0xA0-byte scratch region. */
    union {
        OvlQuad rotation[10];
        struct {
            OvlQuad reserved;
            OvlQuad transform[4];
            OvlQuad offset;
            OvlQuad inverse[4];
        } help;
    } scratch;
    f32 phase, scale, wave;
    s32 i, span, red, green, blue, alpha;
    s32 color, color2;

    advance_hero_animation();
    FUN_L00_00211670();
    apply_hero_selected_flags_and_state_transitions();
    g = (char *)&hero;
    if (*(u8 *)(g + 0x20a4) == 1) {
        check_ledge_grab();
        update_hero_motion_response_parameters();
    }
    FUN_L00_00229b70();
    if (*(u8 *)(g + 0x20a4) == 0)
        return;
    update_hero_delayed_voice_requests();
    update_hero_easing_records();
    if (*(u8 *)(g + 0x20a4) == 0)
        return;
    update_hero_vehicle_moby();
    update_hero_scalar_pulse();
    apply_hero_and_item_scale_sequence();
    if (*(u8 *)(g + 0x20a4) == 1) {
        D_L00_0017B85C_lui = D_0015EE18_macro;
        update_hero_glow_and_extra_manipulators();
        if (*(char **)(g + 0x1624) == 0) {
            *(char **)(g + 0x1624) = create_moby(0x4b4);
            if (*(char **)(g + 0x1624)) {
                char *moby = *(char **)(g + 0x1624);
                MOBY(moby)->glow_color = 0x801432d7;
                *(u32 *)(g + 0x2300) = 0x301432d7;
                moby = *(char **)(g + 0x1624);
                qcopy(moby + 0x10, g + 0x80);
                qcopy(moby + 0x40, g + 0x90);
                MOBY(moby)->draw_dist = 0x40;
                (*(char **)(g + 0x1624))[0x31] = 1;
                (*(char **)(g + 0x1624))[0x30] = 0;
                *(u16 *)(*(char **)(g + 0x1624) + 0x34) = 0;
                *(f32 *)(*(char **)(g + 0x1624) + 0x2c) *= 1.7f;
                *(u16 *)(*(char **)(g + 0x1624) + 0x34) |= 0x10;
                update_entity(*(char **)(g + 0x1624));
            }
        }
        {
            char *g = (char *)&hero;
            if ((*(u16 *)(*(char **)(g + 0x2080) + 0x34) ^ 1) & 1) {
                FUN_L00_0024f7c8(*(char **)(g + 0x2080), 7, scratch.rotation);
                qcopy(g + 0x1d80, &scratch.rotation[0]);
                enqueue_hero_glow_draw_callback_once();
                if (*(char **)(g + 0x1624) != 0) {
                    build_moby_bone_transform(*(char **)(g + 0x2080), 7, scratch.help.transform);
                    qcopy_nc(*(char **)(g + 0x1624) + 0x10, &scratch.help.transform[3]);
                    copy_matrix3x4(*(char **)(g + 0x1624) + 0xc0, scratch.help.transform);
                    scratch.help.offset = 0;
                    ((f32 *)&scratch.help.offset)[2] = 0.0257f;
                    transform_vector_by_basis(
                        &scratch.help.offset, &scratch.help.offset, *(char **)(g + 0x1624) + 0xc0);
                    *(f32 *)(*(char **)(g + 0x1624) + 0x18) -= ((f32 *)&scratch.help.offset)[2];
                    update_entity(*(char **)(g + 0x1624));
                    scale = 1.7f;
                    if (D_0015EDB3_macro)
                        scale = 3.1f;
                    if (*(s16 *)(g + 0x22fc))
                        scale *= 1.4f;
                    FUN_L00_0025b868(*(f32 *)(*(char **)(*(char **)(g + 0x1624) + 0x24) + 0x24) *
                                         scale,
                                     0.02f,
                                     *(char **)(g + 0x1624) + 0x2c);
                }
            }
        }
        {
            char *g = (char *)&hero;
            color = 0x281414c8;
            color2 = 0x1e00004b;
            {
                char *g = (char *)&hero;
                *(f32 *)(g + 0x1628) =
                    FUN_L00_002000e8(*(f32 *)(g + 0x1628), frame_time_macro * 2.9670596f);
            }
            if (FUN_L00_001fef98(g + 0x22fc) == 0) {
                char *g = (char *)&hero;
                *(f32 *)(g + 0x1628) =
                    FUN_L00_002000e8(*(f32 *)(g + 0x1628), frame_time_macro * 12.217304f);
                switch (*(s16 *)(g + 0x22fe)) {
                case GREEN_GLOW_PALETTE:
                    color = 0x2814c814;
                    color2 = 0x1e004b00;
                    break;
                case YELLOW_GLOW_PALETTE:
                    color = 0x2814c8c8;
                    color2 = 0x1e004b4b;
                    break;
                case RED_GLOW_PALETTE:
                    color = 0x323232fa;
                    color2 = 0x1e000078;
                    break;
                case BLUE_GLOW_PALETTE:
                    color = 0x28f01414;
                    color2 = 0x1e4b0000;
                    break;
                }
            }
            {
                char *g = (char *)&hero;
                s32   tint;
                wave = FUN_L00_001ff800(*(f32 *)(g + 0x1628));
                tint = FUN_L00_00200298(wave * 0.5f + 0.5f, color, color2);
                if (*(char **)(g + 0x1624))
                    *(u32 *)(*(char **)(g + 0x1624) + 0x90) = FUN_L00_00200298(
                        D_0015ED60_gp * 0.1f, *(u32 *)(*(char **)(g + 0x1624) + 0x90), tint);
                *(u32 *)(g + 0x2300) =
                    FUN_L00_00200298(frame_scale_macro * 0.1f, *(u32 *)(g + 0x2300), tint);
                if (*(char **)(g + 0x1620) == 0) {
                    *(char **)(g + 0x1620) = create_moby(0x47a);
                    if (*(char **)(g + 0x1620)) {
                        char *moby = *(char **)(g + 0x1620);
                        qcopy(moby + 0x10, g + 0x80);
                        qcopy(moby + 0x40, g + 0x90);
                        MOBY(moby)->draw_dist = 0x40;
                        (*(char **)(g + 0x1620))[0x31] = 1;
                        (*(char **)(g + 0x1620))[0x30] = 0;
                        *(u16 *)(*(char **)(g + 0x1620) + 0x34) = 0;
                        update_entity(*(char **)(g + 0x1620));
                    }
                }
                if (*(char **)(g + 0x1620)) {
                    build_moby_bone_transform(*(char **)(g + 0x2080), 5, scratch.rotation);
                    {
                        OvlQuad *position = &scratch.rotation[3];
                        char    *moby = *(char **)(g + 0x1620);
                        qcopy_nc(moby + 0x10, position);
                        update_entity(moby);
                    }
                    clear_u64_value((scratch.rotation + 4));
                    ((f32 *)(scratch.rotation + 4))[2] = -*(f32 *)(g + 0x98);
                    euler_rotation_matrix(scratch.help.inverse, (scratch.rotation + 4));
                    multiply_matrix(scratch.rotation, scratch.rotation, scratch.help.inverse);
                    copy_matrix3x4(*(char **)(g + 0x1620) + 0xc0, scratch.rotation);
                    *(u64 *)(*(char **)(g + 0x1620) + 0x38) =
                        *(u64 *)(*(char **)(g + 0x2080) + 0x38);
                    if (*(s32 *)(g + 0x2084) == 0x4f) {
                        if (*(u8 *)(*(char **)(g + 0x1620) + 0x53) != 1)
                            blend_moby_animation_2062b0(
                                *(char **)(g + 0x1620), 1, 0, FUN_L00_001fef20(10));
                    } else if (*(u8 *)(*(char **)(g + 0x1620) + 0x53) != 0) {
                        blend_moby_animation_2062b0(
                            *(char **)(g + 0x1620), 0, 0, FUN_L00_001fef20(0x11));
                    }
                }
            }
        }
        {
            char *g = (char *)&hero;
            if (*(char **)(g + 0x22c0)) {
                FUN_L00_0028dbe8(*(char **)(g + 0x22c0) + 0x10, 0, *(char **)(g + 0x2080));
                *(s16 *)(g + 0x22fe) = *(u16 *)(g + 0x22c0);
                *(s16 *)(g + 0x22fc) = FUN_L00_001fef20(0x39);
            }
            *(u32 *)(g + 0x22c0) = 0;
        }
    }
    reveal_map_around_position(&hero.motion.pos);
    if (hero.vehicle_mode == 2) {
        char *g = (char *)&hero;
        if (D_0015EDB3_macro)
            *(f32 *)((char *)&D_L00_0017B85C_lui + 0x210) = 1.4f * D_0015EE18_gp;
        update_hero_glow_and_extra_manipulators();
        update_hero_bone_attachment();
        g = (char *)&hero;
        if ((*(u16 *)(*(char **)(g + 0x2080) + 0x34) ^ 1) & 1) {
            FUN_L00_0024f7c8(*(char **)(g + 0x2080), 6, scratch.rotation);
            /* Retail uses &hero.motion.pos + 0x1d00, hence hero + 0x1d80. */
            qcopy(g + 0x1d80, &scratch.rotation[0]);
            enqueue_hero_glow_draw_callback_once();
        }
    }
    if (hero.vehicle_mode == 3) {
        s32 color;
        /* D_0015EDB1 is the byte two bytes before the checked D_0015EDB3 base. */
        if (*((u8 *)&D_0015EDB3_macro - 2))
            *(f32 *)((char *)&D_L00_0017B85C_lui + 0x370) = 1.9f;
        {
            char *g = (char *)&hero;
            alpha = 0x80;
            span = FUN_L00_001fef20(0xaa);
            if (*(s16 *)(g + 0x22e0) == 2)
                span = FUN_L00_001fef20(0x32);
            else if (*(s16 *)(g + 0x22e0) == 1)
                span = FUN_L00_001fef20(0x5a);
            phase = FUN_L00_00200240(span);
            wave = FUN_L00_001ff800((((f32)(D_L00_0015F5CC % span) / phase) * 2.0f) * 3.1415927f +
                                    -3.1415927f);
        }
        red = FUN_L00_00200250(wave * 20.0f) + 0x32;
        green = FUN_L00_00200250(wave * 70.0f) + 0xb4;
        blue = FUN_L00_00200250(wave * 10.0f) + 0x14;
        {
            char *g = (char *)&hero;
            {
                s32 span = red < 0x100;
                s32 i = green < 0x100;
                green = i ? green : 0xff;
                *(u16 *)(*(char **)(g + 0x2080) + 0x34) |= 0x10;
                red = span ? red : 0xff;
            }
            if (*(s16 *)(g + 0x22e0) == 2)
                color = ((u32)alpha << 24) | ((u32)blue << 16) | ((u32)(red / 2) << 8) | (u32)green;
            else
                color = ((u32)alpha << 24) | ((u32)blue << 16) | ((u32)green << 8) | (u32)red;
        }
        {
            char *g;
            {
                char *g = (char *)&hero;
                *(u32 *)(*(char **)(g + 0x2080) + 0x90) =
                    FUN_L00_00200298(0.1f, *(u32 *)(*(char **)(g + 0x2080) + 0x90), color);
            }
            g = (char *)&hero;
            for (i = 0; i < 3; i++)
                FUN_L00_0024f7c8(hero.moby, i, g + 0x1d50 + i * 0x10);
            g = (char *)&hero;
            moby = *(char **)(g + 0x2080);
            /* Retail snapshots these channels before the alpha helper calls. */
            blue = *(u8 *)(moby + 0x92);
            green = *(u8 *)(moby + 0x91);
            red = *(u8 *)(moby + 0x90);
            alpha = 0x30;
            if (*(s16 *)(g + 0x22de))
                alpha = FUN_L00_00200250(FUN_L00_00200240(*(s16 *)(g + 0x22de)) /
                                         FUN_L00_00200240(0x12) * (f32)alpha);
            *(u32 *)(g + 0x22e4) = (alpha << 24) | ((u32)blue << 16) | ((u32)green << 8) | (u32)red;
            enqueue_hero_glow_draw_callback_once();
            *(s16 *)(g + 0x22e0) = 0;
            if (*(u8 *)(g + 0x20a4) == 3) {
                if (*(s32 *)(g + 0x20b8) != 0 && *(s32 *)(g + 0x20b8) != 0x1f) {
                    if (*(s16 *)(g + 0x22de) == 0) {
                        allocate_hero_voice_for_entry(0x19, 0);
                        *(s16 *)(g + 0x22de) = 0x12;
                        *(s16 *)(g + 0x22dc) = 1;
                    }
                } else {
                    char *g = (char *)&hero;
                    *(s32 *)(g + 0x20b8) = 0;
                    if (*(u8 *)(g + 0x12e7) && *(s32 *)(g + 0x300) &&
                        *(u8 *)(D_0013F350_002062b0 - 0x1e74) && *(s16 *)(g + 0x22de) == 0) {
                        allocate_hero_voice_for_entry(0x19, 0);
                        *(s16 *)(g + 0x22de) = 0x12;
                        *(s16 *)(g + 0x22dc) = 1;
                    }
                }
            }
        }
    }
    update_hero_moby_probed_height_bounds();
    FUN_L00_00257024((s32)hero.moby);
}

#include "rnc/audio/voice_pool.h"

/* Stops the sounds the hero still owns in its 8 sound slots and clears them. */
void release_and_clear_hero_voice_slots(void) __asm__("FUN_L00_00206c08");

void release_and_clear_hero_voice_slots(void) {
    s32          *slot;
    s32           i;
    s32           expected_owner_byte_offset;
    s32           voice;
    struct Moby  *moby;
    s32           active;
    struct Hero  *hero_state;
    struct Moby **mobys;

    hero_state = &hero;
    mobys = hero_state->unk2238;
    slot = &hero_state->grind_voice;
    expected_owner_byte_offset = 0;
    for (i = 7; i >= 0; i--) {
        moby = *(struct Moby **)(expected_owner_byte_offset + (s32)mobys);
        voice = *slot;
        if (moby != 0) {
            if (voice == -1 || voice_pool.voices[voice].owner != (VoiceMoby *)moby) {
                goto clear;
            }
            active = voice_pool.voices[voice].state;
        } else {
            if (voice == -1 || voice_pool.voices[voice].owner != (VoiceMoby *)hero_state->moby) {
                goto clear;
            }
            active = voice_pool.voices[voice].state;
        }
        if (active) {
            release_voice_slot(voice);
        }
    clear:
        *slot = -1;
        slot++;
        /* Fakematch: the empty block makes retail step slot before off. */
        do {
        } while (0);
        expected_owner_byte_offset += 4;
    }
    i = 0;
}

#include "sda.h"
#include "rnc/overlay/quad.h"

typedef struct {
    u8  p[0x280];
    u16 x280;
    u8  p282[0x13E];
    u16 x3c0;
} G00206cf8;

extern G00206cf8      help_records __asm__("D_00141968");
extern s32            D_L00_0015F5CC_00206cf8 __asm__("D_L00_0015F5CC");
s32                   hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");
extern u16 D_001418F0 NOT_SDA;
void                  check_hero_item_reward_conditions(void) __asm__("FUN_L00_00206cf8");

void check_hero_item_reward_conditions(void) {
    s32 i, qualifying_item_count;
    if (help_records.x3c0 == 0 && item_available[4] != 0 && hero_is_in_special_state() != 0) {
        help_request(0x4E2E, 0x78);
    }
    if (D_L00_0015F5CC_00206cf8 <= 0)
        return;
    if (D_L00_0015F5CC_00206cf8 & 0x7F)
        return;
    if (help_records.x280 != 0)
        return;
    if (D_001418F0 != 0)
        return;
    qualifying_item_count = 0;
    for (i = 0; i < 0x25; i++) {
        if (item_defs[i].slot == 0 && i != 8 && i != 0x18 && item_available[i] != 0)
            qualifying_item_count++;
    }
    if (qualifying_item_count >= 9)
        help_request(0x4E26, 0x50);
}
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00207418), where it is exact; names translated to the US level program. */

extern int D_0015EEA0 MACRO_ADDR;
extern unsigned char  big_head_cheat_enabled_macro[] __asm__("D_0015EDB0") MACRO_ADDR;
extern unsigned char  D_0015EDB4[4] MACRO_ADDR;
extern unsigned char  D_0015EDC0[] MACRO_ADDR;

void scan_hero_input_sequences(void) __asm__("FUN_L00_00206e00");

void scan_hero_input_sequences(void) {
    char *g;
    int   input_token;
    int   i;
    if (D_0015EEA0 == 0 && alternate_animation_sequence == 0)
        return;
    g = (char *)&hero;
    input_token = 0;
    if (*(int *)(g + 0x198) == scale_game_frames(0xF)) {
        if (*(int *)(g + 0x208C) == 4) {
            int b = *(int *)(g + 0x2084);
            if (b == 0xB) {
                int token_variant_selector = *(int *)(g + 0x450);
                if (token_variant_selector == 0) {
                    input_token = 1;
                    if (D_0015EDB4[0])
                        input_token = 2;
                } else if (token_variant_selector == 1) {
                    input_token = 2;
                    if (D_0015EDB4[0])
                        input_token = 1;
                } else if (token_variant_selector == 3) {
                    input_token = 3;
                }
            } else if (b == 0x11) {
                input_token = 9;
            } else if (b == 0xA) {
                input_token = 0xA;
            } else if (b == 0xE) {
                input_token = 5;
            }
        } else if (*(int *)(g + 0x208C) == 6) {
            int b = *(int *)(g + 0x2084);
            if (b == 0x14) {
                input_token = 6;
            } else if (b == 0x13) {
                input_token = *(int *)(g + 0xA60) + 0xC;
            } else if (b == 0x15) {
                input_token = 4;
            }
        } else if (*(int *)(g + 0x2084) == 0x22) {
            input_token = 0xB;
        }
    } else if (*(int *)(g + 0x198) == scale_game_frames(0x3C)) {
        if (*(int *)(g + 0x2084) == 4) {
            input_token = 7;
        } else if (*(int *)(g + 0x2084) == 8) {
            input_token = 8;
        }
    }
    if (input_token == 0)
        return;
    {
        char          *history_owner = (char *)&hero;
        unsigned char *input_history = (unsigned char *)history_owner + 0x21C4;
        input_history[*(int *)(history_owner + 0x21D4)] = input_token;
        *(int *)(history_owner + 0x21D4) += 1;
        for (i = 0; i < 12; i++) {
            char          *hero_history_owner = (char *)&hero;
            unsigned char *b = (unsigned char *)hero_history_owner + 0x21C4;
            int            sequence_length = 0;
            int            j;
            int            k;
            while (D_L00_00179700[sequence_length + i * 16] != 0)
                sequence_length++;
            if (sequence_length == 0)
                continue;
            k = *(int *)(hero_history_owner + 0x21D4) - sequence_length;
            if (k < 0)
                k += 16;
            for (j = 0; j < sequence_length; j++) {
                if (*(unsigned char *)(k + (int)b) != D_L00_00179700[j + i * 16]) {
                    j = -1;
                    break;
                }
                k++;
                if (k >= 16)
                    k -= 16;
            }
            if (j == sequence_length) {
                int toggled_selector = big_head_cheat_enabled_macro[i] == 0;
                D_0015EDC0[i] = 1;
                big_head_cheat_enabled_macro[i] = toggled_selector;
                queue_dialogue_message(toggled_selector ? 0x4FBE : 0x4FBF, -1);
                break;
            }
        }
    }
    {
        char *r = (char *)&hero;
        if (*(int *)(r + 0x21D4) >= 16)
            *(int *)(r + 0x21D4) = 0;
    }
}

void hero_update(void) __asm__("FUN_L00_00211670");
void apply_hero_selected_flags_and_state_transitions(void) __asm__("FUN_L00_0020b960");
void check_ledge_grab(void) __asm__("FUN_L00_0020bed0");
void find_grind_rail(void) __asm__("FUN_L00_0020cf58");
void update_hero_offset_rail_candidate(void) __asm__("FUN_L00_0020d330");
void hero_update_transitions(void) __asm__("FUN_L00_00229b70");
void update_hero_delayed_voice_requests(void) __asm__("FUN_L00_00216f10");
void update_hero_easing_targets_and_point_effects(void) __asm__("FUN_L00_0020b1d8");
void update_hero_collision_ease_target(void) __asm__("FUN_L00_0020a318");

void update_hero_pitch_and_yaw_easing_targets(void) __asm__("FUN_L00_0020a540");
void update_hero_target_tracking(void) __asm__("FUN_L00_0020ac98");
void update_hero_idle_random_rotation_targets(void) __asm__("FUN_L00_0020a9e8");
void update_hero_transformed_negative_z_angle_parameters(void) __asm__("FUN_L00_00214fe8");
void update_hero_moby(void) __asm__("FUN_L00_00208820");

void update_hero_world_matrices(s32 moby_override) __asm__("FUN_L00_00209240");

void update_hero_world_matrices(int) __asm__("FUN_L00_00209240");
void update_hero_equipped_items(void) __asm__("FUN_L00_00210928");
void update_hero_scalar_pulse(void) __asm__("FUN_L00_00207c48");
void apply_hero_and_item_scale_sequence(void) __asm__("FUN_L00_00207a58");
void noop_main_callback(int) __asm__("FUN_001e93e8");
void FUN_L00_00257024(int);
void update_hero_color_and_item_tint(void) __asm__("FUN_L00_002076a8");

void update_hero_moby_probed_height_bounds(void) __asm__("FUN_L00_00208b60");
void coordinate_hero_frame_update(void) __asm__("FUN_L00_002070d0");

void coordinate_hero_frame_update(void) {
    struct Hero *hero_state = &hero;
    if (hero_state->motion.pos.f[0] < 2.0f || hero_state->motion.pos.f[1] < 2.0f ||
        1022.0f < hero_state->motion.pos.f[0] || 1022.0f < hero_state->motion.pos.f[1]) {
        fade_to_black(scale_game_frames(0x10));
        {
            struct Hero *g2 = &hero;
            g2->fall_fade_pending = 1;
        }
        return;
    }
    hero_state->unk1EB2 = 0;
    hero_state->unk1EB0 = 0;
    if (hero_state->vehicle_mode) {
        update_hero_in_vehicle_mode();
        return;
    }
    advance_hero_animation();
    hero_update();
    apply_hero_selected_flags_and_state_transitions();
    check_ledge_grab();
    find_grind_rail();
    update_hero_offset_rail_candidate();
    hero_update_transitions();
    if (hero_state->vehicle_mode)
        return;
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
    noop_main_callback((int)hero_state->moby);
    if (hero_state->state.control_mode != 0x16 && hero_state->state.current != 0x32)
        reveal_map_around_position(&hero_state->motion.pos);
    {
        struct Hero *g2 = &hero;
        FUN_L00_00257024((int)g2->moby);
    }
    update_hero_color_and_item_tint();
    update_hero_moby_probed_height_bounds();
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_002078E0), where it is exact; names translated to the US level program. */

void copy_hero_color_to_item_mobys(void) __asm__("FUN_L00_002072c8");

void copy_hero_color_to_item_mobys(void) {
    int   i, j;
    char *hero_base = ((char *)&hero);
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 2; j++) {
            char *hero_item_stride_base = hero_base + i * 0x50;
            int   destination_moby = *(int *)(hero_item_stride_base + 0x1090);
            if (j == 1) {
                destination_moby = *(int *)(hero_item_stride_base + 0x1094);
            }
            if (destination_moby != 0) {
                MOBY(destination_moby)->spawn_frame =
                    *(long *)(*(int *)(hero_base + 0x2080) + 0x38);
            }
        }
    }
}
/* Tint the effect from the moby's colour bytes, then set the four shape floats from its vector sum. */
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00207948), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} Vx __attribute__((aligned(16)));

extern char D_L00_001801D0[] NOT_SDA;
extern void pack_render_command_fields(void *, s32, s32, s32, s32) __asm__("FUN_0020d4f0");
extern void get_moby_light_direction(void *, void *) __asm__("FUN_0020d510");
extern void FUN_L00_002503c0(void *, void *);

void update_item_tint_from_hero(void *item_moby, char *hero_moby) __asm__("FUN_L00_00207330");

void update_item_tint_from_hero(void *item_moby, char *hero_moby) {
    Vx             hero_vector_sample;
    float         *global_float_base;
    float          vector_component_sum;
    unsigned char *hero_color_block = hero_moby + 0x38;
    int color_channel_sum = hero_color_block[4] + hero_color_block[5] + hero_color_block[6];
    pack_render_command_fields(
        item_moby, color_channel_sum * 4 / 10 + ((color_channel_sum * 3 / 10) << 8), 12, 12, 0);
    get_moby_light_direction(hero_moby, D_L00_001801D0);
    FUN_L00_002503c0(hero_moby, &hero_vector_sample);
    global_float_base = (float *)(D_L00_001801D0 - 0x310);
    vector_component_sum = hero_vector_sample.x + hero_vector_sample.y + hero_vector_sample.z;
    global_float_base[0xC0] = vector_component_sum * 0.5f;
    global_float_base[0xC1] = vector_component_sum * 0.4f;
    global_float_base[0xC2] = vector_component_sum * 0.0f;
    global_float_base[0xC3] = -0.3f;
}

extern int           D_15F5CC_207430 __asm__("D_L00_0015F5CC");
extern unsigned char D_0015EDB3_lui __asm__("D_0015EDB3") NOT_SDA;
extern float         D_0015EE18 __asm__("D_0015EE18");
extern unsigned char D_L00_0017C380_lui[] __asm__("D_L00_0017C380") NOT_SDA;
extern unsigned char D_L00_0017C3C0_lui[] __asm__("D_L00_0017C3C0") NOT_SDA;
extern unsigned char D_L00_0017C400_lui[] __asm__("D_L00_0017C400") NOT_SDA;
extern void          FUN_L00_00208038_c(void *, int, float, float) __asm__("FUN_L00_00208038");

extern float D_0015EE18_gp __attribute__((sda));
void         update_moby_pulsing_glow_effects(struct Moby *moby) __asm__("FUN_L00_00207430");

void update_moby_pulsing_glow_effects(struct Moby *moby) {
    union {
        OvlQuad q;
        float   f[4];
    } v;
    int            k, red_glow_offset, green_glow_offset;
    float          f, glow_wave, red_blue_swing;
    unsigned char *manipulator;

    moby->flags |= 0x10;
    k = scale_game_frames(100);
    f = convert_integer_to_float(k);
    glow_wave = fast_sin((float)(D_15F5CC_207430 % k) / f * 2.0f * 3.1415927f + -3.1415927f);
    red_blue_swing = glow_wave * 24.0f;
    red_glow_offset = truncate_float_to_s32(red_blue_swing);
    green_glow_offset = truncate_float_to_s32(glow_wave * 52.0f);
    {
        int glow_color = ((truncate_float_to_s32(red_blue_swing) + 0x4C) << 16) | 0x80000000;
        glow_color |= (green_glow_offset + 0xA2) << 8;
        glow_color |= red_glow_offset + 0x44;
        moby->glow_color = glow_color;
    }
    if (moby->oclass == 0x1A3) {
        moby_point_position(moby, 6, &v);
        v.f[2] -= 0.07f;
        if (2.0f < absolute_float(moby->pos.z - v.f[2])) {
            FUN_L00_00208038_c(&v, 0x200000C0, 0.25f, 0.15f);
        }
    } else {
        moby_point_position(moby, 1, &v);
        v.f[2] -= 0.00700000022f;
        FUN_L00_00208038_c(&v, 0x200000C0, 0.0469999984f, 0.15f);
    }
    manipulator = D_L00_0017C380_lui;
    if (moby->oclass == 0x1A3) {
        manipulator = D_L00_0017C3C0_lui;
    } else if (moby->oclass == 0x555) {
        manipulator = D_L00_0017C400_lui;
    }
    if (*(unsigned char *)0x15EDB3 && manipulator[1] == 0) {
        float x;
        if (moby->oclass == 0x1A3) {
            attach_manipulator(moby, 1, manipulator);
            x = D_0015EE18_gp;
        } else {
            attach_manipulator(moby, 2, manipulator);
            x = D_0015EE18;
        }
        *(float *)(manipulator + 0x20) = x;
        *(float *)(manipulator + 0x24) = x;
        *(float *)(manipulator + 0x28) = x;
    }
}

extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");

void update_hero_color_and_item_tint(void) {
    struct Hero *player = &hero;
    int          blink;

    if (player->color_fade_active != 0) {
        int color = player->tint_color;
        int red = color & 0xff;
        int blue = (color >> 16) & 0xff;
        int green = (color >> 8) & 0xff;
        red -= 7;
        green -= 7;
        blue -= 7;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        {
            int alpha = (unsigned int)color >> 24;
            set_moby_color(((void *)player->moby), red, green, blue);
            player->tint_color = alpha << 24 | blue << 16 | green << 8 | red;
        }
    }

    blink = 0;
    if (hero.state.current == 0x80 || hero.state.current == 0x82) {
        blink = hero.state_timer < scale_game_frames(0x1e);
    }
    if ((current_level_index == 0xf || current_level_index == 0x11) && hero.state.current == 0x76 &&
        hero.state_timer < scale_game_frames(0x14)) {
        blink = 1;
    }
    if (blink) {
        int red, green, blue;
        get_moby_color(((void *)hero.moby), &red, &green, &blue);
        if (hero.state_timer % 4 < 3) {
            red = 0;
            green = 0;
            blue = 0;
        } else {
            red = 0x90;
            green = 0x90;
            blue = 0xf0;
        }
        set_moby_color(((void *)hero.moby), red, green, blue);
        copy_hero_color_to_item_mobys();
    }
    if (hero.items[0].item_id >= 0 && item_text_variant[hero.items[0].item_id] != 0 &&
        ((int)hero.items[0].moby) != 0) {
        update_item_tint_from_hero(((void *)hero.items[0].moby), ((char *)hero.moby));
    }
}

typedef unsigned int u128_2078a8 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2078a8 q;
    float       f[4];
} Vec4_2078a8;

typedef struct {
    u8    p0[0x6A8];
    float f6A8;
    float f6AC;
    u8    p1[0x86C - 0x6B0];
    char *o86C;
    u8    p2[0x8BC - 0x870];
    s16   h8BC;
    u8    p3[0x2080 - 0x8BE];
    char *w2080;
    int   w2084;
} S13F350;

extern float D_0015ED6C;
void         update_moby_vertical_oscillation(char *, float *, float *, float, float) __asm__("FUN_L00_00262b00");
extern char D_L00_0017A6C0[] NOT_SDA;
void        apply_hero_vertical_oscillation(void) __asm__("FUN_L00_002078a8");

void apply_hero_vertical_oscillation(void) {
    struct Hero *P = &hero;
    Vec4_2078a8  position_offset;
    float        position_offset_rotation_matrix[16] __attribute__((aligned(16)));
    if (P->state.current == 0x7F) {
        update_moby_vertical_oscillation(
            P->moby, &P->unk6A8, &P->unk6AC, 0.3f, D_0015ED6C * 6.981317f);
    }
    if (P->state.current == 0x6B && P->hoverboard_airborne == 0) {
        update_moby_vertical_oscillation(
            P->moby, &P->unk6A8, &P->unk6AC, 1.1f, D_0015ED6C * 5.2359877f);
        if (P->hoverboard_moby) {
            char *hero_moby = P->moby;
            qcopy(((char *)P->hoverboard_moby) + 0x10, hero_moby + 0x10);
            position_offset.q = 0;
            position_offset.f[2] = 0.21f;
            transform_vector_by_basis(&position_offset, &position_offset, hero_moby + 0xC0);
            euler_rotation_matrix(position_offset_rotation_matrix, D_L00_0017A6C0);
            transform_vector(&position_offset, &position_offset, position_offset_rotation_matrix);
            fast_vec_add(((char *)P->hoverboard_moby) + 0x10,
                         ((char *)P->hoverboard_moby) + 0x10,
                         &position_offset);
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00208000), where it is exact; names translated to the US level program. */

extern void update_moby_rotation_oscillation(int, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");

void apply_hero_rotation_oscillation(void) __asm__("FUN_L00_002079e8");

void apply_hero_rotation_oscillation(void) {
    struct Hero *hero_state = &hero;
    if (hero_state->state.current == 0x7F) {
        update_moby_rotation_oscillation(((int)hero_state->moby),
                                         &hero_state->unk6B0,
                                         &hero_state->unk6B4,
                                         0.1745329201221466f,
                                         frame_time * 1.5707963705062866f,
                                         frame_time * 2.094395160675049f);
    }
}

extern float D_L00_001797D8[] NOT_SDA;

void apply_hero_and_item_scale_sequence(void) {
    struct Hero   *G = &hero;
    OvlQuad        planar_offset[1];
    float          sequence_scale, item_scale;
    int            i, j;
    unsigned char *item_moby;
    if (G->unk22DC) {
        sequence_scale = D_L00_001797D8[++G->unk22DC];
        if (sequence_scale == -1.0f) {
            G->unk22DC = 0;
            return;
        }
        fast_vec_scale(
            ((unsigned char *)G->moby) + 0xC0, ((unsigned char *)G->moby) + 0xC0, sequence_scale);
        fast_vec_scale(
            ((unsigned char *)G->moby) + 0xD0, ((unsigned char *)G->moby) + 0xD0, sequence_scale);
        if (G->vehicle_mode == 0) {
            for (i = 0; i < 7; i++) {
                for (j = 0; j < 3; j++) {
                    struct Hero *hero_state = &hero;
                    item_moby = *(unsigned char **)&hero_state->items[i].moby;
                    if (j == 1)
                        item_moby = *(unsigned char **)&hero_state->items[i].moby2;
                    item_scale = sequence_scale;
                    if (i == 0 && hero_state->vehicle_mode == 0 && 1.0f < sequence_scale)
                        item_scale = 1.0f;
                    if (item_moby) {
                        fast_vec_scale(item_moby + 0xC0, item_moby + 0xC0, item_scale);
                        fast_vec_scale(item_moby + 0xD0, item_moby + 0xD0, item_scale);
                        fast_vec_sub(planar_offset, item_moby + 0x10, &hero.motion.body_center);
                        gravity_set_z(planar_offset, planar_offset, 0.0f);
                        fast_vec_sub(item_moby + 0x10, item_moby + 0x10, planar_offset);
                        fast_vec_scale(planar_offset, planar_offset, sequence_scale);
                        fast_vec_add(item_moby + 0x10, item_moby + 0x10, planar_offset);
                    }
                }
            }
        }
    }
}

extern int   hero_scalar_pulse_active_gp __asm__("D_L00_0015F680") __attribute__((sda));
extern int   hero_scalar_pulse_phase_gp __asm__("D_L00_0015F684") __attribute__((sda));
extern float hero_scalar_pulse_step_gp __asm__("D_L00_0015F67C") __attribute__((sda));
float        FUN_00213ed8(float *, float, float);

void update_hero_scalar_pulse(void) {
    if (hero_scalar_pulse_active_gp == 0)
        return;
    if (hero_scalar_pulse_phase_gp == 0) {
        FUN_00213ed8(&fade_level, 1.0f, hero_scalar_pulse_step_gp);
        if (fade_level == 1.0f)
            hero_scalar_pulse_phase_gp = 1;
    } else if (hero_scalar_pulse_phase_gp == 1) {
        hero_scalar_pulse_phase_gp = 2;
    } else if (hero_scalar_pulse_phase_gp == 2) {
        FUN_00213ed8(&fade_level, 0.0f, hero_scalar_pulse_step_gp);
        if (fade_level == 0.0f)
            hero_scalar_pulse_active_gp = 0;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00208318), where it is exact; names translated to the US level program. */

extern void draw_hero_glow_quads(void) __asm__("FUN_L00_00207d40");

/* Once (flag at ((char *)&hero)+0x1EB2) queues FUN_L00_00207d40 with the object at +0x2080. */
void enqueue_hero_glow_draw_callback_once(void) {
    struct Hero *hero_state = &hero;
    if (hero_state->unk1EB2 == 0) {
        hero_state->unk1EB2 = 1;
        add_draw_callback(draw_hero_glow_quads, ((int)hero_state->moby));
    }
}

typedef struct {
    f32 f[4];
} V_207d40;

typedef struct {
    u8       p0[0x162C];
    f32      f162C;
    u8       p1[0x1D50 - 0x1630];
    V_207d40 v1D50[3];
    V_207d40 v1D80;
    V_207d40 v1D90[4];
    V_207d40 v1DD0[8];
    s32      i1E50[8];
    f32      f1E70[8];
    f32      f1E90[8];
    s16      h1EB0;
    u8       p2[0x20A4 - 0x1EB2];
    u8       b20A4;
    u8       p3[0x22D8 - 0x20A5];
    s16      h22D8;
    u8       p4[0x22E4 - 0x22DA];
    s32      i22E4;
    u8       p5[0x22FC - 0x22E8];
    s16      h22FC;
    u8       p6[2];
    s32      i2300;
} P_207d40;

extern P_207d40 D_0013F350_lui __asm__("D_0013F350") NOT_SDA;
extern void     draw_offset_textured_effect_quad(void *, s32, f32, f32) __asm__("FUN_L00_00263618");
#define P D_0013F350_lui

void draw_hero_glow_quads(void) {
    f32       k;
    int       i;
    int       vehicle_mode;
    int       glow_color;
    int       mode;
    V_207d40 *glow_position_cursor;

    mode = D_L00_0015F5C4;
    if (mode == 2 || mode == 6) {
        for (i = 0; i < P.h1EB0; i++)
            draw_offset_textured_effect_quad(&P.v1DD0[i], P.i1E50[i], P.f1E70[i], P.f1E90[i]);
        return;
    }
    vehicle_mode = P.b20A4;
    if (vehicle_mode == 3) {
        int j;
        glow_color = P.i22E4;
        glow_position_cursor = P.v1D50;
        for (j = 2; j >= 0; j--) {
            draw_offset_textured_effect_quad(glow_position_cursor, glow_color, 0.2f, 0.08f);
            glow_position_cursor++;
        }
        return;
    }
    if (vehicle_mode == 0) {
        if ((hero_slot_item_id(3) == 3 || hero_slot_item_id(3) == 2) && P.h22D8 == 0) {
            P.v1D80.f[2] += 0.015f;
            draw_offset_textured_effect_quad(&P.v1D80, 0x280000C0, 0.057f, 0.08f);
            P.v1D80.f[2] -= 0.015f;
        }
        for (i = 0; i < P.h1EB0; i++)
            draw_offset_textured_effect_quad(&P.v1DD0[i], P.i1E50[i], P.f1E70[i], P.f1E90[i]);
        return;
    }
    if (vehicle_mode == 2) {
        P.v1D80.f[2] -= 0.07f;
        draw_offset_textured_effect_quad(&P.v1D80, 0x300000C0, 0.25f, 0.08f);
        P.v1D80.f[2] += 0.07f;
        return;
    }
    if (vehicle_mode == 1) {
        k = 0.07f;
        if (P.h22FC)
            k = 0.101500004529953f;
        if (*(u8 *)0x15EDB3) {
            P.v1D80.f[2] -= 0.01f;
            k *= 1.7f;
        } else {
            P.v1D80.f[2] -= 0.02f;
        }
        approach_value(&P.f162C, k, frame_scale * 0.003f);
        draw_offset_textured_effect_quad(&P.v1D80, P.i2300, P.f162C, 0.08f);
    }
}

#include "eetypes.h"

typedef struct {
    char  pad[0x1dd0];
    u128  pos[8];
    int   b[8];
    float x[8];
    float y[8];
    short n;
} G_00208038;

extern G_00208038 D_0013F350_00208038 __asm__("D_0013F350");
void              enqueue_hero_glow_draw_callback_once(void) __asm__("FUN_L00_00207d00");

/* Appends v, b, x and y to the list in ((char *)&hero) and calls FUN_L00_00207d00. */
void FUN_L00_00208038(u128 *glow_position, int b, float x, float y) {
    G_00208038 *hero_glow_queue_owner = &D_0013F350_00208038;
    short       n = hero_glow_queue_owner->n;
    u128       *pos = hero_glow_queue_owner->pos;
    qcopy(&pos[n], glow_position);
    hero_glow_queue_owner->b[n] = b;
    hero_glow_queue_owner->x[hero_glow_queue_owner->n] = x;
    hero_glow_queue_owner->y[hero_glow_queue_owner->n] = y;
    enqueue_hero_glow_draw_callback_once();
    hero_glow_queue_owner->n++;
}

#include "rnc/mobys/moby.h"

typedef int u128_2080b0 __attribute__((mode(TI)));

typedef union {
    u128_2080b0 q;
    float       f[4];
} Vec4_2080b0 __attribute__((aligned(16)));

extern void
queue_glow_sprite_2080b0(void *vec, int a, float f0, float f1) __asm__("FUN_L00_00208038");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern float D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;

// Hero gadget glow: pulses the colour of the hero's gadget moby (hero state 13, 16, 17, 19, 20) and
// queues its glow sprites. c and g are shared by cases 17 and 20 (one pseudo each, retail's $s0/$s1).
void update_hero_and_equipped_item_glows(void) __asm__("FUN_L00_002080b0");

void update_hero_and_equipped_item_glows(void) {
    enum {
        ITEM_RED_GREEN_GLOW = 13,
        ITEM_SINE_RED_GREEN_GLOW = 16,
        ITEM_GRAY_PULSE_GLOW = 17,
        ITEM_RED_BLUE_GLOW = 19,
        ITEM_BRIGHT_GRAY_PULSE_GLOW = 20
    };
    Vec4_2080b0  glow_sprite_position;
    struct Moby *item_moby;
    int          i;
    int          glow_channel_sample, g;

    if (hero.items[2].item_id == 5) {
        struct Moby *glow_source_moby = hero.items[2].moby;
        if (glow_source_moby != 0 && ((glow_source_moby->flags ^ 1) & 1)) {
            for (i = 0; i < 2; i++) {
                moby_point_position(glow_source_moby, i, &glow_sprite_position);
                queue_glow_sprite_2080b0(&glow_sprite_position, 0x301EAA1E, 0.057f, 0.15f);
            }
        }
    }

    hero.glow_phase = fast_add_rotations(hero.glow_phase, D_0015ED6C_gp * 2.26892805f);
    hero.moby->glow_color =
        fast_tween_color(0x806E8C6E, 0x805A3232, fast_sin(hero.glow_phase) * 0.5f + 0.5f);

    item_moby = hero.items[0].moby;
    if (item_moby == 0)
        return;
    if (!((item_moby->flags ^ 1) & 1))
        return;

    switch (hero.items[0].item_id) {
    case ITEM_GRAY_PULSE_GLOW: {
        int   c5;
        int   x1, x2, col;
        float fr;
        float f = fast_add_rotations(hero.items[0].glow_phase, D_0015ED6C_gp * 4.36332321f);
        c5 = 0x28C8C8C8;
        hero.items[0].glow_phase = f;
        if (0.0f < f && f < 2.26892805f)
            c5 = 0x280A0A0A;
        item_moby->glow_color = fast_tween_color(item_moby->glow_color, c5, 0.07f);
        glow_sprite_position.q = 0;
        glow_sprite_position.f[0] = 0.0173452497f;
        glow_sprite_position.f[1] = -0.0391772911f;
        glow_sprite_position.f[2] = 0.0962345004f;
        transform_vector_by_basis(
            &glow_sprite_position, &glow_sprite_position, &item_moby->rot_row0);
        fast_vec_add(&glow_sprite_position, &glow_sprite_position, &item_moby->pos);
        glow_channel_sample = (*(unsigned char *)&item_moby->glow_color);
        g = ((unsigned char *)&item_moby->glow_color)[1];
        fr = (float)glow_channel_sample;
        glow_channel_sample = ((unsigned char *)&item_moby->glow_color)[2];
        x1 = truncate_float_to_s32(fr * 0.2f);
        x2 = truncate_float_to_s32((float)glow_channel_sample * 0.2f);
        col = ((g << 5) / 200) << 24;
        col |= x2 << 16;
        g <<= 8;
        col |= g;
        col |= x1;
        queue_glow_sprite_2080b0(&glow_sprite_position, col, 0.15f, 0.1f);
        break;
    }
    case ITEM_SINE_RED_GREEN_GLOW:
        if (scale_game_frames(50) < hero.items[0].ready_frames) {
            float k, f;
            int   r, g, b, sprite_alpha, col, col1, bs, gs;
            k = D_0015ED6C_gp * 3.14159274f;
            if (hero.item_anim_active != 0)
                k = D_0015ED6C_gp * 4.71238899f;
            f = fast_add_rotations(hero.items[0].glow_phase, k);
            hero.items[0].glow_phase = f;
            r = truncate_float_to_s32(fast_sin(f) * 20.0f) + 30;
            g = truncate_float_to_s32(fast_sin(f) * 90.0f) + 150;
            b = truncate_float_to_s32(fast_sin(f) * 20.0f) + 30;
            if (hero.item_anim_active != 0) {
                int saved_red = r;
                r = g;
                g = saved_red;
            }
            bs = b << 16;
            col1 = bs | 0x20000000;
            gs = g << 8;
            col1 |= gs;
            col1 |= r;
            item_moby->glow_color = fast_tween_color(item_moby->glow_color, col1, 0.13f);
            sprite_alpha = hero.items[0].ready_frames - scale_game_frames(50);
            if (sprite_alpha > 32)
                sprite_alpha = 32;
            col = (sprite_alpha << 24) | bs | gs | r;
            moby_point_position(item_moby, 1, &glow_sprite_position);
            queue_glow_sprite_2080b0(&glow_sprite_position, col, 0.15f, 0.04f);
        }
        break;
    case ITEM_RED_BLUE_GLOW: {
        float k, f;
        int   r, g, b, col;
        k = D_0015ED6C_gp * 2.26892805f;
        if (hero.item_anim_active != 0)
            k = D_0015ED6C_gp * 7.33038282f;
        f = fast_add_rotations(hero.items[0].glow_phase, k);
        hero.items[0].glow_phase = f;
        if (hero.item_anim_active != 0) {
            r = truncate_float_to_s32(fast_sin(f) * 30.0f) + 50;
            g = truncate_float_to_s32(fast_sin(f) * 30.0f) + 50;
            b = truncate_float_to_s32(fast_sin(f) * 90.0f) + 170;
        } else {
            r = truncate_float_to_s32(fast_sin(f) * 70.0f) + 100;
            g = truncate_float_to_s32(fast_sin(f) * 40.0f) + 70;
            b = truncate_float_to_s32(fast_sin(f) * 40.0f) + 70;
        }
        col = b << 16;
        col |= 0x80000000;
        col |= g << 8;
        col |= r;
        item_moby->glow_color = fast_tween_color(item_moby->glow_color, col, 0.15f);
        break;
    }
    case ITEM_BRIGHT_GRAY_PULSE_GLOW: {
        int   c5;
        int   x1, x2, col;
        float fr;
        float f = fast_add_rotations(hero.items[0].glow_phase, D_0015ED6C_gp * 4.36332321f);
        c5 = 0xDCDCDC;
        hero.items[0].glow_phase = f;
        if (0.0f < f && f < 2.44346094f)
            c5 = 0xA0A0A;
        item_moby->glow_color = fast_tween_color(item_moby->glow_color, c5, 0.07f);
        glow_sprite_position.q = 0;
        glow_sprite_position.f[0] = 0.0171042103f;
        glow_sprite_position.f[1] = -0.0939194188f;
        glow_sprite_position.f[2] = 0.140954122f;
        transform_vector_by_basis(
            &glow_sprite_position, &glow_sprite_position, &item_moby->rot_row0);
        fast_vec_add(&glow_sprite_position, &glow_sprite_position, &item_moby->pos);
        glow_channel_sample = (*(unsigned char *)&item_moby->glow_color);
        g = ((unsigned char *)&item_moby->glow_color)[1];
        fr = (float)glow_channel_sample;
        glow_channel_sample = ((unsigned char *)&item_moby->glow_color)[2];
        x1 = truncate_float_to_s32(fr * 0.2f);
        x2 = truncate_float_to_s32((float)glow_channel_sample * 0.2f);
        col = ((g << 5) / 200) << 24;
        col |= x2 << 16;
        g <<= 8;
        col |= g;
        col |= x1;
        queue_glow_sprite_2080b0(&glow_sprite_position, col, 0.17f, 0.1f);
        break;
    }
    case ITEM_RED_GREEN_GLOW: {
        int   c5;
        float f = fast_add_rotations(hero.items[0].glow_phase, D_0015ED6C_gp * 4.71238899f);
        c5 = 0x2814D214;
        hero.items[0].glow_phase = f;
        if (0.0f < f && f < 2.09439516f)
            c5 = 0x281414D2;
        item_moby->glow_color = fast_tween_color(item_moby->glow_color, c5, frame_scale * 0.05f);
        moby_point_position(item_moby, 0, &glow_sprite_position);
        queue_glow_sprite_2080b0(&glow_sprite_position, item_moby->glow_color, 0.1f, 0.03f);
        break;
    }
    }
}

extern void FUN_L00_00232fe8(void);
extern void FUN_L00_002330d0(void);
extern void apply_hero_vertical_oscillation(void) __asm__("FUN_L00_002078a8");
extern void apply_hero_rotation_oscillation(void) __asm__("FUN_L00_002079e8");
extern void offset_hero_position_against_gravity(void *, void *, float) __asm__("FUN_L00_002336e8");
extern void
set_moby_local_table_entry(void *, int, void *, float, float) __asm__("FUN_L00_0024eed8");
extern int hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");

void update_hero_moby(void) {
    OvlQuad      position_scratch[1];
    float        minimum_top_gap, second_position_offset, top_gap;
    struct Moby *hero_moby;

    ((unsigned char *)hero.moby)[0xA4] = 0xFF;
    if (hero.unk20A5) {
        FUN_L00_00232fe8();
    } else {
        FUN_L00_002330d0();
    }
    qcopy(((unsigned char *)hero.moby) + 0x10, &hero.motion.pos);
    apply_hero_vertical_oscillation();
    qcopy(((unsigned char *)hero.moby) + 0x40, &hero.motion.rot);
    apply_hero_rotation_oscillation();
    copy_memory_aligned(((unsigned char *)hero.moby) + 0xC0, &hero, 0x30);
    update_entity(((unsigned char *)hero.moby));
    if (hero.collision_off_frames == 0 && *(int *)(((unsigned char *)hero.moby) + 0x94) != 0) {
        qcopy(position_scratch, &hero.motion.pos);
        minimum_top_gap = 0.05f;
        offset_hero_position_against_gravity(
            position_scratch, position_scratch, hero.capsule_bottom);
        top_gap = hero.capsule_top - hero.capsule_bottom;
        if (top_gap < minimum_top_gap)
            top_gap = minimum_top_gap;
        set_moby_local_table_entry(
            ((unsigned char *)hero.moby), 0, position_scratch, hero.capsule_radius, top_gap);
        second_position_offset = hero.capsule_radius + minimum_top_gap;
        if (hero.state.control_mode == 4)
            second_position_offset += 0.15f;
        if (hero_is_in_special_state())
            second_position_offset = hero.capsule_bottom;
        qcopy(position_scratch, &hero.motion.pos);
        offset_hero_position_against_gravity(
            position_scratch, position_scratch, second_position_offset);
        top_gap = hero.capsule_top - second_position_offset;
        if (top_gap < minimum_top_gap)
            top_gap = minimum_top_gap;
        set_moby_local_table_entry(
            ((unsigned char *)hero.moby), 1, position_scratch, hero.capsule_radius, top_gap);
    }
    hero_moby = ((unsigned char *)hero.moby);
    if (hero_moby->oclass == 0) {
        if (opt_mirror_anim) {
            hero_moby->flags |= 0x8000;
        } else {
            hero_moby->flags &= 0x7FFF;
        }
    }
}

extern void set_hero_moby_flag_bit0(void) __asm__("FUN_L00_00233210");
extern void clear_hero_moby_flag_bit0(void) __asm__("FUN_L00_00233248");
extern void FUN_L00_00232fe8_00208a10(void) __asm__("FUN_L00_00232fe8");
extern void
set_moby_local_table_entry(void *, int, void *, float, float) __asm__("FUN_L00_0024eed8");

void update_hero_vehicle_moby(void) {
    u128           position_scratch;
    float          second_position_offset;
    unsigned char *hero_moby;
    if (hero.unk20A5) {
        set_hero_moby_flag_bit0();
    } else {
        clear_hero_moby_flag_bit0();
    }
    FUN_L00_00232fe8_00208a10();
    ((unsigned char *)hero.moby)[0xA4] = 0xFF;
    hero_moby = ((unsigned char *)hero.moby);
    qcopy(hero_moby + 0x10, &hero.motion.pos);
    qcopy(hero_moby + 0x40, &hero.motion.rot);
    copy_memory_aligned(hero_moby + 0xC0, &hero, 0x30);
    update_entity(((unsigned char *)hero.moby));
    if (hero.collision_off_frames == 0 && *(int *)(((unsigned char *)hero.moby) + 0x94) != 0) {
        qcopy(&position_scratch, &hero.motion.pos);
        offset_hero_position_against_gravity(
            &position_scratch, &position_scratch, hero.capsule_bottom);
        set_moby_local_table_entry(((unsigned char *)hero.moby),
                                   0,
                                   &position_scratch,
                                   hero.capsule_radius,
                                   hero.capsule_top - hero.capsule_bottom);
        second_position_offset = hero.capsule_radius + 0.05f;
        qcopy(&position_scratch, &hero.motion.pos);
        offset_hero_position_against_gravity(
            &position_scratch, &position_scratch, second_position_offset);
        set_moby_local_table_entry(((unsigned char *)hero.moby),
                                   1,
                                   &position_scratch,
                                   hero.capsule_radius,
                                   hero.capsule_top - second_position_offset);
    }
}

typedef union {
    OvlQuad q;
    f32     f[4];
} V_208b60;

typedef struct {
    u8  p0[0xC];
    f32 fC;
    u8  p10[0x34 - 0x10];
    u16 h34;
    u8  p36[0x84 - 0x36];
    f32 f84;
    f32 f88;
    u8  p8C[0xBD - 0x8C];
    u8  bBD;
} M_208b60;

typedef struct {
    u8        p0[0x2D8];
    f32       f2D8;
    u8        p2DC[0x2F0 - 0x2DC];
    f32       f2F0;
    f32       f2F4;
    u8        p2F8[0x30E - 0x2F8];
    s16       h30E;
    u8        p310[0xD20 - 0x310];
    f32       fD20;
    u8        pD24[0x2080 - 0xD24];
    M_208b60 *m2080;
    s32       i2084;
    u8        p2088[4];
    s32       i208C;
} G_208b60;

typedef struct {
    f32 x, y, z, w;
} T_208b60;

typedef struct {
    u8  p0[0x28];
    f32 hit_z; /* CollisionHit.point.f[2] */
} U_208b60;

extern G_208b60 D_0013F350_208b60 __asm__("D_0013F350");
extern int      D_L00_00173E40[];
s32             collision_line(V_208b60 *, V_208b60 *, s32, s32, s32) __asm__("FUN_001efa68");
#define G D_0013F350_208b60
extern V_208b60 l00_hero_height_probe_direction __asm__("D_L00_001AEB90");
extern T_208b60 l00_hero_height_probe_offsets[] __asm__("D_L00_0017C300");

void update_hero_moby_probed_height_bounds(void) {
    V_208b60 v, collision_probe_start, x;
    f32 k, rotation_step, planar_angle, planar_cosine, planar_sine, probe_reference_z, hi, lo, sc;
    T_208b60 *t;
    V_208b60 *pw, *px;
    s32       i;
    if (G.i208C == 0x12 || G.i208C == 0x15 || G.i2084 == 0x7D || G.i208C == 0x16) {
        G.m2080->h34 &= 0xFBFF;
        return;
    }
    G.m2080->h34 |= 0x400;
    G.m2080->bBD = 1;
    k = 0.97f;
    if (G.h30E != 0 || G.i208C == 4)
        k = 1.5f;
    rotation_step = fast_subtract_rotations(k, G.fD20);
    if (0.052f < rotation_step)
        rotation_step = 0.052f;
    else if (rotation_step < -0.052f)
        rotation_step = -0.052f;
    G.fD20 = fast_add_rotations(G.fD20, rotation_step);
    get_moby_light_direction(G.m2080, &v);
    planar_angle = fast_arc_tan(v.f[0], v.f[1]);
    planar_cosine = fast_cos(planar_angle);
    planar_sine = fast_sin(planar_angle);
    {
        f32 direction_xy_scale = fast_cos(G.fD20);
        v.f[0] = planar_cosine * direction_xy_scale;
        v.f[1] = planar_sine * direction_xy_scale;
    }
    v.f[2] = -fast_sin(G.fD20);
    qcopy(&l00_hero_height_probe_direction, &v);
    probe_reference_z = G.f2D8;
    if (probe_reference_z < 1.0f) {
        f32 t = G.f2F0;
        if (1.0f <= t)
            probe_reference_z = t;
        else {
            t = G.f2F4;
            if (1.0f < t)
                probe_reference_z = t;
            else {
                G.m2080->f84 = 0.0f;
                G.m2080->f88 = 0.0f;
                return;
            }
        }
    }
    hi = probe_reference_z;
    lo = hi;
    pw = &collision_probe_start;
    px = &x;
    t = l00_hero_height_probe_offsets;
    for (i = 3; i >= 0; i--, t++) {
        M_208b60 *m;
        U_208b60 *u = &(*(U_208b60 *)D_L00_00173E40);
        m = G.m2080;
        sc = m->fC * 0.0009765625f;
        fast_vec_scale(pw, m, 0.0009765625f);
        collision_probe_start.f[0] += planar_cosine * t->x * sc;
        collision_probe_start.f[1] += planar_sine * t->y * sc;
        collision_probe_start.f[2] += t->z * sc;
        fast_vec_normalize(
            px, &v, ((collision_probe_start.f[2] - probe_reference_z) + 0.5f) / -v.f[2]);
        fast_vec_add(px, pw, px);
        if (collision_line(pw, px, 0x22, 0, 0)) {
            lo = min_float(lo, u->hit_z);
            hi = max_float(hi, u->hit_z);
        }
    }
    G.m2080->f84 = lo - 0.12f;
    G.m2080->f88 = hi + 0.24f;
    {
        M_208b60 *m = G.m2080;
        f32       z = m->f84 + 3.0f;
        if (z < m->f88)
            m->f88 = z;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00209598), where it is exact; names translated to the US level program. */

f32 adjust_hero_animation_speed_for_target_frame(f32 target_anim_frame,
                                                 f32 anim_speed_divisor,
                                                 f32 frame_delta_scale,
                                                 f32 anim_speed_step) __asm__("FUN_L00_00208f20");

f32 adjust_hero_animation_speed_for_target_frame(f32 target_anim_frame,
                                                 f32 anim_speed_divisor,
                                                 f32 frame_delta_scale,

                                                 f32 anim_speed_step) {
    struct Hero *hero_animation_state = &hero;
    f32          thresh = hero_animation_state->anim_frame;
    f32          step;
    f32          negone = -1.0f;

    if (thresh < target_anim_frame) {
        step = target_anim_frame - thresh;
    } else {
        s32 flag = hero_animation_state->anim_changing;
        f32 rate = 2.0f;

        if (flag == 0) {
            rate = 0.5f / hero_animation_state->anim_frame_rate;
        }

        {
            void *hero_moby = ((void *)hero_animation_state->moby);
            s32   animation_sequence_index = *(u8 *)((u8 *)hero_moby + 0x53);
            u8   *arr = *(u8 **)((u8 *)hero_moby + 0x24);
            void *entry = *(void **)(arr + (animation_sequence_index << 2) + 0x48);
            s32   sequence_wrap_count = *(u8 *)((u8 *)entry + 0x10);
            step = target_anim_frame + (rate * (f32)sequence_wrap_count - thresh);
        }
    }

    if (anim_speed_divisor > 0.0f) {
        struct Hero *anim_speed_owner = &hero;
        f32          rate2 = anim_speed_owner->anim_frame_rate;

        if (anim_speed_owner->anim_changing != 0) {
            void *animation_rate_moby = ((void *)anim_speed_owner->moby);
            rate2 = *(f32 *)(*(void **)((u8 *)animation_rate_moby + 0x6C));
        }

        {
            f32 target = frame_delta_scale * step;
            f32 speed = anim_speed_divisor * rate2;
            f32 result = target / speed;

            if (anim_speed_step == negone) {
                anim_speed_owner->anim_speed = result;
                return negone;
            }
            return approach_value((f32 *)(&anim_speed_owner->anim_speed), result, anim_speed_step);
        }
    }

    return 0.0f;
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00209690), where it is exact; names translated to the US level program. */

#include "rnc/data_labels.h"

float FUN_L00_00209018(float *a, int n);

float FUN_L00_00209018(float *sample_values, int sample_count) {
    char  sample_integral_part_scratch[16] __attribute__((aligned(16)));
    char *hero_base = (char *)((unsigned char *)&D_0013E533) + 0xE1D;
    int   i = truncate_float_to_s32(*(float *)(hero_base + 0xAA8));
    int   last_sample_index = sample_count - 1;
    int   previous_sample_index = (i + last_sample_index) % sample_count;
    float current_sample_weight;
    if (*(int *)(hero_base + 0xA98) & 2) {
        previous_sample_index = last_sample_index;
    }
    current_sample_weight =
        split_float_fraction(sample_integral_part_scratch, *(float *)(hero_base + 0xAA8));
    return current_sample_weight * sample_values[i] +
           (*(float *)(hero_base + 0xAAC) - current_sample_weight) *
               sample_values[previous_sample_index];
}

typedef unsigned int u128_002090d0 __attribute__((mode(TI), aligned(16)));

typedef struct {
    float x, y, z, w_002090d0;
} __attribute__((aligned(16))) Vec4_002090d0;

typedef struct {
    Vec4_002090d0 r[4];
} Mtx;

void FUN_001f9d20_002090d0(Vec4_002090d0 *, Vec4_002090d0 *, Mtx *) __asm__("FUN_001f9d20");
void FUN_001f9a28_002090d0(Vec4_002090d0 *,
                           Vec4_002090d0 *,
                           Vec4_002090d0 *) __asm__("FUN_001f9a28");
void FUN_001f9a10_002090d0(Vec4_002090d0 *,
                           Vec4_002090d0 *,
                           Vec4_002090d0 *) __asm__("FUN_001f9a10");
extern Vec4_002090d0 hero_motion_rotation_for_local_pivot __asm__("D_0013F3E0");
void                 rotate_hero_about_local_pivot(float *inf,
                                                   float  x,
                                                   float  y,
                                                   float  z) __asm__("FUN_L00_002090d0");

void rotate_hero_about_local_pivot(float *inf, float x, float y, float z) {
    u128_002090d0 *in = (u128_002090d0 *)inf;
    struct {
        Vec4_002090d0 p;
        Vec4_002090d0 a;
        Vec4_002090d0 b;
        Vec4_002090d0 d;
        Mtx           m;
        Mtx           r;
        Vec4_002090d0 e;
    } L_002090d0;
    Vec4_002090d0 *hero_rotation = &hero_motion_rotation_for_local_pivot;
    *(u128_002090d0 *)&L_002090d0.p = *in;
    euler_rotation_matrix(&L_002090d0.m, hero_rotation);
    FUN_001f9d20_002090d0(&L_002090d0.a, &L_002090d0.p, &L_002090d0.m);
    L_002090d0.e.x = x;
    L_002090d0.e.y = y;
    L_002090d0.e.z = z;
    euler_rotation_matrix(&L_002090d0.r, &L_002090d0.e);
    multiply_matrix(&L_002090d0.m, &L_002090d0.m, &L_002090d0.r);
    FUN_001f9d20_002090d0(&L_002090d0.b, &L_002090d0.p, &L_002090d0.m);
    FUN_001f9a28_002090d0(&L_002090d0.d, &L_002090d0.a, &L_002090d0.b);
    FUN_001f9a10_002090d0(hero_rotation - 1, hero_rotation - 1, &L_002090d0.d);
    matrix_to_euler(&L_002090d0.m, hero_rotation);
}

/* Builds a {0,0,0.6,0} param vector and forwards it (plus the caller's
   position) to func_L00_00209748. */
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00209850), where it is exact; names translated to the US level program. */

typedef float V[4] __attribute__((aligned(16)));

extern void rotate_hero_about_local_pivot(float *, float, float, float) __asm__("FUN_L00_002090d0");

/* Calls FUN_L00_002090d0 with a vector (0, 0, 0.6) and x, y, z. */
void rotate_hero_about_fixed_local_pivot(float x, float y, float z) __asm__("FUN_L00_002091d8");

void rotate_hero_about_fixed_local_pivot(float x, float y, float z) {
    V rotation_reference_vector;

    clear_u64_value(rotation_reference_vector);
    rotation_reference_vector[2] = 0.6f;
    rotate_hero_about_local_pivot(rotation_reference_vector, x, y, z);
}

#include "sda.h"

typedef struct {
    s32 v[9];
} T00209240;

extern T00209240 l00_hero_world_matrix_selection_indices __asm__("D_L00_001E7C10");
extern int       D_001413D0[] NOT_SDA;
void compute_selected_world_matrices(s32, s32, T00209240 *, void *) __asm__("FUN_L00_0024f8f0");

/* Calls 0024f8f0 with mode 9 and a copy of D_L00_001E7C10 on a, or on D_001413D0 when a is 0. */
void update_hero_world_matrices(s32 moby_override) {
    T00209240 matrix_input_copy;
    if (moby_override == 0)
        moby_override = (*(s32 *)D_001413D0);
    matrix_input_copy = l00_hero_world_matrix_selection_indices;
    compute_selected_world_matrices(
        moby_override, 9, &matrix_input_copy, (((char *)&hero) + 0xac0));
}

/* A packed pose: after this header come three lists of 64-bit entries, four
   saturated s16 each. Rotations (quaternions * 32768) start with the root at
   0x10; scales (4096 = 1.0) and translations follow, each entry tagged with
   its joint index in the top s16 (scales also with 0x8000). */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 size;               /* 0x6: quadwords from 0x10 to the end */
    u16 scale_offset;       /* 0x8: bytes from 0x10 to the scale list */
    u16 scale_count;        /* 0xA */
    u16 translation_offset; /* 0xC: bytes from 0x10 to the translation list */
    u16 translation_count;  /* 0xE */
    u64 root_rotation;      /* 0x10 */
} PackedPose;

/* The identity quaternion, packed: w = 0x7FFF. */
#define PACKED_IDENTITY 0x7fff000000000000ULL
/* A packed scale of 1.0 (x = y = z = 0x1000), which the scale list leaves out. */
#define PACKED_UNIT_SCALE 0x100010001000ULL
#define PACKED_XYZ_MASK   0xffffffffffffULL

/* Scratchpad joint records, 0x40 bytes each: rotation, scale, translation. */
#define SPR_JOINT_ROTATION    0x70000000
#define SPR_JOINT_SCALE       0x70000010
#define SPR_JOINT_TRANSLATION 0x70000020

extern u64  pack_vector_s16(s32 vector, f32 scale) __asm__("FUN_L00_001ff768");
extern void FUN_00254b58_002092c8(s32) __asm__("FUN_L00_00254b58");

/* Packs the scratchpad joints listed in joints[1..] (ended by a negative
   entry) into pose: every rotation, padding identity rotations, the scales
   that are not 1.0 and the translations that differ from the bind pose. */
void pack_pose(s32 *joints, PackedPose *pose, u8 *model, s32 padding, s32 context) __asm__("FUN_L00_002092c8");

void pack_pose(s32 *joints, PackedPose *pose, u8 *model, s32 padding, s32 context) {
    u64 *out;
    s32  rotation_count;
    s32  scale_count;
    s32  translation_count;
    s32  i;
    s32  joint;
    s32  rotation_size;
    u64  value;

    if (context == 0)
        context = (*(s32 *)D_001413D0);
    out = (u64 *)(pose + 1);
    FUN_00254b58_002092c8(context);
    pose->root_rotation = PACKED_IDENTITY;
    rotation_count = 1;
    for (i = 1; joints[i] >= 0; i++) {
        *out++ = pack_vector_s16((joints[i] << 6) + SPR_JOINT_ROTATION, 32768.0f);
        rotation_count++;
    }
    if (padding > 0) {
        s32 remaining = padding;
        do {
            *out = PACKED_IDENTITY;
            remaining--;
            out++;
            rotation_count++;
        } while (remaining != 0);
    }
    scale_count = 0;
    i = 1;
    rotation_size = rotation_count << 3;
    for (; joints[i] >= 0; i++) {
        value = pack_vector_s16(SPR_JOINT_SCALE + (joints[i] << 6), 4096.0f);
        if ((value & PACKED_XYZ_MASK) != PACKED_UNIT_SCALE) {
            *out = value & PACKED_XYZ_MASK;
            scale_count++;
            *(u16 *)((u8 *)out + 6) = i | 0x8000;
            out++;
        }
    }
    translation_count = 0;
    for (joint = 1; joints[joint] >= 0; joint++) {
        value = pack_vector_s16(SPR_JOINT_TRANSLATION + (joints[joint] << 6), 1.0f);
        if ((value & PACKED_XYZ_MASK) !=
            (pack_vector_s16(*(s32 *)(model + 0x18) + (joint << 4), 1.0f) & PACKED_XYZ_MASK)) {
            *out = value & PACKED_XYZ_MASK;
            translation_count++;
            *(u16 *)((u8 *)out + 6) = joint;
            out++;
        }
    }
    if ((u32)out & 0xf)
        *out++ = 0;
    pose->translation_count = translation_count;
    pose->scale_offset = rotation_size;
    pose->size = ((u8 *)out - (u8 *)&pose->root_rotation) >> 4;
    pose->scale_count = scale_count;
    pose->translation_offset = (rotation_count + scale_count) << 3;
    pose->unk2 = 0;
    pose->unk4 = 0;
    pose->unk0 = 0;
}

typedef struct {
    float x, y, z, w_00209540;
} V4;

typedef struct {
    char  pad[0x80];
    float x, y;
    char  pad2[0x2f0 - 0x88];
    V4    pos;
    char  pad3[0x904 - 0x300];
    short cnt;
} G_00209540;

extern G_00209540 D_0013F350_00209540 __asm__("D_0013F350");
void FUN_L00_002715e8_00209540(float *, V4 *, int, float, float) __asm__("FUN_L00_002715e8");
void emit_hero_periodic_surface_effect(
    int counter_reset_range, int effect_spawn_counter_threshold) __asm__("FUN_L00_00209540");

void emit_hero_periodic_surface_effect(int counter_reset_range,

                                       int effect_spawn_counter_threshold) {
    G_00209540 *hero_state = &D_0013F350_00209540;
    if (effect_spawn_counter_threshold < ++hero_state->cnt) {
        float spawn_position[4];
        float lo = -0.05f;
        float hi = 0.05f;
        spawn_position[0] = hero_state->x + random_float_between(lo, hi);
        spawn_position[1] = hero_state->y + random_float_between(lo, hi);
        spawn_position[2] = hero_state->pos.x;
        {
            float random_effect_scale = random_float_between(0.4f, 0.6f);
            FUN_L00_002715e8_00209540(
                spawn_position, &hero_state->pos, -1, random_effect_scale, 5250.0f);
        }
        hero_state->cnt = random_integer_below(counter_reset_range);
    }
}

extern void spawn_splash(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");

void fast_sin_cf(int seed, int limit) __asm__("FUN_L00_00209638");

void fast_sin_cf(int seed, int limit) {
    float        planar_effect_vector[4];
    float        pos[4];
    float        planar_effect_angle_radians;
    int          i;
    struct Hero *hero_state;

    if (limit < ++hero.unk908) {
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                planar_effect_angle_radians = fast_add_rotations(hero.motion.rot.f[2], 0.785398f);
            } else {
                planar_effect_angle_radians =
                    fast_subtract_rotations(hero.motion.rot.f[2], 0.785398f);
            }
            hero_state = &hero;
            planar_effect_vector[0] = fast_cos(planar_effect_angle_radians) * (D_0015ED6C * 0.6f);
            planar_effect_vector[1] = fast_sin(planar_effect_angle_radians) * (D_0015ED6C * 0.6f);
            planar_effect_vector[2] = 0.0f;
            pos[0] = hero_state->motion.pos.f[0] + planar_effect_vector[0] * 18.0f;
            pos[1] = hero_state->motion.pos.f[1] + planar_effect_vector[1] * 18.0f;
            pos[2] = hero_state->water_surface_z;
            planar_effect_vector[0] -= fast_cos(hero_state->motion.rot.f[2]) * (D_0015ED6C * 1.5f);
            planar_effect_vector[1] -= fast_sin(hero_state->motion.rot.f[2]) * (D_0015ED6C * 1.5f);
            {
                float random_effect_scale = random_float_between(0.4f, 0.5f);
                float effect_side_value = -2.0f;
                if (i == 0)
                    effect_side_value = 2.0f;
                spawn_splash(pos,
                             planar_effect_vector,
                             &hero_state->water_surface_z,
                             random_effect_scale,
                             effect_side_value);
            }
            if (seed) {
                hero_state->unk908 = random_integer_below(seed);
            } else {
                hero_state->unk908 = 0;
            }
        }
    }
}

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_209848;

void spawn_debris_chunk(V_209848 *, V_209848 *, int, int) __asm__("FUN_L00_0026f548");
void emit_hero_periodic_surface_particles(
    int counter_reset_range, int effect_spawn_counter_threshold) __asm__("FUN_L00_00209848");

void emit_hero_periodic_surface_particles(int counter_reset_range,

                                          int effect_spawn_counter_threshold) {
    struct Hero *P = &hero;
    if (effect_spawn_counter_threshold < ++P->unk906) {
        V_209848 spawn_position, outgoing_motion;
        float    planar_angle, planar_motion_scale;
        int      k;
        spawn_position.x = P->motion.pos.f[0] + random_float_between(-0.15f, 0.15f) +
                           P->motion.displacement.f[0] * 8.0f;
        {
            float y = P->motion.pos.f[1] + random_float_between(-0.15f, 0.15f) +
                      P->motion.displacement.f[1] * 8.0f;
            spawn_position.z = P->water_surface_z + 0.1f;
            spawn_position.y = y;
        }
        planar_angle = random_angle_radians();
        planar_motion_scale = random_float_between(D_0015ED6C * 0.0f, D_0015ED6C * 3.0f);
        outgoing_motion.x =
            fast_cos(planar_angle) * planar_motion_scale + P->motion.displacement.f[0] * 0.75f;
        outgoing_motion.y =
            fast_sin(planar_angle) * planar_motion_scale + P->motion.displacement.f[1] * 0.75f;
        outgoing_motion.z = random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 6.5f);
        k = random_int_between(0x5A, 0x78);
        if (P->water_depth < 0.25f)
            spawn_debris_chunk(&spawn_position, &outgoing_motion, 3, k);
        else
            spawn_debris_chunk(&spawn_position, &outgoing_motion, 1, k);
        {
            short        h = random_integer_below(counter_reset_range);
            struct Hero *Q = &hero;
            Q->unk906 = h;
        }
    }
}

typedef union {
    OvlQuad q;
    float   f[4];
} V_209a40;

extern int random_integer_below_cf(void *, void *, float, float) __asm__("FUN_L00_0026f080");

void emit_hero_randomized_motion_effects(int iteration_count, int mode) __asm__("FUN_L00_00209a40");

void emit_hero_randomized_motion_effects(int iteration_count, int mode) {
    float        pos[4];
    V_209a40     randomized_displacement;
    int          i;
    struct Hero *hero_state;
    V_209a40    *hero_displacement_source;

    for (i = 0; i < iteration_count; i++) {
        hero_state = &hero;
        hero_displacement_source = (V_209a40 *)&hero_state->motion.displacement;
        if (mode == 0) {
            pos[0] = hero_state->motion.pos.f[0] + random_float_between(-0.1f, 0.1f);
            pos[1] = hero_state->motion.pos.f[1] + random_float_between(-0.1f, 0.1f);
            pos[2] = hero_state->motion.pos.f[2] + random_float_between(-0.5f, 0.1f);
        } else if (mode == 1) {
            if (!(i & 1)) {
                moby_point_position(hero_state->moby, 0, pos);
            } else {
                moby_point_position(hero_state->moby, 0xE, pos);
            }
        } else if (mode == 2) {
            if (!(i & 1)) {
                moby_point_position(hero_state->moby, 0x17, pos);
            } else {
                moby_point_position(hero_state->moby, 0x16, pos);
            }
        }
        qcopy(&randomized_displacement, hero_displacement_source);
        if (hero_state->state.current == 0x34) {
            fast_vec_scale(&randomized_displacement, &randomized_displacement, 1.15f);
        } else {
            fast_vec_scale(&randomized_displacement, &randomized_displacement, 0.7f);
        }
        randomized_displacement.f[0] += random_float_between(D_0015ED6C * -0.4f, D_0015ED6C * 0.4f);
        randomized_displacement.f[1] += random_float_between(D_0015ED6C * -0.4f, D_0015ED6C * 0.4f);
        randomized_displacement.f[2] += random_float_between(-D_0015ED6C, D_0015ED6C * 0.0f);
        random_integer_below_cf(
            pos, &randomized_displacement, random_float_between(4200.0f, 7350.0f), -1.0f);
    }
}

#include "rnc/math/circle_offset.h"

extern s32  FUN_L00_00257b90(s32, s32);
extern void spawn_burst_moby(Vec4 *, f32) __asm__("FUN_L00_002d7e90");
extern void spawn_moving_resizing_height_snapped_alpha_fade_particle(
    Vec4 *, f32 *, s32, f32, f32) __asm__("FUN_L00_002715e8");
extern void FUN_L00_0026f548(Vec4 *, Vec4 *, s32, s32);

/* Kicks up dust at the hero's feet (hero.water_surface_z): with flash set,
   first a flash at the hero; then `puffs` puffs within 0.3 of the hero and
   `debris` bits thrown out in random directions. The debris start point adds
   the x velocity to both x and y, as in the original. */
void emit_hero_surface_effect_burst(s32 puffs, s32 debris, s32 flash) __asm__("FUN_L00_00209ca8");

void emit_hero_surface_effect_burst(s32 puffs, s32 debris, s32 flash) {
    Vec4 effect_vector;
    Vec4 start;
    f32  debris_planar_angle_radians;
    f32  speed;
    s32  color;
    s32  life;
    s32  i;
    s32  j;

    if (flash) {
        qcopy(&effect_vector, &hero.motion.pos);
        effect_vector.f[2] = hero.water_surface_z;
        spawn_burst_moby(&effect_vector, 2.25f);
    }
    for (j = 0; j < puffs; j++) {
        effect_vector.f[0] = hero.motion.pos.f[0] + random_float_between(-0.3f, 0.3f);
        effect_vector.f[1] = hero.motion.pos.f[1] + random_float_between(-0.3f, 0.3f);
        effect_vector.f[2] = hero.water_surface_z;
        spawn_moving_resizing_height_snapped_alpha_fade_particle(
            &effect_vector, &hero.water_surface_z, -1, random_float_between(0.3f, 0.6f), 5250.0f);
    }
    for (i = 0; i < debris; i++) {
        debris_planar_angle_radians = random_angle_radians();
        speed = random_float_between(D_0015ED6C * 0.0f, D_0015ED6C * 3.0f);
        effect_vector.f[0] = fast_cos(debris_planar_angle_radians) * speed;
        effect_vector.f[1] = fast_sin(debris_planar_angle_radians) * speed;
        effect_vector.f[2] = random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 8.0f);
        start.f[0] = hero.motion.pos.f[0] + effect_vector.f[0] * 8.0f;
        start.f[1] = hero.motion.pos.f[1] + effect_vector.f[0] * 8.0f;
        start.f[2] = hero.water_surface_z - random_float_between(0.0f, 0.2f);
        color = random_integer_below(2);
        life = FUN_L00_00257b90(0x5A, 0x78);
        FUN_L00_0026f548(&start, &effect_vector, color, life);
    }
}

void        spawn_color_decaying_effect_particle(void *, void *, int) __asm__("FUN_L00_0026d598");
void        FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern char D_0013F7EE[] NOT_SDA;
void        emit_hero_timed_effects(void) __asm__("FUN_L00_00209ec8");

void emit_hero_timed_effects(void) {
    OvlQuad  hero_position_copy[1];
    OvlQuad  effect_vector[1];
    char    *hero_effect_timer_anchor;
    char    *base;
    float    lateral_angle, spherical_offset_scale, planar_scale, alpha_fraction, theta, phi;
    int      frame_count, alpha, col, life, small_effect_sample, large_effect_sample, k;
    OvlQuad *pt;
    hero_effect_timer_anchor = D_0013F7EE;
    if (fast_dec_timer_s16(hero_effect_timer_anchor)) {
        qcopy(hero_position_copy, hero_effect_timer_anchor - 0x41E);
        base = hero_effect_timer_anchor - 0x49E;
        lateral_angle = fast_add_rotations(*(float *)(base + 0x98), 1.5707963705062866f);
        planar_scale = randf_sym(D_0015ED6C * 0.2f, D_0015ED6C * 0.7f);
        ((float *)effect_vector)[0] = fast_cos(lateral_angle) * planar_scale;
        ((float *)effect_vector)[1] = fast_sin(lateral_angle) * planar_scale;
        ((int *)effect_vector)[2] = 0;
        ((float *)effect_vector)[3] = frame_time_sq * -8.0f;
        spawn_color_decaying_effect_particle(hero_position_copy, effect_vector, 0);
        *(short *)(base + 0x49E) = random_int_between(2, 3);
    }
    if (!fast_dec_timer_s16(hero_effect_timer_anchor - 2))
        return;
    frame_count = scale_game_frames(0x28);
    alpha_fraction = (float)(frame_count - *(int *)(hero_effect_timer_anchor - 0x306)) /
                     (float)scale_game_frames(0x28);
    if (1.0f < alpha_fraction)
        alpha_fraction = 1.0f;
    if (alpha_fraction < 0.0f)
        alpha_fraction = 0.0f;
    alpha = truncate_float_to_s32(alpha_fraction * 240.0f);
    alpha = alpha < 0xA1 ? alpha : 0xA0;
    alpha = 0x1F < alpha ? alpha : 0x20;
    col = (alpha << 24) | 0xC0F0;
    life = scale_game_frames(0x28);
    pt = effect_vector;
    for (k = 0x38; k >= 0; k--) {
        small_effect_sample =
            truncate_float_to_s32(random_float_between(4200.0f, 7350.0f) / 1000.0f);
        large_effect_sample =
            truncate_float_to_s32(random_float_between(21000.0f, 31500.001953125f) / 1000.0f);
        spherical_offset_scale = random_float_between(D_0015ED6C * 0.15f, D_0015ED6C * 0.57f);
        qcopy(hero_position_copy, hero_effect_timer_anchor - 0x41E);
        theta = random_angle_radians();
        phi = random_angle_radians();
        build_spherical_offset(pt, spherical_offset_scale, theta, phi);
        FUN_L00_00269958(
            hero_position_copy, pt, col, 0x70F0, life, small_effect_sample, large_effect_sample, 1);
    }
    frame_count = random_int_between(scale_game_frames(3), scale_game_frames(7));
    {
        struct Hero *hero_state = &hero;
        hero_state->unk49C = frame_count;
    }
}

typedef struct {
    char  pad[0xA4];
    float a;
    float b;
} S0020a1e0;

extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
/* Sets the accel and max_speed of the first hero ease. */
void set_first_hero_easing_parameters(float a, float b) __asm__("FUN_L00_0020a1e0");

void set_first_hero_easing_parameters(float a, float b) {
    struct HeroEase *destination_record = hero_eases;
    destination_record->accel = a;
    destination_record->max_speed = b;
}

typedef struct V0020a240 {
    f32 x, y, z, w;
} V0020a240;

typedef struct G0020a240 {
    u8  pad[0x80];
    u8  v80[0x1b4];
    f32 f234;
} G0020a240;

extern u8 D_L00_00173E60[];
extern void
    transform_hero_polar_offset_to_world(V0020a240 *, f32, f32, f32) __asm__("FUN_L00_002335c8");
s32 probe_hero_forward_collision(f32 *out,
                                 f32  probe_z_offset,
                                 f32  probe_end_radius) __asm__("FUN_L00_0020a240");

s32 probe_hero_forward_collision(f32 *out, f32 probe_z_offset, f32 probe_end_radius) {
    G0020a240 *hero_state = (G0020a240 *)&hero;
    V0020a240  probe_start;
    V0020a240  probe_end;

    transform_hero_polar_offset_to_world(
        &probe_start, hero_state->f234 - 0.02f, 0.0f, probe_z_offset);
    transform_hero_polar_offset_to_world(&probe_end, probe_end_radius, 0.0f, probe_z_offset);
    if (collision_line(&probe_start, &probe_end, 2, 0, 0)) {
        if (out != 0)
            *out = vector_distance_xy(hero_state->v80, D_L00_00173E60);
        return 1;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020A990), where it is exact; names translated to the US level program. */

extern V               collision_hit_point __asm__("D_L00_00173E60") NOT_SDA;
extern struct HeroEase hero_eases_lui[] __asm__("D_L00_0017A680") NOT_SDA;

#include "rnc/overlay/collision.h"

void update_hero_collision_ease_target(void) {
    char            *hero_state_base = (char *)((unsigned char *)&D_0013E533) + 0xE1D;
    struct HeroEase *state_ease_table;
    struct HeroEase *hit_ease_table;
    char            *hero_position;
    char            *hit_hero_state_base;
    float probe_z_offset, probe_end_radius, distance_based_ease_value, state_frame_scale_squared,
        frame_scale_squared;
    *(short *)(hero_state_base + 0x22C8) = 0;
    if (*(unsigned char *)(hero_state_base + 0x20AB) == 0)
        return;
    if (hero_slot_item_id(0) == 0x1A)
        return;
    if (*(int *)(hero_state_base + 0x2084) == 0x81) {
        state_ease_table = hero_eases_lui;
        state_frame_scale_squared = frame_scale_sq;
        state_ease_table[9].rot_target.f[1] = -0.349065840f;
        state_ease_table[9].accel = state_frame_scale_squared * 0.05f;
        state_ease_table[9].max_speed = state_frame_scale_squared * 0.3f;
    }
    probe_end_radius = 1.29999995f;
    if (*(short *)(hero_state_base + 0x22C8) != 0)
        probe_end_radius = 1.5999999f;
    probe_z_offset = 0.569999993f;
    if (*(unsigned char *)(hero_state_base + 0x20A8) != 0)
        probe_z_offset = 0.699999988f;
    if (probe_hero_forward_collision(0, probe_z_offset, probe_end_radius) == 0)
        return;
    if (*(unsigned char *)(hero_state_base + 0x20A8) != 0) {
        if (coll_hit_moby != 0) {
            if (is_moby_in_class_range_500_to_540(coll_hit_moby) != 0)
                return;
        }
    }
    frame_scale_squared = frame_scale_sq;
    hit_ease_table = hero_eases_lui;
    hit_ease_table[9].accel = frame_scale_squared * 0.05f;
    hit_ease_table[9].max_speed = frame_scale_squared * 0.3f;
    hero_position = (char *)((unsigned char *)&D_0013E533) + 0xE9D;
    distance_based_ease_value = fast_vec_dist(hero_position, collision_hit_point);
    hit_hero_state_base = (char *)((unsigned char *)&D_0013E533) + 0xE1D;
    distance_based_ease_value = distance_based_ease_value - *(float *)(hit_hero_state_base + 0x234);
    distance_based_ease_value = (1.29999995f - distance_based_ease_value) / 1.29999995f;
    distance_based_ease_value = distance_based_ease_value * 1.25f;
    distance_based_ease_value = distance_based_ease_value * -1.22173047f;
    if (distance_based_ease_value > -0.523598790f)
        distance_based_ease_value = -0.523598790f;
    if (distance_based_ease_value < -1.22173047f)
        distance_based_ease_value = -1.22173047f;
    hit_ease_table[9].rot_target.f[1] = distance_based_ease_value;
    *(short *)(hit_hero_state_base + 0x22C8) = 1;
}

extern char D_0013F350_0020a500[] __asm__("D_0013F350");
/* Clears the words at 0x1004/0x1008 of ((char *)&hero) and four 0x10-byte slots from 0x1024. */
void clear_hero_idle_rotation_targets(void) __asm__("FUN_L00_0020a500");

void clear_hero_idle_rotation_targets(void) {
    char *hero_base = D_0013F350_0020a500;
    char *slot_cursor;
    int   i;
    *(int *)(hero_base + 0x1004) = 0;
    *(int *)(hero_base + 0x1008) = 0;
    slot_cursor = hero_base;
    for (i = 3; i >= 0; i--) {
        *(int *)(slot_cursor + 0x1024) = 0;
        *(int *)(slot_cursor + 0x1028) = 0;
        slot_cursor += 0x10;
    }
}

typedef struct {
    char  p000[0x248];
    f32   f248;
    char  p24c[0x258 - 0x24C];
    f32   f258;
    f32   f25c;
    s32   i260;
    char  p264[0x1004 - 0x264];
    f32   f1004;
    f32   f1008;
    char  p100c[4];
    s32   i1010;
    f32   f1014;
    f32   f1018;
    char  p101c[0x1184 - 0x101C];
    s32   i1184;
    char  p1188[0x2080 - 0x1188];
    void *p2080;
    s32   i2084;
    char  p2088[0x2274 - 0x2088];
    void *p2274;
} G_a540;

extern G_a540          D_0013F350_a540 __asm__("D_0013F350");
extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
extern f32             D_0015EE14 __asm__("D_0015EE14");

extern void set_first_hero_easing_parameters(f32, f32) __asm__("FUN_L00_0020a1e0");
extern f32  interpolate_scalar_linear(f32, f32, f32) __asm__("FUN_L00_0025b6a8");

extern u8 D_0015EDB3 __asm__("D_0015EDB3");
#define G D_0013F350_a540

void update_hero_pitch_and_yaw_easing_targets(void) {
    G_a540 *g;
    G_a540 *h;
    f32     pitch_target_radians, random_blend_factor, k;
    s32     hero_animation_sequence_index;
    s32     min_delay_frames;

    if (D_0015EDB3 != 0 && G.i1184 != 0) {
        hero_eases[18].scale = D_0015EE18;
    }
    hero_eases[17].scale = D_0015EE14;
    if (*(u8 *)0x15EDB1 != 0) {
        approach_value(&D_0015EE14, 1.57f, 0.05f);
    } else {
        approach_value(&D_0015EE14, 0.92f, 0.05f);
    }
    if (G.i260 != 0 && 1.4f < G.f258 && 1.0f < G.f248) {
        set_fourth_hero_easing_parameters(frame_scale_sq * 0.015f, frame_scale_sq * 0.3f);
        pitch_target_radians = G.f258 * 1.5f * DEG_TO_RAD + 0.19198622f;
        if (0.47123888f < pitch_target_radians)
            pitch_target_radians = 0.47123888f;
        if ((1.0f - G.f25c) * 35.0f * DEG_TO_RAD < pitch_target_radians)
            pitch_target_radians = (1.0f - G.f25c) * 35.0f * DEG_TO_RAD;
        hero_eases[3].rot_target.f[1] = pitch_target_radians;
        set_second_hero_easing_parameters(frame_scale_sq * 0.008f, frame_scale_sq * 0.3f);
        hero_eases[1].rot_target.f[1] = hero_eases[3].rot_target.f[1] * 0.7f;
        return;
    }
    if (G.i2084 != 0)
        return;
    hero_animation_sequence_index = *(u8 *)((char *)G.p2080 + 0x53);
    if ((u8)hero_animation_sequence_index >= 3 && (u8)hero_animation_sequence_index != 0x3A)
        return;
    if (G.p2274 != 0)
        return;
    if ((u32)(hero_animation_sequence_index - 1) < 2) {
        min_delay_frames = scale_game_frames(40);
        G.i1010 = random_int_between(min_delay_frames, scale_game_frames(70));
    }
    if (fast_dec_timer(&G.i1010) != 0) {
        G.f1004 = -random_float_between(-0.15707964f, 0.43633232f);
        G.f1008 = randf_sym(0.34906584f, 0.99483764f);
        random_blend_factor = random_float_between(0.0f, 1.0f);
        G.f1014 = interpolate_scalar_linear(0.015f, 0.007f, random_blend_factor);
        G.f1018 = interpolate_scalar_linear(0.29f, 0.3f, random_blend_factor);
        min_delay_frames = scale_game_frames(90);
        G.i1010 = random_int_between(min_delay_frames, scale_game_frames(200));
        G.i1010 += truncate_float_to_s32((f32)scale_game_frames(80) * random_blend_factor);
    }
    if ((u32) * (u8 *)((char *)G.p2080 + 0x53) - 1 < 2) {
        if (0.34906584f < G.f1008)
            G.f1008 = 0.34906584f;
        else if (G.f1008 < -0.34906584f)
            G.f1008 = -0.34906584f;
        if (0.2617994f < G.f1004)
            G.f1004 = 0.2617994f;
        if (G.f1004 < 0.0f)
            G.f1004 = 0.0f;
    }
    set_fourth_hero_easing_parameters(G.f1014, G.f1018);
    hero_eases[3].rot_target.f[2] = G.f1008;
    hero_eases[3].rot_target.f[1] = G.f1004;
    set_second_hero_easing_parameters(frame_scale_sq * 0.008f, frame_scale_sq * 0.3f);
    hero_eases[1].rot_target.f[2] = hero_eases[3].rot_target.f[2] * 0.52f;
    hero_eases[1].rot_target.f[1] = hero_eases[3].rot_target.f[1] * 0.55f;
}
/* Re-rolls four random motion parameters and copies each pair into its 0xB0-byte record. */
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020B060), where it is exact; names translated to the US level program. */

extern float FUN_L00_00257c48(float lo, float hi);
extern char  hero_idle_rotation_timers[] __asm__("D_001403B0");

void update_hero_idle_random_rotation_targets(void) {
    char *base = (char *)((char *)&D_0013F350_002062b0);
    int  *motion_timers;
    char *hero_state_base;
    int   i, j;
    char *motion_ease_table;

    if (*(int *)(base + 0x2084) != 0)
        return;
    if (*(unsigned char *)(*(int *)(base + 0x2080) + 0x53) != 0)
        return;
    motion_timers = (int *)(base + 0x1060);
    hero_state_base = base;
    motion_ease_table = ((char *)hero_eases);
    for (i = 0; i < 4; i++) {
        char *motion_slot_base;
        if (fast_dec_timer(&motion_timers[i])) {
            if (i == 0) {
                int *first_motion_timer;
                *(float *)(hero_state_base + 0x1028) = FUN_L00_00257c48(0.17453292f, 0.5235988f);
                first_motion_timer = (int *)(((char *)&hero_idle_rotation_timers));
                *first_motion_timer =
                    FUN_L00_00257b90(scale_game_frames(0x46), scale_game_frames(0x96));
            } else if (i == 1) {
                *(float *)((char *)motion_timers - 0x28) =
                    FUN_L00_00257c48(0.17453292f, 0.5235988f);
                motion_timers[1] =
                    FUN_L00_00257b90(scale_game_frames(0x28), scale_game_frames(0x5A));
            } else if (i == 2) {
                *(float *)((char *)motion_timers - 0x18) =
                    FUN_L00_00257c48(0.2617994f, 0.87266463f);
                motion_timers[2] =
                    FUN_L00_00257b90(scale_game_frames(0x28), scale_game_frames(0x5A));
            } else {
                float old, nw;
                *(float *)(hero_state_base + i * 0x10 + 0x1024) =
                    random_float_between(-0.08726646f, 0.5235988f);
                old = *(float *)(hero_state_base + i * 0x10 + 0x1028);
                nw = FUN_L00_00257c48(0.34906584f, 0.95993108f);
                *(float *)(hero_state_base + i * 0x10 + 0x1028) = nw;
                if ((old <= 0 && nw <= 0) || (old >= 0 && nw >= 0))
                    *(float *)(hero_state_base + i * 0x10 + 0x1028) =
                        -*(float *)(hero_state_base + i * 0x10 + 0x1028);
                motion_timers[i] =
                    FUN_L00_00257b90(scale_game_frames(0x23), scale_game_frames(0x46));
            }
        }
        motion_slot_base = hero_state_base + i * 0x10;
        j = i + 13;
        base = (char *)(j * 0xB0);
        {
            char *motion_record_destination = motion_ease_table + (int)base;
            *(float *)(motion_record_destination + 0x68) = *(float *)(motion_slot_base + 0x1028);
            *(float *)(motion_record_destination + 0x64) = *(float *)(motion_slot_base + 0x1024);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020B310), where it is exact; names translated to the US level program. */

extern char           *active_mobies[] __asm__("D_L00_001ABA00");
extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
extern int             D_L00_0015F6B0 __asm__("D_L00_0015F5CC");
extern int             FUN_001efa68_ac98(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern unsigned char   hero_pos[] __asm__("D_0013F3D0");
extern void
     transform_hero_local_offset_to_world(float *, float, float, float) __asm__("FUN_L00_00233660");
void set_third_hero_easing_parameters(float a, float b) __asm__("FUN_L00_0020a210");

void update_hero_target_tracking(void) {
    char *hero_control_mode_owner = (char *)hero_pos - 0x80;
    char *hero_animation_owner, *candidate_reference_hero_state, *target_selection_owner,
        *target_aim_owner, *aim_limit_hero_state;
    int              i;
    char            *candidate_moby;
    char            *best;
    float            bestPri, bestDist;
    float            aim_position_scratch[8];
    char            *secondary_position;
    char            *info, *info2, *selected_moby;
    struct HeroEase *manipulator_table;
    float            yaw_delta, pitch, s;
    int              mode = *(int *)(hero_control_mode_owner + 0x208C);
    if (mode != 1 && mode != 4 && mode != 2 && mode != 7 && mode != 0) {
        *(int *)(hero_control_mode_owner + 0x2274) = 0;
        return;
    }
    hero_animation_owner = (char *)hero_pos - 0x80;
    if (*(unsigned char *)(*(int *)(hero_animation_owner + 0x2080) + 0x53) == 0x54)
        return;
    if (D_L00_0015F6B0 % 7 == 0) {
        bestDist = 100000000.0f;
        bestPri = 0.0f;
        best = 0;
        for (i = 0; (candidate_moby = active_mobies[i]) != 0; i++) {
            info = (char *)suck_get_aim_height(candidate_moby);
            if (info != 0) {
                float candidate_distance, candidate_yaw_difference, sc, pri;
                candidate_distance = vector_distance_xy(hero_pos + 0x0, candidate_moby + 0x10);
                if ((float)*(unsigned char *)(info + 0x38) < candidate_distance)
                    continue;
                candidate_reference_hero_state = (char *)hero_pos - 0x80;
                candidate_yaw_difference = fast_difference_between_rotations(
                    fast_arc_tan(MOBY(candidate_moby)->pos.x -
                                     *(float *)(candidate_reference_hero_state + 0x80),
                                 MOBY(candidate_moby)->pos.y -
                                     *(float *)(candidate_reference_hero_state + 0x84)),
                    *(float *)(candidate_reference_hero_state + 0x98));
                if (1.91986215f < candidate_yaw_difference)
                    continue;
                if (1.04719758f < absolute_float(fast_arc_tan(
                                      vector_distance_xy(candidate_moby + 0x10, hero_pos + 0x0),
                                      *(float *)(candidate_reference_hero_state + 0x88) -
                                          MOBY(candidate_moby)->pos.z)))
                    continue;
                sc = candidate_yaw_difference * candidate_distance + candidate_distance;
                if ((int)candidate_moby == *(int *)(candidate_reference_hero_state + 0x2274)) {
                    sc -= 1.5f;
                    if (sc < 0.0f)
                        sc = 0.0f;
                }
                pri = (float)*(unsigned char *)(info + 0x39);
                if (bestPri < pri)
                    goto upd;
                if (sc < bestDist) {
                upd:
                    if (pri > bestPri)
                        bestPri = pri;
                    bestDist = sc;
                    best = candidate_moby;
                }
            }
        }
        target_selection_owner = (char *)hero_pos - 0x80;
        *(char **)(target_selection_owner + 0x2274) = best;
    }
    target_aim_owner = (char *)hero_pos - 0x80;
    if (*(int *)(target_aim_owner + 0x2274) == 0)
        return;
    transform_hero_local_offset_to_world(aim_position_scratch, 0.0f, 0.0f, 1.2f);
    secondary_position = (char *)&aim_position_scratch[4];
    qcopy(secondary_position, *(char **)(target_aim_owner + 0x2274) + 0x10);
    aim_position_scratch[6] += 0.5f;
    if (FUN_001efa68_ac98(aim_position_scratch, secondary_position, 2, 0, 0))
        *(int *)(target_aim_owner + 0x2274) = 0;
    selected_moby = *(char **)(target_aim_owner + 0x2274);
    if (selected_moby == 0)
        return;
    qcopy(aim_position_scratch, selected_moby + 0x10);
    info2 = (char *)suck_get_aim_height(selected_moby);
    if (info2 != 0)
        aim_position_scratch[2] += (float)*(unsigned char *)(info2 + 0x3A) * 0.125f;
    transform_hero_local_offset_to_world(&aim_position_scratch[4], 0.3f, 0.0f, 0.85f);
    yaw_delta =
        fast_subtract_rotations(fast_arc_tan(aim_position_scratch[0] - aim_position_scratch[4],
                                             aim_position_scratch[1] - aim_position_scratch[5]),
                                *(float *)(target_aim_owner + 0x98));
    pitch = fast_arc_tan(vector_distance_xy(aim_position_scratch, secondary_position),
                         aim_position_scratch[6] - aim_position_scratch[2]);
    if (yaw_delta > 1.02974427f)
        yaw_delta = 1.02974427f;
    else if (yaw_delta < -1.02974427f)
        yaw_delta = -1.02974427f;
    if (pitch > 0.610865235f)
        pitch = 0.610865235f;
    if (pitch < -0.436332315f)
        pitch = -0.436332315f;
    aim_limit_hero_state = (char *)hero_pos - 0x80;
    if (scale_game_frames(0x4D) - scale_game_frames(0x32) <
        *(int *)(aim_limit_hero_state + 0x1C0)) {
        if (yaw_delta > 0.296705961f)
            yaw_delta = 0.296705961f;
        else if (yaw_delta < -0.296705961f)
            yaw_delta = -0.296705961f;
        pitch = 0.0f;
    }
    set_fourth_hero_easing_parameters(frame_scale_sq * 0.021f, frame_scale_sq * 0.27f);
    manipulator_table = hero_eases;
    manipulator_table[3].rot_target.f[1] = pitch;
    manipulator_table[3].rot_target.f[2] = yaw_delta * 0.7f;
    set_third_hero_easing_parameters(frame_scale_sq * 0.014f, frame_scale_sq * 0.3f);
    manipulator_table[2].rot_target.f[2] = manipulator_table[3].rot_target.f[2] * 0.55f;
}

typedef struct {
    u8  p0[0x2C0];
    f32 f2C0, f2C4, f2C8, f2CC, f2D0, f2D4;
    u8  p2D8[0x308 - 0x2D8];
    s16 h308;
    u8  p30A[0x418 - 0x30A];
    s32 i418;
    u8  p41C[0x10E0 - 0x41C];
    s32 i10E0;
    u8  p10E4[0x12A0 - 0x10E4];
    f32 f12A0, f12A4;
    s32 i12A8;
    s16 h12AC;
    u16 h12AE;
    u8  p12B0[0x2080 - 0x12B0];
    u8 *p2080;
    s32 i2084;
    u8  p2088[4];
    s32 i208C;
} P_20b1d8;

extern P_20b1d8        D_0013F350_20b1d8 __asm__("D_0013F350");
extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
#define P D_0013F350_20b1d8
extern s32 hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");
extern void
    configure_hero_point_effect_emission(s32, s32, s32, s32, f32, f32) __asm__("FUN_L00_00233288");
extern void
spawn_jittered_effect_particle_with_optional_basis(void *, void *, f32) __asm__("FUN_L00_0026de90");

void update_hero_easing_targets_and_point_effects(void) {
    u8  emission_point_world_position[0x10] __attribute__((aligned(16)));
    f32 clamped_rotation_angle, secondary_clamped_rotation_angle;
    u8 *optional_moby_basis;
    s32 i;
    if (hero_is_in_special_state())
        return;
    if (0.03f < absolute_float(P.f2C0) || 0.03f < absolute_float(P.f2C4) ||
        0.06981317f < absolute_float(P.f2C8) || 0.06981317f < absolute_float(P.f2CC) ||
        0.06981317f < absolute_float(P.f2D0) || 0.06981317f < absolute_float(P.f2D4)) {
        if (P.h308 != 1) {
            hero_eases[4].rot_target.f[1] = -P.f2C8;
            hero_eases[5].rot_target.f[1] = -P.f2CC;
            clamped_rotation_angle = fast_arc_tan(0.2f, P.f2C0) * 1.2f;
            secondary_clamped_rotation_angle = -fast_arc_tan(0.2f, P.f2C4) * 1.2f;
            if (0.7853982f < clamped_rotation_angle)
                clamped_rotation_angle = 0.7853982f;
            if (clamped_rotation_angle < -0.17453292f)
                clamped_rotation_angle = -0.17453292f;
            if (0.17453292f < secondary_clamped_rotation_angle)
                secondary_clamped_rotation_angle = 0.17453292f;
            if (secondary_clamped_rotation_angle < -0.7853982f)
                secondary_clamped_rotation_angle = -0.7853982f;
            hero_eases[6].rot_target.f[0] = clamped_rotation_angle;
            hero_eases[7].rot_target.f[0] = secondary_clamped_rotation_angle;
            hero_eases[4].rot_target.f[0] =
                -fast_subtract_rotations(P.f2D0, -clamped_rotation_angle);
            hero_eases[5].rot_target.f[0] =
                -fast_subtract_rotations(P.f2D4, -secondary_clamped_rotation_angle);
        }
    }
    if (P.i10E0) {
        hero_eases[4].scale = 0.01f;
        hero_eases[5].scale = 0.01f;
    }
    if (!(P.h12AE & 0x80)) {
        if (P.i2084 == 3)
            configure_hero_point_effect_emission(1, 2, 1, 0, 0.021f, 0.002f);
        if (P.i208C == 4 && P.i418 > 0 && P.i418 < scale_game_frames(12))
            configure_hero_point_effect_emission(1, 2, 0, 1, 0.016f, 0.002f);
        if (P.i12A8 <= 0)
            return;
        if (P.h12AC == 0)
            return;
        optional_moby_basis = 0;
        if (P.h12AE & 1)
            optional_moby_basis = P.p2080 + 0xC0;
        P.i12A8--;
        moby_point_position(P.p2080, 0x16, emission_point_world_position);
        for (i = 0; i < P.h12AC; i++)
            spawn_jittered_effect_particle_with_optional_basis(
                emission_point_world_position,
                optional_moby_basis,
                P.f12A0 + random_float_between(-P.f12A4, P.f12A4));
        moby_point_position(P.p2080, 0x17, emission_point_world_position);
        for (i = 0; i < P.h12AC; i++)
            spawn_jittered_effect_particle_with_optional_basis(
                emission_point_world_position,
                optional_moby_basis,
                P.f12A0 + random_float_between(-P.f12A4, P.f12A4));
    } else
        P.i12A8 = 0;
}

typedef unsigned int u128_20b5b8 __attribute__((mode(TI)));

typedef union {
    u128_20b5b8 q;
    f32         f[4];
} V4_20b5b8;

typedef struct {
    u8  pad[0x40];
    f32 x, y;
} S2;

extern s32 probe_hero_ledge_edge(void *, void *) __asm__("FUN_L00_0020c758");
extern s32 FUN_001efa68_20b5b8(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");
extern s32 collision_hit_material(void) __asm__("FUN_001f0b58");
extern f32 compute_gravity_relative_vector_inclination(void *) __asm__("FUN_L00_00233d30");

void probe_hero_airborne_wall_contact(void) __asm__("FUN_L00_0020b5b8");

void probe_hero_airborne_wall_contact(void) {
    V4_20b5b8    position_scratch, collision_probe_end, z;
    struct Hero *g;
    struct Hero *hero_state;
    struct Hero *k2;
    u8          *pz;
    s32          prediction_frames;
    s32          hit_material;
    f32          f;
    f32          r;
    f32          max_hit_distance;
    f32 vertical_acceleration_times_frames, b, projected_x_displacement, projected_y_displacement;

    g = &hero;
    if (g->ground_distance < 0.4f) {
        return;
    }
    if (gravity_z(&g->motion.velocity) < D_0015ED6C * -20.0f) {
        return;
    }
    if (g->ledge_found != 0) {
        g->wall_jump_window = 0;
        return;
    }
    qcopy(&position_scratch, &g->motion.pos);
    prediction_frames = scale_game_frames(0x14);
    {
        f32 prediction_interval_frames = (f32)prediction_frames;
        vertical_acceleration_times_frames = g->jump_gravity * prediction_interval_frames;
        b = g->motion.velocity.f[2] * prediction_interval_frames;
        projected_x_displacement = g->motion.displacement.f[0] * prediction_interval_frames;
        projected_y_displacement = g->motion.displacement.f[1] * prediction_interval_frames;
        position_scratch.f[2] =
            position_scratch.f[2] + b -
            vertical_acceleration_times_frames * prediction_interval_frames * 0.5f;
    }
    position_scratch.f[0] = position_scratch.f[0] + projected_x_displacement;
    position_scratch.f[1] = position_scratch.f[1] + projected_y_displacement;
    if (probe_hero_ledge_edge(&position_scratch, &g->motion.rot.f[2]) != 0) {
        g->wall_jump_window = 0;
        g->unk1F0 = prediction_frames + 2;
        return;
    }
    if (probe_hero_forward_collision(0, 0.3f, 1.0f) == 0) {
        return;
    }
    r = 1.7f;
    if (g->state.current != 0x11) {
        if (g->wall_probe_hit_moby == 0 || g->wall_probe_moby_in_class_range != 0) {
            r = 3.0f;
        }
    }
    k2 = &hero;
    transform_hero_polar_offset_to_world(&position_scratch, 0.0f, 0.0f, r);
    transform_hero_polar_offset_to_world(&collision_probe_end, k2->capsule_radius + 1.4f, 0.0f, r);
    if (FUN_001efa68_20b5b8(&position_scratch, &collision_probe_end, 2, 0, 0) == 0) {
        return;
    }
    hit_material = collision_hit_material();
    if (hit_material == 0xA) {
        return;
    }
    if (hit_material == 0xC) {
        return;
    }
    pz = (u8 *)&coll_hit_normal;
    if (compute_gravity_relative_vector_inclination(pz) < 1.3089969f) {
        return;
    }
    transform_vector(&z, pz, k2->inv_rot_matrix);
    f = fast_arc_tan(z.f[0], z.f[1]);
    f = fast_difference_between_rotations(f, 3.1415927f);
    if (0.87266463f < f) {
        return;
    }
    if (k2->state.current == 0x11) {
        max_hit_distance = 0.95f;
    } else {
        max_hit_distance = 0.72f;
    }
    f = vector_distance_xy(pz - 0x20, &position_scratch);
    if (max_hit_distance < f) {
        return;
    }
    {
        S2 *collision_result;
        k2->unk460.q = *(u128_20b5b8 *)pz;
        collision_result = (S2 *)(pz - 0x40);
        f = fast_arc_tan(collision_result->x, collision_result->y);
    }
    k2->unk460.f[3] = f;
    if (k2->state.control_mode != 4 && k2->state.prev_control_mode != 4 &&
        k2->state.prev2_control_mode != 4) {
        return;
    }
    hero_state = &hero;
    if (hero_state->unk22B4 != 0) {
        f = fast_difference_between_rotations(hero_state->unk470.f[3], hero_state->unk460.f[3]);
        if (f < 2.9670596f) {
            return;
        }
    }
    if (hero_state->state.current == 0x11) {
        hero_state->wall_jump_window = scale_game_frames(7);
    } else {
        hero_state->wall_jump_window = scale_game_frames(6);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020BFA8), where it is exact; names translated to the US level program. */

extern void exit_hero_vehicle_mode(void) __asm__("FUN_L00_00210b30");

/* Calls FUN_L00_00210b30 when the byte at 0x20A4 of ((char *)&hero) is 3. */
void exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");

void exit_hero_vehicle_mode_if_mode_three(void) {
    if (hero.vehicle_mode == 3) {
        exit_hero_vehicle_mode();
    }
}

typedef struct {
    u8  pad0[0x88];
    f32 f88;
    u8  pad8C[0x7C];
    f32 f108;
    u8  pad10C[0x8C];
    s32 i198;
    u8  pad19C[0x98];
    f32 f234;
    u8  pad238[0xA0];
    f32 f2D8;
    f32 f2DC;
    u8  pad2E0[0x10];
    f32 f2F0;
    f32 f2F4;
    u8  pad2F8[0x8];
    s32 i300;
    u8  pad304[0x4];
    s16 h308;
    u8  pad30A[0x2];
    s16 h30C;
    u8  pad30E[0x110];
    s16 h41E;
    u8  pad420[0xCC0];
    u8 *p10E0;
    u8  pad10E4[0x1FC];
    s16 h12E0;
    u8  b12E2;
    u8  b12E3;
    u8  b12E4;
    u8  b12E5;
    u8  b12E6;
    u8  b12E7;
    u8  pad12E8[0x2];
    u8  b12EA;
    u8  b12EB;
    u8  b12EC;
    u8  b12ED;
    u8  b12EE;
    u8  pad12EF[0xD95];
    s32 i2084;
    u8  pad2088[0x4];
    s32 i208C;
    u8  pad2090[0x14];
    u8  b20A4;
    u8  pad20A5[0x4];
    u8  b20A9;
    u8  pad20AA[0x1FA];
    f32 f22A4;
    s32 i22A8;
} P20b960;

extern P20b960 D_0013F350_20b960 __asm__("D_0013F350");
int            coll_sphere(void *, int, int, f32) __asm__("FUN_L00_001f0d60");
int            FUN_001f0b58_20b960(void) __asm__("FUN_001f0b58");
#define P D_0013F350_20b960
void apply_hero_selected_flags_and_state_transitions(void) {
    u8  save = P.b12ED;
    s16 saved_flag_selector = P.h12E0;
    f32 height_difference;
    int target_state;
    fast_mem_zero16(&P.h12E0, 0x10);
    P.b12ED = save;
    P.h12E0 = -1;
    P.b20A9 = 0;
    P.h308 = 0;
    if (saved_flag_selector == -1)
        return;
    if (saved_flag_selector == 2) {
        if (P.h30C == 0 || P.f2DC < 0.3f) {
            P.b12E7 = 1;
            if (P.b20A4 == 0 && P.p10E0 != 0 && *(s16 *)(P.p10E0 + 0xA6) == 0xAD)
                P.h308 = 1;
        }
    }
    if (saved_flag_selector == 14) {
        P.b12E4 = 1;
        P.f22A4 = 0.2f;
        P.f2F0 = P.f2D8 + 0.2f;
    }
    if (saved_flag_selector == 0) {
        height_difference = P.f2F0 - P.f2D8;
        P.f22A4 = height_difference;
        if (height_difference < 0.85f && 0.25f < height_difference)
            P.b20A9 = 1;
        P.b12E4 = 1;
    }
    if (saved_flag_selector == 3)
        P.b12E6 = 1;
    if (current_level_index == 13 && P.i2084 != 0x7B &&
        coll_sphere((u8 *)&P + 0xD0, 2, 0, P.f234 + 0.03f) && FUN_001f0b58_20b960() == 11) {
        goto s7B;
    }
    if (saved_flag_selector == 11)
        P.b12EB = 1;
    if (saved_flag_selector == 2)
        P.b12E7 = 1;
    if (saved_flag_selector == 4)
        P.b12E3 = 1;
    if (saved_flag_selector == 7)
        P.b12E2 = 1;
    if (saved_flag_selector == 13)
        P.b12EC = 1;
    if (saved_flag_selector == 1)
        P.b12E5 = 1;
    if (saved_flag_selector == 8)
        P.b12EA = 1;
    if (saved_flag_selector == 9)
        P.b12EE = 1;
    if (saved_flag_selector == 12)
        P.b12EA = 1;
    if (P.b12E3 && P.i300 && P.i208C != 0x10 && P.i208C != 0x14 && P.i208C != 7) {
        target_state = 0x31;
        goto call;
    }
    if (P.b12E5 && P.i300 && P.b20A4 == 1 && P.i2084 != 0x7D) {
        target_state = 0x7D;
        goto call;
    }
    if (P.b12E5 && P.i300 && (P.i2084 != 0x3C || P.h41E != 0)) {
        exit_hero_vehicle_mode_if_mode_three();
        if (P.i22A8)
            goto call3c;
        target_state = 0x7C;
        goto call;
    }
    if (P.b12EC && P.i2084 != 0x7F && absolute_float(P.f2F0 - (P.f88 + 0.25f)) < 1.0f &&
        0.0f < P.f2F0 - P.f88 && P.f108 < 0.0f) {
        exit_hero_vehicle_mode_if_mode_three();
        target_state = 0x7F;
        goto call;
    }
    if (P.b12E6 && P.i2084 != 0x68 && P.i2084 != 0x7B &&
        absolute_float(P.f2F4 - (P.f88 + 0.25f)) < 1.0f && 0.0f < P.f2F4 - P.f88 &&
        (P.i2084 != 0x69 || P.h41E != 0) && P.f108 < 0.0f) {
        exit_hero_vehicle_mode_if_mode_three();
        if (P.i22A8) {
            target_state = 0x68;
            goto call;
        }
    s7B:
        target_state = 0x7B;
    call:
        hero_set_state(target_state, 1);
        return;
    call3c:
        target_state = 0x3C;
        goto call;
    }
    if (P.b12EB && P.i2084 != 0x7B && absolute_float(P.f2F4 - (P.f88 + 0.25f)) < 1.0f &&
        0.0f < P.f2F4 - P.f88 && P.f108 < 0.0f) {
        exit_hero_vehicle_mode_if_mode_three();
        hero_set_state(0x7B, 1);
    }
}
#undef P
#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"

/* the ledge probe of the hero: looks for an edge ahead to grab, and where it finds one stores the grab point and direction */
/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_0020C568), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    char  p0[0x18];
    char *m18;
    char  p1C[4];
    float v20[4];
    char  p30[0x10];
    float v40[4];
} Hit_20C568;

typedef struct {
    char           p0[0x1E];
    unsigned short h1E;
    char           p20[0x1C];
    int            i3C;
} Obj;

extern int           FUN_001efa68_c(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int           collision_hit_material(void) __asm__("FUN_001f0b58");
extern int           rig_probe() __asm__("FUN_L00_001f19a0");
extern unsigned char D_0013F350[];
void predict_position_with_vertical_drop(void *a, void *b, void *c, int d, float f) __asm__("FUN_L00_00259508");
extern char D_0013F450_20C568[] __asm__("D_0013F350");
extern void predict_position_with_vertical_drop(void *a, void *b, void *c, int d, float f) __asm__("FUN_L00_00259508");
extern int  func_L00_001EFFF0_20C568(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int  func_L00_001F3958_20C568(void) __asm__("FUN_001f0b58");
extern Obj *FUN_L00_002603d0(char *);
extern int  func_L00_001F1D20_20C568(float, float, void *, int, int) __asm__("FUN_L00_001f19a0");

void check_ledge_grab(void) {
    float probe[4], ledge[4], ray_end[4], hero_level[4], clearance[4], edge_start[4], edge_end[4],
        grab_point[4];
    float down, fwd, up, top, drop, rise, back;
    float speed, ledge_yaw, offset;
    int   can_grab, found, edge, i, j, surface, grabbable, frames;
    Obj  *moby_info;
    Obj  *moby_props;

    if (((Hero *)D_0013F450_20C568)->unk308 == 2)
        return;
    ((Hero *)D_0013F450_20C568)->ledge_found = 0;
    if (((Hero *)D_0013F450_20C568)->ledge_probe_cooldown != 0)
        return;
    can_grab = 0;
    if (((Hero *)D_0013F450_20C568)->vehicle_mode == 0) {
        if ((((Hero *)D_0013F450_20C568)->state.control_mode == 4 &&
             (((Hero *)D_0013F450_20C568)->past_jump_apex != 0 ||
              ((Hero *)D_0013F450_20C568)->motion.velocity.f[2] < D_0015ED6C * 3.5f)) ||
            ((Hero *)D_0013F450_20C568)->state.control_mode == 2 ||
            ((Hero *)D_0013F450_20C568)->state.current == 8 ||
            ((Hero *)D_0013F450_20C568)->state.current == 0x1A ||
            ((Hero *)D_0013F450_20C568)->state.current == 0x1B)
            can_grab = 1;
        if (((Hero *)D_0013F450_20C568)->state.current == 0xE &&
            ((Hero *)D_0013F450_20C568)->anim_frame < 37.0f)
            can_grab = 0;
    }
    if (((Hero *)D_0013F450_20C568)->vehicle_mode == 1) {
        if ((((Hero *)D_0013F450_20C568)->state.control_mode == 4 &&
             (((Hero *)D_0013F450_20C568)->past_jump_apex != 0 ||
              ((Hero *)D_0013F450_20C568)->motion.velocity.f[2] < D_0015ED6C * 2.5f)) ||
            ((Hero *)D_0013F450_20C568)->state.control_mode == 2 ||
            ((Hero *)D_0013F450_20C568)->state.current == 0x4F ||
            ((Hero *)D_0013F450_20C568)->state.current == 0x4D ||
            ((Hero *)D_0013F450_20C568)->state.current == 0x4E)
            can_grab = 1;
    }
    if (!can_grab)
        return;

    down = -0.7f;
    fwd = 0.3f;
    up = 1.5f;
    top = 1.15f;
    drop = -1.43f;
    rise = 1.8f;
    back = 0.45f;
    if (((Hero *)D_0013F450_20C568)->vehicle_mode == 1) {
        down = -0.4f;
        fwd = 0.25f;
        up = 0.725f;
        top = 0.7f;
        drop = -0.71f;
        rise = 0.7f;
        back = 0.32f;
    }
    speed = ((Hero *)D_0013F450_20C568)->fall_state_gravity;
    if (((Hero *)D_0013F450_20C568)->state.control_mode == 4)
        speed = ((Hero *)D_0013F450_20C568)->jump_gravity;
    frames = scale_game_frames(2);
    predict_position_with_vertical_drop(probe,
                                        ((Hero *)D_0013F450_20C568)->motion.pos.f,
                                        ((Hero *)D_0013F450_20C568)->motion.resolved_velocity.f,
                                        frames,
                                        speed);
    found = 0;
    probe[0] += fast_cos(((Hero *)D_0013F450_20C568)->motion.rot.f[2]) *
                ((Hero *)D_0013F450_20C568)->capsule_radius;
    probe[1] += fast_sin(((Hero *)D_0013F450_20C568)->motion.rot.f[2]) *
                ((Hero *)D_0013F450_20C568)->capsule_radius;
    for (i = 0; !found && i < 4; i++) {
        probe[0] += fast_cos(((Hero *)D_0013F450_20C568)->motion.rot.f[2]) * fwd * 0.25f;
        probe[1] += fast_sin(((Hero *)D_0013F450_20C568)->motion.rot.f[2]) * fwd * 0.25f;
        qcopy(ledge, probe);
        ledge[2] += up + 0.15f;
        qcopy(ray_end, probe);
        ray_end[2] += top;
        if (func_L00_001EFFF0_20C568(
                ledge, ray_end, 4, (int)((Hero *)D_0013F450_20C568)->moby, 0)) {
            surface = func_L00_001F3958_20C568();
            if (surface != 9 && surface != 12 && surface != 0) {
                if (fast_arc_tan(((Hit_20C568 *)((char *)D_L00_00173E40))->v40[2],
                                 vector_length_xy(((Hit_20C568 *)((char *)D_L00_00173E40))->v40)) <
                    0.3490658402442932f)
                    found = 1;
            }
        }
    }
    if (!found)
        return;

    if (((Hit_20C568 *)((char *)D_L00_00173E40))->m18 != 0) {
        grabbable = 0;
        moby_info = suck_get_aim_height(((Hit_20C568 *)((char *)D_L00_00173E40))->m18);
        moby_props = FUN_L00_002603d0(((Hit_20C568 *)((char *)D_L00_00173E40))->m18);
        if (moby_info != 0 && (moby_info->h1E & 1)) {
            grabbable = 1;
        } else if (moby_props != 0) {
            grabbable = moby_props->i3C & 1;
        }
        if (!grabbable)
            return;
    }
    ((Hero *)D_0013F450_20C568)->ledge_moby = (s32)(((Hit_20C568 *)((char *)D_L00_00173E40))->m18);
    if (((Hero *)D_0013F450_20C568)->motion.pos.f[2] + up <
        ((Hit_20C568 *)((char *)D_L00_00173E40))->v20[2])
        return;
    qcopy(ledge, ((Hit_20C568 *)((char *)D_L00_00173E40))->v20);
    qcopy(ray_end, ((Hit_20C568 *)((char *)D_L00_00173E40))->v20);
    ray_end[2] += 0.05f;
    qcopy_nc(hero_level, ((Hero *)D_0013F450_20C568)->motion.pos.f);
    hero_level[2] = ray_end[2];
    if (func_L00_001EFFF0_20C568(hero_level, ray_end, 4, (int)((Hero *)D_0013F450_20C568)->moby, 0))
        return;
    qcopy(clearance, ((Hit_20C568 *)((char *)D_L00_00173E40))->v20);
    clearance[2] += 0.6f;
    if (func_L00_001F1D20_20C568(
            0.45f, 0.6f, clearance, 4, (int)((Hero *)D_0013F450_20C568)->moby)) {
        ((Hero *)D_0013F450_20C568)->ledge_flags |= 1;
    } else {
        ((Hero *)D_0013F450_20C568)->ledge_flags &= 0xFFFFFFFE;
    }
    ledge_yaw = fast_arc_tan(ledge[0] - ((Hero *)D_0013F450_20C568)->motion.pos.f[0],
                             ledge[1] - ((Hero *)D_0013F450_20C568)->motion.pos.f[1]);
    ((Hero *)D_0013F450_20C568)->ledge_top_z = ledge[2];
    if (ledge[2] - ((Hero *)D_0013F450_20C568)->ground_z.f < rise)
        return;

    edge = 0;
    for (j = 0; j < 5; j++) {
        float edge_probe_extension_distance;

        qcopy(edge_start, ((Hero *)D_0013F450_20C568)->motion.pos.f);
        qcopy(edge_end, ledge);
        edge_probe_extension_distance = ((Hero *)D_0013F450_20C568)->capsule_radius * 2.0f;
        edge_end[0] += fast_cos(ledge_yaw) * edge_probe_extension_distance;
        edge_end[1] += fast_sin(ledge_yaw) * edge_probe_extension_distance;
        edge_end[2] += down * (1.0f - (float)j / 5.0f);
        edge_start[2] = edge_end[2];
        if (func_L00_001EFFF0_20C568(edge_start, edge_end, 2, 0, 0)) {
            edge = 1;
            ((Hero *)D_0013F450_20C568)->ledge_normal_yaw =
                fast_arc_tan(((Hit_20C568 *)((char *)D_L00_00173E40))->v40[0],
                             ((Hit_20C568 *)((char *)D_L00_00173E40))->v40[1]);
            break;
        }
    }
    if (!edge)
        return;
    if (!(fast_difference_between_rotations(
              ((Hero *)D_0013F450_20C568)->motion.rot.f[2],
              fast_add_rotations(((Hero *)D_0013F450_20C568)->ledge_normal_yaw, 3.1415927f)) <
          1.1344640f))
        return;
    {
        float max = 2.4f;
        float step = 0.03f;

        for (offset = 0.0f; offset < max; offset += step) {
            qcopy(edge_end, ledge);
            edge_end[2] = ledge[2];
            edge_end[0] += fast_cos(((Hero *)D_0013F450_20C568)->ledge_normal_yaw) * offset;
            edge_end[1] += fast_sin(((Hero *)D_0013F450_20C568)->ledge_normal_yaw) * offset;
            qcopy(edge_start, edge_end);
            edge_end[2] += 0.5f;
            edge_start[2] -= 0.5f;
            if (!func_L00_001EFFF0_20C568(
                    edge_end, edge_start, 4, (int)((Hero *)D_0013F450_20C568)->moby, 0)) {
                qcopy(grab_point, edge_end);
                offset = step * 0.5f;
                offset = -offset;
                grab_point[2] = ledge[2];
                grab_point[0] += fast_cos(((Hero *)D_0013F450_20C568)->ledge_normal_yaw) * offset;
                grab_point[1] += fast_sin(((Hero *)D_0013F450_20C568)->ledge_normal_yaw) * offset;
                qcopy(((Hero *)D_0013F450_20C568)->ledge_hang_pos.f, grab_point);
                ((Hero *)D_0013F450_20C568)->ledge_hang_pos.f[0] +=
                    fast_cos(((Hero *)D_0013F450_20C568)->ledge_normal_yaw) * back;
                ((Hero *)D_0013F450_20C568)->ledge_hang_pos.f[1] +=
                    fast_sin(((Hero *)D_0013F450_20C568)->ledge_normal_yaw) * back;
                ((Hero *)D_0013F450_20C568)->ledge_found = 1;
                ((Hero *)D_0013F450_20C568)->ledge_hang_pos.f[2] += drop;
                return;
            }
        }
    }
}

s16 D_L00_0017A722[1] __attribute__((section(".data"))) = {0};

f32 D_L00_0017A724[1] __attribute__((section(".data"))) = {0};

f32 D_L00_0017A728[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001798CC[1] __attribute__((section(".data"))) = {0};

s16 D_L01_0017ABA2[1] __attribute__((section(".data"))) = {0};

f32 D_L01_0017ABA4[1] __attribute__((section(".data"))) = {0};

f32 D_L01_0017ABA8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_00179A4C[1] __attribute__((section(".data"))) = {0};

f32 D_L02_0017AD24[1] __attribute__((section(".data"))) = {0};

f32 D_L02_0017AD28[1] __attribute__((section(".data"))) = {0};

s32 D_L03_0017954C[1] __attribute__((section(".data"))) = {0};

s16 D_L03_0017A822[1] __attribute__((section(".data"))) = {0};

f32 D_L03_0017A824[1] __attribute__((section(".data"))) = {0};

f32 D_L03_0017A828[1] __attribute__((section(".data"))) = {0};

s16 D_L04_0017A8A2[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0017A8A4[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0017A8A8[1] __attribute__((section(".data"))) = {0};

s32 D_L05_0017984C[1] __attribute__((section(".data"))) = {0};

s16 D_L05_0017AB82[1] __attribute__((section(".data"))) = {0};

f32 D_L05_0017AB84[1] __attribute__((section(".data"))) = {0};

f32 D_L05_0017AB88[1] __attribute__((section(".data"))) = {0};

s16 D_L06_0017AEA2[1] __attribute__((section(".data"))) = {0};

f32 D_L06_0017AEA4[1] __attribute__((section(".data"))) = {0};

f32 D_L06_0017AEA8[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001794CC[1] __attribute__((section(".data"))) = {0};

s16 D_L07_0017A7A2[1] __attribute__((section(".data"))) = {0};

f32 D_L07_0017A7A4[1] __attribute__((section(".data"))) = {0};

f32 D_L07_0017A7A8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_00179C4C[1] __attribute__((section(".data"))) = {0};

f32 D_L08_0017AF24[1] __attribute__((section(".data"))) = {0};

f32 D_L08_0017AF28[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001795CC[1] __attribute__((section(".data"))) = {0};

s16 D_L09_0017A8A2[1] __attribute__((section(".data"))) = {0};

f32 D_L09_0017A8A4[1] __attribute__((section(".data"))) = {0};

f32 D_L09_0017A8A8[1] __attribute__((section(".data"))) = {0};

s16 D_L10_0017ABA2[1] __attribute__((section(".data"))) = {0};

f32 D_L10_0017ABA4[1] __attribute__((section(".data"))) = {0};

f32 D_L10_0017ABA8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_00179E4C[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001798CC[1] __attribute__((section(".data"))) = {0};

s16 D_L12_0017ABA2[1] __attribute__((section(".data"))) = {0};

f32 D_L12_0017ABA4[1] __attribute__((section(".data"))) = {0};

f32 D_L12_0017ABA8[1] __attribute__((section(".data"))) = {0};

f32 D_L13_0017AA24[1] __attribute__((section(".data"))) = {0};

f32 D_L13_0017AA28[1] __attribute__((section(".data"))) = {0};

s32 D_L14_00179B4C[1] __attribute__((section(".data"))) = {0};

f32 D_L14_0017AE24[1] __attribute__((section(".data"))) = {0};

f32 D_L14_0017AE28[1] __attribute__((section(".data"))) = {0};

f32 D_L15_0017AD24[1] __attribute__((section(".data"))) = {0};

f32 D_L15_0017AD28[1] __attribute__((section(".data"))) = {0};

s32 D_L16_0017984C[1] __attribute__((section(".data"))) = {0};

s16 D_L16_0017AB82[1] __attribute__((section(".data"))) = {0};

f32 D_L16_0017AB84[1] __attribute__((section(".data"))) = {0};

f32 D_L16_0017AB88[1] __attribute__((section(".data"))) = {0};

f32 D_L17_0017B024[1] __attribute__((section(".data"))) = {0};

f32 D_L17_0017B028[1] __attribute__((section(".data"))) = {0};

s32                  D_L01_0015F6F8 = 2;
char D_L01_00180650  NOT_SDA = 0;
char D_L01_0017AB40  NOT_SDA = 0;
float D_L01_00179C58 NOT_SDA = 0;
int                  D_L01_0015F680 = 0;
int                  D_L01_0015F684 = 0;
float                D_L01_0015F67C = 0.1f;
V_208b60             D_L01_001AF010 = {0};

T00209240 D_L01_00208C70 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

s32                  D_L02_0015F6F8 = 2;
char D_L02_001807D0  NOT_SDA = 0;
char D_L02_0017ACC0  NOT_SDA = 0;
float D_L02_00179DD8 NOT_SDA = 0;
int                  D_L02_0015F680 = 0;
int                  D_L02_0015F684 = 0;
float                D_L02_0015F67C = 0.1f;
V_208b60             D_L02_001AF190 = {0};

T00209240 D_L02_001F8510 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

s32                  D_L03_0015F6F8 = 2;
char D_L03_001802D0  NOT_SDA = 0;
char D_L03_0017A7C0  NOT_SDA = 0;
float D_L03_001798D8 NOT_SDA = 0;
int                  D_L03_0015F680 = 0;
int                  D_L03_0015F684 = 0;
float                D_L03_0015F67C = 0.1f;
V_208b60             D_L03_001AEC90 = {0};

T00209240 D_L03_001E0890 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

s32                  D_L04_0015F6F8 = 2;
f32 D_L04_0017B9DC   NOT_SDA = 1.0f;
char D_L04_00180350  NOT_SDA = 0;
char D_L04_0017A840  NOT_SDA = 0;
float D_L04_00179958 NOT_SDA = 0;
int                  D_L04_0015F680 = 0;
int                  D_L04_0015F684 = 0;
float                D_L04_0015F67C = 0.1f;
V_208b60             D_L04_001AED10 = {0};

T00209240 D_L04_001D9A00 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

S0020a1e0 D_L04_0017A800 = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0},
    0.0f,
    0.0f
};

char D_L05_00179810  NOT_SDA = 0;
s32                  D_L05_0015F708 = 2;
char D_L05_00180650  NOT_SDA = 0;
float D_L05_00179C38 NOT_SDA = 0;
V_208b60             D_L05_001AF010 = {0};

T00209240 D_L05_00216300 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

s32                  D_L06_0015F6F8 = 2;
char D_L06_00180950  NOT_SDA = 0;
char D_L06_0017AE40  NOT_SDA = 0;
float D_L06_00179F58 NOT_SDA = 0;
int                  D_L06_0015F680 = 0;
int                  D_L06_0015F684 = 0;
float                D_L06_0015F67C = 0.1f;
V_208b60             D_L06_001AF310 = {0};

T00209240 D_L06_001FF060 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

char D_L07_00179490  NOT_SDA = 0;
s32                  D_L07_0015F6F8 = 2;
f32 D_L07_0017B8DC   NOT_SDA = 1.0f;
char D_L07_00180250  NOT_SDA = 0;
char D_L07_0017A740  NOT_SDA = 0;
float D_L07_00179858 NOT_SDA = 0;
int                  D_L07_0015F680 = 0;
int                  D_L07_0015F684 = 0;
float                D_L07_0015F67C = 0.1f;
V_208b60             D_L07_001AEC10 = {0};

T00209240 D_L07_0020EF00 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

S0020a1e0 D_L07_0017A700 = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0},
    0.0f,
    0.0f
};

s32                  D_L08_0015F6F8 = 2;
char D_L08_001809D0  NOT_SDA = 0;
char D_L08_0017AEC0  NOT_SDA = 0;
float D_L08_00179FD8 NOT_SDA = 0;
int                  D_L08_0015F680 = 0;
int                  D_L08_0015F684 = 0;
float                D_L08_0015F67C = 0.1f;
V_208b60             D_L08_001AF390 = {0};

T00209240 D_L08_001F9B50 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

char D_L09_00179590  NOT_SDA = 0;
s32                  D_L09_0015F6F8 = 2;
f32 D_L09_0017B9DC   NOT_SDA = 1.0f;
char D_L09_00180350  NOT_SDA = 0;
char D_L09_0017A840  NOT_SDA = 0;
float D_L09_00179958 NOT_SDA = 0;
int                  D_L09_0015F680 = 0;
int                  D_L09_0015F684 = 0;
float                D_L09_0015F67C = 0.1f;
V_208b60             D_L09_001AED10 = {0};

T00209240 D_L09_00206770 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

S0020a1e0 D_L09_0017A800 = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0},
    0.0f,
    0.0f
};

s32                  D_L10_0015F6F8 = 2;
f32 D_L10_0017BCDC   NOT_SDA = 1.0f;
char D_L10_00180650  NOT_SDA = 0;
char D_L10_0017AB40  NOT_SDA = 0;
float D_L10_00179C58 NOT_SDA = 0;
int                  D_L10_0015F680 = 0;
int                  D_L10_0015F684 = 0;
float                D_L10_0015F67C = 0.1f;
V_208b60             D_L10_001AF010 = {0};

T00209240 D_L10_001DA9E0 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

S0020a1e0 D_L10_0017AB00 = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0},
    0.0f,
    0.0f
};

s32                  D_L11_0015F6F8 = 2;
char D_L11_00180BD0  NOT_SDA = 0;
char D_L11_0017B0C0  NOT_SDA = 0;
float D_L11_0017A1D8 NOT_SDA = 0;
int                  D_L11_0015F680 = 0;
int                  D_L11_0015F684 = 0;
float                D_L11_0015F67C = 0.1f;
V_208b60             D_L11_001AF590 = {0};

T00209240 D_L11_00218D60 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

char D_L12_00179890  NOT_SDA = 0;
s32                  D_L12_0015F6F8 = 2;
char D_L12_00180650  NOT_SDA = 0;
char D_L12_0017AB40  NOT_SDA = 0;
float D_L12_00179C58 NOT_SDA = 0;
int                  D_L12_0015F680 = 0;
int                  D_L12_0015F684 = 0;
float                D_L12_0015F67C = 0.1f;
V_208b60             D_L12_001AF010 = {0};

T00209240 D_L12_0020A0F0 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

s32                  D_L13_0015F6F8 = 2;
f32 D_L13_0017BB5C   NOT_SDA = 1.0f;
char D_L13_001804D0  NOT_SDA = 0;
char D_L13_0017A9C0  NOT_SDA = 0;
float D_L13_00179AD8 NOT_SDA = 0;
int                  D_L13_0015F680 = 0;
int                  D_L13_0015F684 = 0;
float                D_L13_0015F67C = 0.1f;
V_208b60             D_L13_001AEE90 = {0};

T00209240 D_L13_001F26D0 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

S0020a1e0 D_L13_0017A980 = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0},
    0.0f,
    0.0f
};

char D_L14_00179B10  NOT_SDA = 0;
s32                  D_L14_0015F6F8 = 2;
char D_L14_001808D0  NOT_SDA = 0;
char D_L14_0017ADC0  NOT_SDA = 0;
float D_L14_00179ED8 NOT_SDA = 0;
int                  D_L14_0015F680 = 0;
int                  D_L14_0015F684 = 0;
float                D_L14_0015F67C = 0.1f;
V_208b60             D_L14_001AF290 = {0};

T00209240 D_L14_001F9F60 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

s32                  D_L15_0015F6F8 = 2;
char D_L15_001807D0  NOT_SDA = 0;
float D_L15_00179DD8 NOT_SDA = 0;
int                  D_L15_0015F680 = 0;
int                  D_L15_0015F684 = 0;
float                D_L15_0015F67C = 0.1f;
V_208b60             D_L15_001AF190 = {0};

T00209240 D_L15_001E07D0 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

char D_L16_00179810  NOT_SDA = 0;
s32                  D_L16_0015F708 = 2;
char D_L16_00180650  NOT_SDA = 0;
char D_L16_0017AB20  NOT_SDA = 0;
float D_L16_00179C38 NOT_SDA = 0;
int                  D_L16_0015F680 = 0;
int                  D_L16_0015F684 = 0;
float                D_L16_0015F67C = 0.1f;
V_208b60             D_L16_001AF010 = {0};

T00209240 D_L16_001E5EC0 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

s32                  D_L17_0015F6F8 = 2;
char D_L17_00180AD0  NOT_SDA = 0;
char D_L17_0017AFC0  NOT_SDA = 0;
float D_L17_0017A0D8 NOT_SDA = 0;
int                  D_L17_0015F680 = 0;
int                  D_L17_0015F684 = 0;
float                D_L17_0015F67C = 0.1f;
V_208b60             D_L17_001AF490 = {0};

T00209240 D_L17_001E4620 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

s32                  D_L18_0015F6F8 = 2;
char D_L18_00180BD0  NOT_SDA = 0;
float D_L18_0017A1D8 NOT_SDA = 0;
int                  D_L18_0015F680 = 0;
int                  D_L18_0015F684 = 0;
float                D_L18_0015F67C = 0.1f;
V_208b60             D_L18_001AF590 = {0};

T00209240 D_L18_001F0340 = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};

V_208b60 l00_hero_height_probe_direction __asm__("D_L00_001AEB90") = {0};

T00209240 l00_hero_world_matrix_selection_indices __asm__("D_L00_001E7C10") = {
    {0, 1, 2, 3, 4, 5, 6, 0x1d, 0x1e}
};
